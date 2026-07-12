
#include <MIDI.h>

#include <SoftwareSerial.h>
using Transport = MIDI_NAMESPACE::SerialMIDI<SoftwareSerial>;
int rxPin = 12;
int txPin = 13;
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);
Transport serialMIDI(mySerial);
MIDI_NAMESPACE::MidiInterface<Transport> MIDI((Transport&)serialMIDI);

int midi_note_offset = -36;
int note_offset = -7;
int keyb_led_offset_a = -8;
int keyb_led_offset_b = -2;
IntervalTimer myTimer;
#define sample_bank_size 120000
int16_t sample_bank1[sample_bank_size];
int sampler_bank_sel = 1;
#include <WS2812Serial.h>

const int numled = 39;  //actually 37
const int pin = 1;
byte drawingMemory[numled * 3];          //  3 bytes per LED
DMAMEM byte displayMemory[numled * 12];  // 12 bytes per LED
WS2812Serial leds(numled, displayMemory, drawingMemory, pin, WS2812_GRB);
float max_brightness = .1;
#include "mem_sampler.h"
#include "samples.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


#define voices 8
#include "audio_con.h"


int in_tune;


byte button_order[32] = { 0, 1, 2, 3, 7, 6, 5, 4, 11, 9, 10, 8, 14, 13, 15, 12, 19, 17, 18, 16, 21, 23, 20, 22, 25, 30, 28, 26, 27, 29, 31, 24 };

//byte button_order[32] = { 0, 1, 2, 3, 7, 6, 5, 4, 11, 9, 10, 8, 15, 13, 12, 14, 19, 17, 18, 16, 22, 20, 23, 21, 31, 24, 27, 28, 26, 29, 25, 30 };
byte key_led_order[64] = { 27, 14, 28, 15, 29, 30, 17, 31, 18, 32, 19, 33, 34, 21, 35, 22, 36, 37, 24, 38, 25, 39, 26, 40 };
byte pot_order[18] = { 7, 6, 16, 15, 14, 5, 0, 13, 8, 2, 4, 10, 11, 3, 1, 12, 9, 17 };
//byte pot_order[18] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
//byte pot_order[18] = { 6, 5, 16, 15, 8, 0, 7, 13, 14, 2, 3, 11, 9, 1, 4, 10, 12, 17 };
const static float chromatic[121] = { 16.3516, 17.32391673, 18.35405043, 19.44543906, 20.60172504, 21.82676736, 23.12465449, 24.499718, 25.95654704, 27.50000365, 29.13523896, 30.86771042, 32.7032, 34.64783346, 36.70810085, 38.89087812, 41.20345007, 43.65353471, 46.24930897, 48.99943599, 51.91309407, 55.00000728, 58.27047791, 61.73542083, 65.40639999, 69.29566692, 73.4162017, 77.78175623, 82.40690014, 87.30706942, 92.49861792, 97.99887197, 103.8261881, 110.0000146, 116.5409558, 123.4708417, 130.8128, 138.5913338, 146.8324034, 155.5635124, 164.8138003, 174.6141388, 184.9972358, 195.9977439, 207.6523763, 220.0000291, 233.0819116, 246.9416833, 261.6255999, 277.1826676, 293.6648067, 311.1270248, 329.6276005, 349.2282776, 369.9944716, 391.9954878, 415.3047525, 440.0000581, 466.1638231, 493.8833665, 523.2511997, 554.3653352, 587.3296134, 622.2540496, 659.2552009, 698.4565551, 739.9889431, 783.9909755, 830.6095048, 880.0001162, 932.3276461, 987.7667329, 1046.502399, 1108.73067, 1174.659227, 1244.508099, 1318.510402, 1396.91311, 1479.977886, 1567.981951, 1661.219009, 1760.000232, 1864.655292, 1975.533466, 2093.004798, 2217.46134, 2349.318453, 2489.016198, 2637.020803, 2793.82622, 2959.955772, 3135.963901, 3322.438019, 3520.000464, 3729.310584, 3951.066931, 4186.009596, 4434.92268, 4698.636906, 4978.032395, 5274.041605, 5587.652439, 5919.911543, 6271.927802, 6644.876037, 7040.000927, 7458.621167, 7902.133861, 8372.019192, 8869.845359, 9397.273811, 9956.06479, 10548.08321, 11175.30488, 11839.82309, 12543.8556, 13289.75207, 14080.00185, 14917.24233, 15804.26772, 16744.03838 };
byte control_buttons[8];
byte prev_control_buttons[8];
unsigned long current_time;
unsigned long prev_time[8];
float freq1;
float freq2;
float amp1, amp2;
int peak_env_read[voices];
int prev_peak_env_read[voices];
#define buttnum 32
char ser_data[6];
byte rawButts[buttnum];
byte prevRawButts[buttnum];
byte debButts[buttnum];
float pitchpot;
float print1;
byte keys[buttnum];
byte prev_keys[buttnum];
uint32_t bounceTime[buttnum];
byte fell[buttnum], rose[buttnum];
int debTime = 30;
#define inLatchpin 5
#define inClockPin 4
#define inDataPin 3
float pot_max = 1000;
float rp_led;
byte mux_select;
byte sm_count;
int pot[20];
int raw_pot[20];
int sm_pots[20];
int raw_piezo;
int prev_raw_piezo;
int thresh;
float drum_vel;
byte osc_select;
int poly_bank[voices];
int poly_time[voices];
int voice_status[voices];
int octave = 0;
int base_note[voices];
int prev_base_note[voices];
float peak_reading;
int rec_mode;
float mapt;
float expp2;
int shape_sel, prev_shape_sel;
float fm1, filter_mod, filter_mod_final, fmf;
uint32_t seq_rate;
int bpm, prev_bpm;
int main_clock_step, prev_main_clock_step;
int arp_seq[32] = { 8, 12, 14, 20, 0, 0, 0, 0, 0, 0 };
int arp_note_time[32];
int arp_note_store[32];
int arp_step_len = 4;
int arp_note_len;
int arp_mode = 0;
int arp_enable = 0;
int ordered_buttons[50];
int prev_lfo_shape_sel, lfo_shape_sel;
int avail_num;
uint32_t rec_len = 512;
#define getsize AUDIO_BLOCK_SAMPLES
uint32_t rec_size, address;
#define mem_size 120
float hp_follow;
float reverb_wet, reverb_dry;
float allenv;
int newest_voice;
float fff, ff;
float lfo_freq, lfo_filter, lfo_fm;
int input_sel;
float mic_peak, line_peak;
uint32_t countdown;
int hold_en;
int prev_hold_en;
float res;
float lfop;
float op;
uint32_t drum_timer, count_in;
byte drum_note_on;
int last_note = 0;
int drum_note;
uint32_t rec_ready_timer;
float lfo_peak_read_raw;
float envt, lfot;
int has_rec = 0;
#include "printer.h"
#include "sampler_functions.h"


