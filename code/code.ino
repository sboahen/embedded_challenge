
#include <Adafruit_CircuitPlayground.h>

float X, Y, Z;
long numReps =10; // By default number of reps is 10
long currCount = 0;
volatile bool lightOn = false;
volatile long msElapsed;

/*
 * States:
 * State 0:
 *    pushup resting position (Chest Up) or Jumping Jack resting position (Hands down) 
 *    
 * State 1:
 *    Situp resting position (lying down) 
 * 
 * State 2:
 *    pushup down
 * 
 * State 3:
 *    Jumping Jack (hands up) or situp Up 
 *
 *State 4:
 *    Squat down
 *    
 *    
 * * * * * * * * * * Possible transitions * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Only the transitions below are valid. For example, you cannot transition from pushup up state
 * to pushup up state, preventing a double count 
 * 
 * State 0 -> State 2 (Pushup - went down)
 * State 2 -> State 0 (Pushup - went up)
 * 
 * State 0 -> State 3 (Jumping jack went up)
 * State 3 -> State 0 (Went to JJ resiting position)
 * 
 * State 1 -> State 3 (Situp)
 * State 3 -> State 1 (back to rest mode)
 * 
 * State 3 -> State 4 (Squat down)
 * State 4 -> State 2 (Squat went up)
 * 
 * NB: State 0, and 1 are possible start points
 */
unsigned char state;


void setup() {

  CircuitPlayground.begin(); /////////////////////////////////remove as compl.

  //Setup 
  cli();
  TCCR1A = 0b00000010;
  //               -- Counter OCR1A Top
  TCCR1B = 0b00000011;
  //             - WGM bit
  //              --- 64 CPS, each tick is 4us, so 250 ticks = 1 ms
  OCR1A = 250;
  TIMSK1 = 0b00000010; //setting this shuts off board for some reason
  
  DDRD &= ~(1 << 4); // Left button - setup to input
  DDRF &= ~(1<< 6); // Right button

  Serial.println("Get into starting position");
  delay(5000); // 5 second wait

  sei();

  long tempRep = 0;
  delay(100);
  while (!((PIND & (1 << 4)) && (PINF & (1 << 6)))) { // both buttons are pressed to start
    Serial.println("In here");

    if (PIND & (1 << 4)) { tempRep++; }

    if (PINF & (1 << 6)) { tempRep += 10; }
    delay(200); // debounce
  }

  if (tempRep > 0) { numReps = tempRep; } 
  
  Serial.begin(9600);

}


ISR(TIMER1_COMPA_vect)
{
  if (msElapsed > 250){
    CircuitPlayground.clearPixels();
    msElapsed = 0;
    lightOn = false;
  }
  if (lightOn) { msElapsed++; }
}



void loop() {

  if (currCount == numReps) { 

    light_up();
  }

  else {

    X = CircuitPlayground.motionX();
    Y = CircuitPlayground.motionY();
    Z = CircuitPlayground.motionZ();
  
    switch(state){
    
      case 0: 
  
        if (is_pushup_down()){
          light_up();
          Serial.println("Pushup Down");
          state = 2; 
        }
  
        else if (is_jj_ss_up()) {
          light_up();
          Serial.println("Jumping Jack UP or Situp Up");
          state = 3;
        }
        
        break;
  
  
      case 1:
  
        if (is_jj_ss_up()) {
          light_up();
          Serial.println("Jumping Jack UP or Situp Up");
          state = 3;
        }
  
        break;
  
  
      case 2:
  
        if (is_pushup_up_jj_down()){
          light_up();
          Serial.println("Pushup Up Jumping Jack Down");
          state = 0;
          currCount++; // from state 2 to 0 is a rep
        }
  
        break;
  
  
      case 3:
  
        if (!is_jj_ss_up && is_situp_down){
          light_up();
          Serial.println("Situp Down");
          state = 1;
          currCount++;
        }
  
        else if (is_pushup_up_jj_down()){
          light_up();
          Serial.println("Pushup Up Jumping Jack Down");
          state = 0;
          currCount++;
        }

        else if (is_squat_down()) {
          light_up();
          Serial.println("Squat Down");
          state = 4;
        }
        break;

      case 4:

        if (is_jj_ss_up()) {
          light_up();
          Serial.println("Jumping Jack UP or Situp Up");
          state = 3;
          currCount++;
        }
        break;
    }

    delay(500); // allows for mechanical debouncing
  }
  
  

}


void light_up() {

  CircuitPlayground.setPixelColor(0, 255,   0,   0);
  CircuitPlayground.setPixelColor(1, 128, 128,   0);
  CircuitPlayground.setPixelColor(2,   0, 255,   0);
  CircuitPlayground.setPixelColor(3,   0, 128, 128);
  CircuitPlayground.setPixelColor(4,   0,   0, 255);
  
  CircuitPlayground.setPixelColor(5, 0xFF0000);
  CircuitPlayground.setPixelColor(6, 0x808000);
  CircuitPlayground.setPixelColor(7, 0x00FF00);
  CircuitPlayground.setPixelColor(8, 0x008080);
  CircuitPlayground.setPixelColor(9, 0x0000FF);

  lightOn = true;
 
  //delay(delayTime);
  //CircuitPlayground.clearPixels();
}


// Exercise states
bool is_pushup_down() {
  return ((X < -5) & (Y > 3));
}

bool is_pushup_up_jj_down() {
  return ((X < -5) & (Y < 3));
}

bool is_jj_ss_up() {
  return (X > 5) && (Z < 5);
}

bool is_situp_down() {
  return ((Z < -7.8) & (Y > -4) & (Y < 2));
}

bool is_squat_down() {
  return ((Z > 5) && (X <8.6) && (X > 6));
}
