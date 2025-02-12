/*********************************************************************
 *
 * (c) Copyright 2019 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxSdbSite class.
 *
 * Revision Control:
 *   05/01/22 v1.18  SP 	Remove ClearDesc().
 *
 * Previous Changes:
 *   09/03/20 1.17  SP 	Correct comments.
 *   04/03/20 1.16  SP 	Add GetStream().
 *   19/02/20 1.15  SP 	Add AppendHTML().
 *   29/01/20 1.14  SP 	Keep UUIDs consistent across all monitors.
 *   28/01/20 1.13  SP 	Support discovery, aggregation and reporting scope.
 *   22/11/19 1.12  SP 	Changes to descriptor mask.
 *   19/11/19 1.11  SP 	Support saving sitemap to config.
 *   18/11/19 1.10  SP 	Mark items as expected when set from config.
 *   15/11/19 1.9   SP 	Don't make public functions virtual.
 *   08/11/19 1.8   SP 	Add ClearDesc().
 *   07/11/19 1.7   SP 	Fix build warning.
 *   06/11/19 1.6   SP 	Improve operation of Compare().
 *   31/10/19 1.5   SP 	Further development.
 *   30/10/19 1.4   SP 	Further development.
 *   29/10/19 1.3   SP 	Support generation of JSON messages.
 *   25/10/19 1.2   SP 	Rename class and further development.
 *   24/10/19 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_SDB_SITE_H
#define _SPX_SDB_SITE_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxSdbItem.h"
#include "SPxLibUtils/SPxSdbSystem.h"

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/* Forward declare classes we use. */
class SPxSiteDatabase;

/*
 *  SPxSdbSite class.
 */
class SPxSdbSite : public SPxSdbItem
{
public:
    /*
     * Public variables.
     */

    /*
     * Public types.
     */

    /* Descriptor static field masks. */
    static const UINT32 DESC_MASK_IS_LOCAL      = 0x00000001;
    static const UINT32 DESC_MASK_NAME          = 0x00000002;
    static const UINT32 DESC_MASK_ALL_STATIC    = 0x0000FFFF;

    /* Descriptor dynamic field masks. */
    static const UINT32 DESC_MASK_ALL_DYNAMIC   = 0xFFFF0000;

    /* All descriptor mask bits. */
    static const UINT32 DESC_MASK_ALL           = 0xFFFFFFFF;

    /* Site descriptor. This consists of both static and 
     * dynamic fields. Dynamic fields also have a last updated 
     * time and may be purged if not updated regularly. Unlike
     * static fields, dynamic fields are not saved to config.
     */
    struct Desc_t
    {
        /* Which of the following fields are set? */
        UINT32 mask;

        /* Static fields. */
        int isLocal;                    /* Local or remote? */
        char name[NAME_MAX_LEN];        /* Site name. */
    };

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSdbSite(SPxSiteDatabase *database,
               const char *uuid,
               const Desc_t *desc);
    virtual ~SPxSdbSite(void);

    /* General. */
    SPxErrorCode UpdateDesc(const Desc_t *desc, UINT32 clearMask=0);
    SPxErrorCode GetDesc(Desc_t *desc);
    int IsLocal(void);
    unsigned int Compare(const char *uuid, const Desc_t *desc);
    SPxSdbSystem *AddOrGetSystem(const char *uuid, 
                                 const SPxSdbSystem::Desc_t *desc, 
                                 int allowCreate);
    SPxSdbStream *GetStream(UINT32 hostAddr, unsigned int senderPort);
    SPxErrorCode HandleJSON(const void *jsonSiteVoid,
                            const MonitorScope_t *scope,
                            EndPoint_t source);
    SPxErrorCode AppendJSON(void *writerVoid, 
                            const MonitorScope_t *scope,
                            EndPoint_t dest);
    SPxErrorCode AppendHTML(char **buf, 
                            unsigned int *bufLen, 
                            unsigned int level,
                            int isExpected);

    /*
     * Public static functions.
     */

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* General. */
    // cppcheck-suppress virtualCallInConstructor
    virtual void updateDisplayName(void);
    virtual SPxErrorCode purgeDesc(double timeoutSecs);

private:

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;       /* Private data. */
    Desc_t m_desc;              /* Our descriptor. */

    /*
     * Private functions.
     */

    /* General. */
    SPxErrorCode addOrUpdateSystem(const void *jsonSysVoid, 
                                   const MonitorScope_t *scope,
                                   EndPoint_t source);

    /*
     * Private static functions.
     */

}; /* SPxSdbSite */

#endif /* _SPX_SDB_SITE_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
