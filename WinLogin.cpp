// WinLogin.cpp: 实现文件
//
#include "pch.h"
#include "FaceRecognition.h"
#include "WinLogin.h"
#include "afxdialogex.h"

#include <opencv2/opencv.hpp>
#include "tools.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include "faceTool.h"
#include "WinWelcome.h"
#include <vector>
#pragma warning(disable:4996)
using namespace std;

extern MHandle faceModel;

using namespace cv;

static VideoCapture cap;

// WinLogin 对话框

IMPLEMENT_DYNAMIC(WinLogin, CDialogEx)

WinLogin::WinLogin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FACERECOGNITION_LOGIN, pParent)
{

}

WinLogin::~WinLogin()
{
}

BOOL WinLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LoadPicture(m_imgBG, IDB_PNG4);
	m_btnFaceRecognition.Init(IDB_PNG5, 4, BTN_TYPE_NORMAL);


	//char name[256];
	CString name;
	for (int i = 0; i < 16; i++) {
		name.Format(L"res/snow/snow_%d.bmp", i);
		m_snows[i] = (HBITMAP) LoadImage(0, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

	// Set timer

	SetTimer(1, 30, NULL);

	::MoveWindow(m_snow.m_hWnd, 440, 0, 640, 609, 1);

	return 0;
}

void WinLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_Face_Recognition, m_btnFaceRecognition);
	DDX_Control(pDX, IDC_SNOW, m_snow);
	DDX_Control(pDX, IDC_PHOTO_REAL2, m_photo_real2);
}


BEGIN_MESSAGE_MAP(WinLogin, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON1, &WinLogin::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_Face_Recognition, &WinLogin::OnBnClickedButtonFaceRecognition)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_PHOTO_REAL2, &WinLogin::OnStnClickedPhotoReal2)
END_MESSAGE_MAP()


// WinLogin 消息处理程序




void WinLogin::OnBnClickedButtonFaceRecognition()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	cap.open(0);

	HBITMAP pic = (HBITMAP) LoadImage(0, L"res/wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_snow.SetBitmap(pic);
	::MoveWindow(m_photo_real2.m_hWnd, 440, 64, 640, 480, 1);
	SetTimer(2, 100, NULL);


}


void WinLogin::OnPaint()
{
	drawPicOnPait(&m_imgBG, this, 0, 0);
}

bool faceCheck(const char* fileName, char* userName) {
	vector<CString> vetPath;

	WIN32_FIND_DATA fileData;
	HANDLE file = FindFirstFile(L"users/*.jpg", &fileData);
	if (file != INVALID_HANDLE_VALUE) {
		vetPath.push_back(fileData.cFileName);
		while (FindNextFile(file, &fileData)) {
			vetPath.push_back(fileData.cFileName);
		}
	}

	IplImage *img1 = cvLoadImage("tmp.jpg");
	for (int i = 0; i < vetPath.size(); i++) {
		USES_CONVERSION;
		char *otherName = T2A(vetPath[i]);
		char name[256];
		sprintf_s(name, 256, "users/%s", otherName);
		IplImage *img2 = cvLoadImage(name);
		float ret = faceCompare(faceModel, img2, img1);
		if (ret >= -1) {
			string tmpStr = otherName;
			string nameStr = tmpStr.substr(0, tmpStr.length() - 4);
			strcpy(userName, nameStr.c_str());
			return true;
		}
	}


	return false;
}

void WinLogin::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	static int snowIndex = 0;
	static int count = 0;
	static char userName[256] = "";
	static bool success = false;
	if (nIDEvent == 1) {
		m_snow.SetBitmap(m_snows[snowIndex]);
		snowIndex = (snowIndex + 1) % 16;
	}
	else if (nIDEvent == 2) {
		refreshCamera(&cap, m_hWnd, IDC_PHOTO_REAL2);

		count++;
		

		if (count == 10) {
			const char* fileName = "tmp.jpg";
			paiZhao(&cap, fileName);
			if (faceCheck(fileName, userName)) {
				mciSendString(L"play res/login.mp3", 0, 0, 0);
				success = true;
			}

		}
		else if (count == 20) {
			count = 0;
			KillTimer(2);
			cap.release();
			if (success) {
				strcpy(m_job, strtok(userName, "-"));
				strcpy(m_userName, strtok(userName, "-"));
				m_yanZhi = atoi(strtok(userName, "-"));
				m_logined = true;
				CDialogEx::OnOK();
			}
			else {
				MessageBox(L"Login Failed");
				SetTimer(1, 100, NULL);
				m_logined = false;
			}

		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void WinLogin::OnStnClickedPhotoReal2()
{
	// TODO: 在此添加控件通知处理程序代码
}
