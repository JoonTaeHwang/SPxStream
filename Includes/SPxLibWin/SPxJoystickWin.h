/*********************************************************************
 *
 * (c) Copyright 2013 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   A reusable joystick control.
 *
 * Revision Control:
 *  07/10/21 v1.17   AGC	Workaround window size issue when used as popup.
 *
 * Previous Changes:
 *  28/11/18 1.16   SP 	Add SetKnobDblClickEnabled().
 *  02/10/18 1.15   AGC	Support double click knob handler.
 *  13/08/18 1.14   AGC	Support absolute focus control.
 *  28/11/17 1.13   AGC	Relayout on DPI change.
 *  29/06/17 1.12   AGC	Add SetFont().
 *  19/10/16 1.11   AGC	Add button support.
 *			Improve control positioning.
 *  13/10/16 1.10   AGC	Improve resizing.
 *		 	Support disabling zoom/focus controls.
 *  06/05/16 1.9    SP 	Support multi-language.
 *  03/03/16 1.8    AGC	Fix clang warnings.
 *  17/07/15 1.7    AGC Support non-linear focus control.
 *  13/01/15 1.6    SP  Tweak formatting.
 *                      Add SetDisabledARGB();
 *  01/09/14 1.5    AGC VS2015 format string changes.
 *  17/02/14 1.4    AGC Forward declare Gdiplus classes.
 *  15/10/13 1.3    SP  Add SetPosition(), etc.
 *  23/08/13 1.2    SP  Add SetYPositionEnabled().
 *                      Optimise for touch screen.
 *  14/08/13 1.1    SP  Initial version.
 *
 **********************************************************************/

#pragma once
#include "afxcmn.h"

/* Standard Windows and GDI+ headers */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* Library headers. */
#include "SPxResource.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibWin/SPxBitmapWin.h"

/* Forward declare GDI+ classes. */
namespace Gdiplus
{
    class Graphics;
    class FontFamily;
    class Font;
    class Brush;
}

/* SPxJoystickWin dialog */
class SPxJoystickWin : public CDialog, public SPxObj
{
    DECLARE_DYNAMIC(SPxJoystickWin)

public:

    /*
     * Public types.
     */

    /* Auxiliary control position. */
    enum AuxPos_t
    {
        AUX_POS_BOTTOM,
        AUX_POS_RIGHT
    };

    /* Type of focus control. */
    enum FocusControl_t
    {
        FOCUS_CTRL_REL = 0,
        FOCUS_CTRL_MIX = 1,
        FOCUS_CTRL_ABS = 2
    };

    typedef void (*Handler_t)(void *userArg);

    typedef void (*ButtonHandler_t)(unsigned int index, void *userArg);

    typedef void (*KnobDblClickHandler_t)(void *userArg);

    /*
     * Public variables.
     */

    /* Dialog Data. */
    enum { IDD = IDD_SPX_JOYSTICK_DIALOG };

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxJoystickWin(BOOL touchMode=FALSE);
    virtual ~SPxJoystickWin(void);
    virtual BOOL Create(CWnd* pParent);

