/*********************************************************************
*
* (c) Copyright 2007 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for RIB class.
*
* Revision Control:
*   24/04/19 v1.54   AGC	Add SPX_RIB_PACKING_RAW8_11111111.
*				Add SPX_PACKING_CHAN_DIG4..7.
*
* Previous Changes:
*   12/04/19 1.53   SP 	Add SPX_SCAN_MODE_AUTO.
*   12/02/19 1.52   AGC	Coalesce RIB writes.
*   23/10/17 1.51   AGC	Add SPX_RIB_PACKING_RAW10.
*   02/09/16 1.50   AGC	Use atomic variable for bytes to be read.
*   03/03/16 1.49   AGC	Add SPxGetPackingNumBytes().
*			Add SPxGetPackingMaxVal().
*   14/01/16 1.48   REW	Add numTriggers to return header.
*   07/01/16 1.47   AGC	Use const for receiving data/headers.
*   23/12/15 1.46   AGC	Extend packing shift/mask functions.
*			Add SPxGetPackingNumAnalogueChannels().
*			Add SPxGetPackingNumDigitalBits().
*   25/03/15 1.45   REW	Add SPX_RIB_SOURCETYPE_HTTP.
*   08/11/13 1.44   REW	Add SPX_RIB_PACKING_RAW8_7_1, 6_11 etc. modes.
*   04/10/13 1.43   AGC	Simplify headers.
*   22/08/13 1.42   REW	Add SPX_RIB_PACKING_RAW4_4.
*   19/07/13 1.41   AGC	Rename UPSIDE_DOWN flag to ANTI_CLOCKWISE.
*   17/06/13 1.40   REW	Add SPX_RIB_SOURCETYPE_UNKNOWN.
*   06/06/13 1.39   AGC	Add SPX_RIB_DATA_FLAG_UPSIDE_DOWN.
*   22/02/13 1.38   REW	Move PRF monitors to new process.
*   21/02/13 1.37   REW	Support Get/SetParameter() and PRF monitors.
*   21/01/13 1.36   REW	Add SPxGetPackingShift().
*   11/01/13 1.35   REW	Add SPxGetPackingExtraBitsMask()
*			and SPxGetPackingNormalBitsMask().
*   12/12/12 1.34   AGC	Add SPX_RIB_SOURCETYPE_SIMULATOR.
*   25/11/11 1.33   REW	Add SPX_RIB_CHANGED_SENSORDATA.
*   10/11/11 1.32   REW	Add SPX_RIB_HEADER_MAGIC1_SENSOR.
*   26/10/11 1.31   SP 	Fix typo in comment.
*   30/08/11 1.30   SP 	Add SPxGetPackingSamplesPerByte().
*   19/08/11 1.29   SP 	Add more 16-bit packing options.
*                       Add dataFlags field to header (was reserved35).
*                       Add SPX_RIB_DATA_FLAG_... defines.
*                       Add SPxGetPackingBytesPerSample().
*   04/04/11 1.28   REW	Add more PACKING options.
*   15/09/10 1.27   REW	Make destructor virtual.
*   04/12/09 1.26   REW	Add heading to SPxReturnHeader.
*   16/03/09 1.25   REW	Clarify comment for endRange field.
*   06/03/09 1.24   REW	Add SPX_SCAN_MODE_NO_AZI_DATA.
*   10/02/09 1.23   REW	Add names to structs. Byte-swapping functions
*			moved to SPxByteSwap module.
*   24/11/08 1.22   REW	Add SPX_RIB_SOURCETYPE_HARDWARE and aliases.
*   10/10/08 1.21   REW	Add SPX_RIB_SOURCETYPE_EXTERNAL.
*   31/07/08 1.20   REW	Make headerSize/sourceType/sourceCode 8-bit.
*			Add pimFlags to return header structure.
*   14/03/08 1.19   REW	Add SPX_RIB_SOURCETYPE_HPX100.
*   10/12/07 1.18   REW	Add SPX_RIB_CHANGED_STATE.
*   05/12/07 1.17   DGJ	Add GetSize, GetAllocationType()
*   29/11/07 1.16   REW	Add m_numWrites.
*   14/11/07 1.15   REW	Add SPX_SCAN_MODE_... definitions and field.
*   22/10/07 1.14   REW	Add SPX_RIB_SOURCETYPE_SYNC_COMBINE.
*   16/10/07 1.13   REW	Fix missing semicolon in last checkin.
*   12/10/07 1.12   DGJ	Allow reading of pointer positions (for debug)
*   22/09/07 1.11   DGJ	Support GetThread()
*   13/09/07 1.10   REW	Add SPX_RIB_CHANGED_INIT.
*   12/09/07 1.9    REW	Add SetChangeFlags(), GetAndClearChangeFlags().
*   20/08/07 1.8    DGJ	Provide default NULL for memory pointer in
*			constructor, and default 0 Read/Write flags.
*   26/07/07 1.7    REW	Add totalSize to SPxReturnHeader and rename
*			the previous size field to radarVideoSize.
*   25/07/07 1.6    REW	Add overloaded Write() function for rtn hdr.
*			Add SOURCETYPE and PACKING definitions.
*   29/06/07 1.5    DGJ	Derive from SPxObj
*   05/06/07 1.4    REW	Tidy up, comments, unique prefixes etc.
*   04/06/07 1.3    DGJ	Use SPxCommon.h for REAL32.
*			Add m_bufferSpaceIsAllocatedByClass.
*   24/05/07 1.2    REW	Added header, changed to SPx etc.
*   14/05/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_BUFFER_H
#define _SPX_BUFFER_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxAtomic.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxEvents.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Define magic values for the RIB. */
#define SPX_RIB_HEADER_MAGIC1		0xC0DE5837	/* For radar data */
#define SPX_RIB_HEADER_MAGIC1_SENSOR	0xC0DE5E45	/* For sensor data */
#define SPX_RIB_HEADER_MAGIC2		0xC0DE6948	/* End of header */

