#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

//#include "ADSREnvelope.h"
#include "../models/SynthOptions.h"
#include "NoiseSource.h"

using namespace juce;

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    //    void updateAmpEnvParams (const float attack, const float decay, const float sustain, const float release);
    //    void updateModEnvParams (const float attack, const float hold, const float decay);
    //    void updateWaveParams (const float ratio, const float speed, const float waveStart, const float tilt,
    //        const int pattern1Type,
    //        const int osc1Type, const int waveCount);
    //    void updateFilterParams(const float freq, const float res, const float env);
    //    void updateVoiceParams (const float gain);
    //    void updateImage (Image img);

    void setSynthOptions (SynthOptions& options)
    {
        synthOptions = options;
        _wave.setSynthOptions (options);
    }

private:
    bool _isPreparedToPlay { false }; // TODO: add this to PluginProcessor too

    SynthOptions synthOptions;

    //    AHDEnvelope _modAHDEnv;
    ADSR _ampADSREnv;

    AudioBuffer<float> synthBuffer;

    //    float _cutoffFreqHz = audio::FILTER_MAX_FREQ;
    float _filterEnv = 0.0f;

    dsp::Gain<float> _voiceGain;
    dsp::LadderFilter<float> _ladderFilter;
    NoiseSource _wave;
};