    /* Configuration. */
    void SetAuxPosition(AuxPos_t pos) { m_auxPos = pos; }
    void SetBackgroundFillARGB(UINT32 argb) { m_backgroundFillARGB = argb; }
    void SetControlFillARGB(UINT32 argb) { m_controlFillARGB = argb; }
    void SetControlOutlineARGB(UINT32 argb) { m_controlOutlineARGB = argb; }
    void SetMarkerARGB(UINT32 argb) { m_markerARGB = argb; }
    void SetKnobFillARGB(UINT32 argb) { m_knobFillARGB = argb; }
    void SetKnobOutlineARGB(UINT32 argb) { m_knobOutlineARGB = argb; }
    void SetLabelARGB(UINT32 argb) { m_labelARGB = argb; }
    void SetDisabledARGB(UINT32 argb) { m_disabledARGB = argb; }
    void SetXBorderPixels(int pixels);
    void SetYBorderPixels(int pixels);
    void SetXPositionEnabled(int state);
    int IsXPositionEnabled(void) const;
    void SetYPositionEnabled(int state);
    int IsYPositionEnabled(void) const;
    int IsPositionCtrlOperating(void) const;
    void SetZoomEnabled(int state);
    int IsZoomEnabled(void) const;
    void SetZoomVisible(int state);
    int IsZoomCtrlOperating(void) const;
    void SetZoomLabel(const char *label);
    void SetFocusEnabled(int state);
    int IsFocusEnabled(void) const;
    void SetAbsFocusEnabled(int state);
    int IsAbsFocusEnabled(void) const;
    void SetFocusControl(FocusControl_t focusControl);
    FocusControl_t GetFocusControl(void) const;
    void SetFocusVisible(int state);
    int IsFocusCtrlOperating(void) const;
    void SetFocusLabel(const char *label);
    BOOL GetPosition(double *xRtn, double *yRtn);
    void SetPosition(double x, double y);
    double GetZoom(void);
    void SetZoom(double val);
    double GetFocus(void);
    void SetFocus(double val);
    int GetFocusAbs(void);
    void SetHandler(Handler_t fn, void *userArg);
    void SetFocusNonLinearity(double val);
    void SetFont(const char *name, float size);

    void SetButtonEnabled(unsigned int index, int state);
    int IsButtonEnabled(unsigned int index) const;
    void SetButtonHandler(ButtonHandler_t fn, void *userArg);

    void SetKnobDblClickEnabled(int state);
    int IsKnobDblClickEnabled(void) const;
    void SetKnobDblClickHandler(KnobDblClickHandler_t fn, void *userArg);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    virtual LRESULT OnQuerySystemGestureStatus(WPARAM, LPARAM);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS *);
    afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
    afx_msg void OnDpiChanged(WPARAM wParam, LPARAM lParam);

