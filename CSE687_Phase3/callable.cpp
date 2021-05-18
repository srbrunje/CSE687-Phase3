#include "callable.h"

callable::callable(int A, int B, int answer)
{
	_A = A;
	_B = B;
	_Answer = answer;
}

bool callable::add()
{
	return ((_A + _B) == _Answer);
}

bool callable::operator()() { return add(); }
