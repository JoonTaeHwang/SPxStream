/*********************************************************************
*
* (c) Copyright 2007 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for SPxPIM class and associated code.
*
* Revision Control:
*   10/08/20 v1.67   AGC	Add option to use return heading for azi referencing.
*
* Previous Changes:
*   25/09/19 1.66   DGJ New argument for video track function
*   20/06/19 1.65   DGJ	Support centroiding of video area.
*   22/02/19 1.64   AGC	Support PIM rescaling.
*   05/11/18 1.63   AGC	Avoid potential deadlock when getting state.
*   08/10/18 1.62   AGC	Support automatic pitch/roll correction.
*   31/08/18 1.61   AGC	Add GetNavData().
*   27/10/17 1.60   REW	Add flag to Clear().
*   27/09/17 1.59   REW	Add CompareWithPIM().
*   25/07/17 1.58   REW	Add more Get functions.
*   08/03/17 1.57   AGC	Make various functions const.
*   29/11/16 1.56   AGC	Add GetReturn() function.
*   03/10/16 1.55   AGC	Make mutex mutable.
*   23/08/16 1.54   AGC	Add overload of GetLastReturnHeader().
*   15/06/16 1.53   AGC	Add option to disable heading extrapolation.
*   18/05/16 1.52   AGC	Support course-up radar video.
*   03/03/16 1.51   SP 	Add mutex to protect m_navData.
*   01/02/16 1.50   AGC	Add GetAzimuthRepeat().
*   03/11/15 1.49   AGC	Packet decoder callback data now const.
*   11/11/14 1.48   JP 	Fix portability issue with last change.
*   10/11/14 1.47   JP 	Add member function to update last azimuth.
*			PimCopy process is now friend of SPxPim to 
*	    		be able to update the lastPimAzimth.
*   24/09/14 1.46   SP 	Add GetAveragePRF().
*   23/07/14 1.45   SP 	Add GetLastNominalLength().
*   10/04/14 1.44   SP 	Add ResetAveragePeriod().
*   10/10/13 1.43   REW	Support high-res azimuth offsets.
*   04/10/13 1.42   AGC	Simplify headers.
*   13/07/12 1.41   SP 	Add GetAzimuthSubsample().
*   13/01/12 1.40   REW	Support interference suppression.
*   25/11/11 1.39   REW	Tweak Get/SetSensorSpecificData().
*   10/11/11 1.38   REW	Add Get/SetSensorSpecificData().
*   04/10/11 1.37   REW	Add CopyFromPIM(), SaveToFile(), LoadFromFile().
*   30/08/11 1.36   SP 	Remove debug print.  Add m_lastUpdateErrorTimeMsecs.
*   19/08/11 1.35   REW	Avoid trailing comma warning in last change.
*   19/08/11 1.34   SP 	Support 16-bit samples.
*   28/10/10 1.33   AGC	Add GetParameter() function.
*   28/10/10 1.32   SP 	Add GetAveragePeriod() and Set/GetAziRefMode()
*   15/09/10 1.31   REW	Make destructor virtual.
*   11/03/10 1.30   DGJ	Additional debug option for video "quick look"
*   04/03/10 1.29   DGJ	Option to debug video data.
*   18/02/10 1.28   DGJ	Per-object m_debug. Add SPX_PIM_DEBUG_NAV_DATA.
*   09/02/10 1.27   DGJ	Support debug.
*   04/12/09 1.26   REW	Add SPX_PIM_FLAGS_HEADING_SET.
*   20/10/09 1.25   REW	Add SetConstantAziLimit() etc.
*   24/04/09 1.24   DGJ	Support NorthReference.  New PIM flags
*			REW corrected versions back to V1.9.
*   30/03/09 1.23   REW	Add m_justChangedSourceType flag.
*   24/03/09 1.22   REW	Add brackets around a in SPX_PIM_OUTPUT.
*   23/01/09 1.21   REW	Add GetEndRange() and similar functions.
*   28/08/08 1.20   REW	Maintain m_lastNominalLength.
*   01/08/08 1.19   DGJ	Add GetLastInputAzimuth()/UpdatedAzimuth().
*   31/07/08 1.18   REW	Add SPX_PIM_FLAGS_... definitions etc.
*   04/07/08 1.17   REW	Add GetPRF() function.
*   07/02/08 1.16   REW	Add m_scanMode field.
*   17/01/08 1.15   REW	Avoid icc compiler warnings.
*   15/01/08 1.14   REW	Avoid icc compiler warnings.
*   12/10/07 1.13   DGJ	Store the buffer used to create the PIM
*			Support GetThread, GetBuffer
*   14/09/07 1.12   DGJ	Clear PIM, move activity state to SPxObj
*   12/09/07 1.11   REW	Add NotifyChanges().
*   01/09/07 1.10   DGJ	Default args to PIM constructor
*   26/08/07 1.9    DGJ Add GetAzimuthOffset() to PIM.
*			Add GetAzimuthSubSample() member to PIM, 
*   20/08/07 1.8    DGJ	Add ManagerHasGone function that pim manager
*			can call when it is being deleted.
*   07/07/07 1.7    DGJ	Support control for making PIM inactive
*   29/06/07 1.6    DGJ	Add SetParameter. Derive from SPxObj.
*   28/06/07 1.5    REW	Add SetAzimuthOffset() and EnableFill().
*   12/06/07 1.4    REW	PIM creates its own process manager.
*   07/06/07 1.3    REW	Formatting, comments, unique prefixes etc.
*			Use thread objects, event objects etc.
*			SPxPimManager now a separate object etc.
*   24/05/07 1.2    REW	Added header, changed to SPx etc.
*   14/05/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_PIM_H
#define _SPX_PIM_H

/*
 * Other headers required.
 */
