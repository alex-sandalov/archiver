#include "HammingCode.h"

std::vector<uint8_t> EncodeHamming(const std::vector<uint8_t>& array, int len_array) {
    int extra_bits = HammingCodeExtraBits(len_array);
	int size_bits = len_array + extra_bits;
	std::vector<uint8_t> result(size_bits / kSizeOneByte + 1, 0);
	int index = 0;
	int index_array = 0;
	while (index < size_bits) {
		if (!((index + 1) & index)) {
            ++index;
            continue;
        }
		bool value = array[index_array / kSizeOneByte] & (1 << (kSizeOneByte - 1 - index_array % kSizeOneByte));
		uint8_t bit = (value << (kSizeOneByte - 1 - index % kSizeOneByte));
		result[index / kSizeOneByte] |= bit;
		++index_array;
		++index;
	}
	index = 0;
	while (index < size_bits) {
		bool value = result[index / kSizeOneByte] & (1 << (kSizeOneByte - 1 - index % kSizeOneByte));
        if (!((index + 1) & index)) {
            ++index;
            continue;
        }
        int r = 0;
        while ((1 << r) - 1 < index) {
            int power_two = (1 << r) - 1;
            int index_byte = power_two / kSizeOneByte;
            int index_bit = kSizeOneByte - 1 - power_two % kSizeOneByte;
            if ((r && (index + 1) / (power_two + 1) % 2) || (!r && (index + 1) % 2)) {
                result[index_byte] ^= (value << index_bit); 
            }
            ++r;
        }
		++index;
	}
	return result;
}

std::vector<uint8_t> DecodeHamming(const std::vector<uint8_t>& array, int len_array) {
	int extra_bits = HammingCodeExtraBits(len_array);
	std::vector<uint8_t> result(len_array / kSizeOneByte + static_cast<bool>(len_array % kSizeOneByte), 0);
	int index = 0;
	int index_array = 0;
	while (index < len_array + extra_bits) {
	    if (!((index + 1) & index)) {
	        ++index;
	        continue;
	    }
	    int index_byte = index / kSizeOneByte;
	    int index_bit = kSizeOneByte - 1 - index % kSizeOneByte;
	    bool value = array[index_byte] & (1 << index_bit);
	    index_byte = index_array / kSizeOneByte;
	    index_bit = kSizeOneByte - 1 - index_array % kSizeOneByte;
	    result[index_byte] |= (value << index_bit);
	    ++index_array;
	    ++index;
	}
	return result;
}

bool IsCorrect(const std::vector<uint8_t>& array) {
	int len_array = array.size() * kSizeOneByte;
	std::vector<uint8_t> result = array;
	int index = 0;
	int index_array = 0;
	while (index < len_array) {
		bool value = result[index / kSizeOneByte] & (1 << (kSizeOneByte - 1 - index % kSizeOneByte));
        if (!((index + 1) & index)) {
            ++index;
            continue;
        }
        int r = 0;
        while ((1 << r) - 1 < index) {
            int power_two = (1 << r) - 1;
            int index_byte = power_two / kSizeOneByte;
            int index_bit = kSizeOneByte - 1 - power_two % kSizeOneByte;
            if ((r && (index + 1) / (power_two + 1) % 2) || (!r && (index + 1) % 2)) {
                result[index_byte] ^= (value << index_bit); 
            }
            ++r;
        }
		++index;
	}

	int r = 0;
	while ((1 << r) - 1 < len_array) {
		int power_two = (1 << r) - 1;
		int index_byte = power_two / kSizeOneByte;
        int index_bit = kSizeOneByte - 1 - power_two % kSizeOneByte;
		bool value = result[index_byte] & (1 << index_bit);
		if (value) {
			return false;
		}
		++r;
	}

	return true;
}

int HammingCodeExtraBits(int lenght_array) {
	int r = 1;
	while ((1 << r) < lenght_array + r + 1) {
		r++;
	}
	return r;
}
