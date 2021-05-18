#pragma once
class callable
{
private:
	int _A;
	int _B;
	int _Answer;

public:

	callable(int A, int B, int answer);

	bool add();

	bool operator()();

};
