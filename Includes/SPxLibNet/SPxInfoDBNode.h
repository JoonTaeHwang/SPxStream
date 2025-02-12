/*********************************************************************
*
* (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxInfoDBNode class.
*
*	This class form a single node in a distributed information
*	database.
*	It will handle creation, updating and deletion of all items
*	it is resposible for managing.
*	It will also handle all communications with other nodes. 
*
* Revision Control:
*   15/09/22 v1.32  BTB	Rework how enum definition names are configured.
*
* Previous Changes:
*   28/02/22 1.31  BTB	Support saving messages to records file.
*   25/01/22 1.30  BTB	Support operation over a WAN.
*   26/04/21 1.29  BTB	Support Boolean field type.
*   07/04/21 1.28  BTB	Support purging of old/pending deleted items.
*   07/04/21 1.27  BTB	Support items being pending/live.
*   17/03/21 1.26  BTB	Add generic string-based units.
*   15/09/20 1.25  BTB	Support IFF code field type.
*   09/09/20 1.24  BTB	Add GetItemState().
*   08/09/20 1.23  BTB	Support date field type.
*   08/09/20 1.22  BTB	Add GetItemTimestamp().
*   04/09/20 1.21  BTB	Allow configuration of whether to check item state when editing.
*   04/09/20 1.20  BTB	Allow node state to be specified.
*   03/09/20 1.19  BTB	Add ItemHasField().
*   01/09/20 1.18  BTB	Support "states" to track which nodes are managing which items.
*   27/08/20 1.17  BTB	Support more general file loading/saving.
*   13/08/20 1.16  BTB	Support enumeration field type.
*   16/07/20 1.15  BTB	Make file saving/loading separately configurable.
*   06/07/20 1.14  BTB	Use correct variable type for time fields.
*   06/07/20 1.13  BTB	Support time field type.
*   03/07/20 1.12  BTB	Support configuring displayed number of DP.
*   30/06/20 1.11  BTB	Add ItemHasField().
*   29/06/20 1.10  BTB	Add GetItemNumFields().
*   15/06/20 1.9   BTB	Support latitude and longitude field types.
*   12/06/20 1.8   BTB	Support configuration of display of field strings.
*   08/06/20 1.7   BTB	Support distance and speed field types.
*   20/05/20 1.6   BTB	Support saving to/loading from file.
*   18/05/20 1.5   BTB	Support field types other than string.
*                     	Support fetching of field value.
*   06/05/20 1.4   BTB	Support TCP request sending/receiving.
*                     	Send updates more intelligently.
*   05/05/20 1.3   BTB	Support receipt of item updates.
*   30/04/20 1.2   BTB	Add DeleteItem().
*                     	Fix updates.
*   29/04/20 1.1   BTB	Initial Version.
*
**********************************************************************/

#ifndef _SPX_INFODB_NODE_H
#define _SPX_INFODB_NODE_H

/*
 * Other headers required.
 */

#include "SPxLibNet/SPxInfoDBItem.h"
#include "SPxLibNet/SPxNetTCP.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxTimer.h"

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
class SPxPacketDecoder;

/*
 *  SPxInfoDBNode class.
 */
class SPxInfoDBNode : public SPxInfoDBItem
{
public:
    /*
     * Public types.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxInfoDBNode(const char *state = "", int checkState = FALSE);
    ~SPxInfoDBNode(void);

    /* Start all network communications. */
    void Start(void);

    /* Access to existing items. */
    SPxErrorCode IterateItems(SPxCallbackListFn_t fn, void *userArg);
    SPxErrorCode IterateActiveItems(SPxCallbackListFn_t fn, void *userArg);
    SPxErrorCode IterateInactiveItems(SPxCallbackListFn_t fn, void *userArg);

    /* Management of items. */
    const char *CreateItem(const char *type);
    int IsItemPending(const char *uuid);
    SPxErrorCode SetItemLive(const char *uuid);
    SPxErrorCode UpdateItemType(const char *uuid, const char *type);
    SPxErrorCode UpdateItemState(const char *uuid, const char *state);
    SPxErrorCode DeleteItem(const char *uuid);

    /* Number of fields within an item. */
    unsigned int GetItemNumFields(const char *uuid);

    /* Access to info for enum value/string configurations. */
    unsigned int GetEnumValueCount(const char *enumName);
    SPxErrorCode GetEnumValueList(const char * enumName, int *valueBufRtn, unsigned int bufLen);
    SPxErrorCode GetEnumStringFromValue(const char * enumName, int value, char *strBufRtn, unsigned int bufLen);

