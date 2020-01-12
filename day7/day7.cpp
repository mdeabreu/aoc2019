#include "day7.hpp"
#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <cassert>
#include <numeric>
#include <algorithm>

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
        halt = true;
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
    bool halt{false};
    std::array<int, 1024>::iterator pc;
    OpCode opcode;
    std::stack<ParameterMode> modes;
    std::array<int, 1024> ram{0};
    std::queue<int> input;
    std::queue<int> output;
};

int run_amplifiers(const std::vector<int> &program, const std::vector<int> &phase)
{
    // Init amplifiers with program code
    IntCode ampa(program),
            ampb(program),
            ampc(program),
            ampd(program),
            ampe(program);
    
    // Init amplifiers with phase sequence
    ampa.input.push(phase.at(0));
    ampb.input.push(phase.at(1));
    ampc.input.push(phase.at(2));
    ampd.input.push(phase.at(3));
    ampe.input.push(phase.at(4));

    // Test if we enable the feedback loop
    bool enable_feedback = (*std::max_element(phase.begin(), phase.end()) > 4);

    int possible_output;

    // Run the sequence
    ampa.input.push(0);
    do
    {
        ampa.run();

        ampb.input.push(ampa.output.front());
        ampa.output.pop();
        ampb.run();

        ampc.input.push(ampb.output.front());
        ampb.output.pop();
        ampc.run();

        ampd.input.push(ampc.output.front());
        ampc.output.pop();
        ampd.run();

        ampe.input.push(ampd.output.front());
        ampd.output.pop();
        ampe.run();

        ampa.input.push(ampe.output.front());
        if (!ampe.output.empty()) possible_output = ampe.output.front();
        ampe.output.pop();
    } while (enable_feedback && !ampe.hcf);

    // Return the signal
    return possible_output;
};

void part1_test1()
{
    std::vector<int> input{3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0};

    int signal = run_amplifiers(input, {4,3,2,1,0});

    assert(signal == 43210);
}

void part1_test2()
{
    std::vector<int> input{3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0};

    int signal = run_amplifiers(input, {0,1,2,3,4});
    
    assert(signal == 54321);
}

void part1_test3()
{
    std::vector<int> input{3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0};

    int signal = run_amplifiers(input, {1,0,4,3,2});

    assert(signal == 65210);
}

int part1()
{
    std::vector<int> phase{0, 1, 2, 3, 4};
    int max_signal{std::numeric_limits<int>::min()};

    do
    {
        int signal = run_amplifiers(kInput, phase);
        if (signal > max_signal)
            max_signal = signal;
    } while (std::next_permutation(phase.begin(), phase.end()));

    assert(max_signal == 398674);

    return max_signal;
}

void part2_test1()
{
    std::vector<int> input{3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5};

    int signal = run_amplifiers(input, {9,8,7,6,5});

    assert(signal == 139629729);
}

void part2_test2()
{
    std::vector<int> input{3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,-5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10};

    int signal = run_amplifiers(input, {9,7,8,5,6});

    assert(signal == 18216);
}

int main()
{
    part1_test1();
    part1_test2();
    part1_test3();
    std::cout << "Part1: " << part1() << std::endl;

    part2_test1();
    part2_test2();

    return 0;
}