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

public:
	Stepper(uint8_t dir_pin, uint8_t step_pin, uint16_t spr, uint32_t us_update = 700) :
		_dir(dir_pin),
		_step(step_pin),
		_spr(spr),
		_us_update(us_update),
		_prev_update(0),
		_steps(0),
		_desired_state(0.0) { }

	void begin() {
		pinMode(_step.pin, OUTPUT);
		pinMode(_dir.pin, OUTPUT);
	}

	void update() {
		uint32_t time = micros();
		if(abs(time - _prev_update) >= _us_update) {
			_prev_update = time;

			float err = _desired_state - get_state();
      if(err > ((float) _spr / 360)) {
        _dir.out = 1;
        _step.out = !_step.out;
        ++_steps;
      } else if (err < -((float) _spr / 360)) {
        _dir.out = 0;
        _step.out = !_step.out;
        --_steps;
      }
//      Serial.println(err);
      digitalWrite(_dir.pin, _dir.out);
      digitalWrite(_step.pin, _step.out);
		}
	}

	float get_state() {
		return ((float) _steps) * 180.0 / _spr; // Steps needs to be divided by 2
	}

	void set_desired_state(float desired_state) {
		_desired_state = desired_state;
	}

	void set_us_update(uint32_t us_update){
		_us_update = us_update;
	}

	void set_speed(float revolutions_per_sec) {
	 	_us_update = 1000000.0 / (_spr * revolutions_per_sec * 2);
	}
};

#endif
