#pragma once

#include "color.hpp"
#include "type.hpp"

namespace ch {

class CLocation {
public:
    CLocation(EType type, EColor color);
    CLocation();
    ~CLocation();

    static CLocation emptyLocation();

    void setPiece(EType type, EColor color);

    EType getType() const;
    EColor getColor() const;

    void removePiece();

    bool isEmpty() const;

protected:

private:
    bool _isEmpty;

    EType _type;
    EColor _color;
};

}