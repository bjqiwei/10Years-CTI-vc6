// ListBoxExBuddy.cpp: implementation of the CListBoxExBuddy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "ListBoxExBuddy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                       CListBoxExBuddy class                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


               ///////////////////////////////////////////////
               //               Message map                 //
               ///////////////////////////////////////////////



BEGIN_MESSAGE_MAP(CListBoxExBuddy, CWnd)
   //{{AFX_MSG_MAP(CListBoxExBuddy)
   ON_WM_PAINT()
   ON_WM_MOUSEMOVE()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_SIZE()
   ON_WM_NCMOUSEMOVE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
   //ON_NOTIFY( TTN_NEEDTEXT, LBB_TTIP_ID, OnTooltipNeedText )
END_MESSAGE_MAP()



               ///////////////////////////////////////////////
               //           Constructor/Destructor          //
               ///////////////////////////////////////////////



///*-
// FUNCTION NAME: CListBoxExBuddy::CListBoxExBuddy()
//
// DESCRIPTION:   CListBoxExBuddy class constructor
//
// PARAMETER(S):  None.
//
// RETURN:        None
//
// NOTES:         None
//+*/
CListBoxExBuddy::CListBoxExBuddy()
{
	/*
   // Load the bitmap by using a temp file
   TCHAR *szFileName = tmpnam( NULL );
   FILE *pfBmpFile = fopen( szFileName, "wb" );
   fwrite( lbbuddy_data, sizeof(unsigned char), __bmp_size, pfBmpFile );
   fclose( pfBmpFile );

   
   m_ButtonBitmap.Attach( (HBITMAP)LoadImage( NULL,
                                              szFileName,
                                              IMAGE_BITMAP,
                                              0,
                                              0,
                                              LR_LOADFROMFILE | LR_LOADMAP3DCOLORS ) );


   unlink( szFileName );
	*/
 //m_ButtonBitmap.LoadImage( IDB_LIST_TOOL );

	m_ButtonBitmap.LoadBitmap( IDB_LIST_TOOL );

   // Init other data
   m_bButtonPressed = FALSE;
   m_iButton = __BMP_NUMBTN;
   m_hWnd = NULL;
   m_arcButtons = new CRect[ __BMP_NUMBTN ];
	m_nID =0;
   /*
#ifdef _DEBUG
   // Verify the dimensions
   BITMAP BmpInfo;
   m_ButtonBitmap.GetObject( sizeof(BITMAP), &BmpInfo );
   ASSERT( BmpInfo.bmWidth == __BMP_WIDTH );
   ASSERT( BmpInfo.bmHeight == __BMP_HEIGHT );
#endif
*/

} // CListBoxEx::CListBoxEx



///*-
// FUNCTION NAME: CListBoxExBuddy::~CListBoxExBuddy()
//
// DESCRIPTION:   CListBoxExBuddy class destructor
//
// PARAMETER(S):  None.
//
// RETURN:        None
//
// NOTES:         None
//+*/
CListBoxExBuddy::~CListBoxExBuddy()
{
   delete[] m_arcButtons;
} // CListBoxExBuddy::~CListBoxExBuddy



               ///////////////////////////////////////////////
               //             Initialization                //
               ///////////////////////////////////////////////



///*-
// FUNCTION NAME: CListBoxExBuddy::OnCreate
//
// DESCRIPTION:   Called when the window handle is created
//
// PARAMETER(S):
//                lpCreateStruct:
//                   TYPE:          CREATESTRUCT structure
//                   MODE:          In
//                   MECHANISM:     By reference
//                   DESCRIPTION:   Allows to change window features
//
// RETURN:        Inherited value.
//
// NOTES:         None
//+*/
int CListBoxExBuddy::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
   if ( CWnd::OnCreate(lpCreateStruct) == -1 )
   {
      return -1;
   }
   
   // Enable Tooltips
   CreateTooltips();

   return 0;
} // CListBoxExBuddy::OnCreate()


///*-
// FUNCTION NAME: CListBoxEx::PreSubclassWindow
//
// DESCRIPTION:   Called before the window handle is attached to a dialog item.
//
// PARAMETER(S):  None.
//
// RETURN:        Inherited value.
//
// NOTES:         None
//+*/
void CListBoxExBuddy::PreSubclassWindow() 
{
   // Enable Tooltips
   CreateTooltips();

   // Send a WM_SIZE message, as WM_CREATE would do
   CRect rcClient;
   GetClientRect( &rcClient );
   OnSize( 0, rcClient.Width(), rcClient.Height() );

   // Call default
   CWnd::PreSubclassWindow();
} // CListBoxExBuddy::PreSubclassWindow()



               ///////////////////////////////////////////////
               //                 Paint                     //
               ///////////////////////////////////////////////



