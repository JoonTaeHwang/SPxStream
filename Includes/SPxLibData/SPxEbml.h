/*********************************************************************
*
* (c) Copyright 2014, 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxEbml.h,v $
* ID: $Id: SPxEbml.h,v 1.2 2016/04/20 13:17:57 rew Exp $
*
* Purpose:
*   Interface for EBML file interaction.
*
* Revision Control:
*   20/04/16 v1.2    AGC	Abstract file interface.
*
* Previous Changes:
*   05/03/14 1.1    AGC	Initial version.

**********************************************************************/
#ifndef _SPX_EBML_H
#define _SPX_EBML_H

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

/* Forward declarations. */

class SPxEbmlStream
{
public:
    SPxEbmlStream(void) {}
    virtual ~SPxEbmlStream(void) {}

    virtual const char *GetName(void) const { return NULL; }
    virtual unsigned int Write(const unsigned char *data, unsigned int dataBytes) = 0;

    virtual int HasSeek(void) const { return FALSE; }
    virtual int Seek(INT64 /*pos*/) { return -1; }
    virtual INT64 Tell(void) { return 0; }
};

class SPxEbml : public SPxObj
{
public:
    SPxEbml(void);
    virtual ~SPxEbml(void);

    SPxErrorCode OpenFile(const char *filename,
			  long width, long height);
    SPxErrorCode CloseFile(void) { return Close(); }
    SPxErrorCode GetFilename(char *buf, int bufLen) const
    { return GetStreamName(buf, bufLen); }

    SPxErrorCode Open(SPxEbmlStream *stream,
		      long width, long height);
    SPxErrorCode Close(void);
    SPxErrorCode GetStreamName(char *buf, int bufLen) const;

    SPxErrorCode WriteFrame(const void *data, unsigned int dataBytes,
	INT64 timecode, int keyframe,
	const void *alphaData=NULL, unsigned int alphaBytes=0);

    static int EncodeVal(UINT64 val, UINT8 *buf=NULL, int bufLen=0);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxErrorCode open(SPxEbmlStream *stream,
		      long width, long height,
		      int fileIsOwned);
    SPxErrorCode writeSegmentHdr(INT64 size=0);
    SPxErrorCode writeSeekHeadHdr(void);
    SPxErrorCode writeClusterHdr(INT64 size=0);
    SPxErrorCode writeCues(void);
    SPxErrorCode rewriteSegmentHdr(void);
    SPxErrorCode rewriteSeekHeadHdr(INT64 cuesLoc);
    SPxErrorCode rewriteClusterHdr(void);
    SPxErrorCode createCue(INT64 timecode);

}; /* SPxEbml */

#endif /* _SPX_EBML_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
