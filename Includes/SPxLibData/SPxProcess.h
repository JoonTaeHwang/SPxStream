/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for Process classes.
*
* Revision Control:
*   05/07/22 v1.46   BTB	Support additional data handlers.
*
* Previous Changes:
*   05/11/21 1.45   REW	Make GetParamValueI() and GetParamValueR() const
*   03/06/21 1.44   AGC	Add GetAndClearParamChangeFlag().
*   14/10/19 1.43   AGC	Suppress cppcheck warning.
*   08/03/17 1.42   AGC	Use atomic variables for user args.
*   02/09/16 1.41   AGC	Use atomic variables for call counts.
*   22/01/16 1.40   AGC	Remove unused default process type.
*   21/05/14 1.39   REW	Add lockListOnSuccess option to GetRegionHandle().
*   26/03/14 1.38   AGC	Support SPxProcParamIface.
*   17/02/14 1.37   AGC	Use const for process names.
*   05/09/13 1.36   SP 	Fix N/C #260 - Change args to SPxProcFuncReturn.
*   23/11/12 1.35   REW	Fix compiler warning.
*   23/11/12 1.34   SP 	Add GetReturnHandler() and GetDataHandler().
*			Add extra args to SPxProcFuncData.
*			Add return handler mutex.
*   14/11/12 1.33   SP 	Add SetReturnHandler() and SetDataHandler().
*   09/11/12 1.32   REW	Support regions for run-processes.
*   23/10/12 1.31   REW	Support PIMD.
*   21/12/11 1.30   SP 	Add Set/GetMaxBytesPerSample().
*   08/11/10 1.29   SP 	Add GetLastParameter().
*   28/10/10 1.28   AGC	Override SetConfigFromState() function.
*   15/09/10 1.27   REW	Make destructor virtual.
*   27/05/10 1.26   SP 	Override SetStateFromConfig().
*   20/11/09 1.25   SP 	Add SetParamsFromConfig() to SPxRunProcess.
*   13/08/09 1.24   SP 	Make process name const in constructor.
*   13/05/09 1.23   REW	Add TestParamExists().
*			Fix version numbers back to 1.4.
*   23/01/09 1.22   DGJ	Remove process type (user/system)
*   13/11/08 1.21   REW	Add GetParameter() from base class.
*   22/10/08 1.20   DGJ	Add SetPIMA/B/C() functions.
*   01/07/08 1.19   REW	Add isStatus to AddParameterI/R/S() funcs.
*   21/01/08 1.18   REW	SPxProcessRunParameters handle string memory.
*   18/01/08 1.17   DGJ	Provide buffer for string param value
*   07/01/08 1.16   DGJ	Add process statistics
*   11/12/07 1.15   DGJ	Support string parameter type for Process
*   10/12/07 1.14   REW	Override SetActive() in SPxRunProcess.
*   12/10/07 1.13   DGJ	Add GetThread(), tidy up add comments etc.
*   06/10/07 1.12   DGJ	Add cleanup function
*   30/09/07 1.11   DGJ	New SPxRunProcess API.
*   17/09/07 1.10   DGJ	Set/Get Active moved to SPxObj
*   13/09/07 1.9    REW	Add NotifyChanges() and supporting code.
*   13/09/07 1.8    DGJ	Constructor supports initial activity state.
*   10/09/07 1.7    REW	Add SetParameter().
*   31/08/07 1.6    DGJ	Support UserArgs2
*   24/08/07 1.5    DGJ	Support GetFirstRunParameter(), 
*			Support system/user process types.
*			Added destructor for SPxProcess (missing!)
*   20/08/07 1.4    DGJ	Support ManagerHasGone message
*			SourcePIMisDeleted message on SPxRunProcess
*   29/06/07 1.3    DGJ	Derive from SPxObj
*   27/06/07 1.2    DGJ	Allow pim to be NULL.
*   12/06/07 1.1    REW	Initial Version from SPxProcMan.h
**********************************************************************/

#ifndef _SPX_PROCESS_H
#define _SPX_PROCESS_H

/*
 * Other headers required.
 */
/* We need SPxReturn information and PIMs. */
#include "SPxLibUtils/SPxAtomic.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxPim.h"
#include "SPxLibData/SPxRib.h"

/* We need process parameters. */
#include "SPxLibData/SPxProcParam.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Define the maximum number of output proceses that a RunProcess can have. */
#define SPX_RUNPROC_MAX_NUM_OUTPUT_PROCESSES 16