/* Define the protocol version currently being used in this library. */
#define SPX_RIB_VIDEO_PROTOCOL_USED 1

/* Define options for how to read from the buffer. */
#define SPX_RIB_READ_ALL_OR_NOTHING 0x0001

/*
 * Define the values for the SPxReturnHeader->sourceType field.
 */
#define	SPX_RIB_SOURCETYPE_NETWORK	0
#define	SPX_RIB_SOURCETYPE_TEST		1
#define	SPX_RIB_SOURCETYPE_REPLAY	2
#define	SPX_RIB_SOURCETYPE_SCENARIO	3
#define	SPX_RIB_SOURCETYPE_HARDWARE	4
#define	SPX_RIB_SOURCETYPE_EXTERNAL	5
#define SPX_RIB_SOURCETYPE_SIMULATOR	6
#define	SPX_RIB_SOURCETYPE_SYNC_COMBINE	10
#define	SPX_RIB_SOURCETYPE_HTTP		20
#define	SPX_RIB_SOURCETYPE_UNKNOWN	255

/* Define aliases for the hardware sources, all generic interfaces. */
#define	SPX_RIB_SOURCETYPE_HPX100	SPX_RIB_SOURCETYPE_HARDWARE
#define	SPX_RIB_SOURCETYPE_HPX150	SPX_RIB_SOURCETYPE_HARDWARE
#define	SPX_RIB_SOURCETYPE_HPX200	SPX_RIB_SOURCETYPE_HARDWARE


/*
 * Define the values for the SPxReturnHeader->packing field, noting that
 * it's a single byte field so the values should be in range 0 to 255.
 *
 * Be sure to update SPxGetPackingBytesPerSample() or
 * SPxGetPackingSamplesPerByte() when adding new packing modes.
 *
 * NB. The last two (INVALID and OPTIMAL) are special values used within
 * the code but never put in real headers.
 */

