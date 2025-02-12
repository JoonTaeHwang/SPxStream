/*********************************************************************
*
* (c) Copyright 2013-2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxNetCat253Server.h,v $
* ID: $Id: SPxNetCat253Server.h,v 1.2 2015/12/16 16:10:05 rew Exp $
*
* Purpose:
*	Header for ASTERIX Cat-253 network server object.
*
* Revision Control:
*   16/12/15 v1.2    AJH	Add BuildMesg() and SendMesgUDP().
*
* Previous Changes:
*   08/04/13 1.1    SP 	Initial Version.
**********************************************************************/

#ifndef _SPX_NET_CAT_253_SERVER_H
#define _SPX_NET_CAT_253_SERVER_H

/*
 * Other headers required.
 */

/* We need the base class headers. */
#include "SPxLibNet/SPxNetCat253.h"
#include "SPxLibNet/SPxNetSend.h"
#include "SPxLibNet/SPxPacketSend.h"


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

class SPxNetCat253Server : public SPxNetCat253, public SPxNetworkSend
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    /* Constructor, destructor etc. */
    SPxNetCat253Server(void);
    virtual ~SPxNetCat253Server();

    /* Message sending. */
    SPxErrorCode SendMesg(UINT8 *appData, unsigned int numAppDataBytes);
    SPxErrorCode BuildMesg(UINT8 *appData, unsigned int numAppDataBytes,
			   unsigned int bufOffset, unsigned int *mesgBytes);
    SPxErrorCode SendMesgUDP(unsigned int numBytes);
    SPxErrorCode PollForMesgs(void);
    void SetStatusConnection(SPxPacketSender *sender) { m_packetSender = sender; };
    SPxNetworkSendClient_t *GetClientInfo(int idx) { return TCPGetClientInfo(idx); }

    /* Parameter setting functions. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:

    /*
     * Protected functions.
     */

    /*
     * Protected fields.
     */

private:

    /*
     * Private fields.
     */
    SPxPacketSender *m_packetSender;

    /*
     * Private functions.
     */

    void TCPDataReceived(SPxNetworkSendClient_t *client);

    /*
     * Private static functions.
     */

}; /* SPxNetCat253Server class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NET_CAT_253_SERVER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
