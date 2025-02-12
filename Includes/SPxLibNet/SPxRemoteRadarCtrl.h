/*********************************************************************
*
* (c) Copyright 2018 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxRemoteRadarCtrl class, which allows client
*	applications to remoteley control radars via SPxServer.
*
*
* Revision Control:
*   24/11/20 v1.7   BTB 	Support enumerated range scale.
*
* Previous Changes:
*   19/11/20 1.6   BTB 	Add to/enhance generic functionality.
*   16/11/20 1.5   BTB 	Add interface for generic control.
*   30/11/18 1.4   BTB 	Store address in own memory, don't rely on parameter related memory.
*   11/06/18 1.3   BTB 	Use auto-reconnect functionality in SPxRemoteServer.
*   16/05/18 1.2   BTB 	Add GetRadartype() (fixes build).
*   15/05/18 1.1   BTB 	Initial Version.
*
**********************************************************************/

#ifndef _SPX_REMOTE_RADAR_CTRL_H
#define _SPX_REMOTE_RADAR_CTRL_H

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxCriticalSection. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* For SPxTimer. */
#include "SPxLibUtils/SPxTimer.h"

/* For SPxObj. */
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

enum SPxRemoteRadarType_t
{
    SPX_REMOTE_RADAR_P226 = 0,  /* P226 Radar (default). */
    SPX_REMOTE_RADAR_P430 = 1,  /* P430 Radar. */
    SPX_REMOTE_RADAR_P437 = 2   /* P437 Radar. */

};

/* Forward declare classes we need in case of wrong order headers. */
class SPxRemoteServer;

/*
 * Define our class.
 */
class SPxRemoteRadarCtrl : public SPxObj
{
public:

    /*
     * Public functions.
     */

    /* Constructors, destructor etc. */
    explicit SPxRemoteRadarCtrl(SPxRemoteRadarType_t remoteRadarType = SPX_REMOTE_RADAR_P226);
    SPxRemoteRadarCtrl(SPxRemoteRadarType_t remoteRadarType, const char *addr, unsigned int port);
    SPxRemoteRadarCtrl(SPxRemoteRadarType_t remoteRadarType, SPxRemoteServer *srvConnection);
    virtual ~SPxRemoteRadarCtrl(void);

    void SetServerAddress(const char *addr);
    void SetServerPort(unsigned int port);

    /* Connection related functions. */
    SPxErrorCode OpenConnection(void);
    SPxErrorCode CloseConnection(void);
    int IsConnected(void);
    int IsRadarAvailable(void);
    SPxRemoteRadarType_t GetRadarType(void) { return m_remoteRadarType; }

    /* Generic control functions. */
    virtual SPxErrorCode GetIdentifierString(char * /*idBuf*/, unsigned int /*bufLen*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetDescriptionString(char * /*descBuf*/, unsigned int /*bufLen*/) { return SPX_ERR_NOT_SUPPORTED; }

    virtual SPxErrorCode SetTransmitting(unsigned int /*enable*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetTransmitting(unsigned int * /*enableRtn*/) { return SPX_ERR_NOT_SUPPORTED; }

    virtual SPxErrorCode SetRangePercent(double /*range*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetRangePercent(double * /*rangeRtn*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode SetRangeScaleValue(unsigned int /*enumValue*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetRangeScaleValue(unsigned int * /*enumValueRtn*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetRangeMetres(double * /* rangeMetresRtn */) { return SPX_ERR_NOT_SUPPORTED; }

    virtual unsigned int GetNumRangeScales(void) { return 0; }
    virtual SPxErrorCode GetRangeScaleInfoAtIndex(unsigned int /*index*/, char * /*infoBuf*/, unsigned int /*bufLen*/) { return SPX_ERR_NOT_SUPPORTED; }

    virtual SPxErrorCode SetGainLevelPercent(double /*level*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetGainLevelPercent(double * /*levelRtn*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode SetGainAuto(unsigned int /*enable*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetGainAuto(unsigned int * /*enableRtn*/) { return SPX_ERR_NOT_SUPPORTED; }

    virtual SPxErrorCode SetSeaClutterLevelPercent(double /*level*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetSeaClutterLevelPercent(double * /*levelRtn*/) { return SPX_ERR_NOT_SUPPORTED; }

    virtual unsigned int GetNumSeaClutterModes(void) { return 0; }
    virtual int IsSeaClutterManual(void) { return FALSE; }

    virtual SPxErrorCode SetSeaClutterModeGeneric(unsigned int /*mode*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetSeaClutterModeGeneric(unsigned int * /*modeRtn*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetSeaClutterModeString(unsigned int /*mode*/, char * /*modeBuf*/, unsigned int /*bufLen*/) { return SPX_ERR_NOT_SUPPORTED; }

    virtual SPxErrorCode SetRainLevelPercent(double /*level*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetRainLevelPercent(double * /*levelRtn*/) { return SPX_ERR_NOT_SUPPORTED; }

protected:
    /*
     * Protected functions.
     */

    /* Setting remote radar parameter. */
    SPxErrorCode SetRadarParameter(const char *parameterName,
				   const char *parameterValue);
    SPxErrorCode SetRadarParameter(const char *parameterName,
				   int parameterValue);
    SPxErrorCode SetRadarParameter(const char *parameterName,
				   unsigned int parameterValue);
    SPxErrorCode SetRadarParameter(const char *parameterName,
				   REAL32 parameterValue);

    /* Getting remote radar parameter. */
    SPxErrorCode GetRadarParameter(const char *parameterName,
				   char *valueBuf,
				   int bufLen);
    SPxErrorCode GetRadarParameter(const char *parameterName,
				   int *valuePtr);
    SPxErrorCode GetRadarParameter(const char *parameterName,
				   unsigned int *valuePtr);
    SPxErrorCode GetRadarParameter(const char *parameterName,
				   REAL32 *valuePtr);

private:
    /*
     * Private fields.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxAutoPtr<SPxTimer> m_timer;

    /* Type of radar we are controlling. */
    SPxRemoteRadarType_t m_remoteRadarType;

    /* Object used to connect to the Server. */
    SPxRemoteServer *m_srvConnection;

    /* Whether or not this class needs to manage the connection. */
    int m_manageConnection;

    unsigned int m_port;

    /*
    * Private functions.
    */

    void Init(void);

}; /* SPxRemoteRadarCtrl class */

#endif /* _SPX_REMOTE_RADAR_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
