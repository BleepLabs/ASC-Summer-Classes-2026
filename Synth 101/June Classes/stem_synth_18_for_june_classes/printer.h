#include "wiring.h"
void printer() {
  if (current_time - prev_time[0] > 100) {
    prev_time[0] = current_time;
    byte ref1 = 0;

    //Serial.println(has_rec);

    if (out_peak.available()) {
      op = out_peak.read();
    }


    //Serial.println(print1);
    // Serial.println();
    if (0) {  //envs

      Serial.print(0);
      Serial.print(" ");
      Serial.print(255);
      Serial.print(" ");
      for (byte j = 0; j < voices; j++) {
        Serial.print(peak_env_read[j]);
        Serial.print(" ");
      }
      Serial.println();
    }

    if (0) {  //out_peak
      float out_p;
      // if (out_peak.available()) {
      //   out_p = out_peak.read();
      // }
      Serial.print(0);
      Serial.print(" ");
      Serial.print(100);
      Serial.print(" ");
      Serial.print(out_p * 100);

      Serial.println();
    }

    if (0) {  //drum
      Serial.print(0);
      Serial.print(" ");
      Serial.print(4095);
      Serial.print(" ");
      Serial.println(raw_piezo);
    }
    if (0) {  //poly
      for (byte j = 0; j < 8; j++) {
        Serial.print(base_note[j]);
        Serial.print(" ");
      }
      Serial.println();
      for (byte j = 0; j < 8; j++) {
        Serial.print(peak_env_read[j]);
        Serial.print(" ");
      }
      Serial.println();
    }



    if (0) {  //poly
      for (byte j = 0; j < voices; j++) {
        Serial.print(poly_bank[j]);
        Serial.print(" ");
      }
      Serial.println();
      for (byte j = 0; j < voices; j++) {
        Serial.print(voice_status[j]);
        Serial.print(" ");
      }
      Serial.println();
      Serial.println();
    }

    if (0) {
      Serial.print(arp_step_len);
      Serial.print("   ");
      for (byte j = 0; j < 8; j++) {
        Serial.print(arp_seq[j]);
        Serial.print(" ");
      }
      Serial.println();
    }


    if (0) {  ///buttons
      for (byte j = 0; j < buttnum; j++) {
        byte br = rawButts[j];
        if (br == 0) {
          Serial.print(j);
          Serial.print(" ");

        } else {
          Serial.print("_");
          Serial.print(" ");
        }
        if ((j + 1) % 8 == 0) {
          Serial.println();
        }
      }
      Serial.println();
      Serial.println();
    }

    if (0) {  ///buttons
      for (byte j = 0; j < buttnum; j++) {
        Serial.print(j);
        Serial.print(" ");
        if (debButts[j] == 1) {
          Serial.print("_");
        } else {
          Serial.print("R");
        }
        Serial.print("  ");
      }

      Serial.println();
    }
    if (0) {  ///buttons
      for (byte j = 0; j < buttnum; j++) {
        Serial.print(j);
        Serial.print(" ");
        if (ordered_buttons[j] == 1) {
          Serial.print("_");
        } else {
          Serial.print("*");
        }
        Serial.print("  ");
      }

      Serial.println();
      Serial.println();
      Serial.println();
    }


    if (1) {  //pots
      // Serial.print(0);
      // Serial.print(" ");
      // Serial.print(pot_max);
      // Serial.print(" ");
      for (byte j = 0; j < 5; j++) {
        Serial.print(j);
        Serial.print(" ");
        Serial.print(pot[j]);
        Serial.print("\t");
      }
      Serial.println();

      for (byte j = 5; j < 9; j++) {
        Serial.print(j);
        Serial.print(" ");
        Serial.print(pot[j]);
        Serial.print("\t");
      }
      Serial.println();

      for (byte j = 9; j < 13; j++) {
        Serial.print(j);
        Serial.print(" ");
        Serial.print(pot[j]);
        Serial.print("\t");
      }
      Serial.println();

      for (byte j = 13; j < 18; j++) {
        Serial.print(j);
        Serial.print(" ");
        Serial.print(pot[j]);
        Serial.print("\t");
      }
      Serial.println();
      Serial.println();
    }
  }

  if (current_time - prev_time[7] > 1000 && 1) {
    prev_time[7] = current_time;
    if (AudioMemoryUsageMax() > mem_size - 2) {
      Serial.println(" OVER MEM");
    }
    if (AudioProcessorUsageMax() > 90) {
      Serial.println(" OVER %");
    }


    // Serial.print(AudioProcessorUsageMax());
    // Serial.print("%  ");
    // Serial.print(AudioMemoryUsageMax());
    // Serial.println();



    AudioProcessorUsageMaxReset();
    AudioMemoryUsageMaxReset();
  }
}