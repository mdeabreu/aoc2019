#include "day8.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <map>

typedef int Pixel;
typedef std::vector<Pixel> Row;
typedef std::vector<Row> Layer;
typedef std::vector<Layer> Image;

Image process(const std::string &input, const unsigned int &width, const unsigned int &height)
{
    Row row;
    Layer layer;
    Image image;

    for (auto c : input)
    {
        if (layer.size() == height)
        {
            image.push_back(layer);
            layer.clear();
        }

        if (row.size() == width)
        {
            layer.push_back(row);
            row.clear();
        }

        row.push_back(atoi(&c));
    }

    layer.push_back(row);
    image.push_back(layer);
    return image;
}

int checksum(const Image &image)
{
    std::map<int, std::pair<int, int>> zeros_per_layer;
    for (auto layer : image)
    {
        int num_zeros{0};
        int num_ones{0};
        int num_twos{0};
        for (auto row : layer)
        {
            num_zeros += std::count(row.begin(), row.end(), 0);
            num_ones += std::count(row.begin(), row.end(), 1);
            num_twos += std::count(row.begin(), row.end(), 2);
        }

        zeros_per_layer[num_zeros] = std::make_pair(num_ones, num_twos);
    }

    auto nums = (*zeros_per_layer.begin()).second;
    return nums.first * nums.second;
}

void print_image(const Image &image)
{
    for (auto layer : image)
    {
        for (auto row : layer)
        {
            for (auto pixel : row)
            {
                std::cout << pixel;
            }

            std::cout << std::endl;
        }

        std::cout << std::endl;
        std::cout << std::endl;
    }
}

void process_test()
{
    std::string input = "123456789012";

    auto result = process(input, 3, 2);

    assert(result.size() == 2); // Two layers
    assert(result.at(0).size() == 2); // Two rows
    assert(result.at(0).at(0).size() == 3); // Three pixels

    assert(checksum(result) == 1);
}

int part1()
{
    auto result = process(kInput, 25, 6);

    int validation = checksum(result);
    assert(validation == 1548);
    return validation;
}

int main()
{
    process_test();
    std::cout << "Part 1: " << part1() << std::endl;
    return 0;
}