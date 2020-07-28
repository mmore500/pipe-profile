#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "../source/safe_compare.h"

#include <limits>

TEST_CASE("Test safe_less<signed, signed>") {

  // shorts
  for (short i = -100; i < 100; ++i) {
    REQUIRE( safe_less(i, i+2) );
    REQUIRE( safe_less(i, i+1) );
    REQUIRE( !safe_less(i, i) );
    REQUIRE( !safe_less(i+1, i) );
    REQUIRE( !safe_less(i+2, i) );
    REQUIRE( safe_less(i, (int) 100) );
    REQUIRE( safe_less( (long) -101, i) );
  }

  // ints
  for (int i = -100; i < 100; ++i) {
    REQUIRE( safe_less(i, i+2) );
    REQUIRE( safe_less(i, i+1) );
    REQUIRE( !safe_less(i, i) );
    REQUIRE( !safe_less(i+1, i) );
    REQUIRE( !safe_less(i+2, i) );
    REQUIRE( safe_less(i, (short) 100) );
    REQUIRE( safe_less( (short) -101, i) );
  }

}

TEST_CASE("Test safe_less<unsigned, unsigned>") {

  // big size_t
  for (
    size_t i = std::numeric_limits<size_t>::max() - 99;
    i < std::numeric_limits<size_t>::max();
    ++i
  ) {
    REQUIRE( safe_less(i, i+1) );
    REQUIRE( !safe_less(i, i) );
    REQUIRE( !safe_less(i+1, i) );
    REQUIRE( safe_less((unsigned char) 0, i) );
  }

  // size_t wraparound
  REQUIRE( safe_less( (size_t) 0,  (size_t) -1) );
  REQUIRE( safe_less( (size_t) 0,  (size_t) 1) );
  REQUIRE( !safe_less( (size_t) -1,  (size_t) 0) );
  REQUIRE( !safe_less( (size_t) 1,  (size_t) 0) );
  REQUIRE( !safe_less( (size_t) -1,  (size_t) -1) );
  REQUIRE( !safe_less( (size_t) 0,  (size_t) 0) );

  // small size_t
  for (size_t i = 0; i < 99; ++i) {
    REQUIRE( safe_less(i, i+1) );
    REQUIRE( !safe_less(i, i) );
    REQUIRE( !safe_less(i+1, i) );
    REQUIRE( safe_less(i, (unsigned char) 100) );
  }

  // unsigned char
  for (unsigned char i = 0; i < 99; ++i) {
    REQUIRE( safe_less(i, i+1) );
    REQUIRE( !safe_less(i, i) );
    REQUIRE( !safe_less(i+1, i) );
    REQUIRE( safe_less(i, (size_t) 100) );
  }

}

TEST_CASE("Test safe_less<signed, unsigned>") {

  REQUIRE( safe_less( (short) -1, (size_t) 0) );
  REQUIRE( safe_less( (int) -1, (size_t) 0) );
  REQUIRE( safe_less( (int) 0, (size_t) 1) );

  REQUIRE( safe_less( (int) 0, (size_t) -1) );
  REQUIRE( !safe_less( (int) 1, (unsigned short) 0) );
  REQUIRE( !safe_less( (int) 2, (size_t) 1) );
  REQUIRE( !safe_less( (short) 42, (size_t) 25) );


  REQUIRE( safe_less( (int) -1, (unsigned short) -1) );
  REQUIRE( !safe_less( (short) 0, (size_t) 0) );
  REQUIRE( !safe_less( (int) 1, (size_t) 1) );

}

TEST_CASE("Test safe_less<unsigned, signed>") {

  REQUIRE( !safe_less( (size_t) 0, (short) -1) );
  REQUIRE( !safe_less( (size_t) 0, (int) -1) );
  REQUIRE( !safe_less( (size_t) 1, (int) 0) );

  REQUIRE( !safe_less( (size_t) -1, (int) 0) );
  REQUIRE( safe_less( (unsigned short) 0, (int) 1) );
  REQUIRE( safe_less( (size_t) 1, (int) 2) );
  REQUIRE( safe_less( (size_t) 25, (short) 42) );

  REQUIRE( !safe_less( (unsigned short) -1, (int) -1) );
  REQUIRE( !safe_less( (size_t) 0, (short) 0) );
  REQUIRE( !safe_less( (size_t) 1, (int) 1) );

}

