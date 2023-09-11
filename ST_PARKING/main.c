/*
 * Copyrights: Luca Comentale
 *             Salvatore Cangiano
 *             Giovanni D'Ambrosio
 */

#include "myLib.h"


static int power_max = 0;


/*
 * Entry Parking Thread
 */
static THD_WORKING_AREA(waThreadExit,512);
static THD_FUNCTION(ThreadExit,arg){

  (void)arg;
  chRegSetThreadName("ExitServoThread");

  /*
   * Mode Settings
   */
  set_ADC();
  set_PWMLED();
  set_PWMSERVOEXIT();
  set_IREXIT();

  pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, SERVO_ZERO_DEGREES));
  pwmEnableChannel(&PWMD3, 0,PWM_PERCENTAGE_TO_WIDTH(&PWMD3, pw_value_zero));

  while(true){

    adcConvert(&ADCD1, &adcgrpcfg1, samples, ADC_GRP_BUF_DEPTH);
    chThdSleepMilliseconds(100);
    /*
     * Night
     */
    if(samples[0] <= 223){


      /*
       * If it is night I turn on the lights in energy-saving mode
       */

      if(power_max == 1){
         continue;
      }

      pwmEnableChannel(&PWMD3, 0,PWM_PERCENTAGE_TO_WIDTH(&PWMD3, pwm_value_min));

      if(palReadLine(IR_EXIT_LINE) == PAL_LOW && posti<6){

        /*
         * If the exit sensor detects the car and the parking space is not empty then
         * open the exit barrier
         */
        pwmEnableChannel(&PWMD4,1,PWM_PERCENTAGE_TO_WIDTH(&PWMD4, SERVO_NINETY_DEGREES));


        /*
         * As long as the sensor detects the machine, it keeps the barrier open
         */

        while(palReadLine(IR_EXIT_LINE)==PAL_LOW){
                    chThdSleepMilliseconds(200);
        }//End While PalReadLine

        /*
         * Once the car has left, the available seats are increased and the barrier is
         * closed after a set delay.
         */
        posti++;
        chThdSleepMilliseconds(2000);
        pwmEnableChannel(&PWMD3, 0,PWM_PERCENTAGE_TO_WIDTH(&PWMD3, pwm_value_min));
        pwmEnableChannel(&PWMD4, 1,PWM_PERCENTAGE_TO_WIDTH(&PWMD4, SERVO_ZERO_DEGREES));
      }//End if PalReadLine && posti<6
    }//End If Samples

    /*
     * Daylight
     */
    else{

      /*
       * The opening and closing operation is the same with the absence
       * of lights being daytime.
       */
      if(palReadLine(IR_EXIT_LINE) == PAL_LOW && posti<6){

        pwmEnableChannel(&PWMD4,1,PWM_PERCENTAGE_TO_WIDTH(&PWMD4, SERVO_NINETY_DEGREES));

        while(palReadLine(IR_EXIT_LINE)==PAL_LOW){
                  chThdSleepMilliseconds(200);
        }//End While PalReadline

        posti++;
        chThdSleepMilliseconds(2000);
        pwmEnableChannel(&PWMD4, 1,PWM_PERCENTAGE_TO_WIDTH(&PWMD4, SERVO_ZERO_DEGREES));

      }//End if PalReadLine && posti<6
    }//End Else
    chThdSleepMilliseconds(200);
  }//End While Thread

}//End Thread




/*
 * Entry Parking Thread
 */
static THD_WORKING_AREA(waThreadEntry,512);
static THD_FUNCTION(ThreadEntry,arg){

  (void)arg;
  chRegSetThreadName("EntryThread");

  /*
   * Mode Settings
   */
  set_ADC();
  set_PWMLED();
  set_PWMSERVOENTRY();
  set_IRENTRY();

  /*
   * Initial setting of the entry barrier and Leds
   */
  pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, SERVO_ZERO_DEGREES));
  pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, pw_value_zero));


  while(true){

    /*
     * ADC Start Conversion
     */
    adcConvert(&ADCD1, &adcgrpcfg1, samples, ADC_GRP_BUF_DEPTH);

    /*
     * Night
     */
    if(samples[0] <= 223){

      /*
       * Energy-saving parking lights are switched on at night
       */
      pwmEnableChannel(&PWMD3, 0,PWM_PERCENTAGE_TO_WIDTH(&PWMD3, pwm_value_min));


      /*
       * If the entrance sensor detects a car and places are still available
       * then the barrier opens, otherwise the barrier remains closed.
       */

      if(palReadLine(IR_ENTRY_LINE)==PAL_LOW && posti>0){

        power_max = 1;
        pwmEnableChannel(&PWMD3, 0,PWM_PERCENTAGE_TO_WIDTH(&PWMD3, pwm_value_max));
        pwmEnableChannel(&PWMD4,0,PWM_PERCENTAGE_TO_WIDTH(&PWMD4, SERVO_NINETY_DEGREES));

        /*
         * The barrier remains open as long as the sensor detects a car
         */
        while(palReadLine(IR_ENTRY_LINE)==PAL_LOW){
                  chThdSleepMilliseconds(200);
        }//End While PalReadLine

        /*
         * Once the car has entered, the number of available seats
         * is decreased and the bar is lowered after a set delay.
         */
        posti--;
        chThdSleepMilliseconds(2000);
        pwmEnableChannel(&PWMD4, 0,PWM_PERCENTAGE_TO_WIDTH(&PWMD4, SERVO_ZERO_DEGREES));
        power_max = 0;
        chThdSleepMilliseconds(2000);
        pwmEnableChannel(&PWMD3, 0,PWM_PERCENTAGE_TO_WIDTH(&PWMD3, pwm_value_min));


      }// End if palReadLine && posti >0
    }//End If Samples

    /*
     * Daylight
     */
    else{
      /*
       * Barrier management is the same except for lighting management
       */
      pwmEnableChannel(&PWMD3, 0,PWM_PERCENTAGE_TO_WIDTH(&PWMD3, pw_value_zero));
      pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, SERVO_ZERO_DEGREES));

      if(palReadLine(IR_ENTRY_LINE)==PAL_LOW && posti>0){

        pwmEnableChannel(&PWMD4,0,PWM_PERCENTAGE_TO_WIDTH(&PWMD4,SERVO_NINETY_DEGREES));

        while(palReadLine(IR_ENTRY_LINE)==PAL_LOW){
          chThdSleepMilliseconds(200);
        }//End While PalReadLine

        posti--;
        chThdSleepMilliseconds(2000);
        pwmEnableChannel(&PWMD4, 0,PWM_PERCENTAGE_TO_WIDTH(&PWMD4, SERVO_ZERO_DEGREES));
      }//End If PalReadLIne && Posti >0

    }//End Else

    chThdSleepMilliseconds(200);

  }//End While Thread

}//End Thread




