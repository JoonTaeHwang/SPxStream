/*********************************************************************
*
* (c) Copyright 2010 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for SPxWizard functionality.
*
*
* Revision Control:
*   27/05/21 v1.20   AGC	Add loadWizard() function for testing.
*
* Previous Changes:
*   18/05/21 1.19   AJH	Fix compile error.
*   18/05/21 1.18   AJH	Add setFromState flag to RunWizard().
*   03/07/20 1.17   AGC	Support "requires" element.
*   10/08/18 1.16   SP 	Expand support for "requires" condition.
*   06/08/18 1.15   AGC	Support instance numbers in require params.
*   03/05/18 1.14   AGC	Support require param for controls and pages.
*   13/09/17 1.13   AGC	Support require param for option.
*   31/10/16 1.12   AGC	Improve XML error reporting.
*   06/01/16 1.11   REW	Add IsLoaded().
*   04/09/15 1.10   AGC	Support groups.
*			Generate error when too many options for menu/radio.
*			Support hidden controls.
*			Support parameter value replacement in labels.
*   04/07/14 1.9    AGC	Move string header to source file.
*   06/06/14 1.8    AGC	Support arbitrary text for instance replacement.
*   21/10/13 1.7    REW	Support multiple instances of same page.
*   04/10/13 1.6    AGC	Simplify headers.
*   06/06/13 1.5    AGC	Support multiple params for buttonChanged.
*   10/01/11 1.4    AGC	Add silent option to LoadWizard().
*   16/12/10 1.3    REW	Add SPxWizardResult_t support to RunWizard().
*   02/12/10 1.2    REW	Add m_buildingPage.
*   26/11/10 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_WIZARD_H
#define _SPX_WIZARD_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxProcParam.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Debug flags. */
#define	SPX_WIZARD_DEBUG_LOAD		0x00000001	/* Loading from file */
#define	SPX_WIZARD_DEBUG_BUILD		0x00000002	/* Building pages */
#define	SPX_WIZARD_DEBUG_RUN		0x00000004	/* Running order */
#define	SPX_WIZARD_DEBUG_USER		0x00000008	/* User actions */
#define	SPX_WIZARD_DEBUG_VERBOSE	0x80000000	/* Verbose */


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Result of running an entire wizard. */
typedef enum
{
    SPX_WIZARD_RESULT_NO_CHANGES = 0,
    SPX_WIZARD_RESULT_SAVE_CHANGES = 1,
    SPX_WIZARD_RESULT_SAVE_AND_RESTART = 2
} SPxWizardResult_t;

/* Result of running a single page of the wizard. */
typedef enum
{
    SPX_WIZARD_PAGE_RESULT_ERROR = 0,
    SPX_WIZARD_PAGE_RESULT_NEXT = 1,
    SPX_WIZARD_PAGE_RESULT_PREV = 2,
    SPX_WIZARD_PAGE_RESULT_CANCEL = 3,
    SPX_WIZARD_PAGE_RESULT_FINISH = 4
} SPxWizardPageResult_t;

typedef enum
{
    SPX_WIZARD_GROUP_NORMAL = 0,
    SPX_WIZARD_GROUP_INTERLEAVE = 1

} SPxWizardGroup_t;

/*
 * Define the abstract base class.
 */
class SPxWizard :public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor. */
    SPxWizard(void);
    virtual ~SPxWizard(void);

    /* Loading. */
    SPxErrorCode LoadWizard(const char *filename, int silent=FALSE);
    int IsLoaded(void) const;

    /* Retrieval functions. */
    const char *GetTitle(void);

    /* Executing. */
    SPxErrorCode RunWizard(SPxWizardResult_t *resultPtr=NULL, int setFromState=TRUE);

    /* Debug. */
    void SetDebug(UINT32 debug)		{ m_debug = debug; }
    UINT32 GetDebug(void)		{ return(m_debug); }

    /* Generic parameter control. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /*
     * Static functions for class.
     */

