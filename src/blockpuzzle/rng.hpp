#ifndef BLOCKPUZZLE_RANDOM_HPP
#define BLOCKPUZZLE_RANDOM_HPP

#include <random>


namespace blockpuzzle {

class RNG {
public:
    RNG(std::default_random_engine engine):
        engine_{std::move(engine)}
    {}

    std::size_t get_int()
    {
        return distribution(engine_);
    }

private:
    std::default_random_engine engine_;
    std::uniform_int_distribution<std::size_t> distribution{0, 6};
};

}

#endif
