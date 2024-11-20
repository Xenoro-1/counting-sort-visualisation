#ifndef FILLER_H
#define FILLER_H 

#include <vector>
#include <random>

class Filler {
    private:
        int minValue, maxValue;

    public:
        Filler(int minValue = 1, int maxValue = 100) : minValue(minValue), maxValue(maxValue) {
        }

        void fill(
            std::vector<int>& numbers, 
            std::size_t size) { 

                std::random_device rd;
                std::uniform_int_distribution<> dist(minValue, maxValue);

                for (std::size_t i = 0; i < size; i++) {
                    numbers.push_back(dist(rd));
                }
        }
};

#endif