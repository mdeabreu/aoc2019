#include <iostream>
#include <array>
#include <vector>
#include <limits>
#include <cmath>
#include <cassert>
#include "day5.hpp"

enum ParameterMode
{
    Position,
    Immediate
};

enum OpCode
{
    Nop,
    Add,
    Mul,
    In,
    Out,
    Hcf = 99
};

class IntCode
{
public:
    // Default constructor with empty ram
    IntCode()
    {
        pc = ram.begin();
    };

    // Initialize the ram of the CPU with a set of ints
    IntCode(std::vector<int> input)
    {
        auto i = ram.begin();
        for (const auto &data : input)
        {
            *i = data;
            i++;
        }

        pc = ram.begin();
    };

    ~IntCode(){};

    // Infinite loop until cpu halts
    void run(){};

    // Decode the instruction at pc
    // Sets opcode, and access flags
    void decode()
    {
        // The last two digits of the number are the opcode
        int code = *pc % 100;
        opcode = static_cast<OpCode>(code);
    };

    void execute(){};

    void write(int addr, int data)
    {
        ram[addr] = data;
    };

    int read(int addr)
    {
        return ram[addr];
    };

    // Members
public:
    std::array<int, 1024>::iterator pc;
    OpCode opcode;
    std::vector<ParameterMode> modes;
    std::array<int, 1024> ram{0};
};

int main()
{
    std::vector<int> input {1,2,3,4,99};
    IntCode computer(input);

    computer.decode();
    assert(computer.opcode == Add);

    computer.pc++;
    computer.decode();
    assert(computer.opcode == Mul);

    computer.pc++;
    computer.decode();
    assert(computer.opcode == In);

    computer.pc++;
    computer.decode();
    assert(computer.opcode == Out);

    computer.pc++;
    computer.decode();
    assert(computer.opcode == Hcf);

    return 0;
}