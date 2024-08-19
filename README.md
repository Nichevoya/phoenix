# Phoenix

## High level C++ library

Phoenix is a custom library that implement high level methods seen in other programming languages such as Python, or Flutter, while keeping low level accessibility to them. It also serves as a wrapper of my experience and my knowledge of C++, its goals is to be as reusable as possible.

## Example

```C++
#include "phoenix.hpp"

int main(void)
{
    // phoenix string takes unknown number of parameters, whatever their types and returns a string representation of the input
    phx::string welcome("Welcome to ", phx::log("Phoenix", phx::colors::green), " Library, Version: ", 1, '.', 0);

    // numbers
    phx::number nint(5);    // int32_t by default
    phx::number<double> ndouble(3.14);
    
    // random numbers
    phx::random<int32_t> rint(-10, 10); // ranged random int32_t
    phx::random<double> rdouble;

    rdouble.set_precision(3); // possibility to set custom precision

    std::cout << welcome.endl() << std::endl
        << "\tint32 number : " << nint.string() << std::endl // string representation of nint
        << "\tdouble number : " << ndouble << std::endl // precision set and fixed by default
        << "\trandom int32 : " << rint << std::endl
        << "\trandom double : " << rdouble << std::endl << std::endl;
}
```
