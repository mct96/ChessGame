#pragma once

#include <string>

namespace ch {

struct CCoordinate {
    int i;
    int j;

    operator std::string() const
    {
        return std::string{"["} + std::to_string(i)
                + ", " + std::to_string(j) + "]"  ;
    }

    inline bool operator==(const CCoordinate& other) const
    {
        return i == other.i && j == other.j;
    }

    inline bool operator!=(const CCoordinate& other) const
    {
        return !((*this) == other);
    }
};

}