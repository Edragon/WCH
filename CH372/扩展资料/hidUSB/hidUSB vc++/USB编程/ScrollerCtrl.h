// CScrollerCtrl : class definition
// Copyright 2002, Joshua Heyer
//  You are free to use this code for whatever you want, provided you
// give credit where credit is due: if you use this code without substantial
// modification, please presearve this comment block.
//  I'm providing this code in the hope that it is useful to someone, as i have
// gotten much use out of other peoples code over the years.
//  If you see value in it, make some improvements, etc., i would appreciate it 
// if you sent me some feedback.
//  Have fun!
//

#ifndef _SCROLLER_CTRL_H_INCLUDED_
#define _SCROLLER_CTRL_H_INCLUDED_

class CScrollerCtrl : public CWnd
{
public:
	CScrollerCtrl();

   // public API

   // create the window; remove WS_VSCROLL to avoid showing scrollbar, remove WS_TABSTOP to disable keyboard scrolling.
	BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT uStyle = WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|WS_GROUP, UINT nID = 0);
   // activate/deactivate wrapping mode:
   void SetWrapping(BOOL bWrap);
   // Sets the color used for the background (if no pattern is set) or margins (if pattern is set and not tiled)
   void SetBgColor(COLORREF clrBg);
   // Sets the color used for text
   void SetFgColor(COLORREF clrBg);
   // Sets the font; size is in points, see LOGFONT documentation for weight constants
   void SetFont(const CString& strName, int nSize, int nWeight);
   // Sets the text to be displayed
   void SetText(const CString& strText);
   // Sets the bitmap to be displayed above the text
   CBitmap* SetLogo(CBitmap* pbmpLogo);
   // Sets the background pattern
   CBitmap* SetPattern(CBitmap* pbmpPattern, BOOL bTile);
   // Sets the time between frames (autoscrolling speed) (will revert to default if less than 0) (milliseconds)
   void SetScrollDelay(int nScrollDelay);
   // Sets the delay when autoscrolling pauses (will disable pausing if set less than scroll delay) (milliseconds)
   void SetScrollPause(int nScrollPause);

   // command messages:
   // sent when text has scrolled completely off the window
   static const int  SC_SCROLL_COMPLETE;
protected:
   // utility: override these in a derived class if you
   // want to do something fancy.
   virtual void RecalcLayout(CDC* pDC);
   virtual void FillBackground(CDC* pDC);
	virtual int DrawLogo(CDC* pDC, int nOffset, BOOL bDraw = TRUE);
	virtual int DrawBodyText(CDC* pDC, int nOffset, BOOL bDraw = TRUE);

   // message handlers
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
   afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
private:
   // defaults
   static const int   nSCROLL_DELAY;   // time between each frame (milliseconds)
   static const int   nSCROLL_PAUSE;   // time to pause before autoscrolling (milliseconds)
   static const int   nMARGIN;         // (pixels)
   static const int   nFONT_SIZE;      // (points)
   static const int   nFONT_WEIGHT;
   static const char* szFONT_NAME;

   // instance data
   enum { SCROLLING, PAUSED } m_eState;
	COLORREF m_crBackground;
	COLORREF m_crForeground;
   CFont    m_font;
   CString  m_strText;
   CBitmap* m_pbmpPattern;
	CBitmap* m_pbmpLogo;
	CBitmap  m_bmpBackBuffer;
	CSize    m_sizeBuffer;
	int      m_nContentHeight;
	int      m_nScrollOffset;
   BOOL     m_bTilePattern;
   BOOL     m_bShowScroll;
   BOOL     m_bWrap;
   int      m_nScrollDelay;
   int      m_nScrollPause;
   UINT_PTR m_unTimerPause;
};

#endif _SCROLLER_CTRL_H_INCLUDED_