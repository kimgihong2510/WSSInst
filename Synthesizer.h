/*
  ==============================================================================

    Synthesizer.h
    Created: 10 Mar 2024 10:14:16am
    Author:  JohnKim

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GlobalVariables.h"
#include "GetPath.h"
#include "TorchLoader.h"
#include "torch/script.h"

class SineOscillator
{
public:
    SineOscillator();
    ~SineOscillator();

    void setFrequencyZero(float frequency, float sampleRate);
    float getNextSample();
    void updateAngle();
    void removeKeyOn(float frequency, float sampleRate);
    void getWaveform(std::vector<float>& ICXvalues, std::vector<float>& ICYvalues, std::vector<float>& SCXvalues, TorchModel& model, float gain);
    float waveform[1024] = {0.0f};
    float mAttackDuration;
    float mReleaseDuration;
    float mDecayDuration;
    float mSustain;
 
private:
    void addKeyOn(float frequency);
    float currentAngle[10] = {0};
    float angleDelta[10] = {0};
    bool keyboardOn[10] = {false};
    float mNumKeyOnSample[10];

    int mInRelease[10]={0};
    int mInAttack[10]={0};
    int mInDecay[10]={0}; 

    int howManyKeyOn = 0;
    float mSampleRate;

    PathManager pathManager;
};

bool isTwoFloatSame(float a, float b);
