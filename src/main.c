
#include "mgos.h"
#include "mgos_pwm.h"
#include "mgos_timers.h"
#include "mgos_bt.h"
#include "mgos_bt_gatts.h"

static enum mgos_bt_gatt_status mgos_bt_freq_svc_ev(
    struct mgos_bt_gatts_conn *c, 
    enum mgos_bt_gatts_ev ev, 
    void *ev_arg,
    void *handler_arg) {

  switch(ev) {
    case MGOS_BT_GATTS_EV_CONNECT:
    case MGOS_BT_GATTS_EV_DISCONNECT:
      LOG(LL_INFO, ("Connection Event!"));
      break;
    case MGOS_BT_GATTS_EV_READ: 
      {
        struct mgos_bt_gatts_read_arg *ra =
          (struct mgos_bt_gatts_read_arg *) ev_arg;

        if (ra->offset != 0) {
          return MGOS_BT_GATT_STATUS_INVALID_OFFSET;
        }

        int freq_r = mgos_sys_config_get_ws_pwm_freq();
        mgos_bt_gatts_send_resp_data(
            c, 
            ra,
            mg_mk_str_n((char*)&freq_r, sizeof(freq_r))
            );
        break;
      }
    case MGOS_BT_GATTS_EV_WRITE:
      {
        struct mgos_bt_gatts_write_arg *wa = 
          (struct mgos_bt_gatts_write_arg *) ev_arg;
        int freq_w = *(wa->data.p);
        LOG(LL_INFO, ("f: %d", freq_w));
        mgos_sys_config_set_ws_pwm_freq(freq_w);
        break;
      }
    default:
      return MGOS_BT_GATT_STATUS_REQUEST_NOT_SUPPORTED;
  }
  return MGOS_BT_GATT_STATUS_OK;

}
static const struct mgos_bt_gatts_char_def s_freq_svc_def[] = {
    {
     .uuid = "7256e521-335a-44c0-82f3-adf4e9b3b1fb", /* current time */
     .prop = MGOS_BT_GATT_PROP_RWNI(1,1, 0, 0),
    },
    {.uuid = NULL},
};

void initialize_bt() {
  mgos_bt_gatts_register_service(
      "c827622b-f151-4c6f-8724-294ea4aacfaf", 
      MGOS_BT_GATT_SEC_LEVEL_NONE,
      s_freq_svc_def, 
      mgos_bt_freq_svc_ev, 
      NULL);
}

static void timer_cb(void *arg){
  int pin = mgos_sys_config_get_ws_pwm_pin();
  int freq = mgos_sys_config_get_ws_pwm_freq();
  int duty_i = mgos_sys_config_get_ws_pwm_duty();
  float duty = (float)duty_i / 100.0;
  
  mgos_pwm_set(pin, freq, duty);
  (void) arg;
}

enum mgos_app_init_result mgos_app_init(void) {
  LOG(LL_INFO, ("PWM demo starting..."));
  mgos_set_timer(1000, MGOS_TIMER_REPEAT, timer_cb, NULL);
  initialize_bt();

  return MGOS_APP_INIT_SUCCESS;
}
// vim: et:sw=2:ts=2

