/*********************************************************************
*
* (c) Copyright 2007 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for LUT.
*
* Revision Control:
*   17/03/21 v1.24   AGC	Add SPX_LUT16_TOP8D_MID4.
*				Add SPX_LUT16_TOP4_BOTTOM8D.
*				Add SPX_LUT16_TOP8D_BOTTOM8D.
*
* Previous Changes:
*   24/04/19 1.23   AGC	Add SPX_LUT16_BIT...
*			Add SPX_LUT16_MID...
*			Add SPX_LUT16_BOTTOM...
*   17/05/18 1.22   AGC	Add output packing type option.
*   02/05/18 1.21   REW	Add SPX_LUT_DIG8 and arg5, arg6, arg7, arg8.
*   07/01/16 1.20   AGC	Add SPX_LUT16_TOP4_MID4.
*   17/02/14 1.19   AGC	Use const for SetFromFile() parameter.
*   14/11/13 1.18   REW	Add arg3, arg4 and SPX_LUT16_TOP8_MID7_BIT0 etc.
*   08/03/13 1.17   AGC	Add required SPx headers.
*   14/11/11 1.16   SP 	Add SPX_LUT12_LINEAR, SPX_LUT16_LINEAR
*                       and SPX_LUT24_LINEAR. Set as default LUTs.
*   25/10/11 1.15   SP 	Add some built-in 16-bit LUTs.
*                       Add GetTableSizeBytes().
*                       Add GetSampleSizeBytes().
*                       Drop numBits arg from look-up functions.
*   27/06/11 1.14   AGC	Add SPxLUT24.
*   24/01/11 1.13   REW	Add SPX_LUT_GAIN_OFFSET.
*   17/12/10 1.12   AGC	Derive SPxLUT from SPxObj.
*			Add SetParameter()/GetParameter().
*			Use SPxPath for storing filename.
*   26/11/10 1.11   SP 	Add extended LUT info.
*   15/09/10 1.10   REW	Make destructor virtual.
*   23/06/10 1.9    REW	Add SPxLUT16.
*   18/06/10 1.8    REW	Add SPX_LUT12_SSI_DETECT4.
*   18/02/10 1.7    SP 	Add BOTTOMn and TOP4_BOTTOM4 predefined LUTs.
*   23/01/09 1.6    REW	Add BITn and TOPn predefined LUTs.
*   19/09/08 1.5    REW	Fix syntax of SetFromFile() in last change.
*   19/09/08 1.4    DGJ	Add extra LUT modes for 8-bit and SetFromFile()
*   17/01/08 1.3    REW	Avoid icc compiler warnings.
*   09/11/07 1.2    REW	Add SPX_LUT_OFFSET to SPxLUTtype.
*   20/09/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_LUT_H_
#define	_SPX_LUT_H_

/* Required headers. */
#include "SPxLibUtils/SPxObj.h" /* For SPxLUT base class. */
#include "SPxLibUtils/SPxSysUtils.h" /* For SPxPath. */

/* Predefined LUT types. 
 * 
 * KEEP LUT INFO TABLES IN SYNC WITH THIS LIST (see SPxLUT.cpp)
 */
