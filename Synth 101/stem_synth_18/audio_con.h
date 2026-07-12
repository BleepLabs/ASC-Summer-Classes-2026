
// GUItool: begin automatically generated code
AudioRecordQueue rec_queue;
MemSampler sampler[voices];

AudioSynthNoiseWhite white_noise;
AudioSynthWaveformDc dc_lfo;
AudioSynthWaveformDc dc1;
AudioSynthWaveformDc dc_wave;
AudioMixer4 filter_mod_amp[voices], fm_amp[voices];
AudioSynthWaveformModulated wave[2][voices], lfo1;
AudioMixer4 mix_wave[voices],vel_mix[voices];

AudioSynthWaveformModulated drum_wave;

AudioEffectEnvelope env_vca[voices];
AudioEffectEnvelope env_mod[voices];
AudioAnalyzePeak peak_env[voices], rec_peak, out_peak, peak_lfo;
//AudioFilterStateVariable filter[voices];
AudioFilterLadder filter[voices];
AudioMixer4 lfo_offset_mixer;

AudioMixer4 mixer_sub_2;
AudioMixer4 mixer_sub_1;
AudioMixer4 mixer_f_right;
AudioMixer4 mixer_f_left;
AudioEffectFreeverbStereo freeverbs1;
AudioMixer4 mixer_pre;
AudioInputI2S mic_in;
AudioOutputI2S i2s1;
AudioControlSGTL5000 sgtl5000_1;
AudioEffectFreeverbStereo reverb;
AudioConnection yup[] = {
#define vs 0
  { filter[vs], 0, mixer_sub_1, vs },  //

  { wave[0][vs], 0, mix_wave[vs], 0 },
  { wave[1][vs], 0, mix_wave[vs], 1 },
  { sampler[vs], 0, mix_wave[vs], 3 },
  { dc_wave, 0, wave[0][vs], 1 },
  { dc_wave, 0, wave[1][vs], 1 },
  { white_noise, 0, mix_wave[vs], 2 },
  { mix_wave[vs], env_vca[vs] },
  { env_vca[vs], 0, vel_mix[vs], 0 },
  { vel_mix[vs], 0, filter[vs], 0 },
  { dc1, env_mod[vs] },
  { env_mod[vs], 0, peak_env[vs], 0 },
  { env_mod[vs], 0, fm_amp[vs], 0 },
  { env_mod[vs], 0, filter_mod_amp[vs], 0 },
  { fm_amp[vs], 0, wave[0][vs], 0 },
  { fm_amp[vs], 0, wave[1][vs], 0 },
  { lfo_offset_mixer, 0, fm_amp[vs], 1 },
  { lfo_offset_mixer, 0, filter_mod_amp[vs], 1 },
  { filter_mod_amp[vs], 0, filter[vs], 1 },

#define vs 1
  { filter[vs], 0, mixer_sub_1, vs },  //
  
  { wave[0][vs], 0, mix_wave[vs], 0 },
  { wave[1][vs], 0, mix_wave[vs], 1 },
  { sampler[vs], 0, mix_wave[vs], 3 },
  { dc_wave, 0, wave[0][vs], 1 },
  { dc_wave, 0, wave[1][vs], 1 },
  { white_noise, 0, mix_wave[vs], 2 },
  { mix_wave[vs], env_vca[vs] },
  { env_vca[vs], 0, vel_mix[vs], 0 },
  { vel_mix[vs], 0, filter[vs], 0 },
  { dc1, env_mod[vs] },
  { env_mod[vs], 0, peak_env[vs], 0 },
  { env_mod[vs], 0, fm_amp[vs], 0 },
  { env_mod[vs], 0, filter_mod_amp[vs], 0 },
  { fm_amp[vs], 0, wave[0][vs], 0 },
  { fm_amp[vs], 0, wave[1][vs], 0 },
  { lfo_offset_mixer, 0, fm_amp[vs], 1 },
  { lfo_offset_mixer, 0, filter_mod_amp[vs], 1 },
  { filter_mod_amp[vs], 0, filter[vs], 1 },

#define vs 2

  { filter[vs], 0, mixer_sub_1, vs },
    { wave[0][vs], 0, mix_wave[vs], 0 },
  { wave[1][vs], 0, mix_wave[vs], 1 },
  { sampler[vs], 0, mix_wave[vs], 3 },
  { dc_wave, 0, wave[0][vs], 1 },
  { dc_wave, 0, wave[1][vs], 1 },
  { white_noise, 0, mix_wave[vs], 2 },
  { mix_wave[vs], env_vca[vs] },
  { env_vca[vs], 0, vel_mix[vs], 0 },
  { vel_mix[vs], 0, filter[vs], 0 },
  { dc1, env_mod[vs] },
  { env_mod[vs], 0, peak_env[vs], 0 },
  { env_mod[vs], 0, fm_amp[vs], 0 },
  { env_mod[vs], 0, filter_mod_amp[vs], 0 },
  { fm_amp[vs], 0, wave[0][vs], 0 },
  { fm_amp[vs], 0, wave[1][vs], 0 },
  { lfo_offset_mixer, 0, fm_amp[vs], 1 },
  { lfo_offset_mixer, 0, filter_mod_amp[vs], 1 },
  { filter_mod_amp[vs], 0, filter[vs], 1 },
#define vs 3

  { filter[vs], 0, mixer_sub_1, vs },

   { wave[0][vs], 0, mix_wave[vs], 0 },
  { wave[1][vs], 0, mix_wave[vs], 1 },
  { sampler[vs], 0, mix_wave[vs], 3 },
  { dc_wave, 0, wave[0][vs], 1 },
  { dc_wave, 0, wave[1][vs], 1 },
  { white_noise, 0, mix_wave[vs], 2 },
  { mix_wave[vs], env_vca[vs] },
  { env_vca[vs], 0, vel_mix[vs], 0 },
  { vel_mix[vs], 0, filter[vs], 0 },
  { dc1, env_mod[vs] },
  { env_mod[vs], 0, peak_env[vs], 0 },
  { env_mod[vs], 0, fm_amp[vs], 0 },
  { env_mod[vs], 0, filter_mod_amp[vs], 0 },
  { fm_amp[vs], 0, wave[0][vs], 0 },
  { fm_amp[vs], 0, wave[1][vs], 0 },
  { lfo_offset_mixer, 0, fm_amp[vs], 1 },
  { lfo_offset_mixer, 0, filter_mod_amp[vs], 1 },
  { filter_mod_amp[vs], 0, filter[vs], 1 },
#define vs 4

  { filter[vs], 0, mixer_sub_2, vs - 4 },

    { wave[0][vs], 0, mix_wave[vs], 0 },
  { wave[1][vs], 0, mix_wave[vs], 1 },
  { sampler[vs], 0, mix_wave[vs], 3 },
  { dc_wave, 0, wave[0][vs], 1 },
  { dc_wave, 0, wave[1][vs], 1 },
  { white_noise, 0, mix_wave[vs], 2 },
  { mix_wave[vs], env_vca[vs] },
  { env_vca[vs], 0, vel_mix[vs], 0 },
  { vel_mix[vs], 0, filter[vs], 0 },
  { dc1, env_mod[vs] },
  { env_mod[vs], 0, peak_env[vs], 0 },
  { env_mod[vs], 0, fm_amp[vs], 0 },
  { env_mod[vs], 0, filter_mod_amp[vs], 0 },
  { fm_amp[vs], 0, wave[0][vs], 0 },
  { fm_amp[vs], 0, wave[1][vs], 0 },
  { lfo_offset_mixer, 0, fm_amp[vs], 1 },
  { lfo_offset_mixer, 0, filter_mod_amp[vs], 1 },
  { filter_mod_amp[vs], 0, filter[vs], 1 },
#define vs 5
  { filter[vs], 0, mixer_sub_2, vs - 4 },

  { wave[0][vs], 0, mix_wave[vs], 0 },
  { wave[1][vs], 0, mix_wave[vs], 1 },
  { sampler[vs], 0, mix_wave[vs], 3 },
  { dc_wave, 0, wave[0][vs], 1 },
  { dc_wave, 0, wave[1][vs], 1 },
  { white_noise, 0, mix_wave[vs], 2 },
  { mix_wave[vs], env_vca[vs] },
  { env_vca[vs], 0, vel_mix[vs], 0 },
  { vel_mix[vs], 0, filter[vs], 0 },
  { dc1, env_mod[vs] },
  { env_mod[vs], 0, peak_env[vs], 0 },
  { env_mod[vs], 0, fm_amp[vs], 0 },
  { env_mod[vs], 0, filter_mod_amp[vs], 0 },
  { fm_amp[vs], 0, wave[0][vs], 0 },
  { fm_amp[vs], 0, wave[1][vs], 0 },
  { lfo_offset_mixer, 0, fm_amp[vs], 1 },
  { lfo_offset_mixer, 0, filter_mod_amp[vs], 1 },
  { filter_mod_amp[vs], 0, filter[vs], 1 },

#define vs 6
  { filter[vs], 0, mixer_sub_2, vs - 4 },
  { wave[0][vs], 0, mix_wave[vs], 0 },
  { wave[1][vs], 0, mix_wave[vs], 1 },
  { sampler[vs], 0, mix_wave[vs], 3 },
  { dc_wave, 0, wave[0][vs], 1 },
  { dc_wave, 0, wave[1][vs], 1 },
  { white_noise, 0, mix_wave[vs], 2 },
  { mix_wave[vs], env_vca[vs] },
  { env_vca[vs], 0, vel_mix[vs], 0 },
  { vel_mix[vs], 0, filter[vs], 0 },
  { dc1, env_mod[vs] },
  { env_mod[vs], 0, peak_env[vs], 0 },
  { env_mod[vs], 0, fm_amp[vs], 0 },
  { env_mod[vs], 0, filter_mod_amp[vs], 0 },
  { fm_amp[vs], 0, wave[0][vs], 0 },
  { fm_amp[vs], 0, wave[1][vs], 0 },
  { lfo_offset_mixer, 0, fm_amp[vs], 1 },
  { lfo_offset_mixer, 0, filter_mod_amp[vs], 1 },
  { filter_mod_amp[vs], 0, filter[vs], 1 },
#define vs 7
  { filter[vs], 0, mixer_sub_2, vs - 4 },

 { wave[0][vs], 0, mix_wave[vs], 0 },
  { wave[1][vs], 0, mix_wave[vs], 1 },
  { sampler[vs], 0, mix_wave[vs], 3 },
  { dc_wave, 0, wave[0][vs], 1 },
  { dc_wave, 0, wave[1][vs], 1 },
  { white_noise, 0, mix_wave[vs], 2 },
  { mix_wave[vs], env_vca[vs] },
  { env_vca[vs], 0, vel_mix[vs], 0 },

  { vel_mix[vs], 0, filter[vs], 0 },
  
  { dc1, env_mod[vs] },
  { env_mod[vs], 0, peak_env[vs], 0 },
  { env_mod[vs], 0, fm_amp[vs], 0 },
  { env_mod[vs], 0, filter_mod_amp[vs], 0 },
  { fm_amp[vs], 0, wave[0][vs], 0 },
  { fm_amp[vs], 0, wave[1][vs], 0 },
  { lfo_offset_mixer, 0, fm_amp[vs], 1 },
  { lfo_offset_mixer, 0, filter_mod_amp[vs], 1 },
  { filter_mod_amp[vs], 0, filter[vs], 1 },


  { lfo1, 0, lfo_offset_mixer, 0 },
  { dc_lfo, 0, lfo_offset_mixer, 1 },
  { lfo_offset_mixer, 0, peak_lfo, 0 },

  { mixer_sub_1, 0, mixer_pre, 0 },
  { mixer_sub_2, 0, mixer_pre, 1 },
  { mixer_pre, 0, reverb, 0 },

  { reverb, 0, mixer_f_left, 0 },
  { reverb, 1, mixer_f_right, 0 },
  { mixer_pre, 0, mixer_f_left, 1 },
  { mixer_pre, 0, mixer_f_right, 1 },
  { mic_in, 0, mixer_f_left, 2 },
  { mic_in, 0, mixer_f_right, 2 },

  { mixer_f_left, 0, i2s1, 0 },
  { mixer_f_right, 0, i2s1, 1 },
  { mixer_f_right, 0, out_peak, 0 },
  { mic_in, 0, rec_queue, 0 },
  { mic_in, 0, rec_peak, 0 },


};