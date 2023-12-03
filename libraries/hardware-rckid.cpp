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

#include "rckid/common/config.h"
#include "rckid/ST7789.h"
#include "ST7789_rgba.pio.h"

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
  
  // general IO (AVR comms) -------------------------------------------------------------

  float _battery_voltage() {
    // TODO read state info accordingly
    return 3.8;
  }

  uint32_t battery() {
    // TODO read state info accordingly
    return 80;
  }

  uint32_t _gpio_get() {
    // construct the io (pins & friends as if it were obtained by picoystem)
    return 0;
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
    using namespace rckid;
    ST7789::waitVSync();
  }

void __isr dma_complete() {
    if(dma_channel_get_irq0_status(dma_channel)) {
      dma_channel_acknowledge_irq0(dma_channel); // clear irq flag
      _in_flip = false;
    }
  }


  void _flip() {
    using namespace rckid;
    if (!_is_flipping()) {
        _in_flip = true;
        uint32_t c = SCREEN->w * SCREEN->h;
        ST7789::updateContinuous(SCREEN->data, c);
        ST7789::waitUpdateDone();
        _in_flip = false;
        //dma_channel_transfer_from_buffer_now(dma_channel, SCREEN->data, c);
    }
  }


  // audio ------------------------------------------------------------------------------

  void _play_note(uint32_t f, uint32_t v) {
    // TODO frequency & volume
  }





// initialization ---------------------------------------------------------------------

  void _init_hardware() {
    using namespace rckid;
    ST7789::initialize();
    ST7789::setColorMode(ST7789::ColorMode::RGB666);
    ST7789::naturalRotation();
    ST7789::enterContinuousMode(120, 120);
    ST7789::loadPIODriver(ST7789_rgba_program, ST7789_rgba_program_init);
    ST7789::startPIODriver();
    // set IRQ handler
    dma_channel_set_irq0_enabled(0, true);
        
    irq_set_exclusive_handler(DMA_IRQ_0, dma_complete);
    irq_set_enabled(DMA_IRQ_0, true);

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

