#include "json.hpp"
#include "iostream"
#include <fstream>
#include <string>

using json = nlohmann::json;
using namespace std;

struct Config
{
  int m1;
  int m2;
  int l;
  int n;
  double b[6];
};