/* We need SPXxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxError.h"

/* We also need the RIB interface and PIM manager. */
#include "SPxLibData/SPxRib.h"
#include "SPxLibData/SPxPimManager.h"

/* Nav Data */
#include "SPxLibData/SPxNavData.h"

/* We need process managers. */
#include "SPxLibData/SPxProcMan.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Combination modes.
 */
#define SPX_COMBINE_PEAK        0 /* 8-bit and 12-bit. */
#define SPX_COMBINE_MIN         1 /* 8-bit and 12-bit. */
#define SPX_COMBINE_SMOOTH      2 /* 8-bit and 12-bit. */
#define SPX_COMBINE_START_USER  3 /* Index where user combine modes start. */

/* Number of 8-bit combination tables we'll support. */
#define SPX_PIM_MAX_NUM_8BIT_COMBINE_TABLES 10

/* Number of 12-bit combination tables we'll support. */
#define SPX_PIM_MAX_NUM_12BIT_COMBINE_TABLES 3

/* For backward compatibility */
//#defiine SPX_PIM_MAX_NUM_COMBINE_TABLES SPX_PIM_MAX_NUM_8BIT_COMBINE_TABLES

/* Maximum level for interference suppression. */
#define	SPX_PIM_IF_MAX_LEVEL	3

/*
 * Flags for the pimFlags field in the RIB return header structure.
 */
#define	SPX_PIM_FLAGS_REPEAT	0x01	/* Return is repeat of previous one */
#define	SPX_PIM_FLAGS_BLACKFILL	0x02	/* Return is black-filled */
#define	SPX_PIM_FLAGS_AZI_CORR	0x04	/* Return is azimuth-correlated */
#define SPX_PIM_FLAGS_AZI_REF_SET 0x08	/* Return has valid azi ref mode */
#define SPX_PIM_FLAGS_AZI_REF	0x10	/* Azi ref (if valid): 0=North 1=Hdg/Course */
#define SPX_PIM_FLAGS_AZI_REF_ORIG 0x20	/* Original ref before compensation */
#define	SPX_PIM_FLAGS_HEADING_SET 0x40	/* Return has valid 'heading' */
#define SPX_PIM_FLAGS_AZI_REF_COURSE 0x80 /* Azi ref (if valid): 0=Heading 1=Course */

