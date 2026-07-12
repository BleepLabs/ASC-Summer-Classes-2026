
#include "wt.h"

#define TIMER_INTERRUPT_DEBUG 1
#define _TIMERINTERRUPT_LOGLEVEL_ 4

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "RPi_Pico_TimerInterrupt.h"
//https://github.com/khoih-prog/RPI_PICO_TimerInterrupt
const float chromatic[121] = { 21.82676736, 23.12465449, 24.499718, 25.95654704, 27.50000365, 29.13523896, 30.86771042, 32.7032, 34.64783346, 36.70810085, 38.89087812, 41.20345007, 43.65353471, 46.24930897, 48.99943599, 51.91309407, 55.00000728, 58.27047791, 61.73542083, 65.40639999, 69.29566692, 73.4162017, 77.78175623, 82.40690014, 87.30706942, 92.49861792, 97.99887197, 103.8261881, 110.0000146, 116.5409558, 123.4708417, 130.8128, 138.5913338, 146.8324034, 155.5635124, 164.8138003, 174.6141388, 184.9972358, 195.9977439, 207.6523763, 220.0000291, 233.0819116, 246.9416833, 261.6255999, 277.1826676, 293.6648067, 311.1270248, 329.6276005, 349.2282776, 369.9944716, 391.9954878, 415.3047525, 440.0000581, 466.1638231, 493.8833665, 523.2511997, 554.3653352, 587.3296134, 622.2540496, 659.2552009, 698.4565551, 739.9889431, 783.9909755, 830.6095048, 880.0001162, 932.3276461, 987.7667329, 1046.502399, 1108.73067, 1174.659227, 1244.508099, 1318.510402, 1396.91311, 1479.977886, 1567.981951, 1661.219009, 1760.000232, 1864.655292, 1975.533466, 2093.004798, 2217.46134, 2349.318453, 2489.016198, 2637.020803, 2793.82622, 2959.955772, 3135.963901, 3322.438019, 3520.000464, 3729.310584, 3951.066931, 4186.009596, 4434.92268, 4698.636906, 4978.032395, 5274.041605, 5587.652439, 5919.911543, 6271.927802, 6644.876037, 7040.000927, 7458.621167, 7902.133861, 8372.019192, 8869.845359, 9397.273811, 9956.06479, 10548.08321, 11175.30488, 11839.82309, 12543.8556, 13289.75207, 14080.00185, 14917.24233, 15804.26772, 16744.03838 };
const byte major[65] = { 1, 3, 5, 6, 8, 10, 12, 13, 15, 17, 18, 20, 22, 24, 25, 27, 29, 30, 32, 34, 36, 37, 39, 41, 42, 44, 46, 48, 49, 51, 53, 54, 56, 58, 60, 61, 63, 65, 66, 68, 70, 72, 73, 75, 77, 78, 80, 82, 84, 85, 87, 89, 90, 92, 94, 96, 97, 99, 101, 102 };


#ifndef LED_BUILTIN
#define LED_BUILTIN 25
#endif

int wavelength = 4095;
unsigned long accumulator[8] = {};  //these are arrays here where are 8 separate variables called accumulator. accumulator[0],accumulator[1],etc
unsigned long increment[8] = {};
unsigned long waveindex[8] = {};

float freq = 41666.666 * 2.0;
byte outpin = 13;
unsigned long prev[4], cm;
volatile uint32_t dds_tune;
volatile uint32_t dds_tune_slow;
float dds_rate;
float dds_rate_slow;

byte tick;
int lfo_test, amp0;
int j;
int out[6];
int arp1;
byte toggle = 1;
int adc_ch0;
int raw_read[9];
int sm_in[9][16];
int sm_out[9];
int sm_count;
volatile float expo_pot[9];
int32_t toprint;
int artick;
int osctick = 0;
int osc_flip = 0;
int quant_en = 0;
int quant;
int mux_select;
int ctrl_pin1 = 16;
int ctrl_pin2 = 17;
int ctrl_pin3 = 18;
int expo_array[1026];
int spi_tick = 0;
int am_in_array[9];
float lerp_array[4100];
int am_sm;
volatile int vp, vj;
int pwm_sel;
float follow;
int led_tick;
float lerp_step[8];
int prev_random[8];
int r_slope[8];
int lerp_out[8];
int sq_out[4];
int div_out[4];
int read_tick;
int fm_pot, fm_cv;
int am_cv;
int t1;
float qn;

