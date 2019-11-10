#pragma once

#include <string>

namespace ch {

struct CPosition {
    int i;
    int j;

    operator std::string() const
    {
        return std::string{"["} + std::to_string(i)
                + ", " + std::to_string(j) + "]"  ;
    }

    inline bool operator==(const CPosition& other) const
    {
        return i == other.i && j == other.j;
    }
};

}