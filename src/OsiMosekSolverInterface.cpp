#include "OsiMosekSolverInterface.hpp"
#include <CoinMpsIO.hpp>
#include <mosek.h>
#include <algorithm>

// todo(aykut)
// error check routine. Copied from OsiMskSolverInterface.
// I am not sure whether this is a license violation. This
// may be removed/replaced in future.
// I would not experince any problem if this function was not static
// in OsiMskSolverInterface. Since it is not I have to copy it here
// and create redundancy.
static inline void
checkMSKerror( int err, std::string mskfuncname, std::string osimethod )
{
  if( err != MSK_RES_OK )
  {
    char s[100];
    sprintf( s, "%s returned error %d", mskfuncname.c_str(), err );
    std::cout << "ERROR: " << s << " (" << osimethod <<
    " in OsiMskSolverInterface)" << std::endl;
    throw CoinError( s, osimethod.c_str(), "OsiMskSolverInterface" );
  }
}


// default constructor
OsiMosekSolverInterface::OsiMosekSolverInterface(): OsiMskSolverInterface() {
  // set number of thread to 1.
  MSKrescodee res;
  MSKtask_t task = getLpPtr();
  // set number of threads to 1
  res = MSK_putintparam(task, MSK_IPAR_NUM_THREADS, 1);
  checkMSKerror(res, "MSK_putintparam", "OsiMosekSolverInterface");
  // ignore integrality constraints
  res = MSK_putintparam(task, MSK_IPAR_MIO_MODE, MSK_MIO_MODE_IGNORED);
  checkMSKerror(res, "MSK_putintparam", "OsiMosekSolverInterface");
  // set optimizer to conic
  res = MSK_putintparam (task, MSK_IPAR_OPTIMIZER,  MSK_OPTIMIZER_CONIC);
  checkMSKerror(res, "MSK_putintparam", "OsiMosekSolverInterface");
}

// copy constructor
OsiMosekSolverInterface::OsiMosekSolverInterface(const OsiMosekSolverInterface & other):
  OsiMskSolverInterface(other) {
  // set number of thread to 1.
  MSKrescodee res;
  MSKtask_t task = getLpPtr();
  // set number of threads to 1
  res = MSK_putintparam(task, MSK_IPAR_NUM_THREADS, 1);
  checkMSKerror(res, "MSK_putintparam", "OsiMosekSolverInterface");
  // ignore integrality constraints
  res = MSK_putintparam(task, MSK_IPAR_MIO_MODE, MSK_MIO_MODE_IGNORED);
  checkMSKerror(res, "MSK_putintparam", "OsiMosekSolverInterface");
  // set optimizer to conic
  res = MSK_putintparam (task, MSK_IPAR_OPTIMIZER,  MSK_OPTIMIZER_CONIC);
  checkMSKerror(res, "MSK_putintparam", "OsiMosekSolverInterface");
}

// copy assignment operator
OsiMosekSolverInterface & OsiMosekSolverInterface::operator=(const OsiMosekSolverInterface & rhs) {
  // copy rhs to this
  OsiMskSolverInterface::operator=(rhs);
  // set number of thread to 1.
  MSKrescodee res;
  MSKtask_t task = getLpPtr();
  // set number of threads to 1
  res = MSK_putintparam(task, MSK_IPAR_NUM_THREADS, 1);
  checkMSKerror(res, "MSK_putintparam", "OsiMosekSolverInterface");
  // ignore integrality constraints
  res = MSK_putintparam(task, MSK_IPAR_MIO_MODE, MSK_MIO_MODE_IGNORED);
  checkMSKerror(res, "MSK_putintparam", "OsiMosekSolverInterface");
  // set optimizer to conic
  res = MSK_putintparam (task, MSK_IPAR_OPTIMIZER,  MSK_OPTIMIZER_CONIC);
  checkMSKerror(res, "MSK_putintparam", "OsiMosekSolverInterface");
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
  checkMSKerror(res, "MSK_getconeinfo", "getConicConstraints");
  submem = new int[nummem];
  // get conic constraint
  res = MSK_getcone(task, index, &conetype, &conepar, &nummem, submem);
  checkMSKerror(res, "MSK_getcone", "getConicConstraints");
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
  MSKtask_t task = OsiMskSolverInterface::getLpPtr();
  double conepar = 0.0;
  if (type==OSI_QUAD) {
    conetype = MSK_CT_QUAD;
  }
  else {
    conetype = MSK_CT_RQUAD;
  }
  res = MSK_appendcone(task, conetype, conepar, numMembers, members);
  checkMSKerror(res, "MSK_appendcone", "addConicConstraint");
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
  checkMSKerror(res, "MSK_removecones", "removeConicConstraint");
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
  checkMSKerror(res, "MSK_getnumcone", "getNumCones");
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
  checkMSKerror(res, "MSK_getconeinfo", "getConeSize");
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
  checkMSKerror(res, "MSK_getconeinfo", "getLorentzConeType");
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
    checkMSKerror(res, "MSK_getconeinfo", "getConeSize");
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
  checkMSKerror(res, "MSK_writedata", "writeMps");
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

// solve continuous problem. ignore discrete variables if any
void OsiMosekSolverInterface::initialSolve() {
  MSKrescodee res;
  MSKtask_t task = OsiMskSolverInterface::getLpPtr();
  // MSK MIO MODE IGNORED
  // res = MSK_putintparam (task, MSK_IPAR_OPTIMIZER,  MSK_OPTIMIZER_FREE);
  // res = MSK_putintparam (task, MSK_IPAR_OPTIMIZER,  MSK_OPTIMIZER_MIXED_INT_CONIC);
  // res = MSK_putintparam (task, MSK_IPAR_OPTIMIZER,  MSK_OPTIMIZER_INTPNT);
  MSKrescodee trmcode;
  /* Run optimizer */
  res = MSK_optimizetrm(task,&trmcode);
  if (res!=MSK_RES_OK) {
    std::cerr << "Mosek status " << res << std::endl;
    throw std::exception();
  }
  // todo(aykut) update optimization status using trmcode.
}

void OsiMosekSolverInterface::resolve() {
  initialSolve();
}

