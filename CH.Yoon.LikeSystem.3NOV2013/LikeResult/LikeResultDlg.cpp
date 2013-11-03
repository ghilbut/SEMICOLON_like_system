
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
	: CDialogEx(CLikeResultDlg::IDD, pParent), client_(0)
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
    ON_BN_CLICKED(IDC_DISCONNECT, &CLikeResultDlg::OnBnClickedDisconnect)
    ON_BN_CLICKED(IDC_LIKE, &CLikeResultDlg::OnBnClickedLike)
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
    ::EnableWindow(::GetDlgItem(*this, IDC_DISCONNECT), FALSE);
    ::EnableWindow(::GetDlgItem(*this, IDC_LIKE), FALSE);

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



void CLikeResultDlg::OnBnClickedConnect()
{
    // TODO: Add your control notification handler code here
    char ip[260];
    char port[10];
    wchar_t target[1024];
    wchar_t user[1024];
    ::GetDlgItemTextA(*this, IDC_HOST, ip, 260);
    ::GetDlgItemTextA(*this, IDC_PORT, port, 10);
    ::GetDlgItemTextW(*this, IDC_TARGET, target, 1024);
    ::GetDlgItemTextW(*this, IDC_USER, user, 1024);

    tcp::resolver resolver(io_service_);
    tcp::resolver::query query(ip, port);
    tcp::resolver::iterator iterator = resolver.resolve(query);
    client_ = new chat_client(io_service_, iterator);
    thread_.swap(boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
}

void CLikeResultDlg::OnBnClickedDisconnect()
{
    // TODO: Add your control notification handler code here
    if (client_) {
        client_->close();
        thread_.join();
        delete client_;
    }
}

void CLikeResultDlg::OnBnClickedLike()
{
    // TODO: Add your control notification handler code here
    if (client_) {
    }
}

void CLikeResultDlg::OnCancel()
{
    // TODO: Add your specialized code here and/or call the base class
    OnBnClickedDisconnect();

    CDialogEx::OnCancel();
}