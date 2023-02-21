/*
  StepperMotor_h - Library to control a stepper motor.
  Created by ClayXrex, February 14, 2023.
  Released into the public domain.
*/
#ifndef StepperMotor_h
#define StepperMotor_h

#include "Arduino.h"

class StepperMotor
{
  private:
    int m_enable_pin;
    int m_direction_pin;
    int m_pulse_pin;
    int m_max_rpm;
    bool m_hold_position;
  public:
    int m_steps_per_revolution;
    int m_steps_from_home;
    bool m_is_home_position_set;
    StepperMotor(int enable_pin, int direction_pin, int pulse_pin, int steps_per_revolution, int max_rpm, bool hold_position);
    void set_pin_modes();
    void update_position(bool turn_clockwise);
    long calculate_delay_from_rpm(int rpm);
    void step(int steps, bool turn_clockwise, int rpm);
    void do_one_rotation(bool turn_clockwise, int rpm);
    void rotate(int rotations, bool turn_clockwise, int rpm);
    void set_current_position_as_home();
    void go_to_position(bool turn_clockwise, int rpm, int steps_relative_to_home);
};

#endif