/*
 * Debug flags.
 */
#define SPX_PIM_DEBUG_INPUT_SUMMARY	0x00000001 /* Summary */
#define SPX_PIM_DEBUG_UPDATES   	0x00000002 /* Update details */
#define SPX_PIM_DEBUG_UPDATES_HEADER	0x00000004 /* Extra Header details */
#define SPX_PIM_DEBUG_UPDATE_STATE	0x00000008 /* PIM state on Update */
#define SPX_PIM_DEBUG_NAV_DATA		0x00000010 /* Navigation data */
#define SPX_PIM_DEBUG_VIDEO		0x00000020 /* Dump video data */
#define SPX_PIM_DEBUG_VIDEO_QUICKLOOK	0x00000040 /* Quick look of video */

/* The default value for the maximum number of azimuths that can
 * be repeated.
 */
#define DEFAULT_AZIMUTH_REPEAT 10

/* Define the magic value for this object. */
#define PIM_MAGIC 0x12345678


/*********************************************************************
*
*   Macros
*
**********************************************************************/

/* Define macros that report the azimuth update interval as either
 * input azimuths or output azimuths.
 */
#define SPX_PIM_INPUT(a) (a)
#define SPX_PIM_OUTPUT(a) (-(a))

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* 8-bit LUT */
typedef unsigned char SPxCombineTable8[SPX_UINT8_MAXVAL+1][SPX_UINT8_MAXVAL+1];
#define SPxCombineTable SPxCombineTable8 /* For backward compatibility */

/* 12-bit LUT */
typedef UINT16 SPxCombineTable12[SPX_UINT12_MAXVAL+1][SPX_UINT12_MAXVAL+1];

typedef int SPxPIMupdateInterval;

typedef enum {
        /* The PIM maintains a 2D store */
	SPX_PIM_POLAR_STORE = 1
 
        /* The PIM maintains a 1D store - no azimuth correlation */
	// SPX_PIM_LINE_STORE = 2,
} SPxPIMtypeCode;

/* PIM azimuth referencing modes */
typedef enum
{
    /* Do no azimuth referencing. */
    SPX_AZI_REF_NONE = 0,

    /* Reference the output azimuth to North assuming the input azimuth
     * is North referenced.
     */
    SPX_AZI_REF_NORTH_FROM_NORTH = 1,

    /* Reference the output azimuth to North assuming the input azimuth
     * is Heading referenced.
     */
    SPX_AZI_REF_NORTH_FROM_HEADING = 2,

    /* Reference the output azimuth to Heading assuming the input azimuth
     * is North referenced.
     */
    SPX_AZI_REF_HEADING_FROM_NORTH = 3,

    /* Reference the output azimuth to Heading assuming the input azimuth
     * is Heading referenced.
     */
    SPX_AZI_REF_HEADING_FROM_HEADING = 4,

    /* Reference the output azimuth to North using the information in the
     * header to define the input referencing.
     */
    SPX_AZI_REF_NORTH_FROM_INPUT = 5,

    /* Reference the output azimuth to Heading using the information in the
     * header to define the input referencing.
     */
    SPX_AZI_REF_HEADING_FROM_INPUT = 6,
    
    /* Reference the output azimuth to North assuming the input azimuth
     * is Course referenced.
     */
    SPX_AZI_REF_NORTH_FROM_COURSE = 7,
    
    /* Reference the output azimuth to Heading assuming the input azimuth
     * is Course referenced.
     */
    SPX_AZI_REF_HEADING_FROM_COURSE = 8,

    /* Reference the output azimuth to Course assuming the input azimuth
     * is North referenced.
     */
    SPX_AZI_REF_COURSE_FROM_NORTH = 9,

    /* Reference the output azimuth to Course assuming the input azimuth
     * is Heading referenced.
     */
    SPX_AZI_REF_COURSE_FROM_HEADING = 10,

    /* Reference the output azimuth to Course using the information in the
     * header to define the input referencing.
     */
    SPX_AZI_REF_COURSE_FROM_INPUT = 11,

    /* Reference the output azimuth to Course assuming the input azimuth
     * is Course referenced.
     */
    SPX_AZI_REF_COURSE_FROM_COURSE = 12

} SPxAziRefMode;

