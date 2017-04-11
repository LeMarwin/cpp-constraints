#include "iostream"
#include "constraints.cpp"
#include "configManager.cpp"

using namespace std;

int main(int argc, char const *argv[])
{
	//Считывает все входные данные с консоли
	//auto cnf = readConfigFromCommandLine("constraints.json");

	//Читает все входные данные из json-файла
	auto cnf = readConfigJson("default_config.json", "constraints.json");
	
	return 0;
}