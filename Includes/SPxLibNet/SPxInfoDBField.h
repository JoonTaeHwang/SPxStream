/*********************************************************************
*
* (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxInfoDBField class and all derived classes.
*
*	These classes represent a single field in a distributed
*	information database.
*
* Revision Control:
*   15/09/22 v1.15  BTB	Rework name used for enum definitions to use.
*
* Previous Changes:
*   26/04/21 1.14  BTB	Implement Boolean field type.
*   17/03/21 1.13  BTB	Add generic string-based units.
*   15/09/20 1.12  BTB	Implement IFF code field type.
*   08/09/20 1.11  BTB	Implement date field type.
*   13/08/20 1.10  BTB	Implement enumeration field type.
*   06/07/20 1.9   BTB	Implement time field type.
*   15/06/20 1.8   BTB	Implement latitude and longitude field types.
*   12/06/20 1.7   BTB	Improve configuration of display of field strings.
*   29/05/20 1.6   BTB	Make more Get functions public.
*                     	More build warning fixes.
*   27/05/20 1.5   BTB	Further build warning fixes.
*   26/05/20 1.4   BTB	Fix build warnings.
*   26/05/20 1.3   BTB	Implement more field types.
*   19/05/20 1.2   BTB	Fix build warnings.
*   18/05/20 1.1   BTB	Initial Version.
*
**********************************************************************/

#ifndef _SPX_INFODB_FIELD_H
#define _SPX_INFODB_FIELD_H

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

/* Forward declarations. */
class SPxInfoDBNode;

/*
 *  SPxInfoDBField class.
 */
class SPxInfoDBField : public SPxObj
{
public:
    /* The item this field belongs to is responsible for managing it
     * so must be given full access.
     */
    friend class SPxInfoDBItem;

    /* Supported field types. */
    enum FieldType
    {
        FIELD_TYPE_STRING = 1,
        FIELD_TYPE_INT = 2,
        FIELD_TYPE_DOUBLE = 3,
        FIELD_TYPE_DISTANCE = 4,
        FIELD_TYPE_SPEED = 5,
        FIELD_TYPE_LATITUDE = 6,
        FIELD_TYPE_LONGITUDE = 7,
        FIELD_TYPE_ENUM = 8,
        FIELD_TYPE_TIME = 9,
        FIELD_TYPE_DATE = 10,
        FIELD_TYPE_IFF_CODE = 11,
        FIELD_TYPE_BOOL = 12
    };

    /* Constructor and destructor. */
    SPxInfoDBField(const char *name, FieldType type, SPxInfoDBNode *parentNode);
    ~SPxInfoDBField(void);

    /* Get common field information. */
    const char *GetName(void) const;
    FieldType GetType(void) const { return m_type; }

