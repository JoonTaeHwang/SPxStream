/*********************************************************************
*
* (c) Copyright 2008 - 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxScreenRecord.h,v $
* ID: $Id: SPxScreenRecord.h,v 1.16 2014/03/05 15:04:55 rew Exp $
*
* Purpose:
*	Header for SPx Screen Recorder class.
*
*
* Revision Control:
*   05/03/14 v1.16   AGC	Fix #282 - header rewrite corrupts TOC offsets.
*
* Previous Changes:
*   04/10/13 1.15   AGC	Simplify headers.
*   08/03/13 1.14   SP 	Add SetBaseFilename().
*   01/03/13 1.13   SP 	Make filename arg const.
*   21/02/13 1.12   SP 	Write TOC and next file blocks to file.
*   21/12/12 1.11   SP 	Increase max screens to 3.
*                       Move MAX_NUM_SCREENS to SPxScreen.h.
*                       Add Create() arg to control optimised writes.
*   15/09/10 1.10   REW	Make destructor virtual.
*   26/08/09 1.9    REW	Add m_optimiseWrites.
*   23/07/09 1.8    REW	Change handling of filenames etc.
*			Add SetMaxFileSecs().
*   23/07/09 1.7    REW	Change captureScreenIndex() last arg.
*   22/07/09 1.6    REW	Derive from SPxObj.  Support Get/SetParams.
*   21/07/09 1.5    REW	Support writing of partial grabs to file.
*   08/07/09 1.4    SP 	Support optimised capture using XDamage.
*   10/02/09 1.3    REW	Add SetHeaderRewritePeriodSecs().
*   27/11/08 1.2    REW	Use new file format with packet headers.
*   27/11/08 1.1    DGJ	Initial checked-in version.
**********************************************************************/

#ifndef _SPX_SCREENRECORD_H
#define	_SPX_SCREENRECORD_H

/*
 * Other headers required.
 */
/* We need the common header (which may include others as well). */
#include "SPxScreen.h"

/* For SPX_MAX_NUM_SCREENS. */
#include "SPxLibData/SPxPackets.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Debug flags.
 */
#define SPX_SR_DEBUG_BASIC		(1 << 0)
#define SPX_SR_DEBUG_PER_CAPTURE	(1 << 1)
#define SPX_SR_DEBUG_OPTIMISER	        (1 << 2)

/* ID used to denote an invalid region object */
#define INVALID_REGION                  0

/* Size of the Table of Contents. A 5000 entry TOC gives us 1-second 
 * resolution up to 83 mins, or 17 second resolution up to 24 hours.
 */
#define SPX_SR_TOC_NUM_ENTRIES          5000

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

/*
 * Screen recorder class.
 */