void setup() {  //////////////////////////////////////////////////////////////////

  leds.begin();
  if (1) {
    //while (1) {
    float f1;
    for (int i = 0; i < 40; i++) {

      for (int j = 0; j < numled; j++) {
        float c1 = ((j / 50.0) + f1);
        if (c1 > 1) { c1 -= 1; }
        set_led(j, c1 + (j / (20.0 * (1.0 + c1))), (i / 40.0), .15);
      }
      set_led(6, 0, 0, 0);
      // set_led(8, 0, 0, 0);
      // set_led(12, 0, 0, 0);
      // set_led(16, 0, 0, 0);
      // set_led(19, 0, 0, 0);

      leds.show();
      f1 += .15;
      delay(25);
    }
  }

  for (int j = 0; j < numled; j++) {
    set_led(j, 0, 0, 0);
  }
  set_led(0, 0, 0, .1);
  leds.show();


  sgtl5000_1.enable();  //Turn the adapter board on
  sgtl5000_1.muteLineout();
  sgtl5000_1.muteHeadphone();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);  //Tell it what input we want to use. Not necessary is you're not using the ins
  sgtl5000_1.lineInLevel(0);                   //The volume of the input. 0-15 with 15 bing more amplifications
  sgtl5000_1.volume(0.5);                      //hp_vol
  sgtl5000_1.lineOutLevel(21);                 //11-32, the smaller the louder. 21 is about 2 Volts peak to peak
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(56);

  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(57600);



  AudioMemory(mem_size);

  pinMode(2, INPUT_PULLUP);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, INPUT);

  pinMode(inLatchpin, OUTPUT);
  pinMode(inClockPin, OUTPUT);
  pinMode(inDataPin, INPUT_PULLUP);

  white_noise.amplitude(1);

  lfo1.begin(1, 220.0, WAVEFORM_SINE);

  for (int j = 0; j < voices; j++) {
    poly_bank[j] = -1;
    wave[0][j].begin(1, 220.0, WAVEFORM_SINE);
    wave[0][j].frequencyModulation(0);
    wave[1][j].begin(1, 220.0, WAVEFORM_SINE);
    wave[1][j].frequencyModulation(0);

    mix_wave[j].gain(0, 0);
    mix_wave[j].gain(1, 0);
    mix_wave[j].gain(2, 0);
    mix_wave[j].gain(3, 0);

    vel_mix[j].gain(0, 1);
    vel_mix[j].gain(1, 0);
    vel_mix[j].gain(2, 0);
    vel_mix[j].gain(3, 0);

    env_vca[j].attack(10);
    env_vca[j].decay(5);
    env_vca[j].sustain(.9);
    env_vca[j].release(200);

    filter[j].frequency(15000);
    filter[j].resonance(.6);
    filter[j].octaveControl(0);

    //sampler[j].begin(cow, cow_size);
    sampler[j].begin(sample_bank1, rec_len);

    sampler[j].sample_loop(1);
    sampler[j].frequency(1.0);
    sampler[j].sample_reverse(0);
  }
  reverb.roomsize(.8);
  reverb.damping(.9);

  //float mix_lvl = 1.0 / 9.0;
  float mix_lvl = 1;
  dc1.amplitude(1);
  dc_wave.amplitude(.25);

  dc_lfo.amplitude(.5);
  lfo_offset_mixer.gain(0, .5);
  lfo_offset_mixer.gain(1, 1);

  mixer_sub_1.gain(0, mix_lvl);
  mixer_sub_1.gain(1, mix_lvl);
  mixer_sub_1.gain(2, mix_lvl);
  mixer_sub_1.gain(3, mix_lvl);
  mixer_sub_2.gain(0, mix_lvl);
  mixer_sub_2.gain(1, mix_lvl);
  mixer_sub_2.gain(2, mix_lvl);
  mixer_sub_2.gain(3, mix_lvl);
  mixer_pre.gain(0, 1);
  mixer_pre.gain(1, 1);
  mixer_pre.gain(2, 0);
  mixer_pre.gain(3, 0);
  mixer_f_left.gain(0, 0);  //
  mixer_f_left.gain(1, 0);
  mixer_f_left.gain(2, 0);
  mixer_f_left.gain(3, 0);
  mixer_f_right.gain(0, 0);  //
  mixer_f_right.gain(1, 0);
  mixer_f_right.gain(2, 0);
  mixer_f_right.gain(3, 0);

  bpm = 120;
  seq_rate = 60000000.0 / float(bpm) / 4.0;
  myTimer.begin(seq_clk, seq_rate);
  myTimer.priority(16);


  analogReadResolution(12);
  analogReadAveraging(32);

  delay(100);

  sgtl5000_1.unmuteLineout();
  sgtl5000_1.unmuteHeadphone();

}  //setup



