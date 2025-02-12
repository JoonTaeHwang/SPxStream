/*********************************************************************
*
* (c) Copyright 2013 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSimTargetShapeCtrlPopupDlgWin.h,v $
* ID: $Id: SPxSimTargetShapeCtrlPopupDlgWin.h,v 1.5 2017/01/31 14:46:17 rew Exp $
*
* Purpose:
*	Header for the Simulated Target shape control child dialog.
*
*
* Revision Control:
*   31/01/17 v1.5    AGC	Support units configuration.
*
* Previous Changes:
*   03/03/16 1.4    AGC	Fix clang warnings.
*   16/05/14 1.3    AGC	Support use with templates or targets.
*   07/05/13 1.2    AGC	Use SPxObjPtr to guard against target deletion.
*   25/04/13 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibUtils/SPxObjPtr.h"
#include "SPxLibWin/SPxBitmapWin.h"

/* Forward declarations. */

/*
 * SPxSimTargetShapeCtrlPopupDlgWin dialog
 */
template <typename T>
class SPxSimTargetShapeCtrlPopupDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxSimTargetShapeCtrlPopupDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTargetShapeCtrlPopupDlgWin(CWnd* pParent,
				     T *obj);
    virtual ~SPxSimTargetShapeCtrlPopupDlgWin(void);
    BOOL Create(void);
    void SyncFromObj(void);
    
    void SetDisplayUnits(SPxUnitsDist_t unitsDist);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TARGET_SHAPE_CTRL_DIALOG };

    /* Configuration functions. */
    void SetObj(T *obj);
    T *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    void OnDrawItem(int nIDCtrl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point);
    void OnRButtonUp(UINT nFlags, CPoint point);
    void OnMouseMove(UINT nFlags, CPoint point);
    BOOL OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message);
    void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */

    /* The parent window */
    CWnd *m_parentWin;

    /* Display units. */
    SPxUnitsDist_t m_unitsDist;

    /* Off-screen bitmap for double buffering. */
    SPxAutoPtr<SPxBitmapWin> m_bitmap;

    /* The object we are controlling */
    SPxObjPtr<T> m_obj;

    /* Is editing enabled? */
    int m_editEnabled;

    /* Point being moved. */
    int m_movingPoint;

    LPTSTR m_cursor;

    /* Controls. */
    CStatic m_lblName;
    CButton m_checkSymmetric;
    CButton m_btnReset;
    CStatic m_shapeArea;

    /* Private functions */
    void OnBnClickedSpxSimTargetShapeCheckSymmetric(void);
    void OnBnClickedSpxSimTargetShapeBtnReset(void);
    void drawShape(void);
    int isEditAllowed(void) const;
    int isPointInShapeArea(CPoint &point) const;
    void addPoint(double x, double y);
    void removePoint(double x, double y);

    using CDialog::Create;

}; /* class SPxSimTargetShapeCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
