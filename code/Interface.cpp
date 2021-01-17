#include "Interface.h"

using namespace std;

void help_with_interface (
    const string &begin_string,
    const string &repeat_string,
    int &operation,
    int min_number,
    int number_of_operations
) noexcept
{
    string local_string;
    int correctness;

    cout << begin_string;
    getline (cin, local_string);
    correctness = is_correct (min_number, number_of_operations, operation, local_string);
    while (!correctness) 
    {
        cout << repeat_string;
        getline (cin, local_string);
        correctness = is_correct (min_number, number_of_operations, operation, local_string);
    }
}

bool is_correct (
    int min_number,
    int number_of_operations,
    int &operation,
    const string &local_string
) noexcept
{
    istringstream istream (local_string);
    try 
    {
        istream >> operation;
    }
    catch (std::ios_base::failure &) // 
    {
        return 0;
    }
    string string_for_operation = to_string (operation);
    return !(istream.str().length() != string_for_operation.length() ||
             operation < min_number ||
             operation > min_number + number_of_operations - 1);
}