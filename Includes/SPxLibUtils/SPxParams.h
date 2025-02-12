/*********************************************************************
*
* (c) Copyright 2007 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPx parameter handling.
*
* Revision Control:
*   18/10/21 v1.54   AGC	Support multiple string suffixes in SPxCompareParam().
*
* Previous Changes:
*   28/08/20 1.53   AGC	Forward declare SPxCriticalSection.
*   15/04/20 1.52   AGC	Avoid SPxLoadParamsFromStringList()
*			causing duplicate param errors.
*   16/03/20 1.51   BTB	Add SPxLoadParamsFromStringList().
*   29/01/20 1.50   SP 	Add SPxCompareParam() overload.
*   06/01/20 1.49   SP 	Support additional indicies in SPxCompareParam().
*   26/05/19 1.48   SP 	Add SPxCompareParam().
*   09/05/19 1.47   REW	Support alsoApplyParam in SPxSetParameter().
*   05/04/19 1.46   AGC	Support domain for SPxRunConfig().
*   03/05/18 1.45   AGC	Support disabling history for parameters.
*   26/07/17 1.44   AGC	Support defaults in config files.
*   23/08/16 1.43   AGC	Add SPxDumpParameters() overload for writing to string.
*   23/05/16 1.42   SP 	Add SPxGetLangString().
*   18/05/16 1.41   SP 	Change language param prefix.
*   05/05/16 1.40   AJH	Add SPxSetConfigFilename().
*   01/12/15 1.39   AGC	Add domain for filename retrieval.
*   09/11/15 1.38   AGC	Add SPxGetParamNameSuffix().
*   15/07/15 1.37   AGC	Support prefix for SPxTestP().
*   21/11/14 1.36   AGC	Add SPxRemoveFileHistory().
*   07/07/14 1.35   AGC	Include required stdio header.
*   04/07/14 1.34   AGC	Remove use of std::string.
*   25/04/14 1.33   AGC	Allow calling SPxReadParameter() in loop for single line.
*   26/03/14 1.32   AGC	Fix default end string for SPxReadParameter().
*   26/03/14 1.31   AGC	Add SPxReadParameter().
*   04/12/13 1.30   AGC	Use const value for SPxSetString().
*   07/10/13 1.29   AGC	Add replaceCtrlChars argument to SPxGetString and variants.
*   01/10/12 1.28   AGC	Add SPxGetString... const functions.
*   31/08/12 1.27   AGC	Add SPxRunConfig().
*   02/12/11 1.26   AGC	Support removing parameters from config file.
*   17/11/11 1.25   AGC	Add SPxGetConfigShortPath().
*   27/07/11 1.24   AGC	Return error from SPxDumpParameters().
*   24/05/11 1.23   AGC	Add SPxGetLastFilenameWithError().
*   24/03/11 1.22   AGC	Warn if duplicate parameters found when loading config
*			if new optional warnIfSet flag is TRUE.
*   28/01/11 1.21   AGC	Add optional mutexAlreadyLocked parameter to SPxLookupParameterName().
*   10/01/11 1.20   AGC	Add optional silent parameter to SPxMigrateConfig().
*   26/11/10 1.19   REW	Use const for parameter value too.
*   26/11/10 1.18   REW	Use const for parameter names.
*   10/11/10 1.17   AGC	Add optional precision to SPxSetReal().
*   08/11/10 1.16   AGC	Refactor config migration.
*   28/10/10 1.15   AGC	Support writing only a subset of all config.
*			Add SPxGetAndSetSafe...() functions.
*			Add SPxCheckForLegacyParams().
*			Store name/value as strings.
*   04/10/10 1.14   AGC	Implement SPxWriteConfig().
*   27/05/10 1.13   SP 	Make SPxLookupParameterName() public.
*   13/05/10 1.12   DGJ Support optional message fn in SPxLoadConfig()
*   28/08/09 1.11   REW	Add SPxParamsInit().
*   13/08/09 1.10   SP 	Share param list with plugin.
*   31/07/09 1.9    SP 	Enable filename to be retrieved.
*   29/07/09 1.8    DGJ	Add SPxGetSPxEnv()
*   23/06/09 1.7    REW	Add SPxFindBestParamName() and support
*			prefix/suffix in GetSafe functions.
*   21/10/08 1.6    DGJ	Add SPxDeleteConfig().
*   16/10/08 1.5    REW	Add Uint versions.
*   25/03/08 1.4    DGJ	Add Safe versions
*   10/01/08 1.3    REW	Use const for filename in SPxLoadConfig().
*   24/05/07 1.2    REW Added header, changed to SPx etc.
*   29/03/07 1.1    DJG	Initial Version
**********************************************************************/

#ifndef _SPX_PARAMS_H
#define	_SPX_PARAMS_H

/*********************************************************************
*
*  Other headers 
*
**********************************************************************/

#include <stdio.h>
#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxCriticalSection;

/* The stored parameters are held as a linked list of structures of
 * type SPxStringList. A pair of pointers FirstParameter and LastParameter
 * point to the start and end of the list respectively.
 */
struct SPxStringList
{
    char *name;                 /* Name of the parameter */
    char *value;		/* Value expressed as a string */
    char *valueCtrl;		/* Value with control characters replaced. */
    unsigned int dotPos;	/* Position to place dot in name or zero for no dot. */
    int mark;			/* General purpose mark for parameter. */
    SPxStringList *next;	/* Next parameter in the list */
    SPxStringList *prev;	/* Previous parameter in the list */
    int original;		/* Was this parameter loaded from the config file? */
    int isDefault;		/* Is this parameter loaded from a 'defaults' file? */
    int allowDuplicate;		/* Allow this parameter to be duplicated once? */
    int saveHistory;		/* Should history be saved for this parameter? */

};

