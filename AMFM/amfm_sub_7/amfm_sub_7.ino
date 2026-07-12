#include "wt.h"

#define TIMER_INTERRUPT_DEBUG 0
#define _TIMERINTERRUPT_LOGLEVEL_ 0

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "RPi_Pico_TimerInterrupt.h"
//https://github.com/khoih-prog/RPI_PICO_TimerInterrupt


#ifndef LED_BUILTIN
#define LED_BUILTIN 25
#endif


const static float chromatic[121] = { 21.82676736, 23.12465449, 24.499718, 25.95654704, 27.50000365, 29.13523896, 30.86771042, 32.7032, 34.64783346, 36.70810085, 38.89087812, 41.20345007, 43.65353471, 46.24930897, 48.99943599, 51.91309407, 55.00000728, 58.27047791, 61.73542083, 65.40639999, 69.29566692, 73.4162017, 77.78175623, 82.40690014, 87.30706942, 92.49861792, 97.99887197, 103.8261881, 110.0000146, 116.5409558, 123.4708417, 130.8128, 138.5913338, 146.8324034, 155.5635124, 164.8138003, 174.6141388, 184.9972358, 195.9977439, 207.6523763, 220.0000291, 233.0819116, 246.9416833, 261.6255999, 277.1826676, 293.6648067, 311.1270248, 329.6276005, 349.2282776, 369.9944716, 391.9954878, 415.3047525, 440.0000581, 466.1638231, 493.8833665, 523.2511997, 554.3653352, 587.3296134, 622.2540496, 659.2552009, 698.4565551, 739.9889431, 783.9909755, 830.6095048, 880.0001162, 932.3276461, 987.7667329, 1046.502399, 1108.73067, 1174.659227, 1244.508099, 1318.510402, 1396.91311, 1479.977886, 1567.981951, 1661.219009, 1760.000232, 1864.655292, 1975.533466, 2093.004798, 2217.46134, 2349.318453, 2489.016198, 2637.020803, 2793.82622, 2959.955772, 3135.963901, 3322.438019, 3520.000464, 3729.310584, 3951.066931, 4186.009596, 4434.92268, 4698.636906, 4978.032395, 5274.041605, 5587.652439, 5919.911543, 6271.927802, 6644.876037, 7040.000927, 7458.621167, 7902.133861, 8372.019192, 8869.845359, 9397.273811, 9956.06479, 10548.08321, 11175.30488, 11839.82309, 12543.8556, 13289.75207, 14080.00185, 14917.24233, 15804.26772, 16744.03838 };
int wavelength = 4095;
unsigned long accumulator[8] = {};  //these are arrays here where are 8 separate variables called accumulator. accumulator[0],accumulator[1],etc
unsigned long increment[8] = {};
unsigned long waveindex[8] = {};
unsigned long prev_waveindex[8] = {};
unsigned long random_out[8] = {};


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
int ctrl_pin1 = 21;
int ctrl_pin2 = 20;
int ctrl_pin3 = 19;
int expo_array[1026];
int spi_tick = 0;
int am_in_array[9];
float lerp_array[4100];
int am_sm;
volatile int vp, vj;
int pwm_sel;
float follow;
RPI_PICO_Timer ITimer0(0);
int led_tick;
float lerp_step[8];
int prev_random[8];
int r_slope[8];
int lerp_out[8];
byte sq_out[8];
float fm1;

void setup() {
  Serial.begin(115200);
  for (int j = 0; j < 1024; j++) {
    expo_array[j] = expo_converter(j, 1023, 2);
  }
  analogWriteFreq(500000);
  analogWriteResolution(12);

  pinMode(LED_BUILTIN, OUTPUT);
  for (int j = 0; j < 23; j++) {
    pinMode(j, OUTPUT);
  }

  dds_rate = 25;

  if (ITimer0.attachInterruptInterval(dds_rate, TimerHandler0)) {
    Serial.print(F("Starting ITimer0 OK, millis() = "));
    Serial.println(millis());
  }

  dds_tune = (1.00 / (dds_rate * .000001));

  Serial.flush();
}

