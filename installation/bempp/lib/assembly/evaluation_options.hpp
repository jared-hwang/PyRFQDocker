// Copyright (C) 2011-2012 by the BEM++ Authors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef bempp_evaluation_options_hpp
#define bempp_evaluation_options_hpp

#include "../common/common.hpp"
#include "../common/types.hpp"

#include "../common/deprecated.hpp"
#include "../fiber/opencl_options.hpp"
#include "../fiber/parallelization_options.hpp"
#include "../fiber/verbosity_level.hpp"

namespace Bempp {

using Fiber::OpenClOptions;
using Fiber::ParallelizationOptions;
using Fiber::VerbosityLevel;

/** \ingroup potential_operators
 *  \brief Options controlling evaluation of potentials.
 */
class EvaluationOptions {
public:
  enum { AUTO = -1 };

  /** \brief Constructor. */
  EvaluationOptions();

  /** \brief Constructor. */
  EvaluationOptions(const ParameterList &parameters);

  /** @name Evaluation mode
    @{ */

  /** \brief Possible evaluation modes. */
  enum Mode {
    /** \brief Assemble dense matrices. */
    DENSE,
    /** \brief Assemble hierarchical matrices using HMat. */
    HMAT
  };

  /** \brief Use dense-matrix representations of elementary potential operators.
   *
   *  This is the default evaluation mode. If it is active, potentials
   *  are evaluated in the following way.
   *
   *  (1) When the potential needs to be evaluated only for a single charge
   *  distribution \f$\psi(y)\f$, as is the case in the
   *  PotentialOperator::evaluateAtPoints() and evaluateOnGrid() functions,
   *  the defining formula of the potential
   *
   *  \f[ k(x) = \int_\Gamma F[x, \psi(y)] \, \mathrm{d}\Gamma, \f]
   *
   *  is approximated with a numerical quadrature rule
   *
   *  \f[ k(x) \approx \sum_{j=1}^N w_j F[x, \psi(y_j)], \f]
   *
   *  where \f$y_j \in \Gamma\f$ and \f$w_j\f$ are the quadrature points and
   *  weights. The values of \f$\psi(y)\f$ at the quadrature points \f$y_j\f$
   *  are precalculated and stored, and then the potential is sequentially
   *  evaluated at each requested point \f$x_i\f$ using the formula above.
   *  The values of any kernels involved in \f$F\f$ are evaluated once for
   *  each pair \f$(x_i, y_j)\f$ and then discarded.
   *
   *  (2) If the potentials due to multiple charge distributions need to be
   *  evaluated at a fixed set of points, the PotentialOperator::assemble()
   *  function can be used to generate an AssembledPotentialOperator object
   *  storing a (dense) matrix representation of the potential operator, with
   *  the element (c * i, j) containing the value of cth component of the
   *  potential produced at ith point by the charge distribution equal to jth
   *  basis function of a certain function space. The potential generated by
   *  a specific charge distribution expanded in this space is then
   *  calculated by left-multiplying the vector of its expansion coefficients
   *  by the matrix representation of the potential operator.
   */
  void switchToDenseMode();

  /** \brief Return current evaluation mode.
   *
   *  The evaluation mode can be changed by calling switchToDenseMode() or
   *  switchToAcaMode(). */
  Mode evaluationMode() const;

  /** @}
    @name Parallelization
    @{ */

  // Temporarily removed (OpenCl support is broken).
  // void enableOpenCl(const OpenClOptions& openClOptions);
  // void disableOpenCl();

  /** \brief Set the maximum number of threads used during evaluation of
   *potentials.
   *
   *  \p maxThreadCount must be a positive number or \p AUTO. In the latter
   *  case the number of threads is determined automatically. */
  void setMaxThreadCount(int maxThreadCount);

  /** \brief Set the maximum number of threads used during evaluation of
   *potentials.
   *
   *  \deprecated Use setMaxThreadCount() instead. */
  BEMPP_DEPRECATED void switchToTbb(int maxThreadCount = AUTO);

  /** \brief Return current parallelization options. */
  const ParallelizationOptions &parallelizationOptions() const;

  /** @}
    @name Verbosity
    */

  /** \brief Set the verbosity level.
   *
   *  This setting determines the amount of information printed out by
   *  functions from BEM++. */
  void setVerbosityLevel(VerbosityLevel::Level level);

  /** \brief Return the verbosity level. */
  VerbosityLevel::Level verbosityLevel() const;

  /** @} */

private:
  /** \cond */
  Mode m_evaluationMode;
  ParallelizationOptions m_parallelizationOptions;
  VerbosityLevel::Level m_verbosityLevel;
  ParameterList m_parameterList;
  /** \endcond */
};

} // namespace Bempp

#endif