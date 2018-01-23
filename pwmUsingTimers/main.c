/*
 *  IN THIS EXAMPLE:
 *      WE USE THE TIMER MODULE AND PORTF TO PRODUCE PWM OUTPUT
 *
 *      Sarea Al Hariri
 *
 *
 *  NOTE:
 *      till now i do NOT know how to choose the timer1 A or B
 *      when configure the timer but A does NOT work.
 *      when configuring the B half it works well

 */
#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"
#include "inc/hw_types.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/pwm.h"
#include "driverlib/ssi.h"
#include "driverlib/systick.h"
#include <string.h>
#include <SysTick_Init.h>



int main(){

    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ) ;

    // enable clocking on PORTF
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF) ;
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) ;    // wait for ready peripheral

    // PF3 configurations
    GPIOPinConfigure(GPIO_PF3_T1CCP1) ;
    GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_3) ;


    // enable clocking on timer1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1) ;
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)) ;
/*
    // timer1 configurations
    TimerDisable(TIMER1_BASE, TIMER_A) ;
    TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM) ;
*/
    // define no of counts for a pwm certain frequency and the dutyCycle
    uint32_t u32Counts = SysCtlClockGet() / 100000 ; // that is the pwm signal is 100 khz
    uint32_t duty =  2 ;                  // start with 50 % duty cycle
/*
    // timer match and load value
    TimerLoadSet(TIMER1_BASE, TIMER_A, u32Counts) ;
    TimerMatchSet(TIMER1_BASE, TIMER_A, duty) ;
*/

      TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_B_PWM);
      TimerLoadSet(TIMER1_BASE, TIMER_B, u32Counts -1);
      TimerMatchSet(TIMER1_BASE, TIMER_B, duty);

    // enable timer
    TimerEnable(TIMER1_BASE, TIMER_B) ;

    uint32_t i ; // i is used in loops
    while (1){
        for(i = 0; i < u32Counts; i++ ){
            TimerMatchSet(TIMER1_BASE, TIMER_B, i) ;
            SysTick80_Delay_10ms(1) ;
                        }

        for(i = u32Counts; i > 0 ; i-- ){
            TimerMatchSet(TIMER1_BASE, TIMER_B, i) ;
            SysTick80_Delay_10ms(1) ;
        }

    }


}
