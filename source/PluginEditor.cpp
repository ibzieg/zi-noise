#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };

    setupSlider (AudioParameters::paramName (Parameter::ModFreqRatio), _modFreqRatioSliderAttachment, _modFreqRatioSlider, Slider::SliderStyle::RotaryHorizontalVerticalDrag, &_lookMidi);
    setupSlider (AudioParameters::paramName (Parameter::LFOFreqRatio), _lfoFreqRatioSliderAttachment, _lfoFreqRatioSlider, Slider::SliderStyle::RotaryHorizontalVerticalDrag, &_lookNote);
    setupSlider (AudioParameters::paramName (Parameter::LFOModAmount), _lfoModAmountSliderAttachment, _lfoModAmountSlider, Slider::SliderStyle::RotaryHorizontalVerticalDrag, &_lookNote);
    setupSlider (AudioParameters::paramName (Parameter::PhaseModAmount), _phaseModAmountSliderAttachment, _phaseModAmountSlider, Slider::SliderStyle::RotaryHorizontalVerticalDrag, &_lookMidi);

    setupSlider (AudioParameters::paramName (Parameter::AmpEnvAttack), _ampEnvAttackSliderAttachment, _ampEnvAttackSlider, Slider::SliderStyle::LinearVertical, &_lookMidi);
    setupSlider (AudioParameters::paramName (Parameter::AmpEnvDecay), _ampEnvDecaySliderAttachment, _ampEnvDecaySlider, Slider::SliderStyle::LinearVertical, &_lookMidi);
    setupSlider (AudioParameters::paramName (Parameter::AmpEnvSustain), _ampEnvSustainSliderAttachment, _ampEnvSustainSlider, Slider::SliderStyle::LinearVertical, &_lookMidi);
    setupSlider (AudioParameters::paramName (Parameter::AmpEnvRelease), _ampEnvReleaseSliderAttachment, _ampEnvReleaseSlider, Slider::SliderStyle::LinearVertical, &_lookMidi);

    setupSlider (AudioParameters::paramName (Parameter::ModEnvAttack), _modEnvAttackSliderAttachment, _modEnvAttackSlider, Slider::SliderStyle::LinearVertical, &_lookMidi);
    setupSlider (AudioParameters::paramName (Parameter::ModEnvDecay), _modEnvDecaySliderAttachment, _modEnvDecaySlider, Slider::SliderStyle::LinearVertical, &_lookMidi);
    setupSlider (AudioParameters::paramName (Parameter::ModEnvSustain), _modEnvSustainSliderAttachment, _modEnvSustainSlider, Slider::SliderStyle::LinearVertical, &_lookMidi);
    setupSlider (AudioParameters::paramName (Parameter::ModEnvRelease), _modEnvReleaseSliderAttachment, _modEnvReleaseSlider, Slider::SliderStyle::LinearVertical, &_lookMidi);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (Theme::phiUnits (18), Theme::phiUnits (24));
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::setupSlider (
    const juce::String& parameterID,
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>& sliderAttachment,
    LabelSlider& slider,
    Slider::SliderStyle sliderStyle,
    LookAndFeel_V4* lookAndFeel)
{
    sliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.getParams(),
        parameterID,
        slider.getSlider());

    slider.attachParameter (processorRef.getParams(), parameterID);
    slider.getSlider().setSliderStyle (sliderStyle);
    slider.setLookAndFeel (lookAndFeel);
    addAndMakeVisible (slider);
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    auto area = getLocalBounds();
    g.setColour (juce::Colours::white);
    g.setFont (Theme::PHI / 2.0f);
    auto helloWorld = juce::String ("") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION + " running in " + CMAKE_BUILD_TYPE;
    g.drawText (helloWorld, area.removeFromTop (Theme::PHI), juce::Justification::left, false);
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    area.setHeight (Theme::phiUnits (1));
    area.removeFromLeft (area.getWidth() / 2);
    inspectButton.setBounds (area);

    Grid mainGrid;

    int row;
    int rowEnd;
    int col;

    int rowInc = 2;

    col = 1;
    row = 1;
    rowEnd = row + rowInc-1;

    // Row 1
    mainGrid.items.add (
        GridItem (_lfoFreqRatioSlider)
            .withArea (row, col, rowEnd, col)
            .withWidth (Theme::SLIDER_WIDTH)
            .withHeight (Theme::SLIDER_HEIGHT)
            .withJustifySelf (GridItem::JustifySelf::center));


    col++;

    mainGrid.items.add (
        GridItem (_lfoModAmountSlider)
            .withArea (row, col, rowEnd, col)
            .withWidth (Theme::SLIDER_WIDTH)
            .withHeight (Theme::SLIDER_HEIGHT)
            .withJustifySelf (GridItem::JustifySelf::center));


    col++;

    mainGrid.items.add (
        GridItem (_modFreqRatioSlider)
            .withArea (row, col, rowEnd, col)
            .withWidth (Theme::SLIDER_WIDTH)
            .withHeight (Theme::SLIDER_HEIGHT)
            .withJustifySelf (GridItem::JustifySelf::center));

    col++;

    mainGrid.items.add (
        GridItem (_phaseModAmountSlider)
            .withArea (row, col, rowEnd, col)
            .withWidth (Theme::SLIDER_WIDTH)
            .withHeight (Theme::SLIDER_HEIGHT)
            .withJustifySelf (GridItem::JustifySelf::center));


    // Row 2
    row = row + rowInc;
    rowEnd = row + rowInc-1;
    col = 1;

    mainGrid.items.add (
        GridItem (_ampEnvAttackSlider)
            .withArea (row, col, rowEnd, col)
            .withWidth (Theme::SLIDER_WIDTH)
            .withHeight (Theme::SLIDER_HEIGHT)
            .withJustifySelf (GridItem::JustifySelf::center));

    col++;
    mainGrid.items.add (
        GridItem (_ampEnvDecaySlider)
            .withArea (row, col, rowEnd, col)
            .withWidth (Theme::SLIDER_WIDTH)
            .withHeight (Theme::SLIDER_HEIGHT)
            .withJustifySelf (GridItem::JustifySelf::center));

    col++;
    mainGrid.items.add (
        GridItem (_ampEnvSustainSlider)
            .withArea (row, col, rowEnd, col)
            .withWidth (Theme::SLIDER_WIDTH)
            .withHeight (Theme::SLIDER_HEIGHT)
            .withJustifySelf (GridItem::JustifySelf::center));

    col++;
    mainGrid.items.add (
        GridItem (_ampEnvReleaseSlider)
            .withArea (row, col, rowEnd, col)
            .withWidth (Theme::SLIDER_WIDTH)
            .withHeight (Theme::SLIDER_HEIGHT)
            .withJustifySelf (GridItem::JustifySelf::center));

    // Row 3
    row = row + rowInc;
    rowEnd = row + rowInc-1;
    col = 1;

    mainGrid.items.add (
        GridItem (_modEnvAttackSlider)
            .withArea (row, col, rowEnd, col)
            .withWidth (Theme::SLIDER_WIDTH)
            .withHeight (Theme::SLIDER_HEIGHT)
            .withJustifySelf (GridItem::JustifySelf::center));

    col++;
    mainGrid.items.add (
        GridItem (_modEnvDecaySlider)
            .withArea (row, col, rowEnd, col)
            .withWidth (Theme::SLIDER_WIDTH)
            .withHeight (Theme::SLIDER_HEIGHT)
            .withJustifySelf (GridItem::JustifySelf::center));

    col++;
    mainGrid.items.add (
        GridItem (_modEnvSustainSlider)
            .withArea (row, col, rowEnd, col)
            .withWidth (Theme::SLIDER_WIDTH)
            .withHeight (Theme::SLIDER_HEIGHT)
            .withJustifySelf (GridItem::JustifySelf::center));

    col++;
    mainGrid.items.add (
        GridItem (_modEnvReleaseSlider)
            .withArea (row, col, rowEnd, col)
            .withWidth (Theme::SLIDER_WIDTH)
            .withHeight (Theme::SLIDER_HEIGHT)
            .withJustifySelf (GridItem::JustifySelf::center));

    // Make bounds with inner padding
    auto bounds = getLocalBounds();
    int pad = Theme::PHI;
    bounds.setX (bounds.getX() + pad);
    bounds.setY (bounds.getY() + pad);
    bounds.setWidth (bounds.getWidth() - (pad * 2));
    bounds.setHeight (bounds.getHeight() - (pad * 2));

    mainGrid.performLayout (bounds);
}
