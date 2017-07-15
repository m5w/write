// This file is part of write.
//
// write is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// write is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with write.  If not, see <http://www.gnu.org/licenses/>.

#include "write.h"

namespace lttoolbox {

template <std::size_t n>
std::ostream &Write<n>::write(std::ostream &os, const std::uint64_t &x) {

#if ENABLE_DEBUG

  std::cerr << "-----BEGIN std::ostream &\n"
               "Write<"
            << n << ">::write(std::ostream &os, const std::uint64_t &x)-----\n"
                    "x = "
            << x << "\n";

#endif

  if (x > maximum_x)
    return Write<n + 1>::write(os, x);

  char s[n];
  copy_least_significant_bytes(s, maximum_s_index, n);
  s[0] |= mask;

#if ENABLE_DEBUG

  std::cerr << "-----END std::ostream &\n"
               "Write<"
            << n << ">::write(std::ostream &os, const std::uint64_t &x)-----\n";

#endif

  return os.write(s, n);
}

std::ostream &Write<1>::write(std::ostream &os, const std::uint64_t &x) {

#if ENABLE_DEBUG

  std::cerr
      << "-----BEGIN std::ostream &\n"
         "Write<1>::write(std::ostream &os, const std::uint64_t &x)-----\n"
         "x = "
      << x << "\n";

#endif

  if (x > maximum_x)
    return Write<2>::write(os, x);

#if ENABLE_DEBUG

  std::cerr
      << "-----END std::ostream &\n"
         "Write<1>::write(std::ostream &os, const std::uint64_t &x)-----\n";

#endif

  return os.put(x);
}

std::ostream &Write<9>::write(std::ostream &os, const std::uint64_t &x) {

#if ENABLE_DEBUG

  std::cerr
      << "-----BEGIN std::ostream &\n"
         "Write<9>::write(std::ostream &os, const std::uint64_t &x)-----\n"
         "x = "
      << x << "\n";

#endif

  char s[9];
  copy_least_significant_bytes(s + 1, 7, x);
  *s = mask;

#if ENABLE_DEBUG

  std::cerr
      << "-----END std::ostream &\n"
         "Write<9>::write(std::ostream &os, const std::uint64_t &x)-----\n";

#endif

  return os.write(s, 9);
}

void copy_least_significant_bytes(char *s, std::size_t maximum_s_index,
                                  std::uint64_t x) {
  for (;;) {
    unsigned char byte = x;
    s[maximum_s_index] = byte;

    if (maximum_s_index == 0)
      break;

    x >>= 8;
    --maximum_s_index;
  }
}

std::ostream &write(std::ostream &os, const std::uint64_t &x) {
  return Write<1>::write(os, x);
}

}  // end namespace lttoolbox