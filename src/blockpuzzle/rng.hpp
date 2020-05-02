#ifndef BLOCKPUZZLE_RANDOM_HPP
#define BLOCKPUZZLE_RANDOM_HPP

#include <algorithm>
#include <array>
#include <random>

namespace blockpuzzle {

class RNG {
public:
    RNG(std::default_random_engine engine): engine_{std::move(engine)} {}

    std::array<std::size_t, 7> random_piece_sequence()
    {
        auto sequence = std::array<std::size_t, 7>{0, 1, 2, 3, 4, 5, 6};

        std::shuffle(begin(sequence), end(sequence), engine_);

        return sequence;
    }

private:
    std::default_random_engine engine_;
};

}

#endif
