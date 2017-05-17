
#include <stdio.h>
#include <iostream>
#include <thread>
#include <math.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

vector<Mat> sampleMat;

void init_sampleMat();
String go_detect(Mat &src);
void thres_rgb(Mat src, Mat &gray);
void k_means_spilter(Mat src, Mat &dst);
void check_is_line(Mat src, Mat &dst);
int col_length(Mat src, int dst_row, int dst_col);
int row_length(Mat src, int dst_row, int dst_col);
void delete_target(Mat src, Mat dst_0, Mat dst_1);
void delete_point(Mat src);
vector<Mat> find_number(Mat src);
bool rect_cmp_by_center(Rect a, Rect b);
String dect_number(vector<Mat> src);
void rotateImage(Mat src, Mat dst, int degree);


int main() {
	init_sampleMat();

	String filename = "..\\..\\..\\test_data\\captcha_data\\01.jpg";
	Mat src = imread(filename);
	imshow("src", src);

	go_detect(src);

	waitKey(0);

	return 0;
}

void init_sampleMat() {

	//	載入樣本圖片並旋轉
	for (int i = 0; i < 20; i++) {
		Mat sample = imread("number_samples\\" + to_string(i) + ".png", IMREAD_GRAYSCALE);

		for (int rota = -90; rota <= 90; rota += 5) {
			Mat rotateImg = sample.clone();
			rotateImage(rotateImg, rotateImg, rota);
			sampleMat.push_back(rotateImg);
		}
	}
}

void rotateImage(Mat src, Mat dst, int degree) {
	Point2f center(src.cols / 2.0 + 0.5, src.rows / 2.0 + 0.5);
	Mat M = getRotationMatrix2D(center, degree, 1.0);
	warpAffine(src, dst, M, src.size());
}

String go_detect(Mat &src) {
	Mat src_gray;
	thres_rgb(src, src_gray);

	Mat noise;
	k_means_spilter(src, noise);

	Mat line;
	check_is_line(src_gray, line);

	delete_target(src_gray, noise, line);

	delete_point(src_gray);

	Mat src_resize;
	resize(src_gray, src_resize, Size(src_gray.cols * 2, src_gray.rows * 2));
	//imshow("src_resize", src_resize);

	vector<Mat> num_roi = find_number(src_resize);

	String answer = dect_number(num_roi);

	return answer;
}


void thres_rgb(Mat src, Mat &gray) {

	gray = Mat::zeros(src.size(), CV_8U);

	//	設定閥值, 過濾背景雜訊
	int thres = 150;
	double gamma = 2.5;

	for (int row = 0; row < src.rows; row++) {
		uchar *rgb = src.ptr<uchar>(row);
		uchar *g_ptr = gray.ptr<uchar>(row);
		for (int col = 0; col < src.cols * 3; col += 3) {
			int color_sum = rgb[col] + rgb[col + 1] + rgb[col + 2];

			if (color_sum / 3 > thres) {
				rgb[col] = 255;
				rgb[col + 1] = 255;
				rgb[col + 2] = 255;

				g_ptr[col / 3] = 0;
			}
			else {
				//	gamma 校正
				for (int channel = 0; channel < 3; channel++) {
					double tmp = rgb[col + channel];
					tmp = pow(tmp / 255, gamma) * 255;

					if (tmp < 0)
						rgb[col + channel] = 0;
					else if (tmp > 255)
						rgb[col + channel] = 255;
					else
						rgb[col + channel] = tmp;
				}

				g_ptr[col / 3] = 255;
			}
		}
	}

}

void delete_target(Mat src, Mat dst_0, Mat dst_1) {

	//	刪除 src 當 dst_0 && dst_1 時
	for (int row = 0; row < src.rows; row++) {
		uchar *s_ptr = src.ptr<uchar>(row);
		uchar *d0_ptr = dst_0.ptr<uchar>(row);
		uchar *d1_ptr = dst_1.ptr<uchar>(row);
		for (int col = 0; col < src.cols; col++) {

			if (d0_ptr[col] == 255 && d1_ptr[col] == 255)
				s_ptr[col] = 0;
		}
	}

	//namedWindow("delete_result", WINDOW_NORMAL);
	//imshow("delete_result", src);
}

void delete_point(Mat src) {

	//	刪除單一雜點
	for (int row = 0; row < src.rows; row++) {
		for (int col = 0; col < src.cols; col++) {
			if (src.at<uchar>(row, col) == 255) {
				int count = 0;

				if (row == 0 || src.at<uchar>(row - 1, col) == 0)
					count++;
				if (row == src.rows - 1 || src.at<uchar>(row + 1, col) == 0)
					count++;
				if (col == 0 || src.at<uchar>(row, col - 1) == 0)
					count++;
				if (col == src.cols - 1 || src.at<uchar>(row, col + 1) == 0)
					count++;

				if (count == 4)
					src.at<uchar>(row, col) = 0;
			}
		}
	}

}

