#include <iostream>
#include <array>
#include <vector>
#include <stack>
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

        // Everything else are the parameter modes
        std::string mode = std::to_string(*pc / 100);
        for (const char& c : mode)
        {
            if (c == '0') modes.push(ParameterMode::Position);
            else if (c == '1') modes.push(ParameterMode::Immediate);
        }

        // We've decoded the opcode and parameter modes, increment PC
        pc++;
    };

    void execute()
    {
        switch(opcode)
        {
            case OpCode::Add: Add(); break;
            case OpCode::Mul: Mul(); break;
            case OpCode::In:  In(); break;
            case OpCode::Out: Out(); break;
            case OpCode::Hcf: Hcf(); break;
        }
    };

    void write(int addr, int data)
    {
        ram[addr] = data;
    };

    int read(int addr)
    {
        return ram[addr];
    };

    int load(ParameterMode mode)
    {
        if (mode == ParameterMode::Immediate)
        {
            return *pc++;
        }
        else // if (mode == ParameterMode::Position)
        {
            return read(*pc++);
        }
    }

    void Add()
    {
        // Get parameter 1
        ParameterMode mode = get_mode();
        int param1 = load(mode);

        // Get parameter 2
        mode = get_mode();
        int param2 = load(mode);

        // Write the result and increment PC
        write(*pc++, param1 + param2);
    };

    void Mul()
    {
        // Get parameter 1
        ParameterMode mode = get_mode();
        int param1 = load(mode);

        // Get parameter 2
        mode = get_mode();
        int param2 = load(mode);

        // Write the result
        write(*pc++, param1 * param2);
    };
    void In(){};
    void Out(){};
    void Hcf(){};

    ParameterMode get_mode()
    {
        ParameterMode result = ParameterMode::Position;
        if (modes.size())
        {
            result = modes.top();
            modes.pop();
        }

        return result;
    };

    // Members
public:
    std::array<int, 1024>::iterator pc;
    OpCode opcode;
    std::stack<ParameterMode> modes;
    std::array<int, 1024> ram{0};
};

void test_all_opcodes()
{
    std::vector<int> input {1,2,3,4,99};
    IntCode computer(input);

    computer.decode();
    assert(computer.opcode == Add);

    computer.decode();
    assert(computer.opcode == Mul);

    computer.decode();
    assert(computer.opcode == In);

    computer.decode();
    assert(computer.opcode == Out);

    computer.decode();
    assert(computer.opcode == Hcf);
}

void test_parameter_modes()
{
    std::vector<int> input {1002};
    IntCode computer(input);

    computer.decode();
    std::stack<ParameterMode> expected(std::deque<ParameterMode>{Immediate, Position});
    assert(computer.modes == expected);
}

void test_add()
{
    std::vector<int> input {1001, 4, 3, 4, 33};
    IntCode computer(input);

    computer.decode();
    computer.execute();
    assert(computer.ram[4] == 36);
}

void test_mul()
{
    std::vector<int> input {1002, 4, 3, 4, 33};
    IntCode computer(input);

    computer.decode();
    computer.execute();
    assert(computer.ram[4] == 99);
}

int main()
{
    test_all_opcodes();
    test_parameter_modes();
    test_add();
    test_mul();
    return 0;
}