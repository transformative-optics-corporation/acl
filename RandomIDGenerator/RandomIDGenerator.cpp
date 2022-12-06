/**
 *    \copyright Copyright 2021 Aqueti, Inc. All rights reserved.
 *    \license This project is released under the MIT Public License.
**/

/******************************************************************************
 *
 * \file RandomIDGenerator.cpp
 * \author Andrew Ferg
 * \brief Class methods for RandomIDGenerator
 *
 *****************************************************************************/

#include <random>
#include <functional>
#include "Timer.h"
#include "RandomIDGenerator.hpp"

namespace acl {

std::string RandomIDGenerator::genAlphanumericString(unsigned int len)
{
    std::string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::srand(acl::getUsecTime()); //use current time as seed for RNG

    std::string str;
    str.resize(len);
    int modVal = alphabet.size();
    for (size_t i = 0; i < len; i++) {
        str[i] = alphabet[std::rand() % modVal];
    }

    return str;
}

std::string RandomIDGenerator::genNumericString(unsigned int len)
{
    std::string alphabet = "0123456789";

    std::srand(acl::getUsecTime()); //use current time as seed for RNG

    std::string str;
    int pos;
    while (str.size() <= len) {
        int randomInt = std::rand();
        pos = randomInt % alphabet.length();
        str += alphabet.substr(pos,1);
    }

    return str;
}

std::string RandomIDGenerator::genHexadecimalString(unsigned int len, std::string seed)
{
    std::string alphabet = "0123456789abcdef";

    if (seed.empty()) {
        std::srand(acl::getUsecTime()); //use current time as seed for RNG
    } else {
        std::srand(std::hash<std::string>{}(seed)); // use hash to convert provided seed to unsigned
    }

    std::string str;
    int pos;
    while (str.size() <= len) {
        int randomInt = std::rand();
        pos = randomInt % alphabet.length();
        str += alphabet.substr(pos,1);
    }

    return str;
}

uint64_t RandomIDGenerator::genUint64()
{
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<uint64_t> distribution(0, UINT64_MAX);
    return distribution(generator);
}

}//end namespace acl
