#include "Stepper.h"
#include <string>

#define RADIUS_SCALE 180
#define RADIUS_LEN 1700

Stepper step_angle(13, 14, 400);
Stepper step_radius(20, 21, 400);

void setup() {
  Serial.begin(115200);
  step_angle.begin();
  step_radius.begin();
  pinMode(13, OUTPUT);
  delay(2000);
  
  step_angle.set_speed(0.05);
  step_angle.set_min_max(-360, 360);
  
  step_radius.set_speed(2);
  step_radius.set_min_max(0, RADIUS_LEN);
  step_radius.set_reversed(false);
}

void loop() {
  int8_t sr = step_radius.update();
  int8_t sa = step_angle.update();

  // Always overwrite next_target when receiving data.
  // Receive data as "x y", both floats in string
  if(Serial.available()){
    uint8_t i = 0;
    // Probably overkill, 100 byte buffer for read.
    char des_state[100] = {0};
    while(Serial.available()){
      des_state[i++] = Serial.read();
    }
    char* y_loc = NULL;
    float x = strtof(des_state, &y_loc);
    float y = strtof(y_loc, NULL);

    float r = sqrt(x*x + y*y) * RADIUS_LEN / RADIUS_SCALE;
    float theta = atan2(y, x) * 180 / PI;

    Serial.print("r: ");
    Serial.println(r);
    Serial.print("t: ");
    Serial.println(theta);
    
    step_radius.set_desired_state(r);
    step_angle.set_desired_state(theta);
  }
//  step_angle.set_desired_state(90);
}