/*
 * Oled Thread
 */

static THD_WORKING_AREA(waOledDisplay, 512);
static THD_FUNCTION(thdOledDisplay, arg) {

  (void)arg;
  chRegSetThreadName("thdOledDisplay");

  /*
   * Display Settings
   */

  set_OLED();
  ssd1306ObjectInit(&SSD1306D1);
  ssd1306Start(&SSD1306D1, &ssd1306cfg);
  ssd1306FillScreen(&SSD1306D1, 0x00);


  while (true) {

    /*
     * Print Welcome Image
     */
    led_print(welcome);
    chThdSleepMilliseconds(3000);

    systime_t start = chVTGetSystemTime();
    systime_t end = chTimeAddX(start, TIME_MS2I(7000));

    /*
     * Print image of full car park if places are sold out,
     * otherwise screen showing free parking
     */

    if (posti == 0) {
      led_print(parcheggio_pieno);
    }
    else {
      led_print(parking_spots);
      posti_liberi();
    }

    while (chVTIsSystemTimeWithin(start, end)) {

      chThdSleepMilliseconds(10);
    }
  }
}





/*
 * Main
 */


int main(void) {

  halInit();
  chSysInit();

  /* Threads */

  chThdCreateStatic(waOledDisplay, sizeof(waOledDisplay), NORMALPRIO+1,thdOledDisplay, NULL);
  chThdCreateStatic(waThreadEntry,sizeof(waThreadEntry),NORMALPRIO + 5,ThreadEntry,NULL);
  chThdCreateStatic(waThreadExit,sizeof(waThreadExit),NORMALPRIO + 5,ThreadExit,NULL);

  /*Main Activity*/

  while (true) {
    palToggleLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
  }

  adcStop(&ADCD1);

}






/*
 * Functions
 */

void led_print(const ssd1306_color_t image_data[]) {
  int x = 0;
  int y = 0;
  int c = 0;
  for (y = 0; y < SSD1306_HEIGHT; y++) {
    for (x = 0; x < SSD1306_WIDTH; x++) {
      ssd1306DrawPixel(&SSD1306D1, x, y, image_data[c]);
      c++;
    }
    ssd1306UpdateScreen(&SSD1306D1);
  }
  ssd1306UpdateScreen(&SSD1306D1);
}


void posti_liberi(void) {
  ssd1306GotoXy(&SSD1306D1, 10, 4);
  chsnprintf(buffer, 20, "POSTI LIBERI:%d", posti);
  ssd1306Puts(&SSD1306D1, buffer, &ssd1306_font_7x10, SSD1306_COLOR_BLACK);
  ssd1306UpdateScreen(&SSD1306D1);
}

void set_ADC(void){
  adcStart(&ADCD1,NULL);
  adcSTM32EnableVREF(&ADCD1);
  palSetPadMode(GPIOA,0U,PAL_MODE_INPUT_ANALOG);
}

void set_PWMLED(void){
  pwmStart(&PWMD3,&pwmcfg_leds);
  pwmEnablePeriodicNotification(&PWMD3);
  palSetLineMode(LEDS_LINE,PAL_MODE_ALTERNATE(2));

}

void set_PWMSERVOENTRY(void){
  pwmStart(&PWMD4, &pwmcfg_servo);
  pwmEnablePeriodicNotification(&PWMD4);
  palSetLineMode(SERVO_ENTRY_LINE,PAL_MODE_ALTERNATE(2));
}

void set_PWMSERVOEXIT(void){
  palSetLineMode(SERVO_EXIT_LINE,PAL_MODE_ALTERNATE(2));
}

void set_IRENTRY(void){
  palSetLineMode(IR_ENTRY_LINE,PAL_MODE_INPUT_PULLUP);
}

void set_IREXIT(void){
  palSetLineMode(IR_EXIT_LINE,PAL_MODE_INPUT_PULLUP);
}

void set_OLED(void){
  palSetLineMode(PAL_LINE(GPIOB, 8U), PAL_MODE_ALTERNATE(4) |

  PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST |

  PAL_STM32_PUPDR_PULLUP);

  palSetLineMode(PAL_LINE(GPIOB, 9U), PAL_MODE_ALTERNATE(4) |

  PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST |

  PAL_STM32_PUPDR_PULLUP);
}