void k_means_spilter(Mat src, Mat &dst) {
	int width = src.cols;
	int height = src.rows;
	int dims = src.channels();

	//	分類數目
	int clusterCount = 3;

	Mat points(width*height, dims, CV_32F, Scalar(0));
	Mat centers;
	Mat labels;

	//	將圖片資料存入 points
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			int index = row * width + col;
			Vec3b rgb = src.at<Vec3b>(row, col);
			points.at<float>(index, 0) = static_cast<int>(rgb[0]);
			points.at<float>(index, 1) = static_cast<int>(rgb[1]);
			points.at<float>(index, 2) = static_cast<int>(rgb[2]);
		}
	}

	//	knn 運算
	TermCriteria criteria = TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 0.1);
	kmeans(points, clusterCount, labels, criteria, 3, KMEANS_PP_CENTERS, centers);

	//	依照顏色濃度分辨前景, 背景
	int backgroung_index, line_index, num_index;
	int temp[3] = { centers.at<float>(0, 0) + centers.at<float>(0, 1) + centers.at<float>(0, 2),
		centers.at<float>(1, 0) + centers.at<float>(1, 1) + centers.at<float>(1, 2),
		centers.at<float>(2, 0) + centers.at<float>(2, 1) + centers.at<float>(2, 2) };

	backgroung_index = distance(temp, max_element(temp, temp + 3));
	temp[backgroung_index] = 0;

	line_index = distance(temp, max_element(temp, temp + 3));
	temp[line_index] = 0;

	num_index = distance(temp, max_element(temp, temp + 3));

	//	將分類結果寫入 src, result
	dst = Mat::zeros(src.size(), CV_8U);
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			int index = row*width + col;
			int label = labels.at<int>(index, 0);

			/*
			src.at<Vec3b>(row, col)[0] = centers.at<float>(label, 0);
			src.at<Vec3b>(row, col)[1] = centers.at<float>(label, 1);
			src.at<Vec3b>(row, col)[2] = centers.at<float>(label, 2);
			*/
			/*
			switch (label) {
				case 0:
				src.at<Vec3b>(row, col)[0] = 255;
				src.at<Vec3b>(row, col)[1] = 255;
				src.at<Vec3b>(row, col)[2] = 255;
				break;

				case 1:
				src.at<Vec3b>(row, col)[0] = 255;
				src.at<Vec3b>(row, col)[1] = 0;
				src.at<Vec3b>(row, col)[2] = 0;
				break;

				case 2:
				src.at<Vec3b>(row, col)[0] = 0;
				src.at<Vec3b>(row, col)[1] = 255;
				src.at<Vec3b>(row, col)[2] = 0;
				break;
			}
			*/

			if (label == line_index)
				dst.at<uchar>(row, col) = 255;
		}
	}
	/*
	namedWindow("kmeans_spilt", WINDOW_NORMAL);
	imshow("kmeans_spilt", dst);
	*/
	/*
	namedWindow("kmeans_result", WINDOW_NORMAL);
	imshow("kmeans_result", src);
	*/
}

void check_is_line(Mat src, Mat &dst) {

	dst = Mat::zeros(src.size(), CV_8U);

	int min_length = 3;

	//	檢查是否為垂直線
	for (int row = 0; row < src.rows; row++) {
		for (int col = 0; col < src.cols; col++) {
			if (src.at<uchar>(row, col) == 0)
				continue;

			//	檢查左右是否都是黑色
			bool left_black = false, right_black = false;

			if (col == 0 || src.at<uchar>(row, col - 1) == 0)
				left_black = true;

			if (col == src.cols - 1 || src.at<uchar>(row, col + 1) == 0)
				right_black = true;

			if (!left_black || !right_black)
				continue;

			//	檢查直線長度
			int length = col_length(src, row, col);
			if (length > min_length)
				dst.at<uchar>(row, col) = 255;
		}
	}


	//	檢查是否為水平線
	for (int row = 0; row < src.rows; row++) {
		for (int col = 0; col < src.cols; col++) {
			if (src.at<uchar>(row, col) == 0)
				continue;

			//	檢查上下是否都是黑色
			bool up_black = false, down_black = false;

			if (row == 0 || src.at<uchar>(row - 1, col) == 0)
				up_black = true;

			if (row == src.rows - 1 || src.at<uchar>(row + 1, col) == 0)
				down_black = true;

			if (!up_black || !down_black)
				continue;

			//	檢查直線長度
			int length = row_length(src, row, col);
			if (length > min_length)
				dst.at<uchar>(row, col) = 255;
		}
	}
	/*
	namedWindow("check_result", WINDOW_NORMAL);
	imshow("check_result", dst);
	*/
}

