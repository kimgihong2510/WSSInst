/*
  ==============================================================================

    TorchLoader.h
    Created: 10 Mar 2024 11:37:16am
    Author:  JohnKim

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h> 
#include <torch/script.h>
#include "GlobalVariables.h"

class TorchModel
{
public:
    
    bool LoadTorch(const char* path);
    void getWave(std::unique_ptr<std::vector<float>> params, float* result);
private:

    std::unique_ptr<torch::jit::script::Module> torchModule;
};
