/*********************************************************************
 *
 * (c) Copyright 2017 - 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for XML utility functions.
 *
 *
 * Revision Control:
 *  07/02/20 v1.4    BTB	Add SPxXmlDoc.
 *
 * Previous Changes:
 *  18/01/19 1.3    AGC	Add SPxXmlAddDurationElement().
 *			Add SPxXmlParseDuration().
 *  05/10/18 1.2    AGC	Support filtering elements based on attribute.
 *  13/11/17 1.1    AGC	Initial Version.
 **********************************************************************/
#ifndef SPX_XML_UTILS_H_
#define	SPX_XML_UTILS_H_

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxError.h"

/* Forward declarations. */
struct mxml_node_s;

/* Deleter for MXML documents. */
struct SPxXmlDeleter
{
    void operator() (mxml_node_s *node);
};
/* Auto-deleting wrapper for MXML documents. */
typedef SPxAutoPtr<mxml_node_s, SPxXmlDeleter> SPxXmlDoc;

extern mxml_node_s *SPxXmlAddElement(mxml_node_s *top,
				     const char *name,
				     const char *nsPrefix = NULL);
extern mxml_node_s *SPxXmlAddTextElement(mxml_node_s *top,
					 const char *name,
					 const char *value,
					 const char *nsPrefix = NULL);
extern mxml_node_s *SPxXmlAddIntElement(mxml_node_s *top,
					const char *name,
					int value,
					const char *nsPrefix = NULL);
extern mxml_node_s *SPxXmlAddRealElement(mxml_node_s *top,
					 const char *name,
					 double value,
					 const char *nsPrefix = NULL);
extern mxml_node_s *SPxXmlAddDurationElement(mxml_node_s *top,
					     const char *name,
					     double durationSecs,
					     const char *nsPrefix = NULL);

extern const char *SPxXmlGetLocalName(mxml_node_s *node);
extern const char *SPxXmlGetNamespace(mxml_node_s *node);
extern const char *SPxXmlGetNamespaceFromPrefix(mxml_node_s *node,
						const char *prefix);
extern void SPxXmlGetElementText(mxml_node_s *node,
				 char *buf, unsigned int bufLen);

extern mxml_node_s *SPxXmlFindElement(mxml_node_s *node,
				      mxml_node_s *top,
				      const char *localName,
				      const char *ns = NULL,
				      const char *attr = NULL,
				      const char *nsAttr = NULL,
				      int attrTrue = FALSE);

extern int SPxXmlGetBool(mxml_node_s *node);

extern const char *SPxXmlGetAttribute(mxml_node_s *node,
				      const char *localName,
				      const char *ns = NULL);

extern SPxErrorCode SPxXmlParseDuration(mxml_node_s *node,
					double *durationSecs);

#endif /* SPX_XML_UTILS_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
