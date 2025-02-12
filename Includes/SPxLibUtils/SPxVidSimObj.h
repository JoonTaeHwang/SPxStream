/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimObj class header.
 *
 * Revision Control:
 *   27/09/22 v1.27   AGC	Support range finders.
 *
 * Previous Changes:
 *   27/09/22 1.26  AGC	Rename SPxVidSimCameraMount to SPxVidSimMount.
 *   09/09/22 1.25  SP 	Rework display object types.
 *   30/08/22 1.24  SP 	Fix shutdown issues.
 *   05/08/22 1.23  SP 	Add more camera events.
 *   04/08/22 1.22  SP 	Fix build warning.
 *   04/08/22 1.21  SP 	Add DeleteChildren().
 *                     	Support object creation event.
 *   01/08/22 1.20  SP 	Rework code to remove const casts.
 *   29/07/22 1.19  SP 	Add GetTopMovable().
 *   27/07/22 1.18  SP 	Fix build warning.
 *   26/07/22 1.17  SP 	Add object deletion event.
 *   26/07/22 1.16  SP 	Add object deletion event.
 *   22/07/22 1.15  SP 	Remove trailing comma.
 *   22/07/22 1.14  SP 	Changes to simplify creation and deletion.
 *   14/07/22 1.13  SP 	Support unique ID and display name.
 *                     	Renumber events.
 *   10/08/18 1.12  AGC	Make various functions const.
 *   25/06/18 1.11  SP 	Rename event.
 *   10/10/17 1.10  AGC	Remove GL header dependency.
 *   01/12/16 1.9   SP 	Move ZRefMode_t here.
 *   22/11/16 1.8   SP 	Add SPX_VID_SIM_EVENT_CAMERA_MOUNT_PRESET.
 *   31/10/16 1.7   SP 	Move TriStripPoint_t to SPxVidSimTerrainRenderer.h.
 *                     	Add SPX_VID_SIM_EVENT_SCENE_TERRAIN_MAP.
 *   13/10/16 1.6   SP 	Move some utility functions here.
 *   07/10/16 1.5   SP 	Move event types here.
 *   14/10/15 1.4   SP  Add NUM_COLOUR_MODES.
 *   06/10/15 1.3   SP 	Add ColourMode_t.
 *   04/10/13 1.2   AGC	Simplify headers.
 *   02/07/13 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_OBJ_H
#define _SPX_VID_SIM_OBJ_H

/* Other headers required. */
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxCallbackList.h"

/*********************************************************************
 *
 *  Constants
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

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Declare all dervied classes. */
class SPxVidSimMovable;
class SPxVidSimScene;
class SPxVidSimPlatform;
class SPxVidSimRadar;
class SPxVidSimMount;
class SPxVidSimCamera;
class SPxVidSimRangeFinder;
class SPxVidSimModelDatabase;
class SPxVidSimModel;
class SPxVidSimDispGL;

class SPxVidSimObj : public SPxObj
{
public:

    /*
     * Public types.
     */

    /* Object type. */
    enum ObjectType_t
    {
        OBJECT_TYPE_NONE = 0,   /* No object or any object. */
        OBJECT_TYPE_OTHER,      /* Generic or unnamed object. */
        OBJECT_TYPE_SCENE,
        OBJECT_TYPE_PLATFORM,
        OBJECT_TYPE_RADAR,
        OBJECT_TYPE_MOUNT,
        OBJECT_TYPE_CAMERA,
        OBJECT_TYPE_RANGE_FINDER,
        OBJECT_TYPE_MODEL_DATABASE,
        OBJECT_TYPE_MODEL,
        OBJECT_TYPE_DISPLAY,    /* Display derived from SPxVidSimDispGL. */
        OBJECT_TYPE_STREAM,     /* Stream derived from SPxVidSimDispGL. */
        OBJECT_TYPE_TARGET_RENDERER_GL,
        OBJECT_TYPE_MODEL_RENDERER_GL,
        OBJECT_TYPE_TERRAIN_RENDERER_GL,
        MAX_OBJECT_TYPE         /* MUST BE LAST ENTRY. */
    };

    /* Event type. */
    enum EventType_t
    {
        /* Object events (100 to 199). */
        EVENT_OBJ_CREATE                    = 100,
        EVENT_OBJ_DELETE                    = 101,

        /* Moveable events (200 to 299). */
        EVENT_MOVEABLE_POSITION_LATLONG     = 200,
        EVENT_MOVEABLE_POSITION_XYZ         = 201,
        EVENT_MOVEABLE_ROTATION             = 202,

