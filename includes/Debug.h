#pragma once

#include <string>
#include <iostream>

#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

class Debug
{
public:
	static void Print(std::string text);
};