typedef enum
{
    SPX_LUT_LINEAR = 0,         /* 8-bit linear */
    SPX_LUT_NULL = 1,
    SPX_LUT_THRESHOLD = 2,
    SPX_LUT_GAIN = 3,
    SPX_LUT_CONSTANT = 4,
    SPX_LUT_OFFSET = 5,
    SPX_LUT_CLIP = 6,
    SPX_LUT_MAP7 = 7,
    SPX_LUT_MAP6 = 8,
    SPX_LUT_MAP5 = 9,
    SPX_LUT_MAP4 = 10,
    SPX_LUT_GAIN_OFFSET = 11,
    SPX_LUT12_LINEAR = 19,      /* 12-bit linear */
    SPX_LUT12_SSI_DETECT1 = 20,
    SPX_LUT12_SSI_DETECT2 = 21,
    SPX_LUT12_SSI_DETECT3 = 22,
    SPX_LUT12_SSI_DETECT4 = 23,
    SPX_LUT_BIT0 = 30,		/* Just bit 0, mapped to 0x00 or 0xFF */
    SPX_LUT_BIT1 = 31,		/* Just bit 1, mapped to 0x00 or 0xFF */
    SPX_LUT_BIT2 = 32,		/* Just bit 2, mapped to 0x00 or 0xFF */
    SPX_LUT_BIT3 = 33,		/* Just bit 3, mapped to 0x00 or 0xFF */
    SPX_LUT_BIT4 = 34,		/* Just bit 4, mapped to 0x00 or 0xFF */
    SPX_LUT_BIT5 = 35,		/* Just bit 5, mapped to 0x00 or 0xFF */
    SPX_LUT_BIT6 = 36,		/* Just bit 6, mapped to 0x00 or 0xFF */
    SPX_LUT_BIT7 = 37,		/* Just bit 7, mapped to 0x00 or 0xFF */
    SPX_LUT_TOP7 = 40,		/* Bits 7..1, mapped to 0x00 to 0xFF */
    SPX_LUT_TOP6 = 41,		/* Bits 7..2, mapped to 0x00 to 0xFF */
    SPX_LUT_TOP5 = 42,		/* Bits 7..3, mapped to 0x00 to 0xFF */
    SPX_LUT_TOP4 = 43,		/* Bits 7..4, mapped to 0x00 to 0xFF */
    SPX_LUT_TOP3 = 44,		/* Bits 7..5, mapped to 0x00 to 0xFF */
    SPX_LUT_TOP2 = 45,		/* Bits 7..6, mapped to 0x00 to 0xFF */
    SPX_LUT_TOP1 = 46,		/* Bit 7 only, mapped to 0x00 to 0xFF */
    SPX_LUT_TOP7_BIT0 = 50,	/* TOP7, +arg1 (or 0xff if 0) if bit 0 set */
    SPX_LUT_TOP6_BIT01 = 51,	/* TOP6, +arg1 if bit 0 set, +arg2 if bit 1 */
    SPX_LUT_TOP5_BIT012 = 52,	/* TOP5, +arg1 for bits 0/1, +arg2 bit 2 */
    SPX_LUT_TOP4_BIT0123 = 53,	/* TOP4, +arg1 for bits 0/1, +arg2 bits 2/3 */
    SPX_LUT_BOTTOM7 = 60,       /* Bits 6..0, mapped to 0x00 to 0xFF */
    SPX_LUT_BOTTOM6 = 61,       /* Bits 5..0, mapped to 0x00 to 0xFF */
    SPX_LUT_BOTTOM5 = 62,       /* Bits 4..0, mapped to 0x00 to 0xFF */
    SPX_LUT_BOTTOM4 = 63,       /* Bits 3..0, mapped to 0x00 to 0xFF */
    SPX_LUT_BOTTOM3 = 64,       /* Bits 2..0, mapped to 0x00 to 0xFF */
    SPX_LUT_BOTTOM2 = 65,       /* Bits 1..0, mapped to 0x00 to 0xFF */
    SPX_LUT_BOTTOM1 = 66,       /* Bits 0 only, mapped to 0x00 to 0xFF */
    SPX_LUT_TOP4_BOTTOM4 = 70,  /* Bits 7..4 and 3..0 -> 0x00 to 0xFF */
    SPX_LUT_DIG8 = 80,		/* +arg1 if bit0 set, +arg2 bit1, etc. etc. */
    SPX_LUT16_LINEAR = 100,     /* 16-bit linear, same as SPX_LUT16_TOP8 */
    SPX_LUT16_TOP8 = 101,       /* Bits 15..8 mapped to 0x00 to 0xFF */
    SPX_LUT16_BOTTOM8 = 102,    /* Bits 7..0 mapped to 0x00 to 0xFF */
    SPX_LUT16_TOP8_BOTTOM8 = 103, /* Highest of Bits 15..8 * arg2 and Bits 7..0 * arg1 */
    SPX_LUT16_TOP8_MID7_BIT0 = 104, /* [15..8]*arg1 + [7..1]*arg2 + [0]*arg3 */
    SPX_LUT16_TOP8_MID6_BIT01 = 105, /* Like 104 + arg4 if bit 1 set */
    SPX_LUT16_TOP8_MID5_BIT012 = 106, /* Like 104 but + arg3 for bits 0/1, arg4 bit 2 */
    SPX_LUT16_TOP8_MID4_BIT0123 = 107, /* Like 104 but + arg3 for 0/1, arg4 for 2/3 */
    SPX_LUT16_TOP4_MID4 = 108,	/* Bits 15-12 in top 4 bits, bits 7-4 in bottom 4 bits. */
    SPX_LUT16_BIT0 = 110,	/* Just bit 0, mapped to 0x00 or 0xFF */
    SPX_LUT16_BIT1 = 111,	/* Just bit 1, mapped to 0x00 or 0xFF */
    SPX_LUT16_BIT2 = 112,	/* Just bit 2, mapped to 0x00 or 0xFF */
    SPX_LUT16_BIT3 = 113,	/* Just bit 3, mapped to 0x00 or 0xFF */
    SPX_LUT16_BIT4 = 114,	/* Just bit 4, mapped to 0x00 or 0xFF */
    SPX_LUT16_BIT5 = 115,	/* Just bit 5, mapped to 0x00 or 0xFF */
    SPX_LUT16_BIT6 = 116,	/* Just bit 6, mapped to 0x00 or 0xFF */
    SPX_LUT16_BIT7 = 117,	/* Just bit 7, mapped to 0x00 or 0xFF */
    SPX_LUT16_MID7 = 120,	/* Bits 7..1, mapped to 0x00 to 0xFF */
    SPX_LUT16_MID6 = 121,	/* Bits 7..2, mapped to 0x00 to 0xFF */
    SPX_LUT16_MID5 = 122,	/* Bits 7..3, mapped to 0x00 to 0xFF */
    SPX_LUT16_MID4 = 123,	/* Bits 7..4, mapped to 0x00 to 0xFF */
    SPX_LUT16_MID3 = 124,	/* Bits 7..5, mapped to 0x00 to 0xFF */
    SPX_LUT16_MID2 = 125,	/* Bits 7..6, mapped to 0x00 to 0xFF */
    SPX_LUT16_MID1 = 126,	/* Bit 7 only, mapped to 0x00 to 0xFF */
    SPX_LUT16_BOTTOM7 = 135,	/* Bits 6..0, mapped to 0x00 to 0xFF */
    SPX_LUT16_BOTTOM6 = 136,	/* Bits 5..0, mapped to 0x00 to 0xFF */
    SPX_LUT16_BOTTOM5 = 137,	/* Bits 4..0, mapped to 0x00 to 0xFF */
    SPX_LUT16_BOTTOM4 = 138,	/* Bits 3..0, mapped to 0x00 to 0xFF */
    SPX_LUT16_BOTTOM3 = 139,	/* Bits 2..0, mapped to 0x00 to 0xFF */
    SPX_LUT16_BOTTOM2 = 140,	/* Bits 1..0, mapped to 0x00 to 0xFF */
    SPX_LUT16_BOTTOM1 = 141,	/* Bits 0 only, mapped to 0x00 to 0xFF */
    SPX_LUT16_TOP8D_MID4 = 142,	/* Bits 15-8 divided by arg1 for top 4 bits, bits 7-4 in bottom 4 bits. */
    SPX_LUT16_TOP4_BOTTOM8D = 143, /* Bits 15-12 in top 4 bits, bits 7-0 divided by arg2 for bottom 4 bits. */
    SPX_LUT16_TOP8D_BOTTOM8D = 144, /* Bits 15-8 divided by arg1 for top 4 bits,
				     * bits 7-0 divided by arg2 for bottom 4 bits.
				     */
    SPX_LUT24_LINEAR = 150,	/* 24-bit linear */

    /* Must be last entry and must be -1. DO NOT add to LUT info table. */
    SPX_LUT_USER_DEFINED = -1

} SPxLUTtype;


