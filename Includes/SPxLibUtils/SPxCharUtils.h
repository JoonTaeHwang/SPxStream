/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for character based utilities.
*
* Revision Control:
*   08/08/22 v1.1    BTB	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_CHARUTILS_H
#define _SPX_CHARUTILS_H

/* Characters outside normal ASCII range.
 * For use with SPxGetCharacter() function.
 */
enum SPxCharacter_t
{
    SPX_CHAR_DEGS,
    SPX_CHAR_SUPER2,
    SPX_CHAR_MICRO,
    SPX_CHAR_SIGMA_LOWER,
    SPX_CHAR_COPYRIGHT,
    SPX_CHAR_POUND,
    SPX_CHAR_LRM,		/* Unicode only. */
    SPX_CHAR_RLM,		/* Unicode only. */
    SPX_CHAR_UP_ARROW,		/* Unicode only. */
    SPX_CHAR_DOWN_ARROW		/* Unicode only. */
};

/*********************************************************************
*
*   Functions
*
**********************************************************************/

/* Get a character in the current internal string format (ACP or UTF-8). */
extern const char *SPxGetCharacter(SPxCharacter_t c);

#endif /* _SPX_CHARUTILS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
