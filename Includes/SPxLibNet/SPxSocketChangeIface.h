/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for socket change interface.
*
*
* Revision Control:
*   11/11/21 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_SOCKET_CHANGE_IFACE_H
#define _SPX_SOCKET_CHANGE_IFACE_H

/* For SPX_SOCKET_TYPE. */
#include "SPxLibUtils/SPxCommon.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

typedef void (*SPxSocketChangeCallback)(SOCKET_TYPE sock, void *userArg);

class SPxSocketChangeIface
{
public:
    /* Constructor and destructor. */
    SPxSocketChangeIface(void) {}
    virtual ~SPxSocketChangeIface(void) {}

    virtual SPxErrorCode AddSocketChangeCallback(SPxSocketChangeCallback fn,
						 void *userArg) = 0;
    virtual SPxErrorCode RemoveSocketChangeCallback(SPxSocketChangeCallback fn,
						    void *userArg) = 0;

}; /* SPxSocketChangeIface */

#endif /* SPX_PACKET_DECODER_NET_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
