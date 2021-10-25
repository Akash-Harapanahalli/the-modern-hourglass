#ifndef STEPPER_H
#define STEPPER_H

typedef struct digitaloutpin {
  uint8_t pin;
  bool out;
  digitaloutpin(uint8_t p, bool o = 0) : pin(p), out(o) { }
} dopin_t;

class Stepper {
private:
  dopin_t _step;
  dopin_t _dir;
  uint16_t _spr;
  uint32_t _us_update;
  uint32_t _prev_update;
  int32_t _steps;
  float _desired_state;
  float _min;
  float _max;
  bool _reversed;

public:
  Stepper(uint8_t dir_pin, uint8_t step_pin, uint16_t spr, uint32_t us_update = 700) :
    _dir(dir_pin),
    _step(step_pin),
    _spr(spr),
    _us_update(us_update),
    _prev_update(0),
    _steps(0),
    _desired_state(0.0),
    _min(-180.0),
    _max( 180.0) { }

  void begin() {
    pinMode(_step.pin, OUTPUT);
    pinMode(_dir.pin, OUTPUT);
  }

  int8_t update() {
    int8_t ret = 0;
    uint32_t time = micros();
    
    float err = _desired_state - get_state();
    
    if(err > ((float) _spr / 360)) {
      ret = 1;
    } else if (err < -((float) _spr / 360)) {
      ret = -1;
    }
    
    if(abs(time - _prev_update) >= _us_update) {
      _prev_update = time;
      if(ret == 1) {
        _dir.out = 1;
        _step.out = !_step.out;
      } else if (ret == -1) {
        _dir.out = 0;
        _step.out = !_step.out;
      }
      _steps += ret;
      if(!_reversed) digitalWrite(_dir.pin, _dir.out);
      else digitalWrite(_dir.pin, !_dir.out);
      digitalWrite(_step.pin, _step.out);
    }
    return ret;
  }

  float set_min_max(float min, float max){
    _min = min;
    _max = max;
  }

  float get_state() {
    return ((float) _steps) * 180.0 / _spr; // Steps needs to be divided by 2
  }

  void set_desired_state(float desired_state) {
    if(desired_state < _min) {
      _desired_state = _min;
    } else if (desired_state > _max) {
      _desired_state = _max;
    } else {
      _desired_state = desired_state;
    }
  }

  void set_us_update(uint32_t us_update){
    _us_update = us_update;
  }

  void set_speed(float revolutions_per_sec) {
    _us_update = 1000000.0 / (_spr * revolutions_per_sec * 2);
  }

  void set_reversed(bool reversed){
    _reversed = reversed;
  }
};

#endif