/* Structure used to store information on built-in LUTs */
typedef struct
{
    SPxLUTtype type;                /* LUT type */
    const char *name;               /* LUT name */
    int sampleSizeBytes;            /* Sample size in bytes */
    int tableSizeSamples;           /* Table size in samples */

} SPxLUTinfo_t;


/* Class (Abstract base) to represent a LUT. */
class SPxLUT : public SPxObj
{
private:
    unsigned char *m_lutData;
    int m_tableSizeSamples;
    int m_sampleSizeBytes;

    /* Is the table identically zero ? */
    int m_lutIsNULL;

    /* Current LUT type and args */
    SPxLUTtype m_type;
    UINT8 m_packing;
    int m_arg1;
    int m_arg2;
    int m_arg3;
    int m_arg4;
    int m_arg5;
    int m_arg6;
    int m_arg7;
    int m_arg8;
    SPxPath m_fileName;

public:
    /* Constructor - be careful about extra param between arg2 and arg3. */
    SPxLUT(int tableSizeSamples, SPxLUTtype type, 
           int arg1, int arg2, int sampleSizeBytes=1, int arg3=0, int arg4=0,
	   int arg5=0, int arg6=0, int arg7=0, int arg8=0);
    virtual ~SPxLUT(void);

    unsigned char *GetTable(void) {return m_lutData;}
    int GetTableSizeSamples(void) {return m_tableSizeSamples;}
    int GetSampleSizeBytes(void) {return m_sampleSizeBytes;}
    
