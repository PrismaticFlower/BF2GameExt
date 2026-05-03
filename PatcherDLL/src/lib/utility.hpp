#pragma once

/// @brief Return the min (by value!) of two arguments.
/// @tparam T The type of arguments.
/// @param l The first value.
/// @param r The second value.
/// @return The minimum of the two arguments.
template<typename T>
inline constexpr auto min(const T& a, const T& b) -> T
{
   return a < b ? a : b;
}

/// @brief Return the max (by value!) of two arguments.
/// @tparam T The type of arguments.
/// @param l The first value.
/// @param r The second value.
/// @return The maximum of the two arguments.
template<typename T>
inline constexpr auto max(const T& a, const T& b) -> T
{
   return a > b ? a : b;
}
