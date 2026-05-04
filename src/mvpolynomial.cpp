#include <iostream>
#include <stdexcept>
#include "../include/mvpolynomial.hpp"

mvpolyT::mvpolyT () {
  std::cin>>nvars;
  coeff c;
  exps e(nvars);
  
  while(std::cin>>c) {
    for (int i = 0; i<nvars; i++)
      std::cin>>e[i];
    addterm(e,c);
  }
}

void mvpolyT::addterm (const exps& _e, coeff _c) {
  if(_c == 0)
    return;

  auto it = terms.find(_e);
  if(it != terms.end()) {
    it->second += _c;

    if(it->second == 0)
      terms.erase(it);
  }
  else {
    terms[_e] = _c; 
  }
}

int mvpolyT::degree() const {
  int d = 0;
  
  std::map<exps, coeff>::const_iterator it;
  for (it = terms.begin(); it!= terms.end(); it++) {
    int sum = 0;
    for (int e : it->first)
      sum+=e;

    d = std::max(d,sum);
  }
  return(d);
}

bool mvpolyT::homogeneous() const {
  if(terms.empty())
    return (true);

  std::map<exps, coeff>::const_iterator it;
  it = terms.begin();
  int firs_degree = 0;
  for (int e: it->first)
    firs_degree+=e;

  for (it = terms.begin(); it != terms.end(); it++) {
    int d = 0;
    for (int e: it->first)
      d+=e;

    if(d != firs_degree)
      return(false);
  }
  return(true);
}

/*
 * The number of entries in this vector is C(n+d, d).
 * Note that the monomial indices are all tuples of 
 * length n with non-negative integrer entries summing
 * to at most d (which can be generated with recursion
 * and backtracking).
 * Remark: If the input polynomial is homogeneous of 
 * degree 2d, then it is sufficiente to restrict the 
 * components of monomialbasis to the monomials of 
 * degree exactly equal to d.
 */ 
void mvpolyT::rec_monomialbasis(int v, int remain, exps& curr, std::vector<exps>& ans) const {
  /*
   * Last variable
   */
  if (v == nvars-1) {
    curr[v] = remain;
    ans.push_back(curr);
    return;
  }

  /*
   * For the actual variable, try all the powers
   */
  for ( int e = 0; e<=remain; e++) {
    curr[v]=e;
    rec_monomialbasis(v+1, remain - e, curr, ans);
  }
}

monomialbasisT mvpolyT::monomialbasis() const {
  int twod = degree();
  if (twod % 2 != 0)
    throw std::runtime_error("Polynomials with odd degree always have negative points");
  
  monomialbasisT m;
  m.degree = twod / 2;
  m.homogeneous = homogeneous();

  if (m.homogeneous) {
    exps curr(nvars);
    rec_monomialbasis(0, m.degree, curr, m.monomials);
  }
  else {
    for(int d = 0; d<= m.degree; d++){
      exps curr(nvars);
      rec_monomialbasis(0, d, curr, m.monomials);
    }
  }
  
  m.size = m.monomials.size();
  return(m);
}


/*
 *	Just for debuging
 */
void mvpolyT::dbg () {
  if (terms.empty()) {
    std::cout<<0<<'\n';
    return;
  }

  bool first_term = true;
  std::map<exps, coeff>::iterator it;
  for (it = terms.begin(); it != terms.end(); it++) {
    if (!first_term) {
      if(it->second > 0)
	std::cout<<" + ";
      else
	std::cout<<" - ";
    }
    else {
      /*
       * first_term
       */
      if(it->second < 0)
	std::cout<<" - ";
    }
    first_term = false;
    
    std::cout<<std::abs(it->second)<<" ";
    
    for (int i = 0; i<it->first.size(); i++) {
      if(it->first[i] == 0)
	continue;
      else
	std::cout<<" x_"<<i+1<<"^"<<it->first[i]<<" ";
    }
  }
  std::cout<<'\n';
}

void monomialbasisT::dbg() const {
  std::cout<<"|m| = "<<size<<'\n';
  std::cout<<"d = "<<degree<<'\n'; 
  std::cout<<(homogeneous ? "This is a base for a Form" : 
	      "This is a base for a Polynomial")<<'\n';

  for (int i = 0; i<size; i++) {
    std::cout<<"m["<<i<<"] = [";
    for (int k = 0; k<monomials[i].size(); k++) {
      std::cout<<monomials[i][k];
      if (k < monomials[i].size() - 1)
	std::cout << ", ";
    }
    std::cout << "]";

    std::cout << " (";
    bool first = true;
    for (int k = 0; k < monomials[i].size(); k++) {
      if (monomials[i][k] > 0) {
	if(!first)
	  std::cout << " ";
	
	std::cout << "x_" << (k+1);

	if(monomials[i][k] !=1)
	  std::cout << "^" << monomials[i][k];

	first = false;
      } 
    }
    if (first)
      std::cout<<"1";
    std::cout<<")"<<'\n';
  }
}
