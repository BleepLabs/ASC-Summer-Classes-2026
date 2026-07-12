


void startRecording() {
  rec_queue.begin();
  address = 0;
  rec_mode = 1;
}

void stopRecording() {
  if (rec_mode == 1) {
    rec_mode = 0;
    rec_queue.end();
    rec_len = address;
    has_rec = 1;
    Serial.println("rec_stop ");

    while (rec_queue.available() > 0) {
      rec_queue.freeBuffer();
      rec_queue.clear();
    }
    for (int j = 0; j < voices; j++) {
      sampler[j].begin(sample_bank1, rec_len);
    }
  }
}

void continueRecording() {
  int que_size = 128;

  if (rec_queue.available()) {
    memcpy(sample_bank1 + address, rec_queue.readBuffer(), que_size * 2);

    rec_queue.freeBuffer();
    address += que_size;

    if (address > sample_bank_size - que_size) {
      Serial.println("max bank size reached ");
      stopRecording();
    }
  }
}
