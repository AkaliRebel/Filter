/*
  ==============================================================================

    MyLookAndFeelClass.h
    Created: 7 Mar 2025 10:32:01pm
    Author:  Akali

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class MyLookAndFeelClass : public juce::LookAndFeel_V4
{
public:
    MyLookAndFeelClass();
    juce::Label* createSliderTextBox(juce::Slider&) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyLookAndFeelClass)
};