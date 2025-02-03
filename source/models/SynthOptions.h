#pragma once

#include "../config/AudioParameters.h"

using namespace juce;

using Parameter = AudioParameters::Parameter;

struct SynthOptions
{
public:
    float modFreqRatio = 3.0f / 2.0f;
    float phaseModAmount = 0.25f;

    float lfoModAmount = 0.25f;

    float lfoFreqRatio = 0.15f;

    float ampEnvAttack = 0.01f;
    float ampEnvDecay = 0.1f;
    float ampEnvSustain = 0.8f;
    float ampEnvRelease = 0.1f;

    float modEnvAttack = 0.01f;
    float modEnvDecay = 0.1f;
    float modEnvSustain = 0.8f;
    float modEnvRelease = 0.1f;

    SynthOptions() = default;

    explicit SynthOptions (juce::AudioProcessorValueTreeState& params)
    {
        setState (params);
    }

    void setState (juce::AudioProcessorValueTreeState& params)
    {
        modFreqRatio = params.getParameterAsValue (
                                 AudioParameters::paramName (Parameter::ModFreqRatio))
                           .getValue();
        lfoFreqRatio = params.getParameterAsValue (
                                 AudioParameters::paramName (Parameter::LFOFreqRatio))
                           .getValue();
        lfoModAmount = params.getParameterAsValue (
                                 AudioParameters::paramName (Parameter::LFOModAmount))
                           .getValue();

        phaseModAmount = params.getParameterAsValue (
                                   AudioParameters::paramName (Parameter::PhaseModAmount))
                             .getValue();

        ampEnvAttack = params.getParameterAsValue (
                                 AudioParameters::paramName (Parameter::AmpEnvAttack))
                           .getValue();
        ampEnvDecay = params.getParameterAsValue (
                                AudioParameters::paramName (Parameter::AmpEnvDecay))
                          .getValue();
        ampEnvSustain = params.getParameterAsValue (
                                  AudioParameters::paramName (Parameter::AmpEnvSustain))
                            .getValue();
        ampEnvRelease = params.getParameterAsValue (
                                  AudioParameters::paramName (Parameter::AmpEnvRelease))
                            .getValue();

        modEnvAttack = params.getParameterAsValue (
                                 AudioParameters::paramName (Parameter::ModEnvAttack))
                           .getValue();
        modEnvDecay = params.getParameterAsValue (
                                AudioParameters::paramName (Parameter::ModEnvDecay))
                          .getValue();
        modEnvSustain = params.getParameterAsValue (
                                  AudioParameters::paramName (Parameter::ModEnvSustain))
                            .getValue();
        modEnvRelease = params.getParameterAsValue (
                                  AudioParameters::paramName (Parameter::ModEnvRelease))
                            .getValue();
    }
};