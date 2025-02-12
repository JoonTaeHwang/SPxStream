/*********************************************************************
*
* (c) Copyright 2007 - 2016, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for Test Generator.
*
* Revision Control:
*   14/10/19 v1.33   AGC	Suppress cppcheck warning.
*
* Previous Changes:
*   09/05/19 1.32   AGC	Allow RIB to be changed.
*   06/03/18 1.31   REW	Support packing format for user test patterns.
*   05/03/18 1.30   REW	Support user arg for user test patterns.
*   02/09/16 1.29   AGC	Improve mutex behaviour.
*			Support testing.
*   26/09/14 1.28   SP 	Allow negative start range.
*   18/07/14 1.27   SP 	Add Get/SetStartRangeMetres().
*   04/10/13 1.26   AGC	Simplify headers.
*   02/05/12 1.25   REW	Make more functions members.
*   19/08/11 1.24   SP 	Add support for 16-bit test patterns.
*   10/01/11 1.23   AGC	Limit min End Range to 10m.
*   23/12/10 1.22   SP 	Limit max PRF to 16000Hz.
*   04/10/10 1.21   AGC	Sanity check period and prf args.
*   15/09/10 1.20   REW	Make destructor virtual.
*   28/07/10 1.19   DGJ	Add a capability to install a user function to
*			be invoked on each return.
*   16/12/09 1.18   DGJ	Support GetAzimuthScanMode()
*   13/11/08 1.17   REW	GetParameter() returns value in a string.
*   03/03/08 1.16   DGJ	Additional per instance parameter
*   17/01/08 1.15   REW	Avoid icc compiler warnings.
*   21/11/07 1.14   REW	Use SPxScanMode type for SetAzimuthScanMode()
*   15/11/07 1.13   DGJ	Support different scanning modes
*   19/09/07 1.12   DGJ	Fix problem with static variables in some tests.
*   10/09/07 1.11   DGJ	Add a user-defined test picture (test picture 0)
*   07/09/07 1.10   DGJ	Support GetParameter().
*   26/08/07 1.9    DGJ	Support GetRangeMetres(), 
*			GetThread() GetPattern() function
*   24/08/07 1.8    DGJ	Allow range to be set.
*   07/07/07 1.7    DGJ	Set writeFailed to 25 (was 100) on buffer full.
*   06/07/07 1.6    DGJ	Set writeFailed to 100 (was 500) on buffer full.
*   29/06/07 1.5    DGJ	Derive from SPxObj and support parameters
*   27/06/07 1.4    DGJ	Line buffer is per object now, and > 8k okay.
*   07/06/07 1.3    REW	Tidy, format, use thread objects etc. etc.
*   24/05/07 1.2    REW Added header, changed to SPx etc.
*   03/05/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_TESTGEN_H
#define _SPX_TESTGEN_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"

/* We need the base class header. */
#include "SPxLibData/SPxRadarSource.h"

/* We also need SPxRIB definitions. */
#include "SPxLibData/SPxRib.h"

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

/* Possible types of random scan for test generator. */
enum
{
    /* Simple random scan - random azimuths. */
    SPX_SCAN_RANDOM_SIMPLE = 0,

    /* Simulated quad scan. */
    SPX_SCAN_RANDOM_QUAD_SCAN = 1
};

/* Supported endian types for multibyte data */
typedef enum
{
    SPX_TEST_GEN_DATA_NATIVE_ENDIAN = 0,    /* Same as host */
    SPX_TEST_GEN_DATA_LITTLE_ENDIAN,        /* Force little-endian */
    SPX_TEST_GEN_DATA_BIG_ENDIAN            /* Force big-endian */

} SPxTestGenDataEndian_t;

/* Forward declarations. */
class SPxTestGenerator;
class SPxThread;

/* Function type to generate user-defined test pattern. */
typedef void (*SPxTestGenUserPatternFn)(SPxTestGenerator *testGen,
					int azimuth,
					unsigned char *dataPtr);

/* Function type called for each generated azimuth. */
typedef void (*SPxTestGenDataFn)(SPxTestGenerator *testGen,
				 void *userData,
				 SPxReturnHeader *returnHeader,
				 unsigned char *returnData);

/* Maintain the legacy user test pattern function. */
typedef SPxTestGenUserPatternFn TestGenUserPatternFn;

/*
 * Define our test generator class, derived from a generic radar source.
 */
