#ifndef MVPOLYNOMIAL_HPP
#define MVPOLYNOMIAL_HPP

#include <vector>
#include <map>

using exps = std::vector<int>;
using coeff=double;

class monomialbasisT {
  public:
  std::vector<exps> monomials;
  int size;
  bool homogeneous;
  int degree;	// half of max degree of mvpolyT

  void dbg() const;
};

class mvpolyT {
  int nvars;
  std::map<exps, coeff> terms;
  
  int degree() const;
  bool homogeneous() const;
  void rec_monomialbasis(int v, int remain, 
       exps& curr, std::vector<exps>& ans) const;

  public:
  mvpolyT ();
  void addterm(const exps& _e, coeff _c);
  monomialbasisT monomialbasis() const;
  void dbg();
};

#endif // MVPOLYNOMIAL_HPP