#define	SPX_RIB_PACKING_RAW8		0   /* 8 bit samples uncompressed */
#define	SPX_RIB_PACKING_RAW4		1   /* 4 bit samples tightly packed */
#define	SPX_RIB_PACKING_RAW2		2   /* 2 bit samples tightly packed */
#define	SPX_RIB_PACKING_RAW1		3   /* 1 bit samples tightly packed */
#define	SPX_RIB_PACKING_RAW4_1111	4   /* 4+1+1+1+1 bits per byte */
#define	SPX_RIB_PACKING_RAW5_111	5   /* 5+1+1+1 bits per byte */
#define	SPX_RIB_PACKING_RAW6_11		6   /* 6+1+1 bits per byte */
#define	SPX_RIB_PACKING_RAW7_1		7   /* 7+1 bits per byte */
#define	SPX_RIB_PACKING_RAW16		8   /* 16-bits, LS-byte first */
#define	SPX_RIB_PACKING_RAW4_4		9   /* 4-bits A (MSB), 4-bits B (LSB)*/
#define	SPX_RIB_PACKING_ORC		10  /* RAW8 ORC compressed */
#define	SPX_RIB_PACKING_ZLIB		20  /* RAW8 ZLib compressed */
#define SPX_RIB_PACKING_RAW10		25  /* 10-bit sample in MSBs */
#define SPX_RIB_PACKING_RAW12		30  /* 12-bit sample in MSBs */
#define SPX_RIB_PACKING_RAW12_1111	31  /* 12+1+1+1+1 */
#define SPX_RIB_PACKING_RAW8_8		40  /* 8-bits A plus 8-bits B */
#define SPX_RIB_PACKING_RAW8_7_1	41  /* 8-bits + 7-bits + 1-bit */
#define SPX_RIB_PACKING_RAW8_6_11	42  /* 8-bits + 6-bits + 1+1-bit */
#define SPX_RIB_PACKING_RAW8_5_111	43  /* 8-bits + 5-bits + 1+1+1-bit */
#define SPX_RIB_PACKING_RAW8_4_1111	44  /* 8-bits + 4-bits + 1+1+1+1-bit */
#define SPX_RIB_PACKING_RAW8_11111111	45  /* 8-bits + 1+1+1+1+1+1+1+1-bit */
/* See note above regarding the next two values. */
#define	SPX_RIB_PACKING_OPTIMAL		254 /* Never put in headers */
#define	SPX_RIB_PACKING_INVALID		255 /* Invalid */

/* Used in compression functions to mean uncompressed data. */
#define	SPX_RIB_PACKING_RAW SPX_RIB_PACKING_RAW8

/* Maximum number of bytes per sample that is supported. */
#define SPX_RIB_PACKING_SAMPLE_BYTES_MAX 2

/*
 * Define the bits for the NotifyChanges() flags.
 */
#define	SPX_RIB_CHANGED_INIT		0x00000001	/* Initialisation */
#define	SPX_RIB_CHANGED_TYPE		0x00000002	/* File/net/hw etc. */
#define	SPX_RIB_CHANGED_CONTENT		0x00000004	/* Filename, hw chan */
#define	SPX_RIB_CHANGED_DIMENSIONS	0x00000008	/* Num azis/samples */
#define	SPX_RIB_CHANGED_RANGE		0x00000010	/* Start/end range */
#define	SPX_RIB_CHANGED_TIMING		0x00000020	/* Period, PRF */
#define	SPX_RIB_CHANGED_SCANMODE	0x00000040	/* Random/rotate */
#define	SPX_RIB_CHANGED_STATE		0x00000080	/* Enabled/disabled */
#define	SPX_RIB_CHANGED_SENSORDATA	0x00000100	/* Sensor-specific */

/*
 * Define the values for the SPxReturnHeader->dataFlags field, noting that
 * it's a single byte field.
 */

