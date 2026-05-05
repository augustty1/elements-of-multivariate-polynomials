#ifndef MVPOLYNOMIAL_HPP
#define MVPOLYNOMIAL_HPP

#include <cstddef>
#include <vector>
#include <map>

using exps = std::vector<unsigned int>;
using coeff=double;

struct constraintT {
  std::map<unsigned int, coeff> lhs;
  coeff rhs;
};

class monomialbasisT {
  public:
  std::vector<exps> monomials;
  size_t size;
  bool homogeneous;
  unsigned int degree;	// half of max degree of mvpolyT

  void dbg() const;
};

class mvpolyT {
  unsigned int nvars;
  std::map<exps, coeff> terms;
  monomialbasisT mb;
  bool ismbcomputed;
  
  unsigned int degree() const;
  bool homogeneous() const;
  void rec_monomialbasis(unsigned int v, unsigned int remain, 
       exps& curr, std::vector<exps>& ans) const;

  public:
  mvpolyT ();
  void addterm(const exps& _e, coeff _c);
  const monomialbasisT& monomialbasis();
  std::vector<constraintT> gram();
  void dbg();
};

#endif // MVPOLYNOMIAL_HPP
