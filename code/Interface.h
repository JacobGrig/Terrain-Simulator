#pragma once

#include <iostream>
#include <string>
#include <sstream>

void help_with_interface (
    const std::string &begin_string,
    const std::string &repeat_string,
    int &operation,
    int min_number,
    int number_of_operations
) noexcept;

bool is_correct (
    int min_number,
    int number_of_operations,
    int &operation,
    const std::string &local_string
) noexcept;