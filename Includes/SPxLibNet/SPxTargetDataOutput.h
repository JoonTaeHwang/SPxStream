/*********************************************************************
 *
 * (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for base class of object for re-encoding SPx extended tracks
 *	received from an SPxTargetDecoder object and outputting the
 *	encoded tracks using any installed SPxPacketSender objects.
 *
 *
 * Revision Control:
 *   11/04/22 v1.2   BTB	Use filtered rather than decoded data as a source.
 *
 * Previous Changes:
 *   02/10/20 1.1   BTB	Initial Version.
 **********************************************************************/

#ifndef _SPX_TARGET_DATA_OUTPUT_H
#define _SPX_TARGET_DATA_OUTPUT_H

/*
 * Other headers required.
 */
#include "SPxLibNet/SPxExtHeartbeatSender.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxSdbStream.h"

/*********************************************************************
 *
 *   Constants
 *
 **********************************************************************/

/*********************************************************************
 *
 *   Macros
 *
 **********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 **********************************************************************/

/* Forward declarations. */
class SPxPacketSender;
class SPxTargetDataFilter;

class SPxTargetDataOutput : public SPxObj
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTargetDataOutput(SPxTargetDataFilter *dataSrc,
                        SPxExtHeartbeatSender *hbSender);
    virtual ~SPxTargetDataOutput(void);

    SPxErrorCode AddDataDestination(SPxPacketSender *dest);

protected:
    /*
     * Protected variables.
     */
    SPxTargetDataFilter *m_dataSrc;
    SPxSdbStream::Type_t m_streamType;
    SPxSdbStream::Subtype_t m_streamSubType;

    /*
     * Protected functions.
     */
    unsigned int GetNumDataDestinations(void);
    SPxPacketSender *GetDataDestination(unsigned int index);

 private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxExtHeartbeatSender *m_hbSender;

}; /* SPxTargetDataOutput */

#endif /* _SPX_TARGET_DATA_OUTPUT_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
