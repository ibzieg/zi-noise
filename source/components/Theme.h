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

#include <zi-juce-tools/zi-juce-tools.h>

using namespace juce;

namespace Theme {
    const int PHI = 24;
    const float FPHI = static_cast<float> (PHI);

    static int phiUnits(const int units = 1) { return units * PHI; }

    static float fphi(const float units = 1.0f) { return units * FPHI; }

    const float SLIDER_WIDTH = fphi(4.0f);
    const float SLIDER_HEIGHT = fphi(5.0f);

    const bool DRAW_DEBUG_RECT = false;

    const juce::Colour midiColour = juce::Colour::fromHSL(0.07f, 0.50f, 0.50f, 1.0f);
    const juce::Colour generatorColour = juce::Colour::fromHSL(0.20f, 0.50f, 0.50f, 1.0f);
    const juce::Colour noteColour = juce::Colour::fromHSL(0.35f, 0.50f, 0.50f, 1.0f);
    const juce::Colour velocityColour = juce::Colour::fromHSL(0.50f, 0.50f, 0.50f, 1.0f);
    const juce::Colour mod1Colour = juce::Colour::fromHSL(0.65f, 0.50f, 0.60f, 1.0f);
    const juce::Colour mod2Colour = juce::Colour::fromHSL(0.80f, 0.50f, 0.50f, 1.0f);
    const juce::Colour seedColour = juce::Colour::fromHSL(0.95f, 0.50f, 0.50f, 1.0f);

    class LookMidi : public ZiLookAndFeel {
    public:
        LookMidi() {
            setColour(juce::Slider::thumbColourId, midiColour);
        }
    };

    class LookGenerator : public ZiLookAndFeel {
    public:
        LookGenerator() {
            setColour(juce::Slider::thumbColourId, generatorColour);
        }
    };

    class LookNote : public ZiLookAndFeel {
    public:
        LookNote() {
            setColour(juce::Slider::thumbColourId, noteColour);
        }
    };

    class LookVelocity : public ZiLookAndFeel {
    public:
        LookVelocity() {
            setColour(juce::Slider::thumbColourId, velocityColour);
        }
    };

    class LookMod1 : public ZiLookAndFeel {
    public:
        LookMod1() {
            setColour(juce::Slider::thumbColourId, mod1Colour);
        }
    };

    class LookMod2 : public ZiLookAndFeel {
    public:
        LookMod2() {
            setColour(juce::Slider::thumbColourId, mod2Colour);
        }
    };

    class LookSeed : public ZiLookAndFeel {
    public:
        LookSeed() {
            setColour(juce::Slider::thumbColourId, seedColour);
        }
    };
}
