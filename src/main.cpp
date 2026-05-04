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

  monomialbasisT m = F.monomialbasis();
  m.dbg();
  }
  catch (const std::exception& e){
    std::cerr << "Error: "<<e.what()<<'\n';
    return(1);
  }

  return(0);
}
