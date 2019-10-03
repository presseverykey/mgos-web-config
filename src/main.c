
#include "mgos.h"
#include "mgos_pwm.h"

enum mgos_app_init_result mgos_app_init(void) {
  LOG(LL_INFO, ("PWM demo starting..."));
  int pin = mgos_sys_config_get_ws_pwm_pin();
  int freq = mgos_sys_config_get_ws_pwm_freq();
  int duty_i = mgos_sys_config_get_ws_pwm_duty();
  float duty = (float)duty_i / 100.0;
  
  mgos_pwm_set(pin, freq, duty);
  return MGOS_APP_INIT_SUCCESS;
}
// vim: et:sw=2:ts=2