///*-
// FUNCTION NAME: DrawBitmap
//
// DESCRIPTION:   Draw bitmaps helper
//
// PARAMETER(S):
//                dc
//                   TYPE:          CDC
//                   MODE:          In
//                   MECHANISM:     By reference
//                   DESCRIPTION:   Target dc
//                bmp
//                   TYPE:          CBitmap
//                   MODE:          In
//                   MECHANISM:     By reference
//                   DESCRIPTION:   bitmap object
//                x
//                   TYPE:          int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   x-pos
//                y
//                   TYPE:          int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   y-pos
//
//                cx
//                   TYPE:          int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   width
//                cy
//                   TYPE:          int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   height
//
// RETURN:        None.
//
// NOTES:         None.
//+*/
static
void DrawBitmap( CDC     & dc,
                 CBitmap & bmp,
                 int       x,
                 int       y,
                 int       cx,
                 int       cy )
{
   CDC memDC;
   memDC.CreateCompatibleDC( &dc );

   CBitmap *poldbmp = memDC.SelectObject( &bmp );
   
   dc.BitBlt( x, y, cx, cy, &memDC, 0, 0, SRCCOPY );
   
   memDC.SelectObject( poldbmp );
   memDC.DeleteDC();
} // DrawBitmap()


///*-
// FUNCTION NAME: CListBoxExBuddy::OnPaint
//
// DESCRIPTION:   Called in response to a WM_PAINT message.
//
// PARAMETER(S):  None.
//
// RETURN:        None.
//
// NOTES:         None.
//+*/
void CListBoxExBuddy::OnPaint() 
{
   CPaintDC dc( this ); // device context for painting

   // Create a compatible memory DC
   CDC memDC;
   memDC.CreateCompatibleDC( &dc );

   // Get aware of the size of the client area
   CRect rcClient;
   GetClientRect( &rcClient );

   // This is used to center the button bitmap
   int nBmpTopY = (rcClient.Height() - __BMP_HEIGHT) / 2;

   // To store old selected objects
   CBitmap *pOldBmp;
   CFont   *pOldFont;

   // Select the font
   CFont font;
   font.Attach( (HFONT)GetStockObject( DEFAULT_GUI_FONT ) );
   pOldFont = memDC.SelectObject( &font );

   // Select the out-of-screen bitmap
   CBitmap memBmp;
   memBmp.CreateCompatibleBitmap( &dc,
                                   rcClient.Width(),
                                   rcClient.Height() );
   pOldBmp = memDC.SelectObject( &memBmp );

   // Erase the background
   CBrush brush;
   brush.CreateSolidBrush( ::GetSysColor(COLOR_3DFACE) );
   memDC.FillRect( &rcClient, &brush );
   brush.DeleteObject();

   //
   // Window Text
   //

   // Prepare to draw the text transparently
   memDC.SetBkMode( TRANSPARENT );
   memDC.SetTextColor( ::GetSysColor(COLOR_WINDOWTEXT) );

   // Draw the text
   CString strWindowText;
   GetWindowText( strWindowText );
   memDC.DrawText( strWindowText, rcClient, DT_SINGLELINE | DT_VCENTER );

   //
   // Buttons
   //

   // Draw the button bitmap
   DrawBitmap( memDC,
               m_ButtonBitmap, 
               rcClient.right - __BMP_WIDTH - 2,
               nBmpTopY,
               __BMP_WIDTH,
               __BMP_HEIGHT );

   // Draw the button edge
   if ( m_iButton != __BMP_NUMBTN )
   {
      CRect rcButtonEdge( rcClient.right - (__BMP_NUMBTN - m_iButton)*__BMP_BTNWID - 2,
                          nBmpTopY,
                          rcClient.right - (__BMP_NUMBTN - m_iButton - 1)*__BMP_BTNWID - 2,
                          __BMP_HEIGHT + nBmpTopY );
      memDC.DrawEdge( &rcButtonEdge, 
                       m_bButtonPressed ? BDR_SUNKENOUTER : BDR_RAISEDINNER, 
                       BF_RECT );
   }

   //
   // Bit copy
   //

   dc.BitBlt(  2, 
               0, 
               rcClient.Width()-2, 
               rcClient.Height(), 
              &memDC, 
               0, 
               0, 
               SRCCOPY );

   //
   // Tidy up
   //
   // Select the bitmap out of the device context
   memDC.SelectObject( pOldBmp );

   // Select the font out of the device context
   memDC.SelectObject( pOldFont );

} // CListBoxExBuddy::OnPaint()



               ///////////////////////////////////////////////
               //             Mouse Management              //
               ///////////////////////////////////////////////



