/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyLookAndFeelClass.h"
//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    NewProjectAudioProcessor& audioProcessor;
    MyLookAndFeelClass myLookAndFeel;
    juce::Slider myCutoffSlider;
    juce::Slider myResonanceSlider;
    juce::ComboBox myTypeComboBox;
    juce::ToggleButton myBypassToggleButton{ "BYPASS" };
    juce::Slider myLfoDepthSlider;
    juce::Label myResonanceLabel{ {}, "RES" };
    juce::Label myCutoffLabel{ {},"FREQ" };
    juce::Label myTitleLabel{ {},"FILTER" };
    juce::Label myLfoDepthLabel{ {}, "LFO DEPTH" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> myCutoffSliderAttachmentptr;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> myResonanceSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> myTypeComboBoxAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> myBypassToggleButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> myLfoDepthSliderAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
