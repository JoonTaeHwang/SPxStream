/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAeroDb class.
*
* Revision Control:
*   16/10/20 v1.4    REW	Add support for NDBs.
*
* Previous Changes:
*   15/10/20 1.3    REW	Add support for VORs.
*   14/10/20 1.2    REW	Add support for corridors.
*   13/10/20 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_AERO_DB_H
#define _SPX_AERO_DB_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxAeroNavAid.h"


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

/* Forward-declare other classes we use. */
class SPxAeroNavAid;
class SPxCriticalSection;

/*
 * Define our class.
 */
class SPxAeroDb : public SPxObj
{
public:
    /*
     * Public variables.
     */
    /* Constructor and destructor. */
    SPxAeroDb(void);
    virtual ~SPxAeroDb(void);

    /*
     * Public functions.
     */
    /* Add objects. */
    SPxErrorCode AddNavAid(SPxAeroNavAid *na, int alreadyLocked=FALSE);

    /* Read from file. */
    SPxErrorCode ReadNavAids(const char *filename, int deleteFirst=FALSE);
    SPxErrorCode ReadCorridors(const char *filename, int deleteFirst=FALSE);

    /* Iteration. NB: callbacks must not modifiy the database. */
    SPxErrorCode IterateItems(SPxCallbackListFn_t fn, void *userArg);

    /* Searching etc. */
    SPxAeroNavAid *FindNavAid(const char *name);

    /* Delete content. */
    SPxErrorCode DeleteAll(void);
    SPxErrorCode DeleteNavAids(void);
    SPxErrorCode DeleteCorridors(void);
    SPxErrorCode DeleteType(SPxAeroNavAid::SPxANAType type);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Mutex protection. */
    SPxCriticalSection *m_mutex;	/* Protection mutex  */

    /* Filenames. */
    char *m_filenameCorridors;		/* Copy of filename */
    char *m_filenameNavAids;		/* Copy of filename */

    /*
     * Private functions.
     */

    
}; /* SPxAeroDb */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_AERO_DB_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
