/*********************************************************************
*
* (c) Copyright 2011 - 2017, 2020, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPacketDecoderFile class.
*
*	This is a generic class that may be used to read and decode an SPx
*	packet stream from a file.
*	It parses the messages given to it and invokes callback functions
*	according to the packet types found.
*
*
* Revision Control:
*   19/01/22 v1.28   AGC	Support rewind handler.
*
* Previous Changes:
*   04/12/20 1.27   SP 	Add SetErrorReplay().
*   07/07/17 1.26   AGC	Fix mutex lock issues.
*   23/03/17 1.25   SP 	Add optional silent flag to SetFileName().
*   27/09/16 1.24   SP	Add channel filter to ProcessNextPacket().
*			Add Get/GotoFileOffset().
*   16/09/16 1.23   AGC	Improve mutex protection.
*   08/07/16 1.22   AGC	Play back recorded error descriptions.
*   22/01/16 1.21   SP 	Add GetNumChannels().
*                       Make FastProcessNextPacket() arg optional.
*   11/01/16 1.20   SP 	Add SetAutoRewind().
*   10/12/15 1.19   SP 	Change FastProcessNextPacket() arg to set
*                       packet type.
*   10/12/15 1.18   AGC	Add play state callback.
*   04/12/15 1.17   AGC	Support single stepping.
*   03/11/15 1.16   AGC	Packet decoder callback data now const.
*   05/06/15 1.15   SP 	Remove used variable.
*   18/05/15 1.14   SP 	Modify handleError().
*   15/05/15 1.13   SP 	Add FastProcessNextPacket().
*                       Add setFileNameInternal().
*   23/04/15 1.12   SP 	Fix mutex issues.
*   22/04/15 1.11   SP 	Support replay between A/B markers.
*   16/05/14 1.10   AGC	Support ignoring timing information.
*   13/03/14 1.9    AGC	Support processing packets from within handlers.
*   10/03/14 1.8    AGC	Recover incorrectly byteswapped TOC entries.
*   08/11/13 1.7    AGC	Use const on appropriate functions.
*   21/02/13 1.6    SP 	Support auto link to next file in sequence.
*   18/04/12 1.5    REW	Add inHandler arg to the GetFileTime functions.
*   07/09/11 1.4    AGC	Make GetInitialised() function const.
*   12/05/11 1.3    AGC	Make GetFileName() and GetFullPath() const.
*   13/04/11 1.2    AGC	Return time to sleep from ProcessNextPacket().
*   06/04/11 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_PACKET_DECODER_FILE_H
#define _SPX_PACKET_DECODER_FILE_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxSysUtils.h"
#include "SPxLibData/SPxPacketDecoder.h"

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

/* Need to forward-declare a number of structures from SPxPackets.h. */
struct SPxPacketTOCHdr_tag;
struct SPxPacketTOCEntry_tag;
union SPxPacketHeaderUnion_tag;
class SPxPacketDecoderFile;

typedef void (*SPxPacketDecoderFilePlayStateFn_t)(SPxPacketDecoderFile *decoder,
						  void *userArg);
typedef void (*SPxPacketDecoderFileRewindFn_t)(SPxPacketDecoderFile *decoder,
					       void *userArg);
typedef void (*SPxPacketDecoderFileErrorFn_t)(SPxPacketDecoderFile *decoder,
					      void *userArg);

/*
 * Define our class.
 */
class SPxPacketDecoderFile : public SPxPacketDecoder
{
public:
    /*
     * Public functions.
     */
    SPxPacketDecoderFile(void);
    virtual ~SPxPacketDecoderFile(void);

    int IsPaused(void) const		{ return m_isPaused; }

    /* Define the basic controls. */
    int SetFileName(const char *fileName, int silent=FALSE);
    const char *GetFileName(void) const	{ return(m_fileName.GetShortPath()); }
    const char *GetFullPath(void) const	{ return(m_fileName.GetFullPath()); }
    void Pause(int inHandler=FALSE);
    void Play(void);
    void Rewind(void);

    /* Read next packet from file and process it. */
    SPxErrorCode ProcessNextPacket(UINT32 *sleepTimeMsecs,
	int mutexAlreadyLocked=FALSE, int useTiming=TRUE,
	int processIfPaused=FALSE,
	int channelFilter=-1);

    /* Quickly read next packet from file and process it, optionally
     * making use of link packets in the file.
     */
    SPxErrorCode FastProcessNextPacket(UINT16 packetType=0);

    /* Flag to say replayed errors should be reported. */
    void SetErrorReplay(int enable);
    int GetErrorReplay(void) const		{ return(m_errorReplay); }

    /* Flag to say replay should automatically loop again at end of file. */
    void SetAutoLoop(int enable);
    int GetAutoLoop(void) const		{ return(m_autoLoop); }

