#include "day9.hpp"
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <array>
#include <cassert>

enum ParameterMode
{
    Position,
    Immediate,
    Relative
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
    Rbo,
    Hcf = 99
};

class IntCode
{
public:
    // Default constructor with empty ram
    IntCode()
    {
        ram.reserve(2048);
        std::fill_n(std::back_inserter(ram), 2048, 0);
        pc = ram.begin();
    };

    // Initialize the ram of the CPU with a set of ints
    IntCode(std::vector<long> input)
    {
        ram.reserve(2048);
        std::fill_n(std::back_inserter(ram), 2048, 0);

        auto i = ram.begin();
        for (long data : input)
        {
            *i = data;
            i++;
        }

        pc = ram.begin();
    };

    //~IntCode(){};

    // Infinite loop until cpu halts
    void run()
    {
        while (!hcf && !halt)
        {
            decode();
            execute();
        }

        halt = false;
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
            else if (c == '2') modes.push(ParameterMode::Relative);
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
            case OpCode::Rbo: Rbo(); break;
            case OpCode::Hcf: Hcf(); break;
            
            case OpCode::Nop: break;
            default: break;
        }

        // Clear opcode and parameter modes
        opcode = Nop;
        while (!modes.empty())
        {
            modes.pop();
        }
    };

    void write(int addr, long data)
    {
        ram.at(addr) = data;
    };

    void write(ParameterMode mode, long data)
    {
        if (mode == ParameterMode::Relative)
        {
            write(relative_base + *pc++, data);
        }
        else
        {
            write(*pc++, data);
        }
    };

    long read(int addr)
    {
        return ram.at(addr);
    };

    long load(ParameterMode mode)
    {
        if (mode == ParameterMode::Immediate)
        {
            return *pc++;
        }
        else if (mode == ParameterMode::Relative)
        {
            return read(relative_base + *pc++);
        }
        else // if (mode == ParameterMode::Position)
        {
            return read(*pc++);
        }
    }

    void Add()
    {
        // Get parameter 1
        long param1 = load(get_mode());

        // Get parameter 2
        long param2 = load(get_mode());

        // Write the result and increment PC
        write(get_mode(), param1 + param2);
    };

    void Mul()
    {
        // Get parameter 1
        long param1 = load(get_mode());

        // Get parameter 2
        long param2 = load(get_mode());

        // Write the result
        write(get_mode(), param1 * param2);
    };

    void In()
    {
        // Write input to ram
        long data = input.front();
        input.pop_front();

        write(get_mode(), data);
    };

    void Out()
    {
        // Load data to output
        long param1 = load(get_mode());

        // Output
        output.push(param1);
        halt = true;
    };

    void Hcf()
    {
        hcf = true;
        pc++;
    };

    void Jit()
    {
        long param1 = load(get_mode());
        long param2 = load(get_mode());
        
        if (param1)
        {
            pc = (ram.begin() + param2);
        }
    };

    void Jif()
    {
        long param1 = load(get_mode());
        long param2 = load(get_mode());
        
        if (!param1)
        {
            pc = (ram.begin() + param2);
        }
    };

    void Lt()
    {
        long param1 = load(get_mode());
        long param2 = load(get_mode());

        if (param1 < param2)
        {
            write(get_mode(), 1);
        }
        else
        {
            write(get_mode(), 0);
        }
    };

    void Eq()
    {
        long param1 = load(get_mode());
        long param2 = load(get_mode());

        if (param1 == param2)
        {
            write(get_mode(), 1);
        }
        else
        {
            write(get_mode(), 0);
        }
    };

    void Rbo()
    {
        relative_base += load(get_mode());
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
    bool halt{false};
    long relative_base{0};
    std::vector<long>::iterator pc;
    OpCode opcode;
    std::stack<ParameterMode> modes;
    std::vector<long> ram;
    std::deque<long> input;
    std::queue<long> output;
};

void part1_test1()
{
    std::vector<long> input{109, 1, 204, -1, 1001, 100, 1, 100, 1008, 100, 16, 101, 1006, 101, 0, 99};
    std::vector<long> results;
    IntCode computer(input);
    while (!computer.hcf)
    {
        computer.run();
        if (!computer.output.empty())
        {
            results.push_back(computer.output.front());
            computer.output.pop();
        }
    }

    assert(results == input);
}

void part1_test2()
{
    std::vector<long> input{1102, 34915192, 34915192, 7, 4, 7, 99, 0};
    long result{0};
    IntCode computer(input);
    while (!computer.hcf)
    {
        computer.run();
        if (!computer.output.empty())
        {
            result = computer.output.front();
            computer.output.pop();
        }
    }

    assert(1219070632396864 == result);
}

void part1_test3()
{
    std::vector<long> input{104, 1125899906842624, 99};
    long result{0};
    IntCode computer(input);
    while (!computer.hcf)
    {
        computer.run();
        if (!computer.output.empty())
        {
            result = computer.output.front();
            computer.output.pop();
        }
    }

    assert(1125899906842624 == result);
}

long part1()
{
    IntCode computer(kInput);
    long result{0};
    computer.input.push_back(1);

    while(!computer.hcf)
    {
        computer.run();
        if (!computer.output.empty())
        {
            result = computer.output.front();
            computer.output.pop();
        }
    }

    assert(3638931938 == result);

    return result;
}

long part2()
{
    IntCode computer(kInput);
    long result{0};
    computer.input.push_back(2);

    while(!computer.hcf)
    {
        computer.run();
        if (!computer.output.empty())
        {
            result = computer.output.front();
            computer.output.pop();
        }
    }

    assert(86025 == result);

    return result;
}

int main()
{
    part1_test1();
    part1_test2();
    part1_test3();

    std::cout << "Part 1: " << part1() << std::endl;
    std::cout << "Part 2: " << part2() << std::endl;
}