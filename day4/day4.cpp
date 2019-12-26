#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <cmath>
#include "day4.hpp"

std::vector<std::string> tokenize(const std::string &input, const char &token = ',')
{
    std::vector<std::string> results;
    std::string current_chunk;

    // Iterate and tokenize the input string
    for (const char &current_character : input)
    {
        if (current_character == token)
        {
            // We found the token, add the current chunk to our results
            results.push_back(current_chunk);
            current_chunk.clear();
        }
        else
        {
            // We have not yet found the token, store the current character
            current_chunk += current_character;
        }
    }
    // Don't forget to flush the current chunk into the results
    results.push_back(current_chunk);

    return results;
}

bool rule_six_digit(const int &password)
{
    bool res = (password >= 100000) && (password < 1000000);
    return res;
}

bool rule_adjacent(const int &password)
{
    std::string password_string = std::to_string(password);

    char prev_number{};
    for (const char& number : password_string)
    {
        if (prev_number == number)
        {
            return true;
        }
        else
        {
            prev_number = number;
        }
    }

    return false;
}

bool rule_never_decrease(const int &password)
{
    int n1 = fmod(password / 100000, 10);
    int n2 = fmod(password / 10000, 10);
    int n3 = fmod(password / 1000, 10);
    int n4 = fmod(password / 100, 10);
    int n5 = fmod(password / 10, 10);
    int n6 = fmod(password / 1, 10);

    bool res =  (n1 <= n2) &&
           (n2 <= n3) &&
           (n3 <= n4) &&
           (n4 <= n5) &&
           (n5 <= n6);

           return res;
}

bool validate(const int &password)
{
    return (rule_six_digit(password) &&
            rule_adjacent(password) &&
            rule_never_decrease(password));
}

int part1()
{
    bool valid;

    valid = validate(111111);
    assert(true == valid);

    valid = validate(223450);
    assert(false == valid);

    int num_valid{0};
    std::vector<std::string> input = tokenize(kInput, '-');
    int i = atoi((*input.begin()).data());
    for (int i = atoi((*input.begin()).data()); i <= atoi((*(input.end()-1)).data()); i++)
    {
        if (validate(i))
        {
            num_valid++;
        }
    }

    assert(2090 == num_valid);

    return num_valid;
}

int part2()
{
    return 2;
}

int main()
{
    std::cout << "Part 1: " << part1() << std::endl;
    std::cout << "Part 2: " << part2() << std::endl;
}