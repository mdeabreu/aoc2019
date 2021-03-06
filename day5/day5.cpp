#include <iostream>
#include <array>
#include <vector>
#include <stack>
#include <limits>
#include <cmath>
#include <cassert>
#include <string>
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
    Jit,
    Jif,
    Lt,
    Eq,
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
    void run()
    {
        while (!hcf)
        {
            decode();
            execute();
        }
    };

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
            case OpCode::In:  In();  break;
            case OpCode::Out: Out(); break;
            case OpCode::Jit: Jit(); break;
            case OpCode::Jif: Jif(); break;
            case OpCode::Lt:  Lt();  break;
            case OpCode::Eq:  Eq();  break;
            case OpCode::Hcf: Hcf(); break;
        }

        // Clear opcode and parameter modes
        opcode = Nop;
        while (!modes.empty())
        {
            modes.pop();
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
        int param1 = load(get_mode());

        // Get parameter 2
        int param2 = load(get_mode());

        // Write the result and increment PC
        write(*pc++, param1 + param2);
    };

    void Mul()
    {
        // Get parameter 1
        int param1 = load(get_mode());

        // Get parameter 2
        int param2 = load(get_mode());

        // Write the result
        write(*pc++, param1 * param2);
    };

    void In()
    {
        // Get input
        int data{atoi(input.c_str())};

        // Write input to ram
        write(*pc++, data);
    };

    void Out()
    {
        // Load data to output
        int param1 = load(get_mode());

        // Output
        output+= std::to_string(param1);
    };

    void Hcf()
    {
        hcf = true;
        pc++;
    };

    void Jit()
    {
        int param1 = load(get_mode());
        int param2 = load(get_mode());
        
        if (param1)
        {
            pc = (ram.begin() + param2);
        }
    };

    void Jif()
    {
        int param1 = load(get_mode());
        int param2 = load(get_mode());
        
        if (!param1)
        {
            pc = (ram.begin() + param2);
        }
    };

    void Lt()
    {
        int param1 = load(get_mode());
        int param2 = load(get_mode());

        if (param1 < param2)
        {
            write(*pc++, 1);
        }
        else
        {
            write(*pc++, 0);
        }
    };

    void Eq()
    {
        int param1 = load(get_mode());
        int param2 = load(get_mode());

        if (param1 == param2)
        {
            write(*pc++, 1);
        }
        else
        {
            write(*pc++, 0);
        }
    };

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
    bool hcf{false}; // Flag to Halt Catch Fire
    std::array<int, 1024>::iterator pc;
    OpCode opcode;
    std::stack<ParameterMode> modes;
    std::array<int, 1024> ram{0};
    std::string input;
    std::string output;
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

void test_in()
{
    std::vector<int> input {3, 2, 0};
    IntCode computer(input);

    computer.input = "99";
    computer.decode();
    computer.execute();
    assert(computer.ram[2] == 99);
}

void test_out()
{
    std::vector<int> input {104, 123, 4, 4, 321};
    IntCode computer(input);

    computer.decode();
    computer.execute();
    assert(computer.output == "123");
    computer.output.erase();

    computer.decode();
    computer.execute();
    assert(computer.output == "321");
    computer.output.erase();
}

void test_jit()
{
    std::vector<int> input{1105,1,5,0,0,99};
    IntCode computer(input);
    computer.decode();
    computer.execute();
    assert(*computer.pc == 99);
}

void test_jif()
{
    std::vector<int> input{1106,0,5,0,0,99};
    IntCode computer(input);
    computer.decode();
    computer.execute();
    assert(*computer.pc == 99);
}

void test_lt()
{
    std::vector<int> input{1107, 50, 70, 8, 1107, 70, 50, 9, 99, 99};
    IntCode computer(input);
    computer.decode();
    computer.execute();
    assert(computer.ram[8] == 1);

    computer.decode();
    computer.execute();
    assert(computer.ram[9] == 0);
}

void test_eq()
{
    std::vector<int> input{1108, 50, 50, 8, 1108, 50, 70, 9, 99, 99};
    IntCode computer(input);
    computer.decode();
    computer.execute();
    assert(computer.ram[8] == 1);

    computer.decode();
    computer.execute();
    assert(computer.ram[9] == 0);
}

void test_hcf()
{
    std::vector<int> input {1101, 1, 2, 5, 99, 0};
    IntCode computer(input);
    computer.run();
}

void part1()
{
    IntCode computer(kInput);
    computer.input = "1";
    computer.run();
    assert(computer.output == "00000000011933517");
}

void part2()
{
    IntCode computer(kInput);
    computer.input = "5";
    computer.run();
    assert(computer.output == "10428568");
}

int main()
{
    test_all_opcodes();
    test_parameter_modes();
    test_add();
    test_mul();
    test_in();
    test_out();
    test_jit();
    test_jif();
    test_lt();
    test_eq();
    test_hcf();
    
    part1();
    part2();

    return 0;
}