/*
 * Azimuth combination mode
 */
typedef enum
{
    /* When the azimuth combine mode is SPX_PIM_AZI_NOCOMBINE, no
     * combination of returns is done. A return is only entered to
     * the polar-store if it is at a different azimuth to the previous
     * return. So for a sequence of returns at the same azimuth, only the
     * first return gets written to the store - the rest are ignored.
     */
    SPX_PIM_AZI_NOCOMBINE = 1,
    SPX_PIM_AZI_SUBSAMPLE = 1,

    /* When the azimuth combine mode is SPX_PIM_AZI_OVERWRITE, every
     * return is written into the polar-store, but no combination occurs.
     * This is generally wasteful because there is no combination of
     * returns going on. When the azimuth advances, it is just the
     * last return at the azimuth that has been recorded.
     */
    SPX_PIM_AZI_OVERWRITE = 2,

    SPX_PIM_AZI_PEAK = 3,
    SPX_PIM_AZI_MIN  = 4,
    SPX_PIM_AZI_SMOOTH = 5,
    SPX_PIM_AZI_LAST_ENTRY = 5
    /* ADD TO THE STRING SPxPIMazimuthCombineStr[] IF YOU ADD TO THESE MODES */
} SPxPIMazimuthCombine;


/*
 * Range combination mode.
 */
typedef enum
{
    SPX_PIM_RAN_SUBSAMPLE = 1,
    SPX_PIM_RAN_PEAK = 2,
    SPX_PIM_RAN_MIN = 3,
    SPX_PIM_RAN_SMOOTH = 4,
    SPX_PIM_RAN_LAST_ENTRY = 5
    /* ADD TO THE STRING SPxPIMrangeCombineStr[] IF YOU ADD TO THESE MODES */
} SPxPIMrangeCombine;

/* Forward define some things we need. */
class SPxRunProcess;
class SPxPacketDecoder;
class SPxThread;

/*
 * Define the PIM class.
 */
class SPxPIM :public SPxObj
{
private:
	/*
	 * Private fields.
	 */
	SPxRIB *m_buffer;	/* RIB used to create the PIM (may be NULL). */
	int m_magic;		/* Magic Number for consistency checking. */
	SPxPIMtypeCode m_type;	/* The type of the PIM */
	unsigned m_rangeDim;	/* Range dimension of the polar-store. */
	unsigned int m_lastNominalLength; /* Last nominal length used */
	unsigned int m_lastBytesPerSample; /* Last bytes per sample used */
	REAL32 m_lastStartRange;    /* Last start range. */
	REAL32 m_lastEndRange;	    /* Last end range. */
	UINT8 m_lastSourceType;	    /* Last source type. */
	UINT8 m_lastSourceCode;	    /* Last source code. */
	UCHAR m_lastScanMode;	    /* Last scan mode. */

