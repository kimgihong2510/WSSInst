/*
  ==============================================================================

    Utils.cpp
    Created: 9 Mar 2024 7:04:08pm
    Author:  JohnKim

  ==============================================================================
*/

#include "Utils.h"

juce::String makeParamName(juce::String condType, juce::String pointType)
{
    return condType + "_" + pointType;
}
juce::String makeParamName(juce::String condType, juce::String pointType, int numCond)
{
    return condType + "_" + pointType + "_" + juce::String(numCond);
}


juce::StringArray splitString(const juce::String& text, const juce::String& delimiter)
{
    juce::StringArray result;

    int startIndex = 0;
    int endIndex = text.indexOf(delimiter);

    while (endIndex != -1)
    {
        if(startIndex<endIndex)
        {
            result.add(text.substring(startIndex, endIndex));
        }
        startIndex = endIndex + delimiter.length();
        endIndex = text.indexOf(startIndex, delimiter);
    }

    // Add the remaining part of the string
    result.add(text.substring(startIndex));

    return result;
}

