
#include <Adafruit_CircuitPlayground.h>

float X, Y, Z;

bool stateArr[4];

void setup() {
  Serial.begin(9600);

  CircuitPlayground.begin(); /////////////////////////////////remove as compl.
  
  DDRD &= ~(1 << 4); // Left button - setup to input
  DDRF &= ~(1<< 6); // Right button
  

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

  if (is_pushup_down()){
    light_up(500);
    Serial.println("Pushup Down");
  }

  else if (is_pushup_up()){
    light_up(500);
    Serial.println("Pushup Up");
  }
  
  else if (is_jj_ss_up()) {

    light_up(500);
    Serial.println("Jumping Jack UP or Situp Up");
  }

  else if (is_jj_down()){
    light_up(500);
    Serial.println("Jumping Jack Down");
  }

  else if (is_situp_down){
    light_up(500);
    Serial.println("Situp Down");
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
  return ((X < -5) & (Y > 3.5));
}

bool is_pushup_up() {
  return ((X < -5) & (Y < 3));
}

bool is_jj_ss_up() {
  return (X > 5);
}

bool is_jj_down() {
  return (X < -5);
}

bool is_situp_down() {
  return ((Z < -7.8) & (Y > -4) & (Y < 2));
}