int col_length(Mat src, int dst_row, int dst_col) {
	int length = 0;
	for (int row = dst_row; row >= 0; row--) {
		if (src.at<uchar>(row, dst_col) == 255)
			length++;
		else
			break;
	}

	for (int row = dst_row + 1; row < src.rows; row++) {
		if (src.at<uchar>(row, dst_col) == 255)
			length++;
		else
			break;
	}

	//cout << length << endl;
	return length;
}

int row_length(Mat src, int dst_row, int dst_col) {
	int length = 0;
	for (int col = dst_col; col >= 0; col--) {
		if (src.at<uchar>(dst_row, col) == 255)
			length++;
		else
			break;
	}

	for (int col = dst_col + 1; col < src.cols; col++) {
		if (src.at<uchar>(dst_row, col) == 255)
			length++;
		else
			break;
	}

	//cout << length << endl;
	return length;
}

vector<Mat> find_number(Mat src) {
	Mat src_tmp = src.clone();

	//	膨脹
	dilate(src_tmp, src_tmp, Mat());

	Mat edge;
	blur(src_tmp, src_tmp, Size(3, 3));
	Canny(src_tmp, edge, 50, 150, 3);

	//imshow("edge", edge);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	//	尋找邊緣
	findContours(edge, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//cout << contours.size() << endl;

	//	描繪邊緣
	//for (int i = 0; i<contours.size(); i++)
		//drawContours(src_tmp, contours, i, Scalar(128), 2, 8, hierarchy);
	


	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect;

	//  透過邊緣, 找出一定大小之範圍
	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		Rect rect = boundingRect(Mat(contours_poly[i]));

		//if (rect.area() > 300)
			//cout << "h : " << rect.height << ", w : " << rect.width << ", aera :  " << rect.area() << endl;

		if (rect.height >= 21 && rect.width >= 21 && rect.area() >= 700)
			boundRect.push_back(rect);
	}

	//	將該範圍標註起來
	//for (int i = 0; i< boundRect.size(); i++)
		//rectangle(src_tmp, boundRect[i].tl(), boundRect[i].br(), Scalar(128), 2, 8, 0);
	

	sort(boundRect.begin(), boundRect.end(), rect_cmp_by_center);

	//	將標註座標, 套用在原圖
	vector<Mat> numRoi(boundRect.size());
	for (int i = 0; i < boundRect.size(); i++) {
		numRoi[i] = src(boundRect[i]);

		//String str = "num_" + to_string(i);
		//imshow(str, numRoi[i]);
	}


	//namedWindow("find_number_result", WINDOW_NORMAL);
	//imshow("find_number_result", src_tmp);

	return numRoi;
}

bool rect_cmp_by_center(Rect a, Rect b) {
	int center_a = a.x + a.width / 2;
	int center_b = b.x + b.width / 2;

	return center_a < center_b;
}

String dect_number(vector<Mat> src) {
	String answer = "";

	//	比對出相似度最高之圖片
	for (int num = 0; num < src.size(); num++) {
		Mat numRoi = src[num];

		Mat zoomNum(numRoi.rows * 2, numRoi.cols * 2, CV_8UC1, Scalar(0));
		numRoi.copyTo(zoomNum(Rect(numRoi.cols / 2, numRoi.rows / 2, numRoi.cols, numRoi.rows)));

		double matchMin = DBL_MAX;
		int matchSample = -1;

		for (int i = 0; i < sampleMat.size(); i++) {
			Mat rotateImg = sampleMat[i];

			int result_cols = zoomNum.cols - rotateImg.cols + 1;
			int result_rows = zoomNum.rows - rotateImg.rows + 1;
			Mat resultImg(result_rows, result_cols, CV_32FC1);

			matchTemplate(zoomNum, rotateImg, resultImg, CV_TM_SQDIFF);

			double minVal;
			Point minLoc;
			minMaxLoc(resultImg, &minVal, NULL, &minLoc, NULL, Mat());

			if (matchMin > minVal) {
				matchMin = minVal;
				matchSample = i/37;
			}
		}

		answer = answer + to_string(matchSample / 2);
		//cout << "num_" << to_string(num) << "\tmatch sample : " << to_string(matchSample)
			 //<< "\tmatch value : " << matchMin << endl;

	}
	cout << "answer is : " << answer << endl;

	return answer;
}
