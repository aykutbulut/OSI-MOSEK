# OsiMosek

OsiMosek is a conic solver interface for [Mosek][1] solver. OsiMosek
implements [OsiConic][2] interface, which extends Open Solver Interface (OSI)
to second order conic optimization problems.

OsiMosek depends on [CoinUtils][3], [OSI][4] and OsiConic. OsiMosek
extends COIN-OR's linear Mosek interface OsiMsk.

OsiMosek is used by [DisCO][6] to solve mixed integer conic optimization
problems.

[1]: https://mosek.com/
[2]: https://github.com/aykutbulut/OSI-CONIC
[3]: https://projects.coin-or.org/CoinUtils
[4]: https://projects.coin-or.org/Osi
[6]: https://github.com/aykutbulut/DisCO

# Basic Usage

Following code snippet reads problem from an MPS file (an extended MPS
format for second order cone problems) and solves it using Mosek.

```C++
// Solves conic problems, reads them in mosek mps input format
// usage: ./cont_solver input.mps

#include <OsiMosekSolverInterface.hpp>

int main(int argc, char ** argv) {
  OsiConicSolverInterface * solver = new OsiMosekSolverInterface();
  solver->readMps(argv[1]);
  solver->initialSolve();
  delete solver;
  return 0;
}
```

Currently only Mosek style MPS files are supported. You can find the complete
example in the examples directory.

# Install

## Basic installation

OsiMosek is tested/works in Linux environment only for now. To install
OsiMosek you need to have a Mosek solver with a valid license installed
in your computer. You should compile OSI with Mosek first. You can do this
with the following command.

```shell
./configure --prefix=build_dir --with-mosek-incdir=/mosek_dir/h --with-mosek-lib="-L/mosek_dir/bin -lmosek64 -lmosekxx7_1 -lmosekjava7_1 -lmosekscopt7_1 -liomp5"
make install
```

First command configures Osi with Mosek. You need to replace ```build_dir```
for the directory that you want to install OSI. You need to replace
```mosek_dir``` with your mosek installation directory. This command assumes
you are using Mosek 7.1. You need to update related words for a different Mosek
version. Second command install OSI to ```build_dir```. Please see
[Osi documentation][4] for details.

You need to configure CoinUtils and OsiConic the same way using the same ```build_dir```.

Once Osi and other dependencies are installed you can install OsiMosek with the
following command

```shell
./configure --prefix=build_dir && make install
```

OsiMosek configure script will find dependencies as long as you use same
```build_dir``` that you used during configuration of them.


## Installation Instructions for Advanced Users

If you already have the dependencies somewhere else in your computer and you do
not want to install them again, you are covered. First you need to make sure
that dependencies can be found with package config (```pkgconfig```
command). For this you need to add the directory that has ```.pc``` files of
dependencies to your ```PKG_CONFIG_PATH```. You can test whether the
dependencies are accesible with pkg-config with the following command,
```pkg-config --cflags --libs osi-mosek```.

Once the dependencies are accessible through pkg-config you can install
OsiMosek by using regular ```configure```, ```make``` and ```make install```
sequence.  Configure script will find the dependencies through pkg-config and
link OsiMosek to them.

[1]: https://mosek.com/
[2]: https://github.com/aykutbulut/OSI-CONIC
[3]: https://projects.coin-or.org/CoinUtils
[4]: https://projects.coin-or.org/Osi
[6]: https://github.com/aykutbulut/DisCO
