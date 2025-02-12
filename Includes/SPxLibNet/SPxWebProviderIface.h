/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Interface for objects that can provide web resources to
*	SPxWebInterface.
*
* Revision Control:
*   14/10/22 v1.2    AGC	Add errCode to getHttpFilePath().
*
* Previous Changes:
*   22/09/22 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_WEB_PROVIDER_IFACE
#define _SPX_WEB_PROVIDER_IFACE

#include "SPxLibUtils/SPxError.h"

class SPxWebProviderIface
{
public:

    SPxWebProviderIface() {}
    virtual ~SPxWebProviderIface() {}

    virtual SPxErrorCode getHttpFilePath(const char *urlExt,
					 char *filePath,
					 unsigned int filePathSize,
					 unsigned int *errCode) = 0;
};

#endif /* _SPX_WEB_PROVIDER_IFACE */

/*********************************************************************
*
* End of file
*
**********************************************************************/
