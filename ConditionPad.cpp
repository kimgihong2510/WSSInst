/*
  ==============================================================================

    ConditionPad.cpp
    Created: 7 Mar 2024 5:29:52pm
    Author:  JohnKim

  ==============================================================================
*/

#include "ConditionPad.h"

//considerable code 무조건 NUM POINT가 같아야함... 이렇게 짜면
ConditionPad::ConditionPad(int num_condition_point_max, int* num_condition, juce::String cond_name, bool using_X, bool using_Y, juce::Slider::Listener* parent_ptr, float condition_min, float condition_max, juce::Slider* sliderX, juce::Slider* sliderY)
{
    NUM_CONDITION_POINT_MAX = num_condition_point_max;
    NUM_CONDITION = num_condition;
    COND_NAME = cond_name;
    USING_X = using_X;
    USING_Y = using_Y;
    CONDITION_MIN = condition_min;
    CONDITION_MAX = condition_max;

    mSliderX = sliderX;
    if(USING_Y)
        mSliderY = sliderY;

    addAndMakeVisible(mConditionPoint);
    mConditionPoint.Callback = [&](float posX, float posY)
    {
        std::cout<<"point"<<std::endl;
        std::cout<<posX<<" "<<posY<<std::endl;

        auto parentWidth = getLocalBounds().toFloat().getWidth();
        auto parentHeight = getLocalBounds().toFloat().getHeight();
        auto thumbWidth = static_cast<float>(THUMBWIDTH);

        posY = (getHeight()-thumbWidth) - posY;
        auto mCondValueX = juce::jmap(posX, 0.0f, parentWidth - thumbWidth, CONDITION_MIN, CONDITION_MAX);
        mSliderX->setValue(mCondValueX);
        if(USING_Y)
        {
            auto mCondValueY = juce::jmap(posY, 0.0f, parentHeight - thumbWidth, CONDITION_MIN, CONDITION_MAX);
            mSliderY->setValue(mCondValueY);
            std::cout<<mCondValueX<<" "<<mCondValueY<<std::endl;
        }
    };
}


void ConditionPad::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    //g.drawRoundedRectangle(bounds, 10, 7);
    g.setGradientFill(juce::ColourGradient{ juce::Colours::black.brighter(0.2f), bounds.getTopLeft(), juce::Colours::black.brighter(0.1f), bounds.getBottomLeft(), false });
    g.fillRoundedRectangle(bounds, 10);
    
}
void ConditionPad::resized()
{
    mConditionPoint.setBounds(getLocalBounds().withSizeKeepingCentre(THUMBWIDTH, THUMBWIDTH));
}

void ConditionPad::sliderValueChanged(juce::String type, float value)
{
    int newPosX, newPosY;
    auto thumbWidth = static_cast<float>(THUMBWIDTH);
    auto bounds = getLocalBounds().toFloat();
    if(type.equalsIgnoreCase("x"))
    {
        newPosX = static_cast<int>(juce::jmap(value, CONDITION_MIN, CONDITION_MAX, 0.0f, bounds.getWidth() - thumbWidth));
        newPosY = mConditionPoint.getY();
    }
    else
    {
        newPosX = mConditionPoint.getX();
        newPosY = (bounds.getHeight() - thumbWidth) - static_cast<int>(juce::jmap(value, CONDITION_MIN, CONDITION_MAX, 0.0f , bounds.getHeight() - thumbWidth));
    }
    mConditionPoint.setTopLeftPosition(newPosX, newPosY);        
}