    /* Set/get name of enumeration definition used. */
    virtual SPxErrorCode SetEnumName(const char* /*enumName*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual const char* GetEnumName(void) const { return GetName(); }

    /* Get field value. */
    virtual SPxErrorCode GetValue(char *valBufRtn, unsigned int bufLen)=0;
    virtual SPxErrorCode GetValueAsInt(int * /*valRtn*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetValueAsDouble(double * /*valRtn*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetValueAsFormattedDouble(double * /*valRtn*/, int /*units*/) { return SPX_ERR_NOT_SUPPORTED; }

    /* Set/get displayed units. */
    virtual SPxErrorCode SetDisplayUnits(int /*units*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetDisplayUnits(int * /* unitsRtn */) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode SetDisplayUnitString(const char * /*units*/)  { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetDisplayUnitString(char * /*unitBufRtn*/, unsigned int /*bufLen*/)  { return SPX_ERR_NOT_SUPPORTED; }
    
    /* Set/get displayed number of DP. */
    virtual SPxErrorCode SetDisplayNumDP(int /*numDP*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetDisplayNumDP(int * /*numDPRtn*/) { return SPX_ERR_NOT_SUPPORTED; }

protected:
    /* Access to the parent node. */
    SPxInfoDBNode *m_parentNode;

    /* Set field value. */
    virtual SPxErrorCode SetValue(const char * /*value*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode SetValue(int /*value*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode SetValue(double /*value*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode SetValue(double /*value*/, int /*units*/) { return SPX_ERR_NOT_SUPPORTED; }

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    FieldType m_type;

}; /* SPxInfoDBField */

/*
 *  SPxInfoDBStringField class.
 */
class SPxInfoDBStringField : public SPxInfoDBField
{
public:
    /* The item this field belongs to is responsible for managing it
     * so must be given full access.
     */
    friend class SPxInfoDBItem;

    /* Constructor and destructor. */
    SPxInfoDBStringField(const char *name, SPxInfoDBNode *parentNode);
    ~SPxInfoDBStringField(void);

    /* Get field value. */
    virtual SPxErrorCode GetValue(char *valBufRtn, unsigned int bufLen);

protected:
    /* Set field value. */
    SPxErrorCode SetValue(const char *value);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxInfoDBStringField */

/*
 *  SPxInfoDBIntField class.
 */
class SPxInfoDBIntField : public SPxInfoDBField
{
public:
    /* The item this field belongs to is responsible for managing it
     * so must be given full access.
     */
    friend class SPxInfoDBItem;

    /* Constructor and destructor. */
    SPxInfoDBIntField(const char *name, SPxInfoDBNode *parentNode);
    ~SPxInfoDBIntField(void);

    /* Get field value. */
    virtual SPxErrorCode GetValue(char *valBufRtn, unsigned int bufLen);
    SPxErrorCode GetValueAsInt(int *valRtn);

    /* Set/get displayed units. */
    virtual SPxErrorCode SetDisplayUnitString(const char *units);
    virtual SPxErrorCode GetDisplayUnitString(char *unitBufRtn, unsigned int bufLen);

protected:
    /* Constructor for use by deriving classes. */
    SPxInfoDBIntField(const char *name, FieldType type, SPxInfoDBNode *parentNode);

    /* Set field value. */
    SPxErrorCode SetValue(int value);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxInfoDBIntField */

/*
 *  SPxInfoDBDoubleField class.
 */
class SPxInfoDBDoubleField : public SPxInfoDBField
{
public:
    /* The item this field belongs to is responsible for managing it
     * so must be given full access.
     */
    friend class SPxInfoDBItem;

    /* Constructor and destructor. */
    SPxInfoDBDoubleField(const char *name, SPxInfoDBNode *parentNode);
    ~SPxInfoDBDoubleField(void);

    /* Get field value. */
    virtual SPxErrorCode GetValue(char *valBufRtn, unsigned int bufLen);
    SPxErrorCode GetValueAsDouble(double *valRtn);

    /* Set/get displayed units. */
    virtual SPxErrorCode SetDisplayUnitString(const char *units);
    virtual SPxErrorCode GetDisplayUnitString(char *unitBufRtn, unsigned int bufLen);

    /* Set/get displayed number of DP. */
    SPxErrorCode SetDisplayNumDP(int numDP);
    SPxErrorCode GetDisplayNumDP(int *numDPRtn);

protected:
    /* Constructor for use by deriving classes. */
    SPxInfoDBDoubleField(const char *name, FieldType type, SPxInfoDBNode *parentNode);

    /* Set field value. */
    SPxErrorCode SetValue(double value);
    virtual SPxErrorCode SetValue(double /*value*/, int /*format*/) { return SPX_ERR_NOT_SUPPORTED; }

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxInfoDBDoubleField */

/*
 *  SPxInfoDBDistanceField class.
 */
class SPxInfoDBDistanceField : public SPxInfoDBDoubleField
{
public:
    /* The item this field belongs to is responsible for managing it
     * so must be given full access.
     */
    friend class SPxInfoDBItem;

    /* Constructor and destructor. */
    SPxInfoDBDistanceField(const char *name, SPxInfoDBNode *parentNode);
    ~SPxInfoDBDistanceField(void);

    /* Get field value. */
    virtual SPxErrorCode GetValue(char *valBufRtn, unsigned int bufLen);
    SPxErrorCode GetValueAsFormattedDouble(double *valRtn, int units);

    /* Set/get displayed units. */
    SPxErrorCode SetDisplayUnits(int units);
    SPxErrorCode GetDisplayUnits(int *unitsRtn);

protected:

    /* Set field value. */
    SPxErrorCode SetValue(double value, int units);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxInfoDBDistanceField */

/*
 *  SPxInfoDBSpeedField class.
 */
class SPxInfoDBSpeedField : public SPxInfoDBDoubleField
{
public:
    /* The item this field belongs to is responsible for managing it
     * so must be given full access.
     */
    friend class SPxInfoDBItem;

    /* Constructor and destructor. */
    SPxInfoDBSpeedField(const char *name, SPxInfoDBNode *parentNode);
    ~SPxInfoDBSpeedField(void);

    /* Get field value. */
    virtual SPxErrorCode GetValue(char *valBufRtn, unsigned int bufLen);
    SPxErrorCode GetValueAsFormattedDouble(double * valRtn, int units);

    /* Set/get displayed units. */
    SPxErrorCode SetDisplayUnits(int units);
    SPxErrorCode GetDisplayUnits(int *unitsRtn);

protected:

    /* Set field value. */
    SPxErrorCode SetValue(double value, int units);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxInfoDBSpeedField */

/*
 *  SPxInfoDBLatitudeField class.
 */
class SPxInfoDBLatitudeField : public SPxInfoDBDoubleField
{
public:
    /* The item this field belongs to is responsible for managing it
     * so must be given full access.
     */
    friend class SPxInfoDBItem;

    /* Constructor and destructor. */
    SPxInfoDBLatitudeField(const char *name, SPxInfoDBNode *parentNode);
    ~SPxInfoDBLatitudeField(void);

    /* Get field value. */
    virtual SPxErrorCode GetValue(char *valBufRtn, unsigned int bufLen);

    /* Set/get displayed units. */
    SPxErrorCode SetDisplayUnits(int units);
    SPxErrorCode GetDisplayUnits(int *unitsRtn);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxInfoDBLatitudeField */

/*
 *  SPxInfoDBLongitudeField class.
 */
class SPxInfoDBLongitudeField : public SPxInfoDBDoubleField
{
public:
    /* The item this field belongs to is responsible for managing it
     * so must be given full access.
     */
    friend class SPxInfoDBItem;

    /* Constructor and destructor. */
    SPxInfoDBLongitudeField(const char *name, SPxInfoDBNode *parentNode);
    ~SPxInfoDBLongitudeField(void);

    /* Get field value. */
    virtual SPxErrorCode GetValue(char *valBufRtn, unsigned int bufLen);

    /* Set/get displayed units. */
    SPxErrorCode SetDisplayUnits(int units);
    SPxErrorCode GetDisplayUnits(int *unitsRtn);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxInfoDBLongitudeField */

/*
 *  SPxInfoDBTimeField class.
 */
class SPxInfoDBTimeField : public SPxInfoDBIntField
{
public:
    /* The item this field belongs to is responsible for managing it
     * so must be given full access.
     */
    friend class SPxInfoDBItem;

    /* Constructor and destructor. */
    SPxInfoDBTimeField(const char *name, SPxInfoDBNode *parentNode);
    ~SPxInfoDBTimeField(void);

    /* Get field value. */
    virtual SPxErrorCode GetValue(char *valBufRtn, unsigned int bufLen);

}; /* SPxInfoDBTimeField */

/*
 *  SPxInfoDBEnumField class.
 */
class SPxInfoDBEnumField : public SPxInfoDBIntField
{
public:
    /* The item this field belongs to is responsible for managing it
     * so must be given full access.
     */
    friend class SPxInfoDBItem;

    /* Constructor and destructor. */
    SPxInfoDBEnumField(const char *name, SPxInfoDBNode *parentNode);
    ~SPxInfoDBEnumField(void);

    /* Set/get the name of the set of enumeration definition used. */
    virtual SPxErrorCode SetEnumName(const char* enumName);
    virtual const char* GetEnumName(void) const;

    /* Get field value. */
    virtual SPxErrorCode GetValue(char *valBufRtn, unsigned int bufLen);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxInfoDBEnumField class */

/*
 *  SPxInfoDBDateField class.
 */
class SPxInfoDBDateField : public SPxInfoDBIntField
{
public:
    /* The item this field belongs to is responsible for managing it
     * so must be given full access.
     */
    friend class SPxInfoDBItem;

    /* Constructor and destructor. */
    SPxInfoDBDateField(const char *name, SPxInfoDBNode *parentNode);
    ~SPxInfoDBDateField(void);

    /* Get field value. */
    virtual SPxErrorCode GetValue(char *valBufRtn, unsigned int bufLen);

}; /* SPxInfoDBDateField */

/*
 *  SPxInfoDBIFFCodeField class.
 */
class SPxInfoDBIFFCodeField : public SPxInfoDBIntField
{
public:
    /* The item this field belongs to is responsible for managing it
     * so must be given full access.
     */
    friend class SPxInfoDBItem;

    /* Constructor and destructor. */
    SPxInfoDBIFFCodeField(const char *name, SPxInfoDBNode *parentNode);
    ~SPxInfoDBIFFCodeField(void);

    /* Get field value. */
    virtual SPxErrorCode GetValue(char *valBufRtn, unsigned int bufLen);

}; /* SPxInfoDBIFFCodeField */

/*
 *  SPxInfoDBBoolField class.
 */
class SPxInfoDBBoolField : public SPxInfoDBIntField
{
public:
    /* The item this field belongs to is responsible for managing it
     * so must be given full access.
     */
    friend class SPxInfoDBItem;

    /* Constructor and destructor. */
    SPxInfoDBBoolField(const char *name, SPxInfoDBNode *parentNode);
    ~SPxInfoDBBoolField(void);

    /* Get field value. */
    virtual SPxErrorCode GetValue(char *valBufRtn, unsigned int bufLen);

}; /* SPxInfoDBBoolField */

#endif /* _SPX_INFODB_FIELD_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
