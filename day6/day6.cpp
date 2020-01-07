#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cassert>
#include "day6.hpp"

typedef std::map<std::string, std::string> Orbits;

std::vector<std::string> tokenize(const std::string &input, const char &token = '\n')
{
    std::vector<std::string> results;
    std::string chunk;
    for (const char &c : input)
    {
        if (c == token)
        {
            results.push_back(chunk);
            chunk.clear();
        }
        else
        {
            chunk += c;
        }
    }

    // Last step, add the remaining chunk to the list
    results.push_back(chunk);

    return results;
}

void process(Orbits &orbits, const std::string &input)
{
    size_t n = input.find(')');
    std::string parent_name = input.substr(0, n);
    std::string child_name = input.substr(n + 1);

    orbits[child_name] = parent_name;
    if (orbits.find(parent_name) == orbits.end())
    {
        orbits[parent_name] = "";
    }
}

int count_orbits(const Orbits &orbits)
{
    int num_orbits{0};
    for (auto orbit : orbits)
    {
        std::string parent = orbit.second;
        while (parent != "")
        {
            num_orbits++;
            parent = orbits.at(parent);
        }
    }

    return num_orbits;
}

void test_tokenize()
{
    std::string input{"AAA)BBB\nBBB)CCC\nCCC)DDD"};
    std::vector<std::string> results = tokenize(input, '\n');
    assert(results[0] == "AAA)BBB");
    assert(results[1] == "BBB)CCC");
    assert(results[2] == "CCC)DDD");
}

void test_process()
{
    std::string input{"AAA)BBB"};
    Orbits orbits;
    process(orbits, input);
}

void test_part1()
{
    std::string input = R"ORBITS(COM)B
B)C
C)D
D)E
E)F
B)G
G)H
D)I
E)J
J)K
K)L)ORBITS";
    auto orbit_strings = tokenize(input);
    Orbits orbits;

    for (auto orbit : orbit_strings)
    {
        process(orbits, orbit);
    }

    int num_orbits = count_orbits(orbits);
    assert(num_orbits == 42);
}

int part1()
{
    auto orbit_strings = tokenize(kInput);
    Orbits orbits;

    for (auto orbit : orbit_strings)
    {
        process(orbits, orbit);
    }

    int num_orbits = count_orbits(orbits);
    assert(num_orbits == 150150);
    return num_orbits;
}

int main()
{
    test_tokenize();
    test_process();
    test_part1();

    std::cout<< "Part 1: " << part1() << std::endl;
}