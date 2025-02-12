/*********************************************************************
 *
 * (c) Copyright 2013 - 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx area editing mouse interface - supports mouse editing of
 *   SPxGw... objects and SPxActiveRegion objects.
 *
 * Revision Control:
 *  17/03/20 v1.11   AGC        Support use with multiple renderers.
 *
 * Previous Changes:
 *  31/10/17 1.10   AGC	Use SPxAreaObj event callback.
 *			Use cross cursor for point editing.
 *  25/10/17 1.9    AGC	Fix compiler warning.
 *  23/10/17 1.8    AGC	Improvements to area editing.
 *  04/05/17 1.7    AGC	Fix convertFromPixels() parameter names.
 *  10/09/15 1.6    AGC	Add specification of window handle.
 *  02/07/15 1.5    AGC	Improve area object creation.
 *  24/10/13 1.4    SP 	Allow click margin to be changed.
 *  21/10/13 1.3    AGC	Add creation options to right-click menu.
 *  15/10/13 1.2    AGC	Request renderer redraw when areas changed.
 *  07/10/13 1.1    AGC	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_AREA_EDITOR_MOUSE_H
#define _SPX_AREA_EDITOR_MOUSE_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxAreaObj.h"
#include "SPxLibUtils/SPxMouseControl.h"
#include "SPxLibUtils/SPxObj.h"

/* Forward declare any classes we need. */
class SPxAreaObj;
class SPxPopupMenu;

class SPxAreaEditorMouse : public SPxObj, public SPxMouseInterface
{
public:
    /* Public functions. */
    explicit SPxAreaEditorMouse(SPxAreaObj *obj, SPxRenderer *renderer);
    virtual ~SPxAreaEditorMouse(void);

    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleDoubleClick(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseLeave(void);

    virtual void Repopulate(void) { return; }
    virtual void SyncFromObj(void);
    virtual SPxAreaObj *GetTopObj(void) { return m_obj; }
    virtual void SetCurrentObj(SPxAreaObj *obj, int selectNext=FALSE);
    virtual SPxAreaObj *GetCurrentObj(void);
    virtual void SetCurrentPoint(unsigned int index);
    virtual void SetCreateCurrentObj(int enable, int autoClose);

    virtual void SetClickMarginPixels(unsigned int pixels) { m_clickMarginPx = pixels; }
    virtual unsigned int GetClickMarginPixels(void) { return m_clickMarginPx; }
    virtual void EnableMenu(int enableMenu) { m_enableMenu = enableMenu; }

#ifdef _WIN32
    SPxErrorCode SetWindow(HWND hwnd);
    SPxErrorCode SetMenu(SPxWinMenu *menu);
#endif

    SPxErrorCode AddRenderer(SPxRenderer *renderer, SPxMouseControl* mouseCtrl);
    SPxRenderer *GetRenderer(void) const;

    /* Convert between pixels and area units. */
    void ConvertFromPixels(const SPxAreaObj *obj, int px, int py, double *x, double *y);
    void ConvertToPixels(const SPxAreaObj *obj, double x, double y, int *px, int *py);

protected:
    /* Protected functions. */
    SPxAreaObj *createAreaObj(SPxAreaObj *parent, unsigned int type, int x, int y);

private:
    /* Private variables. */
    SPxAreaObj *m_obj;			    /* Top level object we are controlling. */
    SPxRenderer *m_renderer;		    /* Renderer object (for unit conversions). */
    SPxAreaObj *m_currObj;		    /* Currently selected object. */
    int m_createObj;			    /* Currently creating current object? */
    int m_autoCloseCreateObj;		    /* Auto-close current creation object? */
    unsigned int m_currPoint;		    /* Currently selected point. */
    int m_enableMenu;			    /* Enable context menu? */
#ifdef _WIN32
    HWND m_hwnd;			    /* Window handle for positioning menu. */
    SPxWinMenu *m_menu;			    /* SPx menu to use. */
#endif

    struct impl;
    SPxAutoPtr<impl> m_p;		    /* Private class structure. */
    SPxAreaObj *m_foundSingleObj;	    /* Object found while searching for object to select. */
    int m_isMovePoint;			    /* Is a point being moved? */
    int m_isCreatePoint;		    /* Is a new point being created? */
    int m_isDeletePoint;		    /* Is an existing point being deleted? */
    int m_hasMoved;			    /* Has the selected object been moved? */
    int m_newObj;			    /* New object to select. */

    int m_x;				    /* Current mouse position x. */
    int m_y;				    /* Current mouse position y. */
    int m_refX;				    /* Reference x position for moving objects. */
    int m_refY;				    /* Reference y position for moving objects. */
    unsigned int m_clickMarginPx;	    /* Click margin for finding objects in pixels. */

    /* Private functions. */
    int mouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    void setCursor(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    int selectObjAtPoint(int x, int y, int nextObj);
    void setReference(int x, int y);
    unsigned int findPoint(const SPxAreaObj *obj, int x, int y, unsigned int ignorePoint=SPX_AREA_NO_POINT);
    unsigned int findEdge(const SPxAreaObj *obj, int x, int y);
    int isPointWithin(const SPxAreaObj *obj, int x, int y);
    static void createTop(SPxPopupMenu *menu, int id, void *userArg);
    static void createChild(SPxPopupMenu *menu, int id, void *userArg);
    static void deleteItem(SPxPopupMenu *menu, int id, void *userArg);
    static SPxErrorCode findObjs(SPxAreaObj *obj, void *userArg);
    static SPxErrorCode findSingleObj(SPxAreaObj *obj, void *userArg);
    static SPxErrorCode findObjArg(SPxAreaObj *obj, void *userArg);
    void redraw(void);
    static void areaEventFnWrapper(SPxAreaObj *obj, UINT32 flags, void *userArg);
    void areaEventFn(SPxAreaObj *obj, UINT32 flags);

}; /* SPxAreaEditorMouse */

#endif /* _SPX_AREA_EDITOR_MOUSE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