    /* Flag to say replay should automatically rewind at the end of a file
     * if auto loop is disabled. 
     */
    void SetAutoRewind(int enable);
    int GetAutoRewind(void) const		{ return(m_autoRewind); }

    /* Flag to say replay should automatically play next file in sequence. */
    void SetAutoLink(int enable);
    int GetAutoLink(void) const		{ return(m_autoLink); }

    /* Speed of replay */
    void SetSpeedupFactor(double factor);
    double GetSpeedupFactor(int inHandler=FALSE) const;

    /* Table of contents and searching functions. */
    int IsTOCAvailable(void) const { return(m_tocValid ? TRUE : FALSE); }
    SPxErrorCode GotoFileTime(const SPxTime_t *time, int inHandler=FALSE);
    SPxErrorCode GotoFileTime(UINT32 secs, int inHandler=FALSE)
    {
	SPxTime_t t; t.secs = secs; t.usecs = 0;
	return(GotoFileTime(&t, inHandler));
    }
    SPxErrorCode GotoFileTimePercent(double percent);
    SPxErrorCode GotoFileOffset(INT64 offset, int inHandler=FALSE);

    /* Replay markers. */
    virtual SPxErrorCode SetAMarkerTime(const SPxTime_t *time);
    virtual int GetAMarkerTime(SPxTime_t *time);
    virtual SPxErrorCode SetBMarkerTime(const SPxTime_t *time);
    virtual int GetBMarkerTime(SPxTime_t *time);

    /* File time retrieval. */
    void GetFileTimeStart(SPxTime_t *ptr, int inHandler=FALSE) const;
    void GetFileTimeCur(SPxTime_t *ptr, int inHandler=FALSE) const;
    void GetFileTimeEnd(SPxTime_t *ptr, int inHandler=FALSE) const;
    void SetFileTimeEnd(SPxTime_t *ptr, int inHandler=FALSE);
    SPxErrorCode GetFileTimePercent(double *percentPtr, int inHandler=FALSE) const;
    SPxErrorCode GetFileOffset(INT64 *offset, int inHandler=FALSE) const;

    /* Get number of channels in the file. */
    UINT8 GetNumChannels(int inHandler=FALSE) const;

    /* Base class status function. */
    virtual SPxErrorCode GetInitialised(int isMutexAlreadyLocked=FALSE) const;

    SPxErrorCode AddPlayStateHandler(SPxPacketDecoderFilePlayStateFn_t fn, void *userArg);
    SPxErrorCode RemovePlayStateHandler(SPxPacketDecoderFilePlayStateFn_t fn, void *userArg);
    SPxErrorCode AddRewindHandler(SPxPacketDecoderFileRewindFn_t fn, void *userArg);
    SPxErrorCode RemoveRewindHandler(SPxPacketDecoderFileRewindFn_t fn, void *userArg);

    SPxErrorCode InstallErrorHandler(SPxPacketDecoderFileErrorFn_t fn, void* userArg);

    /*
     * Static functions.
     */

    /* Byte-swapping functions. */
    static void ConvertTOCtoHost(SPxPacketTOCHdr_tag *toc);
    static void ConvertTOCtoNetwork(SPxPacketTOCHdr_tag *toc);
    static void ByteSwapTOC(SPxPacketTOCHdr_tag *toc);
    static void ConvertTOCEntryToHost(SPxPacketTOCEntry_tag *entry);
    static void ConvertTOCEntryToNetwork(SPxPacketTOCEntry_tag *entry);
    static void ByteSwapTOCEntry(SPxPacketTOCEntry_tag *entry);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    mutable SPxCriticalSection m_mutex;

    /* File information. */
    SPxPath m_fileName;			/* Filename for replay. */
    FILE *m_fileHandle;			/* I/O handle */
    UINT64 m_fileSizeBytes;		/* Total file size in bytes. */
    SPxTime_t m_fileTimeStart;		/* Start time, if known */
    SPxTime_t m_fileTimeEnd;		/* End time, if known */
    UINT8 m_numChannels;                /* No. of channels, if known */
    char m_nextFilename[FILENAME_MAX];  /* Filename of next file to replay. */

    /* Replay of errors. */
    int m_errorReplay;			/* TRUE to report replayed errors. */

    /* Playback state. */
    int m_isPaused;			/* Set to TRUE if replay is paused */
    int m_autoLoop;			/* Should we automatically loop? */
    int m_autoRewind;                   /* Should we automatically rewind? */
    int m_autoLink;                     /* Should we automatically link? */

    /* Timing information. */
    double m_speedUpFactor;	/* Factor to speed-up (>1) or slow-down (<1) */
    double m_oneOverLastFactor;	/* Reciprocal of factor in use (may lag) */
    UINT32 m_refMsecs;		/* Timestamp at our last reference point */
    UINT32 m_refFileSecs;	/* File time-component at m_refMsecs */
    UINT32 m_refFileUsecs;	/* File time-component at m_refMsecs */

