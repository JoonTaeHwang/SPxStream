/*********************************************************************
 *
 * (c) Copyright 2014 - 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxStamp class header.
 *
 * Revision Control:
 *   03/01/20 v1.11  SP 	Add status to log event.
 *
 * Previous Changes:
 *   02/01/20 1.10  SP 	Add system stamp IDs.
 *   29/09/16 1.9   SP 	Add isDatabaseLocked flag to Log().
 *   22/01/16 1.8   SP 	Use UINT8 for channel ID.
 *   26/02/15 1.7   SP  Support config load/save.
 *   05/02/15 1.6   SP  Write text to event structure.
 *   26/01/15 1.5   SP  Change how hidden stamps are created.
 *   07/01/15 1.4   SP  Support lat/long position.
 *   21/11/14 1.3   SP  Further development.
 *   11/11/14 1.2   SP  Further development.
 *   06/11/14 1.1   SP  Initial version.
 *
 *********************************************************************/

#ifndef _SPX_STAMP_H
#define _SPX_STAMP_H

/* Other headers required. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxViewControl.h" /* For SPxLatLong_t */
#include "SPxLibData/SPxRib.h"
#include "SPxLibData/SPxPim.h"

/*********************************************************************
 *
 *  Constants
 *
 *********************************************************************/

/* Name to use for stamp objects. This will be prefixed with 
 * the database object name and followed by the stamp ID.
*/
#define SPX_STAMP_OBJ_NAME            "Stamp"

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

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Forward declare any classes required. */
class SPxStampDatabase;
class SPxRIB;
class SPxPIM;
class SPxRunProcess;

class SPxStamp : public SPxObj
{
    /* Grant SPxStampDatabase private access. */
    friend class SPxStampDatabase;

public:

    /*
     * Public types.
     */

    /* Stamp type. */
    typedef enum
    {
        TYPE_NONE = 0,       /* Invalid stamp type. */
        TYPE_USER = 1,       /* User stamps (circle) . */
        TYPE_SYSTEM = 2,     /* System stamps (triangle). */
        MAX_NUM_STAMP_TYPES  /* Must be last entry. */

    } Type_t;

    /*
     * Public variables.
     */

    /* User-defined stamp IDs are allocated in the range 0 to 999.
     * The following IDs are reserved for system stamps. System
     * stamps are used to log records with type set to 
     * SPX_PACKET_RECORD_TYPE_STAMP_SYSTEM. Logging of system 
     * stamp records doesn't need to use this class, however this 
     * is still probably the best place to store these IDs.
     */
    static const unsigned int SYSTEM_ID_SESSION_START       = 1000;
    static const unsigned int SYSTEM_ID_SESSION_STOP        = 1001;
    static const unsigned int SYSTEM_ID_SESSION_STATUS      = 1002;
    static const unsigned int SYSTEM_ID_CHANNEL_ADD         = 1003;
    static const unsigned int SYSTEM_ID_CHANNEL_REMOVE      = 1004;
    static const unsigned int SYSTEM_ID_CHANNEL_STATUS      = 1005;
    static const unsigned int SYSTEM_ID_RADAR_PRF_CHANGE    = 1006;
    static const unsigned int SYSTEM_ID_RADAR_PERIOD_CHANGE = 1007;

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxStamp(SPxStampDatabase *database, 
             Type_t type=TYPE_USER,
             unsigned int id=0);
    SPxErrorCode Create(void);
    virtual ~SPxStamp(void);

    /* Access. */
    virtual SPxStampDatabase *GetDatabase(void) { return m_stampDB; }

    /* Stamp configuration. */
    virtual Type_t GetType(void) { return m_type; }
    virtual unsigned int GetID(void) const { return m_id; }
    virtual SPxErrorCode SetDisplayName(const char *name);
    virtual SPxErrorCode GetDisplayName(char *buf, unsigned int bufSizeBytes);
    virtual SPxErrorCode SetChanID(UINT8 id);
    virtual UINT8 GetChanID(void) const { return m_chanID; }
    virtual SPxErrorCode SetRGB(UINT32 rgb);
    virtual UINT32 GetRGB(void) const { return m_rgb; }
    virtual SPxErrorCode SetDefaultText(const char *text);
    virtual SPxErrorCode GetDefaultText(char *buf, unsigned int bufSizeBytes);
    virtual SPxErrorCode SetDefaultLatLong(const SPxLatLong_t *latLong);
    virtual SPxErrorCode GetDefaultLatLong(SPxLatLong_t *latLong, 
                                           int *isSet=NULL);

    /* User data. */
    virtual SPxErrorCode SetUserData(void *obj, void *data);
    virtual void *GetUserData(void *obj);

    /* Logging. */
    virtual SPxErrorCode Log(const SPxTime_t *time=NULL,
                             const SPxLatLong_t *latLong=NULL,
                             const char *text=NULL,
                             int isDatabaseLocked=FALSE);
    
    /* Colour. */


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

    /* Add stamp to database. */
    virtual SPxErrorCode AddToDatabase(void);

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    /* Used to store user data in an array. */
    typedef struct
    {
        void *obj;      /* Object that installed the data. */
        void *data;     /* The object's user data. */
        
    } UserItem_t;

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Parent stamp database. */
    SPxStampDatabase *m_stampDB;
   
    /* General. */
    unsigned int m_id;              /* Unique ID (0 to 999). */
    UINT32 m_rgb;                   /* Colour to use for displaying stamp. */
    UINT8 m_chanID;                 /* ID of linked channel or zero. */
    int m_isDefaultLatLongSet;      /* Is default lat/long set? */
    SPxLatLong_t m_defaultLatLong;  /* Default lat/long. */
    Type_t m_type;                  /* Stamp type. */

    /*
     * Private static variables.
     */

    /*
     * Private functions.
     */

    UserItem_t *getUserItem(void *obj);

    /*
     * Private static functions.
     */

}; /* SPxStamp */

#endif /* _SPX_STAMP_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
