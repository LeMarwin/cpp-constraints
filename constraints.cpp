#include "climits"
#include "vector"
#include <math.h>
#include "json.hpp"
#include <fstream>
#include <map>
#include <string>
#include "iostream"

using json = nlohmann::json;
using namespace std;


class AbstractConstraint
{
  public:
    virtual bool check(double v) = 0;
    virtual double fit(double v) = 0;
    AbstractConstraint(){};
    virtual ~AbstractConstraint(){};
};

using ConstrMap = map<string, AbstractConstraint*>;

class DiscreteConstraint: public AbstractConstraint
{
  private:
    vector<int> values;
  public:
    DiscreteConstraint(){};
    ~DiscreteConstraint(){};

    DiscreteConstraint(vector<int> v){
      values = v;
    };

    void add(int v){
      values.push_back(v);
    }

    bool check(double v){
      int iv = round(v);
      for (int i : values){
        if(i == iv) return true;
      }
      return false;
    }

    double fit(double v){
      int res = round(v);
      int dist = INT_MAX;
      for (int i = 0; i < values.size(); ++i)
      {
        float tdist = abs(values[i] - v); 
        if(tdist < dist)
        {
          dist = tdist;
          res = values[i];
        }
      }
      return static_cast<double>(res);
    }
};

class DiscreteRangeConstraint: public AbstractConstraint
{
  private:
    int minval;
    int maxval; 

  public:
    DiscreteRangeConstraint(){};
    ~DiscreteRangeConstraint(){};
    DiscreteRangeConstraint(int a, int b){
      minval = a;
      maxval = b;
    }

  bool check(double v){
    int check_int = round(v);
    return (minval <= check_int && check_int <= maxval);
  }

  double fit(double v){
    if(v < minval) return minval;
    if(v > maxval) return maxval;
    return v;
  }
};

class FloatingConstraint: public AbstractConstraint
{
  private:
    double minval;
    double maxval; 
  public:
    FloatingConstraint(){};
    ~FloatingConstraint(){};
    FloatingConstraint(double a, double b){
      minval = a;
      maxval = b;
    }

    bool check(double v){
      return (minval <= v && v <= maxval);
    }

    double fit(double v){
      if(v < minval) return minval;
      if(v > maxval) return maxval;
      return v;
    }
};

vector<int> jsonToVec(json j){
  vector<int> v;
  if(j.is_array()){
    for (int i = 0; i < j.size(); ++i)
    {
      if(j[i].is_number()){
        v.push_back(j[i]);
      }
    }
  }
  return v;
};

ConstrMap readConstraints(string filename){
  map<string, AbstractConstraint*> res;
  json j;
  ifstream constraints_file(filename, ifstream::binary);
  constraints_file >> j;
  if(j.is_array()){
    for (int i = 0; i < j.size(); i++){
      if(j[i]["type"].is_string() && j[i]["name"].is_string())
      {
        string type = j[i]["type"].dump();
        type = type.substr(1, type.size() - 2);
        string name = j[i]["name"].dump();
        name = name.substr(1, name.size() - 2);
        if(type == "discrete"){
          DiscreteConstraint* dc = new DiscreteConstraint(jsonToVec(j[i]["values"]));
          res.insert(pair<string,AbstractConstraint*>(name,dc));
        }
        if(type == "floating"){
          if(j[i]["minval"].is_number() && j[i]["maxval"].is_number()){
            FloatingConstraint* fc = new FloatingConstraint(j[i]["minval"],j[i]["maxval"]);
            res.insert(pair<string,AbstractConstraint*>(name,fc));
          }
        }
        if(type == "discrete_range"){
          if(j[i]["minval"].is_number() && j[i]["maxval"].is_number()){
            DiscreteRangeConstraint* drc = new DiscreteRangeConstraint(j[i]["minval"],j[i]["maxval"]);
            res.insert(pair<string,AbstractConstraint*>(name,drc));
          } 
        }
      }
    }
  }
  return res;
}


template <typename T>
T readWhileConstraint(string key, ConstrMap* cm){
  T res;
  cout << "Enter value for " << key << endl;
  cin >> res;
  while(!cm->at(key)->check(res)){
    cout << "Incorrect value for " << key << ". Try again." << endl;
    cin >> res;
  }
  return res;
}


/*
int main(int argc, char const *argv[])
{
  //Вот это важна
  //Загрузить все ограничения из файла
  map<string, AbstractConstraint*> m = readConstraints("constraints.json");
  // Это просто дебаг
  //  cout << m.size() << "\n";

  //  Это просто дебаг
  //  cout << "L3 check 0\t" << m.at("L3")->check(0) << "\n";
  //  cout << "L3 check -100\t" << m.at("L3")->check(-100) << "\n";
  //  cout << "L3 fit -100 \t" << m.at("L3")->fit(-100) << "\n";
  //  cout << "L1 check -1\t" << m.at("L1")->check(-1) << "\n";
  //  cout << "L1 check 0\t" << m.at("L1")->check(0) << "\n";
  //  cout << "L1 fit 0.75\t" << m.at("L1")->fit(0.75) << "\n";

  int m1_input;
  cin >> m1_input;
  while(!m.at("J")->check(m1_input)){
    cout << "Incorrect value for M1. Try again." << endl;
    cin >> m1_input;
  }
  return 0;
}

*/