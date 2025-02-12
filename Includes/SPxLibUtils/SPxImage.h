/*********************************************************************
*
* (c) Copyright 2011 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for SPxImage class, a utility class for loading data
*   from, and writing data to image files.
*
*   Using this class in a Linux application will require that the 
*   gdk-pixbuf library is linked.
*
* Revision Control:
*   28/05/20 v1.29   AGC	Fix usage across threads.
*
* Previous Changes:
*   21/05/20 1.28   AGC	Add CopyRegionTo().
*   10/03/20 1.27   AGC	Resource module now automatically inferred.
*   10/02/20 1.26   AGC	Support specifying module for resource load.
*   21/03/19 1.25   AGC	Add IsLoadFromBufferSupported().
*   20/03/19 1.24   AGC	Add LoadFromBuffer().
*   05/10/18 1.23   AGC	Support loading image from Windows resource.
*   28/09/18 1.22   AGC	Support scaling when copying image.
*   26/05/17 1.21   AGC	Support ARM build.
*   16/11/16 1.20   SP 	Add Reset().
*   17/08/16 1.19   AJH	Add SaveToBuffer().
*   23/05/16 1.18   AGC	Correct GetImage() comment.
*   07/01/16 1.17   SP 	Add Clear().
*   27/08/15 1.16   AGC	Move TakeScreenshot to SPxScreenshot.h.
*   14/10/14 1.15   AGC	Add floating point Create().
*   01/07/14 1.14   AGC	Support custom pitch for Create().
*   10/04/14 1.13   AGC	Improve screenshots when capture window partially off-screen.
*   17/02/14 1.12   AGC	Fix unused member warnings.
*   15/10/13 1.11   AGC	Initial Exif metadata support.
*   07/10/13 1.10   REW	Avoid icc warning in last change.
*   07/10/13 1.9    AGC	Use GdkPixbuf internally under Linux.
*			Add CopyTo() function.
*   01/08/13 1.8    AGC	Add GetImage() function.
*   25/07/13 1.7    AGC	Add CopyToClipboard() function.
*   09/11/12 1.6    AGC	Add Create() function.
*			Allow modification of data.
*   31/08/12 1.5    AGC	Remove unused memory header.
*			Remove CopyData() function.
*   19/04/12 1.4    AGC	Support loading PNG files under Linux.
*   22/02/12 1.3    SP 	Omit X11 headers on QNX.
*   08/02/12 1.2    AGC	Add support for taking screenshots.
*   14/11/11 1.1    AGC	Initial version.
**********************************************************************/

#ifndef _SPX_IMAGE_H
#define _SPX_IMAGE_H

/*
 * Other headers required.
 */

/* Standard headers. */

/* For error code enum. */
#include "SPxLibUtils/SPxError.h"

/* For SPxPath. */
#include "SPxLibUtils/SPxSysUtils.h"

/* For SPxObj base class. */
#include "SPxLibUtils/SPxObj.h"

#ifdef _WIN32
#include "SPxLibWin/SPxComInit.h"
#include "SPxLibWin/SPxComPtr.h"
#endif

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
#ifdef _WIN32
namespace ATL
{
    class CImage;
}
struct IWICImagingFactory;
#else
struct _GdkPixbuf;
#endif

/* Preservation or specification of bit depth. */
enum SPxImageBpp
{
    SPX_IMAGE_BPP_PRESERVE = 0,
    SPX_IMAGE_BPP_8 = 8,
    SPX_IMAGE_BPP_24 = 24,
    SPX_IMAGE_BPP_32 = 32
};

/*
 * SPx Image class
 */
class SPxImage : public SPxObj
{
public:
    /* Construction and destruction. */
    SPxImage(void);
    virtual ~SPxImage(void);
    SPxErrorCode Create(int width, int height, int bpp,
	const unsigned char *data=NULL, int pitch=0);
    SPxErrorCode Create(int width, int height, int bpp,
	const float *data, int pitch);
    SPxErrorCode CopyTo(SPxImage *dst, int width=-1, int height=-1) const;
    SPxErrorCode CopyRegionTo(SPxImage *dst, int x, int y, int width, int height) const;
    SPxErrorCode Reset(void);

    /* Load and save image. */
#if defined(WIN32) || defined(__BORLAND__)
    SPxErrorCode Load(int resourceID);
#endif
    SPxErrorCode Load(const char *filename, const char *relFromEnvVar=NULL);
    int IsLoadFromBufferSupported(void) const;
    SPxErrorCode LoadFromBuffer(const char *buffer, unsigned int bufSize);
    SPxErrorCode Save(const char *filename);
    SPxErrorCode SaveToBuffer(char *buffer, unsigned int bufSize, const char *ext, unsigned int *retSize);

    const char *GetFullPath(void) const { return m_file.GetFullPath(); }
    const char *GetShortPath(void) const { return m_file.GetShortPath(); }
    const char *GetFilename(void) const { return m_file.GetFilename(); }

    SPxErrorCode SetLatLong(double latDegs, double longDegs, double altMetres = 0.0);
    SPxErrorCode GetLatLong(double *latDegs, double *longDegs, double *altMetres = NULL) const;

    /* Platform object access. */
#if defined(WIN32) || defined(__BORLAND__)
    ATL::CImage *GetImage(void) const { return m_image; }
#else
    struct _GdkPixbuf *GetImage(void) const { return m_image; }
    void SetImage(struct _GdkPixbuf *image);
#endif

    /* Clear an image to a given colour. */
    SPxErrorCode Clear(UINT32 argb);

    /* Data access functions. */
    int GetWidth(void) const;
    int GetHeight(void) const;
    int GetPitch(void) const;
    int GetBpp(void) const;
    unsigned char *GetData(void) const;
    SPxErrorCode CopyToClipboard(const char *name) const;
    static SPxErrorCode ConvertBuffer(const unsigned char *src, SPxImageBpp srcBpp, int srcPitch,
			       unsigned char *dst, SPxImageBpp dstBpp, int dstPitch,
			       int width, int height);
private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxPath m_file;		/* File for image. */
#ifdef _WIN32
    ATL::CImage *m_image;	/* Image object. */
#else
    struct _GdkPixbuf *m_image;
#endif

}; /* SPxImage */

/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_IMAGE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