	UINT16 m_count;		/* The count reflects the count field of
				 * the data written into the RI buffer
				 */
	SPxScanMode m_scanMode;	/* Current scan mode of radar source */
	int m_justChangedSourceType;	/* Did the source type just change? */
	SPxPIMrangeCombine m_rangeSubSample;	/* The method of range data
						 * reduction.
						 */
	SPxPIMazimuthCombine m_azimuthSubSample;/* The method of azimuth data
						 * reduction.
						 */
	int m_azimuthOffset;			/* The azimuth offset */
	int m_azimuthOffsetRemainder;		/* High-res remainder */
	int m_useHighResAziOffset;		/* Use high-res offset? */
	unsigned int m_maxAzimuthDim;		/* Maximum support azimuth
						 * dimension of the polar-store.
						 */
	unsigned int m_azimuthDim;		/* Azimuth dimension of the
						 * polar-store.
						 */
	unsigned int m_azimuthRepeat;		/* Is azimuth repeat needed
						 * and if so how much.
						 */
	int m_lastUpdatedAzimuth;		/* The last azimuth that was
						 * updated (0..64k).
						 */
	int m_firstUpdatedAzimuthInPimStore;	/* The first azimuth in
						 * the pim store that we have
						 * updated since the last
						 * report.
						 */
	int m_lastUpdatedAzimuthInPimStore;	/* Most recent azimuth
						 * in the pim store that we
						 * have updated since the
						 * last report.
						 */
	int m_fillEnable;			/* Is azimuth and black filling
						 * enabled for the PIM. Set to
						 * false for random access.
						 */
	SPxReturn **m_storeMemory;	/* Main polar-store memory */
	SPxReturn *m_workSpace;		/* A one-return workspace */
	SPxReturn *m_ifWorkSpace;	/* For interference suppression */
	SPxReturn *m_ifReturns[SPX_PIM_IF_MAX_LEVEL];	/* Buffers for
							 * interference
							 * suppression.
							 */
	int m_ifLastIdx;		/* Index of last buffer filled */
	unsigned int m_ifLevel;		/* Interference suppression level */
	int m_updateInterval;		/* The interval, measured in source
					 * azimuths, between updates.
					 */
	unsigned int m_constantAziLimit; /* Limit of number of spokes
					 * combined at the same position
					 * if azimuth not changing in output
					 * reporting mode (0 to disable).
					 */
	unsigned m_inputAzimuthCounter;	/* A counter that is used to record
					 * the number of incoming returns.
					 */
	unsigned m_maxAzimuthJumpToClear;	/* The maximum number of
						 * azimuths in the PIM that
						 * we can clear across in the
						 * situation that an azimuth
						 * jumps.
						 */

	double m_PRFHz;			/* Instantaneous PRF in Hertz. */
	double m_averagePRFHz;		/* Average PRF in Hertz. */
	int m_averagePRFReset;		/* Flag set to reset average PRF. */

	double m_averagePeriodSecs;	/* Average rotation period
					 * in seconds.
					 */

	int m_averagePeriodReset;	/* Flag set to reset average period. */

	UINT32 m_northCrossTimestamp;	/* Time of last north 
					 * crossing.
					 */

	UINT16 m_lastInputAzimuth;	/* Last azimuth received
					 * into the PIM before
					 * offset, etc, is applied.
					 */

	SPxProcessManager *m_processManager;	/* The process manager for
						 * this PIM.
						 */
	SPxAtomic<SPxRunProcess*> m_outputProcess;/* The process that attaches
						 * to this PIM.
						 */
	SPxPimManager *m_pimManager;		/* The PIM manager that looks
						 * after this PIM.
						 */
	SPxPIM *m_next;				/* Next PIM in linked-list */

	SPxNavData *m_navData;			/* Nav Data object for North Offset */
	SPxAziRefMode m_azimuthRefMode;		/* The mode for azimuth referencing */
	int m_extrapolateHeading;		/* Extrapolate heading based on change rate? */
	int m_correctPitchRoll;			/* Correct pitch/roll using nav data? */
	int m_useReturnHeading;			/* Use heading from radar returns (instead of nav data)? */

	unsigned int m_maxBytesPerSample;	/* Maximum number of bytes per sample
						 * supported by this PIM.
						 */
	UINT32 m_lastUpdateErrorTimeMsecs;	/* Time last Update() error was reported. */
	mutable SPxCriticalSection m_mutex;	/* Mutex used to protect internal data. */
	mutable SPxCriticalSection m_mutexProps;    /* Mutex used to protect properties. */

	/* Sensor-specific data. */
	unsigned char *m_lastSensorDataBuf;	/* Buffer for data. */
	unsigned int m_lastSensorDataBufSizeBytes;	/* Allocated size */
	unsigned int m_lastSensorDataBufPresentBytes;	/* Data present */
	SPxReturnHeader m_lastSensorDataHdr;	/* Copy of last header */

