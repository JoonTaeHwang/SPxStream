/*********************************************************************
 *
 * (c) Copyright 2013, 2016, 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics image buffer class.
 *
 * Revision Control:
 *  16/09/22 v1.4    AGC	Support use on platforms without SPxImage support.
 *
 * Previous Changes:
 *  08/07/16 1.3    AGC	Support loading image from resource under Windows.
 *  07/10/13 1.2    AGC	Move to SPxLibUtils.
 *			Make cross-platform.
 *  08/03/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_IMAGE_BUFFER_H
#define _SPX_GW_IMAGE_BUFFER_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxSysUtils.h"
#include "SPxLibUtils/SPxGwObj.h"

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

/* Forward declarations. */
class SPxImage;

class SPxGwImageBuffer : virtual public SPxGwObj
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwImageBuffer(void);
    virtual ~SPxGwImageBuffer(void);

    /* Configuration. */
    virtual SPxErrorCode LoadFile(const char *filename);
#ifdef _WIN32
    virtual SPxErrorCode LoadResource(int resourceID);
#endif
    virtual SPxErrorCode GetFilename(char *buf, unsigned int bufLen) const;
    virtual const char *GetFullImagePath(void) const;
    virtual const char *GetShortImagePath(void) const;
    virtual SPxErrorCode LoadFromBuffer(SPxGwImageBuffer *imgBuf);
    virtual unsigned int GetWidthPixels(void) const { return m_widthPixels; }
    virtual unsigned int GetHeightPixels(void) const { return m_heightPixels; }

protected:
    
    /*
     * Protected functions.
     */
    const SPxImage *GetImage(void) const;
    SPxImage *GetImage(void);

private:

    /*
     * Private variables.
     */

#if defined(WIN32) || SPX_X11_SUPPORTED
    /* Pixbuf into which image is loaded. */
    SPxAutoPtr<SPxImage> m_image;
#endif

    /* Image filename. */
    SPxPath m_filename;

    /* Image size. */
    unsigned int m_widthPixels;
    unsigned int m_heightPixels;

    /*
     * Private functions.
     */

}; /* SPxGwImageBuffer */

#endif /* _SPX_GW_IMAGE_BUFFER_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

