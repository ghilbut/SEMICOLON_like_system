
// LikeResultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LikeResult.h"
#include "LikeResultDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLikeResultDlg dialog




CLikeResultDlg::CLikeResultDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLikeResultDlg::IDD, pParent)
    , result_(io_service_, *this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLikeResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLikeResultDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

    ON_BN_CLICKED(IDC_CONNECT, &CLikeResultDlg::OnBnClickedConnect)
    ON_BN_CLICKED(IDC_OPEN, &CLikeResultDlg::OnBnClickedOpen)
    ON_BN_CLICKED(IDC_CLOSE, &CLikeResultDlg::OnBnClickedClose)
    ON_BN_CLICKED(IDC_DISCONNECT, &CLikeResultDlg::OnBnClickedDisconnect)
    ON_MESSAGE(WM_APP+1, &CLikeResultDlg::OnDisconnected)
END_MESSAGE_MAP()


// CLikeResultDlg message handlers

BOOL CLikeResultDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    ::SetDlgItemText(*this, IDC_HOST, _T("127.0.0.1"));
    ::SetDlgItemText(*this, IDC_PORT, _T("8181"));
    ::SetDlgItemText(*this, IDC_USER, _T("p0"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLikeResultDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLikeResultDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLikeResultDlg::OnBnClickedConnect(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_CONNECT), FALSE);

    char ip[260];
    char port[10];
    ::GetDlgItemTextA(*this, IDC_HOST, ip, 260);
    ::GetDlgItemTextA(*this, IDC_PORT, port, 10);

    io_service_.reset();
    result_.Connect(ip, port);
    thread_.swap(boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
}

void CLikeResultDlg::OnBnClickedOpen(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_OPEN), FALSE);

    wchar_t wuser[260];
    ::GetDlgItemTextW(*this, IDC_USER, wuser, 260);

    char user[260] = "";
    const int size = WideCharToMultiByte(CP_UTF8, 0, wuser, wcslen(wuser), user, 260, 0, 0);
    if (size == 0) {
        // TODO(jh81.kim): error handling with size
        // see: http://msdn.microsoft.com/en-us/library/dd374130.aspx
        return;
    }

    result_.Open(user);
}

void CLikeResultDlg::OnBnClickedClose(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_CLOSE), FALSE);
    result_.Close();
}

void CLikeResultDlg::OnBnClickedDisconnect(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), FALSE);
    result_.Disconnect();
}

LRESULT CLikeResultDlg::OnDisconnected(WPARAM wparam, LPARAM lparam) {
    ::EnableWindow(::GetDlgItem(*this, IDC_HOST), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_PORT), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CONNECT), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_USER), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_OPEN), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CLOSE), FALSE);
    ::SetDlgItemTextA(*this, IDC_COUNT, "0");

    if (thread_.joinable()) {
        thread_.join();
    }
    return 0;
}

void CLikeResultDlg::OnCancel(void) {
    result_.Disconnect();
    CDialogEx::OnCancel();
}



void CLikeResultDlg::OnConnected(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_HOST), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_PORT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CONNECT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_USER), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_OPEN), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CLOSE), FALSE);
    ::SetDlgItemTextA(*this, IDC_COUNT, "0");
}

void CLikeResultDlg::OnLikeCount(unsigned int count) {
    char text[10];
    sprintf(text, "%u", count);
    ::SetDlgItemTextA(*this, IDC_COUNT, text);

    ::EnableWindow(::GetDlgItem(*this, IDC_HOST), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_PORT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CONNECT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_USER), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_OPEN), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CLOSE), TRUE);
}

void CLikeResultDlg::OnClosed(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_HOST), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_PORT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CONNECT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_USER), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_OPEN), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CLOSE), FALSE);
}

void CLikeResultDlg::OnDisconnected(void) {
    ::PostMessage(*this, WM_APP+1, 0, 0);
}