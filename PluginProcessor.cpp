/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveSpaceSynthesizerAudioProcessor::WaveSpaceSynthesizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, ("apvts WSS"), createParameterLayout())
#endif
{
    for (auto* param : this->getParameters())
    {
        if (auto* paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID*>(param))
        {
            auto paramID = paramWithID->getParameterID();
            apvts.addParameterListener(paramID, this);
        }
    }
    
    ///##############################################

}

WaveSpaceSynthesizerAudioProcessor::~WaveSpaceSynthesizerAudioProcessor()
{
    for (auto* param : this->getParameters())
    {
        if (auto* paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID*>(param))
        {
            auto paramID = paramWithID->getParameterID();
            apvts.removeParameterListener(paramID, this);
        }
    }
}

//==============================================================================
const juce::String WaveSpaceSynthesizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaveSpaceSynthesizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WaveSpaceSynthesizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WaveSpaceSynthesizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WaveSpaceSynthesizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaveSpaceSynthesizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WaveSpaceSynthesizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WaveSpaceSynthesizerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WaveSpaceSynthesizerAudioProcessor::getProgramName (int index)
{
    return {};
}

void WaveSpaceSynthesizerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WaveSpaceSynthesizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    msampleRate = sampleRate;
}

void WaveSpaceSynthesizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WaveSpaceSynthesizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void WaveSpaceSynthesizerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    for (const juce::MidiMessageMetadata midiMetaData : midiMessages)
    {
        auto midiMessage = midiMetaData.getMessage();

        //auto freq = 440.0f;
        //sineOscillator.setFrequencyZero(static_cast<float>(freq), msampleRate);
        if (midiMessage.isNoteOn())
        {
            auto freq = midiMessage.getMidiNoteInHertz(midiMessage.getNoteNumber(), 440.f);
            sineOscillator.setFrequencyZero(static_cast<float>(freq), msampleRate);
        }
        else if (midiMessage.isNoteOff())
        {
            auto freq = midiMessage.getMidiNoteInHertz(midiMessage.getNoteNumber(), 440.f);
            sineOscillator.removeKeyOn(static_cast<float>(freq), msampleRate);
        }
    }

    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    for(int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        auto nextSample = sineOscillator.getNextSample();
        if(nextSample > 1)
            nextSample = 1;
        else if(nextSample < -1)
            nextSample = -1;
        for(int channel=0; channel<totalNumOutputChannels; channel++)
        {
            auto channelData = buffer.getWritePointer(channel);
            channelData[sample] = nextSample;
            
        }
    }
    
}

//==============================================================================
bool WaveSpaceSynthesizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WaveSpaceSynthesizerAudioProcessor::createEditor()
{
    return new WaveSpaceSynthesizerAudioProcessorEditor (*this);
}

//==============================================================================
void WaveSpaceSynthesizerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WaveSpaceSynthesizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveSpaceSynthesizerAudioProcessor();
}

void WaveSpaceSynthesizerAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if(mICXValue.empty() || mICYValue.empty() || mSCXValue.empty())
        return;
    auto splitStr = splitString(parameterID, "_");
    auto condName = splitStr[0];
    auto typeName = splitStr[1];
    if(condName.equalsIgnoreCase("IC"))
    {
        if(typeName.equalsIgnoreCase("X"))
            mICXValue[mCurCondIC] = newValue;
        else
            mICYValue[mCurCondIC] = newValue;
    }
    else
    {
        mSCXValue[mCurCondSC] = newValue;
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout WaveSpaceSynthesizerAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
 
    //IC param for X
    auto name = juce::String("IC_X");
    auto ID = juce::ParameterID{name, 1};
    auto range =  juce::NormalisableRange<float> (IC_CONDITION_MIN, IC_CONDITION_MAX, 0.001f);
    auto defaultValue = 0.0f;
    layout.add(std::make_unique<juce::AudioParameterFloat>(ID, name, range, defaultValue));
            
    //IC param for Y
    name = juce::String("IC_Y");
    ID = juce::ParameterID{name, 1};
    range =  juce::NormalisableRange<float> (IC_CONDITION_MIN, IC_CONDITION_MAX, 0.001f);
    defaultValue = 0.0f;
    layout.add(std::make_unique<juce::AudioParameterFloat>(ID, name, range, defaultValue));

    //SC param
    name = juce::String("SC_X");
    ID = juce::ParameterID{name, 1};
    range =  juce::NormalisableRange<float> (SC_CONDITION_MIN, SC_CONDITION_MAX, 0.001f);
    defaultValue = 0.0f;
    layout.add(std::make_unique<juce::AudioParameterFloat>(ID, name, range, defaultValue));

    return layout;
}