/* Define the maximum number of active regions a RunProcess can have. */
#define	SPX_RUNPROC_MAX_REGIONS	8

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

/* Forward declare the items we need. */
class SPxRunProcess;
class SPxActiveRegion;

/* Type of data pass to generic data handler function. */
typedef enum
{
    /* An array of threshold data from SPxProcDynamicThreshold. */
    SPX_PROC_DATA_TYPE_DYNAMIC_THRESHOLD = 0

} SPxProcDataType_t;

/*
 * Types for pointers to functions for processes.
 */
/* The function that actually does the work on return data. */
typedef void (*SPxProcFunc)(SPxRunProcess *, SPxReturn *, unsigned, unsigned);

/* The function that handles change reporting, if any. */
typedef int (*SPxProcFuncChanges)(SPxRunProcess *, UINT32);

/* The function that handles clean-up. */
typedef void (*SPxProcFuncCleanup)(SPxRunProcess *);

/* Function that may be installed to access return data
 * directly (from the PIM) after it has been processed by 
 * an SPxRunProcess. 
 */
typedef void (*SPxProcFuncReturn)(void *userArg,          /* User argument. */
                                  SPxRunProcess *rp,      /* Run process. */
                                  unsigned int fisrtAzi,  /* First azimuth index. */
                                  unsigned int numAzis);  /* Number of azimuths. */

/* Function that may be installed to access process specific
 * data directly associated with a return. 
 */
typedef void (*SPxProcFuncData)(void *userArg,          /* User argument. */
                                SPxRunProcess *rp,      /* Run process. */
                                unsigned int azi,       /* Azimuth index. */
                                SPxProcDataType_t type, /* Data type. */
                                UCHAR packing,          /* Data packing. */
                                void *data,             /* Process data. */
                                unsigned int sizeBytes);/* Data size. */

/*
 * Typedef for a process type.
 */
typedef enum {SPX_PROC_USER, SPX_PROC_SYSTEM} SPxProcType;

/* Forward declare the classes we need within our own. */
class SPxProcessManager;
class SPxPIM;


/*
 * Class definining a process (see SPxRunProcess for process to run).
 */
class SPxProcess :public SPxObj
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor/Destructor */
    SPxProcess(const char *name, SPxProcFunc fn,
			SPxProcFuncChanges changeFn = NULL,
			SPxProcFuncCleanup cleanupFn = NULL);
    virtual ~SPxProcess(void);

    /* Process-parameter manipulation (NOT base SPxObj parameters). */
    SPxProcessParameter *GetFirstParameter(void) { return m_firstParameter; }
    SPxProcessParameter *GetLastParameter(void) { return m_lastParameter; }
    void AddParameterI(const char *pname, int defaultValue=0,
					int isStatus=FALSE);
    void AddParameterR(const char *pname, double defaultValue=0.0,
					int isStatus=FALSE);
    void AddParameterS(const char *pname, const char *defaultValue = NULL,
					int isStatus=FALSE);

    /* Configure maximum sample size supported. */
    SPxErrorCode SetMaxBytesPerSample(unsigned int sizeBytes);

    /* Information retrieval. */
    char *GetName(void)			{ return m_processName; }
    SPxProcFunc GetFunction(void)	{ return m_function; }
    SPxProcFuncChanges GetFunctionChanges(void)	{ return m_functionChanges; }
    unsigned int GetMaxBytesPerSample(void) {return m_maxBytesPerSample;}

    /* Retrieve the cleanup function. */
    SPxProcFuncCleanup GetCleanupFunction(void) {return m_functionCleanup;}

private:
    /*
     * Private fields.
     */
    /* User-friendly name for the process. */
    char *m_processName;

    /* Functions that actually do the work. */
    SPxProcFunc m_function;			/* Process returns */
    SPxProcFuncChanges m_functionChanges;	/* Change handling */
    SPxProcFuncCleanup m_functionCleanup;	/* Cleanup Function. */

    /* Linked list of parameters. */
    SPxProcessParameter *m_firstParameter;
    SPxProcessParameter *m_lastParameter;

    /* The maximum sample size supported by this process. */
    unsigned int m_maxBytesPerSample;

    /*
     * Private functions.
     */
}; /* SPxProcess */


/*
 * Class that encapulates the invocation of an SPxProcess.
 */
