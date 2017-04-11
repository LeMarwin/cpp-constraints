#include "json.hpp"
#include "iostream"
#include "constraints.cpp"
#include <fstream>
#include <string>

using json = nlohmann::json;
using namespace std;

struct Config
{
  int m1;
  int m2;
  int l;
  int j;
  int n;
  double b[6];
  double targetY;
};

Config readConfigFromCommandLine(string constraintsFile){
  Config r;
  auto m = readConstraints(constraintsFile);
  r.m1 = readWhileConstraint<int>("M", &m);
  r.m2 = readWhileConstraint<int>("M", &m);
  
  while(r.m1 == r.m2){
    cout << "Error! M1 and M2 should be distinct. Try again!" << endl;
    r.m1 = readWhileConstraint<int>("M", &m);
  }
  r.l = readWhileConstraint<int>("L", &m);
  r.j = readWhileConstraint<int>("J", &m);
  r.n = readWhileConstraint<int>("N", &m);
  
  for(int i = 0; i<6;i++){
    cout << "Enter value for B" << i + 1 << endl;
    cin >> r.b[i];
  }

  cout << "Enter value for target Y" << endl;
  cin >> r.targetY;
  return r;
}

bool checkAllNums(json j){
  if(j.is_array()){
    for(int i = 0; i < j.size(); i++){
      if(!j[i].is_number()) return false;
    }
    return true;
  }
  return false;
}

Config readConfigJson(string filename){
  Config r;
  json j;
  ifstream config_file(filename, ifstream::binary);
  config_file >> j;
  if(j.is_object()){
    
    if(j["M1"].is_number()) r.m1 = j["M1"];
    else cout << "ERROR! M1 should be a number!" << endl;

    if(j["M2"].is_number()) r.m2 = j["M2"];
    else cout << "ERROR! M2 should be a number!" << endl;

    if(j["L"].is_number()) r.l = j["L"];
    else cout << "ERROR! L should be a number!" << endl;

    if(j["J"].is_number()) r.j = j["J"];
    else cout << "ERROR! J should be a number!" << endl;

    if(j["N"].is_number()) r.n = j["N"];
    else cout << "ERROR! N should be a number!" << endl;

    if(j["Y"].is_number()) r.targetY = j["Y"];
    else cout << "ERROR! Y should be a number!" << endl;

    if(j["B"].is_array() && j["B"].size() == 6 && checkAllNums(j["B"])){
      for(int i = 0; i<6;i++){
        r.b[i] = j["B"][i];
      }
    } else cout << "ERROR! Malformed B array!" << endl;
  } else cout << "ERROR! Malformed config object" << endl;
  return r;
}


int main(int argc, char const *argv[])
{
  //auto cnf = readConfigFromCommandLine("constraints.json");
  auto cnf = readConfigJson("default_config.json");
  cout << cnf.m1 << endl;
  cout << cnf.m2 << endl;
  cout << cnf.l << endl;
  cout << cnf.n << endl;
  cout << cnf.targetY << endl;
  return 0;
}