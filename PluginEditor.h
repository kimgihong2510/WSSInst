/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GlobalVariables.h"
#include "PluginProcessor.h"
#include "ConditionPad.h"
#include "TorchLoader.h"
#include "ShowWaveform.h"
#include "TextComponent.h"


class WaveSpaceSynthesizerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener, public juce::Button::Listener
{
public:
    WaveSpaceSynthesizerAudioProcessorEditor (WaveSpaceSynthesizerAudioProcessor&);
    ~WaveSpaceSynthesizerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider *slider) override;
    void buttonClicked(juce::Button* button) override;

private:
   
    juce::Rectangle<int> createSquareFromRectangle(juce::Rectangle<int>* bounds);
    WaveSpaceSynthesizerAudioProcessor& audioProcessor;
    
    std::unique_ptr<ConditionPad> mICPad;
    std::unique_ptr<ConditionPad> mSCPad;
    std::unique_ptr<ShowWaveform> mWFPad;

    juce::Slider mICSlider = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox);
    juce::Slider mSCSlider = juce::Slider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::NoTextBox);
    
    juce::Slider mICSliderX = juce::Slider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::NoTextBox);
    juce::Slider mICSliderY = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox);
    juce::Slider mSCSliderX = juce::Slider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::NoTextBox);

    juce::Slider mGainSlider = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxBelow);
    juce::Slider mAttackSlider = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxBelow);
    juce::Slider mDecaySlider = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxBelow);
    juce::Slider mSustainSlider = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxBelow);
    juce::Slider mReleaseSlider = juce::Slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxBelow);
        
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentICX;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentICY;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachmentSCX;

    TextComponent mICTextBox;
    TextComponent mSCTextBox;
    juce::Rectangle<int> mTextBoxes;


    juce::TextButton mLoadTorchModelButton;
    TorchModel mTorchModel{};
    juce::FileChooser mChooser{"Select a hdgs file to load...", juce::File::getSpecialLocation(juce::File::userDesktopDirectory), "*.hdgs"};

    std::vector<juce::String> mICName;
    std::vector<juce::String> mSCName;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveSpaceSynthesizerAudioProcessorEditor)
};


