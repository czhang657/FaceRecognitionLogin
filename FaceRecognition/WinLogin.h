#pragma once
#include "ButtonPNG.h"


// WinLogin 对话框

class WinLogin : public CDialogEx
{
	DECLARE_DYNAMIC(WinLogin)

public:
	WinLogin(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WinLogin();
	BOOL OnInitDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACERECOGNITION_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFaceRecognition();
	ButtonPNG m_btnFaceRecognition;
	CImage m_imgBG;
	HBITMAP m_snows[16];
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_snow;
	afx_msg void OnStnClickedPhotoReal2();
	CStatic m_photo_real2;

	char m_job[64];
	char m_userName[64];
	int m_yanZhi;
	bool m_logined;
};
