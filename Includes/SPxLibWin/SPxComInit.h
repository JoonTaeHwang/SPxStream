/*********************************************************************
*
* (c) Copyright 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxComInit.h,v $
* ID: $Id: SPxComInit.h,v 1.2 2017/05/09 13:24:45 rew Exp $
*
* Purpose:
*   Class to manage COM initialisation.
*
*
* Revision Control:
*   09/05/17 v1.2    AGC	Fix cppcheck warnings.
*
* Previous Changes:*
*   12/04/17 1.1    AGC	Initial version.
**********************************************************************/
#ifndef SPX_COM_INIT_H_
#define SPX_COM_INIT_H_

#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

class SPxComInit
{
public:
    SPxComInit(void)
	: m_hr(S_OK), m_comOK(FALSE), m_comInit(FALSE)
    {}
    explicit SPxComInit(DWORD dwCoInit)
	: m_hr(S_OK), m_comOK(FALSE), m_comInit(FALSE)
    {
	Initialise(dwCoInit);
    }
    ~SPxComInit()
    {
	Uninitialise();
    }
    void Initialise(DWORD dwCoInit)
    {
	if(!IsInitialised())
	{
	    m_hr = CoInitializeEx(NULL, dwCoInit);
	    if(SUCCEEDED(m_hr))
	    {
		m_comOK = TRUE;
		m_comInit = TRUE;
	    }
	    else if(m_hr == RPC_E_CHANGED_MODE)
	    {
		/* COM is already initialised which is ok. */
		m_comOK = TRUE;
	    }
	    else
	    {
		SPxError(SPX_ERR_SYSTEM, SPX_ERR_SYSCALL, m_hr, 0,
			 SPX_FUNCTION_NAME, "Failed to initialise COM");
	    }
	}
    }
    void Uninitialise(void)
    {
	if(m_comInit)
	{
	    CoUninitialize();
	}

	m_comInit = FALSE;
	m_comOK = FALSE;
    }
    int IsInitialised() const
    {
	return m_comOK;
    }
    HRESULT GetHRESULT() const
    {
	return m_hr;
    }

private:
    HRESULT m_hr;
    int m_comOK;
    int m_comInit;

    /* Disable copy and assignment operators. */
    SPxComInit(const SPxComInit&) SPX_FUNCTION_DELETE;
    SPxComInit& operator=(const SPxComInit&) SPX_FUNCTION_DELETE;
};

#endif /* SPX_COM_INIT_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
