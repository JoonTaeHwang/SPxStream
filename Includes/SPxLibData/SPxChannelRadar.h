/*********************************************************************
 *
 * (c) Copyright 2014 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxChannelRadar class header.
 *
 * Revision Control:
 *   23/02/21 v1.27  SP 	Make Set/GetParemeter() public.
 *
 * Previous Changes:
 *   27/02/19 1.26  SP 	Add GetOutputDeviceIndex().
 *   13/12/18 1.25  SP 	Clear radar video from PIMs when input stops.
 *   19/03/18 1.24  SP 	Add m_copy8Pro process.
 *   12/03/18 1.23  SP 	Add GetTPGSource().
 *   15/01/18 1.22  SP 	Support gain control and TPG source.
 *   26/06/17 1.21  SP 	Override SetSaveUpdateOnly().
 *   23/03/17 1.20  SP 	Add m_rocShouldBeOpen flag.
 *   08/07/16 1.19  SP 	Add m_isNetSenderAsterix.
 *   03/03/16 1.18  SP 	Connect nav data to PIM for azimuth correction.
 *   26/02/16 1.17  SP 	Rename recPim to rawPim.
 *   22/01/16 1.16  SP 	Add fileID constructor arg.
 *                     	Make m_rocIndex unsigned.
 *   03/11/15 1.15  AGC	Packet decoder callback data now const.
 *   03/11/15 1.15  AGC	Packet decoder callback data now const.
 *   28/10/15 1.14  SP 	Support changes to base class.
 *   01/10/15 1.13  SP 	Rename from SPxChannelVideo.
 *   15/06/15 1.12  SP 	Add control over info event reporting.
 *   22/04/15 1.11  SP 	Fix ROC output issues.
 *                     	Allow ROC to be shared between channels.
 *   26/03/15 1.10  SP 	Support output selection.
 *                     	Report info events.
 *   26/02/15 1.9   SP 	Add GetOutputInfo().
 *   23/02/15 1.8   SP 	Change args to HandleReplayPacket().
 *   20/02/15 1.7   SP 	Support parameter loading/saving.
 *   16/02/15 1.6   SP 	Support static lat/long position.
 *   05/02/15 1.5   SP 	Add ID argument to constructor.
 *   26/01/15 1.4   SP 	Remove GetNumOutputs() and GetOutputMedia().
 *                     	Move recording setup here from SPxChannel.
 *   21/11/14 1.3   SP 	Add CheckStatus().
 *   14/10/14 1.2   SP 	Fix ICC warning.
 *   14/10/14 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_CHANNEL_RADAR_H
#define _SPX_CHANNEL_RADAR_H

/* Other headers required. */
#include "SPxLibData/SPxChannel.h"

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
class SPxNetworkSend;
class SPxROC;
class SPxRadarSource;
class SPxTestGenerator;
class SPxRIB;
class SPxPIM;
class SPxRunProcess;

