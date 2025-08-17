#pragma once

#include <utility>
namespace euclid {

int euclid_classic(int a, int b);

int euclid_remainder(int a, int b);

int euclid_recursive(int a, int b);

int euclid_elipsis(int, ...);

std::pair<int, int> euclid_Besu(int a, int b);
} // namespace euclid
