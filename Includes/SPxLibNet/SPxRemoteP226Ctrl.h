/*********************************************************************
*
* (c) Copyright 2018 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxRemoteP226Ctrl class, which allows client
*	applications to remoteley control P226 radars via SPxServer.
*
*
* Revision Control:
*   21/07/22 v1.9   AJH 	Query whether interference modes supported.
*
* Previous Changes:
*   19/07/22 1.8   AJH 	Query whether sector blanking supported.
*   13/07/22 1.7   BTB 	Add new radars.
*   02/09/21 1.6   AJH 	Add new scan mode.
*   19/11/20 1.5   BTB 	Add to/enhance generic functionality.
*   16/11/20 1.4   BTB 	Add generic control functions.
*   23/05/18 1.3   BTB 	Support getting of radar type.
*   17/05/18 1.2   BTB 	Build fix.
*   15/05/18 1.1   BTB 	Initial Version.
*
**********************************************************************/

#ifndef _SPX_REMOTE_P226_CTRL_H
#define _SPX_REMOTE_P226_CTRL_H

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

enum SPxP226ScanRateMode_t
{
    SPX_P226_SCANMODE_NORMAL = 0,
    SPX_P226_SCANMODE_MEDIUM = 1,
    SPX_P226_SCANMODE_HIGH = 2,
    SPX_P226_SCANMODE_VERY_HIGH = 3,
    SPX_P226_SCANMODE_UNKNOWN = 4

};

enum SPxP226SeaClutterAutoMode_t
{
    SPX_P226_SEACLUTTERMODE_MANUAL = 0,
    SPX_P226_SEACLUTTERMODE_HARBOUR = 1,
    SPX_P226_SEACLUTTERMODE_OFFSHORE = 2

};

enum SPxP226SupportedRadars_t
{
    SPX_P226_RADAR_3G = 12,
    SPX_P226_RADAR_4G = 13,
    SPX_P226_RADAR_HALO = 14,
    SPX_P226_RADAR_R5000 = 15,
    SPX_P226_RADAR_HALO24 = 16,
    SPX_P226_RADAR_HALO20E = 17,
    SPX_P226_RADAR_HALO20P = 18

};

/*
 * Define our class.
 */
class SPxRemoteP226Ctrl : public SPxRemoteRadarCtrl
{
public:

    /*
     * Public functions.
     */

    /* Constructors, destructor etc. */
    SPxRemoteP226Ctrl(void);
    SPxRemoteP226Ctrl(const char *addr, unsigned int port);
    explicit SPxRemoteP226Ctrl(SPxRemoteServer *srvConnection);
    virtual ~SPxRemoteP226Ctrl(void){}

    /* Generic control functions from the base class. */
    virtual SPxErrorCode GetIdentifierString(char *idBuf, unsigned int bufLen);
    virtual SPxErrorCode GetDescriptionString(char *descBuf, unsigned int bufLen);

    virtual SPxErrorCode SetTransmitting(unsigned int enable);
    virtual SPxErrorCode GetTransmitting(unsigned int *enableRtn);

    virtual SPxErrorCode SetRangePercent(double range);
    virtual SPxErrorCode GetRangePercent(double *rangeRtn);
    SPxErrorCode GetRangeMetres(double *rangeMetresRtn);

    virtual SPxErrorCode SetGainLevelPercent(double level);
    virtual SPxErrorCode GetGainLevelPercent(double *levelRtn);
    virtual SPxErrorCode SetGainAuto(unsigned int enable);
    virtual SPxErrorCode GetGainAuto(unsigned int *enableRtn);

    virtual SPxErrorCode SetSeaClutterLevelPercent(double level);
    virtual SPxErrorCode GetSeaClutterLevelPercent(double *levelRtn);

    virtual unsigned int GetNumSeaClutterModes(void) { return 3; }
    virtual int IsSeaClutterManual(void);

    virtual SPxErrorCode SetSeaClutterModeGeneric(unsigned int mode);
    virtual SPxErrorCode GetSeaClutterModeGeneric(unsigned int *modeRtn);
    virtual SPxErrorCode GetSeaClutterModeString(unsigned int mode, char *modeBuf, unsigned int bufLen);

    virtual SPxErrorCode SetRainLevelPercent(double level);
    virtual SPxErrorCode GetRainLevelPercent(double *levelRtn);

    /* Simrad-specific control functions. */

    /* Functions for getting radar state. */
    SPxErrorCode GetType(unsigned int *type);
    SPxErrorCode GetTypeString(char *typeBuf, int bufLen);
    SPxErrorCode GetState(char *stateBuf, int bufLen);
    SPxErrorCode GetScanRPM(float *rpm);

    /* Specific function to set a range in metres. */
    SPxErrorCode SetRangeMetres(float rangeMetres);

    /* Functions for handling scan rate. */
    SPxErrorCode SetScanRateMode(SPxP226ScanRateMode_t mode);
    SPxErrorCode GetScanRateMode(SPxP226ScanRateMode_t *mode);

    /* Specific functions for handling gain level. */
    SPxErrorCode SetGainLevel(unsigned int level);
    SPxErrorCode GetGainLevel(unsigned int *level);

    /* Specific functions for handling sea clutter. */
    SPxErrorCode SetSeaClutterLevel(unsigned int level);
    SPxErrorCode GetSeaClutterLevel(unsigned int *level);
    SPxErrorCode SetSeaClutterAutoMode(SPxP226SeaClutterAutoMode_t mode);
    SPxErrorCode GetSeaClutterAutoMode(SPxP226SeaClutterAutoMode_t *mode);

    /* Specific functions for handling rain clutter rejection. */
    SPxErrorCode SetRainLevel(unsigned int level);
    SPxErrorCode GetRainLevel(unsigned int *level);

    /* Functions for handling FTC. */
    SPxErrorCode SetFTCLevel(unsigned int level);
    SPxErrorCode GetFTCLevel(unsigned int *level);

    /* Functions for handling interference rejection. */
    SPxErrorCode GetInterferenceRejectSupported(int *supported);
    SPxErrorCode SetInterferenceReject(unsigned int level);
    SPxErrorCode GetInterferenceReject(unsigned int *level);

    /* Functions for handling local  interference rejection. */
    SPxErrorCode GetLocalIRSupported(int *supported);
    SPxErrorCode SetLocalIR(unsigned int level);
    SPxErrorCode GetLocalIR(unsigned int *level);

    /* Functions for handling sector blanking. */
    SPxErrorCode GetSectorBlankingSupported(int *supported);
    SPxErrorCode SetSectorBlanking(unsigned int sector,
				   unsigned int enable);
    SPxErrorCode GetSectorBlanking(unsigned int sector,
				   unsigned int *enabled);
    SPxErrorCode SetSectorBlankingStartAngle(unsigned int sector,
					     float angleDegs);
    SPxErrorCode GetSectorBlankingStartAngle(unsigned int sector,
					     float *angleDegs);
    SPxErrorCode SetSectorBlankingEndAngle(unsigned int sector,
					   float angleDegs);
    SPxErrorCode GetSectorBlankingEndAngle(unsigned int sector,
					   float *angleDegs);

};

#endif /* _SPX_REMOTE_P226_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
