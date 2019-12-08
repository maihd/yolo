#pragma once

template <typename T, int count>
constexpr int CountOf(const T (&_)[count])
{
    return count;
}

inline int NextPOT(int x)
{
    int result = x - 1;

    result = result | (result >> 1);
    result = result | (result >> 2);
    result = result | (result >> 4);
    result = result | (result >> 8);
    result = result | (result >> 16);

    return result + 1;
}

