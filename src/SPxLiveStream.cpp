/*********************************************************************
*
* (c) Copyright 2010 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxLiveStream.cpp,v $
* ID: $Id: SPxLiveStream.cpp,v 1.11 2017/11/13 16:00:04 rew Exp $
*
* Purpose:
*	SPx example program to use the SPxNetworkReceive class to provide
*	direct access to radar data received on the network.
*
*	Unless changed with the command line options, this program listens
*	to the default network multicast group/port and prints out periodic
*	messages about the incoming data.
*
*	The handleRadar() function is invoked for each spoke of data
*	received.
*
*	Run the program with "-?" as the command line option to get a help
*	message.
*
*
* Revision Control:
*   13/11/17 v1.11   AGC	Remove "using namespace std".
*
* Previous Changes:
*   08/03/17 1.10   AGC	Use correct calling convention for signal handler.
*   17/04/13 1.9    REW	Add -i option for ifAddr.
*   14/11/11 1.8    SP 	Display ASTERIX Cat-240 summary message.
*   02/11/11 1.7    SP 	Support receipt of ASTERIX Cat-240.
*   16/09/11 1.6    SP 	No need to exclude SPxLicInit() on Solaris.
*   31/08/11 1.5    AGC	Use new style error handler.
*   30/08/11 1.4    SP 	Support printing of multi-byte samples.
*   25/03/11 1.3    SP 	Only initialise licensing on Windows and Linux.
*   09/07/10 1.2    REW	Three levels of verbosity means print all data
*   02/03/10 1.1    REW	Initial Version.
**********************************************************************/

/* Standard headers. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include "stdafx.h"
#include <direct.h>  // Windows의 _mkdir를 위해 추가
#else
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>  // Linux의 mkdir를 위해 추가
#include <sys/types.h> // Linux의 mkdir를 위해 추가
#endif

/* SPx Library headers. */
#include "SPx.h"
#ifdef _WIN32
#include "SPxLibUtils/SPxGetOpt.h"
#endif

/*
 * Constants.
 */
#define	USAGE "Usage:\n\tSPxLiveStream [options]\n"			\
		"\nOptions:\n"						\
		"\t-a <addr>\tSet address for receiving radar data\n"	\
		"\t-d <flags>\tSet debug flags\n"			\
		"\t-i <ifAddr>\tSet interface address for multicast\n"	\
		"\t-p <port>\tSet port for receiving radar data\n"	\
		"\t-v\t\tIncrease verbosity\n"				\
		"\t-x\t\tReceive ASTERIX Cat-240 radar video\n"		\
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
static void handleRadar(SPxNetworkReceive *src, void *arg,
				SPxReturnHeader *hdr, unsigned char *data);

