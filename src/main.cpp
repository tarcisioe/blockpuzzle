#include <iostream>

#include "matrix.hpp"
#include "piece.hpp"


template <typename T>
void print_matrix(geom::Matrix<T, 4, 4> const& m, geom::Rotation r)
{
    for (auto i = 0; i < 4; ++i) {
        for (auto j = 0; j < 4; ++j) {
            std::cout << m[{{i, j}, r}] << ' ';
        }
        std::cout << '\n';
    }
}


int main()
{
    for (auto& piece: game::pieces) {
        print_matrix(piece.shape(), geom::Rotation::R0);
        std::cout << "===============\n";
    }
}
