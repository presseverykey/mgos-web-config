
#include "mgos.h"
#include "mgos_pwm.h"

enum mgos_app_init_result mgos_app_init(void) {
  LOG(LL_INFO, ("PWM demo starting..."));
  int pin = 13;
  int freq = 550;
  float duty = (float)50 / 100.0;
  
  mgos_pwm_set(pin, freq, duty);
  return MGOS_APP_INIT_SUCCESS;
}
// vim: et:sw=2:ts=2

