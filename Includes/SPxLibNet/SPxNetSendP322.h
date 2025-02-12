/*********************************************************************
*
* (c) Copyright 2017 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for project-322 network sending module.
*
*
* Revision Control:
*   27/09/19 v1.8    AJH    Add scale factor.
*			    Support second channel for send-only.
*
* Previous Changes:
*   04/06/19 1.7    AJH	Add parameter control.
*   16/07/18 1.6    AGC	Move discovery/status to SPxNetCtrlP322.
*   23/05/18 1.5    AGC	Improve blind sector callback.
*   11/01/18 1.4    AGC	Use 16-bit azimuths for blind sector callback.
*   01/11/17 1.3    AGC	Add more functions for testing.
*   25/10/17 1.2    AGC	Add functions for testing.
*   23/10/17 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef SPX_NET_SEND_P322_H_
#define SPX_NET_SEND_P322_H_

/* For SPxEventHandler. */
#include "SPxLibUtils/SPxEventHandler.h"

/* We need the base class header (which includes several others as well). */
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

class SPxNetworkSendP322 : public SPxNetworkSend
{
public:
    typedef int(*InBlindSectorCb)(UINT16 aziNum);

    /* Constructor, destructor etc. */
    explicit SPxNetworkSendP322(void);
    virtual ~SPxNetworkSendP322(void);

    virtual UINT32 GetProjectCode(void) const { return 322; }

    SPxErrorCode SetAzimuthDim(unsigned int azimuthDim);
    unsigned int GetAzimuthDim(void);

    SPxErrorCode SetScaleFactor(unsigned int scaleFactor);
    unsigned int GetScaleFactor(void);

    virtual int IsEncodeFormatSupported(int format) const;

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

    /* Second channel for send-only function. */
    int SetSecondChannel(SPxNetworkSendP322 *secondChannel) {
	m_secondChannel = secondChannel;
	return SPX_NO_ERROR;
    };

    SPxNetworkSendP322 *m_secondChannel;

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
    SPxEventHandler<InBlindSectorCb> m_inBlindSectorCb;

}; /* SPxNetworkSendP322 class */

#endif /* SPX_NET_SEND_P322_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
