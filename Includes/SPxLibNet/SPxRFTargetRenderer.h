/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxRFTargetRenderer class which is used to render
 *   RF target graphics in a PPI window.
 *
 * Revision Control:
 *   25/10/18 v1.3   SP 	Support new SPxRFTargetDatabase class.
 *
 * Previous Changes:
 *   17/10/18 1.2   SP 	Support  multiple targetDB sources.
 *   09/01/18 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_RF_TARGET_RENDERER_H
#define _SPX_RF_TARGET_RENDERER_H

/*
 * Other headers required.
 */

/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxRenderer.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxStrings.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxImage.h"
#include "SPxLibNet/SPxRFTargetDatabase.h"

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
class SPxRFTarget;
class SPxThreatDatabase;

/*
 * Define our class.
 */
class SPxRFTargetRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxRFTargetRenderer(SPxRFTargetDatabase *targetDB, 
                        SPxThreatDatabase *threatDB);
    virtual ~SPxRFTargetRenderer(void);

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

    SPxImage m_detectorImage;

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName,
                             char *parameterValue);

    virtual int GetParameter(char *parameterName,
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
    SPxRFTargetDatabase *m_targetDB;
    SPxThreatDatabase *m_threatDB;
    int m_cursorXPixels;
    int m_cursorYPixels;
    SPxRFTarget *m_clickedTarget;

    /*
     * Private functions.
     */

    SPxErrorCode renderAll(void);
    void renderSource(const SPxRFTargetDatabase::Source_t *source);
    void renderTarget(SPxRFTarget *target);
    SPxRFTarget *getTarget(int xPixels, int yPixels);

    /*
     * Private static functions.
     */

    static int renderSourceIterator(void *targetDBVoid, 
                                   void *objVoid, 
                                   void *targetVoid);

    static int renderTargetIterator(void *targetDBVoid, 
                                   void *objVoid, 
                                   void *targetVoid);

}; /* SPxRFTargetRenderer */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_RF_TARGET_RENDERER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
