#include "pch.h"
#include "faceTool.h"

//�ü�ͼƬ,�����ֽڶ���
void CutIplImage(IplImage* src, IplImage* dst, int x, int y)
{
	CvSize size = cvSize(dst->width, dst->height);//�����С
	cvSetImageROI(src, cvRect(x, y, size.width, size.height));//����Դͼ��ROI
	cvCopy(src, dst); //����ͼ��
	cvResetImageROI(src);//Դͼ����������ROI
}

void faceInit(MHandle* handle) {
	//����ӿ�,����������
	MRESULT res = ASFOnlineActivation((char*)APPID, (char*)SDKKey);
	if (MOK != res && MERR_ASF_ALREADY_ACTIVATED != res)
		printf("����ʧ��\n");

	//��ȡ�����ļ���Ϣ
	ASF_ActiveFileInfo  activeFileInfo;
	res = ASFGetActiveFileInfo(&activeFileInfo);
	if (res != MOK)
		printf("ASFGetActiveFileInfo fail: %d\n", res);

	//��ʼ���ӿ�
	MInt32 mask = ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE | ASF_LIVENESS | ASF_IR_LIVENESS;
	res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY, 30, 10, mask, handle);
	if (res != MOK)
		printf("�ӿڳ�ʼ��ʧ��\n");
}

float faceCompare(MHandle handle, IplImage* img1, IplImage* img2) {
	ASF_MultiFaceInfo detectedFaces1 = { 0 };
	ASF_SingleFaceInfo SingleDetectedFaces1 = { 0 };
	ASF_FaceFeature feature1 = { 0 };
	ASF_FaceFeature copyfeature1 = { 0 };
	IplImage* cutImg1 = cvCreateImage(cvSize(img1->width - img1->width % 4, img1->height), IPL_DEPTH_8U, img1->nChannels);
	CutIplImage(img1, cutImg1, 0, 0);

	MRESULT res;
	res = ASFDetectFaces(handle, cutImg1->width, cutImg1->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg1->imageData, &detectedFaces1);

	if (MOK == res)
	{
		SingleDetectedFaces1.faceRect.left = detectedFaces1.faceRect[0].left;
		SingleDetectedFaces1.faceRect.top = detectedFaces1.faceRect[0].top;
		SingleDetectedFaces1.faceRect.right = detectedFaces1.faceRect[0].right;
		SingleDetectedFaces1.faceRect.bottom = detectedFaces1.faceRect[0].bottom;
		SingleDetectedFaces1.faceOrient = detectedFaces1.faceOrient[0];

		res = ASFFaceFeatureExtract(handle, cutImg1->width, cutImg1->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg1->imageData, &SingleDetectedFaces1, &feature1);
		if (res == MOK)
		{
			//����feature
			copyfeature1.featureSize = feature1.featureSize;
			copyfeature1.feature = (MByte*)malloc(feature1.featureSize);
			memset(copyfeature1.feature, 0, feature1.featureSize);
			memcpy(copyfeature1.feature, feature1.feature, feature1.featureSize);
		}
		else
			printf("ASFFaceFeatureExtract 1 fail: %d\n", res);
	}
	else
		printf("ASFDetectFaces 1 fail: %d\n", res);


	ASF_MultiFaceInfo	detectedFaces2 = { 0 };
	ASF_SingleFaceInfo SingleDetectedFaces2 = { 0 };
	ASF_FaceFeature feature2 = { 0 };
	IplImage* cutImg2 = cvCreateImage(cvSize(img2->width - img2->width % 4, img2->height), IPL_DEPTH_8U, img2->nChannels);
	CutIplImage(img2, cutImg2, 0, 0);
	res = ASFDetectFaces(handle, cutImg2->width, cutImg2->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg2->imageData, &detectedFaces2);
	if (MOK == res)
	{
		SingleDetectedFaces2.faceRect.left = detectedFaces2.faceRect[0].left;
		SingleDetectedFaces2.faceRect.top = detectedFaces2.faceRect[0].top;
		SingleDetectedFaces2.faceRect.right = detectedFaces2.faceRect[0].right;
		SingleDetectedFaces2.faceRect.bottom = detectedFaces2.faceRect[0].bottom;
		SingleDetectedFaces2.faceOrient = detectedFaces2.faceOrient[0];

		res = ASFFaceFeatureExtract(handle, cutImg2->width, cutImg2->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg2->imageData, &SingleDetectedFaces2, &feature2);
		if (MOK != res)
			printf("ASFFaceFeatureExtract 2 fail: %d\n", res);
	}
	else
		printf("ASFDetectFaces 2 fail: %d\n", res);


	MFloat confidenceLevel;
	res = ASFFaceFeatureCompare(handle, &copyfeature1, &feature2, &confidenceLevel);
	if (res != MOK)
		confidenceLevel = -1;

	SafeFree(copyfeature1.feature);		//�ͷ��ڴ�
	cvReleaseImage(&cutImg1);
	cvReleaseImage(&cutImg2);

	return confidenceLevel;
}