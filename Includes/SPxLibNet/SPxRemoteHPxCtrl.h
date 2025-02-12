/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxRemoteHPxCtrl class, which allows client
*	applications to remoteley control HPx cards.
*
*
* Revision Control:
*   02/12/20 v1.2    BTB	Add GetCardType().
*
* Previous Changes:
*   27/11/20 1.1    BTB	Initial Version.
*
**********************************************************************/

#ifndef _SPX_REMOTE_HPX_CTRL_H
#define _SPX_REMOTE_HPX_CTRL_H

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

/* Forward declare classes we need in case of wrong order headers. */
class SPxRemoteServer;

/*
 * Define our class.
 */
class SPxRemoteHPxCtrl : public SPxObj
{
public:
    /*
     * Public functions.
     */

    /* Constructors, destructor etc. */
    explicit SPxRemoteHPxCtrl(SPxRemoteServer *srvConnection);
    virtual ~SPxRemoteHPxCtrl(void);

    int IsConnected(void);
    int IsHPxAvailable(void);

    /* Control functions. */
    virtual SPxErrorCode GetCardType(unsigned int *typeRtn);

    virtual SPxErrorCode SetChannelAGain(double gain);
    virtual SPxErrorCode GetChannelAGain(double *gainRtn);

    virtual SPxErrorCode SetChannelAOffset(double voltage);
    virtual SPxErrorCode GetChannelAOffset(double *voltageRtn);

    virtual SPxErrorCode SetChannelBGain(double gain);
    virtual SPxErrorCode GetChannelBGain(double *gainRtn);

    virtual SPxErrorCode SetChannelBOffset(double voltage);
    virtual SPxErrorCode GetChannelBOffset(double *voltageRtn);

    virtual SPxErrorCode SetStartRange(double rangeMetres);
    virtual SPxErrorCode GetStartRange(double *rangeMetresRtn);

    virtual SPxErrorCode SetEndRange(double rangeMetres);
    virtual SPxErrorCode GetEndRange(double *rangeMetresRtn);

    virtual SPxErrorCode SetRangeCorrection(double correctionMetres);
    virtual SPxErrorCode GetRangeCorrection(double *correctionMetresRtn);

protected:
    /*
     * Protected functions.
     */

    /* Setting remote HPx parameter. */
    SPxErrorCode SetHPxParameter(const char *parameterName,
                                 const char *parameterValue);
    SPxErrorCode SetHPxParameter(const char *parameterName,
                                 int parameterValue);
    SPxErrorCode SetHPxParameter(const char *parameterName,
                                 unsigned int parameterValue);
    SPxErrorCode SetHPxParameter(const char *parameterName,
                                 REAL32 parameterValue);

    /* Getting remote radar parameter. */
    SPxErrorCode GetHPxParameter(const char *parameterName,
                                 char *valueBuf,
                                 int bufLen);
    SPxErrorCode GetHPxParameter(const char *parameterName,
                                 int *valuePtr);
    SPxErrorCode GetHPxParameter(const char *parameterName,
                                 unsigned int *valuePtr);
    SPxErrorCode GetHPxParameter(const char *parameterName,
                                 REAL32 *valuePtr);

private:
    /*
     * Private fields.
     */

    /* Object used to connect to the Server. */
    SPxRemoteServer *m_srvConnection;

}; /* SPxRemoteHPxCtrl class */

#endif /* _SPX_REMOTE_HPX_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
