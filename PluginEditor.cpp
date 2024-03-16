/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveSpaceSynthesizerAudioProcessorEditor::WaveSpaceSynthesizerAudioProcessorEditor (WaveSpaceSynthesizerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), 
    mAttachmentICX(p.apvts, "IC_X", mICSliderX), 
    mAttachmentICY(p.apvts, "IC_Y", mICSliderY), 
    mAttachmentSCX(p.apvts, "SC_X", mSCSliderX),
    mICTextBox(""), mSCTextBox(""), mLoadTorchModelButton("Load Model")
{
    mICPad = std::make_unique<ConditionPad>(IC_NUM_CONDITION_POINT_MAX, &IC_NUM_CONDITION, juce::String("IC"), true, true, this, IC_CONDITION_MIN, IC_CONDITION_MAX, &mICSliderX, &mICSliderY);
    mSCPad = std::make_unique<ConditionPad>(SC_NUM_CONDITION_POINT_MAX, &SC_NUM_CONDITION, juce::String("SC"), true, false, this, SC_CONDITION_MIN, SC_CONDITION_MAX, &mSCSliderX, nullptr); //considerable code 2  mICSilderVectorY...
    mWFPad = std::make_unique<ShowWaveform>(p.sineOscillator.waveform);

    addAndMakeVisible(mICPad.get());
    
    addAndMakeVisible(mICSlider);
    mICSlider.setRange(0, IC_NUM_CONDITION-1, 1.0);
    mICSlider.setSliderSnapsToMousePosition(true);
    mICSlider.addListener(this);
    
    addAndMakeVisible(mSCPad.get());
    
    addAndMakeVisible(mSCSlider);
    mSCSlider.setRange(0, SC_NUM_CONDITION-1, 1.0);
    mSCSlider.setSliderSnapsToMousePosition(true);
    mSCSlider.addListener(this);

    addAndMakeVisible(mWFPad.get());
    
    
    addAndMakeVisible(mLoadTorchModelButton);
    mLoadTorchModelButton.addListener(this);

    addAndMakeVisible(mICSliderX);
    //addChildComponent(mICSliderX);
    mICSliderX.setRange(IC_CONDITION_MIN, IC_CONDITION_MAX, 0.001f);
    mICSliderX.addListener(this);

    addChildComponent(mICSliderY);
    mICSliderY.setRange(IC_CONDITION_MIN, IC_CONDITION_MAX, 0.001f);
    mICSliderY.addListener(this);

    addChildComponent(mSCSliderX);
    mSCSliderX.setRange(SC_CONDITION_MIN, SC_CONDITION_MAX, 0.001f);
    mSCSliderX.addListener(this);

    addAndMakeVisible(mGainSlider);
    mGainSlider.setRange(0, 10, 0.001f);
    mGainSlider.setValue(2.0f);
    mGainSlider.addListener(this);

    addAndMakeVisible(mAttackSlider);
    mAttackSlider.setRange(0.001, 3, 0.001f);
    mAttackSlider.setValue(0.001f);
    mAttackSlider.addListener(this);

    addAndMakeVisible(mDecaySlider);
    mDecaySlider.setRange(0.001, 3, 0.001f);
    mDecaySlider.setValue(0.5f);
    mDecaySlider.addListener(this);

    addAndMakeVisible(mSustainSlider);
    mSustainSlider.setRange(0.000, 1, 0.001f);
    mSustainSlider.setValue(1);
    mSustainSlider.addListener(this);

    addAndMakeVisible(mReleaseSlider);
    mReleaseSlider.setRange(0.001, 3, 0.001f);
    mReleaseSlider.setValue(0.001f);
    mReleaseSlider.addListener(this);

    addAndMakeVisible(mICTextBox);
    addAndMakeVisible(mSCTextBox);
    
    setSize (960, 500);
    setResizable(false, false);
}

WaveSpaceSynthesizerAudioProcessorEditor::~WaveSpaceSynthesizerAudioProcessorEditor()
{
}

//==============================================================================

void WaveSpaceSynthesizerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::yellow);
    g.drawRect(mTextBoxes, 2); 
}

void WaveSpaceSynthesizerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
     
     //LEFT SIDE
    auto ICSliderBound = area.removeFromLeft(30);
    mICSlider.setBounds(ICSliderBound);
    auto ICPadBound = createSquareFromRectangle(&area);
    area.removeFromLeft(ICPadBound.getWidth());
    mICPad->setBounds(ICPadBound);
    
    //RIGHT SIDE
    
    auto WTBound = area.removeFromTop(area.getHeight() * 1 / 2);
    mWFPad->setBounds(WTBound);
    auto SCPadBound = area.removeFromTop(THUMBWIDTH);
    mSCPad->setBounds(SCPadBound);
    auto SCSliderBound = area.removeFromTop(30);
    mSCSlider.setBounds(SCSliderBound);
    
    auto TextBoxesBound =  mTextBoxes = area.removeFromTop(area.getHeight()*0.2);
    mICTextBox.setBounds(TextBoxesBound.removeFromLeft(TextBoxesBound.getWidth()/2));
    mSCTextBox.setBounds(TextBoxesBound);

    auto AmpEnvBound = area;
    auto SliderWidth = AmpEnvBound.getWidth()*0.1;
    mGainSlider.setBounds(AmpEnvBound.removeFromLeft(SliderWidth));
    mAttackSlider.setBounds(AmpEnvBound.removeFromLeft(SliderWidth));
    mDecaySlider.setBounds(AmpEnvBound.removeFromLeft(SliderWidth));
    mSustainSlider.setBounds(AmpEnvBound.removeFromLeft(SliderWidth));
    mReleaseSlider.setBounds(AmpEnvBound.removeFromLeft(SliderWidth));
    mLoadTorchModelButton.setBounds(AmpEnvBound);
    
    //sliderValueChanged(&mICSliderX);
    //sliderValueChanged(&mICSliderY);
    //sliderValueChanged(&mSCSliderX);
}

void WaveSpaceSynthesizerAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) 
{
    juce::String condName, typeName;

    if(slider == &mAttackSlider)
    {
        audioProcessor.sineOscillator.mAttackDuration = slider->getValue();
    }
    if(slider == &mDecaySlider)
    {
        audioProcessor.sineOscillator.mDecayDuration = slider->getValue();
    }    
    if(slider == &mSustainSlider)
    {
        audioProcessor.sineOscillator.mSustain = slider->getValue();
    }
    if(slider == &mReleaseSlider)
    {
        audioProcessor.sineOscillator.mReleaseDuration = slider->getValue();
    }
    if(slider == &mGainSlider)
    {
        audioProcessor.sineOscillator.getWaveform(audioProcessor.mICXValue, audioProcessor.mICYValue, audioProcessor.mSCXValue,mTorchModel, mGainSlider.getValue());
    }
    if(slider == &mICSlider)
    {
        int newCondNum = slider->getValue();
        mICPad->mcurNumCond = audioProcessor.mCurCondIC = newCondNum;
        mICSliderX.setValue(audioProcessor.mICXValue[newCondNum]);
        mICSliderY.setValue(audioProcessor.mICYValue[newCondNum]);
        mICTextBox.setText(mICName[newCondNum]);
    }
    if(slider == &mSCSlider)
    {
        int newCondNum = slider->getValue();
        mSCPad->mcurNumCond = audioProcessor.mCurCondSC = newCondNum;
        mSCSliderX.setValue(audioProcessor.mSCXValue[newCondNum]);
        mSCTextBox.setText(mSCName[newCondNum]);
    }
    if(slider == &mICSliderX)
    {
        mICPad->sliderValueChanged("X", mICSliderX.getValue());
        audioProcessor.sineOscillator.getWaveform(audioProcessor.mICXValue, audioProcessor.mICYValue, audioProcessor.mSCXValue,mTorchModel, mGainSlider.getValue());
    }
    if(slider == &mICSliderY)
    {
        mICPad->sliderValueChanged("Y", mICSliderY.getValue());
        audioProcessor.sineOscillator.getWaveform(audioProcessor.mICXValue, audioProcessor.mICYValue, audioProcessor.mSCXValue,mTorchModel, mGainSlider.getValue());
    }
    if(slider == &mSCSliderX)
    {
        mSCPad->sliderValueChanged("X", mSCSliderX.getValue());
        audioProcessor.sineOscillator.getWaveform(audioProcessor.mICXValue, audioProcessor.mICYValue, audioProcessor.mSCXValue,mTorchModel, mGainSlider.getValue());
    }

}

/*void WaveSpaceSynthesizerAudioProcessorEditor::valueChanged(juce::Value &value)
{
    //may be a really bad method to implement...
    if(audioProcessor.checkFlagThumbPosChanged())
    {
        updatePadFromCondValue();
    }
}*/

juce::Rectangle<int> WaveSpaceSynthesizerAudioProcessorEditor::createSquareFromRectangle(juce::Rectangle<int>* bounds)
{
    int squareSize = std::min(bounds->getWidth(), bounds->getHeight());
    
    juce::Rectangle<int> square(bounds->getX(), bounds->getY(), squareSize, squareSize);
    return square;
}

void WaveSpaceSynthesizerAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &mLoadTorchModelButton)
    {
        mChooser.launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file.exists())
            {
                audioProcessor.mICXValue.clear();
                audioProcessor.mICYValue.clear();
                audioProcessor.mSCXValue.clear();
                mICName.clear();
                mSCName.clear();

                auto fileData = file.loadFileAsString();
                auto fileStringArray = splitString(fileData, "\n");
                IC_NUM_CONDITION = fileStringArray[0].getIntValue();
                SC_NUM_CONDITION = fileStringArray[IC_NUM_CONDITION+1].getIntValue();
                
                for(int i=1; i<=IC_NUM_CONDITION; i++)
                {
                    auto condName = fileStringArray[i];
                    audioProcessor.mICXValue.push_back(0.0f);
                    audioProcessor.mICYValue.push_back(0.0f);
                    mICName.push_back(condName);
                }
                for(int i=IC_NUM_CONDITION+2; i<IC_NUM_CONDITION+SC_NUM_CONDITION+2; i++)
                {
                    auto condName = fileStringArray[i];
                    if(i == IC_NUM_CONDITION+SC_NUM_CONDITION+1)
                        audioProcessor.mSCXValue.push_back(0.5f);
                    else
                        audioProcessor.mSCXValue.push_back(0.0f);
                    mSCName.push_back(condName);
                }
                mICSlider.setRange(0, IC_NUM_CONDITION-1, 1.0);
                mICSlider.setValue(0);
                mSCSlider.setRange(0, SC_NUM_CONDITION-1, 1.0);
                mSCSlider.setValue(0);
                
                sliderValueChanged(&mICSlider);
                sliderValueChanged(&mSCSlider);

                mTorchModel.LoadTorch(file.getFullPathName().replace(".hdgs", ".pt").toUTF8());
            }
        });
    }
}