class SPxScreenRecord :public SPxObj
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxScreenRecord(UINT32 debug = 0);
    virtual ~SPxScreenRecord();

    /* Scale control - use after construction but before Create(). */
    SPxErrorCode SetScale(int scaleFactor);

    /* Creation. */
    SPxErrorCode Create(Display *disp, 
			const char *fileName, 
			int screenMask,
			int optimiseCapture=FALSE,
                        int optimiseWrites=TRUE);

    /* Set/get filename prefix. */
    SPxErrorCode SetBaseFilename(const char *fileName);
    SPxErrorCode GetBaseFilename(char *buf, unsigned int bufSize);
    
    /* Log X events used for optimisation. */
    SPxErrorCode ReportXEvent(XEvent *event);

    /* Control over options. */
    void CreateNewFileOnResume(int state) {m_newFileOnResume = state;}
    void SetHeaderRewritePeriodSecs(unsigned int secs)
    {
	m_headerRewritePeriodSecs = secs;
    }
    unsigned int GetHeaderRewritePeriodSecs(void)
    {
	return(m_headerRewritePeriodSecs);
    }
    void SetRefFramePeriodSecs(double secs) { m_refFramePeriodSecs = secs; }
    double GetRefFramePeriodSecs(void)	{ return(m_refFramePeriodSecs); }
    void SetMaxFileSecs(unsigned int secs) { m_maxFileSecs = secs; }
    unsigned int GetMaxFileSecs(void)	{ return(m_maxFileSecs); }

    /* Pause and Resume operation */
    SPxErrorCode Pause(void);
    SPxErrorCode Resume(void);
    int IsPaused(void)	{return(m_isPaused); }

    /* Capture API. */
    SPxErrorCode CaptureNow(int screenMask);

    /* Performance test */
    SPxErrorCode PerformanceTest(int screenNumber,
					double *grabTimePtr=0, 
					double *grabEncodeTimePtr=0,
					double *grabEncodeStoreTimePtr=0);

    /* Parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private fields.
     */
    /* Creation flags. */
    int m_constructOK;		/* Indicates if object was constructed ok */

    /* File information. */
    char *m_fileName;		/* Filename for recording */
    char *m_baseFileName;	/* This is the original filename */
    FILE *m_file;		/* FILE object */
    int m_fileCount;            /* Num files created. */
    UINT64 m_fileSizeBytes;     /* Current file size in bytes. */
    int m_startNewFile;         /* Flag to force a new file to be created. */

    /* Details of the screens being captured. */
    Display *m_xdisplay;	/* X Windows display */
    int m_screenMask;		/* Mask for screens being recorded */
    SPxScreenRecordScreen m_screens[SPX_MAX_NUM_SCREENS];

    /* Output buffer. */
    UINT32 *m_outputBuffer;	/* Output buffer */
    unsigned m_outputBufferSz;	/* Size of output buffer (in 32-bit words) */

    /* Pause mode. */
    int m_isPaused;		/* Zero for recording, non-zero for paused. */

    /* Options. */
    int m_newFileOnResume;	/* Create a new file on a Resume event? */
    int m_scale;		/* Scale factor */
    unsigned int m_headerRewritePeriodSecs;	/* Period, or 0 to disable */
    int m_optimiseCapture;      /* Use shared mem pixmaps to grab changes */
    int m_optimiseWrites;	/* Use XDamage to only store changed areas */
    double m_refFramePeriodSecs;	/* Period between reference frames */
    unsigned int m_maxFileSecs;	/* Maximum length of files in seconds */

    /* Timing information. */
    int m_captureCount;			/* Number of captures we have made */
    double m_averageFPS;		/* Average fps for recording */
    SPxTime_t m_startTime;		/* Start of recording */
    SPxTime_t m_lastCaptureTime;	/* End of recording */
    SPxTime_t m_lastTimeWroteFileControl;	/* Last control block write */

    /* Debug. */
    UINT32 m_debugFlags;		/* Debug options */

    /* Event base ID for XServer extensions */
    int m_damageEventBase;
    int m_fixesEventBase; 

    /* Table of contents. */
    SPxPacketTOCHdr m_tocHdr;
    SPxPacketTOCEntry m_tocArray[SPX_SR_TOC_NUM_ENTRIES];
    UINT32 m_tocResolutionSecs;   /* Resolution of TOC entries */
    UINT32 m_tocLastTimeEntered;  /* Time of last entry */

    /*
     * Private functions.
     */
    /* Per-screen setup etc. */
    SPxErrorCode readScreenData(int xscreen, SPxScreenRecordScreen *screen);
    SPxErrorCode createImageForScreen(SPxScreenRecordScreen *screen);

    /* Create the next file (e.g. on a resume event). */
    SPxErrorCode createNextFile(void);

    /* Close current file. */
    SPxErrorCode closeFile(const char *nextFilename);

    /* Master block and TOC creation and output. */
    SPxErrorCode writeFileTOCAndMasterBlock(int rewrite=FALSE);
    SPxErrorCode writeTOC(int rewrite=FALSE);
    SPxErrorCode resetTOC(void);
    SPxErrorCode considerNewTOCEntry(UINT32 packetSecs);

    /* Actual capture and encoding functions. */
    SPxErrorCode captureScreenIndex(int index,
                                    int doEncode=TRUE, 
                                    int doWrite=TRUE,
                                    int forceFullGrab=FALSE);
    int encodeFrame(UINT32 *basePtr, int x, int y, int w, int h,
				int pageWidthPixels, int scale);

    /* Packet writing. */
    SPxErrorCode writeImageToFile(int index, int x, int y, int w, int h,
				void *encodedData, int numEncodedWords,
				SPxTime_t *now);
    SPxErrorCode writeToFile(SPxPacketHeaderB *packetHdr,
				void *payloadHdr, unsigned int payloadHdrSize,
				void *payload, unsigned int payloadSize,
				int rewrite=FALSE);

}; /* SPxScreenRecord class */

	
/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SCREENRECORD_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
