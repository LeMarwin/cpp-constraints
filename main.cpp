#include "iostream"
#include "climits"
#include "vector"
#include <math.h>
#include "constraints.cpp"

using namespace std;

int main(int argc, char const *argv[])
{
	auto m = readConstraints("constraints.json");
	int i = readWhileConstraint<int>("M1",&m);
	cout << "You have entered: " << i << endl;
	return 0;
}