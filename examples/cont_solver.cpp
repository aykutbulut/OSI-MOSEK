// Solves conic problems, reads them in mosek mps input format
// usage: ./cont_solver input.mps

#include <OsiMosekSolverInterface.hpp>
#include <iostream>
#include <iomanip>

int main(int argc, char ** argv) {
  OsiConicSolverInterface * solver = new OsiMosekSolverInterface();
  solver->readMps(argv[1]);
  solver->initialSolve();
  std::cout << "Objective is " << std::setprecision(15)
	    << solver->getObjValue() << std::endl;
  // write mps file
  solver->writeMps("problem");
  return 0;
}