	/* Loading from file. */
	int m_loadFromFileInvoked;		/* Was callback invoked? */
	int m_loadFromFileSucceeded;		/* Did callback succeed? */

	/* Debug. */
	static FILE *m_logFile;
	FILE *m_dataLogFile;			/* Log file for video data */
	UINT32 m_debug;
	
	/*
	 * Private functions.
	 */
	void WritePIMdataToDebug(SPxReturn *spxReturn);
	void UpdateProperties(SPxReturn *spxReturn);
	void CalculatePeriod(UINT16 azimuth);
	void CalculatePRF(UINT16 timeInterval);
	void CalculateAveragePRF(void);
	const SPxReturnHeader *GetLastReturnHeaderInternal(void) const;

	/* For loading PIM from file. */
	static void staticLoadFromFileCallback(SPxPacketDecoder *decoder,
						void *userArg,
						UINT32 packetType,
						struct sockaddr_in *fromAddr,
						SPxTime_t *timestamp,
						const unsigned char *payload,
						unsigned int numBytes);
	void internalLoadFromFileCallback(const unsigned char *payload,
						unsigned int numBytes);

	/* For interference suppression. */
	SPxErrorCode ifCombine(void *dstPtr, void *srcPtr,
				unsigned int dstNumSamples,
				unsigned int srcNumSamples,
				int packing);

public:
	/*
	 * Public fields.
	 */

	/*
	 * Public functions.
	 */
	/* Constructor and destructor. */
	virtual ~SPxPIM(void);
	SPxPIM(SPxRIB *buffer, 
	       unsigned rangeDim, unsigned azimuthDim, 
	       SPxPIMrangeCombine rangeSubSample=SPX_PIM_RAN_PEAK, 
	       SPxPIMazimuthCombine azimuthSubSample=SPX_PIM_AZI_PEAK,
	       SPxPIMupdateInterval updateInterval=SPX_PIM_OUTPUT(1),
	       unsigned int maxBytesPerSample=1);

	/* Dimension retrieval functions. */
	int GetMaxAzimuthDim(void)
	{
	    return m_maxAzimuthDim;
	}
	void SetAzimuthDim(unsigned int azimuthDim);
	int GetAzimuthDim(void)
	{
	    return m_azimuthDim;
	}
	int GetRangeDim(void)
	{
	    return m_rangeDim;
	}
	unsigned int GetMaxBytesPerSample(void)
	{
	    return m_maxBytesPerSample;
	}

	/* Calculate Video Centroid. */
	int CalculateVideoCentroid(double startRmetres, double startAdegrees, double endRmetres, double endAdegrees,
	    double minDensity,
	    double *cr, double *ca, int *strength, double *density);
	int CalculateVideoCentroid(int startR, int startA, int endR, int endA, 
	    double minDensity, 
	    double *cr, double *ca, int *strength, double *density);

	/* Update last Pim azimuth. */
	void UpdateAzimuthInPimStore(int lastUpdatedAzimuthInPimStore);

	/* Static public functions for log file support. */
	static void SetLogFile(FILE *logFile)	{ m_logFile = logFile; }
	void SetDataLogFile(FILE *dataLogFile)	{m_dataLogFile = dataLogFile;}
	static FILE *GetLogFile(void)		{ return(m_logFile); }
	void SetDebug(UINT32 debug)
	{
	    m_debug = debug;
	    if( GetLogFile() == NULL )
	    {
		SetLogFile(stdout);
	    }
	}

	/* Retrieve last updated azimuth */
	int GetLastInputAzimuth(void) {return m_lastUpdatedAzimuth;}
	int GetLastUpdatedAzimuth(void) {return m_lastUpdatedAzimuthInPimStore;}
	unsigned int GetLastNominalLength(void) {return m_lastNominalLength;}

