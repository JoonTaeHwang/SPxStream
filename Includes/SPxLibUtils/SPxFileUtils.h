/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for file utilities with minimal dependencies.
*
* Revision Control:
*   08/08/22 v1.2    BTB	Fix build warning.
*
* Previous Changes:
*   08/08/22 1.1    BTB	Initial version.
**********************************************************************/

#ifndef _SPX_FILEUTILS_H
#define _SPX_FILEUTILS_H

/*
 * Other headers required.
 */

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

extern FILE *SPxFopen(const char *path, const char *mode);
extern FILE *SPxPopen(const char *command, const char *mode);
extern int SPxIsLinuxExecutable(const char *path, int *is64Bit=NULL);
extern int SPxIsWindowsExecutable(const char *path, int *is64Bit=NULL);
extern INT64 SPxFtell(FILE *file);
extern int SPxFseek(FILE *file, INT64 offset, int origin);

#endif /* _SPX_FILEUTILS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
