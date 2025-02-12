/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Extension of CComboBox to support selection of track class 
 *   and display associated colour.
 *
 * Revision Control:
 *   18/09/20 v1.1   AGC	Initial version.
 *
 * Previous Changes:
 *
 *********************************************************************/
#pragma once

/* Interface for track class management.
 * The track class combobx requires an object that implements this
 * interface in order to retrieve the list of track classes.
 */
class SPxTrackClassMngr
{
public:
    virtual unsigned int GetMaxNumTrackClasses(void) const = 0;
    virtual unsigned int GetTrackClassCode(unsigned int index) = 0;
    virtual const char *GetTrackClassName(unsigned int index) = 0;
    virtual const char *GetTrackClassNameFromCode(unsigned int code) = 0;
    virtual UINT32 GetTrackClassRGB(unsigned int index) = 0;
    virtual UINT32 GetTrackClassRGBFromCode(unsigned int code) = 0;
};

/* SPxTrackClassComboBox dialog */
class SPxTrackClassComboBox : public CComboBox
{
    DECLARE_DYNAMIC(SPxTrackClassComboBox)

public:
    /*
     * Public variables.
     */

    /* Dialog Data. */

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxTrackClassComboBox(SPxTrackClassMngr *mngr);
    virtual ~SPxTrackClassComboBox(void);

    /* General. */
    void Sync(void);
    void SetSelectedClass(unsigned int classCode);

protected:
    /* 
     * Protected variables. 
     */

    /* 
     * Protected functions. 
     */

    DECLARE_MESSAGE_MAP()
    virtual void PreSubclassWindow(void);

    /* General. */

    /* GUI callback functions. */
    virtual afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

private:
    /*
     * Private variables.
     */
    SPxTrackClassMngr *m_mngr;
    CStringA m_noClassLbl;

    /* General. */

    /*
     * Private functions.
     */

    /* General. */
};

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
