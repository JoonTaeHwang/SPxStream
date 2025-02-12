/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimModelDatabase class header.
 *
 * Revision Control:
 *   22/07/22 v1.6   SP 	Changes to simplify creation and deletion.
 *
 * Previous Changes:
 *   07/10/16 1.5   SP 	Pass scene to constructor.
 *   20/06/14 1.4   AGC	Move C++ headers to source file.
 *   04/10/13 1.3   AGC	Simplify headers.
 *   19/07/13 1.2   SP 	Further development.
 *   15/07/13 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_MODEL_DATABASE_H
#define _SPX_VID_SIM_MODEL_DATABASE_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxVidSimObj.h"
#include "SPxLibUtils/SPxVidSimModel.h"
#include "SPxLibUtils/SPxVidSimScene.h"

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

class SPxVidSimModelDatabase : public SPxVidSimObj
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxVidSimModelDatabase(SPxVidSimScene *scene);
    virtual ~SPxVidSimModelDatabase(void);

    /* Child access. */
    unsigned int GetNumModels(void) { return GetNumChildren(OBJECT_TYPE_MODEL); }
    SPxVidSimModel *GetModel(unsigned int index);

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

    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */

}; /* SPxVidSimModelDatabase */

#endif /* _SPX_VID_SIM_MODEL_DATABASE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
