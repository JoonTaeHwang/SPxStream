/*********************************************************************
*
* (c) Copyright 2013 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Class to manage a COM object.
*
*
* Revision Control:
*   05/06/19 v1.8    AGC	Avoid new clang Windows warnings.
*
* Previous Changes:
*   24/06/16 1.7    AGC	Fix SPxComPtr assignment to self.
*   05/02/15 1.6    AGC	Replace ULONG with unsigned long.
*   11/12/14 1.5    AGC	Improve formatting.
*   24/09/14 1.4    AGC	Fix typo in operator<.
*   15/01/14 1.3    AGC	Add operator= with rhs of SPxComPtr reference.
*   27/09/13 1.2    AGC	Add operator= for SPxComPtr.
*   17/09/13 1.1    AGC	Initial version.
*
**********************************************************************/
#ifndef SPX_COM_PTR_H
#define SPX_COM_PTR_H

#ifndef SPX_DECLSPEC_NOTHROW
#if (_MSC_VER >= 1200)
#define SPX_DECLSPEC_NOTHROW   __declspec(nothrow)
#else
#define SPX_DECLSPEC_NOTHROW
#endif
#endif

template <typename T>
class SPxAdaptPtr
{
public:
    SPxAdaptPtr(void) {}
    SPxAdaptPtr(const T& p) : m_p(p) {}
    SPxAdaptPtr(const SPxAdaptPtr& ap) : m_p(ap.m_p) {}
    SPxAdaptPtr& operator=(const T& p)
    {
	m_p = p;
	return *this;
    }
    SPxAdaptPtr& operator=(const SPxAdaptPtr<T>& ap)
    {
	if(this != &ap)
	{
	    m_p = ap.m_p;
	}
	return *this;
    }
    bool operator<(const T& p) const
    {
	return m_p < p;
    }
    bool operator==(const T& p) const
    {
	return m_p == p;
    }
    operator T&()
    {
	return m_p;
    }
    operator const T&() const
    {
	return m_p;
    }
    T& operator->()
    {
	return m_p;
    }
    const T& operator->() const
    {
	return m_p;
    }

    T m_p;
};

template <typename T>
class _SPxNoRefComPtr : public T
{
private:
    virtual SPX_DECLSPEC_NOTHROW unsigned long __stdcall AddRef()=0;
    virtual SPX_DECLSPEC_NOTHROW unsigned long __stdcall Release()=0;
};

template<typename T>
class SPxComPtr
{
public:
    SPxComPtr() : m_p(NULL) {}
    SPxComPtr(T* p) : m_p(p)
    {
	if(m_p)
	{
	    m_p->AddRef();
	}
    }
    SPxComPtr(const SPxComPtr<T>& cp) : m_p(cp.m_p)
    {
	if(m_p)
	{
	    m_p->AddRef();
	}
    }
    ~SPxComPtr()
    {
	if(m_p)
	{
	    m_p->Release();
	}
    }
    SPxComPtr<T>& operator=(const SPxComPtr<T>& rhs)
    {
	if(rhs != *this)
	{
	    if(m_p)
	    {
		m_p->Release();
	    }
	    m_p = rhs.m_p;
	    if(m_p)
	    {
		m_p->AddRef();
	    }
	}
	return *this;
    }
    SPxComPtr<T>& operator=(T* rhs)
    {
	if(rhs != *this)
	{
	    if(m_p)
	    {
		m_p->Release();
	    }
	    m_p = rhs;
	    if(m_p)
	    {
		m_p->AddRef();
	    }
	}
	return *this;
    }
    operator T*() const
    {
	return m_p;
    }
    T& operator*() const
    {
	return *m_p;
    }
    T** operator&()
    {
	return &m_p;
    }
    _SPxNoRefComPtr<T>* operator->() const
    {
	return (_SPxNoRefComPtr<T>*)m_p;
    }
    bool operator!() const 
    {
	return (m_p == NULL);
    }
    bool operator<(T* pT) const
    {
	return m_p < pT;
    }
    bool operator!=(T* pT) const
    {
	return !operator==(pT);
    }
    bool operator==(T* pT) const
    {
	return m_p == pT;
    }

    void Release()
    {
	T* tmp = m_p;
	if(tmp)
	{
	    m_p = NULL;
	    tmp->Release();
	}
    }
    void Attach(T* p2)
    {
	if(m_p)
	{
	    m_p->Release();
	}
	m_p = p2;
    }
    T* Detach()
    {
	T* pt = m_p;
	m_p = NULL;
	return pt;
    }

private:
    T* m_p;
};

#endif /* SPX_COM_PTR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
