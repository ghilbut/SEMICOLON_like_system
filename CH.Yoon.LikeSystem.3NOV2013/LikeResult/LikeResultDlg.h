
// LikeResultDlg.h : header file
//

#pragma once

#include "like_result.h"
#include "like_result_delegate.h"


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

    afx_msg void OnBnClickedConnect();
    afx_msg void OnBnClickedOpen();
    afx_msg void OnBnClickedClose();
    afx_msg void OnBnClickedDisconnect();
    virtual void OnCancel();
	DECLARE_MESSAGE_MAP()


private:
    virtual void OnConnected(void);
    virtual void OnLikeCount(unsigned int count);
    virtual void OnClosed(void);
    virtual void OnDisconnected(void);


private:
    LikeResult result_;
};