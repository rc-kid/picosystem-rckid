#include <math.h>
#include <string.h>

#include "hardware/adc.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include "hardware/pwm.h"
#include "hardware/pio.h"
#include "hardware/irq.h"
#include "hardware/vreg.h"

#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#include "picosystem.hpp"

/** PCKid Picosystem hardware compatibility layer. 
 
 */

namespace picosystem {

  PIO               screen_pio  = pio0;
  uint              screen_sm   = 0;
  uint32_t          dma_channel;
  volatile int16_t  dma_scanline = -1;

  uint32_t         _audio_pwm_wrap = 5000;
  struct repeating_timer _audio_update_timer;

  bool _in_flip = false;
  
  // initialization ---------------------------------------------------------------------

  void _init_hardware() {
    

  }

  // general IO (AVR comms) -------------------------------------------------------------

  float _battery_voltage() {
    // TODO read state info accordingly
  }

  uint32_t battery() {
    // TODO read state info accordingly
  }

  uint32_t _gpio_get() {
    // construct the io (pins & friends as if it were obtained by picoystem)
  }

  void _reset_to_dfu() {
    // TODO send avr command
  }

  void led(uint8_t r, uint8_t g, uint8_t b) {
    // TODO send avr command
  }

  void backlight(uint8_t b) {
    // TODO send avr command
  }

  // display control --------------------------------------------------------------------
  
  void _wait_vsync() {
    // TODO code from ST8879 in RCKid
  }

  void _flip() {

  }


  // audio ------------------------------------------------------------------------------

  void _play_note(uint32_t f, uint32_t v) {
    // TODO frequency & volume
  }







  // the following functions are unchanged ----------------------------------------------

  bool pressed(uint32_t b) {
    return !(_io & (1U << b)) && (_lio & (1U << b));
  }

  bool button(uint32_t b) {
    return !(_io & (1U << b));
  }

  uint32_t time() {
    absolute_time_t t = get_absolute_time();
    return to_ms_since_boot(t);
  }

  uint32_t time_us() {
    absolute_time_t t = get_absolute_time();
    return to_us_since_boot(t);
  }

  void sleep(uint32_t d) {
    sleep_ms(d);
  }

  bool _audio_update_callback(struct repeating_timer *t) {
    _update_audio();
    return true;
  }

  void _start_audio() {
    add_repeating_timer_ms(-1, _audio_update_callback, NULL, &_audio_update_timer);
  }

  bool _is_flipping() {
    return _in_flip;
  }



} // namespace picosystem

