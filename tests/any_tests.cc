/*
 * File  : any_tests.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/05 18:44:08
 */

#include "ijk/base/any.h"

#include <catch.hpp>

using namespace ijk;

TEST_CASE("any: Allows to default construct any") {
    any a;

    REQUIRE_FALSE(a.has_value());
}

TEST_CASE("any: Allows to copy-construct from any") {
    any a(7);

    any b(a);

    REQUIRE(a.has_value());
    REQUIRE(b.has_value());
    REQUIRE(any_cast<int>(b) == 7);
}
