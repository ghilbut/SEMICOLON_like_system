
// LikeClientDlg.h : header file
//

#pragma once

#include "like_client.h"
#include "like_client_delegate.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>


// CLikeClientDlg dialog
class CLikeClientDlg : public CDialogEx, public LikeClientDelegate
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

    afx_msg void OnBnClickedConnect(void);
    afx_msg void OnBnClickedJoin(void);
    afx_msg void OnBnClickedLeave(void);
    afx_msg void OnBnClickedLike(void);
    afx_msg void OnBnClickedLikeCancel(void);
    afx_msg void OnBnClickedDisconnect(void);
    afx_msg LRESULT OnDisconnected(WPARAM wparam, LPARAM lparam);
    virtual void OnCancel(void);
	DECLARE_MESSAGE_MAP()


private:
    virtual void OnConnected(void);
    virtual void OnAlreadyLike(bool like);
    virtual void OnLeaved(void);
    virtual void OnDisconnected(void);


private:
    boost::asio::io_service io_service_;
    boost::thread thread_;
    LikeClient client_;
};