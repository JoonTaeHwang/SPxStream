/*********************************************************************
*
* (c) Copyright 2012 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxWorldMapRenderer base class which supports the 
*   display of a world vector shoreline map.
*
*   NOTE: This class differs from other SPx renderering classes
*         as it is not derived from SPxRenderer. Instead all view
*         control is performed using the associated WorldMap object.
*
* Revision Control:
*   04/06/21 v1.12   AGC	Add anti-alias option.
*
* Previous Changes:
*   25/06/18 1.11   AGC	Correctly suppress cppcheck warning.
*   16/05/17 1.10   AGC	Handle view control callback with zero flags.
*   24/09/14 1.9    SP 	Add additional Get...() functions.
*   08/11/13 1.8    AGC	Fix cppcheck warnings.
*   04/10/13 1.7    AGC	Simplify headers.
*   27/09/13 1.6    AGC	Add pure virtual Set/GetRenderHighAndLowRes().
*   06/06/13 1.5    AGC	Make window size variables protected.
*   08/05/12 1.4    SP 	Do not buffer platform lat/long.
*   30/04/12 1.3    SP 	Improve update synchronisation.
*                       Remove fill land areas option.
*   19/04/12 1.2    SP 	Fix multi-window issues.
*                       Add fill land areas option.
*   08/02/12 1.1    SP 	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_WORLD_MAP_RENDERER_H
#define _SPX_WORLD_MAP_RENDERER_H

/*
 * Other headers required.
 */

/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxStrings.h"
#include "SPxLibUtils/SPxWorldMap.h"
#include "SPxLibUtils/SPxWorldMapFeature.h"

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

/* Forward declare other classes required. */
class SPxWorldMap;
class SPxViewControl;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxWorldMapRenderer : virtual public SPxObj
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxWorldMapRenderer(SPxWorldMap *worldMap);
    virtual ~SPxWorldMapRenderer(void);

    /* Access. */
    virtual SPxWorldMap *GetWorldMap(void) const;

    /* Install a view control object. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetViewControl(SPxViewControl *obj);
    virtual SPxViewControl *GetViewControl(void) const;

    /* The following overrides a function in the base class. */
    virtual void SetActive(int active);

    /* Implemented in derived classes. */
    virtual SPxErrorCode SetRenderHighAndLowRes(int enable)=0;
    virtual int GetRenderHighAndLowRes(void)=0;

    SPxErrorCode SetAntiAliasEnabled(int state);
    int GetAntiAliasEnabled(void) const;

    /* Render the world map. */
    virtual SPxErrorCode Render(void);

protected:
    /*
     * Protected variables.
     */

    /* World map data object. */
    SPxWorldMap *m_worldMap;

    /* Installed view control object. */
    SPxViewControl *m_viewCtrl;

    /* Current view obtained from SPxViewControl. */
    unsigned int m_winWidthPixels;
    unsigned int m_winHeightPixels;
    double m_viewPixelsPerMetre;
    double m_viewCXMetres;
    double m_viewCYMetres;
    double m_dispRotationDegs;
    int m_antiAliasEnabled;

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, 
                             char *parameterValue);

    virtual int GetParameter(char *parameterName, 
                             char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */

    static int viewControlHandler(void *invokingObj,
                                  void *contextPtr,
				  void *changeFlagsPtr);

}; /* SPxWorldMapRenderer */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_WORLD_MAP_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
