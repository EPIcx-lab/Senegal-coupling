/*I defines */

#include <cstdlib>      // std::rand, std::srand
#include <cmath>
#include <ctime>
#include <cstring>


#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_sf_pow_int.h>
#include <iostream>
#include <vector>
#include <algorithm>    // std::random_shuffle
#include <fstream>
//#include <unordered_map>
#include <map>
#include <array>

#include <fstream>
#include <sstream>
#include<sys/stat.h>
#include<sys/types.h>
#include<iostream>
#include<fstream>

#include <string.h>

using namespace std;
using std::vector;

/**************************************************/


#define epsilon 0.67 // latency
#define mu 0.33  // rate recovered


#define n_patch 430  
#define TIME 2000
#define n_seed 87
/*TYPEDEF*/



typedef unsigned int unint;
typedef unsigned short sint;
typedef unsigned short int usint;
typedef unsigned int unint;


typedef struct v{
	int n_subpop;
        int degree_out;
        int degree_in;
}nodo;

typedef struct z{
  	int count;
        double prob;

}rec;
