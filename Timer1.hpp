/**
 * @file Timer1.hpp
 * @brief Setup methods for Timer1
 **/
#ifndef TIMER1_HPP
#define TIMER1_HPP

namespace Timer1 {

  static FspTimer   agt_timer;
  volatile uint32_t agt_time_ms = 0;

  static void ISR_Timer1(timer_callback_args_t __attribute((unused))* p_args) {
	  agt_time_ms += 1;
  }


  /**
   * @brief Start Timer1
   * @details configure AGT timer 0 to generate an underflow interrupt every 1 ms
	 * a clock divider 8 works for both the Uno R4 because number of clock ticks 
	 * in 1 ms (period) is an integer number and below the 16-bit counter limit
	 * on the Uno R4 the AGT clock is 24 MHz / 8 -> 3000 ticks per ms
   */
  void begin() {
    // 
	  const uint32_t clock_freq_Hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB);
	  const uint32_t period = clock_freq_Hz / ((1 << TIMER_SOURCE_DIV_8) * 1000UL);
	  agt_timer.begin(/* mode */ TIMER_MODE_PERIODIC,
                    /* type */ AGT_TIMER,
                    /* channel */ 1,
                    period,
                    /* pulse */ 1,
                    TIMER_SOURCE_DIV_8,
                    ISR_Timer1);;
	  agt_timer.setup_overflow_irq(8);
	  agt_timer.open();
	  agt_timer.start(); // bug in R4 1.0.2: calling start() is not necessary: open() starts the counter already !?
  }

}



#endif //TIMER1_HPP