#include "core_pins.h"
#include "wiring.h"

float led_test1 = 0;

void loop() {
  current_time = millis();
  printer();




  readButtons();
  poly_clean();
  readPots();

  if (rec_mode == 1) {
    continueRecording();
  }



  arp_note_len = seq_rate / 1000 / 4;
  if (arp_enable == 1) {
    for (int j = 0; j < arp_step_len; j++) {
      if (current_time - arp_note_time[j] > arp_note_len) {
        poly_note_off(arp_note_store[j]);
      }
    }
  }

  if (1) {  ///////////////////////////////////////////////////////////////////controls

    // keyb_led_offset_a = map(pot[9],0,1000,-16,16);
    // keyb_led_offset_b = map(pot[10],0,1000,-16,16);
    for (int j = 0; j < voices; j++) {
      octave = map(pot[5], 0, pot_max, 1, 6) * 12;
      int detune_size = 400;
      float detune_range = .125;



      in_tune = 0;


      if (pot[0] >= 500 && pot[0] < 500 + detune_size) {
        float top = (detune_range * 100.0) + 100.0;
        pitchpot = map(pot[0], 500, 500 + detune_size, 100, top) / 100.0;
      }

      if (pot[0] >= 500 + detune_size) {
        pitchpot = 2.0;
        in_tune = 1;
      }

      if (pot[0] > 500 - detune_size && pot[0] <= 500) {
        float bottom = 100.0 - (detune_range * 100.0);

        pitchpot = map(pot[0], 500 - detune_size, 500, bottom, 100) / 100.0;
      }

      if (pot[0] < 500 - detune_size) {
        pitchpot = .5;
        in_tune = 1;
      }

      float real_tune = 1.0;
      if (pitchpot < real_tune + .02 && pitchpot > real_tune - .02) {
        in_tune = 1;
      }


      float f1 = chromatic[base_note[j] + octave + note_offset];
      float freq2 = f1;
      float freq1 = f1 * pitchpot;
      wave[0][j].frequency(freq1);
      wave[1][j].frequency(freq2);
      // float sampling_freq = map(pot[0], 0, pot_max, 1000 - detune_range, 1000 + detune_range) / 100.0;
      // if (sampling_freq < 0) {
      //   sampler[j].sample_reverse(1);
      //   sampling_freq = abs(sampling_freq);
      // } else {
      //   sampler[j].sample_reverse(0);
      // }
      envt = ((peak_env_read[j] / 255.0) * (fm1 / 1000.0)) + 1;
      if (envt > .9 && envt < 1.1) {
        envt = 1;
      }
      if (j == 0) {
        print1 = envt;
      }
      lfot = (lfo_peak_read_raw * lfo_fm) + 1;
      float sf = (freq2 / 200.0) * (lfot)*envt;
      sampler[j].frequency(sf);


      fm1 = map(pot[15], 0, pot_max, -1100, 1100);
      if (fm1 > 0 && fm1 < 100) {
        fmf = 0;
      }
      if (fm1 > 100) {
        fmf = (fm1 - 100) / 1000.0;
      }
      if (fm1 < 0 && fm1 > -100) {
        fmf = 0;
      }
      if (fm1 < -100) {
        fmf = (fm1 + 100) / 1000.0;
      }

      dc1.amplitude(1);
      fm_amp[j].gain(0, fmf);
      wave[0][j].frequencyModulation(1);
      wave[1][j].frequencyModulation(1);

      expp2 = expo_converter(pot[2], pot_max, .5) / pot_max;
      if (shape_sel <= 4 || has_rec == 0) {
        mix_wave[j].gain(0, 1 / 9.0);
        mix_wave[j].gain(1, pot[2] / pot_max * 1 / 9.0);
        mix_wave[j].gain(2, pot[6] / pot_max * 1 / 9.0);
        mix_wave[j].gain(3, 0);
      }
      if (shape_sel > 4 && has_rec == 1) {
        mix_wave[j].gain(0, 0);
        mix_wave[j].gain(1, pot[2] / pot_max * 1 / 9.0);
        mix_wave[j].gain(2, pot[6] / pot_max * 1 / 9.0);
        mix_wave[j].gain(3, 1 / 9.0);
      }

      //mix_wave[j].gain(3, 0);

      float at = map(pot[13], 0, pot_max, 1, 3000);
      at = expo_converter(at, 3000, 2);
      float rel = map(pot[14], 0, pot_max, 10, 3000);
      rel = expo_converter(rel, 3000, 2);
      env_vca[j].attack(at);
      env_vca[j].decay(at / 8);
      env_vca[j].sustain(.9);
      env_vca[j].release(rel);

      env_mod[j].attack(at);
      env_mod[j].decay(at / 8);
      env_mod[j].sustain(.9);
      env_mod[j].release(rel);

      filter_mod = map(pot[16], 0, pot_max, -1100, 1100);
      filter_mod_final;
      if (filter_mod > 0 && filter_mod < 100) {
        filter_mod_final = 0;
      }
      if (filter_mod > 100) {
        filter_mod_final = (filter_mod - 100) / 1000.0;
      }
      if (filter_mod < 0 && filter_mod > -100) {
        filter_mod_final = 0;
      }
      if (filter_mod < -100) {
        filter_mod_final = (filter_mod + 100) / 1000.0;
      }



      filter_mod_amp[j].gain(0, filter_mod_final);
      ff = map(pot[3], 0, pot_max, 50, 3200) / 100.0;
      fff = freq1 * ff;
      // if (fff > 2500) {
      //   fff = 2500;
      // }
      filter[j].frequency(fff);
      res = map(pot[7], 0, pot_max, 0, 99) / 100.0;
      filter[j].resonance(res);
      filter[j].octaveControl(3);

      prev_lfo_shape_sel = lfo_shape_sel;
      lfo_shape_sel = map(pot[10], 0, pot_max, 0, 4);

      if (prev_lfo_shape_sel != lfo_shape_sel) {
        for (int j = 0; j < voices; j++) {
          if (lfo_shape_sel == 0) {
            lfo1.begin(WAVEFORM_SINE);
          }
          if (lfo_shape_sel == 1) {
            lfo1.begin(WAVEFORM_SAWTOOTH_REVERSE);
          }
          if (lfo_shape_sel == 2) {
            lfo1.begin(WAVEFORM_SAWTOOTH);
          }
          if (lfo_shape_sel == 3) {
            lfo1.begin(WAVEFORM_SQUARE);
          }
          if (lfo_shape_sel == 4) {
            lfo1.begin(WAVEFORM_SAMPLE_HOLD);
          }
        }
      }


      lfo_freq = map(pot[9], 0, pot_max, 100, 100000);
      lfo_freq = expo_converter(lfo_freq, 100000, 5) / 1000.0;
      lfo1.frequency(lfo_freq);

      lfo_fm = map(pot[11], 0, pot_max, -100, 1000) / 1000.0;
      if (lfo_fm < 0) {
        lfo_fm = 0;
      }
      fm_amp[j].gain(1, lfo_fm);

      lfo_filter = map(pot[12], 0, pot_max, -100, 1000) / 1000.0;
      if (lfo_filter < 0) {
        lfo_filter = 0;
      }
      filter_mod_amp[j].gain(1, lfo_filter);
    }

    prev_shape_sel = shape_sel;
    shape_sel = map(pot[1], 0, pot_max, 0, 6);

    if (prev_shape_sel != shape_sel) {
      for (int j = 0; j < voices; j++) {
        if (shape_sel == 0) {
          wave[0][j].begin(WAVEFORM_SINE);
          wave[1][j].begin(WAVEFORM_SINE);
        }
        if (shape_sel == 1) {
          wave[0][j].begin(WAVEFORM_TRIANGLE);
          wave[1][j].begin(WAVEFORM_TRIANGLE);
        }
        if (shape_sel == 2) {
          wave[0][j].begin(WAVEFORM_BANDLIMIT_SAWTOOTH);
          wave[1][j].begin(WAVEFORM_BANDLIMIT_SAWTOOTH);
        }
        if (shape_sel == 3) {
          wave[0][j].begin(WAVEFORM_BANDLIMIT_SQUARE);
          wave[1][j].begin(WAVEFORM_BANDLIMIT_SQUARE);
        }
        if (shape_sel == 4) {
          wave[0][j].begin(WAVEFORM_BANDLIMIT_PULSE);
          wave[1][j].begin(WAVEFORM_BANDLIMIT_PULSE);
        }
        if (shape_sel >= 5) {
          if (has_rec == 1) {
            sampler[j].begin(sample_bank1, rec_len);
          } else {
            wave[0][j].begin(WAVEFORM_BANDLIMIT_PULSE);
          }
          wave[1][j].begin(WAVEFORM_SINE);
        }
      }
    }
  }

  float vol = pot[4] / 1000.0;
  if (hp_follow < vol) {
    hp_follow += .004;
  }
  if (hp_follow > vol) {
    hp_follow -= .004;
  }
  sgtl5000_1.volume(hp_follow);

  reverb_wet = pot[8] / 1000.0;
  reverb_dry = map(pot[8], 0, 1000, 1000, 400) / 1000.0;
  mixer_f_left.gain(0, reverb_wet);
  mixer_f_right.gain(0, reverb_wet);
  mixer_f_left.gain(1, reverb_dry);
  mixer_f_right.gain(1, reverb_dry);
  mixer_pre.gain(2, reverb_wet / 4.0);
  mixer_pre.gain(3, reverb_wet / 3.0);

  /////////////////////////////////////////////////////////////////////////////// control over

  if (current_time - prev_time[4] > 2) {
    prev_time[4] = current_time;
    prev_bpm = bpm;
    bpm = (pot[17]) + 40;
    seq_rate = 60000000.0 / float(bpm);
    if (prev_bpm != bpm) {
      myTimer.update(seq_rate);
    }
  }

  if (1) {  //peaks
    for (byte j = 0; j < voices; j++) {

      if (env_vca[j].isActive() == 0) {
        poly_note_clear(j);
      }

      if (peak_env[j].available()) {
        prev_peak_env_read[j] = peak_env_read[j];
        peak_env_read[j] = peak_env[j].raw() * 255;
        //peak_env_read[j] = peak_env[j].readMin() * 255;
      }
    }
  }


  if (current_time - prev_time[2] > 20 && 1) {  //////////////////////////// LL EE DD
    prev_time[2] = current_time;



    for (int j = 0; j < numled; j++) {
      set_led(j, 0, 0, 0);
    }

    if (rec_mode == 0) {
      if (in_tune == 0) {
        set_led(0, .7, (op * 3.0) + .2, .3);
      }
      if (in_tune == 1) {
        set_led(0, .7, .9, .5 + (op * 2.0));
      }
    }

    if (rec_mode == 3) {
      set_led(0, .15, .85, .1);
      byte blen = 35;

      if (millis() - count_in < blen) {
        set_led(0, .15, .8, .2);
      }
      if (millis() - count_in > 250 && millis() - count_in < 250 + blen) {
        set_led(0, .15, .8, .2);
      }
      if (millis() - count_in > 500 && millis() - count_in < 500 + blen) {
        set_led(0, .15, .7, .2);
      }
      if (millis() - count_in > 750 && millis() - count_in < 750 + blen) {
        set_led(0, .15, .6, .2);
      }
    }

    if (rec_mode == 1) {
      if (rec_peak.available()) {
        rp_led = rec_peak.read();
      }
      set_led(0, 0, .85, .1 + (rp_led / 2));
    }
    set_led(5, .2, .5, float(arp_enable) * .2);
    set_led(7, .2, .5, float(hold_en) * .2);

    if (arp_enable == 1) {
      for (byte j = 0; j < 16; j++) {
        int lt = key_led_order[arp_seq[j] + keyb_led_offset_a] + keyb_led_offset_b;
        set_led(lt, .66, 1, .05);
      }
    }

    for (byte j = 0; j < voices; j++) {
      prev_base_note[j] = base_note[j];
      int lt = key_led_order[base_note[j] + keyb_led_offset_a] + keyb_led_offset_b;
      float b = (peak_env_read[j] / 255.0);
      set_led(lt, .75, .9, b * .5);
    }

    allenv = (peak_env_read[newest_voice] / 255.0) + .25;
    float ledenv = (allenv + .05) * (fmf * 255.0) * max_brightness;

    if (fmf > 0) {
      leds.setPixel(3, 0, 0, ledenv);
    }
    if (fmf < 0) {
      leds.setPixel(3, 0, 0, (ledenv * -1.0) + (max_brightness * 1.5));
    }

    //float env_mod_filter;

    // if (abs(filter_mod_final * 1000) < 100) {
    //   env_mod_filter = 0;
    // } else {
    //   env_mod_filter = (peak_env_read[newest_voice] / 255.0) + .25;
    // }
    // //set_led(6, .5, .9, (env_mod_filter * .2));

    //   leds.setPixel(6, 0, (env_mod_filter + .05) * 255 * max_brightness, 0);


    ledenv = (allenv + .05) * (filter_mod_final * 255.0) * max_brightness;

    if (filter_mod_final > 0) {
      leds.setPixel(4, 0, ledenv, 0);
    }
    if (filter_mod_final < 0) {
      leds.setPixel(4, 0, (ledenv * -1.0) + (max_brightness), 0);
    }


    float lfo_fm_led;
    float lfo_filter_led;
    if (peak_lfo.available()) {
      lfo_peak_read_raw = peak_lfo.read();
      lfop = (lfo_peak_read_raw)*.2;
    }
    float led_byte = (lfop + .05) * 255.0 * max_brightness;
    if (lfo_fm > 0) {
      //      set_led(1, 0, 1, (lfop + .1));
      leds.setPixel(1, 0, 0, led_byte * 1.25);

    } else {
      set_led(1, 0, 0, 0);
    }


    if (lfo_filter > 0) {
      leds.setPixel(2, 0, led_byte, 0);

      //      set_led(3, .5, 1, (lfop + .1));
    } else {
      set_led(2, 0, 0, 0);
    }


    leds.show();
  }

  int type, note, velocity, channel, d1, d2;
  if (MIDI.read()) {  // Is there a MIDI message incoming ?
    byte type = MIDI.getType();
    // Serial.println("MIDI ");
    // Serial.println(type);
    switch (type) {
      case midi::NoteOn:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        if (velocity > 0) {
          poly_note_on(note + midi_note_offset, 1.0, 3);
        } else {
          poly_note_off(note + midi_note_offset);
        }
        break;
      case midi::NoteOff:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        poly_note_off(note + midi_note_offset);
        break;
      default:
        d1 = MIDI.getData1();
        d2 = MIDI.getData2();
        //Serial.println(String("Message, type=") + type + ", data = " + d1 + " " + d2);
    }
  }

  if (current_time - prev_time[1] > 0 && 1) {  //piez
    prev_time[1] = current_time;
    prev_raw_piezo = raw_piezo;
    raw_piezo = analogRead(22);
    thresh = 500;
    if (prev_raw_piezo < thresh && raw_piezo > thresh && drum_note_on == 0) {
      drum_timer = current_time;
      drum_note = last_note;
      if (drum_note <= 0) {
        drum_note = 10;
      }
      drum_note_on = 1;
      int t1 = map(raw_piezo - thresh, 0, 4095 - thresh, 100, 1000);
      float drum_vel = t1 / 1000.0;
      poly_note_on(drum_note, drum_vel, 2);

      Serial.println(raw_piezo);
      Serial.println(drum_vel, 4);
      Serial.println();
    }
  }

  if (drum_timer > 0 && current_time - drum_timer > 30) {
    poly_note_off_select(drum_note, 2);
    drum_note_on = 0;
    drum_timer = 0;
  }
}