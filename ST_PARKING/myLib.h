#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "ssd1306.h"
#include "parcheggio_pieno.c"
#include "parking_spots.c"
#include "welcome.c"


/*
 * Define values
 */

#define VOLTAGE_RES ((float)3.3/4096)

#define ADC_GRP_NUM_CHANNELS    1
#define ADC_GRP_BUF_DEPTH       1
static adcsample_t samples[ADC_GRP_NUM_CHANNELS*ADC_GRP_BUF_DEPTH];

#define LEDS_LINE                   PAL_LINE(GPIOB,4)     //D5 -> PWM
#define IR_ENTRY_LINE               PAL_LINE(GPIOA,10)    //D2 -> OUT
#define IR_EXIT_LINE                PAL_LINE(GPIOB,5)     //D4 -> OUT
#define SERVO_ENTRY_LINE            PAL_LINE(GPIOB,6)     //D10 -> PWM
#define SERVO_EXIT_LINE             PAL_LINE(GPIOB,7)     //PIN 21 STMorpho Sx
#define SERVO_ZERO_DEGREES          750
#define SERVO_NINETY_DEGREES        1250

#define BUF_SIZE                    20

#define PWM_TIMER_FREQUENCY_LEDS    1000000
#define PWM_PERIOD_LEDS             1000
#define PWM_TIMER_FREQUENCY_SERVO   1000000
#define PWM_PERIOD_SERVO            (PWM_TIMER_FREQUENCY_SERVO * 20 / 1000)

#define PWM_ZERO_CHANNEL             0
#define PWM_ONE_CHANNEL              1

/*
 * Const Values
 */
int posti=6;
static const int pw_value_zero = 0;
static const int pwm_value_min= (int) ((30/100.0)*10000);
static const int pwm_value_max= (int) ((100/100.0)*10000);
char buffer[BUF_SIZE];
static SSD1306Driver SSD1306D1;


/*
 * Function Prototypes
 */
void led_print(const ssd1306_color_t image_data[]);
void posti_liberi(void);
void set_ADC(void);
void set_PWMLED(void);
void set_PWMSERVOENTRY(void);
void set_IRENTRY(void);
void set_OLED(void);
void set_PWMSERVOEXIT(void);
void set_IREXIT(void);



/*
 * Pwm Leds Config
 */
static PWMConfig pwmcfg_leds={
   PWM_TIMER_FREQUENCY_LEDS,
   PWM_PERIOD_LEDS,
   NULL,
   {
    {PWM_OUTPUT_ACTIVE_HIGH,NULL},
    {PWM_OUTPUT_DISABLED,NULL},
    {PWM_OUTPUT_DISABLED,NULL},
    {PWM_OUTPUT_DISABLED,NULL}
   },
   0,
   0,
   0
};


/*
 * Configures PWM Driver
 */
static PWMConfig pwmcfg_servo={
   .frequency=PWM_TIMER_FREQUENCY_SERVO,
   .period=PWM_PERIOD_SERVO,
   .callback=NULL,
   .channels={
    {PWM_OUTPUT_ACTIVE_HIGH,NULL},   // ENTRY SERVO CHANNEL
    {PWM_OUTPUT_ACTIVE_HIGH,NULL},   // EXIT SERVO CHANNEL
    {PWM_OUTPUT_DISABLED,NULL},
    {PWM_OUTPUT_DISABLED,NULL}
   }
};



/*
 * ADC Config
 * IN: IN1      SAMPLES: 1
 */
static const ADCConversionGroup adcgrpcfg1 = {
    .circular=false,
    .num_channels=ADC_GRP_NUM_CHANNELS,
    .end_cb=NULL,
    .error_cb=NULL,
    .cfgr=ADC_CFGR_CONT,
    .cfgr2=0U,
    .tr1=ADC_TR_DISABLED,
    .tr2=ADC_TR_DISABLED,
    .tr3=ADC_TR_DISABLED,
    .awd2cr=0U,
    .awd3cr=0U,
    .smpr={
      ADC_SMPR1_SMP_AN1(ADC_SMPR_SMP_247P5),
      0U
    },
    .sqr={
          ADC_SQR1_SQ1_N(ADC_CHANNEL_IN1),
          0U,
          0U,
          0U
    }
};



/*
 * Oled Config
 */
static const I2CConfig i2ccfg = {
// I2C_TIMINGR address offset
    .timingr = 0x10, .cr1 = 0, .cr2 = 1, };
static const SSD1306Config ssd1306cfg = {&I2CD1, &i2ccfg, SSD1306_SAD_0X78, };




