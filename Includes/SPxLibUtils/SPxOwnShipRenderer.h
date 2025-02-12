/*********************************************************************
*
* (c) Copyright 2012 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxOwnShipRenderer base class which supports the display
*   of an own ship marker.
*
* Revision Control:
*   30/11/18 v1.7    AGC	Support configurable heading colour.
*				Support message rate labels.
*				Support display of heading change rate.
*
* Previous Changes:
*   10/12/15 1.6    AGC	Add ShowExpired option.
*   09/11/15 1.5    AGC	Add label displaying own ship pos/speed/course.
*   07/10/13 1.4    AGC	Move rendering to renderAll().
*   11/01/13 1.3    AGC	Add ClearHistory() function.
*   21/12/12 1.2    AGC	Support trails.
*   08/02/12 1.1    SP 	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_OWN_SHIP_RENDERER_H
#define _SPX_OWN_SHIP_RENDERER_H

/*
 * Other headers required.
 */

/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxStrings.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxRenderer.h"

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
class SPxNavData;

/* Structure to store linked list of trail points. */
struct SPxOwnShipTrailPoint
{
    /* Pointer to next item in list. */
    SPxOwnShipTrailPoint *next;

    /* Timestamp when this trail point was created. */
    SPxTime_t timestamp;

    /* Lat/long of trail point. */
    double latDegs;
    double longDegs;
};

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxOwnShipRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxOwnShipRenderer(SPxNavData *navData=NULL);
    virtual ~SPxOwnShipRenderer(void);

    /* Configuration functions. */
    SPxErrorCode SetColour(UINT32 col);
    UINT32 GetColour(void) { return m_colour; }
    SPxErrorCode SetColourHeading(UINT32 col);
    UINT32 GetColourHeading(void) { return m_colourHeading; }
    SPxErrorCode SetTrailLengthSecs(unsigned int secs);
    unsigned int GetTrailLengthSecs(void) { return m_trailLengthSecs; }
    SPxErrorCode ClearHistory(void);
    SPxErrorCode SetLabelEnabled(int state);
    int GetLabelEnabled(void) const;
    SPxErrorCode SetExtraLabel(const char *extraLabel);
    SPxErrorCode SetShowMessageRate(int state);
    int IsShowingMessageRate(void) const;
    SPxErrorCode SetShowHeadingChangeRate(int state);
    int IsShowingHeadingChangeRate(void) const;
    SPxErrorCode SetUseSpeed(int state);
    int IsUsingSpeed(void) const;
    SPxErrorCode SetShowExpired(int showExpired);
    int IsShowingExpired(void) const;

protected:
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

    SPxNavData *m_navData;  /* Navigation data object. */
    UINT32 m_colour;        /* Colour to use. */
    UINT32 m_colourHeading; /* Colour to use for heading line. */
    int m_trailLengthSecs;  /* Trail length in seconds. */
    SPxOwnShipTrailPoint *m_trailHead;	/* Head of trail point list. */
    int m_labelEnabled;
    char *m_extraLabel;
    int m_showMessageRate;
    int m_showHeadingChangeRate;
    int m_useSpeed;
    int m_showExpired;

    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(void);

}; /* SPxOwnShipRenderer */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_OWN_SHIP_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
