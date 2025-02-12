/*********************************************************************
*
* (c) Copyright 2008 - 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxScreenReplay.h,v $
* ID: $Id: SPxScreenReplay.h,v 1.11 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*	Header for SPx Screen Replay class.
*
*
* Revision Control:
*   04/10/13 v1.11   AGC	Simplify headers.
*
* Previous Changes:
*   21/12/12 1.10   SP 	Increase max screens to 3.
*   15/09/10 1.9    REW	Make destructor virtual.
*   29/07/09 1.8    REW	Debug flags in constructor etc.
*   28/07/09 1.7    REW	More Get functions.
*   27/07/09 1.6    REW	Support scaling down on replay.
*   21/07/09 1.5    REW	Support replay of partial frames.
*   20/02/09 1.4    REW	Fix bug #023, time drift on replay.
*   27/11/08 1.3    REW	Use new file format with packet headers.
*   27/11/08 1.2    REW	Remove configFile from constructor.
*   27/11/08 1.1    DGJ	Initial checked-in version.
**********************************************************************/

#ifndef _SPX_SCREENREPLAY_H
#define	_SPX_SCREENREPLAY_H

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
#define	SPX_SR_REPLAY_DEBUG_BASIC	0x00000001


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
 * Screen replay class.
 */
class SPxScreenReplay
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxScreenReplay(UINT32 debug = 0);
    virtual ~SPxScreenReplay();

    /* Creation. */
    SPxErrorCode Create(Display *disp, char *filename);

    /* Replay speed. */
    SPxErrorCode SetReplaySpeed(double speed);
    double GetReplaySpeed(void) {return m_replaySpeed;}

    /* Control over replay time. */
    SPxErrorCode SetReplaySecsFromStart(double secondsFromStart);
    SPxErrorCode SetReplayEpoch(SPxTime_t *epochTime);

    /* Replay scaling.*/
    SPxErrorCode SetReplayScale(unsigned int scale)
    {
	if( scale > 1 ) { m_scale = scale; }
	else { m_scale = 1; }
	return(SPX_NO_ERROR);
    }
    unsigned int GetReplayScale(void)	{ return(m_scale); }

    /* Time of next frame to be played. */
    SPxErrorCode GetNextTime(SPxTime_t *nextTime, int *pausePtr=NULL);

    /* Pause and Resume operation */
    SPxErrorCode Pause(void);
    SPxErrorCode Resume(void);
    int IsPaused(void)			{ return(m_isPaused); }

    /* Rewind. */
    SPxErrorCode Rewind(void);

    /* Actual replay function. */
    SPxErrorCode ReplayOne(void);

    /* Window and screen control. */
    Window GetWindow(int screen)	{ return(m_screenWindow[screen]); }
    SPxErrorCode SetScreenWindow(int index, Window win);
    SPxErrorCode SetMapping(int input, int output);

    /* File info. */
    const SPxTime_t *GetRecordingStartTime(void) {return &m_recordingStartTime;}
    const SPxTime_t *GetRecordingStopTime(void) {return &m_recordingStopTime;}
    UINT32 GetRecordingDuration(void)
    {
	return(m_fileBlock.durationOfRecording);
    }
    UINT32 GetRecordingFramesPerSec(void)
    {
	return(m_fileBlock.framesPerSecond);
    }


private:
    /*
     * Private fields.
     */
    /* Creation flags. */
    int m_constructOK;		/* Indicates if object was constructed ok */
    UINT32 m_debugFlags;	/* Debug options */

    /* File information. */
    char *m_fileName;		/* Filename for replaying from */
    FILE *m_file;		/* FILE object */
    SPxSRfileMasterBlock m_fileBlock;	

    /* Details of the screens being replayed on. */
    Display *m_xdisplay;	/* X Windows Display for replay */
    int m_screenMask;		/* Mask of which screens are being replayed */
    SPxScreenRecordScreen m_inputScreens[SPX_MAX_NUM_SCREENS];
    SPxScreenRecordScreen m_outputScreens[SPX_MAX_NUM_SCREENS];
    UINT32 *m_interimBuffer[SPX_MAX_NUM_SCREENS]; /* Interim buffers */
    unsigned int m_interimBufferSizeBytes[SPX_MAX_NUM_SCREENS];	/* Sizes of buffers */
    Window m_screenWindow[SPX_MAX_NUM_SCREENS];	/* Windows being output. */
    int m_screenMapper[SPX_MAX_NUM_SCREENS];
    Pixmap m_pixmap[SPX_MAX_NUM_SCREENS];
    GC m_gc[SPX_MAX_NUM_SCREENS]; /* The per-screen graphics context */
    XFontStruct *m_font;

    /* Input buffer. */
    unsigned char *m_inputBuffer;	/* Input buffer memory */
    unsigned m_inputBufferSz;		/* Size of input buffer in bytes */

    /* Pause mode. */
    int m_isPaused;			/* 0 for play, non-zero for paused. */

    /* Options. */
    int m_useDoubleBuffer;		/* Double buffer updates for overlay */
    int m_useFullWindowNoBorders;	/* Use full screen with no borders */
    unsigned int m_scale;		/* Scale factor */

    /* Timing information. */
    SPxTime_t m_recordingStartTime;	/* Start time of the recording */
    SPxTime_t m_recordingStopTime;	/* Stop time of the recording */
    double m_replaySpeed;		/* Factor that affects the rate of
					 * replay (1.0 realtime, 2 speed-up,
					 * 0.5 slow down for example).
					 */

    /*
     * Private functions.
     */
    /* Screen control. */
    SPxScreenRecordScreen *getOutputScreen(int index)
    {
	if( (index < 0) || (index >= SPX_MAX_NUM_SCREENS) )
	{
	    return(NULL);
	}
	return(&m_outputScreens[index]);
    }
    SPxErrorCode createImageForScreen(SPxScreenRecordScreen *screen);

    /* File reading. */
    SPxErrorCode readNextPacketHeader(SPxPacketHeaderB_tag *hdr,
					int reportFileEndErrors=TRUE);
    SPxErrorCode unreadPacketHeader(SPxPacketHeaderB_tag *hdr);
    SPxErrorCode skipPacketPayload(SPxPacketHeaderB_tag *hdr);
    SPxErrorCode readFrameHeader(SPxPacketHeaderB_tag *hdr,
					SPxSRframeHeader *frameHeader);
    SPxErrorCode readFileMasterBlock(void);
    SPxErrorCode readScreenData(int xscreen, SPxScreenRecordScreen *screen);

    /* Actual decoding and output. */
    SPxErrorCode decodeFrame(SPxSRframeHeader *frameHeader,
					unsigned char *compressedData,
					UINT32 *uncompressedData,
					int pageWidthPixels);
}; /* SPxScreenReplay class */

	
/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_SCREENREPLAY_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
