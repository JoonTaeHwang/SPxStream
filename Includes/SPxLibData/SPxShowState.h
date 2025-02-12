
/*********************************************************************
*
* (c) Copyright 2007, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxShowState.h,v $
* ID: $Id: SPxShowState.h,v 1.2 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*   Implementation of a debug function to output SPx state and connections
*
* Revision Control:
*   04/10/13 v1.2    AGC	Simplify headers.
*
* Previous Changes:
*   24/08/07 1.1    DGJ	Initial Version
**********************************************************************/

#include <stdio.h>

extern void SPxShowState( void (*fn)(const char*));
extern void SPxShowState(FILE *foutput);
extern void SPxShowState(const char *fileName);

/*********************************************************************
*
* End of file
*
**********************************************************************/
