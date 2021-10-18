/**
 *    \copyright Copyright 2021 Aqueti, Inc. All rights reserved.
 *    \license This project is released under the MIT Public License.
**/

#include <string>
#include <vector>

namespace acl
{
  /**
   * \brief Function that parses a string based on a delimiter
   **/ 
   std::vector<std::string> stringParser( std::string input, std::string delim );

 /**
  * \brief Function that converts an integer to a string of given precision
  **/
  std::string intToString( uint64_t value, int width );
}
