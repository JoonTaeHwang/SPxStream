/*********************************************************************
*
* (c) Copyright 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxMFInit.h,v $
* ID: $Id: SPxMFInit.h,v 1.1 2017/04/12 09:22:18 rew Exp $
*
* Purpose:
*   Class to manage Media Foundation initialisation.
*
*
* Revision Control:
*   12/04/17 v1.1    AGC	Initial version.
*
* Previous Changes:*
**********************************************************************/
#ifndef SPX_MF_INIT_H_
#define SPX_MF_INIT_H_

#include "Mfapi.h"

#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxError.h"

class SPxMFInit
{
public:
    SPxMFInit() : m_hr(E_NOTIMPL) {}
    SPxMFInit(ULONG version) : m_hr(E_NOTIMPL)
    {
	Initialise(version);
    }
    ~SPxMFInit()
    {
	Uninitialise();
    }
    void Initialise(ULONG version)
    {
	if(!IsInitialised())
	{
	    m_hr = MFStartup(version);
	}
    }
    void Uninitialise()
    {
	if(IsInitialised())
	{
	    MFShutdown();
	    m_hr = E_NOTIMPL;
	}
    }
    int IsInitialised() const
    {
	return SUCCEEDED(m_hr);
    }
    HRESULT GetHRESULT() const { return m_hr; }

private:
    HRESULT m_hr;
};

#endif /* SPX_MF_INIT_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
