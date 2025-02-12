/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxRemoteP437Ctrl class, which allows client
*	applications to remoteley control P437 radars via SPxServer.
*
*
* Revision Control:
*   24/11/20 v1.3   BTB 	Support enumerated range scale.
*
* Previous Changes:
*   19/11/20 1.2   BTB 	Add to/enhance generic functionality.
*   16/11/20 1.1   BTB 	Initial Version.
*
**********************************************************************/

#ifndef _SPX_REMOTE_P437_CTRL_H
#define _SPX_REMOTE_P437_CTRL_H

/* For SPxRemoteRadarCtrl. */
#include "SPxLibNet/SPxRemoteRadarCtrl.h"

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
 * Define our class.
 */
class SPxRemoteP437Ctrl : public SPxRemoteRadarCtrl
{
public:

    /*
     * Public functions.
     */

    /* Constructors, destructor etc. */
    SPxRemoteP437Ctrl(void);
    SPxRemoteP437Ctrl(const char *addr, unsigned int port);
    explicit SPxRemoteP437Ctrl(SPxRemoteServer *srvConnection);
    virtual ~SPxRemoteP437Ctrl(void){}

    /* Generic control functions from the base class. */
    virtual SPxErrorCode GetIdentifierString(char *idBuf, unsigned int bufLen);
    virtual SPxErrorCode GetDescriptionString(char *descBuf, unsigned int bufLen);

    virtual SPxErrorCode SetTransmitting(unsigned int enable);
    virtual SPxErrorCode GetTransmitting(unsigned int *enableRtn);

    virtual SPxErrorCode SetRangeScaleValue(unsigned int enumValue);
    virtual SPxErrorCode GetRangeScaleValue(unsigned int *enumValueRtn);
    SPxErrorCode GetRangeMetres(double *rangeMetresRtn);

    virtual unsigned int GetNumRangeScales(void);
    virtual SPxErrorCode GetRangeScaleInfoAtIndex(unsigned int index, char *infoBuf, unsigned int bufLen);

    virtual SPxErrorCode SetGainLevelPercent(double level);
    virtual SPxErrorCode GetGainLevelPercent(double *levelRtn);
    virtual SPxErrorCode SetGainAuto(unsigned int enable);
    virtual SPxErrorCode GetGainAuto(unsigned int *enableRtn);

    virtual SPxErrorCode SetSeaClutterLevelPercent(double level);
    virtual SPxErrorCode GetSeaClutterLevelPercent(double *levelRtn);

    virtual unsigned int GetNumSeaClutterModes(void) { return 2; }
    virtual int IsSeaClutterManual(void);

    virtual SPxErrorCode SetSeaClutterModeGeneric(unsigned int mode);
    virtual SPxErrorCode GetSeaClutterModeGeneric(unsigned int *modeRtn);
    virtual SPxErrorCode GetSeaClutterModeString(unsigned int mode, char *modeBuf, unsigned int bufLen);

    virtual SPxErrorCode SetRainLevelPercent(double level);
    virtual SPxErrorCode GetRainLevelPercent(double *levelRtn);

};

#endif /* _SPX_REMOTE_P437_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
