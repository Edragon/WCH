

#include "stdafx.h"
#include "ScrollerCtrl.h"

// command messages:
// sent when text has scrolled completely off the window
const int   CScrollerCtrl::SC_SCROLL_COMPLETE = 0;

// defaults
const int   CScrollerCtrl::nSCROLL_DELAY  = 80;    // time between each frame (milliseconds)
const int   CScrollerCtrl::nSCROLL_PAUSE  = 5000;  // time to pause before autoscrolling (milliseconds)
const int   CScrollerCtrl::nMARGIN        = 5;     // (pixels)
const int   CScrollerCtrl::nFONT_SIZE     = 12;    // (points)
const int   CScrollerCtrl::nFONT_WEIGHT   = FW_SEMIBOLD;
const char* CScrollerCtrl::szFONT_NAME    = "Comic Sans MS";

// initialization
CScrollerCtrl::CScrollerCtrl()
{
	m_crBackground   = RGB(0,0,0);
	m_crForeground   = RGB(255,255,255);
   m_pbmpPattern     = NULL;
	m_pbmpLogo        = NULL;
	m_nContentHeight  = 0;
	m_nScrollOffset   = 0;
   m_unTimerPause    = 2;
   m_nScrollDelay    = nSCROLL_DELAY;
   m_nScrollPause    = nSCROLL_PAUSE;
   m_eState          = PAUSED;
   m_bTilePattern    = TRUE;
   m_bShowScroll     = FALSE;
   m_bWrap           = TRUE;
	m_sizeBuffer      = CSize(0,0);
}

// create the window: 
//    remove WS_VSCROLL to avoid showing scrollbar.
//    remove WS_TABSTOP to disable keyboard scrolling.
BOOL CScrollerCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT uStyle, UINT nID)
{
   if ( NULL == m_font.GetSafeHandle() )
      SetFont(szFONT_NAME, nFONT_SIZE, nFONT_WEIGHT);

   // remember if user specified the style, but don't show initially
   m_bShowScroll = uStyle&WS_VSCROLL;
   uStyle &= ~WS_VSCROLL;

	if ( CWnd::Create(::AfxRegisterWndClass(CS_HREDRAW|CS_PARENTDC|CS_VREDRAW,::LoadCursor(NULL,IDC_ARROW)), "Scroller", uStyle, rect, pParentWnd, nID) )
	{
      m_eState = PAUSED;
      SetTimer(1, m_nScrollDelay, NULL);
		m_unTimerPause = SetTimer(m_unTimerPause, m_nScrollPause, NULL);
		return TRUE;
	}
	return FALSE;
}

// activate/deactivate wrapping mode:
// if not set, content is scrolled completely off screen
// before being repeated.
void CScrollerCtrl::SetWrapping(BOOL bWrap)
{
   m_bWrap = bWrap;
   if ( NULL != m_hWnd )
      Invalidate(FALSE);
}

// Sets the color used for the background (if no pattern is set) 
// or margins (if pattern is set and not tiled)
void CScrollerCtrl::SetBgColor(COLORREF clrBg)
{
   m_crBackground = clrBg;
   if ( NULL != m_hWnd )
      Invalidate(FALSE);
}

// Sets the color used for text
void CScrollerCtrl::SetFgColor(COLORREF clrBg)
{
   m_crForeground = clrBg;
   if ( NULL != m_hWnd )
      Invalidate(FALSE);
}

// Sets the font; size is in points, see LOGFONT documentation for weight constants
void CScrollerCtrl::SetFont(const CString& strName, int nSize, int nWeight)
{
   if ( NULL != m_font.GetSafeHandle() )
      m_font.DeleteObject();

   LOGFONT logFont;
   memset(&logFont,0,sizeof(logFont));
   strncpy(logFont.lfFaceName,strName,LF_FACESIZE);
   logFont.lfPitchAndFamily = FF_SWISS;
   logFont.lfQuality = ANTIALIASED_QUALITY;
   logFont.lfWeight = nWeight;
   logFont.lfHeight = nSize*10;

   // actually create the font; if for some reason this fails, use a default
   if ( !m_font.CreatePointFontIndirect(&logFont) )
      m_font.CreateStockObject(SYSTEM_FONT);
}

// Sets the text to be displayed
void CScrollerCtrl::SetText(const CString& strText)
{
   m_strText = strText;
   if ( NULL != m_hWnd )
   {
      // force RecalcLayout()
      m_sizeBuffer = CSize(0,0);
      Invalidate(FALSE);
   }
}

