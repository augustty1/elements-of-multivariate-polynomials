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
  F.todats("mvpoly.dat-s");
  F.scipsdp("solvescript.sh");
  F.readsolution("solution.sol");
  F.soscholesky();

  // just for debuging
  F.mbdbg();
  }
  catch (const std::exception& e){
    std::cerr << "Error: "<<e.what()<<'\n';
    return(1);
  }

  return(0);
}
