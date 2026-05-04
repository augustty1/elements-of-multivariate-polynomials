# Elements of multivariate polynomials
---

# Sum-of-squares decomposition

With effect, all sum-of-squares (of multivariate polynomials) is everywhere nonnegative:

$$\sum_{i=1}^k q_i^2(x)_{j=1}^n = p(x)_{j=1}^n \Rightarrow \forall (x)_{j=1}^n \in \mathbb{R}^n : p(x)_{j=1}^n \ge 0 . $$

However, it is not a biunivocal correspondence, for instance the classical counterexample is the _Motzkin Polynomial_:

$$M(x,y) = x^4y^2+x^2y^4 + 1 - 3x^2y^2.$$

The General problem of polynomial nonnegativity is NP-hard. Yet, sum-of-squares decomposition is a reasonably close approximation to the nonnegative characterization. Also, the bijection holds, easily follows from _Cholesky decompositon_:

$$\text{Let } m = (1 , x_1, x_2, ..., x_1^d, x_1^{d-1} x_2, ..., x_n^d) \text{ where } d \text{ is the polynomial degree}.$$

$$\text{Let } N = \binom{n+d}{d} $$

$$\sum_{i=1}^k q_i^2(x)_{j=1}^n = p(x)_{j=1}^n  \Leftrightarrow \exists G \in \mathbb{R}^{N \times N} : G = G^t \land G \succeq 0 \land p(x)_{j=1}^n = m^t G m .$$

And an important remark is if the polynomial is homogeneous (a form) of degre $2d$, then it is sufficient to restrict the components of $m$ to the monomials of degree exactly equal to $d$. 

$m$ can be generetade with recursion and backtracking. The algorithm bellow computes the monomial basis:

```cpp
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
```

