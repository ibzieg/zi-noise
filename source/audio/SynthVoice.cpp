#include "SynthVoice.h"

bool SynthVoice::canPlaySound(SynthesiserSound *sound) {
    return dynamic_cast<juce::SynthesiserSound *> (sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound *sound, int currentPitchWheelPosition) {
    ignoreUnused(sound);
    ignoreUnused(currentPitchWheelPosition);

    _ampADSREnv.noteOn();
    _wave.startNote(midiNoteNumber, velocity);
}

void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    _ampADSREnv.noteOff();
    _wave.stopNote(velocity);

    if (!allowTailOff || !_ampADSREnv.isActive()) {
        clearCurrentNote();
    }
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {
    ignoreUnused(controllerNumber);
    ignoreUnused(newControllerValue);
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {
    ignoreUnused(newPitchWheelValue);
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    _ampADSREnv.setSampleRate(sampleRate);
    //   _modAHDEnv.prepareToPlay(samplesPerBlock, sampleRate);

    dsp::ProcessSpec spec{};
    spec.maximumBlockSize = (juce::uint32) samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = (juce::uint32) outputChannels;

    _wave.prepareToPlay(samplesPerBlock, sampleRate);

    _voiceGain.prepare(spec);
    //   _voiceGain.setGainLinear(0.0f);
    _voiceGain.setGainLinear(0.5f);

    _ladderFilter.prepare(spec);
    //   _ladderFilter.setCutoffFrequencyHz(_cutoffFreqHz);
    _ladderFilter.setResonance(0.7f);
    _ladderFilter.setMode(juce::dsp::LadderFilterMode::LPF24);

    _isPreparedToPlay = true;
}

void SynthVoice::renderNextBlock(AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
    jassert (_isPreparedToPlay);

    if (!isVoiceActive())
        return;

    synthBuffer.setSize(
            outputBuffer.getNumChannels(),
            numSamples,
            false,
            false,
            true);
    synthBuffer.clear();

    dsp::AudioBlock<float> audioBlock{synthBuffer};
    AudioSourceChannelInfo sourceChannelInfo{synthBuffer};

    _ampADSREnv.setParameters(ADSR::Parameters{
            synthOptions.ampEnvAttack,
            synthOptions.ampEnvDecay,
            synthOptions.ampEnvSustain,
            synthOptions.ampEnvRelease});

    try {
        _wave.getNextAudioBlock(sourceChannelInfo);

    } catch (const std::exception &e) {
        std::stringstream stm;
        stm << "Error:  " << e.what();
        Logger::writeToLog(stm.str());
    }

    dsp::AudioBlock<float> block{*sourceChannelInfo.buffer};

    // Per Sample processing
    for (int s = 0; s < block.getNumSamples(); ++s) {
        //       const float envelope = _modAHDEnv.getNextSample();
        //       float cutoffHz = (1.0f + envelope * _filterEnv) * _cutoffFreqHz;
        //       cutoffHz = fmax(cutoffHz, audio::FILTER_MIN_FREQ);
        //       _ladderFilter.setCutoffFrequencyHz(cutoffHz);
        //       for (int ch = 0; ch < block.getNumChannels(); ++ch) {
        //           //            float sample = block.getSample(ch, s);
        //           //            block.setSample(ch, s, sample);
        //       }
    }

    // Post-processing on the entire buffer
    //   _ladderFilter.process(dsp::ProcessContextReplacing<float>(audioBlock));
    if (!synthOptions.ampDrone) {
        _ampADSREnv.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    }

    // TODO use Gain
    _voiceGain.process(dsp::ProcessContextReplacing<float>(audioBlock));

    // TODO are we just applying the same buffer to both channels?
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(
                channel,
                startSample,
                synthBuffer,
                channel,
                0,
                numSamples);
    }

    //   if (!_ampADSREnv.isActive())
    //   {
    //       clearCurrentNote();
    //   }
}
