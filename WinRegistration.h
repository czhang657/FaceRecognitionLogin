#pragma once

#include "ButtonPNG.h"
// WinRegistration 对话框

class WinRegistration : public CDialogEx
{
	DECLARE_DYNAMIC(WinRegistration)

public:
	WinRegistration(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WinRegistration();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACERECOGNITION_REGISTRATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	BOOL OnInitDialog();
	ButtonPNG m_buttonMouse;
	CEdit m_userName;
	CStatic m_snowBG;

	CImage m_faceBG;
	afx_msg void OnPaint();
	HBITMAP m_snows[16];
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonFaceRecognition();
	afx_msg void OnStnClickedPhotoReal();
	afx_msg void OnStnClickedSnow();
};
