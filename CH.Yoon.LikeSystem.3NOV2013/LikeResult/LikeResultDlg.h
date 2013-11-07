
// LikeResultDlg.h : header file
//

#pragma once

#include "like_result.h"
#include "like_result_delegate.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>


// CLikeResultDlg dialog
class CLikeResultDlg : public CDialogEx, public LikeResultDelegate
{
// Construction
public:
	CLikeResultDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LIKERESULT_DIALOG };

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
    afx_msg void OnBnClickedOpen(void);
    afx_msg void OnBnClickedClose(void);
    afx_msg void OnBnClickedDisconnect(void);
    afx_msg LRESULT OnDisconnected(WPARAM wparam, LPARAM lparam);
    virtual void OnCancel(void);
	DECLARE_MESSAGE_MAP()


private:
    virtual void OnConnected(void);
    virtual void OnLikeCount(unsigned int count);
    virtual void OnClosed(void);
    virtual void OnDisconnected(void);


private:
    boost::asio::io_service io_service_;
    boost::thread thread_;
    LikeResult result_;
};