// Init RPI_PICO_Timer, can use any from 0-15 pseudo-hardware timers
RPI_PICO_Timer ITimer0(0);

void setup() {
  Serial.begin(115200);
  for (int j = 0; j < 1024; j++) {
    expo_array[j] = expo_converter(j, 1023, 1.7);
  }
  analogWriteFreq(500000);
  analogWriteResolution(12);

  pinMode(LED_BUILTIN, OUTPUT);
  for (int j = 10; j < 21; j++) {
    pinMode(j, OUTPUT);
  }
  pinMode(10, INPUT_PULLUP);


  dds_rate = 20;

  if (ITimer0.attachInterruptInterval(dds_rate, TimerHandler0)) {
    Serial.print(F("Starting ITimer0 OK, millis() = "));
    Serial.println(millis());
  }

  dds_tune = (1.00 / (dds_rate * .000001));

  increment[0] = (4294967296.00 * 440) / (dds_tune);

  Serial.flush();
}


bool TimerHandler0(struct repeating_timer *t) {
  (void)t;
  digitalWrite(15, 1);

  analogWrite(16, (out[0] * am_cv) >> 10);
  analogWrite(17, (out[0] * am_cv) >> 10);
  out[0] = wt_osc(0);

  fm_pot = (expo_pot[2] * 1.5) + 30;
  fm_cv = (expo_pot[0] * 4.0) + 1023.0;
  am_cv = 1023 - expo_pot[1];

  if (fm_cv < 1.05) { fm_cv = 1.05; }
  if (am_cv > 1000) { am_cv = 1023; }

  quant_en = digitalRead(10);
  if (quant_en == 0) {
    int fm_final = (fm_pot * fm_cv) >> 10;
    increment[0] = (4294967296.00 * fm_final) / (dds_tune);
  }
    if (quant_en == 1) {
    int fm_final = ((fm_pot * fm_cv) >> 10)/200;
    float cf=chromatic[major[(fm_final*2)+14]];
    increment[0] = (4294967296.00 * cf) / (dds_tune);
  }

  raw_read[mux_select] = analogRead(26 + mux_select);

  sm_in[mux_select][sm_count] = raw_read[mux_select];
  sm_out[mux_select] = 0;
  for (int j = 0; j < 8; j++) {
    sm_out[mux_select] += sm_in[mux_select][j];
  }
  sm_out[mux_select] /= 9;
  expo_pot[mux_select] = expo_array[sm_out[mux_select]];

  mux_select++;
  if (mux_select > 2) {
    mux_select = 0;
    sm_count++;
    if (sm_count > 7) {
      sm_count = 0;
    }
  }


  digitalWrite(15, 0);

  return true;
}


void loop() {
  cm = millis();
  if (cm - prev[0] > 500 && 0) {
    prev[0] = cm;

    //Serial.println(raw_read[0]);
    // Serial.println(raw_read[1]);
    Serial.println(raw_read[2]);
    // Serial.println();

    Serial.println(t1);
    Serial.println(qn);
    Serial.println();
  }
}


int wt_osc(byte sel) {
  int tout;
  //prev_waveindex[sel] = waveindex[sel];
  accumulator[sel] += increment[sel];
  waveindex[sel] = ((accumulator[sel]) >> (32 - 12));
  tout = sine[waveindex[sel]];
  if (waveindex[sel] < 2048) {
    sq_out[sel] = 1;
  } else {
    sq_out[sel] = 0;
  }
  return tout;
}



float expo_converter(float input, float max1, float curve) {
  // exponential conversion = (in^n) / (max value of in^(n-1))
  //input is whatever
  //max is max of input
  //curve .00001-.9999 log
  //1 linear
  //between 1-10 is expo
  //powf more eff on teensy
  float ex1 = powf(input, curve) / powf(max1, curve - 1.0);
  return ex1;
}
