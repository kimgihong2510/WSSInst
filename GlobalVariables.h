/*
  ==============================================================================

    GlobalVariables.h
    Created: 5 Mar 2024 8:52:15pm
    Author:  JohnKim

  ==============================================================================
*/

#pragma once

#include <math.h>

#define _USE_MATH_DEFINES

constexpr float PI = static_cast<float>(M_PI);

static constexpr int THUMBWIDTH = 30;

#define IC_NUM_CONDITION_POINT_MAX 1
#define SC_NUM_CONDITION_POINT_MAX 1

extern int IC_NUM_CONDITION;
extern int SC_NUM_CONDITION;

static constexpr float IC_CONDITION_MIN = 0.0f;
static constexpr float IC_CONDITION_MAX = 5.0f;

static constexpr float SC_CONDITION_MIN = 0.0f;
static constexpr float SC_CONDITION_MAX = 1.0f;
static constexpr float SYM_CONDITION = PI;




