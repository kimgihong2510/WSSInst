/*
  ==============================================================================

    Utils.h
    Created: 9 Mar 2024 7:04:08pm
    Author:  JohnKim

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GlobalVariables.h"
extern int IC_NUM_CONDITION;
extern int SC_NUM_CONDITION;


juce::String makeParamName(juce::String condType, juce::String pointType);
juce::String makeParamName(juce::String condType, juce::String pointType, int numCond);

juce::StringArray splitString(const juce::String& text, const juce::String& delimiter);

/*typedef struct singleCondition
{
    float ICCondX[IC_NUM_CONDITION];
    float ICCondY[IC_NUM_CONDITION];
    float SCCondX[SC_NUM_CONDITION];
}SingleCondition;*/
