/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for P396 network receiving objects.
*
*
* Revision Control:
*   25/05/18 v1.2    AGC	Add complete blocks only option.
*
* Previous Changes:
*   16/05/18 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_NETRECV_P396_H
#define _SPX_NETRECV_P396_H

/*
 * Other headers required.
 */
/* We need the base class header (which includes several others as well). */
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

/* Forward declarations. */
class SPxRIB;

/*
 * Define our project-specific network receiver class, derived from the
 * standard one.
 */
class SPxNetworkReceiveP396 :public SPxNetworkReceive
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    explicit SPxNetworkReceiveP396(SPxRIB *buffer);
    virtual ~SPxNetworkReceiveP396();

    /* Configuration options. */
    SPxErrorCode SetTransmitterNumber(UINT8 transmitterNumber);
    UINT8 GetTransmitterNumber(void) const;
    SPxErrorCode SetCompleteBlocksOnly(int completeBlocksOnly);
    int IsCompleteBlocksOnly(void) const;

    /* Override the base class functions we want to handle ourselves. */
    virtual int ThreadHandleDataFn(unsigned char *data,
				   unsigned int dataSizeBytes);
    virtual int ThreadDecodeDataFn(unsigned char *data,
				   unsigned int dataSizeBytes);

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    /*
     * Private fields.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */

}; /* SPxNetworkReceiveP396 class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECV_P396_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