    /* Access to generic item information */
    int ItemIsActive(const char *uuid);
    double GetItemTimestamp(const char *uuid);
    const char *GetItemState(const char *uuid);

    /* Whether an item has a particular field. */
    int ItemHasField(const char *uuid, const char *fieldName);

    /* Access to existing fields within items. */
    SPxErrorCode IterateItemFields(const char *uuid, SPxCallbackListFn_t fn, void *userArg);

    /* Access to field enum names within items. */
    SPxErrorCode SetItemFieldEnumName(const char* uuid, const char* fieldName, const char* enumName);
    const char* GetItemFieldEnumName(const char* uuid, const char* fieldName);

    /* Access to field values within items. */
    SPxErrorCode GetItemFieldValue(const char *uuid, const char *fieldName, char *valBufRtn, unsigned int bufLen);
    SPxErrorCode GetItemFieldValueAsInt(const char *uuid, const char *fieldName, int *valRtn);
    SPxErrorCode GetItemFieldValueAsDouble(const char *uuid, const char *fieldName, double *valRtn);
    SPxErrorCode GetItemFieldValueAsDistance(const char *uuid, const char *fieldName, double *valRtn, SPxUnitsDist_t units);
    SPxErrorCode GetItemFieldValueAsSpeed(const char *uuid, const char *fieldName, double *valRtn, SPxUnitsSpeed_t units);

    /* Creation/updating of fields within items. */
    SPxErrorCode UpdateItemStringField(const char *uuid, const char *fieldName, const char* fieldValue);
    SPxErrorCode UpdateItemIntField(const char *uuid, const char *fieldName, int fieldValue);
    SPxErrorCode UpdateItemDoubleField(const char *uuid, const char *fieldName, double fieldValue);
    SPxErrorCode UpdateItemDistanceField(const char *uuid, const char *fieldName, double fieldValue, SPxUnitsDist_t units);
    SPxErrorCode UpdateItemSpeedField(const char *uuid, const char *fieldName, double fieldValue, SPxUnitsSpeed_t units);
    SPxErrorCode UpdateItemLatitudeField(const char *uuid, const char *fieldName, double fieldValue);
    SPxErrorCode UpdateItemLongitudeField(const char *uuid, const char *fieldName, double fieldValue);
    SPxErrorCode UpdateItemEnumField(const char *uuid, const char *fieldName, int fieldValue);
    SPxErrorCode UpdateItemTimeField(const char *uuid, const char *fieldName, int fieldValue);
    SPxErrorCode UpdateItemDateField(const char *uuid, const char *fieldName, int fieldValue);
    SPxErrorCode UpdateItemIFFCodeField(const char *uuid, const char *fieldName, int fieldValue);
    SPxErrorCode UpdateItemBoolField(const char *uuid, const char *fieldName, int fieldValue);

    /* Set/get display units for fields within items. */
    SPxErrorCode SetItemFieldDisplayUnits(const char *uuid, const char *fieldName, int units);
    SPxErrorCode GetItemFieldDisplayUnits(const char *uuid, const char *fieldName, int *unitsRtn);
    const char *GetUnitsLabelForField(SPxInfoDBField::FieldType fieldType, int units);
    SPxErrorCode SetItemFieldDisplayUnitString(const char *uuid, const char *fieldName, const char *units);
    SPxErrorCode GetItemFieldDisplayUnitString(const char *uuid, const char *fieldName, char *unitBufRtn, unsigned int bufLen);

    /* Set/get display num DP for fields within items. */
    SPxErrorCode SetItemFieldDisplayNumDP(const char *uuid, const char *fieldName, int numDP);
    SPxErrorCode GetItemFieldDisplayNumDP(const char *uuid, const char *fieldName, int *numDPRtn);

    SPxErrorCode DeleteItemField(const char *uuid, const char *fieldName);

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Network communication management. */
    SPxErrorCode SetSendRecvAddress(const char *address);
    const char *GetSendRecvAddress(void) const;

    SPxErrorCode SetSendRecvPort(int port);
    int GetSendRecvPort(void) const;

    SPxErrorCode SetSendRecvIfAddress(const char *ifAddr);
    const char *GetSendRecvIfAddress(void) const;

