/*********************************************************************
*
* (c) Copyright 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxNetCat253Client.h,v $
* ID: $Id: SPxNetCat253Client.h,v 1.2 2013/11/08 14:27:53 rew Exp $
*
* Purpose:
*	Header for ASTERIX Cat-253 network client object.
*
* Revision Control:
*   08/11/13 v1.2    SP 	Overwrite ThreadBackgroundFn().
*
* Previous Changes:
*   08/04/13 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_NET_CAT_253_CLIENT_H
#define _SPX_NET_CAT_253_CLIENT_H

/*
 * Other headers required.
 */

/* We need the base class headers. */
#include "SPxLibNet/SPxNetCat253.h"
#include "SPxLibNet/SPxNetRecv.h"

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

class SPxNetCat253Client : public SPxNetCat253, public SPxNetworkReceive
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    /* Constructor, destructor etc. */
    SPxNetCat253Client(void);
    virtual ~SPxNetCat253Client();

    /* Message handling. */
    SPxErrorCode SendMesg(UINT8 *appData, unsigned int numDataBytes);
    SPxErrorCode PollForMesgs(void);

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:

    /*
     * Protected functions.
     */

    /* Overwrite the base class functions we want to handle ourselves. */
    int ThreadHandleDataFn(unsigned char *data,
                           unsigned int dataSizeBytes);  

    int ThreadBackgroundFn(void);

 
    /*
     * Protected fields.
     */

private:

    /*
     * Private fields.
     */

    /*
     * Private functions.
     */

}; /* SPxNetCat253Client class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NET_CAT_253_CLIENT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
