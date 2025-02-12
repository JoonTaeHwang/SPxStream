/*********************************************************************
 *
 * (c) Copyright 2016 - 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxSQLDatabase class header.
 *
 * Revision Control:
 *   12/06/20 v1.13  SP 	Add GetTimeRange().
 *
 * Previous Changes:
 *   10/06/20 1.12  SP 	Support recording of RDR event markers.
 *   22/06/18 1.11  AGC	Fix new cppcheck warnings.
 *   14/02/18 1.10  AGC	Column indices are not fixed.
 *   28/07/16 1.9   SP 	Support import from recording file.
 *   14/07/16 1.8   SP 	Add GetTables().
 *   08/07/16 1.7   SP 	Make table names public.
 *                      Add error message return to Query().
 *   24/06/16 1.6   SP 	Add GetTableFromChanType().
 *   16/06/16 1.5   SP 	Add support for nav data, AIS and ADS-B.
 *   20/04/16 1.4   SP 	Add more debug support.
 *   08/04/16 1.3   SP 	Support query callback.
 *   30/03/16 1.2   SP 	Support tracker status messages.
 *                      Remove dependency on sqlite.h.
 *   24/03/16 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_SQL_DATABASE_H
#define _SPX_SQL_DATABASE_H

/* Other headers required. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibData/SPxPackets.h"
#include "SPxLibData/SPxChannel.h"

/*********************************************************************
 *
 *  Constants
 *
 *********************************************************************/

/* Extension to use for SQL database files. */
#define SPX_SQL_DB_FILE_EXT ".db"

/* Table names. */
#define SPX_SQL_DB_TRACK_RPT_TABLE_NAME         "TrackReports"
#define SPX_SQL_DB_TRACKER_STATUS_TABLE_NAME    "TrackerStatus"
#define SPX_SQL_DB_NAV_DATA_RPT_TABLE_NAME      "NavDataReports"
#define SPX_SQL_DB_AIS_RPT_TABLE_NAME           "AISReports"
#define SPX_SQL_DB_ADSB_RPT_TABLE_NAME          "ADSBReports"
#define SPX_SQL_DB_RECORD_TABLE_NAME            "Markers"   /* To match naming convention in RDR */

/* 
 * Create() flags (0x00000001 to 0x00000080). 
 */

/* Allow an existing database to be overwritten. */
#define SPX_SQL_DB_CREATE_ALLOW_OVERWRITE   0x00000001

/* 
 * Open() flags (0x00000100 to 0x00008000). 
 */

/* Open file in database file in read-only mode. */
#define SPX_SQL_DB_OPEN_READ_ONLY           0x00000100

/* 
 * Common flags (0x00010000 to 0x00800000). 
 */

/* By default the database is optimised for speed, specifically 
 * when performing INSERTs. Without this flag set an insert may take 
 * <=1ms. With this flag set it will be approx 100 times slower.
 * Speed increase comes at the expense of database integrity and
 * so disabling this option means that a database is less likely to
 * be corrupted following a PC or disk failure.
 */
#define SPX_SQL_DB_SLOW_AND_ROBUST          0x00010000

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

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Forward declare SPx classes required. */
class SPxNavData;
class SPxTrackDatabase;
class SPxAISDecoder;
class SPxADSBDecoder;
class SPxAISTrack;
class SPxADSBTrack;
class SPxAsterixDecoder;
class SPxAsterixMsg;
class SPxPacketDecoderFile;

/* Forward declare sqlite types. */
struct sqlite3;
struct sqlite3_stmt;

/* Forward declare the database class to use in the callback. */
class SPxSQLDatabase;

/* Define the type of the callback function. */
typedef void (* SPxSQLQueryResultFn_t)(void *userArg,
                                       SPxSQLDatabase *db,
                                       int numColumns,
                                       const char **columnValues,
                                       const char **columnNames);

class SPxSQLDatabase : public SPxObj
{
public:

    /*
     * Public types.
     */

    /* Table info. */
    typedef struct
    {
        char name[256]; /* Table name. */

    } TableInfo_t;

    /* Column info. */
    typedef struct
    {
        unsigned int index; /* Index from zero. */
        char name[256];     /* Column name. */
        char type[32];      /* Column type. */

    } ColumnInfo_t;

    /*
     * Public variables.
     */

    /*
     * Public static functions.
     */

    static SPxChannel::Type_t GetSupportedChanTypes(void);
    static const char *GetTableFromChanType(SPxChannel::Type_t);

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSQLDatabase(void);
    virtual ~SPxSQLDatabase(void);

    /* Create / open / close database. */
    virtual SPxErrorCode Create(const char *name, UINT32 flags);
    virtual SPxErrorCode Open(const char *name, UINT32 flags);
    SPxErrorCode Close(void);
    virtual int IsOpen(void);

    /* Import a recording file. */
    virtual SPxErrorCode Import(const char *filename);
    virtual SPxErrorCode GetImportTime(SPxTime_t *time);
   
    /* Query the database. */
    virtual SPxErrorCode GetTables(TableInfo_t **tablesRtn,
                                   unsigned int *numTablesRtn);

