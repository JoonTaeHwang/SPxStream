/*********************************************************************
 *
 * (c) Copyright 2014 - 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxChannelHPxRadar class header.
 *
 * Revision Control:
 *   27/02/19 v1.10  SP 	Add GetLiveInputDeviceIndex().
 *
 * Previous Changes:
 *   15/01/18 1.9   SP 	Support TPG source in base class.
 *   22/01/16 1.8   SP 	Add fileID constructor arg.
 *   01/10/15 1.7   SP 	Rename from SPxChannelHPxVideo.
 *   05/06/15 1.6   SP 	Check RIC trg/acp/arp signals.
 *                     	Support RIC not opened.
 *   26/03/15 1.5   SP 	Support output selection.
 *   05/02/15 1.4   SP 	Add ID argument to constructor.
 *   23/10/14 1.3   SP 	Remove m_status.
 *   14/10/14 1.2   SP 	Fix ICC warnings.
 *   14/10/14 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_CHANNEL_HPX_RADAR_H
#define _SPX_CHANNEL_HPX_RADAR_H

/* Other headers required. */
#include "SPxLibData/SPxChannelRadar.h"

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
class SPxHPx100Source;
class SPxNetworkSend;
class SPxROC;

class SPxChannelHPxRadar : public SPxChannelRadar
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

    /* Constructors and destructor. */
    SPxChannelHPxRadar(SPxChannelDatabase *database, UINT8 id, UINT8 fileID);
    SPxErrorCode Create(Media_t inputMedia,
                        Media_t outputMedias,
                        int ricIndex, 
                        int rocIndex=0);
    virtual ~SPxChannelHPxRadar(void);

    /* Access. */
    virtual SPxHPx100Source *GetRIC(void) const { return m_ric; }

    /* Live input config functions. */
    virtual void GetLiveInputInfo(char *buf, unsigned int bufSizeBytes) const;
    virtual int IsLiveInputAvail(void) const;
    virtual int GetLiveInputDeviceIndex(void) const { return m_ricIndex; }

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Check and get channel status. */
    virtual SPxStatus_t CheckStatus(char *mesgBuf, unsigned int mesgBufLen);

    /* Live input config functions. */
    virtual SPxErrorCode SetLiveInputEnabled(int isEnabled);
    virtual int IsLiveInputEnabled(void) const;

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private variables.
     */

    /* Input objects. */
    SPxHPx100Source *m_ric;
    int m_ricIndex;
    int m_isRicOpen;

    /* General. */

    /*
     * Private static variables.
     */

    /*
     * Private functions.
     */

}; /* SPxChannelHPxRadar */

#endif /* _SPX_CHANNEL_HPX_RADAR_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