#include "loop.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void seq_clk() {
  if (arp_enable == 0) {
    main_clock_step = 0;
    // for (int j = 0; j < 8; j++) {
    //   arp_seq[j] = 0;
    // }
    // arp_step_len = 0;
  }
  if (arp_enable == 1) {

    if (1) {
      int arp_note = arp_seq[main_clock_step];
      if (arp_note > 0) {
        arp_note_time[main_clock_step] = current_time;
        arp_note_store[main_clock_step] = arp_note;
        poly_note_on(arp_note, 1.0, 1);
      }
      // Serial.println(arp_note);
    }

    prev_main_clock_step = main_clock_step;
    main_clock_step++;
    if (main_clock_step >= arp_step_len) {
      main_clock_step = 0;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void poly_note_off(int key) {
  for (int j = 0; j < voices; j++) {
    if (key == poly_bank[j]) {
      env_vca[j].noteOff();
      env_mod[j].noteOff();

      // Serial.print("  off ");
      // Serial.println(key);
    }
  }
}

void poly_note_clear(int loc) {
  poly_bank[loc] = -1;
  poly_time[loc] = 0;
  voice_status[loc] = 0;
  base_note[loc] = 0;
  sampler[loc].stop();
}

void poly_note_off_select(int key, byte sel) {
  for (int j = 0; j < voices; j++) {
    if (key == poly_bank[j] && voice_status[j] == sel) {
      env_vca[j].noteOff();
      env_mod[j].noteOff();

      // Serial.print("  sel_off ");
      // Serial.println(key);
    }
  }
}

void poly_clean() {
  for (int j = 0; j < voices; j++) {
    if (voice_status[j] == 0) {
      env_vca[j].noteOff();
    }
  }
}

void poly_note_on(int key, float vel, byte stat) {
  int loc_sel;
  int already = 0;
  last_note = key;

  //key = ordered_buttons[key];
  for (int scan = 0; scan < voices; scan++) {
    if (poly_bank[scan] == key) {
      already = 1;
      poly_time[scan] = current_time;
      newest_voice = scan;
      // Serial.print(" already ");
      // Serial.print(key);
      // Serial.print(" ");
      // Serial.println(scan);

      poly_bank[scan] = key;
      voice_status[scan] = stat;
      base_note[scan] = key;

      vel_mix[scan].gain(0, vel);

      env_vca[scan].noteOn();
      env_mod[scan].noteOn();
      sampler[scan].play();
      break;
    }
  }
  if (already == 0) {
    for (int scan = 0; scan < voices; scan++) {
      if (poly_bank[scan] == -1) {
        poly_bank[scan] = key;
        poly_time[scan] = current_time;
        newest_voice = scan;
        // Serial.print(" new ");
        // Serial.print(key);
        // Serial.print(" ");
        // Serial.println(scan);
        poly_bank[scan] = key;
        voice_status[scan] = stat;
        base_note[scan] = key;
        vel_mix[scan].gain(0, vel);

        env_vca[scan].noteOn();
        env_mod[scan].noteOn();
        sampler[scan].play();

        break;
      }
      if (scan == voices - 1 && 0) {
        uint32_t compare1 = poly_time[scan];
        int oldest = 0;
        for (int st = 1; st < voices; st++) {
          if (poly_time[scan] < compare1 && poly_time[scan] > 0) {
            oldest = st;
            poly_time[scan] = compare1;
          }
        }
        poly_bank[oldest] = key;
        poly_time[oldest] = current_time;
        loc_sel = oldest;
      }
    }
  }
}


void readButtons() {


  digitalWrite(inLatchpin, 0);
  digitalWrite(inLatchpin, 1);

  for (int j = 0; j < buttnum; j++) {
    byte oi = button_order[j];
    prevRawButts[oi] = rawButts[oi];
    rawButts[oi] = digitalRead(inDataPin);

    prev_keys[oi] = keys[oi];
    keys[oi] = rawButts[oi];
    fell[oi] = 0;
    rose[oi] = 0;

    if (prev_keys[oi] == 1 && keys[oi] == 0) {
      if (bounceTime[oi] == 0) {
        fell[oi] = 1;
        debButts[oi] = 0;
        bounceTime[oi] = millis();
      }
    }
    if (prev_keys[oi] == 0 && keys[oi] == 1) {
      if (bounceTime[oi] == 0) {
        rose[oi] = 1;
        bounceTime[oi] = millis();
        debButts[oi] = 1;
      }
    }
    if (millis() - bounceTime[oi] > debTime && bounceTime[oi] > 0) {
      bounceTime[oi] = 0;
      debButts[oi] = keys[oi];
    }


    if (oi > 7) {
      if (fell[oi]) {
        if (arp_enable == 0) {
          poly_note_on(oi, 1.0, 1);
        }
        if (arp_enable == 1) {
          int existing = 0;
          byte selected_step = 0;
          for (int i = 0; i < arp_step_len; i++) {
            if (arp_seq[i] == oi) {
              poly_note_off(oi);
              existing = 1;
              selected_step = i;
              // Serial.print("selected_step ");
              // Serial.println(selected_step);
            }
          }
          if (existing == 1 && arp_step_len > 0) {
            //arp_seq[selected_step] = 0;
            //arp_seq[7] = 0;
            for (int i = arp_step_len; i < 8; i++) {
              arp_seq[i] = 0;
            }
            for (int i = selected_step; i < arp_step_len; i++) {
              arp_seq[i] = arp_seq[i + 1];
            }
            arp_step_len--;
            if (arp_step_len < 0) {
              arp_step_len = 0;
            }
          }
          if (existing == 0) {
            arp_seq[arp_step_len] = oi;
            arp_step_len++;
            if (arp_step_len > 16) {
              arp_step_len = 16;
            }
          }
        }
      }

      if (debButts[oi] == 1) {
        if (arp_enable == 0 && hold_en == 0) {
          poly_note_off_select(oi, 1);  //not midi or drum notes
        }
      }

    } else {

      prev_control_buttons[oi] = control_buttons[oi];
      control_buttons[oi] = rawButts[oi];
      if (oi == 7) {
        control_buttons[oi] = digitalRead(2);
      }
      if (prev_control_buttons[oi] == 1 && control_buttons[oi] == 0) {
        if (bounceTime[oi] == 0) {
          fell[oi] = 1;
          debButts[oi] = 0;
          bounceTime[oi] = millis();
        }
      }
      if (prev_control_buttons[oi] == 0 && control_buttons[oi] == 1) {
        if (bounceTime[oi] == 0) {
          rose[oi] = 1;
          bounceTime[oi] = millis();
          debButts[oi] = 1;
        }
      }
      if (millis() - bounceTime[oi] > debTime && bounceTime[oi] > 0) {
        bounceTime[oi] = 0;
        debButts[oi] = control_buttons[oi];
      }
    }

    digitalWrite(inClockPin, 1);
    digitalWrite(inClockPin, 0);
  }

  if (fell[0] == 1) {
    arp_enable = !arp_enable;
    if (arp_enable == 1) {
      hold_en = 0;
      for (int j = 0; j < buttnum; j++) {
        poly_note_off(j);
      }
      for (byte j = 0; j < voices; j++) {
        poly_note_clear(j);
      }
      poly_clean();
    }
    if (arp_enable == 0) {
      for (int j = 0; j < 8; j++) {
        poly_note_off(arp_note_store[j]);
      }
      for (byte j = 0; j < voices; j++) {
        poly_note_clear(j);
      }
      poly_clean();
    }
  }
  if (fell[2] == 1) {
    hold_en = !hold_en;
    if (hold_en == 1) {
      arp_enable = 0;
      for (int j = 0; j < 8; j++) {
        poly_note_off(arp_note_store[j]);
      }
      for (byte j = 0; j < voices; j++) {
        poly_note_clear(j);
      }
      poly_clean();
    }

    if (hold_en == 0) {
      for (int j = 0; j < buttnum; j++) {
        poly_note_off(j);
      }
      for (byte j = 0; j < voices; j++) {
        poly_note_clear(j);
      }
      poly_clean();
    }
  }
  if (fell[7] == 1) {
    //startRecording();
    //Serial.println("rec start");
    rec_ready_timer = current_time;
    rec_mode = 3;
    count_in = millis();
  }

  if (control_buttons[7] == 0) {

    if (current_time - rec_ready_timer > 1000 && rec_ready_timer > 0) {

      Serial.println("rec start");
      startRecording();
      rec_ready_timer = 0;
    }
  }

  if (rose[7] == 1) {
    if (rec_mode == 1) {
      stopRecording();
    }
    if (rec_mode == 3) {
      rec_mode = 0;
    }
  }


  for (int j = 0; j < buttnum; j++) {
    //ordered_buttons[j] = rawButts[button_order[j]];
  }
}



void readPots() {
  mux_select++;

  if (mux_select > 7) {
    raw_pot[16] = smooth(16, analogRead(17));
    raw_pot[17] = smooth(17, analogRead(14));
    // raw_pot[16] = analogRead(17);
    // raw_pot[17] = analogRead(14);
    mux_select = 0;
  }

  raw_pot[mux_select] = smooth(mux_select, analogRead(16));
  raw_pot[mux_select + 8] = smooth(mux_select + 8, analogRead(15));

  for (int i = 0; i < 18; i++) {
    pot[i] = raw_pot[pot_order[i]];
  }


  digitalWrite(9, bitRead(mux_select, 0));
  digitalWrite(10, bitRead(mux_select, 1));
  digitalWrite(11, bitRead(mux_select, 2));
}

#define filterSamples 13                        // filterSamples should  be an odd number, no smaller than 3. Incerease for more smoooothness
#define array_num 20                            //numer of differnt smooths we can take, one for each pot
int sensSmoothArray[array_num][filterSamples];  // array for holding raw sensor values for sensor1

int smooth(int array_sel, int input) {  // "int *sensSmoothArray" passes an array to the function - the asterisk indicates the array name is a pointer
  int j, k, temp, top, bottom;
  long total;
  static int i;
  static int sorted[filterSamples];
  boolean done;

  i = (i + 1) % filterSamples;            // increment counter and roll over if necc. -  % (modulo operator) rolls over variable
  sensSmoothArray[array_sel][i] = input;  // input new data into the oldest slot

  // Serial.print("raw = ");

  for (j = 0; j < filterSamples; j++) {  // transfer data array into anther array for sorting and averaging
    sorted[j] = sensSmoothArray[array_sel][j];
  }

  done = 0;            // flag to know when we're done sorting
  while (done != 1) {  // simple swap sort, sorts numbers from lowest to highest
    done = 1;
    for (j = 0; j < (filterSamples - 1); j++) {
      if (sorted[j] > sorted[j + 1]) {  // numbers are out of order - swap
        temp = sorted[j + 1];
        sorted[j + 1] = sorted[j];
        sorted[j] = temp;
        done = 0;
      }
    }
  }

  // throw out top and bottom 15% of samples - limit to throw out at least one from top and bottom
  //bottom = max(((filterSamples * 15) / 100), 1);
  //top = min((((filterSamples * 85) / 100) + 1), (filterSamples - 1));  // the + 1 is to make up for asymmetry caused by integer rounding
  bottom = 3;
  top = 10;
  k = 0;
  total = 0;
  for (j = bottom; j < top; j++) {
    total += sorted[j];  // total remaining indices
    k++;
  }
  int f = (total / k) / 4;
  f = map(f, 0, 1024, 0, 1000);
  return f;  // divide by number of samples
}

float expo_converter(float input, float max1, float curve) {
  //powf is a more effeinct power for floats
  // powf(base, exponent)
  float ex1 = powf(input, curve) / powf(max1, curve - 1.0);
  return ex1;
}


void set_led(int pixel, float fh, float fs, float fv) {
  byte RedLight;
  byte GreenLight;
  byte BlueLight;

  byte h = fh * 255;
  byte s = fs * 255;
  byte v = fv * max_brightness * 255;

  h = (h * 192) / 256;            // 0..191
  unsigned int i = h / 32;        // We want a value of 0 thru 5
  unsigned int f = (h % 32) * 8;  // 'fractional' part of 'i' 0..248 in jumps

  unsigned int sInv = 255 - s;  // 0 -> 0xff, 0xff -> 0
  unsigned int fInv = 255 - f;  // 0 -> 0xff, 0xff -> 0
  byte pv = v * sInv / 256;     // pv will be in range 0 - 255
  byte qv = v * (256 - s * f / 256) / 256;
  byte tv = v * (256 - s * fInv / 256) / 256;

  switch (i) {
    case 0:
      RedLight = v;
      GreenLight = tv;
      BlueLight = pv;
      break;
    case 1:
      RedLight = qv;
      GreenLight = v;
      BlueLight = pv;
      break;
    case 2:
      RedLight = pv;
      GreenLight = v;
      BlueLight = tv;
      break;
    case 3:
      RedLight = pv;
      GreenLight = qv;
      BlueLight = v;
      break;
    case 4:
      RedLight = tv;
      GreenLight = pv;
      BlueLight = v;
      break;
    case 5:
      RedLight = v;
      GreenLight = pv;
      BlueLight = qv;
      break;
  }
  leds.setPixel(pixel, RedLight, GreenLight, BlueLight);
}
