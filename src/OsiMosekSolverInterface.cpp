#include "OsiMosekSolverInterface.hpp"
#include <CoinMpsIO.hpp>
#include <mosek.h>
#include <algorithm>

// default constructor
OsiMosekSolverInterface::OsiMosekSolverInterface(): OsiMskSolverInterface() {
  // set number of thread to 1.
  MSKrescodee res;
  MSKtask_t task = getLpPtr();
  res = MSK_putintparam(task, MSK_IPAR_NUM_THREADS, 1);
  if (res!=MSK_RES_OK) {
    std::cerr << "Mosek status " << res << std::endl;
    throw std::exception();
  }
}

// copy constructor
OsiMosekSolverInterface::OsiMosekSolverInterface(const OsiMosekSolverInterface & other):
  OsiMskSolverInterface(other) {
  // set number of thread to 1.
  MSKrescodee res;
  MSKtask_t task = getLpPtr();
  res = MSK_putintparam(task, MSK_IPAR_NUM_THREADS, 1);
  if (res!=MSK_RES_OK) {
    std::cerr << "Mosek status " << res << std::endl;
    throw std::exception();
  }
}

// copy assignment operator
OsiMosekSolverInterface & OsiMosekSolverInterface::operator=(const OsiMosekSolverInterface & rhs) {
  // copy rhs to this
  OsiMskSolverInterface::operator=(rhs);
  // set number of thread to 1.
  MSKrescodee res;
  MSKtask_t task = getLpPtr();
  res = MSK_putintparam(task, MSK_IPAR_NUM_THREADS, 1);
  if (res!=MSK_RES_OK) {
    std::cerr << "Mosek status " << res << std::endl;
    throw std::exception();
  }
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
  // get cone size
  res = MSK_getconeinfo(task, index, &conetype, &conepar, &nummem);
  if (res!=MSK_RES_OK) {
    std::cerr << "Mosek status " << res << std::endl;
    throw std::exception();
  }
  submem = new int[nummem];
  // get conic constraint
  res = MSK_getcone(task, index, &conetype, &conepar, &nummem, submem);
  if (res!=MSK_RES_OK) {
    std::cerr << "Mosek status " << res << std::endl;
    throw std::exception();
  }
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
  delete[] submem;
}

// add conic constraint in lorentz cone form
void OsiMosekSolverInterface::addConicConstraint(OsiLorentzConeType type,
					       int numMembers,
					       const int * members) {
  MSKrescodee res;
  MSKconetypee conetype;
  MSKtask_t task = OsiMskSolverInterface::getMutableLpPtr();
  double conepar = 0.0;
  if (type==OSI_QUAD) {
    conetype = MSK_CT_QUAD;
  }
  else {
    conetype = MSK_CT_RQUAD;
  }
  res = MSK_appendcone(task, conetype, conepar, numMembers, members);
  if (res!=MSK_RES_OK) {
    std::cerr << "Mosek status " << res << std::endl;
    throw std::exception();
  }
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
  if (res!=MSK_RES_OK) {
    std::cerr << "Mosek status " << res << std::endl;
    throw std::exception();
  }
  return num;
}

int OsiMosekSolverInterface::getConeSize(int i) const {
  const MSKtask_t task = OsiMskSolverInterface::getMutableLpPtr();
  MSKrescodee res;
  MSKconetypee conetype;
  double conepar;
  int nummem;
  // get conic constraint information
  res = MSK_getconeinfo(task, i, &conetype, &conepar, &nummem);
  if (res!=MSK_RES_OK) {
    std::cerr << "Mosek status " << res << std::endl;
    throw std::exception();
  }
  return nummem;
}

// for mosek all cones are lorentz cones.
OsiConeType OsiMosekSolverInterface::getConeType(int i) const {
  int num_cones = getNumCones();
  if (i>=num_cones) {
    std::cerr << __PRETTY_FUNCTION__ << "Cone " << i << " does not exist!"
              << std::endl;
    throw std::exception();
  }
  return OSI_LORENTZ;
}

OsiLorentzConeType OsiMosekSolverInterface::getLorentzConeType(int i) const {
  const MSKtask_t task = OsiMskSolverInterface::getMutableLpPtr();
  MSKrescodee res;
  MSKconetypee conetype;
  double conepar;
  int nummem;
  OsiLorentzConeType type;
  // get conic constraint information
  res = MSK_getconeinfo(task, i, &conetype, &conepar, &nummem);
  if (res!=MSK_RES_OK) {
    std::cerr << "Mosek status " << res << std::endl;
    throw std::exception();
  }
  if (conetype==MSK_CT_QUAD) {
    type = OSI_QUAD;
  }
  else if (conetype==MSK_CT_RQUAD) {
    type = OSI_RQUAD;
  }
  else {
    std::cerr << __PRETTY_FUNCTION__ << " Unknown mosek cone type!"
              << std::endl;
    throw std::exception();
  }
  return type;
}

// fills array of cone sizes.
void OsiMosekSolverInterface::getConeSize(int * size) const {
  const MSKtask_t task = OsiMskSolverInterface::getMutableLpPtr();
  MSKrescodee res;
  MSKconetypee conetype;
  double conepar;
  int nummem;
  int num_cones = getNumCones();
  for (int i=0; i<num_cones; ++i) {
    // get conic constraint information
    res = MSK_getconeinfo(task, i, &conetype, &conepar, &nummem);
    if (res!=MSK_RES_OK) {
      std::cerr << "Mosek status " << res << std::endl;
      throw std::exception();
    }
    size[i] = nummem;
  }
}

// fills array of cone types.
void OsiMosekSolverInterface::getConeType(OsiConeType * type) const {
  int num_cones = getNumCones();
  for (int i=0; i<num_cones; ++i) {
    type[i] = getConeType(i);
  }
}

void OsiMosekSolverInterface::getConeType(OsiLorentzConeType * type) const {
  int num_cones = getNumCones();
  for (int i=0; i<num_cones; ++i) {
    type[i] = getLorentzConeType(i);
  }
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

void OsiMosekSolverInterface::writeMps (const char *filename, const char *extension,
					double objSense) const {
  const MSKtask_t task = OsiMskSolverInterface::getMutableLpPtr();
  MSKrescodee res;
  std::string fn = std::string(filename)+std::string(".")
    +std::string(extension);
  res = MSK_writedata(task, (char const *) fn.c_str());
  if (res!=MSK_RES_OK) {
    std::cerr << "Mosek status " << res << std::endl;
    throw std::exception();
  }
}

void OsiMosekSolverInterface::markHotStart() {
  // do nothing. no hot start for conic problems
}

void OsiMosekSolverInterface::solveFromHotStart() {
  // solve from scratch
  resolve();
}

void OsiMosekSolverInterface::unmarkHotStart() {
  // do nothing.
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