/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

SPxErrorCode SPxParamsInit(void);
int SPxSetParameter(const char *parameterName, 
		    const char *parameterValue, 
		    int fromConfigFile=FALSE,
		    int isDefault=FALSE,
		    int alsoApplyParam=FALSE,
		    int allowDuplicate=FALSE);
int SPxApplyParameter(const char *parameterName,
		    const char *parameterValue);
int SPxLoadConfig(const char *filename, 
		  void (*messFn)(const char*) = NULL, 
		  int warnIfSet=FALSE, int applyNotStoreParams=FALSE,
		  const char *domain=NULL,
		  int isDefault=FALSE);
int SPxLoadParamsFromStringList(const char **paramList);
int SPxRunConfig(const char *filename, const char *domain=NULL);
const char *SPxGetConfigFilename(const char *domain=NULL);
const char *SPxGetConfigShortPath(const char *domain=NULL);
void SPxSetConfigFilename(const char *filename, const char *domain=NULL);
int SPxGetInt(const char *name);
int SPxGetSafeInt(const char *name, int safeReturn,
		  const char *prefix=NULL, const char *suffix=NULL);
int SPxGetAndSetSafeInt(const char *name, int safeReturn,
			const char *prefix=NULL, const char *suffix=NULL);
unsigned int SPxGetUint(const char *name);
unsigned int SPxGetSafeUint(const char *name, unsigned int safeReturn,
			const char *prefix=NULL, const char *suffix=NULL);
unsigned int SPxGetAndSetSafeUint(const char *name, unsigned int safeReturn,
			const char *prefix=NULL, const char *suffix=NULL);
int SPxTestP(const char *name, const char *prefix=NULL, int checkDefaults=TRUE);
double SPxGetReal(const char *name);
double SPxGetSafeReal(const char *name, double safeReturn,
			const char *prefix=NULL, const char *suffix=NULL);
double SPxGetAndSetSafeReal(const char *name, double safeReturn,
			const char *prefix=NULL, const char *suffix=NULL);
char * SPxGetString(const char *name, int replaceCtrlChars=FALSE);
const char * SPxGetStringConst(const char *name, int replaceCtrlChars=FALSE);
char * SPxGetSafeString(const char *name, char *safeString,
			const char *prefix=NULL, const char *suffix=NULL,
			int replaceCtrlChars=FALSE);
const char * SPxGetSafeStringConst(const char *name, const char *safeString,
			const char *prefix=NULL, const char *suffix=NULL,
			int replaceCtrlChars=FALSE);
char * SPxGetAndSetSafeString(const char *name, char *safeString,
			const char *prefix=NULL, const char *suffix=NULL,
			int replaceCtrlChars=FALSE);
const char * SPxGetAndSetSafeStringConst(const char *name, const char *safeString,
			const char *prefix=NULL, const char *suffix=NULL,
			int replaceCtrlChars=FALSE);
int SPxSetInt(const char *name, int value);
int SPxSetUint(const char *name, unsigned int value);
int SPxSetReal(const char *name, double value, int precision=-1);
int SPxSetString(const char *name, const char *value);
void SPxSetHistoryEnabled(const char *name, int enable);
int SPxWriteConfig(const char *filename=NULL, 
		   const char *incObjList[]=NULL,
		   const char *excObjList[]=NULL,
		   const char *legacyParams[]=NULL,
		   int *numMigrated=NULL,
		   FILE *log=NULL, int keepOldFile=FALSE,
		   const char *domain=NULL);
const char *SPxGetLastFilenameWithError(const char *domain=NULL);
int SPxReadParameter(const char *buffer, char *paramName, int paramNameLen,
		     char *value, int valueLen, const char *sep="=",
		     const char *end="\r\n\t ", const char **const bufEnd=NULL);
int SPxDumpParameters(const char *filename, const char *incObjList[]=NULL,
					    const char *excObjList[]=NULL);
int SPxDumpParameters(char *buffer, unsigned int bufLen,
		      const char *incObjList[] = NULL,
		      const char *excObjList[] = NULL);
void SPxDeleteConfig(void);
extern SPxErrorCode SPxFindBestParamName(const char *baseName,
					const char *prefix,
					const char *suffix,
					char *returnBuf, int bufSize);

void SPxGetParamList(SPxStringList **firstParamRtn,
                     SPxStringList **lastParamRtn,
                     SPxCriticalSection **paramListAccessRtn);

void SPxSetParamList(SPxStringList *firstParamRtn,
                     SPxStringList *lastParamRtn,
                     SPxCriticalSection *paramListAccessRtn);

SPxStringList *SPxLookupParameterName(const char *parameterName, 
                                      SPxStringList *start=NULL,
				      int mutexAlreadyLocked=FALSE);

int SPxDeleteParameter(SPxStringList *item);

int SPxMigrateConfig(const char *legacyParams[], int silent=TRUE);

int SPxRemoveFileHistory(const char *filename, const char *backupFilename=NULL,
			 int silent=TRUE, int verbosity=0);

const char *SPxGetParamNameSuffix(const char *fullParamName,
				  const char *expectedParamName);

const char *SPxGetLangString(const char *nameSuffix, const char *safeString);

int SPxCompareParam(const char *name,
                    const char *param,
                    unsigned int *index1Rtn=NULL,
                    unsigned int *index2Rtn=NULL,
                    unsigned int *index3Rtn=NULL);

int SPxCompareParam(const char *name,
                    const char *param,
                    char *suffix1Buf,
                    unsigned int suffix1BufLen,
                    char *suffix2Buf=NULL,
                    unsigned int suffix2BufLen=0,
                    char *suffix3Buf=NULL,
                    unsigned int suffix3BufLen=0);

#endif /* _SPX_PARAMS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
