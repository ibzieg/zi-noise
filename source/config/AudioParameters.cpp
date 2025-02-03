/*
 * Copyright 2023, Ian Zieg
 *
 * This file is part of a program called "PatGen"
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "AudioParameters.h"

String AudioParameters::paramName (Parameter name)
{
    String identifier;

    switch (name)
    {
        case Parameter::ModFreqRatio:
            identifier = "MOD_FREQ_RATIO";
            break;
        case Parameter::LFOFreqRatio:
            identifier = "LFO_FREQ_RATIO";
            break;
        case Parameter::PhaseModAmount:
            identifier = "PHASE_MOD_AMOUNT";
            break;
        case Parameter::AmpEnvAttack:
            identifier = "AMP_ENV_ATTACK";
            break;
        case Parameter::AmpEnvDecay:
            identifier = "AMP_ENV_DECAY";
            break;
        case Parameter::AmpEnvSustain:
            identifier = "AMP_ENV_SUSTAIN";
            break;
        case Parameter::AmpEnvRelease:
            identifier = "AMP_ENV_RELEASE";
            break;
        case Parameter::ModEnvAttack:
            identifier = "MOD_ENV_ATTACK";
            break;
        case Parameter::ModEnvDecay:
            identifier = "MOD_ENV_DECAY";
            break;
        case Parameter::ModEnvSustain:
            identifier = "MOD_ENV_SUSTAIN";
            break;
        case Parameter::ModEnvRelease:
            identifier = "MOD_ENV_RELEASE";
            break;
        default:
            identifier = "UNKNOWN";
            break;
    }

    return identifier;
}

ParameterID AudioParameters::paramID (Parameter name)
{
    int versionHint = 1;
    return ParameterID { paramName (name), versionHint };
}

ParameterID AudioParameters::paramID (String& name)
{
    int versionHint = 1;
    return ParameterID { name, versionHint };
}

AudioProcessorValueTreeState::ParameterLayout AudioParameters::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> newParams;

    newParams.push_back (std::make_unique<AudioParameterFloat> (
        AudioParameters::paramID (Parameter::ModFreqRatio), "Mod Ratio", 0.0f, 4.0f, 0.5f));
    newParams.push_back (std::make_unique<AudioParameterFloat> (
        AudioParameters::paramID (Parameter::LFOFreqRatio), "LFO Ratio", 0.01f, 0.25f, 0.15f));
    newParams.push_back (std::make_unique<AudioParameterFloat> (
        AudioParameters::paramID (Parameter::LFOModAmount), "LFO Int", 0.0f, 1.0f, 0.25f));

    newParams.push_back (std::make_unique<AudioParameterFloat> (
        AudioParameters::paramID (Parameter::PhaseModAmount), "PM Amount", 0.0f, 1.0f, 0.25f));
    newParams.push_back (std::make_unique<AudioParameterFloat> (
        AudioParameters::paramID (Parameter::AmpEnvAttack), "Amp Attack", 0.0f, 1.0f, 0.1f));
    newParams.push_back (std::make_unique<AudioParameterFloat> (
        AudioParameters::paramID (Parameter::AmpEnvDecay), "Amp Decay", 0.0f, 1.0f, 0.1f));
    newParams.push_back (std::make_unique<AudioParameterFloat> (
        AudioParameters::paramID (Parameter::AmpEnvSustain), "Amp Sustain", 0.0f, 1.0f, 0.1f));
    newParams.push_back (std::make_unique<AudioParameterFloat> (
        AudioParameters::paramID (Parameter::AmpEnvRelease), "Amp Release", 0.0f, 1.0f, 0.1f));

    newParams.push_back (std::make_unique<AudioParameterFloat> (
        AudioParameters::paramID (Parameter::ModEnvAttack), "Mod Attack", 0.0f, 1.0f, 0.1f));
    newParams.push_back (std::make_unique<AudioParameterFloat> (
        AudioParameters::paramID (Parameter::ModEnvDecay), "Mod Decay", 0.0f, 1.0f, 0.1f));
    newParams.push_back (std::make_unique<AudioParameterFloat> (
        AudioParameters::paramID (Parameter::ModEnvSustain), "Mod Sustain", 0.0f, 1.0f, 0.1f));
    newParams.push_back (std::make_unique<AudioParameterFloat> (
        AudioParameters::paramID (Parameter::ModEnvRelease), "Mod Release", 0.0f, 1.0f, 0.1f));

    return { newParams.begin(), newParams.end() };
}