/* Data endian order is a single bit (0). If the BIG_ENDIAN flag is not
 * set then data is assumed to be little-endian. For single-byte data 
 * this bit should be set to 0.
 */
#define SPX_RIB_DATA_FLAG_BIG_ENDIAN        0x01    /* Data is big-endian */

/* Data compression is a two bit field (2:1). Pre-V1.43 compression
 * was defined by the value set in the packing field. Data was always
 * decompressed as RAW8. To enable the original packing to be preserved
 * after decompression, the compression type is now defined by these 
 * flags. For backward compatibility, the packing type for RAW8
 * compressed data is still set to ORC or ZLIB, in addition to
 * setting these flags.
 */
#define SPX_RIB_DATA_FLAG_COMPRESSED_ORC    0x02    /* Data is ORC compressed */
#define SPX_RIB_DATA_FLAG_COMPRESSED_ZLIB   0x04    /* Data is ZLIB compressed */
#define SPX_RIB_DATA_FLAG_COMPRESSION_MASK  0x06    /* Compression mask */

/* Anti-clockwise is a single bit (0). If the ANTI_CLOCKWISE flag is not set,
 * then the radar is rotating clockwise (the default).
 * The ANTI_CLOCKWISE bit was previously named UPSIDE_DOWN.
 */
#define SPX_RIB_DATA_FLAG_ANTI_CLOCKWISE    0x08    /* Radar is rotating anticlockwise. */
#define SPX_RIB_DATA_FLAG_UPSIDE_DOWN       SPX_RIB_DATA_FLAG_ANTI_CLOCKWISE

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

/* Data structures to define the format of the radar video returns
 * received in the RI buffer.
 */

/*
 * Modes for azimuth scanning.
 *
 * NB: These are the values used in the scanMode field of return headers,
 *	which is only an 8-bit value so keep to the range 0 to 255.
 */
typedef enum
{
    /* Normal rotating situation. */
    SPX_SCAN_MODE_ROTATING = 0,

    /* Normal rotating situation but with compensated azimuths that
     * may appear to go backwards for short intervals.
     */
    SPX_SCAN_MODE_COMPENSATED = 1,

    /* Sector scan, in which the sweep scans alternately forwards and
     * backwards between limits.
     */
    SPX_SCAN_MODE_SECTOR = 2,

    /* Random scan, in which each azimuth has no specific relationship
     * to the previous.
     */
    SPX_SCAN_MODE_RANDOM = 3,

    /* Sector scan, in which the sweep scans forwards and then flies back
     * to repeat the scan in the sector (no backward scanning).
     */
    SPX_SCAN_MODE_SECTOR_FLY_BACK = 4,

    /* No azimuth data (e.g. for ascan displays only). */
    SPX_SCAN_MODE_NO_AZI_DATA = 5,

    /* Must not be set in return headers but can be used to tell
     * a source to auto detect the scan mode from the received
     * return azimuths.
     */
    SPX_SCAN_MODE_AUTO = 255

} SPxScanMode;

typedef enum SPxPackingChannel_tag
{
    SPX_PACKING_CHAN_A		= 0,
    SPX_PACKING_CHAN_B		= 1,
    SPX_PACKING_CHAN_C		= 2,
    SPX_PACKING_CHAN_D		= 3,
    SPX_PACKING_CHAN_E		= 4,
    SPX_PACKING_CHAN_F		= 5,
    SPX_PACKING_CHAN_G		= 6,
    SPX_PACKING_CHAN_H		= 7,
    SPX_PACKING_CHAN_DIG_0	= 32,
    SPX_PACKING_CHAN_DIG_1	= 33,
    SPX_PACKING_CHAN_DIG_2	= 34,
    SPX_PACKING_CHAN_DIG_3	= 35,
    SPX_PACKING_CHAN_DIG_4	= 36,
    SPX_PACKING_CHAN_DIG_5	= 37,
    SPX_PACKING_CHAN_DIG_6	= 38,
    SPX_PACKING_CHAN_DIG_7	= 39,
    SPX_PACKING_CHAN_DIG_ALL	= 127,
    SPX_PACKING_CHAN_ALL	= 255

} SPxPackingChannel;