    int IsNULL(void) {return m_lutIsNULL;}    
    void Set(SPxLUTtype type, int arg1=0, int arg2=0, int arg3=0, int arg4=0,
		int arg5=0, int arg6=0, int arg7=0, int arg8=0);
    void Set(unsigned char *data8);
    void Set(UINT16 *data16);
    int SetFromFile(const char *fileName);
    SPxLUTtype GetType(void) { return m_type; }
    void SetPacking(UINT8 packing) { m_packing = packing; }
    UINT8 GetPacking(void) const { return m_packing; }
    int GetArg1(void) { return m_arg1; }
    int GetArg2(void) { return m_arg2; }
    int GetArg3(void) { return m_arg3; }
    int GetArg4(void) { return m_arg4; }
    int GetArg5(void) { return m_arg5; }
    int GetArg6(void) { return m_arg6; }
    int GetArg7(void) { return m_arg7; }
    int GetArg8(void) { return m_arg8; }
    const char *GetFile(void) { return m_fileName.GetShortPath(); }
    const char *GetFullFilePath(void) { return m_fileName.GetFullPath(); }

    /* Static functions */
    static unsigned int GetNumLuts(void);
    static const SPxLUTinfo_t *GetInfoFromIndex(int index);
    static int GetIndexFromType(SPxLUTtype type);

    /* Parameter setting/getting. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);
};

/* Class to represent an 8-bit LUT */
class SPxLUT8 :public SPxLUT
{
public:
    SPxLUT8(SPxLUTtype type=SPX_LUT_LINEAR, int arg1=0, int arg2=0,
						int arg3=0, int arg4=0,
						int arg5=0, int arg6=0,
						int arg7=0, int arg8=0);
};

/* Class to represent an 12-bit LUT */
class SPxLUT12 :public SPxLUT
{
public:
    SPxLUT12(SPxLUTtype type=SPX_LUT12_LINEAR, int arg1=0, int arg2=0,
						int arg3=0, int arg4=0,
						int arg5=0, int arg6=0,
						int arg7=0, int arg8=0);
};

/* Class to represent a 16-bit LUT. */
class SPxLUT16 :public SPxLUT
{
public:
    SPxLUT16(SPxLUTtype type=SPX_LUT16_LINEAR, int arg1=0, int arg2=0,
						int arg3=0, int arg4=0,
						int arg5=0, int arg6=0,
						int arg7=0, int arg8=0);
};

/* Class to represent a 24-bit LUT. */
class SPxLUT24 :public SPxLUT
{
public:
    SPxLUT24(SPxLUTtype type=SPX_LUT24_LINEAR, int arg1=0, int arg2=0,
						int arg3=0, int arg4=0,
						int arg5=0, int arg6=0,
						int arg7=0, int arg8=0);
};

#endif /* _SPX_LUT_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
