#include "day7.hpp"
#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <cassert>

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
        // Write input to ram
        write(*pc++, input.front());
        input.pop();
    };

    void Out()
    {
        // Load data to output
        int param1 = load(get_mode());

        // Output
        output.push(param1);
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
    std::queue<int> input;
    std::queue<int> output;
};

void part1_test1()
{
    std::vector<int> input{3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0};

    // Init amplifiers with program code
    IntCode ampa(input),
            ampb(input),
            ampc(input),
            ampd(input),
            ampe(input);
    
    // Init input with phase sequence 43210
    ampa.input.push(4);
    ampb.input.push(3);
    ampc.input.push(2);
    ampd.input.push(1);
    ampe.input.push(0);

    // Run the sequence
    ampa.input.push(0);
    ampa.run();

    ampb.input.push(ampa.output.front());
    ampb.run();

    ampc.input.push(ampb.output.front());
    ampc.run();

    ampd.input.push(ampc.output.front());
    ampd.run();

    ampe.input.push(ampd.output.front());
    ampe.run();

    assert(ampe.output.front() == 43210);
}

void part1_test2()
{
    std::vector<int> input{3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0};

    // Init amplifiers with program code
    IntCode ampa(input),
            ampb(input),
            ampc(input),
            ampd(input),
            ampe(input);
    
    // Init input with phase sequence 01234
    ampa.input.push(0);
    ampb.input.push(1);
    ampc.input.push(2);
    ampd.input.push(3);
    ampe.input.push(4);

    // Run the sequence
    ampa.input.push(0);
    ampa.run();

    ampb.input.push(ampa.output.front());
    ampb.run();

    ampc.input.push(ampb.output.front());
    ampc.run();

    ampd.input.push(ampc.output.front());
    ampd.run();

    ampe.input.push(ampd.output.front());
    ampe.run();

    assert(ampe.output.front() == 54321);
}

void part1_test3()
{
    std::vector<int> input{3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0};

    // Init amplifiers with program code
    IntCode ampa(input),
            ampb(input),
            ampc(input),
            ampd(input),
            ampe(input);
    
    // Init input with phase sequence 10432
    ampa.input.push(1);
    ampb.input.push(0);
    ampc.input.push(4);
    ampd.input.push(3);
    ampe.input.push(2);

    // Run the sequence
    ampa.input.push(0);
    ampa.run();

    ampb.input.push(ampa.output.front());
    ampb.run();

    ampc.input.push(ampb.output.front());
    ampc.run();

    ampd.input.push(ampc.output.front());
    ampd.run();

    ampe.input.push(ampd.output.front());
    ampe.run();

    assert(ampe.output.front() == 65210);
}

int main()
{
    part1_test1();
    part1_test2();
    part1_test3();
    return 0;
}