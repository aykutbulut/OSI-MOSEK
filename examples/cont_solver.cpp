// Solves conic problems, reads them in mosek mps input format
// usage: ./cont_solver input.mps

#include <OsiMosekSolverInterface.hpp>
#include <iostream>
#include <iomanip>
#include <mosek.h>

static void MSKAPI printstr(void *handle, MSKCONST char str[]) {
  printf("%s",str);
} /* printstr */

int main(int argc, char ** argv) {
  OsiConicSolverInterface * solver = new OsiMosekSolverInterface();
  solver->readMps(argv[1]);
  clock_t start_time = clock();
  solver->initialSolve();
  clock_t duration = clock() - start_time;
  // write mps file
  // solver->writeMps("problem");
  MSKtask_t task = dynamic_cast<OsiMosekSolverInterface*>(solver)->getLpPtr();
  MSK_linkfunctotaskstream(task,MSK_STREAM_LOG,NULL,printstr);
  MSK_solutionsummary(task, MSK_STREAM_MSG);
  std::cout << "Objective is  : " << std::setprecision(15)
	    << solver->getObjValue() << std::endl;
  std::cout << "CPU time spent: " << double(duration)/double(CLOCKS_PER_SEC)
	    << std::endl;
  return 0;
}
