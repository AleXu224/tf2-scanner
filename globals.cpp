//
// Created by Squizell on 27/04/2022.
//
#include "globals.hpp"

std::vector<ImFont*> GLOBALS::FONTS;

float BezierBlend(float t)
{
    return t * t * (3.0f - 2.0f * t);
}