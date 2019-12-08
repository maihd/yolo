#pragma once

template <typename T, int count>
constexpr int CountOf(const T (&_)[count])
{
    return count;
}

