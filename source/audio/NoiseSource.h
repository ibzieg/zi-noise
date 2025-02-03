#pragma once

#include "../models/SynthOptions.h"
#include <juce_dsp/juce_dsp.h>

//#include "AudioParameters.h"
//#include "ADSREnvelope.h"
//#include "AHDEnvelope.h"
//
//#include "../support/MathSupport.h"

using namespace juce;

class NoiseSource : public AudioSource {
public:
    const static int WAVE_TABLE_SIZE = 16384;
    const double PITCH_SMOOTH_SECONDS = 0.001;
    const double PHASE_MOD_SMOOTH_SECONDS = 0.001;

    const int RANDOM_SEED_VALUE = 31337;

    //    void updateParams(
    //            float ratio,
    //            float speed,
    //            const float start,
    //            float tilt,
    //            int patternType,
    //            int oscType,
    //            const int waveCount);
    //    void updateImage(Image img);

    NoiseSource() : _smoothFreq(0.0), _modOscRatioSmooth(0.0), _lfoOscRatioSmooth(0.0), _phaseModAmountSmooth(0.0),
                    _rng(RANDOM_SEED_VALUE) {}


    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

    void releaseResources() override;

    void startNote(int midiNoteNumber, float velocity);

    void stopNote(float velocity);

    // --------------------------------------------
    void setSynthOptions(SynthOptions &options) {
        synthOptions = options;
    }

private:
    SynthOptions synthOptions;

    /** Size of wave table*/
    int _waveTableSize{WAVE_TABLE_SIZE};
    float _waveTableSin[WAVE_TABLE_SIZE]{};
    float _waveTableSaw[WAVE_TABLE_SIZE]{};
    float _waveTableSqr[WAVE_TABLE_SIZE]{};

    bool _isPreparedToPlay = false;
    double _sampleRate{};

    int _oscType{0};

    SmoothedValue<double> _smoothFreq;
    double _carrierOscFreq{0.0};
    double _carrierOscPhase{0.0}; // phase in radians

    double _modOscFreq{0.0};
    double _modOscPhase{0.0}; // phase in radians
    SmoothedValue<float> _modOscRatioSmooth;

    double _lfoOscFreq{0.0};
    double _lfoOscPhase{0.0}; // phase in radians
    SmoothedValue<float> _lfoOscRatioSmooth;

    SmoothedValue<float> _phaseModAmountSmooth;
    ADSR _modEnv;

    double _velocity{0.0};

    Random _rng;
    float _noiseValue = 0.0;
    float _noiseDelta = 0.0;
    int _samplesUntilNextRandom = 0;

    void resetPhase();

    void updateFreqFactors();

    float *getWaveTable(int type);
};
