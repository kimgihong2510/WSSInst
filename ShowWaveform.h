/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================
/**
*/

class ShowWaveform : public juce::Component
{
public:
    ShowWaveform(float* ptr);
    void paint (juce::Graphics& g) override;
    void resized() override;
private:
    float *mPtrWaveform;
    int mNumSamples=1024;
};