    virtual SPxErrorCode GetColumns(const char *table,
                                    ColumnInfo_t **columnsRtn,
                                    unsigned int *numColumnsRtn);

    virtual SPxErrorCode GetTimeRange(const char *table,
                                      SPxTime_t *firstRtn,
                                      SPxTime_t *lastRtn);

    virtual SPxErrorCode Query(const char *sql, 
                               SPxSQLQueryResultFn_t fn=NULL,
                               void *userArg=NULL,
                               char *sqlErrBuf=NULL,
                               unsigned int sqlErrBufSize=0);

    /* Stop a database operation. */
    virtual SPxErrorCode Interrupt(void);

    /* Tracks. */
    virtual SPxErrorCode AddTrackReport(const SPxTime_t *timestamp,
                                        const SPxPacketTrackMinimal *minRpt,
                                        const SPxPacketTrackNormal *normRpt,
                                        const SPxPacketTrackExtended *extRpt,
                                        UINT8 channelIndex=0);

    /* Tracker status. */
    virtual SPxErrorCode AddTrackerStatus(const SPxTime_t *timestamp,
                                          const SPxPacketTrackerStatus_t *status,
                                          UINT8 channelIndex=0);

    /* Nav data. */
    virtual SPxErrorCode AddNavDataReport(const SPxTime_t *timestamp,
                                          const char *sentenceType,
                                          SPxNavData *navData,
                                          UINT8 channelIndex=0);

    /* AIS. */
    virtual SPxErrorCode AddAISReport(const SPxTime_t *timestamp,
                                      int mesgType,
                                      SPxAISTrack *aisTrack,
                                      UINT8 channelIndex=0);

    /* ADS-B. */
    virtual SPxErrorCode AddADSBReport(const SPxTime_t *timestamp,
                                       int mesgType,
                                       SPxADSBTrack *adsbTrack,
                                       UINT8 channelIndex=0);

    /* Records. */
    virtual SPxErrorCode AddRecord(SPxPacketRecord *rec);

    /* Debugging. */
    virtual void SetLogFile(FILE *fPtr) { m_logFile = fPtr; }
    virtual FILE *GetLogFile(void) { return m_logFile; }
    virtual void SetVerbosity(unsigned int level) { m_verbosity = level; }
    virtual unsigned int GetVerbosity(void) { return m_verbosity; }

    /* Stats. */
    virtual unsigned int GetNumRowsAdded(void) { return m_numRowsAdded; }

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Protected types.
     */

    /* Time range info. */
    typedef struct
    {
        SPxTime_t first;
        SPxTime_t last;

    } TimeRangeInfo_t;

    /*
     * Private variables.
     */

    /*
     * Indicies into statement lists and numbers of statements.
     * KEEP THESE SYNCHRONISED TO STATEMENT DEFINITIONS IN SOURCE. 
     */

    /* Indicies into any list of statemnts. */
    static const unsigned int STMT_CREATE_TABLE = 0;
    static const unsigned int STMT_ADD = 1;

    /* Track report statements. */
    static const unsigned int NUM_TRACK_RPT_STMTS = 2;
    sqlite3_stmt *m_trackRptStmts[NUM_TRACK_RPT_STMTS];

    /* Tracker status statements. */
    static const unsigned int NUM_TRACKER_STATUS_STMTS = 2;
    sqlite3_stmt *m_trackerStatusStmts[NUM_TRACKER_STATUS_STMTS];

    /* Nav data report statements. */
    static const unsigned int NUM_NAV_DATA_RPT_STMTS = 2;
    sqlite3_stmt *m_navDataRptStmts[NUM_NAV_DATA_RPT_STMTS];

    /* AIS report statements. */
    static const unsigned int NUM_AIS_RPT_STMTS = 2;
    sqlite3_stmt *m_aisRptStmts[NUM_AIS_RPT_STMTS];

    /* ADS-B report statements. */
    static const unsigned int NUM_ADSB_RPT_STMTS = 2;
    sqlite3_stmt *m_adsbRptStmts[NUM_ADSB_RPT_STMTS];

    /* ADS-B report statements. */
    static const unsigned int NUM_RECORD_STMTS = 2;
    sqlite3_stmt *m_recordStmts[NUM_RECORD_STMTS];

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The database and its mutex. */
    sqlite3 *m_db;
    SPxCriticalSection m_dbMutex;

    /* User callback when runSQL() is invoked. */
    SPxSQLQueryResultFn_t m_queryFn;
    void *m_queryFnArg;
    
    /* Importing. */
    SPxPacketDecoderFile *m_importDecoder;
    int m_stopImporting;
    SPxNavData *m_importNavData;
    SPxAISDecoder *m_importAISDecoder;
    SPxADSBDecoder *m_importADSBDecoder;
    SPxAsterixDecoder *m_importAsterixDecoder;
    SPxTime_t m_asterixTrackTime;
    UINT8 m_asterixTrackChannelIndex;

    /* Debugging. */
    unsigned int m_verbosity;
    FILE *m_logFile;

    /* Stats. */
    unsigned int m_numRowsAdded;

    /*
     * Private functions.
     */

