/*********************************************************************
*
* (c) Copyright 2013 - 2016, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPx Undo functions that manage a stack of undo-able
*   items.
*
* Revision Control:
*   01/05/18 v1.17   AGC	Add max to remove argument to SPxUndoRemoveActions().
*
* Previous Changes:
*   14/03/18 1.16   AGC	Support format warnings with clang on Windows.
*   06/03/18 1.15   AGC	Support SPxUndoAddAction() format warnings.
*   12/02/16 1.14   AGC	Return error on malloc fail.
*   25/09/14 1.13   JP	Reverted to the 1.11 version.
*   24/09/14 1.12   JP	Created declarations of SPxUndoStruct and SPxUndoEntry.
*   08/11/13 1.11   AGC	Add SPxUndoAddActionPtr() to avoid gcc compiler bug.
*   04/11/13 1.10   AGC	Fix SPxUndoAddAction() for pointer argument.
*   10/10/13 1.9    AGC	Prefix va_list with std.
*   09/10/13 1.8    AGC	Put inline back to avoid compiler warning.
*   09/10/13 1.7    AGC	Fix VS 64-bit errors by reworking SPxUndoAddAction.
*   07/10/13 1.6    AGC	Avoid more compiler warnings.
*			Add SPxUndoGetVal().
*   07/10/13 1.5    AGC	Avoid compiler warning.
*   07/10/13 1.4    AGC	Add enhanced SPxUndoAddAction().
*			Add SPxUndoGetVal().
*   25/04/13 1.3    AGC	Pass user objects to iterate function.
*   28/03/13 1.2    AGC	Add SPxUndoSetMaxEntries().
*   04/03/13 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_UNDO_H
#define _SPX_UNDO_H

/* Need std::malloc. */
#include <cstdlib>
#include <cstdarg>

/* For SPX_STATIC_ASSERT. */
#include "SPxLibUtils/SPxTypes.h"

/* Need error codes. */
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

typedef SPxErrorCode (*SPxUndoFn_t)(void *userObj, void *userObj2, void *userArg);
typedef SPxErrorCode (*SPxUndoIterateFn_t)(void *userArg, unsigned int id, const char *name,
					   void *userObj, void *userObj2, void *undoUserArg);
typedef SPxErrorCode (*SPxUndoReportFn_t)(void *userArg);

/* Utility structure. */
template<typename T>
struct SPxUndoFalse
{
    static const bool value = false;
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Functions called by applications to undo actions. */
extern void SPxUndoEnable(int enable);
extern int SPxUndoIsEnabled(void);
extern int SPxUndoIsAvailable(void);
extern SPxErrorCode SPxUndoSetMaxEntries(unsigned int maxEntries);
extern SPxErrorCode SPxUndoAction(unsigned int id=0);
extern SPxErrorCode SPxUndoIterateActions(SPxUndoIterateFn_t fn, void *userArg);
extern SPxErrorCode SPxUndoAddReporter(SPxUndoReportFn_t fn, void *userArg);
extern SPxErrorCode SPxUndoRemoveReporter(SPxUndoReportFn_t fn, void *userArg);

/* Functions called by library and applications to add/remove actions. */
extern SPxErrorCode SPxUndoAddActionV(SPxUndoFn_t fn, void *userObj, void *userObj2,
				      void *userArg, const char *format, std::va_list ap);

/* This overload cannot be used as having a full implementation triggers a compiler
 * error when compiled with gcc 4.2.4.
 * This version simply triggers a compiler error if any attempt is made to call it.
 */
template<typename T>
inline SPxErrorCode SPxUndoAddAction(SPxUndoFn_t fn, void *userObj, void *userObj2,
				     T *userArg, const char *format, ...)
{
    SPX_STATIC_ASSERT_ALWAYS(T, userArg_must_not_be_pointer);
    return SPX_NO_ERROR;
}

/* This version is for object user arguments. A copy is taken by this function. */
template<typename T>
inline SPxErrorCode
#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 5, 6)))
#endif
SPxUndoAddAction(SPxUndoFn_t fn, void *userObj, void *userObj2,
		 T userArg, const char *format, ...)
{
    SPxErrorCode err = SPX_NO_ERROR;
    std::va_list ap;
    va_start(ap, format);
    T *userArgPtr = static_cast<T*>(std::malloc(sizeof(T)));
    if( !userArgPtr )
    {
	return SPX_ERR_BAD_MALLOC;
    }
    *userArgPtr = userArg;
    err = SPxUndoAddActionV(fn, userObj, userObj2, userArgPtr, format, ap);
    va_end(ap);
    return err;
}

/* This version is for pointer user arguments allocated by the caller
 * with std::malloc. The user argument is freed by the SPxUndo module.
 */
inline SPxErrorCode
#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 5, 6)))
#endif
SPxUndoAddActionPtr(SPxUndoFn_t fn, void *userObj, void *userObj2,
		    void *userArg, const char *format, ...)
{
    SPxErrorCode err = SPX_NO_ERROR;
    std::va_list ap;
    va_start(ap, format);
    err = SPxUndoAddActionV(fn, userObj, userObj2, userArg, format, ap);
    va_end(ap);
    return err;
}

extern SPxErrorCode SPxUndoRemoveActions(void *userObj, int maxToRemove=-1);

/* Class for auto-disabling undo temporarily. */
class SPxUndoLock
{
public:
    SPxUndoLock(void) { SPxUndoEnable(FALSE); }
    ~SPxUndoLock(void) { SPxUndoEnable(TRUE); }
};

/* Utilities for extracting value from undo user argument. */
template<typename T>
inline T SPxUndoGetVal(void *val) { T *ret = static_cast<T*>(val); return *ret; }

#endif /* _SPX_UNDO_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
