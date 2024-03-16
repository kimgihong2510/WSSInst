/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "ShowWaveform.h"

ShowWaveform::ShowWaveform(float* ptr)
{
    mPtrWaveform = ptr;
}

void ShowWaveform::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    // Draw waveform
    g.setColour(juce::Colours::white);
    juce::Path waveformPath;
    waveformPath.startNewSubPath(0, getHeight() / 2); // Start path at the middle left

    float absMax = 0.0f;
    for (int i = 0; i < mNumSamples; ++i)
    {
        if(std::abs(mPtrWaveform[i]) > absMax)
          absMax = std::abs(mPtrWaveform[i]);
    }
    float xIncrement = (float)getWidth() / (float)(mNumSamples - 1);
    for (int i = 0; i < mNumSamples; ++i)
    {
        float y = getHeight() / 2 + (mPtrWaveform[i] / absMax * getHeight() / 2); // Scale the amplitude to fit the height

        waveformPath.lineTo(i * xIncrement, y);
    }

    g.strokePath(waveformPath, juce::PathStrokeType(1.5f));


    g.setColour(juce::Colours::yellow);
    g.drawRect(getLocalBounds(), 2); 
    g.setColour(juce::Colours::black);
    juce::Rectangle<float> border(getLocalBounds().toFloat().reduced(2));
    g.drawRoundedRectangle(border, 10, 5); 
}

void ShowWaveform::resized()
{
    repaint();
}

