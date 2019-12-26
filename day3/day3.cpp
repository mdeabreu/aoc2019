#include <iostream>
#include <algorithm>
#include <cassert>
#include <limits>
#include "day3.hpp"

typedef std::pair<int, int> Coord;
typedef std::vector<Coord> Wire;

std::vector<std::string> tokenize(const std::string &input, const char &token = ',')
{
    std::vector<std::string> results;
    std::string current_chunk;

    // Iterate and tokenize the input string
    for (const char &current_character : input)
    {
        if (current_character == token)
        {
            // We found the token, add the current chunk to our results
            results.push_back(current_chunk);
            current_chunk.clear();
        }
        else
        {
            // We have not yet found the token, store the current character
            current_chunk += current_character;
        }
    }
    // Don't forget to flush the current chunk into the results
    results.push_back(current_chunk);

    return results;
}

Wire rasterize(const std::vector<std::string> &instructions)
{
    Wire coords{std::make_pair(0, 0)};

    for (const std::string &instruction : instructions)
    {
        const char direction = instruction.at(0);
        const int num_steps = atoi(instruction.substr(1).data());

        if (direction == 'U')
        {
            for (int i = 0; i < num_steps; i++)
            {
                Coord previous_coord = coords.back();
                previous_coord.second++;
                coords.push_back(previous_coord);
            }
        }
        else if (direction == 'D')
        {
            for (int i = 0; i < num_steps; i++)
            {
                Coord previous_coord = coords.back();
                previous_coord.second--;
                coords.push_back(previous_coord);
            }
        }
        else if (direction == 'L')
        {
            for (int i = 0; i < num_steps; i++)
            {
                Coord previous_coord = coords.back();
                previous_coord.first--;
                coords.push_back(previous_coord);
            }
        }
        else if (direction == 'R')
        {
            for (int i = 0; i < num_steps; i++)
            {
                Coord previous_coord = coords.back();
                previous_coord.first++;
                coords.push_back(previous_coord);
            }
        }
    };

    return coords;
}

int closest_intersection(std::vector<Coord> &intersections)
{
    intersections.erase(std::find(intersections.begin(), intersections.end(), Coord{0,0}));

    auto coord_compare = [](const Coord &a, const Coord &b) -> bool {
        return (abs(a.first) + abs(a.second)) < (abs(b.first) + abs(b.second));
    };
    Coord closest = *std::min_element(intersections.begin(), intersections.end(), coord_compare);

    return closest.first + closest.second;
}

int fewest_steps(std::vector<Coord> &intersections, const Wire &first, const Wire &second)
{
    intersections.erase(std::find(intersections.begin(), intersections.end(), Coord{0, 0}));

    int min{std::numeric_limits<int>::max()};
    for (const Coord &intersection : intersections)
    {
        int sum{0};
        Wire::const_iterator iter = std::find(first.begin(), first.end(), intersection);
        if (iter != first.end())
        {
            sum += std::distance(first.begin(), iter);
        }
        iter = std::find(second.begin(), second.end(), intersection);
        if (iter != second.end())
        {
            sum += std::distance(second.begin(), iter);
        }

        if (sum < min)
        {
            min = sum;
        }
    }

    return min;
}

int part1()
{
    // parse the string for each line, build up an array of line segments
    // for each line segment, find intersections, store
    // find intersection with min x, min y
    std::pair<std::string, std::string> inputs;
    Wire first;
    Wire second;
    std::vector<Coord> intersections;
    int distance;

    // R8,U5,L5,D3
    // U7,R6,D4,L4 = distance 6
    inputs = {"R8,U5,L5,D3",
              "U7,R6,D4,L4"};
    first = rasterize(tokenize(inputs.first));
    second = rasterize(tokenize(inputs.second));
    std::sort(first.begin(), first.end());
    std::sort(second.begin(), second.end());
    intersections.clear();
    std::set_intersection(first.begin(), first.end(),
                          second.begin(), second.end(),
                          std::back_inserter(intersections));
    distance = closest_intersection(intersections);
    assert(6 == distance);

    // R75,D30,R83,U83,L12,D49,R71,U7,L72
    // U62,R66,U55,R34,D71,R55,D58,R83 = distance 159
    inputs = {"R75,D30,R83,U83,L12,D49,R71,U7,L72",
              "U62,R66,U55,R34,D71,R55,D58,R83"};
    first = rasterize(tokenize(inputs.first));
    second = rasterize(tokenize(inputs.second));
    std::sort(first.begin(), first.end());
    std::sort(second.begin(), second.end());
    intersections.clear();
    std::set_intersection(first.begin(), first.end(),
                          second.begin(), second.end(),
                          std::back_inserter(intersections));
    distance = closest_intersection(intersections);
    assert(159 == distance);

    // R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51
    // U98,R91,D20,R16,D67,R40,U7,R15,U6,R7 = distance 135
    inputs = {"R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51",
              "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7"};
    first = rasterize(tokenize(inputs.first));
    second = rasterize(tokenize(inputs.second));
    std::sort(first.begin(), first.end());
    std::sort(second.begin(), second.end());
    intersections.clear();
    std::set_intersection(first.begin(), first.end(),
                          second.begin(), second.end(),
                          std::back_inserter(intersections));
    distance = closest_intersection(intersections);
    assert(135 == distance);

    // Now for the real deal
    inputs = kInput;
    first = rasterize(tokenize(inputs.first));
    second = rasterize(tokenize(inputs.second));
    std::sort(first.begin(), first.end());
    std::sort(second.begin(), second.end());
    intersections.clear();
    std::set_intersection(first.begin(), first.end(),
                          second.begin(), second.end(),
                          std::back_inserter(intersections));
    distance = closest_intersection(intersections);
    assert(896 == distance);
    return distance;
}

int main()
{
    std::cout << "Part1 distance: " << part1() << std::endl;
}