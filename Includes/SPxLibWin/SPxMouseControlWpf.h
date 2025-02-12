/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
*
* Purpose:
*   Header file for SPxMouseControlWpf class.
*
*
* Revision Control:
*   17/10/19 v1.2    BTB	Support installing handler for setting cursor.
*
* Previous Changes:
*   15/10/19 1.1    BTB	Initial Version.
*
**********************************************************************/

#ifndef _SPX_MOUSE_CONTROL_WPF_H
#define _SPX_MOUSE_CONTROL_WPF_H

/*
 * Other headers required.
 */

/* For base class. */
#include "SPxLibUtils/SPxMouseControl.h"

/* Forward declare other classes required. */
class SPxViewControl;

/*********************************************************************
*
*   Class definitions
*
**********************************************************************/

/*
 * Define our class, derived from the SPxMouseControl base object.
 */
class SPxMouseControlWpf : public SPxMouseControl
{
public:
    typedef int (*SetCursorFn)(SPxCursor_t cursor);

     /* Constructor and destructor. */
    explicit SPxMouseControlWpf(SPxViewControl *viewControl=NULL);
    virtual ~SPxMouseControlWpf(void);

    /* Functions for exposing mouse handling on SPxMouseControl. */
    int HandleMousePress(UINT32 flags, int x, int y);
    int HandleMouseRelease(UINT32 flags, int x, int y);
    int HandleDoubleClick(UINT32 flags, int x, int y);
    int HandleMouseMove(UINT32 flags, int x, int y);
    int HandleMouseLeave(void);
    int HandleMouseWheel(UINT32 flags, int x, int y, double delta);
    int HandleModKeyChange(UINT32 flags);

    void InstallSetCursorFn(SetCursorFn setCursorFn);

protected:
    virtual int HandleSetCursor(void);

private:
    virtual SPxPopupMenu *createPopupMenu(void) const { return NULL; }

    SetCursorFn m_setCursorFn;

}; /* SPxMouseControlWpf */

#endif /* _SPX_MOUSE_CONTROL_WPF_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
