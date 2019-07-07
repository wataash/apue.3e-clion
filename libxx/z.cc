#include "apue.h"
// conflict with c++ headers
#undef min
#undef max

#include <string>
#include <vector>

#include <cstdio>

#include <iostream>

static int _cppfunc();

static int
_cppfunc()
{
	std::cout << "bar" << std::endl;
	std::printf("baz\n");
	return 1;
}

extern "C" {
int
cppfunc()
{
	return _cppfunc();
}
}
