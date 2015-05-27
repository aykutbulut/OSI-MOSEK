#include <OsiMosekSolverInterface.hpp>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <numeric>

void print_cone_feasibility(OsiConicSolverInterface const * const solver);

int main(int argc, char ** argv) {
  OsiConicSolverInterface * solver = new OsiMosekSolverInterface();
  solver->readMps(argv[1]);
  // ConeSet * cset = get_cones(solver);
  solver->initialSolve();
  // print_cone_feasibility(solver);
  std::cout << "Objective is " << std::setprecision(15)
	    << solver->getObjValue() << std::endl;
  // print_cone_feasibility(solver, cset);
  // free_cones(cset);
  return 0;
}

void print_cone_feasibility(OsiConicSolverInterface const * const solver) {
  int num_cones = solver->getNumCones();
  OsiLorentzConeType type;
  int size;
  int * members;
  double term1;
  double term2;
  int start = -1;
  double const * sol = solver->getColSolution();
  double feas = 0.0;
  std::cout << std::setw(10) << "Cone"
	    << std::setw(10) << "type"
	    << std::setw(10) << "x1-||x_2n||"
	    << std::endl;
  for (int i=0; i<num_cones; ++i) {
    solver->getConicConstraint(i, type, size, members);
    if (type==OSI_QUAD) {
      term1 = sol[members[0]];
      start=1;
    }
    else if (type==OSI_RQUAD) {
      term1 = 2.0*sol[members[0]]*sol[members[1]];
      start = 2;
    }
    term2 = 0.0;
    for (int j=start; j<size; ++j) {
      term2 += sol[members[i]]*sol[members[i]];
    }
    if (type==OSI_QUAD) {
      feas = term1 - sqrt(term2);
    }
    else if (type==OSI_RQUAD) {
      feas = term1 - term2;
    }
    std::cout << std::setw(10) << i
	      << std::setw(10) << type
	      << std::setw(10) << feas
	      << std::endl;
    delete[] members;
  }
}


// struct Cone {
//   OsiConeType type;
//   int size;
//   int * members;
// };

// struct ConeSet {
//   int num_cones;
//   Cone * cones;
// };

// ConeSet * get_cones(OsiConicSolverInterface const * const solver);
// void print_cone_feasibility(OsiConicSolverInterface const * const solver,
// 			    ConeSet const * const cset);
//void free_cones(ConeSet * cset);

// ConeSet * get_cones(OsiConicSolverInterface const * const solver) {
//   ConeSet * cs;
//   int num_cones = solver->getNumCones();
//   cs->num_cones = num_cones;
//   cs->cones = new Cone[num_cones];
//   OsiConeType type;
//   int size;
//   int * members;
//   for (int i=0; i<num_cones; ++i) {
//     solver->getConicConstraint(i, type, size, members);
//     cs->cones[i].size = size;
//     cs->cones[i].members = members;
//   }
//   return cs;
// }

// void free_cones(ConeSet * cset) {
//   int num_cones = cset->num_cones;
//   for (int i=0; i<num_cones; ++i) {
//     delete[] cset->cones[i].members;
//   }
//   delete[] cset->cones;
// }

// void print_cone_feasibility(OsiConicSolverInterface const * const solver,
// 			    ConeSet const * const cset) {
// }
