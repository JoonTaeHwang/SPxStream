/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAISImageDb class which supports the download
*	and handling of AIS images.
*
*
* Revision Control:
*   05/02/18 v1.2    REW	Add CheckCanTryFetch().
*
* Previous Changes:
*   02/02/18 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_AIS_IMAGE_DB_H
#define _SPX_AIS_IMAGE_DB_H

/*
 * Other headers required.
 */
/* Base class. */
#include "SPxLibUtils/SPxObj.h"

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

/* Need to forward-declare some classes we use. */
class SPxCriticalSection;
class SPxAISTrack;

/*
 * Define our class, derived from the normal base class.
 */
class SPxAISImageDb :public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxAISImageDb(void);
    virtual ~SPxAISImageDb(void);

    /* Status. */
    SPxErrorCode CheckCanTryFetch(void);

    /* Key handling. */
    SPxErrorCode SetKey(const char *key);
    const char *GetKey(void) const { return(m_key); }	/* May be NULL */

    /* Image retrieval. */
    SPxErrorCode GetImageForTrack(SPxPath *path, SPxAISTrack *track,
							int fetch=FALSE);
    SPxErrorCode GetImageForIMO(SPxPath *path, UINT32 imo, int fetch=FALSE);

    /* Generic parameter assignment. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:

private:
    /*
     * Private variables.
     */
    /* Control. */
    SPxCriticalSection *m_mutex;	/* For protection */

    /* Key handling. */
    char *m_key;			/* Key, or NULL if not set */

    /* Internal memory. */
    unsigned char *m_buf;		/* Allocated memory */
    unsigned int m_bufSizeBytes;	/* Size of allocated memory */

    /*
     * Private functions.
     */

}; /* SPxAISImageDb */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_AIS_IMAGE_DB_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