class SPxTestGenerator :public SPxRadarSource
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxTestGenerator(SPxRIB *buffer, int rangeSamples,
		     double scanPeriod, int returnsPerSec,
		     int testPattern, int arg1=0);
    virtual ~SPxTestGenerator();
    
    /* Control and information retrieval. */
    SPxRIB *GetBuffer(void) { return m_buffer; }
    UINT32 GetSetUpCompleteWord(void) const;
    void SetPattern(int newTestPattern);
    int GetPattern(void) const;
    int GetTestPattern(void) { return GetPattern(); }
    int GetRangeSamples(void) const;
    void SetStartRangeMetres(double range);
    double GetStartRangeMetres(void) const;
    void SetEndRangeMetres(double range);
    double GetEndRangeMetres(void) const;
    void SetArg1(int arg);
    int GetArg1(void) const;
    void SetAdjust(int val);
    int GetAdjust(void) const;
    void SetAdjust2(int val);
    int GetAdjust2(void)const;
    void SetScanPeriod(double newScanPeriod);
    double GetScanPeriod(void) const;
    void SetReturnsPerSecond(unsigned newReturnsPerSec);
    int GetReturnsPerSec(void) const;

    /* For backward compatibility. */
    void SetRangeMetres(double range) { SetEndRangeMetres(range); } 
    double GetRangeMetres(void) const { return GetEndRangeMetres(); }
    
    /* Set/get the azimuth scan mode. */
    void SetAzimuthScanMode(SPxScanMode mode);
    SPxScanMode GetAzimuthScanMode(void) const;

    /* Set the azimuth sector. */
    void SetAzimuthSector(int startAzi, int endAzi);

    /* Set the random scan type. */
    void SetRandomScanType(int randomScanType);

    /* Data endianness */
    void SetDataEndian(SPxTestGenDataEndian_t de);
    SPxTestGenDataEndian_t GetDataEndian(void) const;

    /* Functions to get and set the function that implements a user-defined
     * test pattern. The user's function gets called to draw an azimuth.
     * The userArg can be retrieved by the installed function so it can
     * access any data it needs.  The packing should be set to tell the
     * object what format the data is in (e.g. SPX_RIB_PACKING_RAW8).
     */
    void SetUserFunction(SPxTestGenUserPatternFn fn, void *userArg=NULL);
    SPxTestGenUserPatternFn GetUserSetFunction(void) const;
    void *GetUserSetFunctionArg(void) const { return(m_userSetPatternArg); }
    void SetUserPacking(UCHAR packing) { m_userSetPatternPacking = packing; }
    UCHAR GetUserPacking(void) const { return(m_userSetPatternPacking); }

    /* These functions should only be called from within the
     * test generator thread (with mutex locked) or user set function.
     */
    /* Failure handling. */
    void SetWriteFailCount(void);
    int GetWriteFailCount(void) const;
    void DecrementWriteFailCount(void);

    /* Add a user function that the class will call after building
     * each return.
     */
    SPxErrorCode InstallDataFn(SPxTestGenDataFn fn, void *userData);
    
    /* Parameter assignment. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Thread function. */
    virtual void *ThreadFunc(SPxThread *thread);

    /* Return the thread. */
    SPxThread *GetThread(void) { return m_thread; }

protected:
    /* Functions for testing. */
    SPxTestGenerator(SPxRIB *buffer, int rangeSamples,
		     double scanPeriod, int returnsPerSec,
		     int testPattern, int arg1,
		     int startThread);
    void StartThread(void);
    // cppcheck-suppress virtualCallInConstructor
    void Shutdown(void);
    virtual SPxErrorCode OutputAzimuth(void);
    void SetRIB(SPxRIB *rib) { m_buffer = rib; }

private:
    /*
     * Private fields.
     */
    mutable SPxCriticalSection m_mutex;
    SPxRIB *m_buffer;	/* The associated RI buffer where data is written. */
    int m_rangeSamples; /* The number of range samples per return. */
    double m_scanPeriod;	/* The scan period in seconds */
    int m_returnsPerSec;	/* The number of returns per second */
    int m_testPattern;		/* The test pattern number */
    int m_arg1;			/* Optional argument for the test pattern */
    UINT32 m_setupComplete;	/* A flag set when set-up is complete */
    SPxThread *m_thread;	/* Worker thread for the generator. */
    int m_writeFailed;		/* Set to a value if a write to RIB fails then
				 * decrement the value for subsequent
				 * successes.
				 */
    int m_adjust;               /* A per-instance adjustment parameter */
    int m_adjust2;		/* An additional per-instance adjustment parameter */
    SPxTestGenUserPatternFn m_userSetPattern;
				/* The user-supplied function for creating
				 * a test pattern
				 */
    void *m_userSetPatternArg;	/* The associated user argument. */
    UCHAR m_userSetPatternPacking;	/* The SPX_RIB_PACKING_... format */


    unsigned char *m_lineBuffer;/* The buffer */
    double m_startRangeMetres;	/* The start range of each return */
    double m_endRangeMetres;	/* The end range of each return */
    int m_returnsPerScan;	/* Returns per scan (or per sector). */
    int m_azimuthCounter;	/* The azimuth counter. */
    SPxScanMode m_azimuthScanMode;	/* The scanning mode */
    /* Sector start and end values for sector scan. */
    int m_azimuthScanStartSector, m_azimuthScanEndSector;
    int m_sectorScanDirection;
    int m_randomScanType;	/* The type of random scan. */
    int m_compensationFlag;	/* Flag used to control jump-back time. */
    SPxTestGenDataEndian_t m_dataEndian; /* Data byte ordering */
    SPxTestGenDataFn m_userFunction;	/* A user function that gets called on each return constructed */
    void *m_userFunctionData;	    /* User function data */
    int m_returnsPerSecPrevious;
    double m_intervalBetweenReturnsMS;
    UINT32 m_incrementingCount;
    UINT32 m_startTime;
    UINT32 m_numberOfReturnsSinceStart;

    /*
     * Private functions.
     */

    void init(SPxRIB *buffer, int rangeSamples,
	      double scanPeriod, int returnsPerSec,
	      int testPattern, int arg1,
	      int startThread);
    void initialiseHeader(void);
    SPxErrorCode outputAzimuth(void);

    /* Compute the next azimuth number */
    int computeNextAzimuth(void);

    /* Functions for building test patterns. */
    UCHAR createTestPattern(int testPattern,
                               unsigned azimuth,
                               unsigned char *dataLocation,
                               int rangeSamples, int arg);

    void addNoise(unsigned azimuth, 
                     unsigned char *dataLocation,
                     int rangeSamples, 
                     int arg);

    void resetScanMode(void);
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_TESTGEN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
