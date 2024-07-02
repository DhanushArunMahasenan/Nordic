
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
//#include "nrf_delay.h"
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
//#include "boards.h"
//#include "nrf_gpio.h"
//#include "app_util_platform.h"
//#include "nordic_common.h"
#include "nrf.h"
#include "hal/nrf_gpiote.h"
//#include "nrf_drv_gpiote.h"
#include "nrf51_bitfields.h"
//#include "app_timer.h"
//
#define pinTrig 25 // Output pin
#define pinEcho 11 // Input pin
LOG_MODULE_REGISTER(app, CONFIG_LOG_DEFAULT_LEVEL);


// counter
static volatile uint32_t tCount = 0;

// count to us (micro seconds) conversion factor
// set in start_timer()
static volatile float countToUs = 1;

// set up and start Timer1
void start_timer(void)
{ 
NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer; 
NRF_TIMER1->TASKS_CLEAR = 1;

uint8_t prescaler = 0;
NRF_TIMER1->PRESCALER = prescaler; 
NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_16Bit;

uint16_t comp1 = 500;
// set compare
NRF_TIMER1->CC[1] = comp1;

// set conversion factor
countToUs = 0.0625*comp1*(1 << prescaler);
LOG_DBG("App starting");
//shell_print("timer tick = \n");

// enable compare 1
NRF_TIMER1->INTENSET = 
(TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos);

// use the shorts register to clear compare 1
NRF_TIMER1->SHORTS = (TIMER_SHORTS_COMPARE1_CLEAR_Enabled << 
TIMER_SHORTS_COMPARE1_CLEAR_Pos);

// enable IRQ
NVIC_EnableIRQ(TIMER1_IRQn);

// start timer
NRF_TIMER1->TASKS_START = 1;
}

void TIMER1_IRQHandler(void)
{
if (NRF_TIMER1->EVENTS_COMPARE[1] && 
NRF_TIMER1->INTENSET & TIMER_INTENSET_COMPARE1_Msk) {

// clear compare register event 
NRF_TIMER1->EVENTS_COMPARE[1] = 0;

// increment count
tCount++;
}
}

bool getDistance(float* dist)
{
#if 0
nrf_gpio_pin_clear(pinTrig);
nrf_delay_us(20);
nrf_gpio_pin_set(pinTrig);
nrf_delay_us(12);
nrf_gpio_pin_clear(pinTrig);
nrf_delay_us(20);


while(!nrf_gpio_pin_read(pinEcho));
// reset counter
tCount = 0;
// wait till Echo pin goes low
while(nrf_gpio_pin_read(pinEcho));

float duration = countToUs*tCount;

float distance = duration*0.017;

if(distance < 400.0) {

// save
*dist = distance;

return true;
}
else
#endif
{
return false;
}
}

int main() {

// set up timers
//app_timer_init();
	if (IS_ENABLED(CONFIG_LOG_BACKEND_RTT)) {
		/* Give RTT log time to be flushed before executing tests */
		k_sleep(K_MSEC(500));
	}
start_timer();
printk("Threshold trigger\r\n");
int sum;
for(int i=0;i<10;i++)
    sum++;
    sum = sum*10;
// prints to serial port
LOG_INF("starting");
printk("Sum=%d", sum);

// set up HC-SR04 pins
//nrf_gpio_pin_dir_set(pinTrig, 0);//NRF_GPIO_PIN_DIR_OUTPUT);
//nrf_gpio_pin_dir_set(pinEcho, 1);//NRF_GPIO_PIN_DIR_INPUT);


// main loop:
while(1) {

// get HC-SR04 distance
float dist;
if(!getDistance(&dist)) {

// enable to print to serial port
printf("dist = %f cm\n", dist);
k_sleep(K_SECONDS(1));
}

// delay
//nrf_delay_ms(250);
}
}