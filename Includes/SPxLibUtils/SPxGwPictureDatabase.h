/*********************************************************************
 *
 * (c) Copyright 2018, 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget picture database class.
 *
 * Revision Control:
 *  25/07/19 v1.2    AGC	Support enable/disable edit and labels.
 *
 * Previous Changes:
 *  07/11/18 1.1   BTB	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_PICTURE_DATABASE_H
#define _SPX_GW_PICTURE_DATABASE_H

#include "SPxLibUtils/SPxAutoPtr.h"

/* Other headers required. */
#include "SPxLibUtils/SPxAreaObj.h"
#include "SPxLibUtils/SPxGwObj.h"
#include "SPxLibUtils/SPxRendererObj.h"

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
class SPxViewControl;
class SPxGwPicture;

class SPxGwPictureDatabase : public SPxGwObj, public SPxRendererObj, public SPxAreaObj
{
public:
    /* Constructor and destructor. */
    SPxGwPictureDatabase(void);
    virtual ~SPxGwPictureDatabase(void);

    virtual SPxErrorCode AddPicture(SPxGwPicture *picture);

    virtual SPxErrorCode AddEventCallback(SPxAreaEventFn fn, void *userArg);
    virtual SPxErrorCode RemoveEventCallback(SPxAreaEventFn fn, void *userArg);

    /* View control. */
    virtual SPxErrorCode AddViewControl(SPxViewControl *viewCtrl);
    virtual void RequestRedraw(void);

    /* Rendering. */
    virtual SPxErrorCode SetEditEnabled(int editEnabled);
    virtual SPxErrorCode SetLabelsEnabled(int labelsEnabled);
    virtual SPxErrorCode Render(SPxExtRenderer *renderer);
    virtual SPxErrorCode Render(SPxExtRenderer *renderer,
				int editEnabled);
    virtual SPxErrorCode Render(SPxExtRenderer *renderer,
				int editEnabled,
				int labelsEnabled);

    /* SPxAreaObj interface. */
    virtual const char *GetItemName(void) const;

    virtual SPxErrorCode IterateObjects(SPxAreaObjFn fn, void *userArg) const;

    virtual void ForceShowDirectionIndicators(int show);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;	/* Private structure. */

    SPxErrorCode render(SPxExtRenderer *renderer,
			int editEnabled,
			int labelsEnabled);
};

#endif /* _SPX_GW_PICTURE_DATABASE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
