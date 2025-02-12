/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimPlatform class header.
 *
 * Revision Control:
 *   27/09/22 v1.11   AGC	Rename SPxVidSimCameraMount to SPxVidSimMount.
 *
 * Previous Changes:
 *   30/08/22 1.10  SP 	Move Z reference support to SPxVidSimPlatform.
 *   22/07/22 1.9   SP 	Changes to simplify creation and deletion.
 *   15/07/22 1.8   AGC	Make AddCameraMount virtual to break dependency chain.
 *   14/07/22 1.7   SP 	Tidy up code.
 *   25/06/18 1.6   SP 	Support nav data input.
 *   31/10/16 1.5   SP 	Support radars.
 *   07/10/16 1.4   SP 	Derive from SPxVidSimMovable.
 *   20/06/14 1.3   AGC	Move C++ headers to source file.
 *   04/10/13 1.2   AGC	Simplify headers.
 *   15/07/13 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_PLATFORM_H
#define _SPX_VID_SIM_PLATFORM_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxVidSimMovable.h"

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
class SPxVidSimScene;
class SPxVidSimMount;
class SPxVidSimRadar;
class SPxNavData;
class SPxTerrainDatabase;

class SPxVidSimPlatform : public SPxVidSimMovable
{
    /* Grant private access to SPxVidSimScene. */
    friend class SPxVidSimScene;

public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxVidSimPlatform(SPxVidSimScene *scene);
    virtual ~SPxVidSimPlatform(void);
    SPxErrorCode Initialise(void);

    /* Child access. */
    unsigned int GetNumMounts(void) { return GetNumChildren(OBJECT_TYPE_MOUNT); }
    SPxVidSimMount *GetMount(unsigned int index);
    unsigned int GetNumRadars(void) { return GetNumChildren(OBJECT_TYPE_RADAR); }
    SPxVidSimRadar *GetRadar(unsigned int index);

    /* Z position. */
    SPxErrorCode SetZRefMode(ZRefMode_t mode);
    ZRefMode_t GetZRefMode(void) const { return m_zRefMode; }
    double GetZMetres(void) const;

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
    SPxNavData *m_navData;
    ZRefMode_t m_zRefMode;

    /*
     * Private functions.
     */

    void syncToNavData(void);

    /*
     * Private static functions.
     */

    static int navDataHandler(void *invokingObj,
                              void *userArg,
                              void *msgPtr);

}; /* SPxVidSimPlatform */

#endif /* _SPX_VID_SIM_PLATFORM_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
