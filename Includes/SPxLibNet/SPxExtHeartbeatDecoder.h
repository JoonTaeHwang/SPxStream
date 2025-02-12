/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxExtHeartbeatDecoder class.
 *
 * Revision Control:
 *   24/04/20 v1.1   AGC	Initial version.
 *
 * Previous Changes:
 *
 *********************************************************************/

#ifndef _SPX_EXT_HEARTBEAT_DECODER_H
#define _SPX_EXT_HEARTBEAT_DECODER_H

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxSdbApplication.h"
#include "SPxLibNet/SPxPacketDecoderNet.h"

/* Forward declarations. */
struct sockaddr_in;
struct SPxTime_tag;

class SPxExtHeartbeatDecoder : public SPxPacketDecoderNet
{
public:
    typedef void (*HbFn)(SPxExtHeartbeatDecoder *obj,
                         void *userArg);

    /* Constructor and destructor. */
    SPxExtHeartbeatDecoder(void);
    virtual ~SPxExtHeartbeatDecoder(void);

    SPxErrorCode AddHbHandler(HbFn fn, void *userArg,
                              SPxSdbApplication::Type_t appType = SPxSdbApplication::TYPE_UNKNOWN,
                              const char *fromAddr = NULL);
    SPxErrorCode AddHbHandler(HbFn fn, void *userArg,
                              SPxSdbApplication::Type_t appType = SPxSdbApplication::TYPE_UNKNOWN,
                              UINT32 fromAddr = 0);
    SPxErrorCode RemoveHbHandler(HbFn fn, void *userArg,
                                 int waitIfInvoking = TRUE);

    SPxErrorCode GetHbField(const char *fieldQuery, int *value) const;
    SPxErrorCode GetHbField(const char *fieldQuery,
                            char *buf, unsigned int bufLen) const;

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    static void staticHandleHeartbeat(SPxPacketDecoder *decoder,
                                      void *userArg,
                                      UINT32 packetType,
                                      struct sockaddr_in *from,
                                      struct SPxTime_tag *time,
                                      const unsigned char *payload,
                                      unsigned int numBytes);
    void handleHeartbeat(struct sockaddr_in *from,
                         struct SPxTime_tag *time,
                         const unsigned char *payload,
                         unsigned int numBytes);
    void parseHeartbeat(UINT32 fromAddr);

}; /* SPxExtHeartbeatDecoder */

#endif /* _SPX_EXT_HEARTBEAT_DECODER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
