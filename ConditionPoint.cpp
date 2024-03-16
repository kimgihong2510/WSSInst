/*
  ==============================================================================

    ConditionPointVector.cpp
    Created: 5 Mar 2024 8:52:07pm
    Author:  JohnKim

  ==============================================================================
*/

#include "ConditionPoint.h"
#include "ConditionPad.h"

ConditionPoint::ConditionPoint()
{
}

void ConditionPoint::mouseDown(const juce::MouseEvent& e)
{
    mDragger.startDraggingComponent(this, e);
}

void ConditionPoint::mouseDrag(const juce::MouseEvent& e)
{
    mDragger.dragComponent(this, e, &mConstrainer);
    Callback(getX(), getY());
}