class SPxChannelRadar : public SPxChannel
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
    SPxChannelRadar(SPxChannelDatabase *database, UINT8 id, UINT8 fileID);
    SPxErrorCode Create(Media_t inputMedia,
                        Media_t outputMedias,
                        int rocIndex);
    virtual ~SPxChannelRadar(void);

    /* Access. */
    virtual SPxRIB *GetRIB(void) { return m_rib; }
    virtual SPxPIM *GetPIM(void) { return m_pim; }
    virtual SPxRunProcess *GetParentProcess(void) { return m_nullPro; }
    virtual SPxROC *GetROC(void) const { return m_roc; }
    virtual SPxRunProcess *GetGainOffsetProcess(void) { return m_gainOffsetPro; }
    virtual SPxTestGenerator *GetTPGSource(void) { return m_liveTpgSrc; }

    /* Channel linking. */
    SPxErrorCode SetLinkedChannelID(UINT8 id, UINT32 flags);

    /* Azimuth correction. */
    SPxErrorCode SetAzimuthOffset(double degs);
    double GetAzimuthOffset(void);
    SPxErrorCode SetAzimuthRefMode(SPxAziRefMode mode);
    SPxAziRefMode GetAzimuthRefMode(void);

    /* Gain of display video. */
    SPxErrorCode SetGain(double gain);
    double GetGain(void);

    /* Input config functions. */
    virtual SPxErrorCode SetInputEnabled(int isEnabled);
    virtual SPxErrorCode SetLiveInputTPGEnabled(int isEnabled);
    virtual int IsLiveInputTPGEnabled(void) const { return m_isLiveInputTPGEnabled; }

    /* Output config functions. */
    virtual int IsOutputMediaAvail(Media_t media) const;
    virtual SPxErrorCode SetOutputEnabled(unsigned int index, int isEnabled);
    virtual int IsOutputEnabled(unsigned int index) const;
    virtual void GetOutputInfo(unsigned int index,
                               char *buf, 
                               unsigned int bufSizeBytes) const;
    virtual SPxNetAddr *GetOutputNetCtrlIf(unsigned int index);
    virtual int GetOutputDeviceIndex(void) const { return m_rocIndex; }

    /* Statistics. */
    virtual SPxErrorCode GetStats(double *periodRtn,
                                  double *prfRtn,
                                  unsigned int *minSampleRtn,
                                  unsigned int *maxSampleRtn,
                                  double *avSampleRtn,
                                  double *stdDevRtn);

    /* Info event reporting thresholds. */
    virtual SPxErrorCode SetMaxPrfChange(double hz);
    virtual double GetMaxPrfChange(void) { return m_maxPrfChangeHz; }
    virtual SPxErrorCode SetMaxPeriodChange(double hz);
    virtual double GetMaxPeriodChange(void) { return m_maxPeriodChangeSecs; }
    
    /* Config load/save. */
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);
    virtual void SetSaveUpdateOnly(int updateOnly);

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:

    /*
     * Protected variables.
     */

    /* Polar resolution. */
    unsigned int m_numSamples;
    unsigned int m_numAzimuths;

    /* Live source objects. */
    SPxRadarSource *m_liveSrc;      /* Live source, network, hardward, etc. */
    SPxTestGenerator *m_liveTpgSrc; /* Live test pattern generator source. */

    /* Input buffer. */
    unsigned int m_ribSizeBytes;
    SPxRIB *m_rib;

    /*
     * Protected functions.
     */

    /* Live input config functions. */
    virtual SPxErrorCode SetLiveInputEnabled(int isEnabled);
    virtual int IsLiveInputEnabled(void) const;

    /* Check and get channel status. */
    virtual SPxStatus_t CheckStatus(char *mesgBuf, unsigned int mesgBufLen);

    /* Replay packet handler - not used in this class. */
    virtual void HandleReplayPacket(UINT16 packetType,
                                    const unsigned char *data,
                                    unsigned int dataSizeBytes);

private:

    /*
     * Private variables.
     */

    /* Output objects. */
    SPxNetworkSend *m_netSender;
    int m_isNetSenderAsterix;
    SPxRunProcess *m_netSenderPro;
    int m_rocIndex;
    SPxROC *m_roc;
    SPxRunProcess *m_rocPro;

    /* Raw PIM. */
    SPxPIM *m_rawPim;
    SPxRunProcess *m_rawNullPro;

    /* Display PIM. */
    SPxPIM *m_pim;
    SPxRunProcess *m_copy8Pro;
    SPxRunProcess *m_nullPro;
    SPxRunProcess *m_statsPro;
    SPxRunProcess *m_gainOffsetPro;

    /* Info event reporting. */
    double m_lastPrfHz; 
    double m_maxPrfChangeHz;
    double m_lastPeriodSecs;
    double m_maxPeriodChangeSecs;

    /* General. */
    SPxThread *m_thread;
    UINT8 m_linkedNavDataChanID;
    SPxCriticalSection m_linkedNavDataMutex;
    int m_rocShouldBeOpen;
    int m_isLiveInputTPGEnabled;
    int m_isInputStopped;

    /*
     * Private functions.
     */

    int checkOpenROC(void);
    void closeROC(void);
    void reportInfoEvents(void);
    void checkUpdateNavDataLink(UINT8 linkedChanID);
    void clearPIMs(void);
    
    /*
     * Private static variables.
     */

    static void rawReturnHandler(void *userArg,
                                 SPxRunProcess *rp,
                                 unsigned int firstAzi,
                                 unsigned int numAzis);

    /*
     * Private functions.
     */

}; /* SPxChannelRadar */

#endif /* _SPX_CHANNEL_RADAR_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