TEST_CASE("Test safe_greater<signed, signed>") {

  // shorts
  for (short i = -100; i < 100; ++i) {
    REQUIRE( !safe_greater(i, i+2) );
    REQUIRE( !safe_greater(i, i+1) );
    REQUIRE( !safe_greater(i, i) );
    REQUIRE( safe_greater(i+1, i) );
    REQUIRE( safe_greater(i+2, i) );
    REQUIRE( !safe_greater(i, (int) 100) );
    REQUIRE( !safe_greater( (long) -101, i) );
  }

  // ints
  for (int i = -100; i < 100; ++i) {
    REQUIRE( !safe_greater(i, i+2) );
    REQUIRE( !safe_greater(i, i+1) );
    REQUIRE( !safe_greater(i, i) );
    REQUIRE( safe_greater(i+1, i) );
    REQUIRE( safe_greater(i+2, i) );
    REQUIRE( !safe_greater(i, (short) 100) );
    REQUIRE( !safe_greater( (short) -101, i) );
  }

}

TEST_CASE("Test safe_greater<unsigned, unsigned>") {

  // big size_t
  for (
    size_t i = std::numeric_limits<size_t>::max() - 99;
    i < std::numeric_limits<size_t>::max();
    ++i
  ) {
    REQUIRE( !safe_greater(i, i+1) );
    REQUIRE( !safe_greater(i, i) );
    REQUIRE( safe_greater(i+1, i) );
    REQUIRE( !safe_greater((unsigned char) 0, i) );
  }

  // size_t wraparound
  REQUIRE( !safe_greater( (size_t) 0,  (size_t) -1) );
  REQUIRE( !safe_greater( (size_t) 0,  (size_t) 1) );
  REQUIRE( safe_greater( (size_t) -1,  (size_t) 0) );
  REQUIRE( safe_greater( (size_t) 1,  (size_t) 0) );
  REQUIRE( !safe_greater( (size_t) -1,  (size_t) -1) );
  REQUIRE( !safe_greater( (size_t) 0,  (size_t) 0) );

  // small size_t
  for (size_t i = 0; i < 99; ++i) {
    REQUIRE( !safe_greater(i, i+1) );
    REQUIRE( !safe_greater(i, i) );
    REQUIRE( safe_greater(i+1, i) );
    REQUIRE( !safe_greater(i, (unsigned char) 100) );
  }

  // unsigned char
  for (unsigned char i = 0; i < 99; ++i) {
    REQUIRE( !safe_greater(i, i+1) );
    REQUIRE( !safe_greater(i, i) );
    REQUIRE( safe_greater(i+1, i) );
    REQUIRE( !safe_greater(i, (size_t) 100) );
  }

}

TEST_CASE("Test safe_greater<signed, unsigned>") {

  REQUIRE( !safe_greater( (short) -1, (size_t) 0) );
  REQUIRE( !safe_greater( (int) -1, (size_t) 0) );
  REQUIRE( !safe_greater( (int) 0, (size_t) 1) );

  REQUIRE( !safe_greater( (int) 0, (size_t) -1) );
  REQUIRE( safe_greater( (int) 1, (unsigned short) 0) );
  REQUIRE( safe_greater( (int) 2, (size_t) 1) );
  REQUIRE( safe_greater( (short) 42, (size_t) 25) );


  REQUIRE( !safe_greater( (int) -1, (unsigned short) -1) );
  REQUIRE( !safe_greater( (short) 0, (size_t) 0) );
  REQUIRE( !safe_greater( (int) 1, (size_t) 1) );

}

TEST_CASE("Test safe_greater<unsigned, signed>") {

  REQUIRE( safe_greater( (size_t) 0, (short) -1) );
  REQUIRE( safe_greater( (size_t) 0, (int) -1) );
  REQUIRE( safe_greater( (size_t) 1, (int) 0) );

  REQUIRE( safe_greater( (size_t) -1, (int) 0) );
  REQUIRE( !safe_greater( (unsigned short) 0, (int) 1) );
  REQUIRE( !safe_greater( (size_t) 1, (int) 2) );
  REQUIRE( !safe_greater( (size_t) 25, (short) 42) );

  REQUIRE( safe_greater( (unsigned short) -1, (int) -1) );
  REQUIRE( !safe_greater( (size_t) 0, (short) 0) );
  REQUIRE( !safe_greater( (size_t) 1, (int) 1) );

}

TEST_CASE("Test safe_equal<signed, signed>") {

  // shorts
  for (short i = -100; i < 100; ++i) {
    REQUIRE( !safe_equal(i, i+2) );
    REQUIRE( !safe_equal(i, i+1) );
    REQUIRE( safe_equal(i, i) );
    REQUIRE( !safe_equal(i+1, i) );
    REQUIRE( !safe_equal(i+2, i) );
    REQUIRE( !safe_equal(i, (int) 100) );
    REQUIRE( !safe_equal( (long) -101, i) );
  }

  // ints
  for (int i = -100; i < 100; ++i) {
    REQUIRE( !safe_equal(i, i+2) );
    REQUIRE( !safe_equal(i, i+1) );
    REQUIRE( safe_equal(i, i) );
    REQUIRE( !safe_equal(i+1, i) );
    REQUIRE( !safe_equal(i+2, i) );
    REQUIRE( !safe_equal(i, (short) 100) );
    REQUIRE( !safe_equal( (short) -101, i) );
  }

}

