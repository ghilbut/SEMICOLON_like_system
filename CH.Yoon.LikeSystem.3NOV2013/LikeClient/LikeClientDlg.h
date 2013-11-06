
// LikeClientDlg.h : header file
//

#pragma once

#include "like_client.h"
#include "like_client_delegate.h"


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

    afx_msg void OnBnClickedConnect();
    afx_msg void OnBnClickedJoin();
    afx_msg void OnBnClickedLeave();
    afx_msg void OnBnClickedLike();
    afx_msg void OnBnClickedLikeCancel();
    afx_msg void OnBnClickedDisconnect();
    virtual void OnCancel();
	DECLARE_MESSAGE_MAP()


private:
    virtual void OnConnected(void);
    virtual void OnAlreadyLike(bool like);
    virtual void OnLeaved(void);
    virtual void OnDisconnected(void);


private:
    LikeClient client_;
};