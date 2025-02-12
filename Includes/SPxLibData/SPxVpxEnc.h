/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxVpxEnc.h,v $
* ID: $Id: SPxVpxEnc.h,v 1.2 2014/03/26 16:26:15 rew Exp $
*
* Purpose:
*   Interface for libvpx for web-m video encode.
*
* Revision Control:
*   26/03/14 v1.2    AGC	Remove needless forward declaration.
*
* Previous Changes:
*   05/03/14 1.1    AGC	Initial version.

**********************************************************************/
#ifndef _SPX_VPX_ENC_H
#define _SPX_VPX_ENC_H

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxErrorCode enumeration. */
#include "SPxLibUtils/SPxError.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

class SPxVpxEnc : public SPxObj
{
public:
    SPxVpxEnc(void);
    virtual ~SPxVpxEnc(void);
    SPxErrorCode Create(long width, long height);
    SPxErrorCode Destroy(void);

    SPxErrorCode EncodeFrameRGB32(const unsigned char *data, unsigned int numBytes);
    SPxErrorCode EncodeFrameAlpha(const unsigned char *data, unsigned int numBytes);
    SPxErrorCode EncodeFrameAlpha(const unsigned char *data, unsigned int numBytes,
	UINT8 r, UINT8 g, UINT8 b);
    SPxErrorCode EncodeFrameYV12(const unsigned char *data, unsigned int numBytes);

    SPxErrorCode GetEncodedData(const unsigned char **data,
				unsigned int *numBytes,
				INT64 *timecode, int *keyframe);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxErrorCode encodeFrameYV12(unsigned char *data, unsigned int numBytes);

}; /* SPxVpxEnc */

#endif /* _SPX_VPX_ENC_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
