/*********************************************************************
 *
 * (c) Copyright 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget picture base class.
 *
 * Revision Control:
 *  24/02/22 v1.1    AGC	Initial version extracted from SPxGwPicture.
 *
 * Previous Changes:
 *
 *********************************************************************/

#ifndef _SPX_GW_PICTURE_BASE_H
#define _SPX_GW_PICTURE_BASE_H

/* Other headers required. */
#include "SPxLibUtils/SPxAreaObj.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxGwObj.h"

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

/* Forward declarations. */
class SPxGwWidgetGroup;
class SPxGwWidget;
class SPxViewControl;

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/* Function types. */
typedef SPxErrorCode (*SPxGwWidgetGroupFn)(SPxGwWidgetGroup *widgetGroup, 
                                           void *userArg);

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Define our class. */
class SPxGwPictureBase : public SPxGwObj, public SPxAreaObj
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwPictureBase(void);
    virtual ~SPxGwPictureBase(void);

    /* Configuration. */
    virtual SPxErrorCode AddWidgetGroupAbove(SPxGwWidgetGroup *group,
                                             SPxGwWidgetGroup *above=NULL);

    virtual SPxErrorCode AddWidgetGroupBelow(SPxGwWidgetGroup *group,
                                             SPxGwWidgetGroup *below=NULL);

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode RemoveWidgetGroup(SPxGwWidgetGroup *group);

    virtual SPxErrorCode DeleteAllWidgetGroups(void);

    virtual SPxErrorCode AddEventCallback(SPxAreaEventFn fn, void *userArg);
    virtual SPxErrorCode RemoveEventCallback(SPxAreaEventFn fn, void *userArg);

    /* View control. */
    virtual SPxErrorCode SetViewControl(SPxViewControl *viewCtrl);
    virtual SPxErrorCode AddViewControl(SPxViewControl *viewCtrl);
    virtual SPxErrorCode RemoveViewControl(SPxViewControl *viewCtrl);
    virtual void RequestRedraw(void);
    virtual SPxErrorCode SetPulseEnabled(int enabled);
    virtual int IsPulseEnabled(void) const;
    virtual SPxErrorCode SetAutoReloadEnabled(int enabled);
    virtual int IsAutoReloadEnabled(void) const;

    /* Rendering. */
    virtual SPxErrorCode SetShowItemNames(int showItemNames);
    virtual int IsShowItemNamesEnabled(void) const;

    /* SPxAreaObj interface. */
    virtual int GetAreaType(void) const { return SPX_GW_TYPE_PICTURE; }
    virtual const char *GetItemName(void) const;

    virtual unsigned int GetNumTypes(void) const;
    virtual const char *GetTypeName(unsigned int typeIndex) const;
    virtual SPxAreaObj *CreateArea(unsigned int typeIndex);
    virtual SPxErrorCode DeleteAreas(void);
    virtual SPxErrorCode AddArea(SPxAreaObj *childArea, SPxAreaObj *aboveArea=NULL);
    virtual SPxErrorCode RemoveArea(SPxAreaObj *childArea);

    virtual SPxErrorCode IterateObjects(SPxAreaObjFn fn, void *userArg) const;
    virtual SPxErrorCode IterateWidgetGroups(SPxGwWidgetGroupFn fn, void *userArg) const;

    virtual int IsSaveSupported(void) const { return TRUE; }
    virtual SPxErrorCode Load(const char *name);
    virtual SPxErrorCode Save(const char *name) const;
    virtual const char *GetFullPath(void) const;
    virtual const char *GetShortPath(void) const;

    /* Named colours. */
    static SPxErrorCode AddNamedColour(const char *name, UINT32 argb);
    static SPxErrorCode AddNamedColour(const char *name, 
                                       UINT8 r, UINT8 g, 
                                       UINT8 b, UINT8 a=255);
    static SPxErrorCode GetNamedColour(const char *name, UINT32 *argbRtn);

    /* DXF access functions. */
    virtual SPxErrorCode SetDxfUnitsPerMetre(double unitsPerMetre);
    virtual double GetDxfUnitsPerMetre(void) const;
    virtual SPxErrorCode SetDxfOffsetMetres(double xOffset, double yOffset);
    virtual void GetDxfOffsetMetres(double *xOffset, double *yOffset) const;
    virtual SPxErrorCode SetDxfOffsetLatLong(double latOffset, double lonOffset);
    virtual void GetDxfOffsetLatLong(double *latOffset, double *lonOffset) const;
    virtual SPxErrorCode SetDxfRotationDegs(double rotation);
    virtual double GetDxfRotationDegs(void) const;

    /* Unsaved changes? */
    virtual int HasChanges(void) const;

    virtual void ForceShowDirectionIndicators(int show);

    /* JSON retrieval. */
    virtual int GetJSON(char *buf, int buflen) const;

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    struct EventEnabler;
    struct DxfLayer;
    struct IterateParams;
    struct impl;
    SPxAutoPtr<impl> m_p;	/* Private structure. */

    DxfLayer *m_dxfLayer;
    unsigned int m_dxfLayerCount;
    unsigned int m_dxfAngDir;
    double m_dxfUnitsPerMetre;
    double m_dxfOffsetXMetres;
    double m_dxfOffsetYMetres;
    double m_dxfOffsetLat;
    double m_dxfOffsetLon;
    double m_dxfRotationDegs;

    /* Unsaved changes? */
    mutable int m_haveChanges;

    /* Show gate crossing direction indicators? */
    int m_forceShowDirection;

    /* Private functions. */
    SPxGwWidgetGroup *getNextGroup(void);
    void enableEventCallbacks(int enable);
    void callEventCallbacks(SPxGwWidget *widget, UINT32 flags, int addCallbacks);
    void redraw(void);
    void reload(void);
    SPxErrorCode deleteAreas(void);

    static SPxErrorCode iterateFn(SPxAreaObj *obj, void *userArg);
    static SPxErrorCode iterateFnJSON(SPxAreaObj *obj, void *userArg);

    /* XML file importing. */
    SPxErrorCode loadXmlFile(FILE *handle, const char *filename);
    SPxErrorCode createXmlLayer(void *node, 
                                const char *units, 
                                const char *filename);
    SPxGwWidget *createXmlPolygon(void *node, SPxGwWidgetGroup *group);
    double getXmlNodeAttrDouble(void *node,
                                const char *attr,
                                double defaultValue=0.0);
    const char *getXmlNodeAttrText(void *node,
                                   const char *attr,
                                   const char *defaultValue="");
    UINT32 getXmlNodeAttrRGB(void *node,
                             const char *attr,
                             UINT32 defaultValue=0);
    int isXmlNodeAttrTrueOrFalse(void *node, 
                                 const char *attr,
                                 int defaultState=FALSE);

    /* DXF file importing. */
    SPxErrorCode loadDxfFile(FILE *fd, const char *filename);
    SPxGwWidgetGroup *createDxfLayer(FILE *fd);
    SPxGwWidget *createDxfLine(FILE *fd, unsigned int numPoints = 0);
    SPxGwWidget *createDxfHatch(FILE *fd);
    SPxGwWidget *createDxfArc(FILE *fd);
    SPxGwWidget *createDxfText(FILE *fd);
    int getDxfCode(FILE *fd, int *code, char **value);

    DxfLayer *getDxfLayer(const char *name);
    void applyDxfTransform(double *x, double *y);

}; /* SPxGwPictureBase */

#endif /* _SPX_GW_PICTURE_BASE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

