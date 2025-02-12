/*********************************************************************
*
* (c) Copyright 2012 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxENCRendererWin class, used to render charts
*   provided by the SPxENCManagerWin class.
*
* Revision Control:
*   17/10/19 v1.11   AGC	Make Set/GetParameter public.
*
* Previous Changes:
*   02/07/19 1.10   AGC	Move private classes to source.
*   06/07/18 1.9    BTB	Add multi-language support.
*   26/09/17 1.8    SP 	Rework to support hierarchy of 
*                       viewing and text groups.
*   13/07/17 1.7    SP 	Add support for group levels.
*                       Add support for short group names.
*   07/10/13 1.6    AGC	Move rendering to renderAll().
*   13/07/12 1.5    SP 	Add missing override functions.
*   29/03/12 1.4    SP 	Add viewing/text group controls.
*   08/03/12 1.3    SP 	Add colour control.
*                       Add GetDepthMetresAtPixel().
*                       Add parameter handling.
*   20/02/12 1.2    SP 	Store ENCX objects as opaque pointers.
*                       Do not import encxcom.tlb from header.
*   08/02/12 1.1    SP 	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_ENC_RENDERER_WIN_H
#define _SPX_ENC_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base class. */
#include "SPxLibWin/SPxRendererWin.h"

/* We need the ENC manager class. */
#include "SPxLibWin/SPxENCManagerWin.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Maximum number of viewing group levels. */
#define SPX_ENC_NUM_VIEWING_GROUP_LEVELS 3

/* Maximum number of text group levels. */
#define SPX_ENC_NUM_TEXT_GROUP_LEVELS 2

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

/* Need to forward-declare this class as it's used internally. */
class SPxENCRendererWin;

/* Need to forward-declare other classes in case headers in wrong order. */
class SPxRenderer;
class SPxRendererWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxENCRendererWin : public SPxRendererWin
{
public:

    /*
     * Public types.
     */
    struct GroupInfo_t;

    typedef unsigned int ViewingGroupID_t;
    typedef unsigned int TextGroupID_t;

    /* Colour scheme to use for display. */
    typedef enum
    {
        PALETTE_UNKNOWN = 0,
        PALETTE_DAY = 1,
        PALETTE_DUSK = 2,
        PALETTE_NIGHT = 3,
        PALETTE_NIGHT_UNFILTERED = 4

    } Palette_t;

    /* Declutter level. */
    typedef enum
    {
        DECLUTTER_NONE = 0,
        DECLUTTER_L1 = 1,
        DECLUTTER_MAX = 99,
        DECLUTTER_UNKNOWN = 100

    } DeclutterLevel_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxENCRendererWin(void);
    virtual ~SPxENCRendererWin(void);

    /* Configuration. */
    SPxErrorCode SetManager(SPxENCManagerWin *manager);
    SPxENCManagerWin *GetManager(void) { return m_manager; }

    SPxErrorCode SetOverlaySlave(SPxENCRendererWin *obj);
    SPxENCRendererWin *GetOverlaySlave(void) { return m_overlaySlave; }

    unsigned int GetViewingGroupLevel(ViewingGroupID_t id);

    SPxErrorCode SetViewingGroupRangeEnabled(ViewingGroupID_t firstID,
                                             ViewingGroupID_t lastID,
                                             BOOL isEnabled);

    SPxErrorCode SetViewingGroupEnabled(ViewingGroupID_t id,
                                        BOOL isEnabled);
    BOOL IsViewingGroupEnabled(ViewingGroupID_t id);

    SPxErrorCode SetViewingGroupOverlay(ViewingGroupID_t id,
                                        BOOL isOverlay);
    BOOL IsViewingGroupOverlay(ViewingGroupID_t id);

    SPxErrorCode GetViewingGroupName(ViewingGroupID_t id,
                                     char *bufPtr,
                                     unsigned int bufSizeBytes);