        /* Scene events (300 to 399). */
        EVENT_SCENE_SIZE                    = 300,
        EVENT_SCENE_APPEARANCE              = 301,
        EVENT_SCENE_TERRAIN_MESH            = 302,
        EVENT_SCENE_TERRAIN_MAP             = 303,
        EVENT_SCENE_TERRAIN_COLOUR          = 304,

        /* Radar events (400 to 499). */

        /* Mount events (500 to 599). */
        EVENT_MOUNT_PRESET                  = 500,

        /* Camera events (600 to 699). */
        EVENT_CAMERA_ZOOM                   = 600,
        EVENT_CAMERA_FOCUS                  = 601,
        EVENT_CAMERA_FRAME_SIZE             = 602,
        EVENT_CAMERA_FRAME_RATE             = 603,
        EVENT_CAMERA_COLOUR_MODE            = 604
    };

    /* Event info. */
    struct EventInfo_t
    {
        EventType_t eventType;       /* Event type. */
        SPxVidSimObj *reporter;      /* Reporting object. */
    };

    /* Colour modes. */
    enum ColourMode_t
    {
        COLOUR_MODE_COLOUR = 0,
        COLOUR_MODE_GREYSCALE = 1,
        NUM_COLOUR_MODES /* MUST be last entry. */

    };

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxVidSimObj(ObjectType_t type, SPxVidSimObj *parent);
    virtual ~SPxVidSimObj(void);

    /* General. */
    ObjectType_t GetObjectType(void) const { return m_objectType; }
    UINT32 GetID(void) const { return m_id; }
    unsigned int GetIndex(void) const;

    /* Downcasting. */
    SPxVidSimMovable *GetAsMovable(void);
    SPxVidSimScene *GetAsScene(void);
    SPxVidSimPlatform *GetAsPlatform(void);
    SPxVidSimRadar *GetAsRadar(void);
    SPxVidSimMount *GetAsMount(void);
    SPxVidSimCamera *GetAsCamera(void);
    SPxVidSimRangeFinder *GetAsRangeFinder(void);
    SPxVidSimModel *GetAsModel(void);
    SPxVidSimDispGL *GetAsDispGL(void);

    /* Parent and ancestor access. */
    SPxVidSimObj *GetParent(void) const { return m_parent; }
    SPxVidSimObj *GetTopAncestor(void);
    SPxVidSimMovable *GetTopMovable(void);
    SPxVidSimScene *GetAncestorScene(void);
    const SPxVidSimScene *GetAncestorScene(void) const;
    SPxVidSimPlatform *GetAncestorPlatform(void);
    SPxVidSimMount *GetAncestorMount(void);
    SPxVidSimModel *GetAncestorModel(void);
    SPxVidSimDispGL *GetAncestorDispGL(void);
    int IsAncestor(SPxVidSimObj *obj) const;

    /* Child objects. */
    unsigned int GetNumChildren(ObjectType_t type) const;
    SPxVidSimObj *GetChildByIndex(ObjectType_t type, unsigned int index);
    SPxVidSimObj *GetChildByID(UINT32 id);

    /* Display name. */
    SPxErrorCode SetDisplayName(const char *name);
    const char *GetDisplayName(void) const;

protected:

    /*
     * Protected types.
     */

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Utility functions. */
    void SetObjectName(const char *basename);
    void GetSurfaceNormal(const float v1[], const float v2[], const float v3[], float n[]) const;
    float GetRed(UINT32 rgb) const;
    float GetGreen(UINT32 rgb) const;
    float GetBlue(UINT32 rgb) const;

    /* Event reporting. */
    void SetEventsEnabled(int enabled) { m_areEventsEnabled = enabled; }
    int AreEventsEnabled(void) { return m_areEventsEnabled; } 
    void ReportEvent(EventType_t eventType);
    void ForwardEvent(const EventInfo_t *eventInfo);

    /* Event handling. */
    virtual void HandleEvent(const EventInfo_t *eventInfo);

    /* Shutdown. */
    virtual void Shutdown(void);
    void DeleteChildren(ObjectType_t type);

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;
    ObjectType_t m_objectType;
    UINT32 m_id;
    int m_isDisplayNameSet;
    SPxVidSimObj *m_parent;
    int m_areEventsEnabled;
    int m_isShutdownCalled;
    UINT32 m_nextChildID[SPxVidSimObj::MAX_OBJECT_TYPE];

    /*
     * Private functions.
     */

    /* Add or remove children. */
    SPxErrorCode addChild(SPxVidSimObj *child);
    SPxErrorCode removeChild(SPxVidSimObj *child);

    /*
     * Private static functions.
     */

    static int eventHandler(void *invokingObj,
                            void *userArg,
                            void *eventInfoPtr);

}; /* SPxVidSimObj */

#endif /* _SPX_VID_SIM_OBJ_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
