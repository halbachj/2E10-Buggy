/**
 * @file Timer1.hpp
 * @brief Setup methods for Timer1
 **/
#ifndef TIMER1_HPP
#define TIMER1_HPP

#include "FspTimer.h"

namespace BuggyTimer1 {

  static FspTimer agt_timer;
  extern volatile uint32_t counter;

  static void ISR_Timer1(timer_callback_args_t __attribute((unused))* p_args);


  /**
   * @brief Start Timer1
   * @details configure AGT timer 0 to generate an underflow interrupt every 1 ms
	 * a clock divider 8 works for both the Uno R4 because number of clock ticks 
	 * in 1 ms (period) is an integer number and below the 16-bit counter limit
	 * on the Uno R4 the AGT clock is 24 MHz / 8 -> 3000 ticks per ms
   */
  void begin();
}



#endif //TIMER1_HPP