private:

    /*
     * Private variables.
     */
    
    SPxBitmapWin *m_pDIB;   /* Off-screen DIB. */
    CBitmap *m_pBitmap;     /* Off-screen bitmap. */
    CDC *m_pBitmapDC;       /* Bitmap drawing context. */
    Gdiplus::Graphics *m_pGfx;       /* Graphics rendering object. */
    CRect m_winRect;        /* Window size. */
    int m_winXBorder;       /* Window X border size. */
    int m_winYBorder;       /* Window Y border size. */
    BOOL m_isMouseTracking; /* Mouse tracking control. */
    CPoint m_mouseAnchor;   /* Mouse anchor on a control. */
    BOOL m_isMouseAnchorSet; /* TRUE when anchor has been set. */
    CPoint m_mousePos;
    Gdiplus::FontFamily *m_fontFamily;  /* Font family. */
    Gdiplus::Font *m_font;  /* Font to use. */
    float m_fontSize;       /* Current font size. */
    AuxPos_t m_auxPos;      /* Position of zoom and focus controls. */
    BOOL m_touchMode;       /* TRUE if optimised for touch screen. */
    unsigned int m_textHeight; /* Text height in pixels. */

    /* Position control. */
    int m_posCtrlRadius;           /* Total radius of the control. */
    int m_posCtrlKnobRadius;       /* Radius of position control knob. */
    CPoint m_posCtrlCentre;        /* Control centre. */
    BOOL m_isPosCtrlOperating;     /* Adjustment in progress. */
    CPoint m_posCtrlOffset;        /* Offset from control knob centre. */
    int m_posCtrlMaxOffset;        /* Max offset from control knob centre. */
    CPoint m_posCtrlLastOffset;    /* Last offset from control knob centre. */
    BOOL m_isPosCtrlTurboSpeed;    /* Enable / disable turbo speed. */
    int m_isXPosEnabled;           /* Is X position control enabled. */
    int m_isYPosEnabled;           /* Is Y position control enabled. */
    int m_isPosLocked;             /* Is position set externally? */

    /* Zoom control. */
    int m_zmCtrlWidth;             /* Control width. */
    int m_zmCtrlHeight;            /* Control height. */
    int m_zmCtrlKnobRadius;        /* Radius of position control knob. */
    CPoint m_zmCtrlCentre;         /* Control centre. */
    BOOL m_isZmCtrlOperating;      /* Adjustment in progress. */
    int m_zmCtrlOffset;            /* Offset from control knob centre. */
    int m_zmCtrlMaxOffset;         /* Max offset from control knob centre. */
    int m_zmCtrlLastOffset;        /* Last offset from control knob centre. */
    int m_isZoomEnabled;           /* Is zoom control enabled? */
    int m_isZoomVisible;           /* Is zoom control visible? */
    int m_isZoomLocked;            /* Is zoom set externally? */
    CStringA m_zoomLabel;          /* Label in current language. */

    /* Focus control. */
    FocusControl_t m_focCtrlType;  /* Type of focus control. */
    int m_focCtrlWidth;            /* Control width. */
    int m_focCtrlHeight;           /* Control height. */
    int m_focCtrlKnobRadius;       /* Radius of position control knob. */
    CPoint m_focCtrlCentre;        /* Control centre. */
    BOOL m_isFocCtrlOperating;     /* Adjustment in progress. */
    int m_focCtrlOffset;           /* Offset from control knob centre. */
    int m_focCtrlMaxOffset;        /* Max offset from control knob centre. */
    int m_focCtrlLastOffset;       /* Last offset from control knob centre. */
    int m_focCtrlAbsOffset;        /* Offset when applying absolute focus. */
    int m_isFocusEnabled;          /* Is focus control enabled? */
    int m_isAbsFocusEnabled;       /* Is absolute focus control enabled? */
    int m_isFocusVisible;          /* Is focus control visible? */
    int m_isFocusLocked;           /* Is focus set externally? */
    double m_focNonLinearity;      /* Level of non-linearity for focus. */
    CStringA m_focusLabel;         /* Label in current language. */

    /* Buttons. */
    static const unsigned int MAX_BUTTONS = 10;
    int m_isButtonEnabled[MAX_BUTTONS];	/* Are buttons enabled? */
    int m_isButtonDown[MAX_BUTTONS]; /* Are buttons pressed? */
    int m_btnWidth;
    int m_btnHeight;
    CPoint m_btn0TL;
    ButtonHandler_t m_btnFn;
    void *m_btnUserArg;

    /* Knob double click. */
    KnobDblClickHandler_t m_knobDblClickFn;
    void *m_knobDblClickUserArg;
    int m_isKnobDblClickEnabled;

    /* Colours. */
    UINT32 m_backgroundFillARGB;   /* Dialog background brush colour. */
    UINT32 m_controlFillARGB;      /* Control background colour. */
    UINT32 m_controlOutlineARGB;   /* Control outline colour. */
    UINT32 m_markerARGB;           /* Marker (arrow) colour. */
    UINT32 m_knobFillARGB;         /* Control knob fill colour. */
    UINT32 m_knobOutlineARGB;      /* Control knob outline colour. */
    UINT32 m_labelARGB;            /* Label colour. */
    UINT32 m_disabledARGB;         /* Disabled colour. */

    /* Callback. */
    Handler_t m_fn;
    void *m_userArg;

    /*
     * Private functions.
     */

    void ResetControls(void);
    void RequestRedraw(void);
    void Resize(void);
    void Redraw(void);
    void DrawPositionCtrl(void);
    void DrawZoomCtrl(void);
    void DrawFocusCtrl(void);
    void DrawButtons(void);
    double GetPositionNormFactor(double angleDegs);
    int Round(double val);
    double GetRangeFromXY(double x, double y);
    double GetAzimuthFromXY(double x, double y);
    double GetXFromRA(double r, double a);
    double GetYFromRA(double r, double a);
    void DrawText(CStringA *pText,
                  Gdiplus::Font *pFont, Gdiplus::Brush *pBrush,
                  int x, int y);
    unsigned int GetTextWidth(CStringA *pText, Gdiplus::Font *pFont);
    unsigned int GetTextHeight(CStringA *pText, Gdiplus::Font *pFont);
    CRect GetButtonRect(unsigned int i);

    using CDialog::Create;

    /*
     * Private static functions.
     */

}; /* Class SPxJoystickWin. */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