// Sets the bitmap to be displayed above the text
CBitmap* CScrollerCtrl::SetLogo(CBitmap* pbmpLogo)
{
   CBitmap* pbmpOld = m_pbmpLogo;
   m_pbmpLogo = pbmpLogo;
   if ( NULL != m_hWnd )
   {
      // force RecalcLayout()
      m_sizeBuffer = CSize(0,0);
      Invalidate(FALSE);
   }
   return pbmpOld;
}

// Sets the background pattern
CBitmap* CScrollerCtrl::SetPattern(CBitmap* pbmpPattern, BOOL bTile)
{
   m_bTilePattern = bTile;
   CBitmap* pbmpOld = m_pbmpPattern;
   m_pbmpPattern = pbmpPattern;
   if ( NULL != m_hWnd )
      Invalidate(FALSE);
   return pbmpOld;
}

// Sets the time (in milliseconds) between frames (autoscrolling speed) 
// (will revert to default if less than 0)
void CScrollerCtrl::SetScrollDelay(int nScrollDelay)
{
   m_nScrollDelay = nScrollDelay;
   if ( m_nScrollDelay < 0 )
      m_nScrollDelay = nSCROLL_DELAY;
   if ( NULL != m_hWnd )
      SetTimer(1, m_nScrollDelay, NULL);
}

// Sets the delay (in milliseconds) when autoscrolling pauses 
// (will disable pausing if set less than scroll delay)
void CScrollerCtrl::SetScrollPause(int nScrollPause)
{
   m_nScrollPause = nScrollPause;
}