///*-
// FUNCTION NAME: CListBoxExBuddy::FindButton
//
// DESCRIPTION:   Finds a button given a point.
//
// PARAMETER(S):
//                point:
//                   TYPE:          CPoint class
//                   MODE:          In
//                   MECHANISM:     By const reference
//                   DESCRIPTION:   Point of action
//
// RETURN:        The index of the button, between 0 and 4 (bounds included).
//
// NOTES:         None.
//+*/
int CListBoxExBuddy::FindButton( const CPoint & point )
{
   // Find the button
   for ( UINT iIndex = 0; iIndex < __BMP_NUMBTN; iIndex++ )
   {
      if ( m_arcButtons[iIndex].PtInRect( point ) )
      {
         break;
      }
   }

   return iIndex;
} // CListBoxExBuddy::FindButton()

                                  
///*-
// FUNCTION NAME: CListBoxExBuddy::InvalidateButton
//
// DESCRIPTION:   Called to redraw a button.
//
// PARAMETER(S):
//                iIndex:
//                   TYPE:          int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Button index
//
//                bUpdateWindow:
//                   TYPE:          BOOL
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   If TRUE, calls UpdateWindow()
//
// RETURN:        None.
//
// NOTES:         None.
//+*/
void CListBoxExBuddy::InvalidateButton( int  iIndex, 
                                        BOOL bUpdateWindow /*= TRUE */ )
{
   if ( iIndex < __BMP_NUMBTN )
   {
      InvalidateRect( &m_arcButtons[ iIndex ], FALSE );
   }
   if ( bUpdateWindow ) UpdateWindow();
} // CListBoxExBuddy::InvalidateButton


///*-
// FUNCTION NAME: CListBoxExBuddy::OnMouseMove
//
// DESCRIPTION:   Called when it receives a WM_MOUSEMOVE message.
//
// PARAMETER(S):
//                nFlags:
//                   TYPE:          unsigned int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Flags
//
//                point:
//                   TYPE:          CPoint class
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Point of action
//
// RETURN:        None.
//
// NOTES:         None.
//+*/
void CListBoxExBuddy::OnMouseMove( UINT   nFlags, 
                                   CPoint point ) 
{
   if ( !m_bButtonPressed )
   {
      UINT iIndex = FindButton( point );

      // If found a button, update info
      if ( iIndex != m_iButton )
      {
         InvalidateButton( m_iButton, FALSE );
         m_iButton = iIndex;
         InvalidateButton( m_iButton, TRUE );
      }

   }

   // Releay tooltip events
   //m_ToolTip.RelayEvent( const_cast<MSG *>( GetCurrentMessage() ) );
   //m_ToolTip.Activate( TRUE );

   CWnd::OnMouseMove(nFlags, point);
} // CListBoxExBuddy::OnMouseMove


///*-
// FUNCTION NAME: CListBoxExBuddy::OnLButtonDown
//
// DESCRIPTION:   Called when it receives a WM_LBUTTONDOWN message.
//
// PARAMETER(S):
//                nFlags:
//                   TYPE:          unsigned int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Flags
//
//                point:
//                   TYPE:          CPoint class
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Point of action
//
// RETURN:        None.
//
// NOTES:         None.
//+*/
void CListBoxExBuddy::OnLButtonDown( UINT   nFlags,
                                     CPoint point )
{
   // Capture the mouse
   SetCapture();

   // Find the button
   m_iButton = FindButton( point );

   // Redraw the button
   if ( m_iButton != __BMP_NUMBTN )
   {
      m_bButtonPressed = TRUE;

      // Redraw only the affected button
      InvalidateRect( &m_arcButtons[ m_iButton ], FALSE );
      UpdateWindow();
   }

   CWnd::OnLButtonDown(nFlags, point);
} // CListBoxExBuddy::OnLButtonDown


