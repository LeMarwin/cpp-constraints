#include "iostream"
#include <limits>
#include <cmath>
#include "constraints.cpp"
#include "configManager.cpp"
#include "plant.c"
#include "plant.h"

using namespace std;

double calcU(double x1, double x2, Config* cnf, ConstrMap* cmap){
  auto b = cnf->b;
  double u = b[0] + x1 * b[1] + b[2] * log(cnf->targetY + b[3] + b[4] * exp(b[5]*x2));
  return cmap->at("L" + std::to_string(cnf->l))->fit(u);
}

double calcQ(Config* cnf, ConstrMap* cmap){
  Plant plant;
  plant_init(plant);
  double q = 0;
  double x1,x2,u,y;
  for (int i = 0; i < cnf->n; ++i)
  {
    x1 = plant_measure(cnf->m1, plant);
    x2 = plant_measure(cnf->m2, plant);
    u  = calcU(x1,x2,cnf,cmap);
    plant_control(cnf->l, i, plant);
    y = plant_measure(cnf->j,plant);
    q += pow((cnf->targetY - y),2);
    cout << "\t\tx1\t" << x1 << "\tx2\t" << x2 <<"\tu\t" << u << "\ty\t" << y << "\tq\t" << q/cnf->n << endl; 
  }
  return q/cnf->n;
}

void searchBi(int i, double minval, double maxval, int steps, Config* cnf, ConstrMap* cmap){
  double db = abs(maxval - minval)/steps;
  double qmin = std::numeric_limits<double>::max();
  double bi_min = minval;
  for(double bi = minval; bi < maxval; bi+=db){
    cnf->b[i] = bi;
    double q = calcQ(cnf,cmap);
    //cout << "\t" << cnf->b[i] << "\t" << q << endl;
    if(q < qmin){
      qmin = q;
      bi_min = bi;
    }
  }
  cnf->b[i] = bi_min;
  //return bi_min;
}

int main(int argc, char const *argv[])
{
	//Считывает все входные данные с консоли
	//auto cnf = readConfigFromCommandLine("constraints.json");

	//Читает все входные данные из json-файла
	auto cnf = readConfigJson("default_config.json", "constraints.json");
	auto cmap = readConstraints("constraints.json");

  Plant plant;
  plant_init(plant);

  for (int i = 0; i < 100; ++i)
  { 
    cout << plant_measure(90,plant) << endl;
    plant_control(8,i,plant);
  }

//  searchBi(0,-100,100,100,&cnf,&cmap);
//  cout << "b0\t" << cnf.b[0] << endl;
//  searchBi(1,-10,10,100,&cnf,&cmap);
//  cout << "b1\t" << cnf.b[1] << endl;
//  searchBi(2,-10,10,100,&cnf,&cmap);
//  cout << "b2\t" << cnf.b[2] << endl;
//  searchBi(3,-10,10,100,&cnf,&cmap);
//  cout << "b3\t" << cnf.b[3] << endl;
//  searchBi(4,-10,10,100,&cnf,&cmap);
//  cout << "b4\t" << cnf.b[4] << endl;
//  searchBi(5,-0.0005,0.0005,100,&cnf,&cmap);
//  cout << "b5\t" << cnf.b[5] << endl;
//  for (int i = 0; i < 6; ++i)
//  {
//    cout << cnf.b[i] << "\t";
//  }
//  cout << "|\t" << calcQ(&cnf,&cmap) << endl;

//  double q = 0;
//  double x1,x2,u,y;
//  for (int i = 0; i < 100; ++i)
//  {
//    x1 = plant_measure(cnf.m1, plant);
//    x2 = plant_measure(cnf.m2, plant);
//    u  = calcU(x1,x2,&cnf,&cmap);
//    cout << x1 << "\t" << x2 << "\t";
//    cout << u << endl;
//    plant_control(cnf.l, u, plant);
//    y = plant_measure(cnf.j,plant);
//    q += pow((cnf.targetY - y),2);
//    cout << "y\t" << y << "\tq\t" << q << endl;
//  }
//  cout << q/cnf.n << endl;
	return 0;
}