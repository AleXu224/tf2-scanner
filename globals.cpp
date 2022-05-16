//
// Created by Squizell on 27/04/2022.
//
#include "globals.hpp"
#include "scanning/Scanner.hpp"

std::vector<ImFont*> GLOBALS::FONTS;
Console GLOBALS::console;
Scanner GLOBALS::scanner;

float BezierBlend(float t)
{
    return t * t * (3.0f - 2.0f * t);
}