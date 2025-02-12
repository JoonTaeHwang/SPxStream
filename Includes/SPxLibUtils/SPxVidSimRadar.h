/*********************************************************************
 *
 * (c) Copyright 2016 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimRadar class header.
 *
 * Revision Control:
 *   22/07/22 v1.2   SP 	Changes to simplify creation and deletion.
 *
 * Previous Changes:
 *   31/10/16 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_RADAR_H
#define _SPX_VID_SIM_RADAR_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxVidSimMovable.h"
#include "SPxLibUtils/SPxTime.h"

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
class SPxVidSimPlatform;
class SPxRIB;
class SPxPIM;
class SPxNetworkReceive;
class SPxRunProcess;

class SPxVidSimRadar : public SPxVidSimMovable
{
    /* Grant SPxVidSimPlatform private access. */
    friend class SPxVidSimPlatform;

public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxVidSimRadar(SPxVidSimPlatform *platform);
    virtual ~SPxVidSimRadar(void);
    SPxErrorCode Initialise(void);

    /* Object access. */
    SPxPIM *GetPIM(void) { return m_pim; }
    SPxRunProcess *GetPIMPro(void) { return m_pimPro; }

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

    /* Radar video. */
    SPxRIB *m_rib;                /* RIB. */
    SPxPIM *m_pim;                /* PIM. */
    SPxRunProcess *m_pimPro;      /* Process connected to PIM. */
    SPxNetworkReceive *m_netSrc;  /* Network source. */

    /*
     * Private functions.
     */


    /*
     * Private static functions.
     */

}; /* SPxVidSimRadar */

#endif /* _SPX_VID_SIM_RADAR_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