void setup1() {
}
int led_sel;
bool TimerHandler0(struct repeating_timer *t) {
  (void)t;
  digitalWrite(17, HIGH);

  out[1] = wt_osc(1);
  out[2] = wt_osc(2);

  float cv_fm_1 = expo_pot[1] / 1000.0;
  fm1 = ((expo_pot[4] + 1.0) / 2.0) * cv_fm_1;
  increment[1] = (4294967296.00 * fm1) / dds_tune;

  float cv_fm_2 = expo_pot[0] / 1000.0;
  float fm2 = ((expo_pot[6] + 3.0) / 2.0) * cv_fm_2;
  increment[2] = (4294967296.00 * fm2) / dds_tune;

  int cv_am_1 = 1023 - int(sm_out[3]);
  int cv_am_2 = 1023 - int(sm_out[2]);

  int pot_am_1 = sm_out[5];
  int pot_am_2 = sm_out[7];


  int am1 = (cv_am_1 * pot_am_1) >> 10;
  int am2 = (cv_am_2 * pot_am_2) >> 10;

  pwm_sel++;
  if (pwm_sel > 3) {
    pwm_sel = 0;
  }


  if (pwm_sel == 0) {

    analogWrite(13, (out[1] * am1) >> 10);  //sines
    analogWrite(5, (out[1] * am1) >> 10);

    analogWrite(9, (out[2] * am2) >> 10);
    analogWrite(1, (out[2] * am2) >> 10);
  }

  if (pwm_sel == 1) {
    analogWrite(15, ((waveindex[1]) * am1) >> 10);  //ramp
    analogWrite(7, ((waveindex[1]) * am1) >> 10);   //ramp

    int tt = waveindex[2] >> 2;
    int pip = expo_array[1023 - tt];
    analogWrite(11, ((pip << 2) * am2) >> 10);  //saw
    analogWrite(3, ((pip << 2) * am2) >> 10);
  }

  if (pwm_sel == 2) {
    int o1 = (random_out[1] * am1) >> 10;
    int o2 = ((random_out[2] ^ waveindex[2]) * am2) >> 10;
    analogWrite(14, o1);
    analogWrite(6, o1);

    analogWrite(10, o2);
    analogWrite(2, o2);
  }

  if (pwm_sel == 3) {
    analogWrite(12, sq_out[1] * (am1 << 2));
    analogWrite(4, sq_out[1] * (am1 << 2));

    analogWrite(8, sq_out[2] * (am2 << 2));
    analogWrite(0, sq_out[2] * (am2 << 2));
  }
  digitalWrite(17, LOW);
  return true;
}


void loop() {  //output

  // digitalWrite(14, LOW);

  // digitalWrite(14, HIGH);
}




void loop1() {  //input and slow
  cm = millis();

  raw_read[mux_select] = analogRead(28);
  sm_in[mux_select][sm_count] = raw_read[mux_select];
  am_sm = 0;
  sm_out[mux_select] = 0;
  for (int j = 0; j < 8; j++) {
    sm_out[mux_select] += sm_in[mux_select][j];
    am_sm += am_in_array[j];
  }
  am_sm /= 9;
  sm_out[mux_select] /= 8;

  expo_pot[mux_select] = expo_array[sm_out[mux_select]];

  mux_select++;
  if (mux_select > 7) {
    mux_select = 0;
    sm_count++;
    if (sm_count > 7) {
      sm_count = 0;
    }
  }

  digitalWrite(ctrl_pin1, bitRead(mux_select, 0));
  digitalWrite(ctrl_pin2, bitRead(mux_select, 1));
  digitalWrite(ctrl_pin3, bitRead(mux_select, 2));

  if (cm - prev[0] > 200 && 1) {
    prev[0] = cm;
    for (int j = 0; j < 8; j++) {
      Serial.print(j);
      Serial.print(" ");
      Serial.print(expo_pot[j]);

      Serial.println();
    }

    Serial.println();
  }

  //digitalWrite(21, LOW);
}


int sq_tick;
int wt_osc(byte sel) {
  int tout;
  prev_waveindex[sel] = waveindex[sel];
  accumulator[sel] += increment[sel];
  waveindex[sel] = ((accumulator[sel]) >> (32 - 12));
  tout = sine[waveindex[sel]];
  if (prev_waveindex[sel] > 2000 && waveindex[sel] < 2000) {
    random_out[sel] = random(32) << 7;
    sq_out[sel] = !sq_out[sel];
  }
  return tout;
}


int rl_osc(byte sel) {  /// tooo slow
  int tout;
  prev_waveindex[sel] = waveindex[sel];
  accumulator[sel] += increment[sel];
  waveindex[sel] = ((accumulator[sel]) >> (32 - 12));
  //tout = sine[waveindex[sel]];
  if (prev_waveindex[sel] > 2000 && waveindex[sel] < 2000) {
    prev_random[sel] = random_out[sel];
    random_out[sel] = random(32) << 7;

    lerp_array[0] = prev_random[sel];
    lerp_array[4095] = random_out[sel];
    if (prev_random[sel] > random_out[sel]) {  //down
      lerp_step[sel] = -1 * ((prev_random[sel] - random_out[sel]) / 4095.0);
    } else {
      lerp_step[sel] = (random_out[sel] - prev_random[sel]) / 4095.0;
    }
    for (int j = 1; j < 4096; j++) {
      lerp_array[j] = lerp_array[j - 1] + lerp_step[sel];
    }
  }
  lerp_out[sel] = lerp_array[waveindex[sel]];

  return lerp_out[sel];
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
