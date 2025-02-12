/*********************************************************************
 *
 * (c) Copyright 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxDetectedObjRenderer class which is used to 
 *   render object detected in camera video in a PPI window.
 *
 * Revision Control:
 *   25/06/21 v1.1   SP 	Initial version.
 *
 * Previous Changes:
 *
 *********************************************************************/

#ifndef _SPX_DETECTED_OBJ_RENDERER_H
#define _SPX_DETECTED_OBJ_RENDERER_H

/*
 * Other headers required.
 */

/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxRenderer.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibNet/SPxObjectDetector.h"

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

/* Forward declare other classes required. */

/*
 * Define our renderer class.
 */
class SPxDetectedObjRenderer : virtual public SPxRenderer
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
    SPxDetectedObjRenderer(SPxObjectDetector *detector);
    virtual ~SPxDetectedObjRenderer(void);

    /* Detectors. */
    SPxErrorCode AddDetector(SPxObjectDetector *detector);

    /* Functions used to implement SPxMouseInterface. */
    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleDoubleClick(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseLeave(void);

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);

    virtual int GetParameter(char *name,
                             char *valueBuf, 
                             int bufLen);

private:
    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxCriticalSection m_mutex;

    /*
     * Private functions.
     */

    SPxErrorCode renderAll(void);
    void renderDetection(const SPxObjectDetector::Detection_t *detection);

    /*
     * Private static functions.
     */

    /* Private static functions. */
    static int renderDetectionItr(void *detectorVoid,
                                  void *objVoid,
                                  void *detectionVoid);

}; /* SPxDetectedObjRenderer */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_DETECTED_OBJ_RENDERER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
