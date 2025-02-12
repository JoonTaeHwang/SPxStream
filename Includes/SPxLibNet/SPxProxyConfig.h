/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for SPxProxyConfig class, used to configure
*	connections to a proxy server.
*
* Revision Control:
*   08/02/17 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef SPX_PROXY_CONFIG_H_
#define SPX_PROXY_CONFIG_H_

/* For base class. */
#include "SPxLibUtils/SPxObj.h"


class SPxProxyConfig :public SPxObj
{
public:
    static SPxProxyConfig *CreateInstance(void);
    static SPxProxyConfig *GetInstance(void);

    SPxErrorCode SetServer(const char *server);
    SPxErrorCode GetServer(char *buffer, unsigned int bufLen) const;
    SPxErrorCode SetUsername(const char *username);
    SPxErrorCode GetUsername(char *buffer, unsigned int bufLen) const;
    SPxErrorCode SetPassword(const char *password);
    SPxErrorCode GetPassword(char *buffer, unsigned int bufLen) const;

    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private constructor/destructor. */
    SPxProxyConfig(void);
    virtual ~SPxProxyConfig(void);

    /* Disallow copies. */
    SPxProxyConfig(const SPxProxyConfig&) SPX_FUNCTION_DELETE;
    void operator=(const SPxProxyConfig&) SPX_FUNCTION_DELETE;

}; /* SPxProxyConfig */

#endif /* SPX_PROXY_CONFIG_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
