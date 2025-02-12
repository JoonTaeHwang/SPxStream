/*********************************************************************
*
* (c) Copyright 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxCameraDatabase.h,v $
* ID: $Id: SPxCameraDatabase.h,v 1.1 2016/10/31 15:28:51 rew Exp $
*
* Purpose:
*   Header for SPxCameraDatabase class.
*
* Revision Control:
*   31/10/16 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/

#ifndef SPX_CAMERA_DATABASE_H_
#define SPX_CAMERA_DATABASE_H_

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxEventHandler.h"
#include "SPxLibUtils/SPxObj.h"

enum SPxCamDBProp
{
    SPX_CAM_DB_PROP_ADDRESS	= 0x00000001,
    SPX_CAM_DB_PROP_PORT	= 0x00000002,
    SPX_CAM_DB_PROP_USERNAME	= 0x00000004,
    SPX_CAM_DB_PROP_PASSWORD	= 0x00000008,
    SPX_CAM_DB_PROP_RTSP_MEDIA	= 0x00000010,
    SPX_CAM_DB_PROP_RTSP_MODE	= 0x00000020,
    SPX_CAM_DB_PROP_RTSP_ALL	= 0x0000003F
};

class SPxCameraDatabase : public SPxObj
{
public:
    typedef int (*IterateFn)(unsigned int index, const char *name);
    typedef int (*MatchFn)(SPxCamDBProp prop, const char *value);
    
    /* Constructor and destructor. */
    SPxCameraDatabase(void);
    virtual ~SPxCameraDatabase(void);
    
    template<typename F, typename O>
    SPxErrorCode Iterate(F fn, O userArg) const
    {
	SPxEventHandler<IterateFn> handler;
	handler.Add(fn, userArg);
	return iterate(handler);
    }

    int IsSet(unsigned int index, UINT32 propMask, int iterating=FALSE) const;
    int IsSet(const char *camera, UINT32 propMask) const;
    SPxErrorCode Get(unsigned int index, SPxCamDBProp prop, int *value, int iterating=FALSE) const;
    SPxErrorCode Get(unsigned int index, SPxCamDBProp prop, unsigned int *value, int iterating=FALSE) const;
    SPxErrorCode Get(unsigned int index, SPxCamDBProp prop, float *value, int iterating=FALSE) const;
    SPxErrorCode Get(unsigned int index, SPxCamDBProp prop, double *value, int iterating=FALSE) const;
    SPxErrorCode Get(unsigned int index, SPxCamDBProp prop, char *buf, unsigned int bufLen, int iterating=FALSE) const;
    SPxErrorCode Get(const char *camera, SPxCamDBProp prop, int *value) const;
    SPxErrorCode Get(const char *camera, SPxCamDBProp prop, unsigned int *value) const;
    SPxErrorCode Get(const char *camera, SPxCamDBProp prop, float *value) const;
    SPxErrorCode Get(const char *camera, SPxCamDBProp prop, double *value) const;
    SPxErrorCode Get(const char *camera, SPxCamDBProp prop, char *buf, unsigned int bufLen) const;

    template<typename F, typename O>
    SPxErrorCode FindBestMatch(UINT32 propMask, F fn, O userArg,
			       char *cameraNameBuf, unsigned int bufLen) const
    {
	SPxEventHandler<MatchFn> handler;
	handler.Add(fn, userArg);
	return findBestMatch(propMask, handler, cameraNameBuf, bufLen);
    }
    
    /* Generic get/set parameter interface. */
    virtual SPxErrorCode SetStateFromConfig(void);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;   /* Private structure. */

    SPxErrorCode iterate(SPxEventHandler<IterateFn>& handler) const;
    SPxErrorCode get(unsigned int index, SPxCamDBProp prop,
		     const char **value) const;
    SPxErrorCode get(const char *camera, SPxCamDBProp prop,
		     const char **value) const;
    SPxErrorCode findBestMatch(UINT32 propMask, SPxEventHandler<MatchFn>& handler,
			       char *cameraNameBuf, unsigned int bufLen) const;
    
}; /* SPxCameraDatabase */

#endif /* SPX_CAMERA_DATABASE_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
