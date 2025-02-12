/*********************************************************************
*
* (c) Copyright 2020 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxInfoDBItem class.
*
*	This class form a single item of information in a distributed
*	information database.
*
* Revision Control:
*   26/04/21 v1.21  BTB	Support Boolean field type.
*
* Previous Changes:
*   07/04/21 1.20  BTB	Support items being pending/live.
*   22/03/21 1.19  BTB	Improve how items are deleted (fixes race-condition).
*   17/03/21 1.18  BTB	Add generic string-based units.
*   15/09/20 1.17  BTB	Support IFF code field type.
*   08/09/20 1.16  BTB	Support date field type.
*   01/09/20 1.15  BTB	Support "states" to track which nodes are managing which items.
*   14/08/20 1.14  BTB	Add SetParentNode().
*   13/08/20 1.13  BTB	Support enumeration field type.
*   06/07/20 1.12  BTB	Support time field type.
*   03/07/20 1.11  BTB	Support configuring displayed number of DP.
*   30/06/20 1.10  BTB	Add HasField().
*   29/06/20 1.9   BTB	Add GetNumFields().
*   15/06/20 1.8   BTB	Support latitude and longitude field types.
*   12/06/20 1.7   BTB	Improve configuration of display of field strings.
*   08/06/20 1.6   BTB	Support distance and speed field types.
*   18/05/20 1.5   BTB	Support field types other than string.
*                     	Support fetching of field value.
*   06/05/20 1.4   BTB	Manage whether an update needs to be sent.
*   05/05/20 1.3   BTB	Support receipt of item updates in the node.
*   30/04/20 1.2   BTB	Add DeleteAllFields().
*   29/04/20 1.1   BTB	Initial Version.
*
**********************************************************************/

#ifndef _SPX_INFODB_ITEM_H
#define _SPX_INFODB_ITEM_H

/*
 * Other headers required.
 */

#include "SPxLibNet/SPxInfoDBField.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

#define SPX_INFODB_ITEM_FLAGS_DELETED   0x00000001

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
 *  SPxInfoDBItem class.
 */
class SPxInfoDBItem : public SPxObj
{
public:
    /* Node derived class also manages all other items so
     * must be given full access.
     */
    friend class SPxInfoDBNode;

    /*
     * Public types.
     */

    /*
     * Public functions.
     */

    /* Get general item information. */
    const char *GetUUID(void) const;
    const char *GetType(void) const;
    const char *GetState(void) const;
    UINT32 GetFlags(void) const { return m_flags; }
    int IsDeleted(void) const { return (m_flags & SPX_INFODB_ITEM_FLAGS_DELETED) ? TRUE : FALSE; }
    int IsPending(void) const { return m_pending; }

    /* Number of fields in the item. */
    unsigned int GetNumFields(void) const;

    /* Whether the item has a particular field. */
    int HasField(const char *name);

    /* Access to existing fields. */
    SPxErrorCode IterateFields(SPxCallbackListFn_t fn, void *userArg);

    /* Access to field enum name. */
    SPxErrorCode SetFieldEnumName(const char* name, const char* enumName);
    const char* GetFieldEnumName(const char* name);

    /* Access to field values. */
    SPxErrorCode GetFieldValue(const char *name, char *valBufRtn, unsigned int bufLen);
    SPxErrorCode GetFieldValueAsInt(const char *name, int *valRtn);
    SPxErrorCode GetFieldValueAsDouble(const char *name, double *valRtn);
    SPxErrorCode GetFieldValueAsDistance(const char *name, double *valRtn, SPxUnitsDist_t units);
    SPxErrorCode GetFieldValueAsSpeed(const char *name, double *valRtn, SPxUnitsSpeed_t units);

    /* Set/get field display format. */
    SPxErrorCode SetFieldDisplayUnits(const char *name, int units);
    SPxErrorCode GetFieldDisplayUnits(const char *name, int *unitsRtn);
    SPxErrorCode SetFieldDisplayUnitString(const char *name, const char *units);
    SPxErrorCode GetFieldDisplayUnitString(const char *name, char *unitBufRtn, unsigned int bufLen);

    /* Set/get field display num DP. */
    SPxErrorCode SetFieldDisplayNumDP(const char *name, int numDP);
    SPxErrorCode GetFieldDisplayNumDP(const char *name, int *numDPRtn);

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Constructor and destructor. */
    SPxInfoDBItem(const char *uuid, const char *type, const char *state, SPxInfoDBNode *parentNode, int pending = FALSE);
    ~SPxInfoDBItem(void);

    void SetParentNode(SPxInfoDBNode *parentNode) { m_parentNode = parentNode; }

    /* Set general item information. */
    SPxErrorCode SetUUID(const char *uuid);
    SPxErrorCode SetType(const char *type);
    SPxErrorCode SetState(const char *state);
    SPxErrorCode SetFlags(UINT32 flags);
    SPxErrorCode AppendFlags(UINT32 flags);
    SPxErrorCode ClearFlags(UINT32 flags);
    SPxErrorCode SetLive(void);

    /* Management of timestamp. */
    void UpdateTimestamp(void);
    void SetTimestamp(double timestamp) { m_timestamp = timestamp; }
    double GetTimestamp(void) const { return m_timestamp; }

    /* Management of whether an update is needed. */
    int IsUpdateNeeded(void) const { return m_updateNeeded; }
    void SetUpdateNeeded(int isNeeded) { m_updateNeeded = isNeeded; }

    /* Creation/updating of fields. */
    SPxErrorCode UpdateStringField(const char *name, const char *value);
    SPxErrorCode UpdateIntField(const char *name, int value);
    SPxErrorCode UpdateEnumField(const char *name, int value);
    SPxErrorCode UpdateTimeField(const char *name, int value);
    SPxErrorCode UpdateDateField(const char *name, int value);
    SPxErrorCode UpdateIFFCodeField(const char *name, int value);
    SPxErrorCode UpdateBoolField(const char *name, int value);
    SPxErrorCode UpdateDoubleField(const char *name, double value);
    SPxErrorCode UpdateDistanceField(const char *name, double value, SPxUnitsDist_t units);
    SPxErrorCode UpdateSpeedField(const char *name, double value, SPxUnitsSpeed_t units);
    SPxErrorCode UpdateLatitudeField(const char *name, double value);
    SPxErrorCode UpdateLongitudeField(const char *name, double value);

    /* Deletion of fields. */
    SPxErrorCode DeleteField(const char *name);
    SPxErrorCode DeleteAllFields(void);

    /* Append JSON data for item. */
    SPxErrorCode AppendItemData(void *msgWriterVoid, int includeFields);
    SPxErrorCode AppendBaseDataOnly(void *msgWriterVoid);
    SPxErrorCode AppendFieldsOnly(void *msgWriterVoid);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Access to the parent node. */
    SPxInfoDBNode *m_parentNode;

    double m_timestamp;

    UINT32 m_flags;

    int m_updateNeeded;

    int m_pending;

    /*
     * Private functions.
     */

}; /* SPxInfoDBItem */

#endif /* _SPX_INFODB_ITEM_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
