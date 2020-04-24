// 32_HOG_Compare_pictures.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
int main()
{
	//��ȡ���ص�һ��ͼƬ��������RGBֵȡƽ��������ʾ������
	cv::Mat tempMat_0 = imread("C:/Users/lenovo/Desktop/��ͼͼƬ/hogTemplate.jpg");
	cv::Mat Mat1_0 = imread("C:/Users/lenovo/Desktop/��ͼͼƬ/img1.jpg");
	cv::Mat Mat2_0 = imread("C:/Users/lenovo/Desktop/��ͼͼƬ/img2.jpg");

	cv::Mat tempMat = imread("C:/Users/lenovo/Desktop/��ͼͼƬ/hogTemplate.jpg",0);
	cv::Mat Mat1 = imread("C:/Users/lenovo/Desktop/��ͼͼƬ/img1.jpg",0);
	cv::Mat Mat2 = imread("C:/Users/lenovo/Desktop/��ͼͼƬ/img2.jpg",0);
	
	int height = tempMat.rows; //����
	int width = tempMat.cols; //ÿ��Ԫ�ص���Ԫ������
	
	int cellSize = 16;
	int nX = height / cellSize;
	int nY = width / cellSize;
	int bins = nX * nY * 8;
	int cell_t = 0;

	cv::Mat gx, gy;
	cv::Mat mag, angle;

	//��ʼtempMat����
	//
	Sobel(tempMat, gx, CV_32F, 1, 0, 1);
	Sobel(tempMat, gy, CV_32F, 0, 1, 1);
	//�����ݶȺͽǶȷ���
	cartToPolar(gx, gy, mag, angle, true);	
	//������̬����
	float* ref_hist = new float[bins];
	memset(ref_hist, 0, sizeof(float)*bins);
	
	for (int i = 0; i + cellSize <= height; i += cellSize)
	{
		for (int j = 0; j + cellSize <= width; j += cellSize)
		{
			//ÿһ��cell
			for (int det_i = 0; det_i < cellSize; det_i++)
			{
				for (int det_j = 0; det_j < cellSize; det_j++)
				{
					//ÿһ������
					for (int angle_t = 0; angle_t < 8; angle_t++)
					{
						if (angle.at<float>(i + det_i, j + det_j) >= angle_t * 45 && angle.at<float>(i + det_i, j + det_j) < (angle_t+1) * 45)
						{
							ref_hist[cell_t + angle_t] += mag.at<float>(i + det_i, j + det_j);
							break;
						}
					}//�������ش������
				}
			}//����cell�������
			cell_t++;			
		} 
	}
	cell_t = 0;
	//tempMat�������

	//��ʼMat1����
	//
	Sobel(Mat1, gx, CV_32F, 1, 0, 1);
	Sobel(Mat1, gy, CV_32F, 0, 1, 1);
	//�����ݶȺͽǶȷ���
	cartToPolar(gx, gy, mag, angle, true);
	//������̬����
	float* ref_hist_Mat1 = new float[bins];
	memset(ref_hist_Mat1, 0, sizeof(float)*bins);

	for (int i = 0; i + cellSize <= height; i += cellSize)
	{
		for (int j = 0; j + cellSize <= width; j += cellSize)
		{
			//ÿһ��cell
			for (int det_i = 0; det_i < cellSize; det_i++)
			{
				for (int det_j = 0; det_j < cellSize; det_j++)
				{
					//ÿһ������
					for (int angle_t = 0; angle_t < 8; angle_t++)
					{
						if (angle.at<float>(i + det_i, j + det_j) >= angle_t * 45 && angle.at<float>(i + det_i, j + det_j) < (angle_t + 1) * 45)
						{
							ref_hist_Mat1[cell_t + angle_t] += mag.at<float>(i + det_i, j + det_j);
							break;
						}
					}//�������ش������
				}
			}//����cell�������
			cell_t++;
		}
	}
	cell_t = 0;
	//Mat1�������

	//��ʼMat2����
	//
	Sobel(Mat2, gx, CV_32F, 1, 0, 1);
	Sobel(Mat2, gy, CV_32F, 0, 1, 1);
	//�����ݶȺͽǶȷ���
	cartToPolar(gx, gy, mag, angle, true);
	//������̬����
	float* ref_hist_Mat2 = new float[bins];
	memset(ref_hist_Mat2, 0, sizeof(float)*bins);

	for (int i = 0; i + cellSize <= height; i += cellSize)
	{
		for (int j = 0; j + cellSize <= width; j += cellSize)
		{
			//ÿһ��cell
			for (int det_i = 0; det_i < cellSize; det_i++)
			{
				for (int det_j = 0; det_j < cellSize; det_j++)
				{
					//ÿһ������
					for (int angle_t = 0; angle_t < 8; angle_t++)
					{
						if (angle.at<float>(i + det_i, j + det_j) >= angle_t * 45 && angle.at<float>(i + det_i, j + det_j) < (angle_t + 1) * 45)
						{
							ref_hist_Mat2[cell_t + angle_t] += mag.at<float>(i + det_i, j + det_j);
							break;
						}
					}//�������ش������
				}
			}//����cell�������
			cell_t++;
		}
	}
	cell_t = 0;
	//Mat2�������

	//����ŷ����þ��룬�õ�ֱ��ͼ�����ƶ�
	float gap_Mat1 = 0, gap_Mat2 = 0;
	for (int t = 0; t < bins; t++)
	{
		gap_Mat1 += sqrt(pow((ref_hist_Mat1[t] - ref_hist[t]), 2));
		gap_Mat2 += sqrt(pow((ref_hist_Mat2[t] - ref_hist[t]), 2));
	}
	//��ʾԭͼ�͸����Ƶ���һ��ͼ
	imshow("tempMat", tempMat_0);
	if (gap_Mat1 < gap_Mat2)
	{
		imshow("Mat1", Mat1_0);
		std::cout << "img1.jpg��ԭͼ������ "<< std::endl;
	}		
	else if(gap_Mat1 > gap_Mat2)
	{ 
		imshow("Mat2", Mat2_0);
		std::cout << "img2.jpg��ԭͼ������ " << std::endl;
	}		
	else
	{
		imshow("Mat1", Mat1_0);
		imshow("Mat2", Mat2_0);
		std::cout << "��ͼ��ԭͼͬ������ " << std::endl;
	}		
	
	//�ȴ��û�����
	waitKey(0);
	delete[] ref_hist;
	return 0;
}


