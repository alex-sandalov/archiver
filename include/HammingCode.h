#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <iostream>

static const int kSizeOneByte = 8;

std::vector<uint8_t> EncodeHamming(const std::vector<uint8_t>& array, int len_array);

std::vector<uint8_t> DecodeHamming(const std::vector<uint8_t>& array, int len_array);

bool IsCorrect(const std::vector<uint8_t>& array);

int HammingCodeExtraBits(int lenght_array);