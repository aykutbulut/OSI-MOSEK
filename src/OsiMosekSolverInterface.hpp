#ifndef OsiMosekSolverInterface_H
#define OsiMosekSolverInterface_H

#include <OsiConicSolverInterface.hpp>
#include <OsiMskSolverInterface.hpp>
#include <CoinPackedMatrix.hpp>

class OsiMosekSolverInterface: virtual public OsiConicSolverInterface,
			       virtual public OsiMskSolverInterface {
public:
  // default constructor
  OsiMosekSolverInterface();
  // copy constructor
  OsiMosekSolverInterface(const OsiMosekSolverInterface & other);
  // copy assignment operator
  OsiMosekSolverInterface & operator=(const OsiMosekSolverInterface & rhs);
  virtual ~OsiMosekSolverInterface();
  // get conic constraints
  virtual void getConicConstraint(int index, OsiConeType & type,
				  int & numMembers,
				  int *& members) const;
  // add conic constraints
  virtual void addConicConstraint(OsiConeType type,
				  int numMembers,
				  const int * members);
  virtual void removeConicConstraint(int index);
  virtual int getNumCones() const;
  virtual OsiConicSolverInterface * clone(bool copyData=true) const;
  virtual int readMps(const char * filename, const char * extension="mps");
  // inherited from OsiMskSolverInterface <- OsiSolverInterface
  // virtual void loadProblem (const CoinPackedMatrix &matrix, const double *collb,
  // 			    const double *colub, const double *obj,
  // 			    const double *rowlb, const double *rowub);
  //virtual void initialSolve();
  //virtual void resolve();
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

