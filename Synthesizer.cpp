/*
  ==============================================================================

    Synthesizer.cpp
    Created: 10 Mar 2024 10:14:16am
    Author:  JohnKim

  ==============================================================================
*/

#include "Synthesizer.h"
#include "TorchLoader.h"


#define MAX_KEY 10

SineOscillator::SineOscillator(): pathManager() {}
SineOscillator::~SineOscillator() {}

bool isTwoFloatSame(float a, float b)
{
    float epsilon = static_cast<float>(1e-4);
    if(abs(a-b) < epsilon)
        return true;
    else
        return false;
}

void SineOscillator::setFrequencyZero(float frequencyzero, float sampleRate)
{
    auto cyclesPerSample = frequencyzero / sampleRate;
    mSampleRate = sampleRate;
    addKeyOn(cyclesPerSample * PI * 2);
}

float SineOscillator::getNextSample()
{
    float sum = 0.0f;
    
    for(int i = 0; i < MAX_KEY; i++)
    {
        float amp = 0.0f;
        if(keyboardOn[i])
        {
            if(mInAttack[i]>0) // attack
            {
                auto mNumSampleAttack = mSampleRate * mAttackDuration;
                amp = (mNumSampleAttack - mInAttack[i]) / mNumSampleAttack;
                mInAttack[i]--;
            }
            else if(mInAttack[i]<=0 && mInDecay[i]>0) // decay
            {
                auto mNumSampleDecay = mSampleRate * mDecayDuration;
                amp = (1-mSustain) * mInDecay[i] / mNumSampleDecay + mSustain;
                mInDecay[i]--;
            }
            else // sustain
            {
                amp = mSustain;
            }
        }
        else 
        {
            if(mInRelease[i]<=0)
            {
                continue;
            }
            else // release
            {
                auto mNumSampleRelease = mSampleRate * mReleaseDuration;
                amp = mSustain * mInRelease[i] / mNumSampleRelease;
                mInRelease[i]--;
            }
        }
        auto angle = static_cast<int>(std::floor(currentAngle[i] / (2*PI) * 1024));

        sum += waveform[angle] * amp;
    }
    updateAngle();
    
    return sum;
}

void SineOscillator::updateAngle()
{
    for(int i=0; i<MAX_KEY; i++)
        if(keyboardOn[i] == true || mInRelease[i]>=0)
        {
            currentAngle[i] += angleDelta[i];
            if (currentAngle[i] >= PI * 2)
                currentAngle[i] -= PI * 2;
        }
}

void SineOscillator::addKeyOn(float tmpAngleDelta)
{
    bool flag = true;
    int emptyIndex = -1;
    for(int i=0; i<MAX_KEY; i++)
    {
        if(keyboardOn[i] == false && mInRelease[i] <= 0) // add new key not in release
            emptyIndex = i;
        else if(keyboardOn[i] == false && emptyIndex == -1)
            emptyIndex = i;

        if(isTwoFloatSame(angleDelta[i], tmpAngleDelta)) // pressing the same key multiple times
        {
            flag = false;
            emptyIndex = i;
            DBG("again");
            break;
        }
    }
    keyboardOn[emptyIndex] = true;
    angleDelta[emptyIndex] = tmpAngleDelta;
    currentAngle[emptyIndex] = 0.0f;
    mInAttack[emptyIndex] = mAttackDuration * mSampleRate;
    mInDecay[emptyIndex] = mDecayDuration * mSampleRate;
    mInRelease[emptyIndex] = mReleaseDuration * mSampleRate;

    if(flag)
        howManyKeyOn++;
}

void SineOscillator::removeKeyOn(float frequency, float sampleRate)
{
    auto cyclesPerSample = frequency / sampleRate;
    auto tmpAngleDelta = cyclesPerSample * PI * 2;

    for(int i=0; i<MAX_KEY; i++)
    {
        if(isTwoFloatSame(angleDelta[i], tmpAngleDelta))
        {
            keyboardOn[i] = false;
            howManyKeyOn--;
        }
    }
}

void SineOscillator::getWaveform(std::vector<float>& ICXvalues, std::vector<float>&  ICYvalues, std::vector<float>&  SCXvalues, TorchModel& model, float gain)
{
    if(ICXvalues.empty() || ICYvalues.empty() || SCXvalues.empty())
    {
        return;
    }
        
    auto param = std::make_unique<std::vector<float>>();

    for(int i=0; i<IC_NUM_CONDITION; i++)
    {
        param->push_back(ICXvalues[i]);
        param->push_back(ICYvalues[i]);  
    }

    for(int i=0; i<SC_NUM_CONDITION; i++)
    {
        auto value = SCXvalues[i];

        if(i == SC_NUM_CONDITION-1)
            param->push_back(value*2*PI - PI); 
        else
            param->push_back(value);  
    }

    model.getWave(std::move(param), waveform);
    for(int i =0; i< 1024; i++)
    {
        waveform[i] *= gain;
    }
}
