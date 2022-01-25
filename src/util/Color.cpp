#include "Color.hpp"
#include "..\..\Color.hpp"


Color::Color(uint r, uint g, uint b, uint a): r(r), g(g), b(b), a(a)
{
}

Color::Color(uint r, uint g, uint b): r(r), g(g), b(b), a(1)
{
}

Color::Color(uint gray): r(gray), g(gray), b(gray), a(1)
{
}