    /* Utility functions. */
    void reportDebug(unsigned int level, const char *mesg);

    SPxErrorCode checkError(int sqlErr, const char *funcName, int arg=0);

    SPxErrorCode initDatabase(const char *name,
                              int sqliteFlags,
                              UINT32 flags);

    SPxErrorCode checkDatabase(const char *funcName, int reportErr=TRUE);

    SPxErrorCode createTable(const char *tableName,
                             const char *sqlList[],
                             sqlite3_stmt *stmtList[],
                             unsigned int numStmts);

    SPxErrorCode createStatement(const char *sql, sqlite3_stmt **stmt);

    SPxErrorCode destroyStatement(sqlite3_stmt **stmt);

    SPxErrorCode destroyAllStatements(void);

    SPxErrorCode setStatementParam(sqlite3_stmt *stmt, 
                                   const char *param, 
                                   double value);
    SPxErrorCode setStatementParam(sqlite3_stmt *stmt, 
                                   const char *param, 
                                   int value);
    SPxErrorCode setStatementParam(sqlite3_stmt *stmt, 
                                   const char *param, 
                                   unsigned int value);
    SPxErrorCode setStatementParam(sqlite3_stmt *stmt, 
                                   const char *param, 
                                   const char *value);
    SPxErrorCode setStatementParam(sqlite3_stmt *stmt, 
                                   const char *param, 
                                   const UINT8 *value);
    SPxErrorCode setStatementParam(sqlite3_stmt *stmt, 
                                   const char *param, 
                                   const UINT8 *values,
                                   unsigned int numValues);
    SPxErrorCode setStatementParam(sqlite3_stmt *stmt, 
                                   const char *param, 
                                   const UINT32 *values,
                                   unsigned int numValues);

    SPxErrorCode setTimestampParam(sqlite3_stmt *stmt,
                                   const char *param, 
                                   const SPxTime_t *timestamp);

    SPxErrorCode runStatement(sqlite3_stmt *stmt);

    SPxErrorCode runSQL(const char *sql, 
                        SPxSQLQueryResultFn_t fn=NULL,
                        void *userArg=NULL,
                        char *sqlErrBuf=NULL,
                        unsigned int sqlErrBufSize=0);
   
    /*
     * Private static functions.
     */

    static int runSQLCallback(void *dbVoid,
                              int numColumns,
                              char **columnValues,
                              char **columnNames);

    static void getTableInfoCallback(void *userArg,
                                     SPxSQLDatabase *db,
                                     int numColumns,
                                     const char **columnValues,
                                     const char **columnNames);

    static void getColumnInfoCallback(void *userArg,
                                      SPxSQLDatabase *db,
                                      int numColumns,
                                      const char **columnValues,
                                      const char **columnNames);

    static void getTimeRangeCallback(void *userArg,
                                     SPxSQLDatabase *db,
                                     int numColumns,
                                     const char **columnValues,
                                     const char **columnNames);

    static void importNavDataHandler(SPxPacketDecoder *decoder,
                                     void *userArg,
                                     UINT32 packetType,
                                     struct sockaddr_in *from,
                                     SPxTime_t *timestamp,
                                     const unsigned char *payload,
                                     unsigned int numBytes);

    static void importSPxTrackHandler(SPxPacketDecoder *decoder,
                                      void *userArg,
                                      UINT32 packetType,
                                      struct sockaddr_in *from,
                                      SPxTime_t *timestamp,
                                      const unsigned char *payload,
                                      unsigned int numBytes);

     static void importAsterixTrackHandler(SPxPacketDecoder *decoder,
                                           void *userArg,
                                           UINT32 packetType,
                                           struct sockaddr_in *from,
                                           SPxTime_t *timestamp,
                                           const unsigned char *payload,
                                           unsigned int numBytes);

     static void decodedAsterixTrackHandler(const SPxAsterixDecoder *decoder,
                                            void *userArg,
                                            UINT8 category,
                                            SPxAsterixMsg *msg);

     static void importTrackerStatusHandler(SPxPacketDecoder *decoder,
                                            void *userArg,
                                            UINT32 packetType,
                                            struct sockaddr_in *from,
                                            SPxTime_t *timestamp,
                                            const unsigned char *payload,
                                            unsigned int numBytes);

    static void importAISHandler(SPxPacketDecoder *decoder,
                                 void *userArg,
                                 UINT32 packetType,
                                 struct sockaddr_in *from,
                                 SPxTime_t *timestamp,
                                 const unsigned char *payload,
                                 unsigned int numBytes);

    static void importADSBHandler(SPxPacketDecoder *decoder,
                                  void *userArg,
                                  UINT32 packetType,
                                  struct sockaddr_in *from,
                                  SPxTime_t *timestamp,
                                  const unsigned char *payload,
                                  unsigned int numBytes);

    static void importRecordHandler(SPxPacketDecoder *decoder,
                                    void *userArg,
                                    UINT32 packetType,
                                    struct sockaddr_in *from,
                                    SPxTime_t *timestamp,
                                    const unsigned char *payload,
                                    unsigned int numBytes);

}; /* SPxSQLDatabase */

#endif /* _SPX_SQL_DATABASE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
