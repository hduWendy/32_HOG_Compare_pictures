// 32_HOG_Compare_pictures.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
int main()
{
	//读取本地的一张图片，将所有RGB值取平均，并显示出来！
	cv::Mat tempMat_0 = imread("C:/Users/lenovo/Desktop/数图图片/hogTemplate.jpg");
	cv::Mat Mat1_0 = imread("C:/Users/lenovo/Desktop/数图图片/img1.jpg");
	cv::Mat Mat2_0 = imread("C:/Users/lenovo/Desktop/数图图片/img2.jpg");

	cv::Mat tempMat = imread("C:/Users/lenovo/Desktop/数图图片/hogTemplate.jpg",0);
	cv::Mat Mat1 = imread("C:/Users/lenovo/Desktop/数图图片/img1.jpg",0);
	cv::Mat Mat2 = imread("C:/Users/lenovo/Desktop/数图图片/img2.jpg",0);
	
	int height = tempMat.rows; //行数
	int width = tempMat.cols; //每行元素的总元素数量
	
	int cellSize = 16;
	int nX = height / cellSize;
	int nY = width / cellSize;
	int bins = nX * nY * 8;
	int cell_t = 0;

	cv::Mat gx, gy;
	cv::Mat mag, angle;

	//开始tempMat处理
	//
	Sobel(tempMat, gx, CV_32F, 1, 0, 1);
	Sobel(tempMat, gy, CV_32F, 0, 1, 1);
	//计算梯度和角度方向
	cartToPolar(gx, gy, mag, angle, true);	
	//建立动态数组
	float* ref_hist = new float[bins];
	memset(ref_hist, 0, sizeof(float)*bins);
	
	for (int i = 0; i + cellSize <= height; i += cellSize)
	{
		for (int j = 0; j + cellSize <= width; j += cellSize)
		{
			//每一个cell
			for (int det_i = 0; det_i < cellSize; det_i++)
			{
				for (int det_j = 0; det_j < cellSize; det_j++)
				{
					//每一个像素
					for (int angle_t = 0; angle_t < 8; angle_t++)
					{
						if (angle.at<float>(i + det_i, j + det_j) >= angle_t * 45 && angle.at<float>(i + det_i, j + det_j) < (angle_t+1) * 45)
						{
							ref_hist[cell_t + angle_t] += mag.at<float>(i + det_i, j + det_j);
							break;
						}
					}//单个像素处理结束
				}
			}//单个cell处理结束
			cell_t++;			
		} 
	}
	cell_t = 0;
	//tempMat处理结束

	//开始Mat1处理
	//
	Sobel(Mat1, gx, CV_32F, 1, 0, 1);
	Sobel(Mat1, gy, CV_32F, 0, 1, 1);
	//计算梯度和角度方向
	cartToPolar(gx, gy, mag, angle, true);
	//建立动态数组
	float* ref_hist_Mat1 = new float[bins];
	memset(ref_hist_Mat1, 0, sizeof(float)*bins);

	for (int i = 0; i + cellSize <= height; i += cellSize)
	{
		for (int j = 0; j + cellSize <= width; j += cellSize)
		{
			//每一个cell
			for (int det_i = 0; det_i < cellSize; det_i++)
			{
				for (int det_j = 0; det_j < cellSize; det_j++)
				{
					//每一个像素
					for (int angle_t = 0; angle_t < 8; angle_t++)
					{
						if (angle.at<float>(i + det_i, j + det_j) >= angle_t * 45 && angle.at<float>(i + det_i, j + det_j) < (angle_t + 1) * 45)
						{
							ref_hist_Mat1[cell_t + angle_t] += mag.at<float>(i + det_i, j + det_j);
							break;
						}
					}//单个像素处理结束
				}
			}//单个cell处理结束
			cell_t++;
		}
	}
	cell_t = 0;
	//Mat1处理结束

	//开始Mat2处理
	//
	Sobel(Mat2, gx, CV_32F, 1, 0, 1);
	Sobel(Mat2, gy, CV_32F, 0, 1, 1);
	//计算梯度和角度方向
	cartToPolar(gx, gy, mag, angle, true);
	//建立动态数组
	float* ref_hist_Mat2 = new float[bins];
	memset(ref_hist_Mat2, 0, sizeof(float)*bins);

	for (int i = 0; i + cellSize <= height; i += cellSize)
	{
		for (int j = 0; j + cellSize <= width; j += cellSize)
		{
			//每一个cell
			for (int det_i = 0; det_i < cellSize; det_i++)
			{
				for (int det_j = 0; det_j < cellSize; det_j++)
				{
					//每一个像素
					for (int angle_t = 0; angle_t < 8; angle_t++)
					{
						if (angle.at<float>(i + det_i, j + det_j) >= angle_t * 45 && angle.at<float>(i + det_i, j + det_j) < (angle_t + 1) * 45)
						{
							ref_hist_Mat2[cell_t + angle_t] += mag.at<float>(i + det_i, j + det_j);
							break;
						}
					}//单个像素处理结束
				}
			}//单个cell处理结束
			cell_t++;
		}
	}
	cell_t = 0;
	//Mat2处理结束

	//计算欧几里得距离，得到直方图的相似度
	float gap_Mat1 = 0, gap_Mat2 = 0;
	for (int t = 0; t < bins; t++)
	{
		gap_Mat1 += sqrt(pow((ref_hist_Mat1[t] - ref_hist[t]), 2));
		gap_Mat2 += sqrt(pow((ref_hist_Mat2[t] - ref_hist[t]), 2));
	}
	//显示原图和更相似的那一张图
	imshow("tempMat", tempMat_0);
	if (gap_Mat1 < gap_Mat2)
	{
		imshow("Mat1", Mat1_0);
		std::cout << "img1.jpg与原图更相似 "<< std::endl;
	}		
	else if(gap_Mat1 > gap_Mat2)
	{ 
		imshow("Mat2", Mat2_0);
		std::cout << "img2.jpg与原图更相似 " << std::endl;
	}		
	else
	{
		imshow("Mat1", Mat1_0);
		imshow("Mat2", Mat2_0);
		std::cout << "两图与原图同样相似 " << std::endl;
	}		
	
	//等待用户按键
	waitKey(0);
	delete[] ref_hist;
	return 0;
}


