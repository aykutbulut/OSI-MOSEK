#ifndef OsiMosekSolverInterface_H
#define OsiMosekSolverInterface_H

#include <OsiConicSolverInterface.hpp>
#include <OsiMskSolverInterface.hpp>
#include <CoinPackedMatrix.hpp>

class OsiMosekSolverInterface: virtual public OsiConicSolverInterface,
			       virtual public OsiMskSolverInterface {
  OsiLorentzConeType getLorentzConeType(int i) const;
public:
  // default constructor
  OsiMosekSolverInterface();
  // copy constructor
  OsiMosekSolverInterface(const OsiMosekSolverInterface & other);
  // copy assignment operator
  OsiMosekSolverInterface & operator=(const OsiMosekSolverInterface & rhs);
  virtual ~OsiMosekSolverInterface();
  // get conic constraints
  virtual void getConicConstraint(int index, OsiLorentzConeType & type,
				  int & numMembers,
				  int *& members) const;
  // add conic constraints
  // add conic constraint in lorentz cone form
  virtual void addConicConstraint(OsiLorentzConeType type,
				  int numMembers,
				  const int * members);
  // add conic constraint in |Ax-b| <= dx-h form
  virtual void addConicConstraint(CoinPackedMatrix const * A, CoinPackedVector const * b,
				  CoinPackedVector const * d, double h);
  virtual void removeConicConstraint(int index);
  virtual void modifyConicConstraint(int index, OsiLorentzConeType type,
				     int numMembers,
				     const int * members);
  virtual int getNumCones() const;
  virtual int getConeSize(int i) const;
  virtual OsiConeType getConeType(int i) const;
  virtual void getConeSize(int * size) const;
  virtual void getConeType(OsiConeType * type) const;
  virtual void getConeType(OsiLorentzConeType * type) const;
  virtual OsiConicSolverInterface * clone(bool copyData=true) const;
  virtual int readMps(const char * filename, const char * extension="mps");
  virtual void writeMps (const char *filename, const char *extension="mps",
			 double objSense=0.0) const;
  // hot start methods
  // over-write linear mosek solver interface functions
  virtual void markHotStart();
  virtual void solveFromHotStart();
  virtual void unmarkHotStart();
  virtual void initialSolve();
  virtual void resolve();
  // PROBLEM STATUS QUERRY FUNCTIONS
  virtual bool isAbandoned () const;
  virtual bool isProvenOptimal () const;
  virtual bool isProvenPrimalInfeasible () const;
  virtual bool isProvenDualInfeasible () const;
  virtual bool isPrimalObjectiveLimitReached () const;
  virtual bool isDualObjectiveLimitReached () const;
  virtual bool isIterationLimitReached () const;
  // CONSTRUCTORS
  //OsiMosekSolverInterface();
  // copy constructor
  //OsiMosekSolverInterface(const OsiMosekSolverInterface & other);
  // copy constructor from OsiMskSolverInterface
  //OsiMosekSolverInterface(const OsiMskSolverInterface & other);
  // assignment operator
  //OsiMosekSolverInterface & operator=(const OsiMosekSolverInterface & rhs);

};

#endif

