#include "day9.hpp"
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <array>

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

int main()
{
    std::cout << "Hello World" << std::endl;
}