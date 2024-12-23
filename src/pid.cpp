#ifndef _PID_SOURCE_
#define _PID_SOURCE_

#include "pb_omni_pid_pursuit_controller/pid.hpp"

#include <cmath>
#include <iostream>

using namespace std;

class PIDImpl
{
public:
  PIDImpl(double dt, double max, double min, double Kp, double Kd, double Ki);
  ~PIDImpl();
  double calculate(double setpoint, double pv);
  void set_sum_error(double sum_error);

private:
  double _dt;
  double _max;
  double _min;
  double _Kp;
  double _Kd;
  double _Ki;
  double _pre_error;
  double _integral;
};

PID::PID(double dt, double max, double min, double Kp, double Kd, double Ki)
{
  pimpl = new PIDImpl(dt, max, min, Kp, Kd, Ki);
}
double PID::calculate(double setpoint, double pv) { return pimpl->calculate(setpoint, pv); }
void PID::setsumerror(double sum_error) { pimpl->set_sum_error(sum_error); }
PID::~PID() { delete pimpl; }

/**
 * Implementation
 */
PIDImpl::PIDImpl(double dt, double max, double min, double Kp, double Kd, double Ki)
: _dt(dt), _max(max), _min(min), _Kp(Kp), _Kd(Kd), _Ki(Ki), _pre_error(0), _integral(0)
{
}

double PIDImpl::calculate(double setpoint, double pv)
{
  // Calculate error
  double error = setpoint - pv;

  // Proportional term
  double Pout = _Kp * error;

  // Integral term
  _integral += error * _dt;
  double Iout = _Ki * _integral;

  if (_integral > 1) {
    _integral = 1;
  } else if (_integral < -1) {
    _integral = -1;
  }

  // Derivative term
  double derivative = (error - _pre_error) / _dt;
  double Dout = _Kd * derivative;

  // Calculate total output
  double output = Pout + Iout + Dout;

  // Restrict to max/min
  if (output > _max)
    output = _max;
  else if (output < _min)
    output = _min;

  // Save error to previous error
  _pre_error = error;

  return output;
}
void PIDImpl::set_sum_error(double sum_error)
{
  _integral = sum_error;
  return;
}

PIDImpl::~PIDImpl() {}

#endif