protected:
    /*
     * Protected functions.
     */

    /* Function that may be used for testing. */
    SPxErrorCode loadWizard(const char *data, unsigned int dataLen,
			    const char *filename);

    /* Functions to build the wizard, which need to be overridden by
     * derived classes to actually implement the wizard functionality
     * on different platforms.
     */
    virtual SPxErrorCode startPage(const char *label,
				    unsigned int pageIdx,
				    unsigned int numPages,
				    unsigned int pageInstance,
				    unsigned int numInstances) = 0;
    virtual SPxErrorCode addNote(unsigned int indent,
				    const char *note) = 0;
    virtual SPxErrorCode addButton(unsigned int indent, void *callbackArg,
				    int isRadio,
				    const char *label,
				    int initialState) = 0;
    virtual SPxErrorCode addControl(unsigned int indent, void *callbackArg,
				    struct SPxProcParamCtrlInfo_tag *info,
				    const char *initialValue) = 0;
    virtual SPxWizardPageResult_t runPage(void) = 0;

    /* Functions for the derived classes to tell this base class what has
     * happened during the running of a wizard.
     */
    SPxErrorCode buttonChanged(void *buttonArg, int checked);
    SPxErrorCode controlChanged(void *controlArg, const char *newValue,
				int isHidden=FALSE);

private:
    /*
     * Private functions.
     */
    /* Item handling. */
    int isItemEnabled(struct mxml_node_s *page) const;
    const char *getItemText(struct mxml_node_s *item,
				const char *defaultValue) const;

    /* Attribute handling. */
    const char *getAttrText(struct mxml_node_s *item, const char *attr,
				const char *defaultValue) const;
    unsigned int getAttrUint(struct mxml_node_s *item, const char *attr,
				unsigned int defaultValue) const;
    int getAttrInt(struct mxml_node_s *item, const char *attr,
				int defaultValue) const;
    double getAttrDouble(struct mxml_node_s *item, const char *attr,
				double defaultValue) const;
    int isAttrTrueOrFalse(struct mxml_node_s *page, const char *attr,
				int defaultState=TRUE) const;
    int isItemRequired(struct mxml_node_s *item,
		       unsigned int instance,
		       unsigned int numInstances) const;

    /* Wizard building. */
    SPxErrorCode buildPage(struct mxml_node_s *page,
				unsigned int pageNum,
				unsigned int numInstances);
    SPxErrorCode buildControl(struct mxml_node_s *control,
				unsigned int indent);
    int isPageEnabled(unsigned int pageNum) const;
    int isPageInstanceEnabled(unsigned int pageNum, 
                              unsigned int pageInstance) const;
    unsigned int getPageNumInstances(unsigned int pageNum) const;
    SPxErrorCode getPageGroup(unsigned int pageNum, unsigned int *groupNum,
	SPxWizardGroup_t *groupType=NULL) const;
    SPxErrorCode getPrevPageInGroup(unsigned int pageNum, unsigned int pageInstance, unsigned int *prevPageNum) const;
    SPxErrorCode getNextPageInGroup(unsigned int pageNum, unsigned int pageInstance, unsigned int *nextPageNum) const;
    void getPrevPage(unsigned int *pageNum, unsigned int *pageInstance) const;
    void getNextPage(unsigned int *pageNum, unsigned int *pageInstance) const;
    void testForChanges(const char *paramName, const char *newValue);

    /* Error functions. */
    static void xmlErrorFn(const char *err);

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    UINT32 m_debug;				/* Debug flags */
    FILE *m_logFile;				/* Log file */

    /* Page objects. */
    unsigned int m_pageNum;			/* Current page number */
    int m_buildingPage;				/* Flag to say in-progress */
    unsigned int m_pageInstance;		/* Which instance of page? */

    /* Control information. */
    int m_userMadeChanges;			/* Status flag */


    /*
     * Static variables for class.
     */

    /*
     * Static private functions for class.
     */
}; /* SPxWizard */


/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_WIZARD_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
