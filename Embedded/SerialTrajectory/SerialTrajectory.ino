#include "Stepper.h"
#include <string>

#define RADIUS_SCALE 180
#define RADIUS_LEN 1700

Stepper step_angle(13, 14, 400);
Stepper step_radius(20, 21, 400);

typedef struct target {
  float r;
  float theta;
  target() : r(0.0), theta(0.0) { }
  target(float r_, float theta_): r(r_), theta(theta_) { }
} target_t;

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
  static target_t curr_target(0, 180);
  static target_t next_target(0, 180);
  static bool requested_new = false;

  step_radius.set_desired_state(curr_target.r);
  step_angle.set_desired_state(curr_target.theta);
  
  int8_t sr = step_radius.update();
  int8_t sa = step_angle.update();

  // If both step_angle and step_radius are idling, move to next_target and request new next_target
  if(sr == 0 && sa == 0 && !requested_new){
    Serial.write(1);
    curr_target = next_target;
    requested_new = true;
  } 
//  else if (sr == 0 && sa == 0) {  
//    digitalWrite(13, HIGH);
//  } else {
//    digitalWrite(13, LOW);
//  }

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

    next_target.r = sqrt(x*x + y*y) * RADIUS_LEN / RADIUS_SCALE;
    next_target.theta = atan2(y, x) * 180 / PI;

    requested_new = false;
  }
}
