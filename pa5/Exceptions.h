#pragma once
#include <exception>

class InvalidOp : public std::exception
{
	const char* what() const noexcept override { return "Invalid OP code or parameters."; }
};


class FileLoadExcept : public std::exception
{
public:
    const char* what() const noexcept override { return "ZOM file load exception"; }
};

class RangeAttackExcept : public std::exception
{
public:
    const char* what() const noexcept override { return "Zombie's can't range attack"; }
};

class GotoExcept : public std::exception
{
public:
    const char* what() const noexcept override { return "Provided line number doesn't exist."; }
};