#include "Stepper.h"
#include <string>

#define RADIUS_SCALE 180
#define RADIUS_LEN 1700

Stepper step_angle(13, 14, 400);
Stepper step_radius(20, 21, 400);

float r(float theta);

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
  step_angle.set_min_max(0, 720);
  step_angle.set_reversed(true);
  
  step_radius.set_speed(2);
  step_radius.set_min_max(0, RADIUS_LEN);
  step_radius.set_reversed(false);
}

void loop() {
  static float tt = 0;
  
  int8_t sr = step_radius.update();
  int8_t sa = step_angle.update();

  // Both step_angle and step_radius are idling
  if (sr == 0 && sa == 0) {
    digitalWrite(13, HIGH);
    if(tt < 720.0){
      tt += 360.0/400.0;
      step_angle.set_desired_state(tt);
      step_radius.set_desired_state(r(tt));
    }
  } else {
    digitalWrite(13, LOW);
  }
}

float spiral(float theta){
  return RADIUS_LEN*theta/720.0;
}

float limacon(float theta){
  
}

float petals(int n, float theta) {
  return (RADIUS_LEN/2)*(1 + cos(n*(theta-360/n)*PI/180.0));
}

float crazy(float theta) {
  return RADIUS_LEN*cos(64*theta*PI/180.0);
}

float cospcos2(float theta) {
  return (RADIUS_LEN/2)*(cos(theta*PI/180.0) + cos(2*theta*PI/180));
}

float r(float theta) {
  return spiral(theta);
}