	/* Convenience functions for accessing stats etc. */
	SPxReturnHeader *GetLastReturnHeader(void);
	SPxErrorCode GetLastReturnHeader(SPxReturnHeader *rtnHdr) const;
	double GetPRF(void) const;
	double GetAveragePRF(void) const;
	void ResetAveragePRF(void);
	double GetAveragePeriod(void) const;
	void ResetAveragePeriod(void);
	REAL32 GetStartRange(void) const;
	REAL32 GetEndRange(void) const;
	UINT8 GetSourceType(void) const;
	UINT8 GetSourceCode(void) const;
	UCHAR GetScanMode(void) const;

	/* Linked list handling functions. */
	void SetNext(SPxPIM *next)
	{
	    m_next = next;
	}
	SPxPIM *GetNext(void)
	{
	    return m_next;
	}

	/* Interference suppression. */
	SPxErrorCode SetInterferenceLevel(unsigned int level)
	{
	    if( level > SPX_PIM_IF_MAX_LEVEL )
	    {
		level = SPX_PIM_IF_MAX_LEVEL;
	    }
	    m_ifLevel = level;
	    return(SPX_NO_ERROR);
	}
	unsigned int GetInterferenceLevel(void)	{ return(m_ifLevel); }

	/* Sub-sampling control. */
	void SetAzimuthSubsample(SPxPIMazimuthCombine mode)
	{
	    m_azimuthSubSample = mode;
	}
	void SetRangeSubsample(SPxPIMrangeCombine mode)
	{
	    m_rangeSubSample = mode;
	}
	SPxPIMrangeCombine GetRangeSubsample(void)
	{
	    return m_rangeSubSample;
	}
	SPxPIMazimuthCombine GetAzimuthSubsample(void)
	{
	    return m_azimuthSubSample;
	}

	/* Legacy function with inconsistent capitalisation. */
	SPxPIMazimuthCombine GetAzimuthSubSample(void)
	{
	    return GetAzimuthSubsample();
	}

	/* Return printable strings for azimuth/range correlation modes. */
	const char *GetAzimuthSubSampleString(void);
	const char *GetRangeSubSampleString(void);

	/* Other functions. */
	void Clear(UINT8 pimFlagsToClear=0);
	void SetAzimuthOffset(int offset); 
	int GetAzimuthOffset(void) {return m_azimuthOffset;}
	SPxRIB *GetBuffer(void) {return m_buffer;}
	SPxThread* GetThread(void)
	{
	    if (m_buffer)
	    {
		return m_buffer->GetThread();
	    }
	    else
	    {
		return NULL;
	    }
	}
	void SetAzimuthClearLimit(unsigned int azimuthClearLimit);
	unsigned int GetAzimuthClearLimit(void) const
					{ return(m_maxAzimuthJumpToClear); }
	void SetAzimuthRepeat(unsigned int azimuthRepeat);
	unsigned int GetAzimuthRepeat(void) const { return m_azimuthRepeat; }
	void ManagerHasGone(void) {m_pimManager = 0;}
	void SetOutputProcess(SPxRunProcess *p)
	{
	    m_outputProcess = p;
	}
	SPxErrorCode CopyFromPIM(SPxPIM *src);
	SPxErrorCode CompareWithPIM(SPxPIM *other);

	/* High-res azimuth offsets, if enabled and supported by processes. */
	void SetUseHighResAziOffset(int enable);
	int GetUseHighResAziOffset(void) { return(m_useHighResAziOffset); }
	int GetAzimuthOffsetRemainder(void);

	/* Save and restore. */
	SPxErrorCode SaveToFile(const char *filename);
	SPxErrorCode LoadFromFile(const char *filename);

	/* Associate Nav Data when we need to apply a North reference. */
	void NorthReference(SPxNavData*, SPxAziRefMode mode);
	void SetAziRefMode(SPxAziRefMode mode) { m_azimuthRefMode = mode; }
	SPxAziRefMode GetAziRefMode(void) { return m_azimuthRefMode; }
	SPxNavData *GetNavData(void) { return m_navData; }
	void SetUseReturnHeading(int useReturnHeading) { m_useReturnHeading = useReturnHeading; }

