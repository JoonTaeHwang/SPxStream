/*********************************************************************
*
* Purpose:
*	Header for CRC checksum calculations.
*
*	This came from "A Painless Guide to CRC Error Detection Algorithms"
*	by Ross Williams, as instructed by the P133 customer.  The paper
*	can be found at http://www.ross.net/crc if required.
*
*	This implementation was downloaded on 28/01/09 from
*	http://www.repairfaq.org/filipg/LINK/F_crc_v35.html#CRCV002
*	and is marked as "public domain" in the original file so we are
*	free to use it without license.
*
* Revision Control:
*   03/09/19 v1.4    AGC	Copied to SPxLibUtils directory.
*
* Previous Changes:
*   08/10/13 1.3    REW	Name the cm_t structure with cm_tag.
*   28/01/09 1.2    REW	Rename definition of bool.  Extern "C" it all.
*   28/01/09 1.1    REW	Downloaded version.
**********************************************************************/

/******************************************************************************/
/*                             Start of crcmodel.h                            */
/******************************************************************************/
/*                                                                            */
/* Author : Ross Williams (ross@guest.adelaide.edu.au.).                      */
/* Date   : 3 June 1993.                                                      */
/* Status : Public domain.                                                    */
/*                                                                            */
/* Description : This is the header (.h) file for the reference               */
/* implementation of the Rocksoft^tm Model CRC Algorithm. For more            */
/* information on the Rocksoft^tm Model CRC Algorithm, see the document       */
/* titled "A Painless Guide to CRC Error Detection Algorithms" by Ross        */
/* Williams (ross@guest.adelaide.edu.au.). This document is likely to be in   */
/* "ftp.adelaide.edu.au/pub/rocksoft".                                        */
/*                                                                            */
/* Note: Rocksoft is a trademark of Rocksoft Pty Ltd, Adelaide, Australia.    */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* How to Use This Package                                                    */
/* -----------------------                                                    */
/* Step 1: Declare a variable of type cm_t. Declare another variable          */
/*         (p_cm say) of type p_cm_t and initialize it to point to the first  */
/*         variable (e.g. p_cm_t p_cm = &cm_t).                               */
/*                                                                            */
/* Step 2: Assign values to the parameter fields of the structure.            */
/*         If you don't know what to assign, see the document cited earlier.  */
/*         For example:                                                       */
/*            p_cm->cm_width = 16;                                            */
/*            p_cm->cm_poly  = 0x8005L;                                       */
/*            p_cm->cm_init  = 0L;                                            */
/*            p_cm->cm_refin = TRUE;                                          */
/*            p_cm->cm_refot = TRUE;                                          */
/*            p_cm->cm_xorot = 0L;                                            */
/*         Note: Poly is specified without its top bit (18005 becomes 8005).  */
/*         Note: Width is one bit less than the raw poly width.               */
/*                                                                            */
/* Step 3: Initialize the instance with a call cm_ini(p_cm);                  */
/*                                                                            */
/* Step 4: Process zero or more message bytes by placing zero or more         */
/*         successive calls to cm_nxt. Example: cm_nxt(p_cm,ch);              */
/*                                                                            */
/* Step 5: Extract the CRC value at any time by calling crc = cm_crc(p_cm);   */
/*         If the CRC is a 16-bit value, it will be in the bottom 16 bits.    */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* Design Notes                                                               */
/* ------------                                                               */
/* PORTABILITY: This package has been coded very conservatively so that       */
/* it will run on as many machines as possible. For example, all external     */
/* identifiers have been restricted to 6 characters and all internal ones to  */
/* 8 characters. The prefix cm (for Crc Model) is used as an attempt to avoid */
/* namespace collisions. This package is endian independent.                  */
/*                                                                            */
/* EFFICIENCY: This package (and its interface) is not designed for           */
/* speed. The purpose of this package is to act as a well-defined reference   */
/* model for the specification of CRC algorithms. If you want speed, cook up  */
/* a specific table-driven implementation as described in the document cited  */
/* above. This package is designed for validation only; if you have found or  */
/* implemented a CRC algorithm and wish to describe it as a set of parameters */
/* to the Rocksoft^tm Model CRC Algorithm, your CRC algorithm implementation  */
/* should behave identically to this package under those parameters.          */
/*                                                                            */
/******************************************************************************/

/* The following #ifndef encloses this entire */
/* header file, rendering it indempotent.     */
#ifndef CM_DONE
#define CM_DONE

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

/* The following definitions are extracted from my style header file which    */
/* would be cumbersome to distribute with this package. The DONE_STYLE is the */
/* idempotence symbol used in my style header file.                           */

#ifndef DONE_STYLE

typedef unsigned long   ulong;
typedef unsigned        crc_bool;	/* "bool" is a C++ built-in type */
typedef unsigned char * p_ubyte_;

#ifndef TRUE
#define FALSE 0
#define TRUE  1
#endif

/* Change to the second definition if you don't have prototypes. */
#define P_(A) A
/* #define P_(A) () */

/* Uncomment this definition if you don't have void. */
/* typedef int void; */

#endif

/******************************************************************************/

/* CRC Model Abstract Type */
/* ----------------------- */
/* The following type stores the context of an executing instance of the  */
/* model algorithm. Most of the fields are model parameters which must be */
/* set before the first initializing call to cm_ini.                      */
typedef struct cm_tag
  {
   int   cm_width;   /* Parameter: Width in bits [8,32].       */
   ulong cm_poly;    /* Parameter: The algorithm's polynomial. */
   ulong cm_init;    /* Parameter: Initial register value.     */
   crc_bool  cm_refin;   /* Parameter: Reflect input bytes?        */
   crc_bool  cm_refot;   /* Parameter: Reflect output CRC?         */
   ulong cm_xorot;   /* Parameter: XOR this to output CRC.     */

   ulong cm_reg;     /* Context: Context during execution.     */
  } cm_t;
typedef cm_t *p_cm_t;

/******************************************************************************/

/* Functions That Implement The Model */
/* ---------------------------------- */
/* The following functions animate the cm_t abstraction. */

void cm_ini P_((p_cm_t p_cm));
/* Initializes the argument CRC model instance.          */
/* All parameter fields must be set before calling this. */

void cm_nxt P_((p_cm_t p_cm,int ch));
/* Processes a single message byte [0,255]. */

void cm_blk P_((p_cm_t p_cm,p_ubyte_ blk_adr,ulong blk_len));
/* Processes a block of message bytes. */

ulong cm_crc P_((p_cm_t p_cm));
/* Returns the CRC value for the message bytes processed so far. */

/******************************************************************************/

/* Functions For Table Calculation */
/* ------------------------------- */
/* The following function can be used to calculate a CRC lookup table.        */
/* It can also be used at run-time to create or check static tables.          */

ulong cm_tab P_((p_cm_t p_cm,int index));
/* Returns the i'th entry for the lookup table for the specified algorithm.   */
/* The function examines the fields cm_width, cm_poly, cm_refin, and the      */
/* argument table index in the range [0,255] and returns the table entry in   */
/* the bottom cm_width bytes of the return value.                             */

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/* End of the header file idempotence #ifndef */
#endif

/******************************************************************************/
/*                             End of crcmodel.h                              */
/******************************************************************************/
