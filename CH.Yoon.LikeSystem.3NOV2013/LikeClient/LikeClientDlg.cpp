
// LikeClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LikeClient.h"
#include "LikeClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLikeClientDlg dialog




CLikeClientDlg::CLikeClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLikeClientDlg::IDD, pParent)
    , client_(io_service_, *this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLikeClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLikeClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

    ON_BN_CLICKED(IDC_CONNECT, &CLikeClientDlg::OnBnClickedConnect)
    ON_BN_CLICKED(IDC_JOIN, &CLikeClientDlg::OnBnClickedJoin)
    ON_BN_CLICKED(IDC_LEAVE, &CLikeClientDlg::OnBnClickedLeave)
    ON_BN_CLICKED(IDC_LIKE, &CLikeClientDlg::OnBnClickedLike)
    ON_BN_CLICKED(IDC_LIKE_CANCEL, &CLikeClientDlg::OnBnClickedLikeCancel)
    ON_BN_CLICKED(IDC_DISCONNECT, &CLikeClientDlg::OnBnClickedDisconnect)
    ON_MESSAGE(WM_APP+1, &CLikeClientDlg::OnDisconnected)
END_MESSAGE_MAP()


// CLikeClientDlg message handlers

BOOL CLikeClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    ::SetDlgItemText(*this, IDC_HOST, _T("127.0.0.1"));
    ::SetDlgItemText(*this, IDC_PORT, _T("8181"));
    ::SetDlgItemText(*this, IDC_USER, _T("myid"));
    ::SetDlgItemText(*this, IDC_TARGET, _T("apple"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLikeClientDlg::OnPaint()
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
HCURSOR CLikeClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLikeClientDlg::OnBnClickedConnect(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_CONNECT), FALSE);

    char ip[260];
    char port[10];
    ::GetDlgItemTextA(*this, IDC_HOST, ip, 260);
    ::GetDlgItemTextA(*this, IDC_PORT, port, 10);

    io_service_.reset();
    client_.Connect(ip, port);
    thread_.swap(boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
}

void CLikeClientDlg::OnBnClickedJoin(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_JOIN), FALSE);

    wchar_t wuser[260];
    wchar_t wtarget[260];
    ::GetDlgItemTextW(*this, IDC_USER, wuser, 260);
    ::GetDlgItemTextW(*this, IDC_TARGET, wtarget, 260);

    char user[260] = "";
    int size = WideCharToMultiByte(CP_UTF8, 0, wuser, wcslen(wuser), user, 260, 0, 0);
    if (size == 0) {
        // TODO(jh81.kim): error handling with size
        // see: http://msdn.microsoft.com/en-us/library/dd374130.aspx
        return;
    }

    char target[260] = "";
    size = WideCharToMultiByte(CP_UTF8, 0, wtarget, wcslen(wtarget), target, 260, 0, 0);
    if (size == 0) {
        // TODO(jh81.kim): error handling with size
        // see: http://msdn.microsoft.com/en-us/library/dd374130.aspx
        return;
    }

    client_.Join(user, target);
}

void CLikeClientDlg::OnBnClickedLeave(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_LEAVE), FALSE);
    client_.Leave();
}

void CLikeClientDlg::OnBnClickedLike(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_LIKE), FALSE);
    client_.Like(true);
}

void CLikeClientDlg::OnBnClickedLikeCancel(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_LIKE_CANCEL), FALSE);
    client_.Like(false);
}

void CLikeClientDlg::OnBnClickedDisconnect(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), FALSE);
    client_.Disconnect();
}

LRESULT CLikeClientDlg::OnDisconnected(WPARAM wparam, LPARAM lparam) {
    ::EnableWindow(::GetDlgItem(*this, IDC_HOST), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_PORT), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CONNECT), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), FALSE);

    ::EnableWindow(::GetDlgItem(*this, IDC_USER), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_TARGET), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_JOIN), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_LEAVE), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_LIKE), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_LIKE_CANCEL), FALSE);

    if (thread_.joinable()) {
        thread_.join();
    }
    return 0;
}

void CLikeClientDlg::OnCancel(void) {
    client_.Disconnect();
    CDialogEx::OnCancel();
}



void CLikeClientDlg::OnConnected(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_HOST), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_PORT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CONNECT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), TRUE);

    ::EnableWindow(::GetDlgItem(*this, IDC_USER), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_TARGET), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_JOIN), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_LEAVE), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_LIKE), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_LIKE_CANCEL), FALSE);
}

void CLikeClientDlg::OnAlreadyLike(bool like) {
    ::EnableWindow(::GetDlgItem(*this, IDC_HOST), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_PORT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CONNECT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), FALSE);

    ::EnableWindow(::GetDlgItem(*this, IDC_USER), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_TARGET), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_JOIN), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_LEAVE), TRUE);

    ::EnableWindow(::GetDlgItem(*this, IDC_LIKE), like ? FALSE : TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_LIKE_CANCEL), like ? TRUE : FALSE);
}

void CLikeClientDlg::OnLeaved(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_HOST), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_PORT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CONNECT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), FALSE);

    ::EnableWindow(::GetDlgItem(*this, IDC_USER), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_TARGET), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_JOIN), TRUE);
    ::EnableWindow(::GetDlgItem(*this, IDC_LEAVE), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_LIKE), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_LIKE_CANCEL), FALSE);
}

void CLikeClientDlg::OnDisconnected(void) {
    ::PostMessage(*this, WM_APP+1, 0, 0);
}