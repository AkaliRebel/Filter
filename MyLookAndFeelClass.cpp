/*
  ==============================================================================

    MyLookAndFeelClass.cpp
    Created: 7 Mar 2025 10:32:01pm
    Author:  Akali

  ==============================================================================
*/

#include "MyLookAndFeelClass.h"
juce::Label* MyLookAndFeelClass::createSliderTextBox(juce::Slider& myLAFSlider)
{
    juce::Label* myLAFLabelptr = LookAndFeel_V4::createSliderTextBox(myLAFSlider);
    myLAFLabelptr->setColour(juce::Label::textColourId, juce::Colours::palegreen);
    myLAFLabelptr->setColour(juce::Label::backgroundColourId, juce::Colours::black);
    myLAFLabelptr->setColour(juce::Label::outlineColourId, juce::Colours::seagreen);
    return myLAFLabelptr;
}