/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxENCManagerWin.h,v $
* ID: $Id: SPxENCManagerWin.h,v 1.3 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*   Header for SPxENCManagerWin class which provides a library
*   of charts for display by the SPxENCRendererWin class.
*
* Revision Control:
*   04/10/13 v1.3    AGC	Simplify headers.
*
* Previous Changes:
*   20/02/12 1.2    SP 	Store ENCX objects as opaque pointers.
*                       Do not import encxcom.tlb from header.
*   08/02/12 1.1    SP 	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_ENC_MANAGER_WIN_H
#define _SPX_ENC_MANAGER_WIN_H

/*
 * Other headers required.
 */

/* We need the base class. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*
 * Define our class, derived from standard base class.
 */
class SPxENCManagerWin : public SPxObj
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxENCManagerWin(void);
    virtual ~SPxENCManagerWin(void);

    /* Open and close library. */
    SPxErrorCode OpenLibraryS57(const char *path=NULL);
    SPxErrorCode CloseLibrary(void);

    /* Get ENCX S57 manager. */
    IUnknown *GetS57Mgr(void) { return m_encxS57Mgr; }
    
protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

    /* The following ENCX objects are stored as opaque pointers
     * to avoid placing a dependency on encxcom.tlb the library.
     */
    IUnknown *m_encxLib;    /* ENCX chart library. */
    IUnknown *m_encxS57Mgr; /* ENCX chart manager. */

    /*
     * Private functions.
     */

}; /* SPxENCManagerWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_ENC_MANAGER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
