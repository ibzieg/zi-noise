#pragma once

#include "BinaryData.h"
#include "PluginProcessor.h"

#include "./components/Theme.h"
#include "melatonin_inspector/melatonin_inspector.h"

#include <zi-juce-tools/zi-juce-tools.h>

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor {
public:
    explicit PluginEditor(PluginProcessor &);

    ~PluginEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;

private:
    void setupSlider(
            const juce::String &parameterID,
            LabelSlider &slider,
            Slider::SliderStyle sliderStyle,
            LookAndFeel_V4 *lookAndFeel);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor &processorRef;
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton{"Inspect the UI"};


    // Buttons -----------------------------------------------------------------
    ToggleButton _ampDroneButton{"Drone"};
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> _ampDroneButtonAttachment;

    // -------------------------------------------------------------------------
    Theme::LookMidi _lookMidi;
    Theme::LookNote _lookNote;
    Theme::LookSeed _lookSeed;

    LabelSlider _modFreqRatioSlider;
    LabelSlider _lfoFreqRatioSlider;
    LabelSlider _lfoModAmountSlider;
    LabelSlider _phaseModAmountSlider;

    LabelSlider _ampEnvAttackSlider;
    LabelSlider _ampEnvDecaySlider;
    LabelSlider _ampEnvSustainSlider;
    LabelSlider _ampEnvReleaseSlider;

    LabelSlider _modEnvAttackSlider;
    LabelSlider _modEnvDecaySlider;
    LabelSlider _modEnvSustainSlider;
    LabelSlider _modEnvReleaseSlider;

    LabelSlider _grainLengthMinSlider;
    LabelSlider _grainLengthMaxSlider;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
