#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

using namespace juce;

class SynthSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int midiNoteNumber) override {
        ignoreUnused(midiNoteNumber);
        return true;
    }

    bool appliesToChannel(int midiChannel) override {
        ignoreUnused(midiChannel);
        return true;
    }
};
