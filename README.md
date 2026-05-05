# Elements of multivariate polynomials

## Sum-of-squares decomposition

With effect, all sum-of-squares (of multivariate polynomials) is everywhere nonnegative:

$$\sum_{i=1}^k q_i^2(x)_{j=1}^n = p(x)_{j=1}^n \Rightarrow \forall (x)_{j=1}^n \in \mathbb{R}^n : p(x)_{j=1}^n \ge 0 . $$

However, it is not a biunivocal correspondence, for instance the classical counterexample is the _Motzkin Polynomial_:

$$M(x,y) = x^4y^2+x^2y^4 + 1 - 3x^2y^2.$$

The General problem of polynomial nonnegativity is NP-hard. Yet, sum-of-squares decomposition is a reasonably close approximation to the nonnegative characterization. Also, the bijection holds, easily follows from _Cholesky decompositon_:

$$\text{Let } m = (1 , x_1, x_2, ..., x_1^d, x_1^{d-1} x_2, ..., x_n^d) \text{ where } d \text{ is the polynomial degree}.$$

$$\text{Let } N = \binom{n+d}{d} $$

$$\sum_{i=1}^k q_i^2(x)_{j=1}^n = p(x)_{j=1}^n  \Leftrightarrow \exists G \in \mathbb{R}^{N \times N} : G = G^t \land G \succeq 0 \land p(x)_{j=1}^n = m^t G m .$$

The constraints imposed by $p(x)_{j=1}^n = m^t G m$ generates linear constraints, plus the $G \succeq 0$ implies a semidefinite feasibility problem.

---
