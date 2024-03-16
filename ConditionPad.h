/*
  ==============================================================================

    ConditionPad.h
    Created: 7 Mar 2024 5:29:52pm
    Author:  JohnKim

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GlobalVariables.h"
#include "ConditionPoint.h"
#include "PluginProcessor.h"

class ConditionPad : public juce::Component
{
public:
    ConditionPad(int num_condition_point_max, int* num_condition, juce::String cond_name, bool using_X, bool using_Y, juce::Slider::Listener* parent_ptr, float condition_min, float condition_max, juce::Slider* sliderX, juce::Slider* sliderY);

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    enum Axis {X, Y};
    void sliderValueChanged(juce::String type, float value);
    
    int mcurNumCond=0;
    
protected:
    juce::Slider* mSliderX;
    juce::Slider* mSliderY;

    ConditionPoint mConditionPoint{};
    

    int mNumVisibleCondPoints = 0;
    int NUM_CONDITION_POINT_MAX;
    int* NUM_CONDITION;
    float CONDITION_MIN; 
    float CONDITION_MAX;
    juce::String COND_NAME;
    bool USING_X;
    bool USING_Y;
    
};

