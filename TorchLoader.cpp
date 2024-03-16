/*
  ==============================================================================

    TorchLoader.cpp
    Created: 10 Mar 2024 11:37:16am
    Author:  JohnKim

  ==============================================================================
*/

#include "TorchLoader.h"

//include <memory>

//const char* file = "./traced_model.pt";


bool TorchModel::LoadTorch(const char* path)
{ 
    torchModule = std::make_unique<torch::jit::script::Module>(torch::jit::load(path));

    try {
      DBG("loaded");
      return true;
    }
    catch (const c10::Error& e) {
      std::cerr << "error loading the model\n";
      return false;
    }
}

//juce::Complex<float> kcomplexfloat_to_Complex(const at::kComplexFloat& kcomplex)
//{
//    return Complex<float>(kcomplex.real(), kcomplex.imag());
//}

void TorchModel::getWave(std::unique_ptr<std::vector<float>> params, float* result)
{
    if(torchModule == nullptr)
      return;

    for(int i=0; i<18*2+5; i++)
    {
      std::cout<<(*params)[i]<<" ";
    }
    std::cout<<"IC "<< IC_NUM_CONDITION << " SC "<< SC_NUM_CONDITION<<std::endl;
    auto paramsinTensor = torch::zeros({1, IC_NUM_CONDITION*2 + SC_NUM_CONDITION}, torch::kFloat32);

    std::cout<<"input"<<std::endl;
    auto paramsinTenson_numel = static_cast<unsigned long long>(paramsinTensor.numel());
    std::memcpy(paramsinTensor.data_ptr(), params->data(), sizeof(float)*paramsinTenson_numel);
    std::vector<torch::jit::IValue> input;
    input.push_back(paramsinTensor);

    at::Tensor output = torchModule->forward(input).toTensor();
    auto output_numel = static_cast<unsigned long long>(output.numel());

    //at::kComplexFloat complexArray[1024];
    std::memcpy(result, output.data_ptr(), sizeof(float)*output_numel);
    for(int i=0; i<5; i++)
    {
      std::cout<<result[i];
    }
    std::cout<<"output"<<std::endl;
}

