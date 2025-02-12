/*********************************************************************
*
* (c) Copyright 2020 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTPS75Encoder class.
*
*	This class can be used to build a TPS-75 message buffer from
*	a provided SPx Extended Track message.
*
*	TPS-75 message content comes from a specification
*	(confidential) obtained as part of project P478.
*
*
* Revision Control:
*   24/05/21 v1.2    BTB	Add option to calculate Mode C code if missing.
*
* Previous Changes:
*   21/10/20 1.1    BTB	Initial Version.
**********************************************************************/

#ifndef _SPX_TPS75_ENCODER_H
#define _SPX_TPS75_ENCODER_H

/*
 * Other headers required.
 */
#include "SPxLibData/SPxPackets.h"

/* We need our base class. */
#include "SPxLibUtils/SPxObj.h"

class SPxTPS75Encoder : public SPxObj
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTPS75Encoder(void);
    virtual ~SPxTPS75Encoder(void);

    SPxErrorCode SetIncludingPadding(int include);
    int GetIncludingPadding(void) { return m_includePadding; }

    SPxErrorCode SetCalculateModeC(int enable);
    int GetCalculateModeC(void) { return m_calcModeC; }

    SPxErrorCode Encode(UINT16 *tpsMsgBuf,
                        unsigned int bufSizeWords,
                        const SPxPacketTrackExtended *track) const;

    /* Static conversion function. */
    static SPxErrorCode BuildTPS75Message(UINT16 *tpsMsgBuf,
                                          unsigned int bufSizeWords,
                                          const SPxPacketTrackExtended *track,
                                          int includePaddingWord,
                                          int calcModeC=FALSE);

private:
    int m_includePadding;
    int m_calcModeC;

}; /* SPxTPS75Encoder */

#endif /* _SPX_TPS75_ENCODER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