class SPxRunProcess :public SPxObj, public SPxProcParamIface
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor/destructor */
    SPxRunProcess(SPxProcess *process,
			SPxRunProcess *inputProcess,
			SPxPIM *pimA, 
			SPxPIM *pimB,
			SPxPIM *pimC,
			void *userArgs = NULL,
			int initialActivityState = 1);

    SPxRunProcess(SPxProcess *process,
			SPxRunProcess *inputProcess,
			SPxPIM *pimA, 
			SPxPIM *pimB,
			void *userArgs = NULL,
			int initialActivityState = 1);

    SPxRunProcess(SPxProcess *process,
			SPxRunProcess *inputProcess,
			SPxPIM *pimA, 
			void *userArgs = NULL,
			int initialActivityState = 1);

    void CommonInit(SPxProcess *process,
				SPxRunProcess *inputProc,
				SPxPIM *pimA,
				SPxPIM *pimB,
				SPxPIM *pimC,
				void *userArgs,
				int initialActivityState);

    virtual ~SPxRunProcess(void);

    /* Function to actually execute the process. */
    void Execute(SPxReturn *firstNewReturn,
			unsigned firstNewAzimuthInPim,
			unsigned numberOfAzimuths);

    /* Function to tell the process about changes to the source. */
    int NotifyChanges(UINT32 flags);

    /* Parameter manipulation (RunProcess parameters, not SPxObj interface). */
    int TestParamExists(const char *pname)
    {
	/* Return TRUE if it exists, FALSE otherwise. */
	return( (findParameterByName(pname) == NULL) ? FALSE : TRUE );
    }
    int GetParamValueI(const char *pname) const;
    double GetParamValueR(const char *pname) const;
    char *GetParamValueS(const char *pname, char *buffer, int bufLen);
    void SetParamValueI(const char *pname, int val);
    void SetParamValueR(const char *pname, double val);
    void SetParamValueS(const char *pname, const char *val);
    void SetParamsFromConfig(void);
    SPxErrorCode SetStateFromConfig(void);
    SPxErrorCode SetConfigFromState(void);

    /* Override Get/SetParameter() and SetActive() from the base class. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    // cppcheck-suppress virtualCallInConstructor
    void SetActive(int active);

    /* Activity control. */
    /* Process argument manipulation. */
    void *GetUserArgs(void)		{ return m_userArgs; }
    void *GetUserArgs2(void)		{ return m_userArgs2; }
    void SetUserArgs(void *args)	{ m_userArgs = args; }
    void SetUserArgs2(void *args)	{ m_userArgs2 = args;}

    /* Information retrieval. */
    SPxProcess *GetProcess(void)	{ return m_process; }
    char *GetProcessName(void)		{ return m_process ? m_process->GetName() : NULL; }
    char *GetName(void)			{ return SPxObj::GetName(); }
    SPxRunProcess *GetInput(void)	{ return m_input; }
    SPxRunProcess *GetOutput(int i)	{ return m_output[i]; }
    int GetNumOutputProcesses(void)	{ return m_numOutputProcesses; }
    /* Support GetInputPIM for legacy only - same as GetPIMA() */
    SPxPIM *GetInputPIM(void)		{ return m_pimA;}
    SPxPIM *GetPIMA(void)		{ return m_pimA;}
    SPxPIM *GetPIMB(void)		{ return m_pimB;}
    SPxPIM *GetPIMC(void)		{ return m_pimC;}
    SPxPIM *GetPIMD(void)		{ return m_pimD;}
    SPxProcessManager *GetProcessManager(void) { return m_processManager; }
    SPxThread *GetThread(void);

    /* Set the PIMs - these are used when a process gets deleted. */
    void SetPIMA(SPxPIM *pim) {m_pimA = pim;}
    void SetPIMB(SPxPIM *pim) {m_pimB = pim;}
    void SetPIMC(SPxPIM *pim) {m_pimC = pim;}
    void SetPIMD(SPxPIM *pim) {m_pimD = pim;}

    /* Process calling statistics. */
    static SPxAtomic<int> m_totalProcessCalls;
    static int GetTotalProcessCalls(void);
    int GetCalls(void) {return m_calls;}

    /* Report the fact that the Process Manager has gone. */
    void ManagerHasGone(void)		{ m_processManager = 0;}

    /* Retrieve the first RunParameter for this object */
    SPxProcessRunParameter* GetFirstRunParameter(void)
    {
	return m_firstRunParameter;
    }

    /* Linked list manipulation. */
    void SetNext(SPxRunProcess *next)	{ m_next = next; }
    SPxRunProcess *GetNext(void)	{ return m_next; }
    void SourcePIMisDeleted(void)	{ m_pimA = NULL;}

    /* Area region control. */
    int SetRegionName(unsigned int idx, const char *name);
    int SetRegionValue(unsigned int idx, const char *value);
    const char *GetRegionName(unsigned int idx);
    SPxActiveRegion *GetRegionHandle(unsigned int idx,
					int lockListOnSuccess = FALSE);
    const char *GetRegionValue(unsigned int idx);
    const char *GetRegionValueForPoint(double rangeMetres, double aziDegrees);

    /* Return data access callback. */
    SPxErrorCode SetReturnHandler(SPxProcFuncReturn fn, void *userArg);
    SPxProcFuncReturn GetReturnHandler(void) { return m_returnFn; }
    void *GetReturnHandlerArg(void) { return m_returnFnArg; }

    /* Process data access callback. */
    SPxErrorCode SetDataHandler(SPxProcFuncData fn, void *userArg);
    SPxProcFuncData GetDataHandler(void) { return m_dataFn; }
    void *GetDataHandlerArg(void) { return m_dataFnArg; }
    /* Additional data access callbacks. */
    SPxErrorCode AddDataHandler(SPxProcFuncData fn, void *userArg);
    SPxErrorCode RemoveDataHandler(SPxProcFuncData fn, void *userArg);
    int HaveExtraDataHandlers(void);
    void InvokeExtraDataHandlers(SPxRunProcess *rp,
                                 unsigned int azi,
                                 SPxProcDataType_t type,
                                 UCHAR packing,
                                 void *data,
                                 unsigned int sizeBytes);

    int GetAndClearParamChangeFlag(void);
    