TEST_CASE("Test safe_equal<unsigned, unsigned>") {

  REQUIRE(
    !safe_equal( (unsigned int) -1, (unsigned short) -1)
  );
  REQUIRE(
    !safe_equal( (unsigned short) -1, (unsigned int) -1)
  );

  // big size_t
  for (
    size_t i = std::numeric_limits<size_t>::max() - 99;
    i < std::numeric_limits<size_t>::max();
    ++i
  ) {
    REQUIRE( !safe_equal(i, i+1) );
    REQUIRE( safe_equal(i, i) );
    REQUIRE( !safe_equal(i+1, i) );
    REQUIRE( !safe_equal((unsigned char) 0, i) );
  }

  // size_t wraparound
  REQUIRE( !safe_equal( (size_t) 0,  (size_t) -1) );
  REQUIRE( !safe_equal( (size_t) 0,  (size_t) 1) );
  REQUIRE( !safe_equal( (size_t) -1,  (size_t) 0) );
  REQUIRE( !safe_equal( (size_t) 1,  (size_t) 0) );
  REQUIRE( safe_equal( (size_t) -1,  (size_t) -1) );
  REQUIRE( safe_equal( (size_t) 0,  (size_t) 0) );

  // small size_t
  for (size_t i = 0; i < 99; ++i) {
    REQUIRE( !safe_equal(i, i+1) );
    REQUIRE( safe_equal(i, i) );
    REQUIRE( !safe_equal(i+1, i) );
    REQUIRE( !safe_equal(i, (unsigned char) 100) );
  }

  // unsigned char
  for (unsigned char i = 0; i < 99; ++i) {
    REQUIRE( !safe_equal(i, i+1) );
    REQUIRE( safe_equal(i, i) );
    REQUIRE( !safe_equal(i+1, i) );
    REQUIRE( !safe_equal(i, (size_t) 100) );
  }

}

TEST_CASE("Test safe_equal<signed, unsigned>") {

  REQUIRE(
    safe_equal( (int) -1, (short) -1)
  );
  REQUIRE(
    safe_equal( (short) -1, (int) -1)
  );

  REQUIRE(
    !safe_equal( (short) -1, std::numeric_limits<unsigned short>::max())
  );
  REQUIRE(
    !safe_equal( (short) -1, std::numeric_limits<unsigned int>::max())
  );
  REQUIRE(
    !safe_equal( (int) -2, std::numeric_limits<unsigned int>::max() - 1)
  );
  REQUIRE(
    !safe_equal( (int) -2, std::numeric_limits<unsigned short>::max() - 1)
  );

  REQUIRE( !safe_equal( (short) -1, (size_t) 0) );
  REQUIRE( !safe_equal( (int) -1, (size_t) 0) );
  REQUIRE( !safe_equal( (int) 0, (size_t) 1) );

  REQUIRE( !safe_equal( (int) 0, (size_t) -1) );
  REQUIRE( !safe_equal( (int) 1, (unsigned short) 0) );
  REQUIRE( !safe_equal( (int) 2, (size_t) 1) );
  REQUIRE( !safe_equal( (short) 42, (size_t) 25) );


  REQUIRE( !safe_equal( (int) -1, (unsigned short) -1) );
  REQUIRE( safe_equal( (short) 0, (size_t) 0) );
  REQUIRE( safe_equal( (int) 1, (size_t) 1) );

}

TEST_CASE("Test safe_equal<unsigned, signed>") {

  REQUIRE(
    !safe_equal( std::numeric_limits<unsigned short>::max(), (short) -1)
  );
  REQUIRE(
    !safe_equal( std::numeric_limits<unsigned int>::max(), (short) -1)
  );
  REQUIRE(
    !safe_equal( std::numeric_limits<unsigned int>::max() - 1, (int) -2)
  );
  REQUIRE(
    !safe_equal( std::numeric_limits<unsigned short>::max() - 1, (int) -2)
  );

  REQUIRE( !safe_equal( (size_t) 0, (short) -1) );
  REQUIRE( !safe_equal( (size_t) 0, (int) -1) );
  REQUIRE( !safe_equal( (size_t) 1, (int) 0) );

  REQUIRE( !safe_equal( (size_t) -1, (int) 0) );
  REQUIRE( !safe_equal( (unsigned short) 0, (int) 1) );
  REQUIRE( !safe_equal( (size_t) 1, (int) 2) );
  REQUIRE( !safe_equal( (size_t) 25, (short) 42) );

  REQUIRE( !safe_equal( (unsigned short) -1, (int) -1) );
  REQUIRE( safe_equal( (size_t) 0, (short) 0) );
  REQUIRE( safe_equal( (size_t) 1, (int) 1) );

}