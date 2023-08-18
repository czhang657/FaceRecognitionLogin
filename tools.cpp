#include "pch.h"
#include "tools.h"
#include <opencv2/opencv.hpp>
#include "CvvImage.h"
#include <Windows.h>

using namespace cv;

void paiZhao(VideoCapture* cap, const char* name)
{
	CvvImage out; //CvvImage, opencv��������ʾͼƬ����
	CRect outrect;
	if (!cap->isOpened())
	{
		MessageBox(NULL, _T("����ͷ��ʧ�ܣ�"), _T("����"), MB_OK);
		return;
	}

	Mat frame; //Mat ������ʾͼƬ����C++��
	*cap >> frame;
	IplImage outImage = frame;

	//char  fileName[64];
	//sprintf(fileName, "%s.jpg", name);
	//getCamera(&cap);
	cvSaveImage(name, &outImage, 0);//����ͼƬ
}

void showImage(CvvImage* out, HWND hwnd, UINT imgID) {
	CRect outrect;

	int x = out->Width(); //��������ͷ��Ĭ�ϴ�С 640*480
	int y = out->Height();
	//��ȡPicture Control�ؼ��Ĵ�С  
	CWnd* pCWnd = CWnd::FromHandle(hwnd);
	pCWnd->GetDlgItem(imgID)->GetWindowRect(&outrect);
	//���ͻ���ѡ�е��ؼ���ʾ�ľ���������  
	pCWnd->ScreenToClient(&outrect);
	//�����ƶ����ؼ���ʾ������  
	pCWnd->GetDlgItem(imgID)->MoveWindow(outrect.left, outrect.top, x, y, TRUE);
	CWnd* ppWnd = NULL;
	ppWnd = pCWnd->GetDlgItem(imgID);//��ȡ�ؼ����  
	ppWnd->GetClientRect(&outrect);//��ȡ���ָ��ؼ�����Ĵ�С  
	CDC* ppDc = NULL;
	ppDc = ppWnd->GetDC();//��ȡpicture��DC  
	out->DrawToHDC(ppDc->m_hDC, outrect);//��ͼƬ���Ƶ�picture��ʾ��������  
	pCWnd->ReleaseDC(ppDc);
}

void refreshCamera(VideoCapture* cap, HWND hwnd, UINT id)
{
	CvvImage out; //CvvImage, opencv��������ʾͼƬ����
	CRect outrect;
	if (!cap->isOpened())
	{
		MessageBox(NULL, _T("����ͷ��ʧ�ܣ�"), _T("����"), MB_OK);
		return;
	}

	Mat frame; //Mat ������ʾͼƬ����C++��
	*cap >> frame;
	IplImage outImage = frame;
	//outvedio = &outImage;
	out.CopyOf(&outImage);

	showImage(&out, hwnd, id);
}

// draw srcImage to destImage
void putimagePNG(int  picture_x, int picture_y, IMAGE* srcImage, IMAGE* destImage) //xΪ����ͼƬ��X���꣬yΪY����
{
	SetWorkingImage(destImage);
	DWORD* dst = GetImageBuffer(destImage);    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
	DWORD* draw = GetImageBuffer(destImage);
	DWORD* src = GetImageBuffer(srcImage); //��ȡpicture���Դ�ָ��
	int picture_width = srcImage->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
	int picture_height = srcImage->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
	int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
	int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
	int dstX = 0;    //���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
			}
		}
	}
	SetWorkingImage();
}
