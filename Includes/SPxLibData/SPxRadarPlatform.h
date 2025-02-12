/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for SPxRadarPlatform class, a base class for radar
*   platforms that can rotate.
*
* Revision Control:
*   26/10/18 v1.2    AGC	Support debug.
*
* Previous Changes:
*   17/10/18 1.1    AGC	Initial version.
**********************************************************************/

#ifndef _SPX_RADAR_PLATFORM_H
#define _SPX_RADAR_PLATFORM_H

/* Standard headers. */

/* For SPxObj base class. */
#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */

class SPxRadarPlatform : public SPxObj
{
public:
    /* Construction and destruction. */
    SPxRadarPlatform(void);
    virtual ~SPxRadarPlatform(void);

    virtual SPxErrorCode SetBearing(double bearingDegs);
    virtual SPxErrorCode GetBearing(double *bearingDegs) const;

    void SetVerbosity(UINT8 verbosity);
    UINT8 GetVerbosity(void) const;
    void SetLogFile(FILE *logFile);

    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:
    void
#if defined(__GNUC__) || defined(__clang__)
	__attribute__((format(printf, 3, 4)))
#endif
	writeDebug(UINT8 verbosity, const char *format, ...) const;

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxRadarPlatform */

#endif /* _SPX_RADAR_PLATFORM_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
