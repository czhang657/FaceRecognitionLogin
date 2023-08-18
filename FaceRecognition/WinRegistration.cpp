// WinRegistration.cpp: 实现文件
//

#include "pch.h"
#include "FaceRecognition.h"
#include "WinRegistration.h"
#include "afxdialogex.h"
#include <opencv2/opencv.hpp>
#include "tools.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include "faceTool.h"
#include "WinWelcome.h"
extern MHandle faceModel;

using namespace cv;

static VideoCapture cap;

// WinRegistration 对话框

IMPLEMENT_DYNAMIC(WinRegistration, CDialogEx)

WinRegistration::WinRegistration(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FACERECOGNITION_REGISTRATION, pParent)
{

}

WinRegistration::~WinRegistration()
{
}

void WinRegistration::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_Face_Recognition, m_buttonMouse);
	DDX_Control(pDX, IDC_EDIT1, m_userName);
	DDX_Control(pDX, IDC_SNOW, m_snowBG);
}

BOOL WinRegistration::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LoadPicture(m_faceBG, IDB_PNG4);
	m_buttonMouse.Init(IDB_PNG5, 4, BTN_TYPE_NORMAL);
	CString name;
	for (int i = 0; i < 16; i++) {
		name.Format(L"res/snow/snow_%d.bmp", i);
		m_snows[i] = (HBITMAP) LoadImage(0, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

	SetTimer(1, 30, NULL);
	CFont font;
	font.CreatePointFont(200, L"华文行楷", NULL);
	m_userName.SetFont(&font);
	::MoveWindow(m_snowBG.m_hWnd, 440, 0, 640, 609, 1);

	srand(time(NULL));
	return 0;
}


BEGIN_MESSAGE_MAP(WinRegistration, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_Face_Recognition, &WinRegistration::OnBnClickedButtonFaceRecognition)
	ON_STN_CLICKED(IDC_PHOTO_REAL, &WinRegistration::OnStnClickedPhotoReal)
	ON_STN_CLICKED(IDC_SNOW, &WinRegistration::OnStnClickedSnow)
END_MESSAGE_MAP()


// WinRegistration 消息处理程序


void WinRegistration::OnPaint()
{
	drawPicOnPait(&m_faceBG, this, 0, 0);
}


void WinRegistration::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int snowIndex = 0;
	static int count = 0;
	if (nIDEvent == 1) {
		m_snowBG.SetBitmap(m_snows[snowIndex]);
		snowIndex = (snowIndex + 1) % 16;
	}
	else if (nIDEvent == 2) {
		refreshCamera(&cap, m_hWnd, IDC_PHOTO_REAL);
		count++;
		if (count == 1) {
			mciSendString(L"play res/zhuche.mp3", 0, 0, 0);

		}
		else if (count == 30) {
			paiZhao(&cap, "tmp.jpg");
			count = 0;
			KillTimer(2);
			cap.release();



			IplImage* img1 = cvLoadImage("tmp.jpg");
			IplImage* img2 = cvLoadImage("res/neijun.jpg");

			float index = faceCompare(faceModel, img1, img2);
			int yanZhi = index * 10000 + 80 + rand() % 15;
			if (yanZhi <= 0) {
				yanZhi = 0;
			}
			else if (yanZhi >= 100) {
				yanZhi = 110;
			}

			const char* jobs[] = { "cs", "as", "sc" };

			CString editName;
			m_userName.GetWindowText(editName);
			USES_CONVERSION;
			char* editName2 = T2A(editName);
			char cmd[521];
			sprintf_s(cmd, sizeof(cmd), "move tmp.jpg users\\%s-%s-%d.jpg", jobs[rand() % 3], editName2, yanZhi);
			system(cmd);

			WinWelcome win;
			win.DoModal();
			CDialogEx::OnOK();
		}

	}


	CDialogEx::OnTimer(nIDEvent);
}


void WinRegistration::OnBnClickedButtonFaceRecognition()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_userName.GetWindowTextLength() == 0) {
		MessageBox(L"Enter your user name!");
		return;
	}
	KillTimer(1);

	HBITMAP pic = (HBITMAP) LoadImage(0, L"res/wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	cap.open(0);
	m_snowBG.SetBitmap(pic);

	SetTimer(2, 100, NULL);



}


void WinRegistration::OnStnClickedPhotoReal()
{
	// TODO: 在此添加控件通知处理程序代码
}


void WinRegistration::OnStnClickedSnow()
{
	// TODO: 在此添加控件通知处理程序代码
}
