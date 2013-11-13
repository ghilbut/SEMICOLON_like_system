
// LikePannelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LikePannel.h"
#include "LikePannelDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLikePannelDlg dialog




CLikePannelDlg::CLikePannelDlg(const std::string& host, const std::string& port, CWnd* pParent /*=NULL*/)
	: CDialogEx(CLikePannelDlg::IDD, pParent)
    , host_(host)
    , port_(port)
    , result_(io_service_, *this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLikePannelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLikePannelDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

    ON_BN_CLICKED(IDC_CLOSE, &CLikePannelDlg::OnBnClickedClose)
    ON_BN_CLICKED(IDC_DISCONNECT, &CLikePannelDlg::OnBnClickedDisconnect)
    ON_MESSAGE(WM_APP+1, &CLikePannelDlg::OnDisconnected)
END_MESSAGE_MAP()


// CLikePannelDlg message handlers

BOOL CLikePannelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    RECT rc_desktop, rc_this;
    ::GetWindowRect(::GetDesktopWindow(), &rc_desktop);
    ::GetWindowRect(*this, &rc_this);
    ::SetWindowPos(*this
                   , 0
                   , rc_desktop.right-(rc_this.right-rc_this.left)-10
                   , rc_desktop.top+10
                   , 0
                   , 0
                   , SWP_NOSIZE | SWP_NOZORDER);

    io_service_.reset();
    result_.Connect(host_.c_str(), port_.c_str());
    thread_.swap(boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLikePannelDlg::OnPaint()
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
HCURSOR CLikePannelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLikePannelDlg::OnBnClickedClose(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_CLOSE), FALSE);
    result_.Close();
}

void CLikePannelDlg::OnBnClickedDisconnect(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), FALSE);
    result_.Disconnect();
}

LRESULT CLikePannelDlg::OnDisconnected(WPARAM wparam, LPARAM lparam) {
    
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CLOSE), FALSE);
    ::SetDlgItemTextA(*this, IDC_COUNT, "0");

    if (thread_.joinable()) {
        thread_.join();
    }
    return 0;
}

void CLikePannelDlg::OnCancel(void) {
    result_.Disconnect();
    CDialogEx::OnCancel();
}



void CLikePannelDlg::OnConnected(void) {
    ::SetDlgItemTextA(*this, IDC_COUNT, "0");
    result_.Open();
}

void CLikePannelDlg::OnOpened(const std::string& name) {

    wchar_t wname[260] = L"";
    if (::MultiByteToWideChar(CP_UTF8, 0, name.c_str(), name.length(), wname, 260) == 0) {
        // TODO(jh81.kim): error handling with size
        // see: http://msdn.microsoft.com/en-us/library/dd374130.aspx
        return;
    }

    ::SetDlgItemTextW(*this, IDC_NAME, wname);
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_CLOSE), TRUE);
}

void CLikePannelDlg::OnLikeCount(unsigned int count) {
    char text[10];
    sprintf(text, "%u", count);
    ::SetDlgItemTextA(*this, IDC_COUNT, text);
}

void CLikePannelDlg::OnClosed(void) {
    ::EnableWindow(::GetDlgItem(*this, IDC_CLOSE), FALSE);
}

void CLikePannelDlg::OnDisconnected(void) {
    ::PostMessage(*this, WM_APP+1, 0, 0);
}