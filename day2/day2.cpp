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

void part1()
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
}

int main()
{
    part1();
}