/*********************************************************************
 *
 * (c) Copyright 2013, 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *  Header for alert buttons class under Windows.
 *
 * Revision Control:
 *  15/02/18 v1.3    AGC	Revert last change.
 *				Add protected AddAlert() to retrieve alert.
 *
 * Previous Changes:
 *  02/02/18 1.2    AGC	Support retrieving alert pointer.
 *  04/11/13 1.1    SP 	Initial version.
 *
 **********************************************************************/
#ifndef _SPX_ALERT_RENDERER_WIN_H
#define _SPX_ALERT_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* For base class. */
#include "SPxLibUtils/SPxAlertRenderer.h"
#include "SPxLibWin/SPxRendererWin.h"
#include "SPxResource.h"

/*
 * Types.
 */

/* Define our class. */
class SPxAlertRendererWin : public SPxAlertRenderer, public SPxRendererWin
{
public:
    /* Public functions */
    SPxAlertRendererWin(void);
    virtual ~SPxAlertRendererWin(void);

    /* Add alerts. */
    SPxErrorCode AddAlert(const char *text,
                          const char *prefix=NULL,
                          UINT32 prefixColour=0,
                          int bitmap=IDB_SPX_WARNING);

protected:
    /* Add alerts. */
    SPxErrorCode AddAlert(const char *text,
                          const char *prefix,
                          UINT32 prefixColour,
                          int bitmap,
                          SPxAlert_t **alertPtr);

private:

}; /* SPxAlertRendererWin */

#endif /* _SPX_ALERT_RENDERER_WIN_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
