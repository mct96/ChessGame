#pragma once

#include <iterator>

namespace ch
{



template <typename T, template<typename> class V>
class CIterator: public std::iterator<std::input_iterator_tag, T>
{
public:
    CIterator(V<T>& container, std::size_t pos = 0)
        :
        _container{container},
        _pos{pos}
    {

    }

    CIterator(const CIterator& other) = default;

    CIterator& operator++() {
        ++_pos;
        return *this;
    }

    CIterator& operator++(int) {
        CIterator it{*this};
        _pos++;
        return it;
    }

    T& operator*() {
        return _container[_pos];
    }

    bool operator!=(const CIterator& other) {
        return _pos != other._pos;
    }

private:
    std::size_t _pos;
    V<T>& _container;
};

}