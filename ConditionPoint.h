/*
  ==============================================================================

    ConditionPointVector.h
    Created: 5 Mar 2024 8:52:07pm
    Author:  JohnKim

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GlobalVariables.h"
#include "Thumb.h"

class ConditionPad;

class ConditionPoint : public Thumb
{
public:
    ConditionPoint();
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    //void setVisible(bool state);
    std::function<void(float x, float y)> Callback; // considerable code 이거 public?
private:
    float mCondValueX = 0.0f; // converting pos value to Condition value
    float mCondValueY = 0.0f; // converting pos value to Condition value

};
