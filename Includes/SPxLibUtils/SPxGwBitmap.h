/*********************************************************************
 *
 * (c) Copyright 2013, 2017, 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget bitmap class, used to handle XBM files.
 *
 * Revision Control:
 *  25/06/18 v1.6    AGC	Correctly suppress cppcheck warnings.
 *
 * Previous Changes:
 *  03/11/17 1.5    AGC	Support GetAreaType().
 *  07/10/13 1.4    AGC	Move to SPxLibUtils.
 *			Make cross-platform.
 *			Add SPxAreaObj support.
 *  25/01/13 1.3    SP 	Allow group to be set in constructor.
 *  15/01/13 1.2    SP 	Add support for named colours.
 *  11/01/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_BITMAP_H
#define _SPX_GW_BITMAP_H

/* Other headers required. */
#include "SPxLibUtils/SPxGwImage.h"

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
class SPxGwWidgetGroup;

class SPxGwBitmap : public SPxGwImage
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwBitmap(SPxGwWidgetGroup *group=NULL);
    virtual ~SPxGwBitmap(void);

    /* Configuration. */
    virtual SPxErrorCode LoadFile(const char *filename);
    virtual SPxErrorCode SetForegroundColour(UINT32 argb);
    virtual SPxErrorCode SetForegroundColour(UINT8 r, UINT8 g, 
                                             UINT8 b, UINT8 a=0xFF);
    virtual SPxErrorCode SetForegroundColour(const char *name);
    virtual UINT32 GetForegroundColour(void) const;
    virtual UINT8 GetForegroundColourA(void) const;
    virtual UINT8 GetForegroundColourR(void) const;
    virtual UINT8 GetForegroundColourG(void) const;
    virtual UINT8 GetForegroundColourB(void) const;

    virtual SPxErrorCode SetBackgroundColour(UINT32 argb);
    virtual SPxErrorCode SetBackgroundColour(UINT8 r, UINT8 g, 
                                             UINT8 b, UINT8 a=0xFF);
    virtual SPxErrorCode SetBackgroundColour(const char *name);
    virtual UINT32 GetBackgroundColour(void) const;
    virtual UINT8 GetBackgroundColourA(void) const;
    virtual UINT8 GetBackgroundColourR(void) const;
    virtual UINT8 GetBackgroundColourG(void) const;
    virtual UINT8 GetBackgroundColourB(void) const;

    /* SPxAreaObj interface. */
    virtual int GetAreaType(void) const;
    virtual int IsColourSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetFillColour(UINT32 argb);
    // cppcheck-suppress virtualCallInConstructor
    virtual UINT32 GetFillColour(void) const;
    virtual SPxErrorCode SetLineColour(UINT32 argb);
    // cppcheck-suppress virtualCallInConstructor
    virtual UINT32 GetLineColour(void) const;

    virtual SPxErrorCode LoadCfg(FILE *f);
    virtual SPxErrorCode SaveCfg(FILE *f) const;

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

private:

    /*
     * Private variables.
     */

    /* Foreground and background colour. */
    UINT32 m_argbForeground;
    UINT32 m_argbBackground;

    /*
     * Private functions.
     */

    SPxErrorCode ReplaceColour(UINT32 argbSrc, UINT32 argbDst);

}; /* SPxGwBitmap */

#endif /* _SPX_GW_BITMAP_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

