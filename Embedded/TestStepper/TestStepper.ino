#include "Stepper.h"
#include <string>

Stepper step_angle(2, 3, 400, 700);

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  step_angle.begin();
  delay(2000);
//  step_angle.set_speed(4);
  step_angle.set_speed(0.2);
//  step_angle.set_speed(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  static int i = 0;
  static int state = 180;
  
  int8_t ret = step_angle.update();
  if(ret == 0) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  
  // Receive data as "x y", both floats
  if(Serial.available()){
    uint8_t i = 0;
    // Overkill, 100 byte buffer for read.
    char des_state[100] = {0};
    while(Serial.available()){
      des_state[i++] = Serial.read();
    }
    char* y_loc = NULL;
    float x = strtof(des_state, &y_loc);
    float y = strtof(y_loc, NULL);
    Serial.print("x: ");
    Serial.println(x);
    Serial.print("y: ");
    Serial.println(y);
    step_angle.set_desired_state((int)x);
  }
}
