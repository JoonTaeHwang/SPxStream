/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for P396 network sending object.
*
*
* Revision Control:
*   17/05/18 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_NET_SEND_P396_H
#define _SPX_NET_SEND_P396_H

/* For SPxEventHandler. */
#include "SPxLibUtils/SPxEventHandler.h"

/* We need the base class header (which includes several others as well). */
#include "SPxLibNet/SPxNetSend.h"

/* Forward declarations. */
struct SPxReturn_tag;
struct SPxReturnHeader_tag;

/*
 * Define our project-specific network sender class, derived from the
 * standard one.
 */
class SPxNetworkSendP396 : public SPxNetworkSend
{
public:
    typedef int(*InBlinderSectorCb)(UINT16 aziNum);

    /* Constructor, destructor etc. */
    SPxNetworkSendP396(void);
    virtual ~SPxNetworkSendP396();

    /* Type of network sender. */
    virtual UINT32 GetProjectCode(void) const { return 396; }

    virtual int IsEncodeFormatSupported(int format) const;

    /* Configuration options. */
    SPxErrorCode SetTransmitterNumber(UINT8 transmitterNumber);
    UINT8 GetTransmitterNumber(void) const;
    SPxErrorCode SetPulseLengthNS(UINT16 pulseLengthNS);
    UINT16 GetPulseLengthNS(void) const;
    SPxErrorCode SetSampleFormat(UINT8 sampleFormat);
    UINT8 GetSampleFormat(void) const;

    /* Callback for blind sector checking. */
    template<typename F, typename O>
    SPxErrorCode AddInBlindSectorCallback(F fn, O *obj)
    {
	return m_inBlindSectorCb.Add(fn, obj);
    }
    template<typename F, typename O>
    SPxErrorCode RemoveInBlindSectorCallback(F fn, O *obj)
    {
	return m_inBlindSectorCb.Remove(fn, obj);
    }

    /* Parameter setting functions. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:
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
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxEventHandler<InBlinderSectorCb> m_inBlindSectorCb;

}; /* SPxNetworkSendP396 class */

#endif /* _SPX_NET_SEND_P396_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
