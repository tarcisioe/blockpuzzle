#ifndef GEOM_MATRIX_HPP
#define GEOM_MATRIX_HPP

#include <array>


namespace geom {

enum class Rotation {
    R0,
    R90,
    R180,
    R270,
};

struct Position2D {
    int row;
    int column;
};

struct MatrixPosition {
    Position2D position;
    Rotation rotation;
};

template <typename T, int Rows, int Columns>
class Matrix {
public:
    constexpr static auto unsigned_size = static_cast<unsigned>(Rows * Columns);
    using Contents = std::array<T, unsigned_size>;

    constexpr Matrix(Contents const& contents):
        contents_{contents}
    {}

    constexpr T& operator[](MatrixPosition const& position)
    {
        return contents_[index_of(position)];
    }

    constexpr T const& operator[](MatrixPosition const& position) const
    {
        return contents_[index_of(position)];
    }

private:
    struct RotationConstants {
        int base;
        int row_multiplier;
        int column_multiplier;
    };

    constexpr static std::size_t index_of(MatrixPosition position)
    {
        auto const constants = rotation_constants(position.rotation);

        return static_cast<std::size_t>(
            constants.base +
            constants.row_multiplier * position.position.row +
            constants.column_multiplier * position.position.column
        );
    }

    constexpr static RotationConstants rotation_constants(Rotation amount)
    {
        switch (amount) {
            case Rotation::R0: {
                return {
                    0,
                    Columns,
                    1
                };
            }
            case Rotation::R90: {
                return {
                    (Rows-1)*Columns,
                    1,
                    -Columns
                };
            }
            case Rotation::R180: {
                return {
                    (Rows*Columns) - 1,
                    -Columns,
                    -1
                };
            }
            case Rotation::R270: {
                return {
                    Columns - 1,
                    -1,
                    Columns
                };
            }
            default:
                return {
                    0, 0, 0
                };
        }
    }

    Contents contents_;
};

}

#endif
