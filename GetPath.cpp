/*
  ==============================================================================

    GetPath.cpp
    Created: 10 Mar 2024 10:14:10am
    Author:  JohnKim

  ==============================================================================
*/

#include "GetPath.h"

/*SingleCondition PathManager::getPath(juce::AudioProcessorValueTreeState& apvts)
{
    SingleCondition result;
    for(int i=0; i<IC_NUM_CONDITION; i++)
    {
        auto j = 0;
        auto paramNameX = makeParamName("IC", "X");
        auto paramNameY = makeParamName("IC", "Y");
        result.ICCondX[i] = apvts.getParameterAsValue(paramNameX).getValue();
        result.ICCondY[i] = apvts.getParameterAsValue(paramNameY).getValue();
    }
    
    for(int i=0; i<SC_NUM_CONDITION; i++)
    {
        auto j = 0;
        auto paramNameX = makeParamName("SC", "X");
        result.SCCondX[i] = apvts.getParameterAsValue(paramNameX).getValue();
    }
    
    return result;
}
*/
float PathManager::getWaveform()
{
    
}

