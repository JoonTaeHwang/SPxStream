/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimRangeFinder class header.
 *
 * Revision Control:
 *   27/09/22 v1.1   AGC	Initial version.
 *
 * Previous Changes:
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_RANGE_FINDER_H
#define _SPX_VID_SIM_RANGE_FINDER_H

/* Other headers required. */
#include "SPxLibUtils/SPxVidSimMovable.h"
#include "SPxLibData/SPxCamMeasureRangeIface.h"

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

/* Forward declare any classes required. */
class SPxVidSimMount;
class SPxCamMeasureRangeIface;

class SPxVidSimRangeFinder : public SPxVidSimMovable
{
public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxVidSimRangeFinder(SPxVidSimMount *mount, int index=0);
    virtual ~SPxVidSimRangeFinder(void);
    SPxErrorCode Initialise(void);

    SPxCamMeasureRangeIface *GetMeasureRangeIface(void);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private variables.
     */
    class SimMeasureRange;

    struct impl;
    SPxAutoPtr<impl> m_p;


    /*
     * Private static variables.
     */

    static unsigned int m_nextRangeFinderNum; /* Used to name range finders. */

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */

}; /* SPxVidSimRangeFinder */

#endif /* _SPX_VID_SIM_RANGE_FINDER_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
