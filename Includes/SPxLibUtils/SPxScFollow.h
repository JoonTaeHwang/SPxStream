/*********************************************************************
*
* (c) Copyright 2007 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Generic (Win + X11) header for class to track window geometries.
*
* Revision Control:
*   25/06/18 v1.6    AGC	Correctly suppress cppcheck warning.
*
* Previous Changes:
*   04/10/13 1.5    AGC	Simplify headers.
*   11/08/10 1.4    REW	Increase SPX_FOLLOW_WIN_MAX to 12 from 5.
*			Make destructor and AddSC() both virtual.
*			Add RemoveSC().
*   28/07/10 1.3    SP 	Protect against multiple inclusion.
*   23/01/09 1.2    DGJ	Add critical section object
*   27/08/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_SC_FOLLOW_H
#define _SPX_SC_FOLLOW_H

#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */
class SPxSc;

// The maximum number of scan-conversion windows that one MS window can direct.
#define SPX_FOLLOW_WIN_MAX 12

class SPxScFollow :public SPxObj
{
    SPxSc *m_sc[SPX_FOLLOW_WIN_MAX];	// The scan-conversion object that we are controlling.
protected:
    int m_numSCwindowsBeingManaged;	// The number of SC windows being managed.
protected:
    int m_winX, m_winY, m_winW, m_winH;	// The window geometry;

public:
    SPxScFollow(SPxSc *sc);
    virtual ~SPxScFollow(void);
    SPxCriticalSection m_criticalSection;	/* For single thread access */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode AddSC(SPxSc *sc);
    virtual SPxErrorCode RemoveSC(SPxSc *sc);
    int GetSCcount(void) {return m_numSCwindowsBeingManaged;}
    SPxSc *GetSC(int i) {if (i >= 0 && i < SPX_FOLLOW_WIN_MAX) return m_sc[i]; else return NULL;}
    int CurrentX(void) {return m_winX;}
    int CurrentY(void) {return m_winY;}
    int CurrentW(void) {return m_winW;}
    int CurrentH(void) {return m_winH;}
    // Alternate (and preferred) forms of above.
    int GetX(void) {return m_winX;}
    int GetY(void) {return m_winY;}
    int GetW(void) {return m_winW;}
    int GetH(void) {return m_winH;}

    void SetGeometry(int x, int y, int w, int h)
    {
	m_winX = x, m_winY = y; m_winW = w; m_winH = h;
    }
};

#endif /* _SPX_SC_FOLLOW_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
