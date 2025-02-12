/*********************************************************************
*
* (c) Copyright 2013 - 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxKalmanFilter.h,v $
* ID: $Id: SPxKalmanFilter.h,v 1.4 2014/12/11 14:41:07 rew Exp $
*
* Purpose:
*   Generic n-dimensional Kalman Filter.
*
* Revision Control:
*   11/12/14 v1.4    AGC	Initialise covariance to 1.
*
* Previous Changes:
*   18/11/13 1.3    AGC	Fix clang warning.
*   18/11/13 1.2    AGC	Support varying process model.
*   08/11/13 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_KALMAN_FILTER_H
#define _SPX_KALMAN_FILTER_H

/* Need SPxMatrix. */
#include "SPxLibUtils/SPxMatrix.h"

/* Define our class. */
template<class T, std::size_t O>
class SPxKalmanFilter
{
public:
    typedef SPxMatrix<T, O> Matrix;
    typedef SPxVector<T, O> Vector;

    explicit SPxKalmanFilter(void)
	: m_optimalGain(), m_state(), m_covariance() {}
    ~SPxKalmanFilter(void) {}

    void Initialise(const Vector &initState)
    {
	m_optimalGain = Matrix();
	m_state = initState;
	m_covariance = Matrix(static_cast<T>(1.0));
    }

    Vector Update(const Matrix &obsModel,
		  const Matrix &obsNoise,
		  const Vector &measurement)
    {
	Vector residual = measurement - obsModel * m_state;
	Matrix residualCovariance = obsModel * m_covariance * obsModel.t() + obsNoise;
	m_optimalGain = m_covariance * obsModel.t() * residualCovariance.inverse();
	m_state = m_state + m_optimalGain * residual;
	m_covariance = (Matrix::identity() - m_optimalGain * obsModel) * m_covariance;
	return m_state;
    }
    Vector Predict(const Matrix &procModel, const Matrix &procNoise)
    {
	m_state = procModel * m_state;
	m_covariance = procModel * m_covariance * procModel.t() + procNoise;
	return m_state;
    }

    Matrix GetGain(void) const { return m_optimalGain; }
    Vector GetState(void) const { return m_state; }
    Matrix GetCovariance(void) const { return m_covariance; }

private:
    /* Private variables. */
    Matrix m_optimalGain;	/* Current gain. */
    Vector m_state;		/* Current state vector. */
    Matrix m_covariance;	/* Current covariance matrix. */
};

#endif /* _SPX_KALMAN_FILTER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
