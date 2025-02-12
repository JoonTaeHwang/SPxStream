/*********************************************************************
*
* (c) Copyright 2011, 2012, 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: spxfiledatadirect.cpp,v $
* ID: $Id: spxfiledatadirect.cpp,v 1.4 2017/03/08 14:40:08 rew Exp $
*
* Purpose:
*	SPx example program to use the SPxRadarReplay class to provide
*	direct access to radar data in a recording file.
*
*	The name of the file to replay should be given as a command line
*	argument.  Other options may be specified as shown in USAGE below.
*
*	The handleRadar() function is invoked for each spoke of data
*	received.
*
* Revision Control:
*   08/03/17 v1.4    AGC	Use correct calling convention for signal handler.
*
* Previous Changes:
*   12/03/12 1.3    REW	Exit the main loop when file finishes.
*   19/09/11 1.2    REW	No need to exclude SPxLicInit() on solaris.
*   13/09/11 1.1    REW	Initial Version.
**********************************************************************/
 
/* Standard headers. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#else
#include <direct.h>
#endif

/* SPx Library headers. */
#include "SPxNoMFC.h"
#ifdef _WIN32
#include "SPxLibUtils/SPxGetOpt.h"
#endif

/*
 * Constants.
 */
#define	USAGE "Usage:\n\tspxfiledatadirect [options] <filename>\n"	\
		"\nOptions:\n"						\
		"\t-v\t\tIncrease verbosity\n"				\
		"\t-?\t\tPrint usage information.\n\n"

/* Amount of time to sleep on exit, in milliseconds. */
#ifdef _WIN32
#define	EXIT_DELAY_TIME	5000	/* To keep console window on screen */
#else
#define	EXIT_DELAY_TIME	100
#endif

/*
 * Private function prototypes.
 */
/* Error handler. */
static void spxErrorHandler(SPxErrorType errType, SPxErrorCode errCode,
				int arg1, int arg2,
				const char *arg3, const char *arg4);

/* Radar data handler. */
static void handleRadar(SPxRadarReplay *src, void *arg,
				SPxReturnHeader *hdr, unsigned char *data);

/* Init/shutdown utility functions. */
static SPxErrorCode osInit(void);
#ifdef _WIN32
static BOOL WINAPI sigIntHandler(DWORD fdwCtrlType);
#else
static void sigIntHandler(int sig);
#endif


/*
 * Global variables.
 */
/* Verbosity level. */
static int Verbose = 0;

/* Exit flag. */
static int MainLoopFinish = 0;


/*********************************************************************
*
*	Implementation functions
*
**********************************************************************/

/*====================================================================
*
* main
*	Entry point for program.
*
* Params:
*	argc, argv		Standard C arguments.
*
* Returns:
*	Zero on success,
*	Error code otherwise.
*
* Notes:
*
*===================================================================*/
int main(int argc, char **argv)
{
    int c;				/* For parsing command line options */

    /* Initialise operating system specific things. */
    if( osInit() != SPX_NO_ERROR )
    {
	SPxTimeSleepMsecs(EXIT_DELAY_TIME);
	exit(-1);
    }

    /* Process any command line arguments.  */
    opterr = 0;
    while( (c = getopt(argc, argv, "v?")) != -1 )
    {
	switch(c)
	{
	    case 'v':	Verbose++;				break;
	    case '?':	/* fall through */
	    default:
		fprintf(stderr, "\n%s", USAGE);
		SPxTimeSleepMsecs(EXIT_DELAY_TIME);
		exit(-1);
	}
    } /* end of for each option */

    /*
     * Check we have a filename to play.
     */
    if( optind >= argc )
    {
	/* No, we don't have a filename. */
	fprintf(stderr, "\n%s", USAGE);
	SPxTimeSleepMsecs(EXIT_DELAY_TIME);
	exit(-1);
    }
    const char *filename = argv[optind];

    /*
     * Welcome banner.
     */
    printf("\n### Cambridge Pixel SPxDataStream %s ###\n\n",
		SPX_VERSION_STRING);

    /*
     * Install error handler and initialise library.
     */
    SPxSetErrorHandler(spxErrorHandler);
    if( SPxInit() != SPX_NO_ERROR )
    {
	fprintf(stderr, "Failed to initialise SPx library.\n");
	SPxTimeSleepMsecs(EXIT_DELAY_TIME);
	exit(-1);
    }

    /* Initialise dongle-based licensing if available. */
    SPxLicInit();

    /* Create a file replay object, noting that we do not give
     * it a RIB to write into because we want direct data access.
     */
    SPxRadarReplay *src = new SPxRadarReplay(NULL);

    /* Open the file specified on the command line. */
    if( src->SetFileName(filename) != SPX_NO_ERROR )
    {
	fprintf(stderr, "Failed to select file '%s'.\n", filename);
	SPxTimeSleepMsecs(EXIT_DELAY_TIME);
	exit(-1);
    }

    /* Install a routine to get radar data. (We don't use the user arg). */
    if( src->InstallDataFn(handleRadar, NULL) != SPX_NO_ERROR )
    {
	fprintf(stderr, "Failed to install radar handler.\n");
	SPxTimeSleepMsecs(EXIT_DELAY_TIME);
	exit(-1);
    }

    /* Disable auto-looping at the end of the file. */
    src->SetAutoLoop(FALSE);

    /* Start replay. */
    src->Enable(TRUE);

    /*
     * Run the main loop.
     */
    while( !MainLoopFinish )
    {
	/* In a real application, we would normally do things in the main
	 * loop such as provide a user interface, display graphics and
	 * so on, but in this example we just make sure that we don't
	 * busy wait.
	 */
	SPxTimeSleepMsecs(100);

	/* The file replay goes into a paused state when the file finishes
	 * (because we called SetAutoLoop(FALSE) above), so look for this
	 * state to detect the end of the file.
	 */
	if( src->IsPaused() )
	{
	    printf("File finished.\n");
	    MainLoopFinish = TRUE;
	}
    } /* end of main loop */

    /*
     * Tidy up.
     */
    delete src;

    /* Sleep for a while so the console window doesn't vanish immediately
     * in case this isn't being run inside a console box on windows.
     */
    SPxTimeSleepMsecs(EXIT_DELAY_TIME);

    /* Finished. */
    exit(0);
} /* main() */


