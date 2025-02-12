/*********************************************************************
 *
 * (c) Copyright 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *  Header for alert buttons class under Direct2D.
 *
 * Revision Control:
 *  26/07/19 v1.1    AGC	Initial version.
 *
 * Previous Changes:
 *
 **********************************************************************/
#ifndef _SPX_ALERT_RENDERER_D2D_H
#define _SPX_ALERT_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* For base class. */
#include "SPxLibUtils/SPxAlertRenderer.h"
#include "SPxLibWin/SPxRendererD2D.h"
#include "SPxResource.h"

/*
 * Types.
 */

/* Define our class. */
class SPxAlertRendererD2D: public SPxAlertRenderer, public SPxRendererD2D
{
public:
    /* Public functions */
    SPxAlertRendererD2D(void);
    virtual ~SPxAlertRendererD2D(void);

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

}; /* SPxAlertRendererD2D */

#endif /* _SPX_ALERT_RENDERER_D2D_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
