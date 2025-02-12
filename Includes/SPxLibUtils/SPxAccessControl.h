/*********************************************************************
*
* (c) Copyright 2015 - 2016, 2018, 2022, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for SPxAccessControl class, which provides basic password
*	access to admin functions.
*
*
* Revision Control:
*   31/01/22 v1.6    REW	Add CheckPassword().
*
* Previous Changes:
*   19/06/18 1.5    BTB	Support up to 64 feature groups.
*   06/06/18 1.4    BTB	Add IsPasswordSet().
*   01/06/18 1.3    BTB	Add feature group functionality.
*   06/01/16 1.2    REW	Rename maintainer to admin.
*   22/12/15 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_ACCESS_CONTROL_H
#define _SPX_ACCESS_CONTROL_H

/*
 * Other headers required.
 */
/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxError.h"

/* Callback support. */
#include "SPxLibUtils/SPxCallbackList.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Max length of password.  Must be multiple of 8. */
#define	SPX_ACCESS_CONTROL_MAXLEN	64	/* Max num characters */

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

/* Forward declare classes in case headers are in the wrong order. */
class SPxCallbackList;


/*
 * Define our class, derived from SPx base object.
 */
class SPxAccessControl :public SPxObj
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxAccessControl(void);
    virtual ~SPxAccessControl();

    /* Password setting. */
    SPxErrorCode SetAdminPassword(const char *password, int hashed=FALSE);
    int IsPasswordSet(void);

    /* Lock/unlock actions. */
    SPxErrorCode Unlock(const char *password);
    SPxErrorCode CheckPassword(const char *password) const;
    SPxErrorCode Lock(void);

    /* Status retrieval. */
    int IsAdmin(void) const { return(m_isAdmin); }

    /* Event callbacks for lock/unlocks. */
    SPxErrorCode AddEventCallback(SPxCallbackListFn_t fn, void *userArg);
    SPxErrorCode RemoveEventCallback(SPxCallbackListFn_t fn, void *userArg);

    /* Set/get if UAC is enabled on a feature group. */
    SPxErrorCode SetFeatureGroupEnabled(unsigned int group, int enabled);
    SPxErrorCode GetFeatureGroupEnabled(unsigned int group, int *enabled);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private fields.
     */
    /* Current status. */
    int m_isAdmin;			/* TRUE if admin unlocked */

    /* Password. */
    char *m_hashedPassword;		/* Encrypted password, or null/empty */

    /* Field storing feature groups (from 0-63) currently enabled for UAC. */
    unsigned long long m_featureGroupsEnabled;	/* By default all enabled. */

    /* Callback list for state changes. */
    SPxCallbackList *m_callbackList;

    /*
     * Private functions.
     */
    SPxErrorCode hashString(const char *str, char *buf, int bufLen) const;
}; /* SPxAccessControl class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_ACCESS_CONTROL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
