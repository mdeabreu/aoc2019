#include <iostream>
#include <algorithm>
#include <cassert>
#include "day3.hpp"

typedef std::pair<int, int> Coord;

std::vector<std::string> tokenize(const std::string &input, const char &token)
{
    std::vector<std::string> results;
    std::string current_chunk;

    auto string_to_chunks = [&results, &current_chunk, token](const char &current_character) {
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
    };

    // Iterate and tokenize the input string
    std::for_each(input.begin(), input.end(), string_to_chunks);
    // Don't forget to flush the current chunk into the results
    results.push_back(current_chunk);

    return results;
}

std::vector<Coord> parse(const std::string &input)
{
    std::vector<Coord> coords{std::make_pair(0, 0)};
    std::vector<std::string> instructions = tokenize(input, ',');

    auto instructions_to_coords = [&coords](const std::string &current_step) {
        const char direction = current_step.at(0);
        const int num_steps = atoi(current_step.substr(1).data());

        //std::cout<< current_step << ": " << direction << " " << num_steps << std::endl;
        if (direction == 'U')
        {
            for (int i = 0; i < num_steps; i++)
            {
                Coord previous_coord = coords.back();
                previous_coord.second++;
                //std::cout<< "(" << previous_coord.first << "," << previous_coord.second << ")" << std::endl;
                coords.push_back(previous_coord);
            }
        }
        else if (direction == 'D')
        {
                        for (int i = 0; i < num_steps; i++)
            {
                Coord previous_coord = coords.back();
                previous_coord.second--;
                //std::cout<< "(" << previous_coord.first << "," << previous_coord.second << ")" << std::endl;
                coords.push_back(previous_coord);
            }
        }
        else if (direction == 'L')
        {
                        for (int i = 0; i < num_steps; i++)
            {
                Coord previous_coord = coords.back();
                previous_coord.first--;
                //std::cout<< "(" << previous_coord.first << "," << previous_coord.second << ")" << std::endl;
                coords.push_back(previous_coord);
            }
        }
        else if (direction == 'R')
        {
                        for (int i = 0; i < num_steps; i++)
            {
                Coord previous_coord = coords.back();
                previous_coord.first++;
                //std::cout<< "(" << previous_coord.first << "," << previous_coord.second << ")" << std::endl;
                coords.push_back(previous_coord);
            }
        }
    };

    std::for_each(instructions.begin(), instructions.end(), instructions_to_coords);

    return coords;
}

std::vector<Coord> find_intersections(std::vector<Coord> first, std::vector<Coord> second)
{
    std::vector<Coord> intersections;
    auto get_intersections = [&intersections, second](const Coord& coord){
        auto res = std::find(second.begin(), second.end(), coord);
        if (res != second.end() && res != second.begin())
        {
            intersections.push_back(*res);
}
    };

    std::for_each(first.begin(), first.end(), get_intersections);

    return intersections;
}

int closest_intersection(std::vector<Coord> intersections)
{
    auto coord_compare = [](const Coord& a, const Coord& b) -> bool {
        return (abs(a.first) + abs(a.second)) < (abs(b.first) + abs(b.second));
    };
    Coord closest = *std::min_element(intersections.begin(), intersections.end(), coord_compare);

    return closest.first + closest.second;
}

int part1()
{
    // parse the string for each line, build up an array of line segments
    // for each line segment, find intersections, store
    // find intersection with min x, min y
    std::string first_wire_string;
    std::string second_wire_string;
    std::vector<Coord> first_wire;
    std::vector<Coord> second_wire;
    std::vector<Coord> intersections;
    int distance;

    // R8,U5,L5,D3
    // U7,R6,D4,L4 = distance 6
    first_wire_string = "R8,U5,L5,D3";
    second_wire_string = "U7,R6,D4,L4";
    first_wire = parse(first_wire_string);
    second_wire = parse(second_wire_string);
    intersections = find_intersections(first_wire, second_wire);
    distance = closest_intersection(intersections);
    assert(6 == distance);

    // R75,D30,R83,U83,L12,D49,R71,U7,L72
    // U62,R66,U55,R34,D71,R55,D58,R83 = distance 159
    first_wire_string = "R75,D30,R83,U83,L12,D49,R71,U7,L72";
    second_wire_string = "U62,R66,U55,R34,D71,R55,D58,R83";
    first_wire = parse(first_wire_string);
    second_wire = parse(second_wire_string);
    intersections = find_intersections(first_wire, second_wire);
    distance = closest_intersection(intersections);
    assert(159 == distance);

    // R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51
    // U98,R91,D20,R16,D67,R40,U7,R15,U6,R7 = distance 135
    first_wire_string = "R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51";
    second_wire_string = "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7";
    first_wire = parse(first_wire_string);
    second_wire = parse(second_wire_string);
    intersections = find_intersections(first_wire, second_wire);
    distance = closest_intersection(intersections);
    assert(135 == distance);

    // Now for the real deal
    first_wire_string = kInput.first;
    second_wire_string = kInput.second;
    first_wire = parse(first_wire_string);
    second_wire = parse(second_wire_string);
    intersections = find_intersections(first_wire, second_wire);
    distance = closest_intersection(intersections);
    return distance;
}

int main()
{
    //std::cout<< kInput.first << std::endl;
    //std::cout<< kInput.second << std::endl;
    part1();
}