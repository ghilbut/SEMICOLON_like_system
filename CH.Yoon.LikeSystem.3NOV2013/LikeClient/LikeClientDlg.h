
// LikeClientDlg.h : header file
//

#pragma once

#include "like_client.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>


// CLikeClientDlg dialog
class CLikeClientDlg : public CDialogEx
{
// Construction
public:
	CLikeClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LIKECLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

    afx_msg void OnBnClickedConnect();
    afx_msg void OnBnClickedDisconnect();
    afx_msg void OnBnClickedLike();
    virtual void OnCancel();
	DECLARE_MESSAGE_MAP()



private:
    boost::asio::io_service io_service_;
    boost::thread thread_;
    chat_client* client_;
};
