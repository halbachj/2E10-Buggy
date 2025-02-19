#include "Timer1.hpp"


namespace BuggyTimer1 {

  volatile uint32_t counter = 0; 

  void begin() {
    
    //Setup to tick every 0.5 ms

	  const uint32_t clock_freq_Hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB);
	  const uint32_t period = clock_freq_Hz / ((1 << TIMER_SOURCE_DIV_16) * 1000UL);
	  
    agt_timer.begin(/* mode */ TIMER_MODE_PERIODIC,
                    /* type */ AGT_TIMER,
                    /* channel */ 1,
                    period,
                    /* pulse */ 1,
                    TIMER_SOURCE_DIV_8,
                    ISR_Timer1);
	  agt_timer.setup_overflow_irq(8);
	  agt_timer.open();
	  agt_timer.start();
    
  }


}