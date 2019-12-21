#include <iostream>
#include <cassert>
#include "day3.hpp"

typedef std::pair<int, int> Coord;

std::vector<Coord> parse(std::string input)
{
    std::string::size_type start = 0;
    std::string::size_type end = input.find(',', start);
    if (end == std::string::npos)
    {
        return {};
    }
    std::string instruction = input.substr(start, end);
    for(start; start < end; start ++)
    std::cout<<"substring: "<< instruction << std::endl;
    return std::vector<Coord> {std::make_pair(0,0)};
}

std::vector<Coord> find_intersections(std::vector<Coord> first, std::vector<Coord> second)
{
    return std::vector<Coord> {std::make_pair(0,0)};
}

int closest_intersection(std::vector<Coord> intersections)
{
    return 0;
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