///*-
// FUNCTION NAME: CListBoxExBuddy::OnLButtonUp
//
// DESCRIPTION:   Called when it receives a WM_LBUTTONUP message.
//
// PARAMETER(S):
//                nFlags:
//                   TYPE:          unsigned int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Flags
//
//                point:
//                   TYPE:          CPoint class
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Point of action
//
// RETURN:        None.
//
// NOTES:         None.
//+*/
void CListBoxExBuddy::OnLButtonUp( UINT   nFlags,
                                   CPoint point )
{
   // Find the button
   UINT iButton = FindButton( point );

   // Accept only clicks that occur on the same button where the mouse was pressed
   if ( iButton == m_iButton )
   {
      // Take action, if necessary
      if ( m_iButton != __BMP_NUMBTN )
      {
         DoClick( m_iButton );
      }

   }

   // Set default conditions
   m_bButtonPressed = FALSE;

   // Redraw
   Invalidate( FALSE );

   // Memorize last
   m_iButton = iButton;

   // Release mouse capture
   ReleaseCapture();

   // Call base
   CWnd::OnLButtonUp(nFlags, point);
} // CListBoxExBuddy::OnLButtonUp


// FUNCTION NAME: CListBoxExBuddy::DoClick
//
// DESCRIPTION:   Called when a click occurs on one of the action button.
//
// PARAMETER(S):
//                iIndex:
//                   TYPE:          int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Zero-based index of the button
//
// RETURN:        None.
//
// NOTES:         None.
//+*/
void CListBoxExBuddy::DoClick( int iIndex )
{

   switch ( iIndex )
   {
      case __BTN_NEW:
		  ::PostMessage(m_hWnd,WM_LISTBOXEXBUDDY_NEW,m_nID,0);
         break;
      case __BTN_EDIT:
		  ::PostMessage(m_hWnd,WM_LISTBOXEXBUDDY_EDIT,m_nID,0);
         break;
      case __BTN_DEL:
         ::PostMessage(m_hWnd,WM_LISTBOXEXBUDDY_DEL,m_nID,0);
         break;
      case __BTN_UP:
         ::PostMessage(m_hWnd,WM_LISTBOXEXBUDDY_UP,m_nID,0);
         break;

      case __BTN_DOWN:
         ::PostMessage(m_hWnd,WM_LISTBOXEXBUDDY_DOWN,m_nID,0);
         break;
   }
} // CListBoxExBuddy::DoClick


///*-
// FUNCTION NAME: CListBoxExBuddy::OnNcMouseMove
//
// DESCRIPTION:   Called when it receives a WM_NCMOUSEMOVE message.
//
// PARAMETER(S):
//                nHitTest:
//                   TYPE:          unsigned int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Hit test
//
//                point:
//                   TYPE:          CPoint class
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Point of action
//
// RETURN:        None.
//
// NOTES:         None.
//+*/
void CListBoxExBuddy::OnNcMouseMove( UINT   nHitTest, 
                                     CPoint point ) 
{
   // Redraw the affected button
   InvalidateButton( m_iButton, FALSE );
   m_iButton = FindButton( point );
   InvalidateButton( m_iButton, TRUE );

   // Call base
   CWnd::OnNcMouseMove(nHitTest, point);
} // CListBoxExBuddy::OnNcMouseMove



               ///////////////////////////////////////////////
               //            Tooltip Management             //
               ///////////////////////////////////////////////



///*-
// FUNCTION NAME: CListBoxExBuddy::CreateTooltips
//
// DESCRIPTION:   Creates the tooltip control and assigns the ttols to it.
//
// PARAMETER(S):  None.
//
// RETURN:        None.
//
// NOTES:         None.
//+*/
void CListBoxExBuddy::CreateTooltips()
{
   // Create the tooltip
   m_ToolTip.Create( this );

   // Set tip common data
   TOOLINFO ttInfo;
   ttInfo.cbSize   = sizeof( TOOLINFO );
   ttInfo.uFlags   = TTF_SUBCLASS;
   ttInfo.hwnd     = m_hWnd;
   ttInfo.rect     = CRect( 0, 0, 0, 0 ); // OnSize will resize it
   ttInfo.hinst    = NULL;
   ttInfo.lpszText = LPSTR_TEXTCALLBACK;
   ttInfo.lParam   = 0;

   // Add tooltips for each button
   for ( int iTip = 0; iTip < __BMP_NUMBTN; iTip++ )
   {
      ttInfo.uId = iTip+1;
      m_ToolTip.SendMessage( TTM_ADDTOOL, 0, (LPARAM)&ttInfo );
      m_ToolTip.Activate( TRUE );
   }
} // CListBoxExBuddy::CreateTooltips()


