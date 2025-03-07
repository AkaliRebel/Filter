/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    myLookAndFeel.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    myLookAndFeel.setColour(juce::ToggleButton::tickColourId, juce::Colours::palegreen);
    myLookAndFeel.setColour(juce::ToggleButton::tickDisabledColourId, juce::Colours::seagreen);
    getLookAndFeel().setColour(juce::ComboBox::backgroundColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::ComboBox::textColourId, juce::Colours::palegreen);
    getLookAndFeel().setColour(juce::ComboBox::outlineColourId, juce::Colours::seagreen);
    getLookAndFeel().setColour(juce::ComboBox::arrowColourId, juce::Colours::seagreen);
    myLookAndFeel.setColour(juce::Slider::backgroundColourId, juce::Colours::black);
    myLookAndFeel.setColour(juce::Slider::trackColourId, juce::Colours::seagreen);
    myLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::palegreen);
    myLookAndFeel.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::seagreen);
    myLookAndFeel.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    myCutoffSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::palegreen);
    myCutoffSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    myCutoffSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::seagreen);
    myBypassToggleButton.setLookAndFeel(&myLookAndFeel);
    myCutoffSlider.setLookAndFeel(&myLookAndFeel);
    myResonanceSlider.setLookAndFeel(&myLookAndFeel);
    myLfoDepthSlider.setLookAndFeel(&myLookAndFeel);
    setSize (400, 300);
    addAndMakeVisible(myCutoffSlider);
    myCutoffSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    myCutoffSliderAttachmentptr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.myValueTreeState, "cutoff", myCutoffSlider));
    addAndMakeVisible(myCutoffLabel);
    myCutoffLabel.attachToComponent(&myCutoffSlider, false);
    addAndMakeVisible(myResonanceSlider);
    myResonanceSlider.setSliderStyle(juce::Slider::LinearVertical);
    myResonanceSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    myResonanceSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.myValueTreeState, "resonance", myResonanceSlider));
    addAndMakeVisible(myResonanceLabel);
    myResonanceLabel.attachToComponent(&myResonanceSlider, false);
    addAndMakeVisible(myTypeComboBox);
    myTypeComboBox.addItem("LowP", 1);
    myTypeComboBox.addItem("HighP", 2);
    myTypeComboBox.addItem("BandP", 3);
    myTypeComboBoxAttachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.myValueTreeState, "type", myTypeComboBox));

    addAndMakeVisible(myBypassToggleButton);
    myBypassToggleButtonAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.myValueTreeState, "bypass", myBypassToggleButton));

    addAndMakeVisible(myLfoDepthSlider);
    myLfoDepthSlider.setSliderStyle(juce::Slider::Rotary);
    myLfoDepthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    myLfoDepthSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.myValueTreeState, "lfoDepth", myLfoDepthSlider));
    addAndMakeVisible(myLfoDepthLabel);
    myLfoDepthLabel.attachToComponent(&myLfoDepthSlider, true);

    addAndMakeVisible(myTitleLabel);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}
MyLookAndFeelClass::MyLookAndFeelClass()
{

}
MyLookAndFeelClass::~MyLookAndFeelClass()
{

}
//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    myCutoffSlider.setBounds(50, 50, 350, 50);
    myResonanceSlider.setBounds(0, 50, 50, 150);
    myTypeComboBox.setBounds(50, 100, 100, 50);
    myBypassToggleButton.setBounds(50, 150, 200, 50);
    myLfoDepthSlider.setBounds(50, 200, 100, 100);
    myTitleLabel.setBounds(200, 0, 100, 50);
}