BEGIN_MESSAGE_MAP(CScrollerCtrl, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
   ON_WM_VSCROLL()
   ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CScrollerCtrl message handlers

// entire window is updated in OnPaint() so do nothing here.
BOOL CScrollerCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

// resize buffer first if necessary, then compose onto buffer,
// wrapping if specified, finally update the screen.
void CScrollerCtrl::OnPaint()
{
	CPaintDC dc(this);
	CDC dcBackBuffer;
	dcBackBuffer.CreateCompatibleDC(&dc);

   // resize buffer if neccessary, calculate content size.
   RecalcLayout(&dc);

	CBitmap* pOldBmp = dcBackBuffer.SelectObject(&m_bmpBackBuffer);

   FillBackground(&dcBackBuffer);
   int nOffset = nMARGIN + m_nScrollOffset;
   do
   {
      nOffset += DrawLogo(&dcBackBuffer, nOffset) + nMARGIN;
	   nOffset += DrawBodyText(&dcBackBuffer, nOffset) + nMARGIN*2;
   } while ( nOffset < m_sizeBuffer.cy && m_bWrap );

	dc.BitBlt(0,0,m_sizeBuffer.cx,m_sizeBuffer.cy,&dcBackBuffer,0,0,SRCCOPY);

	// cleanup
	dcBackBuffer.SelectObject(pOldBmp);
}

// if buffer size does not match window size, resize buffer.
// Calculate content size.
void CScrollerCtrl::RecalcLayout(CDC* pDC)
{
	CRect rectClient;
	GetClientRect(&rectClient);
	if ( m_sizeBuffer != rectClient.Size() )
	{
		m_sizeBuffer = rectClient.Size();
		if ( m_bmpBackBuffer.GetSafeHandle() != NULL )
			m_bmpBackBuffer.DeleteObject();
		m_bmpBackBuffer.CreateCompatibleBitmap(pDC, m_sizeBuffer.cx, m_sizeBuffer.cy);

      m_nContentHeight = nMARGIN*3;
      m_nContentHeight += DrawLogo(pDC, 0, FALSE);
      m_nContentHeight += DrawBodyText(pDC, 0, FALSE);
	}
}

// Draw the background; uses background color unless a pattern
// bitmap is set, in which case that will be tiled or centered.
void CScrollerCtrl::FillBackground(CDC* pDC)
{
   CRect rectClient;
   GetClientRect(&rectClient);
   if ( NULL == m_pbmpPattern )
   {
	   pDC->FillSolidRect(rectClient, m_crBackground);
   }
   else
   {
      CDC dcPat;
      dcPat.CreateCompatibleDC(pDC);
      CBitmap* pbmpOld = dcPat.SelectObject(m_pbmpPattern);
      BITMAP bitmap;
      if ( m_pbmpPattern->GetBitmap(&bitmap) && bitmap.bmWidth > 0 && bitmap.bmHeight > 0 )
      {
         if ( m_bTilePattern )
         {
            for (int y=0; y<rectClient.bottom+bitmap.bmHeight; y += bitmap.bmHeight)
            {
               for (int x=0; x<rectClient.right+bitmap.bmWidth; x += bitmap.bmWidth)
               {
                  pDC->BitBlt(x,y, bitmap.bmWidth, bitmap.bmHeight, &dcPat, 0,0, SRCCOPY);
               }
            }
         }
         else
         {
      	   pDC->FillSolidRect(rectClient, m_crBackground);
            pDC->BitBlt((m_sizeBuffer.cx-bitmap.bmWidth)/2,(m_sizeBuffer.cy-bitmap.bmHeight)/2, bitmap.bmWidth, bitmap.bmHeight, &dcPat, 0,0, SRCCOPY);
         }
      }
      dcPat.SelectObject(pbmpOld);
   }
}

// Draws the logo (if specified) at the given offset.
// If bDraw is false, calculates height, but does not draw.
// Returns height of logo.
int CScrollerCtrl::DrawLogo(CDC* pDC, int nOffset, BOOL bDraw)
{
   if ( NULL == m_pbmpLogo )
      return 0;

   BITMAP bitmap;
   memset(&bitmap,0,sizeof(bitmap));
   if ( m_pbmpLogo->GetBitmap(&bitmap) && bDraw && bitmap.bmWidth > 0 && bitmap.bmHeight > 0 )
   {
      CDC dcLogo;
      dcLogo.CreateCompatibleDC(pDC);
      CBitmap* pbmpOld = dcLogo.SelectObject(m_pbmpLogo);
      pDC->BitBlt((m_sizeBuffer.cx-bitmap.bmWidth)/2,nOffset, bitmap.bmWidth, bitmap.bmHeight, &dcLogo, 0,0, SRCCOPY);
      dcLogo.SelectObject(pbmpOld);
   }
   return bitmap.bmHeight;
}

// Draws the text at the specified offset.
// If bDraw is false, will calculate text height, but not draw.
// Returns height of text.
int CScrollerCtrl::DrawBodyText(CDC* pDC, int nOffset, BOOL bDraw)
{
	CRect rect(nMARGIN,nOffset,m_sizeBuffer.cx-nMARGIN,m_sizeBuffer.cy);
	UINT uFlags = bDraw ? DT_EXPANDTABS|DT_NOPREFIX|DT_WORDBREAK : DT_EXPANDTABS|DT_NOCLIP|DT_CALCRECT|DT_NOPREFIX|DT_WORDBREAK;

   CFont* pOldFont = pDC->SelectObject(&m_font);
   pDC->SetBkMode(TRANSPARENT);
   // draw shadow if displayed over pattern
   if ( bDraw && NULL != m_pbmpPattern )
   {
      // offset 1/10 of font size
      LOGFONT logFont;
      m_font.GetLogFont(&logFont);
      int nShadowOffset = MulDiv(-logFont.lfHeight, 72, pDC->GetDeviceCaps(LOGPIXELSY)*10);
      // get color between forground and background for shadow (not correct i'm sure)
      int red = (GetRValue(m_crForeground) + GetRValue(m_crBackground)*2)/3;
      int green = (GetGValue(m_crForeground) + GetGValue(m_crBackground)*2)/3;
      int blue = (GetBValue(m_crForeground) + GetBValue(m_crBackground)*2)/3;
      COLORREF crDarker = RGB(red, green, blue);

      pDC->SetTextColor(crDarker);
      rect.OffsetRect(nShadowOffset,nShadowOffset);
      pDC->DrawText(m_strText, &rect, uFlags);
      rect.OffsetRect(-nShadowOffset,-nShadowOffset);
   }
   if(m_eState == PAUSED)
   pDC->SetTextColor(m_crForeground);
   else
   {
	   pDC->SetTextColor(RGB((int)160,0,0));
   }
	int nHeight = pDC->DrawText(m_strText, &rect, uFlags);
   pDC->SelectObject(pOldFont);
	return nHeight;
}

// indicate that this control will process arrow keystrokes.
UINT CScrollerCtrl::OnGetDlgCode()
{
   if ( GetStyle()&WS_TABSTOP )
      return DLGC_WANTARROWS;
   return 0;
}

// Grab focus if required.
void CScrollerCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
   if ( GetStyle()&WS_TABSTOP )
      SetFocus();
}

// Handle keyboard scrolling.
void CScrollerCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
   switch ( nChar )
   {
   case VK_UP:
      PostMessage(WM_VSCROLL, SB_LINEUP);
      break;
   case VK_DOWN:
      PostMessage(WM_VSCROLL, SB_LINEDOWN);
      break;
   case VK_PRIOR:    // why not VK_PAGEUP?
      PostMessage(WM_VSCROLL, SB_PAGEUP);
      break;
   case VK_NEXT:     // why not VK_PAGEDOWN?
      PostMessage(WM_VSCROLL, SB_PAGEDOWN);
      break;
   }
}