///*-
// FUNCTION NAME: CListBoxExBuddy::SetTipText
//
// DESCRIPTION:   Sett the appropriate tip text for the button
//
// PARAMETER(S):
//                nID:
//                   TYPE:          unsigned int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Button identifier (index)
//
//                szTipText:
//                   TYPE:          TCHAR
//                   MODE:          In
//                   MECHANISM:     By pointer
//                   DESCRIPTION:   Pointer to a preallocated buffer to be filled
//                                  with the tip text
//
// RETURN:        None.
//
// NOTES:         None.
//+*/
void CListBoxExBuddy::SetTipText( UINT   nID, 
                                  LPTSTR szTipText )
{
   TCHAR *aszTips[] = { _T("New"),
                        _T("Delete"),
                        _T("Move Up"),
                        _T("Move Down") };

   // Set tooltip text
   if ( nID < __BMP_NUMBTN )
   {
      _tcscpy( szTipText, aszTips[ nID ] );
   }
} // CListBoxExBuddy::SetTipText()



               ///////////////////////////////////////////////
               //             Other Messages                //
               ///////////////////////////////////////////////



///*-
// FUNCTION NAME: CListBoxExBuddy::OnSize
//
// DESCRIPTION:   Called when it receives a WM_SIZE message.
//
// PARAMETER(S):
//                nType:
//                   TYPE:          unsigned int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Specifies the type of resizing requested.
//
//                cx:
//                   TYPE:          int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   New width of the client area
//
//                cy:
//                   TYPE:          int
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   New height of the client area
//
// RETURN:        None.
//
// NOTES:         None.
//+*/
void CListBoxExBuddy::OnSize( UINT nType, 
                              int  cx, 
                              int  cy )
{
   // Get aware of the size of the client area
   CRect rcClient;
   GetClientRect( &rcClient );

   // This is used to center the button bitmap
   int nBmpTopY = (rcClient.Height() - __BMP_HEIGHT) / 2;

   // Update buttons positions
   TOOLINFO ttInfo;
   for ( int iIndex = 0; iIndex < __BMP_NUMBTN; iIndex++ )
   {
      m_arcButtons[ iIndex ].top     = nBmpTopY;
      m_arcButtons[ iIndex ].left    = cx - (__BMP_NUMBTN-iIndex)*__BMP_BTNWID;
      m_arcButtons[ iIndex ].bottom  = __BMP_HEIGHT + nBmpTopY;
      m_arcButtons[ iIndex ].right   = cx - (__BMP_NUMBTN-iIndex-1)*__BMP_BTNWID;

      // Resize tooltip area
      ttInfo.cbSize   = sizeof( TOOLINFO );
      ttInfo.hwnd     = m_hWnd;
      ttInfo.uId      = iIndex+1;
      ttInfo.rect     = m_arcButtons[ iIndex ];
      m_ToolTip.SendMessage( TTM_NEWTOOLRECT, 0, (LPARAM)&ttInfo );
   }


   // Call base
   CWnd::OnSize( nType, cx, cy );
} // CListBoxExBuddy::OnSize


///*-
// FUNCTION NAME: CListBoxExBuddy::OnNotify
//
// DESCRIPTION:   Called when it receives a WM_NOTIFY message.
//
// PARAMETER(S):
//                wParam:
//                   TYPE:          WPARAM
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Identifier of the common control sending the message. 
//
//                lParam:
//                   TYPE:          LPARAM
//                   MODE:          In
//                   MECHANISM:     By value
//                   DESCRIPTION:   Address of an NMHDR structure that contains 
//                                  the notification code and additional information. 
//
//                pResult:
//                   TYPE:          LRESULT
//                   MODE:          In
//                   MECHANISM:     By address
//                   DESCRIPTION:   Message result.
//
// RETURN:        None.
//
// NOTES:         None.
//+*/
BOOL CListBoxExBuddy::OnNotify( WPARAM   wParam, 
                                LPARAM   lParam, 
                                LRESULT *pResult )
{
   UINT nCode = ((NMHDR *)lParam)->code;

   // Get tooltip notification
   if ( nCode == TTN_GETDISPINFO )
   {
      UINT nID = ((NMHDR *)lParam)->idFrom - 1;
      SetTipText( nID, 
                  ((NMTTDISPINFO *)lParam)->szText );
      return TRUE;
   }

   // Call base
   return CWnd::OnNotify( wParam, lParam, pResult );
} // CListBoxExBuddy::OnNotify

BOOL CListBoxExBuddy::MySubclassDlgItem(UINT nID, CWnd *pParent)
{
	if( !SubclassDlgItem(nID,pParent) )
		return FALSE;
	m_nID = nID;
	return TRUE;
}