    SPxErrorCode GetViewingGroupDesc(ViewingGroupID_t id,
                                     char *bufPtr,
                                     unsigned int bufSizeBytes);

    unsigned int GetTextGroupLevel(TextGroupID_t id);

    SPxErrorCode SetTextGroupRangeEnabled(TextGroupID_t firstID,
                                          TextGroupID_t lastID,
                                          BOOL isEnabled);
    
    SPxErrorCode SetTextGroupEnabled(TextGroupID_t id,
                                     BOOL isEnabled);
    BOOL IsTextGroupEnabled(TextGroupID_t id);
    
    SPxErrorCode SetTextGroupOverlay(TextGroupID_t id,
                                     BOOL isOverlay);
    BOOL IsTextGroupOverlay(TextGroupID_t id);

    SPxErrorCode GetTextGroupName(TextGroupID_t id,
                                  char *bufPtr,
                                  unsigned int bufSizeBytes);
    
    SPxErrorCode GetTextGroupDesc(TextGroupID_t id,
                                  char *bufPtr,
                                  unsigned int bufSizeBytes);
      
    SPxErrorCode SetPalette(Palette_t pal);
    SPxErrorCode GetPalette(Palette_t *pal);

    SPxErrorCode SetSaturation(double val);
    SPxErrorCode GetSaturation(double *valRtn);

    SPxErrorCode SetLuminance(double val);
    SPxErrorCode GetLuminance(double *valRtn);

    SPxErrorCode SetDeclutterLevel(DeclutterLevel_t level);
    SPxErrorCode GetDeclutterLevel(DeclutterLevel_t *levelRtn);

    SPxErrorCode GetDepthMetresAtPixel(int xPixels, 
                                       int yPixels, 
                                       double *depthRtn);

    /* The following functions override functions in the base class. */
    SPxErrorCode SetViewCentreMetres(double xMetres,
                                     double yMetres);

    SPxErrorCode SetViewCentreLatLong(double latDegs,
                                      double longDegs);

    SPxErrorCode SetViewSizePixels(unsigned int widthPixels,
                                   unsigned int heightPixels);

    SPxErrorCode SetViewPixelsPerMetre(double pixelsPerMetre);

    SPxErrorCode SetRadarPositionMetres(double xMetres, 
                                        double yMetres);

    SPxErrorCode SetRadarPositionLatLong(double latDegs,
                                         double longDegs);

    SPxErrorCode SetRotationDegs(double degs);

    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /*
     * Public static functions.
     */

    static unsigned int GetNumViewingGroups(void);
    static SPxErrorCode GetViewingGroupAtIndex(unsigned int index,
                                               ViewingGroupID_t *id);

    static unsigned int GetNumTextGroups(void);
    static SPxErrorCode GetTextGroupAtIndex(unsigned int index,
                                            TextGroupID_t *id);

private:

    /*
     * Private types.
     */
    struct Group_t;

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxENCManagerWin *m_manager;  /* Handle of ENC manager. */
    SPxENCRendererWin *m_overlaySlave; /* Overlay renderer. */

    /* The following ENCX objects are stored as opaque pointers
     * to avoid placing a dependency on encxcom.tlb the library.
     */
    IUnknown *m_encxDraw; /* ENCX chart drawing object. */
    IUnknown *m_encxDisp; /* ENCX display settings. */
    IUnknown *m_encxPalette; /* ENCX display palette. */

    /*
     * Private functions.
     */

    Group_t *getViewingGroup(ViewingGroupID_t id);
    Group_t *getTextGroup(TextGroupID_t id);
    unsigned int getViewingGroupLevelFromID(ViewingGroupID_t id);
    unsigned int getTextGroupLevelFromID(TextGroupID_t id);
    SPxErrorCode applyViewingGroupSettings(void);
    SPxErrorCode applyTextGroupSettings(void);
    SPxErrorCode applyAllGroupSettings(void);
    SPxErrorCode renderAll(void);

}; /* SPxENCRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_ENC_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
