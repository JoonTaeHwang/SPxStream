/*********************************************************************
*
* (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxInfoDBEnumDef class.
*
*	This class manages a set of string values and their integer
*	equivalents for a single enumeration type InfoDB field.
*
* Revision Control:
*   15/09/22 v1.2   BTB	Rework enum name.
*
* Previous Changes:
*   13/08/20 1.1   BTB	Initial Version.
*
**********************************************************************/

#ifndef _SPX_INFODB_ENUM_DEF_H
#define _SPX_INFODB_ENUM_DEF_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
 *
 *   Constants
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

/*
 *  SPxInfoDBEnumDef class.
 */
class SPxInfoDBEnumDef : public SPxObj
{
public:
    /*
     * Public types.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxInfoDBEnumDef(const char *name=NULL);
    ~SPxInfoDBEnumDef(void);

    /* Get a count of all the currently configured enumeration values. */
    unsigned int GetNumValues(void);

    /* Get a list of all the currently configured enumeration values. */
    SPxErrorCode GetEnumValueList(int *enumValueBufRtn, unsigned int bufLen);

    /* Get a string value for a given enum value. */
    SPxErrorCode GetFieldValue(int enumValue, char *strValueRtn, unsigned int bufLen);

    /* Configure the name. */
    SPxErrorCode SetEnumName(const char *name);
    const char *GetEnumName(void);

    /* Configure the field enum values. */
    SPxErrorCode SetFieldEnumValue(int index, int enumValue);
    SPxErrorCode GetFieldEnumValue(int index, int *enumValueRtn);

    /* Configure the field string values. */
    SPxErrorCode SetFieldStringValue(int index, const char *strValue);
    SPxErrorCode GetFieldStringValue(int index, char *strValueRtn, unsigned int bufLen);

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxInfoDBEnumDef */

#endif /* _SPX_INFODB_ENUM_DEF_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
