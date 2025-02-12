/*********************************************************************
*
* (c) Copyright 2013, 2016 - 2019, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxNMEADecoder class which supports decoding
*	the component parts of NMEA sentences.
*
*
* Revision Control:
*   21/09/22 v1.8    AJH	Handle group parameter.
*
* Previous Changes:
*   14/07/22 1.7    AGC	Detect unexpected SPx packets.
*   21/06/22 1.6    AGC	Add LWE defines.
*   02/04/19 1.5    AGC	Handle tag blocks.
*   23/11/18 1.4    BTB	Increase maximum NMEA message size.
*   19/10/18 1.3    AGC	Support decoding proprietary sentences.
*   31/10/16 1.2    AGC	Add Skip() function.
*   08/04/13 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_NMEA_DECODER_H
#define _SPX_NMEA_DECODER_H

/*
 * Other headers required.
 */
/* For SPxErrorCode enumeration. */
#include "SPxLibUtils/SPxError.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Maximum number of parts to a single NMEA message. */
#define SPX_NMEA_DECODER_MAX_NMEA_PARTS	    (80)

/* Maximum elements in tag block. */
#define SPX_NMEA_DECODER_MAX_TAG_PARTS	    (26)

/* Maximum length of a single NMEA message in bytes.
 * The limit according to the standard is 82 bytes
 * (not including a NULL terminator), but this buffer is larger
 * to handle messages related to project P386.
 */
#define SPX_NMEA_DECODER_MAX_LEN	    (512)

/* Get the next part of an NMEA message from a GetPart...() function. */
#define SPX_NMEA_DECODER_NEXT_PART	    (-1)

#define SPX_NMEA_LWE_PREFIX		    "UdPbC"
#define SPX_NMEA_LWE_PREFIX_LEN		    (6)

/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

enum SPxNMEATransGroup
{
    SPX_NMEA_TRANS_GROUP_NONE = 0,
    SPX_NMEA_TRANS_GROUP_MISC = 1,
    SPX_NMEA_TRANS_GROUP_TGTD = 2, /* Target data (AIS/Radar Tracks). */
    SPX_NMEA_TRANS_GROUP_SATD = 3, /* High update rate nav data. */
    SPX_NMEA_TRANS_GROUP_NAVD = 4, /* Lower upate rate nav data. */
    SPX_NMEA_TRANS_GROUP_TIME = 7,
    SPX_NMEA_TRANS_GROUP_PROP = 8, /* Proprietary. */
    SPX_NMEA_TRANS_GROUP_USR1 = 9, /* User specified. */
    SPX_NMEA_TRANS_GROUP_USR2 = 11, /* User specified. */
    SPX_NMEA_TRANS_GROUP_USR3 = 12, /* User specified. */
    SPX_NMEA_TRANS_GROUP_USR4 = 13, /* User specified. */
    SPX_NMEA_TRANS_GROUP_USR5 = 14, /* User specified. */
    SPX_NMEA_TRANS_GROUP_USR6 = 15, /* User specified. */
    SPX_NMEA_TRANS_GROUP_USR7 = 16, /* User specified. */
    SPX_NMEA_TRANS_GROUP_USR8 = 17  /* User specified. */
};

enum SPxNMEATag
{
    SPX_NMEA_TAG_NONE		= 0x00,
    SPX_NMEA_TAG_UNIX_TIME	= 0x01,
    SPX_NMEA_TAG_DST_ID		= 0x02,
    SPX_NMEA_TAG_GROUP		= 0x04,
    SPX_NMEA_TAG_LINE_COUNT	= 0x08,
    SPX_NMEA_TAG_REL_TIME	= 0x10,
    SPX_NMEA_TAG_SRC_ID		= 0x20,
    SPX_NMEA_TAG_TEXT		= 0x40
};

/*
 * Define our class, NOT derived from SPxObj.
 */
class SPxNMEADecoder
{
public:
    /*
     * Public functions.
     */
    explicit SPxNMEADecoder(const char *nmea=NULL);
    virtual ~SPxNMEADecoder(void);

    SPxErrorCode Parse(const char *nmea);
    SPxErrorCode GetParseError(void) const { return m_parseError; }

    const char *GetNMEA(void) const { return m_nmea; }
    int GetNumParts(void) const { return m_numParts; }
    char GetStartDelimiter(void) const { return m_startDelim; }
    int IsProprietary(void) const { return m_isProprietary; }
    const char *GetTalker(void) const { return m_talker; }
    const char *GetSentenceType(void) const { return m_sentenceType; }
    int GetChecksum(void) const { return m_reqChecksum; }
    int GetChecksumRead(void) const { return m_checksum; }
    int IsChecksumValid(void) const { return (m_checksum == m_reqChecksum); }
    int GetTagChecksum(void) const { return m_reqTagChecksum; }
    int GetTagChecksumRead(void) const { return m_tagChecksum; }
    int IsTagChecksumValid(void) const { return (m_tagChecksum == m_reqTagChecksum); }

    SPxErrorCode GetPartAsStr(const char **s, int part=SPX_NMEA_DECODER_NEXT_PART) const;
    SPxErrorCode GetPartAsChar(char *c, int part=SPX_NMEA_DECODER_NEXT_PART) const;
    SPxErrorCode GetPartAsInt(int *i, int part=SPX_NMEA_DECODER_NEXT_PART) const;
    SPxErrorCode GetPartAsUint(unsigned int *u, int part=SPX_NMEA_DECODER_NEXT_PART) const;
    SPxErrorCode GetPartAsReal(float *f, int part=SPX_NMEA_DECODER_NEXT_PART) const;
    SPxErrorCode GetPartAsDouble(double *d, int part=SPX_NMEA_DECODER_NEXT_PART) const;
    SPxErrorCode Skip(unsigned int numParts=1) const;

    SPxErrorCode GetTag(char tag, const char **s) const;
    SPxErrorCode GetGroup(const char **s) const;

    /* Convenience functions for checksum handling (return -1 on error). */
    static int CalcChecksum(const char *nmea,
			    int checkFirstChar=TRUE);
    static int ReadChecksum(const char *nmea);

private:
    /*
     * Private variables.
     */
    SPxErrorCode m_parseError;	    /* Most recently parse status. */
    int m_numParts;		    /* Number of parts to most recent message. */
    mutable int m_nextPart;	    /* Number of next part to retreive. */
    char m_nmea[SPX_NMEA_DECODER_MAX_LEN];  /* Last parsed message. */
    char m_msg[SPX_NMEA_DECODER_MAX_NMEA_PARTS][SPX_NMEA_DECODER_MAX_LEN];  /* Parts of message. */
    char m_tags[SPX_NMEA_DECODER_MAX_TAG_PARTS][SPX_NMEA_DECODER_MAX_LEN];
    char m_group[SPX_NMEA_DECODER_MAX_LEN];
    char m_startDelim;		    /* Start delimiter from last message. */
    int m_isProprietary;	    /* Is this a proprietary sentence? */
    char m_talker[4];		    /* Talker from last message. */
    char m_sentenceType[4];	    /* Sentence type from last message. */
    int m_checksum;		    /* Read checksum from last message. */
    int m_reqChecksum;		    /* Calculated checksum from last message. */
    int m_tagChecksum;		    /* Tag block checksum from last message. */
    int m_reqTagChecksum;	    /* Calculated tag block checksum from last message. */
    UINT32 m_lastMsgWarnTime;	    /* Last time of unexpected message warning. */

    /*
     * Private functions.
     */
    SPxErrorCode checkPart(int& part) const;

}; /* SPxNMEADecoder */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_NMEA_DECODER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