    SPxErrorCode SetUseDuplicateSender(int use);
    int GetUseDuplicateSender(void) const;

    SPxErrorCode SetDuplicateSendAddress(const char *address);
    const char *GetDuplicateSendAddress(void) const;

    SPxErrorCode SetDuplicateSendPort(int port);
    int GetDuplicateSendPort(void) const;

    SPxErrorCode SetDuplicateSendIfAddress(const char *ifAddr);
    const char *GetDuplicateSendIfAddress(void) const;

    SPxErrorCode SetRequestRecvPort(int port);
    int GetRequestRecvPort(void) const;

    SPxErrorCode SetRequestRecvIfAddress(const char *ifAddr);
    const char *GetRequestRecvIfAddress(void) const;

    /* Load from/save to file. */
    SPxErrorCode LoadFromFile(const char *filename);
    SPxErrorCode SaveToFile(const char *filename);

    /* Backup file configuration. */
    SPxErrorCode SetLoadFromBackup(int enable);
    int GetLoadFromBackup(void) const;

    SPxErrorCode SetSaveToBackup(int enable);
    int GetSaveToBackup(void) const;

    SPxErrorCode SetBackupFilename(const char *filename);
    const char *GetBackupFilename(void) const;

    /* Records file configuration. */
    SPxErrorCode SetSaveRecords(int enable);
    int GetSaveRecords(void) const;

    SPxErrorCode SetRecordSentMessages(int enable);
    int GetRecordSentMessages(void) const;

    SPxErrorCode SetRecordBaseFilename(const char *filename);
    const char *GetRecordBaseFilename(void) const;

    /* Timeout for purging deleted items. */
    SPxErrorCode SetPurgeTimeout(unsigned int timeoutSecs);
    unsigned int GetPurgeTimeout(void);

    /* Period for sending full data update. */
    SPxErrorCode SetFullUpdatePeriod(unsigned int periodSecs);
    unsigned int GetFullUpdatePeriod(void);

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Timer for periodically sending required messages. */
    SPxTimer m_msgTimer;

    /*
     * Private functions.
     */

    /* Enumeration definitions configuration. */
    SPxErrorCode setEnumDefName(int index, const char *name);
    SPxErrorCode getEnumDefName(int index, char *nameBufRtn, unsigned int bufLen);

    /* Load/save full data from/to backup file. */
    void loadFromBackupFile(void);
    void saveToBackupFile(void);

    /* Write full JSON data to given writer. */
    SPxErrorCode writeItemList(void *msgWriterVoid);

    /* Functions for sending UDP JSON data. */
    void sendUpdate(SPxInfoDBItem *item);
    void sendStatus(void);
    void sendTimes(void);
    void sendMessageData(const char *jsonData, int sendDuplicate, int recordMessage);

    /* Function for sending requested TCP JSON data. */
    void sendRequestedInfo(SOCKET_TYPE sock);

    /* Functions for processing received JSON messages. */
    void handleReceivedInfo(const unsigned char *payload,
                            unsigned int numBytes,
                            struct sockaddr_in *sender);
    void handleMsg(const void *jsonMsgVoid, struct sockaddr_in *sender);

    /* Function for saving records data. */
    void saveRecordData(const char *jsonData);

    /* Check if items have updated or need to be purged. */
    void checkItems(void);

    /* Function for checking the status of possible sent request. */
    void checkRequestStatus(void);

    /* Check if a full update (all item data) needs to be sent. */
    void checkForFullUpdate(void);

    /* Get a field containing connection settings from a stored node item. */
    void getConnectionField(SPxInfoDBField *field);

    /* Handler functions. */
    static void infoRecvHandler(SPxPacketDecoder *decoder,
                                void *userArg,
                                UINT32 packetType,
                                struct sockaddr_in *sender,
                                SPxTime_t *timestamp,
                                const unsigned char *payload,
                                unsigned int numBytes);
    static void requestHandler(SPxNetTCP *tcp, void *userArg, SOCKET_TYPE sock);

    /* Timers for checking status and sending periodic messages. */
    static void fastTimer(void *userArg);
    static void slowTimer(void *userArg);

    /* Iterator for getting the fields containing connection settings
     * from a stored node item.
     */
    static int getConnectionFieldIterator(void *dbItemVoid, 
                                          void *objVoid, 
                                          void *fieldVoid);

}; /* SPxInfoDBNode */

#endif /* _SPX_INFODB_NODE_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
