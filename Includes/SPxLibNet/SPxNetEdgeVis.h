/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxNetEdgeVis class - a class
*	to allow sending events to a Digital Barriers EdgeVis device.
*
* Revision Control:
*   06/07/20 v1.6   AJH	Handle command return code.
*
* Previous Changes:
*   18/06/20 1.5   AJH	Make snapshots optional.
*   15/06/20 1.4   AJH	Add test report facility.
*   15/06/20 1.3   AJH	Fix build warnings.
*   11/06/20 1.2   AJH	Add sending of thumbnail to alert.
*   10/06/20 1.1   AJH	Initial version.
**********************************************************************/
#ifndef _SPX_EDGE_VIS_H
#define _SPX_EDGE_VIS_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxArea.h"
#include "SPxLibUtils/SPxImage.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*
 * Define our class.
 */
class SPxNetEdgeVis : public SPxObj
{
public:
    /* Types. */

    /* Construction and destruction. */
    SPxNetEdgeVis(void);
    virtual ~SPxNetEdgeVis(void);
    SPxErrorCode Create(void);

    /* Access routines. */
    SPxErrorCode SetAddress(const char *address);
    char *GetAddress(void) const;
    SPxErrorCode SetPortNum(int port);
    int GetPortNum(void) const;
    SPxErrorCode SetInterface(const char *ifAddr);
    char *GetInterface(void) const;
    SPxErrorCode SetPassword(const char *password);
    char *GetPassword(void) const;
    SPxErrorCode SetSnapshots(int snapshots);
    int GetSnapshots(void) const;

    /* Device interaction. */
    SPxErrorCode PostEvent(double measuredTemp, double thresholdTemp, SPxAreaF *box, SPxImage *img);

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;	/* Private structure. */

    /* Private functions. */
    int connect(void) const;
    int authenticate(void);
    SPxErrorCode sendCommand(const char *url, const char *request, int *retCode) const;
    int getResponseBool(const char *key) const;
    const char *getResponseString(const char *key) const;
    void addItem(void *msgWriterVoid, const char *key, const char *value);
    SPxErrorCode postTestEvent(void);
    void handleTimer(void);

}; /* SPxNetEdgeVis */

#endif /* _SPX_EDGE_VIS_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
