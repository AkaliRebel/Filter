/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>
//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
     : myValueTreeState(*this, nullptr, juce::Identifier("myParameters"),
         {
             std::make_unique<juce::AudioParameterFloat>("cutoff", "CUTOFF", 20.0f, 20000.0f, 1000.0f),std::make_unique<juce::AudioParameterFloat>("resonance", "RES", 0.707f, 10.0f, 2.66f),
        std::make_unique<juce::AudioParameterChoice>("type", "TYPE", juce::StringArray{"LowP", "HighP", "BandP"}, 2),
        std::make_unique<juce::AudioParameterBool>("bypass", "BYPASS", 0),std::make_unique<juce::AudioParameterFloat>("depth", "DEPTH", 0.0f, 20.0f, 0.0f)
         })
{
    myCutoffptr = dynamic_cast<juce::AudioParameterFloat*>(myValueTreeState.getParameter("cutoff"));
    myresonanceptr = dynamic_cast<juce::AudioParameterFloat*>(myValueTreeState.getParameter("resonance"));
    mytypeptr = dynamic_cast<juce::AudioParameterChoice*>(myValueTreeState.getParameter("type"));
    mybypassptr = dynamic_cast<juce::AudioParameterBool*>(myValueTreeState.getParameter("bypass"));
    myLfoDepthptr= dynamic_cast<juce::AudioParameterFloat*>(myValueTreeState.getParameter("depth"));
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mySpec.sampleRate = sampleRate;
    mySpec.maximumBlockSize = samplesPerBlock;
    mySpec.numChannels = getTotalNumOutputChannels();

    myFilter.prepare(mySpec);
    myFilter.reset();
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    myPlayheadptr = this->getPlayHead();
    myPlayheadptr->getCurrentPosition(myHostTiming);
    mySongPosition = myHostTiming.ppqPosition;
    myLfoPhase = std::fmod(mySongPosition, 1);

    myFilterFrequency = myCutoffptr->get() + (myCutoffptr->get() * (myLfoPhase * myLfoDepthptr->get()));
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    myFilter.setCutoffFrequency(myFilterFrequency);
    myFilter.setResonance(myresonanceptr->get());
    switch (mytypeptr->getIndex())
    {
    case 0:
        myFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        break;
    case 1:
        myFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        break;
    case 2:
        myFilter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
        break;
    default:
        myFilter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
    }
    if (!mybypassptr->get())
    {
        auto myBlock = juce::dsp::AudioBlock<float>(buffer);
        auto myContext = juce::dsp::ProcessContextReplacing<float>(myBlock);

        myFilter.process(myContext);
    }
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new TutorialAudioProcessorEditor(*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = myValueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(myValueTreeState.state.getType()))
            myValueTreeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
