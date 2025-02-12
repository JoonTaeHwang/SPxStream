/*********************************************************************
*
* (c) Copyright 2010, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxLegacyParams.h,v $
* ID: $Id: SPxLegacyParams.h,v 1.1 2010/11/08 14:52:37 rew Exp $
*
* Purpose:
*   This header contains lists of old configuration file parameter
*   names and the new names for these parameters for the SPx Server.
*
*   An application that needs to check the configuration for old
*   parameters can pass one of these lists to the SPxMigrateConfig()
*   function.
*
* Revision Control:
*   08/11/10 v1.1    AGC	Initial Version.
*
* Previous Changes:
*
**********************************************************************/
#ifndef _SPX_LEGACYPARAMS_H
#define	_SPX_LEGACYPARAMS_H

/* Get list of deprecated/new SPx Server parameters. */
const char **SPxGetServerLegacyParams(void);

#endif /* _SPX_LEGACYPARAMS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
