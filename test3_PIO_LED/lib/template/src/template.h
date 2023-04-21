// template.h
#ifndef TEMPLATE_H // include guard
#define TEMPLATE_H
// #pragma once

#include <Arduino.h>

// compute the length of an array
template <typename T, size_t N>
size_t array_length(T (&arr)[N]) {
  return N;
}

#endif