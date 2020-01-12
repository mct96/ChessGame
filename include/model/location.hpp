#pragma once

#include "piece.hpp"
#include "specialized_pieces.hpp"

namespace ch {

class CLocation {
public:
    CLocation();
    ~CLocation();

    void setPiece(std::shared_ptr<CPiece> piece);
    std::shared_ptr<CPiece> getPiece() const;

    void removePiece();

    bool isEmpty() const;

protected:

private:
    std::shared_ptr<CPiece> _piece;
};

}