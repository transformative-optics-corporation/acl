/**
 *    \copyright Copyright 2021 Aqueti, Inc. All rights reserved.
 *    \license This project is released under the MIT Public License.
**/

/******************************************************************************
 *
 * \file RandomIDGenerator.hpp
 * \author Andrew Ferg
 * \brief Class of static functions that generate random strings and integers
 *
 *****************************************************************************/
#pragma once

#include <string>

namespace acl {

class RandomIDGenerator
{
    public:
        static std::string genAlphanumericString(unsigned int len=8);
        static std::string genNumericString(unsigned int len=8);
        static std::string genHexadecimalString(unsigned int len=8, std::string seed="");
        static uint64_t genUint64();
};

}//end namespace acl