	void SetExtrapolateHeading(int enable) { m_extrapolateHeading = enable; }
	int GetExtrapolateHeading(void) const { return m_extrapolateHeading; }

	void SetCorrectPitchRoll(int enable) { m_correctPitchRoll = enable; }
	int GetCorrectPitchRoll(void) const { return m_correctPitchRoll; }

	void SetUpdateInterval(int interval);
	void SetConstantAziLimit(unsigned int limit)
	{
	    m_constantAziLimit = limit;
	}
	unsigned int GetConstantAziLimit(void)
	{
	    return(m_constantAziLimit);
	}
	SPxProcessManager *GetProcessManager(void)
	{
	    return( m_processManager );
	}
	int GetUpdateInterval(void)
	{
	    return m_updateInterval;
	}
	void SetCount(UINT16 count)
	{
	    m_count = count;
	}

	SPxRunProcess *GetOutputProcess(void)
	{
	    return m_outputProcess;
	}
	SPxReturn *GetWorkspace(void)
	{
	    return m_workSpace;
	}
	SPxReturn **GetStoreMemory(void)
	{
	    return m_storeMemory;
	}
	SPxErrorCode GetReturn(int azimuth, SPxReturn *rtn,
			       unsigned int *totalBytes) const;

	void EnableFill(int status) {m_fillEnable = status;}
	int GetEnableFill(void) const { return(m_fillEnable); }

	void AzimuthFill(unsigned int azimuthNumberInPimStore,
                         SPxReturn *spxReturn, 
                         unsigned int nominalLength,
                         unsigned int returnLength,
                         unsigned int bytesPerSample);

	void RangeCorrelation(SPxReturn *newReturn,
                              unsigned int nominalLength,
                              unsigned int returnLength,
                              unsigned int bytesPerSample);

	int AzimuthCorrelation(SPxReturn *spxReturn, 
                               unsigned int nominalLength,
                               unsigned int returnLength,
                               unsigned int bytesPerSample);

	int CheckNeedAzimuthFill(int azimuthNumberInPimStore);

	void WriteAndBlackFillReturn(unsigned int azimuthNumberInPimStore,
                                     unsigned char *newReturn, 
                                     unsigned int nominalLength,
                                     unsigned int returnLength,
                                     unsigned int bytesPerSample);

	void CombineReturn(unsigned int azimuthNumberInPimStore,
                           unsigned char *newReturn,
                           unsigned int nominalLength, 
                           unsigned int returnLength,
                           unsigned int bytesPerSample,
                           int combineMode);

        void CorrelateSamples(void *dstPtr, 
                              void *srcPtr,
                              unsigned int dstBufLenSamples,
                              unsigned int srcBufLenSamples,
                              int packing,
                              int combineMode,
                              unsigned int numSrcSamples=0);

	int SetParameter(char *parameterName, char *parameterValue);
	int GetParameter(char *parameterName, char *valueBuf, int bufLen);

	/* Function to update the PIM with new data. */
	int Update(SPxReturn *spxReturn);

	/* Function to tell the PIM something in the source has changed. */
	int NotifyChanges(UINT32 flags);

	/* Function to give the PIM some sensor-specific data. */
	SPxErrorCode SetSensorSpecificData(SPxReturnHeader *spxReturnHdr,
						unsigned char *data);

	/* Function to retrieve the most recent sensor-specific data into
	 * the given buffer, returning the number of bytes present.  A copy
	 * of the corresponding return header can optionally be retrieved
	 * too.
	 */
	unsigned int GetLastSensorSpecificData(unsigned char *buffer,
						unsigned int bufSizeBytes,
						SPxReturnHeader *hdr=NULL);

	/*
	 * Public static functions.
	 */

	/* Shutdown function */
	static void SPxPIMShutdown(void);

}; /* SPxPIM */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Method for setting up a user-defined combination table. */
extern SPxErrorCode SPxSetUserCombineTable(int index,
					unsigned char (*userTable)[256][256]);

#endif /* _SPX_PIM_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