// Handle scrolling.
// This can be triggered by mouse wheel and keyboard, 
// as well as the scrollbar.
void CScrollerCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
   if ( !(GetStyle()&WS_TABSTOP) )
      return;

   // delay autoscroll after manual scroll
   if ( m_nScrollPause > m_nScrollDelay )
   {
      m_eState = PAUSED;
      m_unTimerPause = SetTimer(m_unTimerPause, m_nScrollPause, NULL);
   }

   switch (nSBCode)
   {
   case SB_BOTTOM:      // Scroll to bottom. 
      m_nScrollOffset = m_sizeBuffer.cy-m_nContentHeight;
      break;
   case SB_TOP:         // Scroll to top. 
      m_nScrollOffset = 0;
      break;
   case SB_LINEDOWN:    // Scroll one line down. 
      m_nScrollOffset -= 8;
      break;
   case SB_LINEUP:      // Scroll one line up. 
      m_nScrollOffset += 8;
      break;
   case SB_PAGEDOWN:    // Scroll one page down. 
      m_nScrollOffset -= m_sizeBuffer.cy;
      break;
   case SB_PAGEUP:      // Scroll one page up. 
      m_nScrollOffset += m_sizeBuffer.cy;
      break;
   case SB_THUMBPOSITION:
   case SB_THUMBTRACK:
      m_nScrollOffset = -((int)nPos);
      break;
   default:
      return;
   }

   // constrain
	if ( m_nScrollOffset < m_sizeBuffer.cy-m_nContentHeight )
		m_nScrollOffset = m_sizeBuffer.cy-m_nContentHeight;
   else if ( m_nScrollOffset > 0 )
      m_nScrollOffset = 0;

   // scroll
   SetScrollPos(SB_VERT, -m_nScrollOffset);
   Invalidate(FALSE);
}

// Handle mouse wheel scrolling.
// I'll put actual effort into handling non-WHEEL_DELTA increments
// when i actually get a mouse that sends them. ;~)
BOOL CScrollerCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
   int nDist = abs(zDelta)/WHEEL_DELTA+1;

   while ( nDist-- )
   {
      PostMessage(WM_VSCROLL, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN);
   }
   return TRUE;
}

// Timers are used for three purposes:
//    + Periodic checking to show/hide the scrollbar
//    + Automatic scrolling (when m_eState == SCROLLING)
//    + Ending automatic scrolling pauses (when m_eState == PAUSED)
// This is accomplished via two timers:
//    The first does double duty, handling both the scrollbar and
//    automatic scrolling; it is active for the life of the window.
//    The second is used for ending autoscroll pauses.
void CScrollerCtrl::OnTimer(UINT nIDEvent)
{
   // don't autoscroll or hide scrollbar when user is dragging the scroll button.
   if ( this == GetCapture() )
      return;

   // the scrollbar can be shown if the following are true:
   //    + The content size is larger than the window
   //    + The mouse cursor is over the window
   //    + The top-level parent containing this control is active
   if ( m_bShowScroll )
   {
	   CRect rectWindow;
	   GetWindowRect(&rectWindow);
      CPoint pntCursor;
      ::GetCursorPos(&pntCursor);
      if ( m_sizeBuffer.cy < m_nContentHeight && GetTopLevelParent() == GetActiveWindow() && rectWindow.PtInRect(pntCursor) )
      {
         SetScrollRange(SB_VERT, 0, m_nContentHeight-m_sizeBuffer.cy, FALSE);
         SetScrollPos(SB_VERT, -m_nScrollOffset, TRUE);
         ShowScrollBar(SB_VERT, TRUE);
      }
      else if ( this != GetCapture() )
      {
         ShowScrollBar(SB_VERT, FALSE);
      }
   }

   if ( nIDEvent == m_unTimerPause )
   {
      m_eState = SCROLLING;
      KillTimer(m_unTimerPause);
   }

   if ( m_eState == SCROLLING )
   {
	   --m_nScrollOffset;
	   if ( m_nContentHeight+m_nScrollOffset < 0 )
      {
         // scrolling is complete; restart
         m_nScrollOffset = m_bWrap ? 0 : m_sizeBuffer.cy;
         CWnd* pwndParent = GetParent();
         if ( NULL != pwndParent )
            pwndParent->SendMessage(WM_COMMAND, MAKELPARAM(GetDlgCtrlID(), SC_SCROLL_COMPLETE), (LPARAM)GetSafeHwnd());
      }

      // pause at top and bottom
      if ( 0 == m_nScrollOffset || (m_nScrollOffset+m_nContentHeight == m_sizeBuffer.cy && m_sizeBuffer.cy < m_nContentHeight) )
      {
         if ( m_nScrollPause > m_nScrollDelay )
         {
            m_eState = PAUSED;
            m_unTimerPause = SetTimer(m_unTimerPause, m_nScrollPause, NULL);
         }
      }
	   Invalidate(FALSE);
   }
}
