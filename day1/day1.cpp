#include <iostream>
#include <algorithm>
#include <numeric>
#include <cassert>
#include "day1.hpp"

// Fuel required to launch a given module is based on its mass.
// Specifically, to find the fuel required for a module, take its mass, divide
// by three, round down, and subtract 2.

// Fuel itself requires fuel just like a module - take its mass, divide by
// three, round down, and subtract 2. However, that fuel also requires fuel,
// and that fuel requires fuel, and so on. Any mass that would require negative
// fuel should instead be treated as if it requires zero fuel; the remaining
// mass, if any, is instead handled by wishing really hard, which has no mass
// and is outside the scope of this calculation.
int mass_to_fuel(const int& mass, const bool& include_fuel)
{
    if (mass < 6)
    {
        return 0;
    }

    int fuel = ((mass / 3) - 2);

    if (include_fuel)
    {
        return fuel + mass_to_fuel(fuel, true);
    }
    else
    {
        return fuel;
    }
}

int part1(std::vector<int> input)
{
    const bool include_fuel = false;
    // For a mass of 12, divide by 3 and round down to get 4,
    // then subtract 2 to get 2.
    assert(2 == mass_to_fuel(12, include_fuel));
    // For a mass of 14, dividing by 3 and rounding down still yields 4, so the
    // fuel required is also 2.
    assert(2 == mass_to_fuel(14, include_fuel));
    // For a mass of 1969, the fuel required is 654.
    assert(654 == mass_to_fuel(1969, include_fuel));
    // For a mass of 100756, the fuel required is 33583.
    assert(33583 == mass_to_fuel(100756, include_fuel));

    // Transform the original mass into the fuel required
    std::vector<int> fuel_counts;
    std::transform(input.begin(), input.end(), std::back_inserter(fuel_counts),
                   [include_fuel](const int &n) { return mass_to_fuel(n, include_fuel); });

    // Sum all the fuel counts for a total
    return std::accumulate(fuel_counts.begin(), fuel_counts.end(), 0);
}

int part2(std::vector<int> input)
{
    const bool include_fuel = true;
    // A module of mass 14 requires 2 fuel. This fuel requires no further fuel
    // (2 divided by 3 and rounded down is 0, which would call for a negative
    // fuel), so the total fuel required is still just 2.
    assert(2 == mass_to_fuel(14, include_fuel));
    // At first, a module of mass 1969 requires 654 fuel. Then, this fuel
    // requires 216 more fuel (654 / 3 - 2). 216 then requires 70 more fuel,
    // which requires 21 fuel, which requires 5 fuel, which requires no further
    // fuel. So, the total fuel required for a module of mass 1969 is
    // 654 + 216 + 70 + 21 + 5 = 966.
    assert(966 == mass_to_fuel(1969, include_fuel));
    // The fuel required by a module of mass 100756 and its fuel is:
    // 33583 + 11192 + 3728 + 1240 + 411 + 135 + 43 + 12 + 2 = 50346.
    assert(50346 == mass_to_fuel(100756, include_fuel));

    // Transform the original mass into the fuel required
    std::vector<int> fuel_counts;
    std::transform(input.begin(), input.end(), std::back_inserter(fuel_counts),
                   [include_fuel](const int &n) { return mass_to_fuel(n, include_fuel); });

    // Sum all the fuel counts for a total
    return std::accumulate(fuel_counts.begin(), fuel_counts.end(), 0);
}

int main()
{
    int p1_total_fuel = part1(kInputs);
    assert(3406432 == p1_total_fuel);
    std::cout<< "Part 1: Total fuel required: " << p1_total_fuel << std::endl;

    int p2_total_fuel = part2(kInputs);
    assert(5106777 == p2_total_fuel);
    std::cout<< "Part 2: Total fuel required: " << p2_total_fuel << std::endl;
}