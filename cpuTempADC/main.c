/* IN THIS EXAMPLE:
 *
 * WE MEASURE THE INTERNAL TEMP SENSOR TO MEASURE THE CPU TEMPERATURE USING ADC0
 * AND SEND THE TEMP TO PC(SERIAL MONITOR).
 *
 * Sarea Al Hariri
 *
 */

#pragma diag_push
#pragma CHECK_MISRA("none")


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
#include <driverlib/adc.h>
#include <SysTick_Init.h>
#include <InitConsole.h>
#include <uartstdio.h>

#pragma diag_pop

void ADC_Init(void) ;
int main(void) ;

int main(void){

	  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	  InitConsole() ;
	  ADC_Init() ;

	  uint32_t adcValue ;

	  /*** trigger the adc .. wait .. read the result ***/
	  while(1){
	        ADCProcessorTrigger(ADC0_BASE, 3) ;
	        while(!ADCIntStatus(ADC0_BASE, 3, false)) ;
	        ADCSequenceDataGet(ADC0_BASE, 3, &adcValue) ;
	        adcValue = (uint32_t)(147.5 - ((75.0*3.3 *(float)adcValue)) / 4096.0);
	        UARTprintf("the temprrature: %d \n\n", adcValue) ;
	        SysTick80_Delay_10ms(100);

	    }
}

void ADC_Init(void) {

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0) ;
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) ;

    // adc0, sampleseq 3, processor triggering, higher priority
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0) ;

    // use temp sensor, enable int on adc level NOT to cpu, end sequence
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_IE|ADC_CTL_TS|ADC_CTL_END) ;

    // enable the sequencer
    ADCSequenceEnable(ADC0_BASE, 3) ;

    // clear int at the beginning
    ADCIntClear(ADC0_BASE, 3) ;


}
