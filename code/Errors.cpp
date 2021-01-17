#include "Errors.h"

BurstingBudget::BurstingBudget (
    std::string operation
) noexcept
{
    std::string result_string = "Your budget is not so big as to afford the operation \"" +
        operation + "\".\n";
    this->exception_string = result_string;
}

UnavailableOperation::UnavailableOperation (
    std::string operation
) noexcept
{
    std::string result_string = "The operation \"" +
        operation + "\" is not available.\n";
    this->exception_string = result_string;
}

MaxValue::MaxValue (
    std::string attribute
) noexcept
{
    std::string result_string = "You already have the maximum value of the attribute \"" +
        attribute + "\".\n";
    this->exception_string = result_string;
}

InvalidArgument::InvalidArgument (
    std::string parameter,
    std::string function
) noexcept
{
    std::string result_string = "There is an invalid parameter \"" +
        parameter + "\" in the function \"" + function + "\".\n";
    this->exception_string = result_string;
}

BurstingDivine::BurstingDivine (
    std::string operation
) noexcept
{
    std::string result_string = "Your divine power is not so big as to afford the operation \"" +
        operation + "\".\n";
    this->exception_string = result_string;
}