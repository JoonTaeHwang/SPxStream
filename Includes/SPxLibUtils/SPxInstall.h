/*********************************************************************
*
* (c) Copyright 2014 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	SPxInstall class for SPx package download/installation.
*
* Revision Control:
*   05/08/22 v1.5    AGC	Remove VS2010 support.
*				Add VS2019/VS2022 support.
*
* Previous Changes:
*   12/04/17 1.4    AGC	Remove VS2005/VS2008 support.
*			Add VS2017 support.
*   05/02/15 1.3    AGC	Support VS2015.
*			Support larger release notes.
*   15/09/14 1.2    AGC	Add IsVisualStudioInstalled().
*   04/07/14 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_INSTALL_H
#define _SPX_INSTALL_H

/* Headers */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"

class SPxInstall : public SPxObj
{
public:
    enum Product
    {
	PRODUCT_NONE = 0,
	PRODUCT_DEV = 1,
	PRODUCT_SERVER = 2,
	PRODUCT_FUSION_SERVER = 3,
	PRODUCT_RADARVIEW = 4,
	PRODUCT_RADARSIM = 5,
	PRODUCT_AV = 6,
	PRODUCT_HPX_BSL = 7,
	PRODUCT_ECDIS = 8
    };
    enum Platform
    {
	PLATFORM_NONE = 0,
	PLATFORM_WINDOWS = 1,
	PLATFORM_LINUX = 2
    };
    enum VisualStudioVersion
    {
	VS_NONE = 0,
	VS_2012 = 2012,
	VS_2013 = 2013,
	VS_2015 = 2015,
	VS_2017 = 2017,
	VS_2019 = 2019,
	VS_2022 = 2022
    };

    SPxInstall(void);
    virtual ~SPxInstall(void);

#ifdef _WIN32
    int IsVisualStudioInstalled(VisualStudioVersion vsVersion);
#endif

    SPxErrorCode GetLatestVersion(Platform platform, Product product,
	VisualStudioVersion vsVersion,
	UINT32 *version) const;

    SPxErrorCode GetFtpUsername(Platform platform, Product product,
	VisualStudioVersion vsVersion, UINT32 version,
	char *ftpUsername, unsigned int *numBytes) const;

    SPxErrorCode GetFtpFolder(Platform platform, Product product,
	VisualStudioVersion vsVersion, UINT32 version,
	char *ftpFolder, unsigned int *numBytes) const;

    SPxErrorCode GetPackageName(Platform platform, Product product,
	VisualStudioVersion vsVersion, UINT32 version,
	char *packageName, unsigned int *numBytes) const;

    SPxErrorCode GetReleaseNotes(Platform platform, Product product,
	VisualStudioVersion vsVersion, UINT32 version,
	char *releaseNotes, unsigned int *numBytes) const;

    SPxErrorCode Download(Platform platform, Product product,
	VisualStudioVersion vsVersion, UINT32 version,
	const char *username,
	const char *password,
	const char *destinationDir);
    SPxErrorCode CancelDownload(void);
    SPxErrorCode GetDownloadProgress(UINT64 *completeBytes, UINT64 *totalBytes) const;

private:
    struct impl;
    struct ProductInfo;
    SPxAutoPtr<impl> m_p;

    SPxErrorCode getProductInfo(Platform platform, Product product,
	VisualStudioVersion vsVersion, UINT32 version,
	ProductInfo **productInfo) const;
    SPxErrorCode cacheLatestVersions(void) const;
    const char *productToTag(Platform platform, Product product) const;
};

#endif /* _SPX_INSTALL_H */

/*********************************************************************
*
*	End of file
*
**********************************************************************/