private:
    /*
     * Private fields.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Pointer to the process that we are running, and the manager. */
    SPxAtomic<SPxProcess*> m_process;
    SPxProcessManager *m_processManager;

    /* Sources of data for the process. */
    SPxPIM *m_pimA;
    SPxPIM *m_pimB;
    SPxPIM *m_pimC;
    SPxPIM *m_pimD;

    SPxRunProcess *m_input;	/* Input process feeding us (if any). */

    /* Destinations for data from the process.  The m_output process list
     * should ideally be a linked list.
     */
    SPxRunProcess *m_output[SPX_RUNPROC_MAX_NUM_OUTPUT_PROCESSES];
    unsigned m_numOutputProcesses;

    /* Per-instance user args. */
    SPxAtomic<void*> m_userArgs;
    SPxAtomic<void*> m_userArgs2;

    /* Number of times this process has been called. */
    SPxAtomic<int> m_calls;

    /* Parameter lists. */
    SPxProcessRunParameter *m_firstRunParameter;
    SPxProcessRunParameter *m_lastRunParameter;

    /* Linked list holding us. */
    SPxRunProcess *m_next;		// Next RunProcess in the list 

    /* Used to throttle error reporting. */
    UINT32 m_lastReturnErrorTimeMsecs;

    /* Area region control. */
    char *m_regionNames[SPX_RUNPROC_MAX_REGIONS];
    char *m_regionValues[SPX_RUNPROC_MAX_REGIONS];

    /* Installed user callback and arg used to access return data 
     * after it has been processed. 
     */
    SPxProcFuncReturn m_returnFn;
    void *m_returnFnArg;
    SPxCriticalSection m_returnFnMutex;

    /* Installed user callback and arg used to access process 
     * specific data associated with a return. 
     */
    SPxProcFuncData m_dataFn;
    void *m_dataFnArg;

    /* Flag indicating parameters have changed. */
    SPxAtomic<int> m_paramsChanged;

    /*
     * Private functions.
     */
    /* Parameter handling. */
    void addParameter(SPxProcessParameter *parameter);
    SPxProcessRunParameter *findParameterByName(const char *pname) const;

    /* Input/output handling. */
    int addRunProcessToOutput(SPxRunProcess *newOutput);
    void removeRunProcessFromOutput(SPxRunProcess *outputToRemove);
    void setInput(SPxRunProcess *input) {m_input = input;}
    
    /* Error checking. */
    SPxErrorCode validateSampleSize(unsigned int firstAzi, 
				    unsigned int numAzis);

}; /* SPxRunProcess */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_PROCESS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
