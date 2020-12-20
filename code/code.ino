
#include <Adafruit_CircuitPlayground.h>

float X, Y, Z;

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
 * NB: State 0, and 1 are possible start points
 */
unsigned char state;


void setup() {
  Serial.begin(9600);

  CircuitPlayground.begin(); /////////////////////////////////remove as compl.
  
  DDRD &= ~(1 << 4); // Left button - setup to input
  DDRF &= ~(1<< 6); // Right button

  Serial.println("Get into starting position");
  delay(5000); // 5 second wait

  /*
  if (is_pushup_up_jj_down()) { 
    stateArr = 0; 
  }

  else {
    stateArr = 1;  
  }
  */

}


void loop() {

  
  //.println(PIND & (1 << 4)); // Left button - high when pressed
  //Serial.println(PINF & (1 <<6)); // Right button 

  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();

  /*
  Serial.print("X: ");
  Serial.print(X);
  Serial.print("  Y: ");
  Serial.print(Y);
  Serial.print("  Z: ");
  Serial.println(Z);
  */

  /*
  if (is_pushup_down()){
    light_up(500);
    Serial.println("Pushup Down");
  }
  
  else if (is_jj_ss_up()) {

    light_up(500);
    Serial.println("Jumping Jack UP or Situp Up");
  }

  else if (is_pushup_up_jj_down()){
    light_up(500);
    Serial.println("Pushup Up Jumping Jack Down");
  }

  else if (is_situp_down){
    light_up(500);
    Serial.println("Situp Down");
  }
  
  */

  switch(state){
  
    case 0: 

      if (is_pushup_down()){
        light_up(500);
        Serial.println("Pushup Down");
        state = 2; 
      }

      else if (is_jj_ss_up()) {
        light_up(500);
        Serial.println("Jumping Jack UP or Situp Up");
        state = 3;
      }
      
      break;


    case 1:

      if (is_jj_ss_up()) {
        light_up(500);
        Serial.println("Jumping Jack UP or Situp Up");
        state = 3;
      }

      break;


    case 2:

      if (is_pushup_up_jj_down()){
        light_up(500);
        Serial.println("Pushup Up Jumping Jack Down");
        state = 0;
      }

      break;


    case 3:

      if (!is_jj_ss_up && is_situp_down){
        light_up(500);
        Serial.println("Situp Down");
        state = 1;
      }

      else if (is_pushup_up_jj_down()){
        light_up(500);
        Serial.println("Pushup Up Jumping Jack Down");
        state = 0;
      }
      break;

  }
  
  
  delay(50);

}


void light_up(int delayTime) {

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
 
  delay(delayTime);
  CircuitPlayground.clearPixels();
}


// Exercise states
bool is_pushup_down() {
  return ((X < -5) & (Y > 3));
}

bool is_pushup_up_jj_down() {
  return ((X < -5) & (Y < 3));
}

bool is_jj_ss_up() {
  return (X > 5);
}

bool is_situp_down() {
  return ((Z < -7.8) & (Y > -4) & (Y < 2));
}