/*
 * Structure for a return header. Note that fields names ending
 * in "size" define a number of bytes. Field names ending in "length"
 * define a number of samples. Depending on the "packing" type,
 * there may be more 1 or 2 bytes per sample.
 */
typedef struct SPxReturnHeader_tag
{
    /* Bytes 0 to 3 */
    UINT32 magic1;		// Constant magic number for integrity checks

    /* Bytes 4 to 7 */
    UINT16 protocolVersion;	// Protocol version number
    UINT8 reserved06;
    UINT8 headerSize;		// Size of header.

    /* Bytes 8 to 11 */
    UINT16 radarVideoSize;	// Size of encoded radar video data.
    UINT8 numTriggers;		// Number of triggers represented by this data
    UINT8 sourceType;		// Source where this return came from

    /* Bytes 12 to 15 */
    UINT8 reserved12;
    UINT8 sourceCode;		// Source-specific code.
    UINT16 count;		// Incrementing count from source

    /* Bytes 16 to 19 */
    UINT16 nominalLength;	// Nominal length of the return.
    UINT16 thisLength;		// Length of this return (<= nominal length)

    /* Bytes 20 to 23 */
    UINT16 azimuth;		// The azimuth (0..65536)
    UCHAR packing;		// Packing type
    UCHAR scanMode;		// One of the SPX_SCAN_MODE_... modes.

    /* Bytes 24 to 27 */
    UINT32 totalSize;		// headerSize + radarVideoSize + extraBytes

    /* Bytes 28 to 31 */
    UINT16 heading;		// Platform heading (0..65536 = 0..360degs)
    UINT16 reserved30;

    /* Bytes 32 to 35 */
    UINT16 timeInterval;	// Interval between returns (microseconds)
    UINT8 pimFlags;		// Bitwise combination of SPX_PIM_FLAGS_...
    UINT8 dataFlags;            // Bitwise combination of SPX_RIB_DATA_FLAGS_...

    /* Bytes 36 to 39 */
    REAL32 startRange;		// Start range in world units for first sample

    /* Bytes 40 to 43 */
    REAL32 endRange;		// End range in world units for nominalLength.

    /* Bytes 44 to 47 */
    UINT32 magic2;		// Magic number for integrity checking
} SPxReturnHeader;

/*
 * Structure for a return, comprising a header and a data block.
 */
typedef struct SPxReturn_tag
{
    SPxReturnHeader header;
    unsigned char data[256];	/* TODO: rename and resize this field */
} SPxReturn;


/* Forward declarations. */
class SPxPimManager;
class SPxThread;

/*
 * Radar Interface Buffer (RIB).
 */
class SPxRIB :public SPxObj
{
private:
    /*
     * Private fields.
     */
    /* The buffer used for storing the data. */
    unsigned char *m_memory;	/* The start address of the buffer memory */
    int m_size;			/* The size of the buffer in bytes */
    int m_bufferSpaceIsAllocatedByClass;    /* Non-zero if buffer space is
					     * allocated by class and is
					     * therefore released on
					     * delete.
					     */

    /* The pointers to the start and end of the data. */
    int m_writePtr;		/* The write pointer (index/byte offset) */
    int m_readPtr;		/* The read pointer (index/byte offset) */
    SPxAtomic<int> m_bytesToBeRead;	/* The number of bytes waiting to be read */

    /* Size in bytes of returns being written to the buffer. */
    int m_returnLength;

    /* Synchronisation objects. */
    SPxEvent m_eventObject;			/* To signal the PIMs */
    unsigned int m_numWrites;			/* Num writes since event */
    SPxCriticalSection m_criticalSection;	/* For single thread access */

