/*
 *	Converts an multivariate polynomial sparse point value representation to the sparse sdpa format
 */
#include <exception>
#include <iostream>
#include "../include/mvpolynomial.hpp"

int main () {
  
  try{
  /*
   * Reading the multivariate polynomial here
   */
  mvpolyT F; //(x_1, ..., x_n)
  
  std::cout<<"Polynomial"<<'\n';
  F.dbg();

  std::cout << '\n';
  /*
   * pre-processing and
   * constraint generation
   */
  F.todats("mvpoly.dat-s");

  /*
   * communicates with 
   * scip via interactive shell
   */
  F.scipsdp("solvescript.sh");

  /*
   * post-processing and
   * sos gram matrix reconstruction
   */
  F.readsolution("solution.sol");

  // TODO projection in affine subspace
  F.exactify();

  F.LDLT();

  // just for debuging
  F.mbdbg();
  }
  catch (const std::exception& e){
    std::cerr << "Error: "<<e.what()<<'\n';
    return(1);
  }

  return(0);
}
