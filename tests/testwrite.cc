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

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <sstream>

#define BOOST_TEST_MODULE testwrite
#include <boost/test/included/unit_test.hpp>

#include "write.h"

template <class T> auto ord(const T &c) -> decltype(+c) { return +c; }

template <class InputIterator>
void print(InputIterator first, InputIterator last) {
  const auto &flags = std::cerr.setf(std::cerr.hex, std::cerr.basefield);

  // Set next because it only affects output when the field width of
  // ``std::cerr`` is set.  The field width is reset after each ASCII value is
  // printed, so, while this does have an effect on the representation of all
  // the ASCII values, it does not affect them "contiguously" -- it has an
  // effect on one ASCII value, then no longer affects the output, and then
  // affects another ASCII value.
  const auto &fill = std::cerr.fill('0');

  // A space must be printed after every other ASCII value -- except the last
  // one.  Therefore, the last ASCII value must be printed after the loop, and
  // the loop must print each of the preceeding pairs of ASCII values, each
  // pair followed by a space.  The loop must end with one element remaining.
  --last;

  for (; first != last; ++first) {
    std::cerr << std::setw(2) << ord(*first);
    ++first;

    if (first == last)
      break;

    std::cerr << std::setw(2) << ord(*first);
    std::cerr << ' ';
  }

  std::cerr << std::setw(2) << ord(*first);

  // Reset the properties of ``std::cerr`` in the order in which they were set.
  std::cerr.fill(fill);
  std::cerr.flags(flags);
}

template <std::size_t N>
bool test_write_write(const std::uint64_t &x, std::array<char, N> s) {
  std::stringstream os;
  lttoolbox::write(os, x);
  const std::string &str = os.str();

#if ENABLE_DEBUG

  std::cerr << "diff s str\n";

#endif

  if (std::equal(str.cbegin(), str.cend(), s.cbegin(), s.cend()))
    return true;

#if ENABLE_DEBUG

  std::cerr << "< ";
  print(s.cbegin(), s.cend());
  std::cerr << "\n"
               "---\n"
               "> ";
  print(str.cbegin(), str.cend());
  std::cerr << '\n';

#endif

  return false;
}

BOOST_AUTO_TEST_CASE(testwrite_test_write_write) {
  BOOST_CHECK(test_write_write(0x00, std::array<char, 1>({'\x00'})));
  BOOST_CHECK(test_write_write(0x40, std::array<char, 1>({'\x40'})));
  BOOST_CHECK(test_write_write(0x7f, std::array<char, 1>({'\x7f'})));
  BOOST_CHECK(test_write_write(0x80, std::array<char, 2>({'\x80', '\x80'})));
  BOOST_CHECK(
      test_write_write(0x20'10, std::array<char, 2>({'\xa0', '\x10'})));
  BOOST_CHECK(
      test_write_write(0x3f'ff, std::array<char, 2>({'\xbf', '\xff'})));
  BOOST_CHECK(test_write_write(0x40'00,
                               std::array<char, 3>({'\xc0', '\x40', '\x00'})));
  BOOST_CHECK(test_write_write(0x10'08'04,
                               std::array<char, 3>({'\xd0', '\x08', '\x04'})));
  BOOST_CHECK(test_write_write(0x1f'ff'ff,
                               std::array<char, 3>({'\xdf', '\xff', '\xff'})));
  BOOST_CHECK(test_write_write(
      0x20'00'00, std::array<char, 4>({'\xe0', '\x20', '\x00', '\x00'})));
  BOOST_CHECK(test_write_write(
      0x08'04'02'01, std::array<char, 4>({'\xe8', '\x04', '\x02', '\x01'})));
  BOOST_CHECK(test_write_write(
      0x0f'ff'ff'ff, std::array<char, 4>({'\xef', '\xff', '\xff', '\xff'})));
  BOOST_CHECK(test_write_write(
      0x10'00'00'00,
      std::array<char, 5>({'\xf0', '\x10', '\x00', '\x00', '\x00'})));
  BOOST_CHECK(test_write_write(
      0x04'02'01'80'40,
      std::array<char, 5>({'\xf4', '\x02', '\x01', '\x80', '\x40'})));
  BOOST_CHECK(test_write_write(
      0x07'ff'ff'ff'ff,
      std::array<char, 5>({'\xf7', '\xff', '\xff', '\xff', '\xff'})));
  BOOST_CHECK(test_write_write(
      0x08'00'00'00'00,
      std::array<char, 6>({'\xf8', '\x08', '\x00', '\x00', '\x00', '\x00'})));
  BOOST_CHECK(test_write_write(
      0x02'01'80'40'20'10,
      std::array<char, 6>({'\xfa', '\x01', '\x80', '\x40', '\x20', '\x10'})));
  BOOST_CHECK(test_write_write(
      0x03'ff'ff'ff'ff'ff,
      std::array<char, 6>({'\xfb', '\xff', '\xff', '\xff', '\xff', '\xff'})));
  BOOST_CHECK(test_write_write(
      0x04'00'00'00'00'00, std::array<char, 7>({'\xfc', '\x04', '\x00', '\x00',
                                                '\x00', '\x00', '\x00'})));
  BOOST_CHECK(
      test_write_write(0x01'80'40'20'10'08'04,
                       std::array<char, 7>({'\xfd', '\x80', '\x40', '\x20',
                                            '\x10', '\x08', '\x04'})));
  BOOST_CHECK(
      test_write_write(0x01'ff'ff'ff'ff'ff'ff,
                       std::array<char, 7>({'\xfd', '\xff', '\xff', '\xff',
                                            '\xff', '\xff', '\xff'})));
  BOOST_CHECK(
      test_write_write(0x02'00'00'00'00'00'00,
                       std::array<char, 8>({'\xfe', '\x02', '\x00', '\x00',
                                            '\x00', '\x00', '\x00', '\x00'})));
  BOOST_CHECK(
      test_write_write(0x80'40'20'10'08'04'02,
                       std::array<char, 8>({'\xfe', '\x80', '\x40', '\x20',
                                            '\x10', '\x08', '\x04', '\x02'})));
  BOOST_CHECK(
      test_write_write(0xff'ff'ff'ff'ff'ff'ff,
                       std::array<char, 8>({'\xfe', '\xff', '\xff', '\xff',
                                            '\xff', '\xff', '\xff', '\xff'})));
  BOOST_CHECK(test_write_write(
      0x01'00'00'00'00'00'00'00,
      std::array<char, 9>({'\xff', '\x01', '\x00', '\x00', '\x00', '\x00',
                           '\x00', '\x00', '\x00'})));
  BOOST_CHECK(test_write_write(
      0x80'40'20'10'08'04'02'01,
      std::array<char, 9>({'\xff', '\x80', '\x40', '\x20', '\x10', '\x08',
                           '\x04', '\x02', '\x01'})));
  BOOST_CHECK(test_write_write(
      0xff'ff'ff'ff'ff'ff'ff'ff,
      std::array<char, 9>({'\xff', '\xff', '\xff', '\xff', '\xff', '\xff',
                           '\xff', '\xff', '\xff'})));
}