    double m_lastSpeedUpFactor;
    int m_previouslyActive;

    /* A/B markers. */
    int m_isAMarkerSet;                 /* Is A marker set? */
    SPxTime_t m_aMarkerTime;            /* A marker time. */
    int m_isBMarkerSet;                 /* Is B marker set? */
    SPxTime_t m_bMarkerTime;            /* B marker time. */

    /* Table of contents. */
    int m_tocValid;			/* Is the TOC valid */
    SPxPacketTOCHdr_tag *m_tocHdr;	/* Header for the TOC */
    SPxPacketTOCEntry_tag *m_tocEntries;/* Array of entries */
    unsigned int m_tocEntriesSize;	/* Number of toc entries in array*/

    /* Packet timestamps. */
    SPxTime_t m_thisPacketTime;	/* Time for packet being processed */

    /* Payload buffer. */
    unsigned char *m_payloadBuf;	/* Buffer for reading payload */
    unsigned int m_payloadBufSize;	/* Size of buffer in bytes */

    /* Error handler. */
    SPxPacketDecoderFileErrorFn_t m_errorFn;
    void *m_errorFnArg;

    /* Link packets. */
    unsigned int m_numLinksFollowed;    /* Number of links followed. */
    UINT16 m_followLinksForPacketType;  /* Use link packets for this 
                                         * packet type (is non-zero). 
                                         */

    /*
     * Private functions.
     */

    /* Packet processing. */
    SPxErrorCode readNextPacketHeader(SPxPacketHeaderUnion_tag *hdr);
    SPxErrorCode unreadPacketHeader(SPxPacketHeaderUnion_tag *hdr);
    SPxErrorCode readAndProcessPayload(SPxPacketHeaderUnion_tag *hdr,
	int mutexAlreadyLocked=FALSE, int channelFilter=-1);
    SPxErrorCode skipPayload(SPxPacketHeaderUnion_tag *hdr);

    /* TOC handling. */
    static void processTocWrapper(SPxPacketDecoder *pktDecoder,
				 void *userArg,
				 UINT32 packetType,
				 struct sockaddr_in *from,
				 SPxTime_t *time,
				 const unsigned char *payload,
				 unsigned int payloadSize);
    void processToc(SPxPacketDecoder *pktDecoder,
				 void *userArg,
				 UINT32 packetType,
				 struct sockaddr_in *from,
				 SPxTime_t *time,
				 const unsigned char *payload,
				 unsigned int payloadSize);

    /* Next filename handling. */
    static void processNextNameWrapper(SPxPacketDecoder *pktDecoder,
					void *userArg,
					UINT32 packetType,
					struct sockaddr_in *from,
					SPxTime_t *time,
					const unsigned char *payload,
					unsigned int payloadSize);
    void processNextName(SPxPacketDecoder *pktDecoder,
					void *userArg,
					UINT32 packetType,
					struct sockaddr_in *from,
					SPxTime_t *time,
					const unsigned char *payload,
					unsigned int payloadSize);

    /* Link packet handling. */
    static void processLinkWrapper(SPxPacketDecoder *pktDecoder,
                                   void *userArg,
                                   UINT32 packetType,
                                   struct sockaddr_in *from,
                                   SPxTime_t *time,
                                   const unsigned char *payload,
                                   unsigned int payloadSize);
    void processLink(SPxPacketDecoder *pktDecoder,
                     void *userArg,
                     UINT32 packetType,
                     struct sockaddr_in *from,
                     SPxTime_t *time,
                     const unsigned char *payload,
                     unsigned int payloadSize);

    /* Error packet handling. */
    static void processErrorWrapper(SPxPacketDecoder *pktDecoder,
				    void *userArg,
				    UINT32 packetType,
				    struct sockaddr_in *from,
				    SPxTime_t *time,
				    const unsigned char *payload,
				    unsigned int payloadSize);
    void processError(SPxPacketDecoder *pktDecoder,
		      void *userArg,
		      UINT32 packetType,
		      struct sockaddr_in *from,
		      SPxTime_t *time,
		      const unsigned char *payload,
		      unsigned int payloadSize);

    /* Handling of errors in the file playback. */
    SPxErrorCode handleError(SPxErrorCode err, const char *obj);

    /* Set/change file name function for internal use. */
    int setFileNameInternal(const char *fileName, int silent=FALSE);

    /* Functions for internal use. */
    void pauseInternal(void);
    void playInternal(void);
    void rewindInternal(void);
    void callPlayStateFns(void);
    SPxErrorCode getInitialised(void) const;

}; /* SPxPacketDecoderFile */

#endif /* _SPX_PACKET_DECODER_FILE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
