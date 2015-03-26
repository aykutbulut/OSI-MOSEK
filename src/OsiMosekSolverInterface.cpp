
#include "OsiMosekSolverInterface.hpp"
#include <CoinMpsIO.hpp>
#include <mosek.h>
#include <algorithm>

// default constructor
OsiMosekSolverInterface::OsiMosekSolverInterface(): OsiMskSolverInterface() {
}

// copy constructor
OsiMosekSolverInterface::OsiMosekSolverInterface(const OsiMosekSolverInterface & other):
  OsiMskSolverInterface(other) {
}

// copy assignment operator
OsiMosekSolverInterface & OsiMosekSolverInterface::operator=(const OsiMosekSolverInterface & rhs) {
  // copy rhs to this
  OsiMskSolverInterface::operator=(rhs);
  return *this;
}

// get conic constraints
void OsiMosekSolverInterface::getConicConstraint(int index,
						 OsiLorentzConeType & type,
					       int & numMembers,
					       int *& members) const {
  //const MSKenv_t env = OsiMskSolverInterface::getEnvironmentPtr();
  const MSKtask_t task = OsiMskSolverInterface::getMutableLpPtr();
  MSKrescodee res;
  MSKconetypee conetype;
  double conepar;
  int nummem;
  int * submem;
  // get conic constraint
  res = MSK_getcone(task, index, &conetype, &conepar, &nummem, submem);
  std::cout << "Mosek error code " << res << std::endl;
  numMembers = nummem;
  // who will free members?
  members = new int[numMembers];
  std::copy(submem, submem+numMembers, members);
  if (conetype==MSK_CT_QUAD) {
    type = OSI_QUAD;
  }
  else if (conetype==MSK_CT_RQUAD) {
    type = OSI_RQUAD;
  }
}

// add conic constraint in lorentz cone form
void OsiMosekSolverInterface::addConicConstraint(OsiLorentzConeType type,
					       int numMembers,
					       const int * members) {
  MSKrescodee res;
  MSKconetypee conetype;
  const MSKtask_t task = OsiMskSolverInterface::getLpPtr();
  double conepar = 0.0;
  if (type==OSI_QUAD) {
    conetype = MSK_CT_QUAD;
  }
  else {
    conetype = MSK_CT_RQUAD;
  }
  res = MSK_appendcone(task, conetype, conepar, numMembers, members);
}

// add conic constraint in |Ax-b| <= dx-h form
void OsiMosekSolverInterface::addConicConstraint(CoinPackedMatrix const * A,
						 CoinPackedVector const * b,
						 CoinPackedVector const * d,
						 double h) {
  std::cerr << "Not implemented yet!" << std::cerr;
  throw std::exception();
}


void OsiMosekSolverInterface::removeConicConstraint(int index) {
  MSKrescodee res;
  MSKtask_t task = OsiMskSolverInterface::getLpPtr();
  int num = 1;
  int * subset;
  subset = new int[1];
  subset[0] = index;
  res = MSK_removecones(task, num, subset);
  delete[] subset;
}

void OsiMosekSolverInterface::modifyConicConstraint(int index,
						    OsiLorentzConeType type,
						    int numMembers,
						    const int * members) {
  std::cerr << "Not implemented yet!" << std::cerr;
  throw std::exception();
}


int OsiMosekSolverInterface::getNumCones() const {
  MSKrescodee res;
  MSKtask_t task = OsiMskSolverInterface::getMutableLpPtr();
  int num;
  res = MSK_getnumcone(task, &num);
  return num;
}

int OsiMosekSolverInterface::getConeSize(int i) const {
  std::cerr << "Not implemented yet!" << std::cerr;
  throw std::exception();
  return 0;
}

OsiConeType OsiMosekSolverInterface::getConeType(int i) const {
  std::cerr << "Not implemented yet!" << std::cerr;
  throw std::exception();
  return OSI_LORENTZ;
}

void OsiMosekSolverInterface::getConeSize(int * size) const {
  std::cerr << "Not implemented yet!" << std::cerr;
  throw std::exception();
}

void OsiMosekSolverInterface::getConeType(OsiConeType * type) const {
  std::cerr << "Not implemented yet!" << std::cerr;
  throw std::exception();
}


OsiConicSolverInterface * OsiMosekSolverInterface::clone(bool copyData) const {
  // we need to clone task and env, I think
  // OsiMskSolverInterface::clone will be enough.
  OsiMosekSolverInterface * new_solver = new OsiMosekSolverInterface(*this);
  return new_solver;
}

OsiMosekSolverInterface::~OsiMosekSolverInterface() {
  // free task and env? or OsiMskSolverInterface handles that.
}

int OsiMosekSolverInterface::readMps(const char * filename,
				     const char * extension) {
  // todo(aykut) this reads linear part and conic part
  OsiConicSolverInterface::readMps(filename, extension);
  // what will happen to loadProblem?
}

// void OsiMosekSolverInterface::loadProblem (const CoinPackedMatrix &matrix,
// 					   const double *collb,
// 					   const double *colub,
// 					   const double *obj,
// 					   const double *rowlb,
// 					   const double *rowub) {
//   OsiMskSolverInterface::loadProblem(matrix, collb, colub, obj, rowlb, rowub);
// }

// OsiMosekSolverInterface::OsiMosekSolverInterface(const OsiMosekSolverInterface & other) {
//   OsiMosekSolver
// }

// OsiMosekSolverInterface::OsiMosekSolverInterface(const OsiMskSolverInterface & other) {
//   OsiSolverInterface * solver = other->clone();

// }

// void OsiMosekSolverInterface::initialSolve() {
//   MSKrescodee res;
//   MSKtask_t task = OsiMskSolverInterface::getMutableLpPtr();
//   res = MSK_putintparam (task, MSK_IPAR_OPTIMIZER,  MSK_OPTIMIZER_CONIC);
//   res = MSK_putintparam (task, MSK_IPAR_OPTIMIZER,  MSK_OPTIMIZER_INTPNT);
//   MSKrescodee trmcode;
//   /* Run optimizer */
//   res = MSK_optimizetrm(task,&trmcode);
// }

// void OsiMosekSolverInterface::resolve() {
//   initialSolve();
// }

