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
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "AudioConfig.h"

using namespace juce;

struct AudioParameters {
    enum class Parameter : int {
        ModFreqRatio,
        PhaseModAmount,
        AmpEnvAttack,
        AmpEnvDecay,
        AmpEnvSustain,
        AmpEnvRelease,
        AmpDrone,
        ModEnvAttack,
        ModEnvDecay,
        ModEnvSustain,
        ModEnvRelease,
        LFOFreqRatio,
        LFOModAmount,
        GrainLengthMin,
        GrainLengthMax
    };

    static String paramName(Parameter name);

    static ParameterID paramID(Parameter name);

    static ParameterID paramID(String &name);

    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
};
