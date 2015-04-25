#pragma once

#include "expressions.h"

namespace codi {
  template<typename Real, typename Tape>
  class ActiveReal : public Expression<Real, ActiveReal<Real, Tape> > {
  public:
    typedef Real RealType;
    typedef Tape TapeType;
    typedef typename Tape::GradientData GradientData;

    static Tape globalTape;

  private:
    Real value;
    GradientData gradientData;

  public:

    inline ActiveReal() : value(0.0) {
      globalTape.initGradientData(value, gradientData);
    }

    inline ActiveReal(const Real& value) : value(value) {
      globalTape.initGradientData(this->value, gradientData);
    }

    inline ActiveReal(const Real& value, const Real& gradient) : value(value) {
      globalTape.initGradientData(this->value, gradientData);
      globalTape.setGradient(gradientData, gradient);
    }

    template<class R>
    inline ActiveReal(const Expression<Real, R>& rhs) {
      globalTape.store(value, gradientData, rhs.cast());
    }

    inline ActiveReal(const ActiveReal<Real, Tape>& v) {
      globalTape.store(value, gradientData, v);
    }

    inline ~ActiveReal() {
      globalTape.destroyGradientData(value, gradientData);
    }

    void calcGradient(Real& gradient) const {
      globalTape.pushJacobi(gradient, value, gradientData);
    }

    void calcGradient(Real& gradient, const Real& multiplier) const {
      globalTape.pushJacobi(gradient, multiplier, value, gradientData);
    }

    GradientData& getGradientData() {
      return gradientData;
    }

    const GradientData& getGradientData() const {
      return gradientData;
    }

    Real getGradient() const {
      return globalTape.getGradient(gradientData);
    }

    void setGradient(const Real& gradient) {
      globalTape.setGradient(gradientData, gradient);
    }

    Real getValue() const {
      return value;
    }

    void setValue(const Real& value) {
      this->value = value;
    }

    inline ActiveReal<Real, Tape>& operator=(const Real& rhs){
      globalTape.store(value, gradientData, rhs);
      return *this;
    }

    template<class R>
    inline ActiveReal<Real, Tape>& operator=(const Expression<Real, R>& rhs){
      globalTape.store(value, gradientData, rhs.cast());
      return *this;
    }

    inline ActiveReal<Real, Tape>& operator=(const ActiveReal<Real, Tape>& rhs) {
      globalTape.store(value, gradientData, rhs);
      return *this;
    }

    template<class R>
    inline ActiveReal<Real, Tape>& operator+=(const Expression<Real, R>& rhs) {
      return *this = (*this + rhs);
    }
    template<class R>
    inline ActiveReal<Real, Tape>& operator-=(const Expression<Real, R>& rhs) {
      return *this = (*this - rhs);
    }
    template<class R>
    inline ActiveReal<Real, Tape>& operator*=(const Expression<Real, R>& rhs) {
      return *this = (*this * rhs);
    }
    template<class R>
    inline ActiveReal<Real, Tape>& operator/=(const Expression<Real, R>& rhs) {
      return *this = (*this / rhs);
    }

    inline ActiveReal<Real, Tape>& operator+=(const Real& rhs) {
      // Optimization of code: If jacobies would be stored an identity operation is produced on the tape
      value += rhs;
      return *this;
    }
    inline ActiveReal<Real, Tape>& operator-=(const Real& rhs) {
      // Optimization of code: If jacobies would be stored an identity operation is produced on the tape
      value -= rhs;
      return *this;
    }
    inline ActiveReal<Real, Tape>& operator*=(const Real& rhs) {
      return *this = (*this * rhs);
    }
    inline ActiveReal<Real, Tape>& operator/=(const Real& rhs) {
      return *this = (*this / rhs);
    }

    inline ActiveReal<Real, Tape> operator++() {
      return *this = *this + 1.0;
    }
    inline ActiveReal<Real, Tape> operator++(int) {
      ActiveReal<Real, Tape> r(*this);
      *this = *this + 1.0;
      return r;
    }
    inline ActiveReal<Real, Tape> operator--() {
      return *this = *this - 1.0;
    }
    inline ActiveReal<Real, Tape> operator--(int) {
      ActiveReal<Real, Tape> r(*this);
      *this = *this - 1.0;
      return r;
    }
  };

  template<typename Real, typename Tape>
  Tape ActiveReal<Real, Tape>::globalTape;
}
