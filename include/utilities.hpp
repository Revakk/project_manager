#pragma once

#include <type_traits>
#include <random>


inline int rngGen(const int& a, const int& b)//funkce pro generaci nahodnych cisel v rozmezi a-b pro cele cisla
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist6(a, b);

    return dist6(rng);
}