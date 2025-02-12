/*********************************************************************
*
* (c) Copyright 2013 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxVariance.h,v $
* ID: $Id: SPxVariance.h,v 1.7 2017/02/21 15:57:00 rew Exp $
*
* Purpose:
*   Online variance calculator.
*
* Revision Control:
*   21/02/17 v1.7    SP 	Fix error in variance calculation.
*
* Previous Changes:
*   07/10/16 1.6    AGC	Add zero template specialisation for running variance.
*   08/07/16 1.5    AGC	Add GetSize() function.
*   26/09/14 1.4    AGC	Fix cppcheck warning.
*   24/09/14 1.3    AGC	Change to limited range.
*   14/04/14 1.2    AGC	Add GetNum() function.
*   22/11/13 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_VARIANCE_H
#define _SPX_VARIANCE_H

template<unsigned int N>
class SPxVariance
{
public:
    SPxVariance() : m_num(0), m_sum(0), m_mean(0.0), m_variance(0.0)
    {
	/* This initialisation silences the cppcheck warning.
	 * The values are not used until after being set when m_num
	 * is also increased.
	 */
	m_values[0] = 0.0;
    }
    ~SPxVariance() {}

    void Reset()
    {
	m_num = 0;
	m_sum = 0;
	m_mean = 0.0;
	m_variance = 0.0;
        m_values[0] = 0.0;
    }

    double GetMean() const { return m_mean; }
    double GetVariance() const { return m_variance; }
    unsigned int GetNum() const { return m_num; }
    static unsigned int GetSize() { return N; }

    double Update(double val)
    {
	++m_num;
	unsigned int index = (m_num - 1) % N;
	if( m_num > N )
	{
	    m_sum -= m_values[index];
	}
	m_values[index] = val;
	m_sum += val;
	unsigned int numInUse = (m_num < N) ? m_num : N;
	m_mean = m_sum / numInUse;
	double sum2 = 0.0;
	for(unsigned int i = 0; i < numInUse; ++i)
	{
	    sum2 += (m_values[i] - m_mean) * (m_values[i] - m_mean);
	}
	m_variance = sum2 / numInUse;
	return m_variance;
    }

private:
    double m_values[N];
    unsigned int m_num;
    double m_sum;
    double m_mean;
    double m_variance;
};

/* Specialisation for zero indicates running variance. */
template<>
class SPxVariance<0>
{
public:
    SPxVariance() : m_num(0), m_k(0.0), m_ex(0.0), m_ex2(0.0), m_mean(0.0), m_variance(0.0) {}
    ~SPxVariance() {}

    void Reset()
    {
	m_num = 0;
	m_k = 0.0;
	m_ex = 0.0;
	m_ex2 = 0.0;
	m_mean = 0.0;
	m_variance = 0.0;
    }

    double GetMean() const { return m_mean; }
    double GetVariance() const { return m_variance; }
    unsigned int GetNum() const { return m_num; }
    static unsigned int GetSize() { return 0; }

    double Update(double val)
    {
	if(m_num == 0)
	{
	    m_k = val;
	}
	++m_num;
	m_ex += val - m_k;
	m_ex2 += (val - m_k) * (val - m_k);
	m_mean = m_k + m_ex / m_num;
	if(m_num > 1)
	{
	    m_variance = (m_ex2 - (m_ex * m_ex / m_num)) / (m_num - 1);
	}
	return m_variance;
    }

private:
    unsigned int m_num;
    double m_k;
    double m_ex;
    double m_ex2;
    double m_mean;
    double m_variance;
};

#endif /* _SPX_VARIANCE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
