#pragma once

#include "BinaryData.h"
#include "PluginProcessor.h"

#include "./components/Theme.h"
#include "melatonin_inspector/melatonin_inspector.h"

#include <zi-juce-tools/zi-juce-tools.h>

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void setupSlider (
        const juce::String& parameterID,
        std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>& sliderAttachment,
        LabelSlider& slider,
        Slider::SliderStyle sliderStyle,
        LookAndFeel_V4* lookAndFeel);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect the UI" };

    // -------------------------------------------------------------------------
    Theme::LookMidi _lookMidi;
    Theme::LookNote _lookNote;

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

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _modFreqRatioSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _lfoFreqRatioSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _lfoModAmountSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _phaseModAmountSliderAttachment;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _ampEnvAttackSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _ampEnvDecaySliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _ampEnvSustainSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _ampEnvReleaseSliderAttachment;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _modEnvAttackSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _modEnvDecaySliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _modEnvSustainSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> _modEnvReleaseSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