/*********************************************************************
*
*	Private functions.
*
**********************************************************************/

/*====================================================================
*
* spxErrorHandler
*	Callback function for errors reported by the SPx library.
*
* Params:
*	errType, errCode	Error type and code,
*	arg1 - arg4		Error values.
*
* Returns:
*	Nothing
*
* Notes
*
*===================================================================*/
static void spxErrorHandler(SPxErrorType errType, SPxErrorCode errCode,
				int arg1, int arg2,
				const char *arg3, const char *arg4)
{
    /* We simply report errors to stdout. */
    printf("SPx Error #%d, args %d, %d, %s, %s.\n",
		errCode, arg1, arg2,
		(arg3 ? arg3 : "<none>"),
		(arg4 ? arg4 : "<none>"));
    return;
} /* spxErrorHandler() */


/*====================================================================
*
* handleRadar
*	Function to handle a spoke of radar data from the file.
*
* Params:
*	src		Pointer to radar source object we are using,
*	arg		User argument we gave when installing this handler
*			function (not used in this example),
*	hdr		Pointer to header structure describing the spoke,
*	data		Pointer to the radar data for this return.
*
* Returns:
*	Nothing
*
* Notes
*
*===================================================================*/
static void handleRadar(SPxRadarReplay *src, void *arg,
				SPxReturnHeader *hdr, unsigned char *data)
{
    static char buffer[4096];
    size_t offset = 0;
    
    /* 방위각을 각도로 변환 (0-65535 -> 0-360도) */
    float azimuthDegrees = (float)hdr->azimuth * 360.0f / 65536.0f;
    
    /* 현재 시간 밀리초 단위로 가져오기 */
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long long current_time_ms = (long long)ts.tv_sec * 1000LL + (ts.tv_nsec / 1000000LL);

    /* 기본 정보 포맷팅: 방위각,끝 거리,시간 */
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, 
                      "%.4f,%.1f,%lld", azimuthDegrees, hdr->endRange, current_time_ms);
    
    /* 샘플 데이터 추가 */
    unsigned int bps = SPxGetPackingBytesPerSample(hdr->packing);
    if (bps == 1) {
        for (unsigned int i = 0; i < hdr->thisLength && 
             offset < (size_t)(sizeof(buffer) - 8); i++) {
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, ",%d", data[i]);
        }
    }
    else if (bps == 2) {
        UINT16 *data16 = (UINT16 *)data;
        for (unsigned int i = 0; i < hdr->thisLength && 
             offset < (size_t)(sizeof(buffer) - 8); i++) {
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, ",%d", data16[i]);
        }
    }
    
    /* 줄바꿈 추가 및 출력 */
    if (offset < (size_t)(sizeof(buffer) - 2)) {
        buffer[offset++] = '\n';
        buffer[offset] = '\0';
        fwrite(buffer, 1, offset, stdout);
        fflush(stdout);
    }
} /* handleRadar() */


/*********************************************************************
*
*	Utility functions to handle init/shutdown per operating system.
*
**********************************************************************/

/*====================================================================
*
* osInit
*	Function to perform operating system specific setup.
*
* Params:
*	None
*
* Returns:
*	SPx error code.
*
* Notes
*
*===================================================================*/
static SPxErrorCode osInit(void)
{
#ifdef _WIN32
    /* Install our tidy-up function. */
    if( SetConsoleCtrlHandler((PHANDLER_ROUTINE)sigIntHandler, TRUE) == 0 )
    {
	printf("Fatal Error: Failed to install ctrl-c handler.\n");
	return(SPX_ERR_SYSCALL);
    }
#else
    /* Install our tidy-up function. */
    signal(SIGINT, sigIntHandler);
#endif

    /* Done. */
    return(SPX_NO_ERROR);
} /* osInit() */


/*====================================================================
*
* sigIntHandler
*	Handler function for SIGINT (i.e. Ctrl-C).
*
* Params:
*	sig		Signal we are being called for.
*
* Returns:
*	Nothing
*
* Notes:
*	Tells the main loop to finish so we can clean up tidily etc.
*
*===================================================================*/
#ifdef _WIN32
static BOOL WINAPI sigIntHandler(DWORD sig)
{
    if( (sig == CTRL_C_EVENT) || (sig == CTRL_CLOSE_EVENT) )
    {
	printf("\nSIGINT received - exiting.\n");
	MainLoopFinish = 1;
	return(TRUE);
    }
    return(FALSE);
} /* sigIntHandler() */
#else
static void sigIntHandler(int sig)
{
    printf("\nSIGINT received - exiting.\n");
    MainLoopFinish = 1;
    return;
} /* sigIntHandler() */
#endif


/*********************************************************************
*
* End of file
*
**********************************************************************/
