/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for P427 network sending object.
*
* Revision Control:
*   08/05/19 v1.4    SP 	Use mutex in base class.
*
* Previous Changes:
*   16/04/19 1.3    SP 	Don't make SPxNetP427 virtual.
*   15/04/19 1.2    SP 	Fix line endings.
*   15/04/19 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef SPX_NET_SEND_P427_H_
#define SPX_NET_SEND_P427_H_

/* We need the base class header. */
#include "SPxLibNet/SPxNetP427.h"
#include "SPxLibNet/SPxNetSend.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
struct SPxReturn_tag;
struct SPxReturnHeader_tag;

/*
 * Define our project-specific network sender class, derived from the
 * standard one.
 */
class SPxNetworkSendP427 : public SPxNetworkSend, public SPxNetP427
{
public:

    /*
     * Public types.
     */

    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    /* Constructor, destructor etc. */
    explicit SPxNetworkSendP427(void);
    virtual ~SPxNetworkSendP427(void);

    /* General. */
    virtual UINT32 GetProjectCode(void) const { return 427; }
    virtual SPxErrorCode SetStreamID(StreamID_t streamID);
    virtual StreamID_t GetStreamID(void) { return m_streamID; }

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:

    /*
     * Protected fields.
     */

    /*
     * Protected functions.
     */

    /* Overwrite the base class encode and send functions. */
    unsigned char *EncodeReturn(const SPxReturn_tag *rtn,
                                unsigned char *destBuf,
                                unsigned int destBufSizeBytes,
                                unsigned int *encodedSizeBytesPtr,
                                unsigned int *formatUsedPtr);

    int SendReturn(const SPxReturnHeader_tag *rtnHeader,
                   unsigned char *data,
                   unsigned int numBytes,
                   unsigned int formatUsed);
private:

    /*
     * Private fields.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;
    StreamID_t m_streamID;
    UINT16 m_lastAzimuth;
    UINT32 m_hdrBuf[256];
    unsigned int m_lastHeaderSizeBytes;

    /*
     * Private functions.
     */

}; /* SPxNetworkSendP427 class */

#endif /* SPX_NET_SEND_P427_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
