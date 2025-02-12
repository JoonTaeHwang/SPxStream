/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimTargetRendererGL class header.
 *
 * Revision Control:
 *   22/07/22 v1.4   SP 	Changes to simplify creation and deletion.
 *
 * Previous Changes:
 *   20/12/19 1.3   AGC	Add extrapolate track position option.
 *   31/10/16 1.2   SP 	Move headers to source files.
 *   19/07/13 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_TARGET_RENDERER_GL_H
#define _SPX_VID_SIM_TARGET_RENDERER_GL_H

/* Other headers required. */
#include "SPxLibUtils/SPxVidSimObj.h"

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
class SPxVidSimDispGL;
class SPxVidSimScene;
class SPxRadarTrack;
class SPxVidSimModelDatabase;
class SPxVidSimModel;
class SPxTrackDatabase;

class SPxVidSimTargetRendererGL : public SPxVidSimObj
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
    SPxVidSimTargetRendererGL(SPxVidSimDispGL *disp);
    virtual ~SPxVidSimTargetRendererGL(void);

    /* Configuration. */
    SPxErrorCode RenderGL(void);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Set link to parent object. */

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private variables.
     */

    /*
     * Private static variables.
     */

    static int staticRenderTrackGL(void *database, void *arg, void *track);

    /*
     * Private functions.
     */

    SPxErrorCode RenderTrackGL(SPxRadarTrack *track);
    SPxVidSimModel *SelectModel(SPxRadarTrack *track);

}; /* SPxVidSimTargetRendererGL */

#endif /* _SPX_VID_SIM_TARGET_RENDERER_GL_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
