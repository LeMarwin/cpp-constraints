#include "iostream"
#include "climits"
#include "vector"
#include <math.h>
#include "constraints.cpp"

using namespace std;

int main(int argc, char const *argv[])
{
	auto m = readConstraints("constraints.json");

	int i = 1;

	cout << m.at("M1")->check(i) << endl;
	return 0;
}