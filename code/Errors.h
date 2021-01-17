#pragma once

#include <exception>
#include <cstring>
#include <string>

class GeneralException : public std::exception
{
protected:
    std::string exception_string;
public:
    GeneralException () noexcept = default;
    explicit GeneralException (std::string exception_string) noexcept 
    { 
        this->exception_string = exception_string; 
    }
    std::string message() const noexcept { return exception_string; }
    const char *what() const override { return exception_string.c_str (); }
};

class BurstingBudget : public GeneralException
{
public:
    BurstingBudget () noexcept : 
        GeneralException ("Your budget is not so big as to afford this operation.\n") { ; }
    explicit BurstingBudget (std::string operation) noexcept;
};

class UnavailableOperation : public GeneralException
{
public:
    UnavailableOperation () noexcept :
        GeneralException ("This operation is not available.\n") { ; }
    explicit UnavailableOperation (std::string operation) noexcept;
};

class MaxValue : public GeneralException
{
public:
    MaxValue () noexcept :
        GeneralException ("You already have the maximum value of this attribute.\n") { ; }
    explicit MaxValue (std::string attribute) noexcept;
};

class InvalidArgument : public GeneralException
{
public:
    InvalidArgument () noexcept :
        GeneralException ("There is an invalid parameter.\n") { ; }
    explicit InvalidArgument (std::string parameter, std::string function = "main ()") noexcept;
};

class BurstingDivine : public GeneralException
{
public:
    BurstingDivine() noexcept :
        GeneralException("Your budget is not so big as to afford this operation.\n") { ; }
    explicit BurstingDivine (std::string operation) noexcept;
};