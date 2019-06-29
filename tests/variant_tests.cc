/*
 * File  : variant_tests.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/06/29 13:54:08
 */

#include "ijk/base/variant.h"

#include <catch.hpp>

using namespace ijk;

class NoDefaultConstruct { NoDefaultConstruct(){} };

TEST_CASE( "variant: Allows non-default constructible as second and later type (first: int)" )
{
    variant<int, NoDefaultConstruct> var;

    REQUIRE(true);
}