    /* The manager for the various PIMs hanging off this RIB. */
    SPxPimManager *m_pimManager;

    /* Flags of things that may have changed (SPX_RIB_CHANGED_...). */
    UINT32 m_changedFlags;

    unsigned int m_numCores;
    static SPxAtomic<unsigned int> m_numRibs;

    /*
     * Private functions.
     */
    /* Sanity check. */
    SPxErrorCode sanityCheck(void);

    /* Internal function used by both Write()'s. */
    SPxErrorCode writeInternal(const unsigned char *data, int numBytes, int flags);

public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRIB(int bufferSize, unsigned char *bufferMemory=NULL);
    virtual ~SPxRIB();

    /* Read and write functions. */
    int Read(int nBytesToRead, unsigned char *dest, int flags=0);
    SPxErrorCode Write(int nBytesToWrite, const unsigned char *src, int flags=0);
    SPxErrorCode Write(const SPxReturnHeader *hdr, const unsigned char *data,
			int dataLenBytes, int flags=0);

    /* Information retrieval functions. */
    SPxThread *GetThread(void);
    int GetSize(void) {return m_size;}
    int GetAllocationType(void) {return m_bufferSpaceIsAllocatedByClass;}
    int BytesWaitingInBuffer(void)
    {
	return(m_bytesToBeRead);
    }
    void GetPointers(int *readPtr, int *writePtr, int *size)
    {
	if(readPtr)	*readPtr = m_readPtr;
	if(writePtr)	*writePtr = m_writePtr;
	if(size)	*size = m_size;
    }
    int SpaceLeftInBuffer(void)
    {
	/* Do not lock the mutex. */
	return(m_size - m_bytesToBeRead);
    }
    int GetReturnLength(void)
    {
	return(m_returnLength);
    }

    /* PIM manager handling. */
    SPxPimManager *GetPimManager(void)
    {
	return(m_pimManager);
    }

    /* Synchronisation handling. */
    SPxEvent *GetEventObject(void)
    {
	return(&m_eventObject);
    }

    /* Change-flag handling. */
    void SetChangeFlags(UINT32 flags);
    UINT32 GetAndClearChangeFlags(void);

    /* Parameter interface. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);
}; /* SPxRIB */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Determine the number of bytes per sample from the packing type. */
extern unsigned int SPxGetPackingBytesPerSample(UCHAR packing);

/* Determine the number of samples per byte from the packing type. */
extern unsigned int SPxGetPackingSamplesPerByte(UCHAR packing);

/* Determine the number of bytes required for the specified number of samples. */
extern unsigned int SPxGetPackingNumBytes(UCHAR packing, unsigned int numSamples);

/* Determine the mask of the bits holding any channel within the sample value. */
extern unsigned int SPxGetPackingBitsMask(UCHAR packing,
	SPxPackingChannel channel=SPX_PACKING_CHAN_A);

/* Determine the mask of the bits holding the main radar sample value. */
extern unsigned int SPxGetPackingNormalBitsMask(UCHAR packing);

/* Determine the mask of the bits holding the digital sample values. */
extern unsigned int SPxGetPackingExtraBitsMask(UCHAR packing,
	SPxPackingChannel channel=SPX_PACKING_CHAN_DIG_ALL);

/* Determine the amount by which any channel within the radar sample is shifted. */
extern int SPxGetPackingShift(UCHAR packing,
	SPxPackingChannel channel=SPX_PACKING_CHAN_A);

/* Determine the number of analogue video channels from the packing type. */
extern unsigned int SPxGetPackingNumAnalogueChannels(UCHAR packing);

/* Determine the number of digital bits from the packing type. */
extern unsigned int SPxGetPackingNumDigitalBits(UCHAR packing);

/* Determine the maximum value for a channel from the packing type. */
extern unsigned int SPxGetPackingMaxVal(UCHAR packing,
	SPxPackingChannel channel=SPX_PACKING_CHAN_A);

#endif /* _SPX_BUFFER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