/* ASTERIX Cat-240 summary handler. */
static void handleCat240Summary(SPxNetworkReceiveAsterix *src, void *arg,
                                UINT8 sac, UINT8 sic,
                                const char *summaryText);

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
#ifdef _WIN32
/* MSVC declarations. */
CWinApp theApp;
#endif

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
    SPxErrorCode err;			/* SPx error value */
    char *addr;				/* For receiving radar data */
    char *ifAddr;			/* For joining multicast groups */
    int port;				/* For receiving radar data */
    UINT32 debug;			/* Debug flags */
    int asterixCat240 = FALSE;		/* Receive ASTERIX Cat-240 */

    /* Initialise operating system specific things. */
    if( osInit() != SPX_NO_ERROR )
    {
	SPxTimeSleepMsecs(EXIT_DELAY_TIME);
	exit(-1);
    }

    /* Initialise configuration. */
    addr = NULL;		/* NULL means use library default */
    port = 0;			/* 0 means use library default */
    ifAddr = NULL;
    debug = 0;

    /* Process any command line arguments.  */
    opterr = 0;
    while( (c = getopt(argc, argv, "a:d:i:p:vx?")) != -1 )
    {
	switch(c)
	{
	    case 'a':	addr = optarg;				break;
	    case 'd':	debug = strtoul(optarg, NULL, 0);	break;
	    case 'i':	ifAddr = optarg;			break;
	    case 'p':	port = strtol(optarg, NULL, 0);		break;
	    case 'v':	Verbose++;				break;
	    case 'x':	asterixCat240 = TRUE;			break;    
	    case '?':	/* fall through */
	    default:
		fprintf(stderr, "\n%s", USAGE);
		SPxTimeSleepMsecs(EXIT_DELAY_TIME);
		exit(-1);
	}
    } /* end of for each option */

    /*
     * Welcome banner.
     */
    printf("\n### Cambridge Pixel  %s ###\n\n",
		SPX_VERSION_STRING);

    /*
     * Install a handler for SPx errors.
     */
    SPxSetErrorHandler(spxErrorHandler);

    /*
     * Initialise library.
     */
    if( SPxInit() != SPX_NO_ERROR )
    {
	fprintf(stderr, "Failed to initialise SPx library.\n");
	SPxTimeSleepMsecs(EXIT_DELAY_TIME);
	exit(-1);
    }

    /*
     * Initialise licensing (not Solaris).
     */
    SPxLicInit();

    /*
     * Set up debug if desired.
     */
    if( (Verbose > 0) || (debug != 0) )
    {
	SPxNetworkReceive::SetLogFile(stdout);
	SPxNetworkReceive::SetDebug(debug);
	printf("Debug flags = 0x%08x\n", debug);
    }

    /* Instantiate the network receiving source, noting that we do not give
     * it a RIB to write into because we want direct data access.
     */
    SPxNetworkReceive *src = NULL;
    if (asterixCat240)
    {   
	/* Receive ASTERIX Cat-240 format radar video. */
	src = new SPxNetworkReceiveAsterix(NULL);
    }
    else
    {
	/* Receive SPx format radar video. */
	src = new SPxNetworkReceive(NULL);
    }

    /*
     * Create the source.
     */
    err = src->Create(addr, port, ifAddr);
    if( err != SPX_NO_ERROR )
    {
	fprintf(stderr, "Failed to create network source.\n");
	SPxTimeSleepMsecs(EXIT_DELAY_TIME);
	exit(-1);
    }

    /*
     * Install a routine to get radar data. (We don't use the user arg).
     */
    err = src->InstallDataFn(handleRadar, NULL);
    if( err != SPX_NO_ERROR )
    {
	fprintf(stderr, "Failed to install radar handler.\n");
	SPxTimeSleepMsecs(EXIT_DELAY_TIME);
	exit(-1);
    }

    if (asterixCat240)
    { 
        /*
         * Install a routine to get ASTERIX Cat-240 summary.
         * (We don't use the user arg).
         */
        SPxNetworkReceiveAsterix *atxSrc = (SPxNetworkReceiveAsterix *)src;
        err = atxSrc->InstallSummaryFn(handleCat240Summary, NULL);
        if( err != SPX_NO_ERROR )
        {
            fprintf(stderr, "Failed to install Cat-240 summary handler.\n");
            SPxTimeSleepMsecs(EXIT_DELAY_TIME);
            exit(-1);
        }
    }

    /*
     * Start acquisition from the network.
     */
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
*	Function to handle a spoke of radar data from the network.
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
static void handleRadar(SPxNetworkReceive *src, void *arg,
				SPxReturnHeader *hdr, unsigned char *data)
{
    static char buffer[4096];
    size_t offset = 0;
    
    float azimuthDegrees = (float)hdr->azimuth * 360.0f / 65536.0f;
    
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long long current_time_ms = (long long)ts.tv_sec * 1000LL + (ts.tv_nsec / 1000000LL);

    offset += snprintf(buffer + offset, sizeof(buffer) - offset, 
                      "%.2f,%.1f,%lld", azimuthDegrees, hdr->endRange, current_time_ms);
    
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
    
    if (offset < (size_t)(sizeof(buffer) - 2)) {
        buffer[offset++] = '\n';
        buffer[offset] = '\0';
        fwrite(buffer, 1, offset, stdout);
        fflush(stdout);
    }
} /* handleRadar() */


/*====================================================================
*
* handleCat240Summary
*	Function to handle an ASTERIX Cat-240 video summary message.
*
* Params:
*	src		Pointer to radar source object we are using,
*	arg		User argument we gave when installing this handler
*			function (not used in this example),
*       sac, sic        SAC and SIC received with the message,
*       summaryText     Summary text received with the message.
*
* Returns:
*	Nothing
*
* Notes
*
*===================================================================*/
static void handleCat240Summary(SPxNetworkReceiveAsterix *src, void *arg,
                                UINT8 sac, UINT8 sic, 
                                const char *summaryText)
{
    /* Sanity check. */
    if( (!src) || (!summaryText) )
    {
	/* Shouldn't ever happen. */
	return;
    }

    if( Verbose > 0 )
    {
        /* Include the time-of-day if present. */
        UINT32 timeOfDayMsecs = 0;
        if (src->GetLastSummaryTimeOfDayMsecs(&timeOfDayMsecs) == SPX_NO_ERROR)
        {
            /* Break down time of day into hour, minutes and seconds. */
            UINT32 hours = timeOfDayMsecs / (60 * 60 * 1000);
            UINT32 mins = (timeOfDayMsecs % (60 * 60 * 1000)) / (60 * 1000);
            UINT32 secs = (timeOfDayMsecs % (60 * 1000)) / 1000;
            UINT32 msecs = (timeOfDayMsecs % 1000);
    
            /* Print time-of-day and summary text. */
            printf("SUMMARY %02u:%02u:%02u.%03u, '%s'.\n", 
                   hours, mins, secs, msecs, summaryText);
        }
        else
        {
            /* Just print summary text. */
            printf("SUMMARY '%s'.\n", summaryText);
        }
    }

} /* handleCat240Summary() */


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
    /* Initialize MFC. */
    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
    {
	printf("Fatal Error: MFC initialization failed.\n");
	return(SPX_ERR_SYSCALL);
    }

    /* Ensure WinSock2 is initialised. */
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD( 2, 2 );
    if( WSAStartup(wVersionRequested, &wsaData) != NO_ERROR )
    {
	SPxError(SPX_ERR_SYSTEM,SPX_ERR_SOCKET_INIT,0,0);
	printf("Fatal Error: MFC initialization failed.\n");
	return(SPX_ERR_SYSCALL);
    }

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

