#include <iostream>
#include <cassert>
#include "day2.hpp"

enum
{
    ADD = 1,
    MUL = 2,
    HCF = 99
} opcodes;

void intcode(std::vector<int> &memory)
{
    // Program counter
    auto pc{memory.begin()};

    // Decode opcode
    while (true)
    {
        switch (*pc)
        {
        case ADD:
        {
            // Load first operand
            int d1{*(pc+1)}; d1 = memory.at(d1);

            // Load second operand
            int d2{*(pc+2)}; d2 = memory.at(d2);

            // Get result address
            int rx{*(pc+3)};

            // Write result
            memory.at(rx) = (d1 + d2);

            // Advance program counter
            pc = pc+4;
            break;
        }
        case MUL:
        {
            // Load first operand
            int d1{*(pc+1)}; d1 = memory.at(d1);

            // Load second operand
            int d2{*(pc+2)}; d2 = memory.at(d2);

            // Get result address
            int rx{*(pc+3)};

            // Write result
            memory.at(rx) = (d1 * d2);

            // Advance program counter
            pc = pc+4;
            break;
        }
        case HCF:
        default:
        {
            // Halt Catch Fire
            // unknown opcode
            return;
        }
        }
    }
}

int part1()
{
    std::vector<int> input;
    std::vector<int> validation;

    // 1,0,0,0,99 becomes 2,0,0,0,99 (1 + 1 = 2).
    input = {1,0,0,0,99};
    validation = {2,0,0,0,99};
    intcode(input);
    assert(validation == input);

    // 2,3,0,3,99 becomes 2,3,0,6,99 (3 * 2 = 6).
    input = {2,3,0,3,99};
    validation = {2,3,0,6,99};
    intcode(input);
    assert(validation == input);

    // 2,4,4,5,99,0 becomes 2,4,4,5,99,9801 (99 * 99 = 9801).
    input = {2,4,4,5,99,0};
    validation = {2,4,4,5,99,9801};
    intcode(input);
    assert(validation == input);

    // 1,1,1,4,99,5,6,0,99 becomes 30,1,1,4,2,5,6,0,99.
    input = {1,1,1,4,99,5,6,0,99};
    validation = {30,1,1,4,2,5,6,0,99};
    intcode(input);
    assert(validation == input);

    input = kInput;
    input.at(1) = 12;
    input.at(2) = 2;
    intcode(input);
    return input.at(0);
}

std::pair<int, int> part2()
{
    int validation = 19690720;
    std::vector<int> input;
    for (int n = 0; n < 100; n++)
    {
        for (int v = 0; v < 100; v++)
        {
            input = kInput;
            input.at(1) = n;
            input.at(2) = v;
            intcode(input);
            if (validation == input.at(0))
            {
                return std::pair<int, int>(n, v);
            }
        }
    }

    return std::pair<int, int>(0, 0);
}

int main()
{
    int p1_output = part1();
    assert(7594646 == p1_output);
    std::cout<< "Part 1: Value at position 0: " << p1_output << std::endl;

    std::pair<int, int> p2_output = part2();
    assert(33 == p2_output.first);
    assert(76 == p2_output.second);
    std::cout<< "Part 2: NounVerb: " << p2_output.first << p2_output.second << std::endl;
}