/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package captchadetection;

import static java.lang.System.out;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import org.opencv.core.Core;
import org.opencv.core.CvException;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.core.TermCriteria;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;

/**
 *
 * @author Jasper
 */
public class CaptchaDetection {

    private static final String SAMPLES_PATH = "number_samples\\";
    
    private static final Map<Integer, List<Mat>> sampleMat = new TreeMap<>();
    private static final rect_cmp_by_center rectSort = new rect_cmp_by_center();
    
    private static boolean INIT_SUCCESS = false; 
    
    static {
        try {
            System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
            
            for (int i = 0; i < 20; i++) {
                Mat sample = Imgcodecs.imread(SAMPLES_PATH + i + ".png", Imgcodecs.IMREAD_GRAYSCALE);

                List<Mat> rotateList = new ArrayList<>();
                for (int rota = -90; rota <= 90; rota += 5) {
                    Mat rotateImg = sample.clone();
                    rotateImage(rotateImg, rotateImg, rota);
                    rotateList.add(rotateImg);
                }
                sampleMat.put(i, rotateList);
            }
            
            INIT_SUCCESS = true;
            out.println("載入驗證碼樣本成功 ! ");
        }catch (UnsatisfiedLinkError | CvException ex) {
            out.println("Initialization failed in case :" + ex.toString());
        }
    }

    
    public static void main(String[] args) {
        if (!INIT_SUCCESS)
            return;
        
        String filename = "..\\..\\test_data\\captcha_data\\01.jpg";
        goDetect(filename);
    }
    
    public static String goDetect(String filename) throws CvException{
        final Mat src = Imgcodecs.imread(filename);
        
        Mat src_gray = thres_rgb(src);
        
        Mat noise = k_means_spilter(src);
        
        Mat line = check_is_line(src_gray);
        
        delete_target(src_gray, noise, line);
        
        delete_point(src_gray);
        
        Mat src_resize = new Mat();
        Imgproc.resize(src_gray, src_resize, new Size(src_gray.cols() * 2, src_gray.rows() * 2));
        
        
        List<Mat> num_rio = find_number(src_resize);
        
        String answer = dect_number(num_rio);
        
        return answer;
    }
    
    private static Mat thres_rgb(Mat src) {
        Mat gray = Mat.zeros(src.size(), CvType.CV_8UC1);
        
        //  設定閥值, 過濾背景雜訊
	int thres = 150;
	double gamma = 2.5;
        
        for (int row = 0; row < src.rows(); row++) {
            for (int col = 0; col < src.cols(); col++) {
                double[] s_data = src.get(row, col);
                
                byte[] s_buff = new byte[3]; 
                byte[] g_buff = new byte[1]; 
                
                double color_sum =  s_data[0] + s_data[1] + s_data[2];
                
                if (color_sum / 3 > thres) {
                    for (int channel = 0; channel < 3; channel++) 
                        s_buff[channel] = (byte) 255;
                    
                    g_buff[0] = 0;
                }
                else {
                    //	gamma 校正
                    for (int channel = 0; channel < 3; channel++) { 
                        double tmp = s_data[channel];
                        tmp = Math.pow(tmp / 255, gamma) * 255;
                        
                        if (tmp < 0)
                            s_buff[channel] = 0;
                        else if (tmp > 255)
                            s_buff[channel] = (byte) 255;
                        else
                            s_buff[channel] = (byte) tmp;
                    }
                    
                    g_buff[0] = (byte) 255;
                }
                src.put(row, col, s_buff);
                gray.put(row, col, g_buff);
            } 
	}
        return gray;
    }
    
    private static Mat k_means_spilter(Mat src) {
        Mat dst = Mat.zeros(src.size(), CvType.CV_8UC1);
        
        int width = src.cols();
	int height = src.rows();
	int dims = src.channels();
        
        //	分類數目
	int clusterCount = 3;

	Mat points = new Mat(width*height, dims, CvType.CV_32F, new Scalar(0));
	Mat centers = new Mat(clusterCount, dims, CvType.CV_32F);
	Mat labels = new Mat(width*height, 1, CvType.CV_32S);
        
        //	將圖片資料存入 points
	for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                int index = row * width + col;
                double[] s_data = src.get(row, col);
                
                for (int channel = 0; channel < 3; channel++) {
                    float[] f_buff = new float[1];
                    f_buff[0] = (float) s_data[channel];
                    
                    points.put(index, channel, f_buff);
                }
            }
	}
        
        //  knn 運算
	TermCriteria criteria = new TermCriteria(TermCriteria.EPS + TermCriteria.MAX_ITER, 10, 0.1);
	Core.kmeans(points, clusterCount, labels, criteria, 3, Core.KMEANS_PP_CENTERS, centers);
        
        //  依照顏色濃度排序 label index
	Map<Integer, Integer> tmp = new TreeMap<>();
        for (int i = 0; i < clusterCount; i++) {
            int sum = 0;
            for (int j = 0; j < dims ; j++) {
                sum += centers.get(i, j)[0];
            }
            while (tmp.containsKey(sum))
                sum++;
            tmp.put(sum, i);
        }
        
        int count = 0;
        int[] label_order = new int[clusterCount];
        for (Map.Entry<Integer, Integer> iter : tmp.entrySet()) {
            label_order[count++] = iter.getValue();
        }
        
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                int index = row*width + col;
                int label = (int) labels.get(index, 0)[0];
                
                if (label == label_order[1]) {
                    byte[] d_buff = new byte[1]; 
                    d_buff[0] = (byte) 255;
                    dst.put(row, col, d_buff);
                }
            }
        }
        
        return dst;
    }   
    
    private static Mat check_is_line(Mat src) {
        Mat dst = Mat.zeros(src.size(), CvType.CV_8UC1);
        
        int min_length = 3;
        
        //  檢查是否為垂直線
	for (int row = 0; row < src.rows(); row++) {
            for (int col = 0; col < src.cols(); col++) {
                if (src.get(row, col)[0] == 0)
                    continue;
                
                //  檢查左右是否都是黑色
                boolean left_black = false, right_black = false;

                if (col == 0 || src.get(row, col - 1)[0] == 0)
                    left_black = true;
                
                if (col == src.cols() - 1 || src.get(row, col + 1)[0] == 0)
                    right_black = true;
                
                if (!left_black || !right_black)
                    continue;
                
                //	檢查直線長度
                int length = col_length(src, row, col);
                if (length > min_length) {
                    byte[] d_buff = new byte[1]; 
                    d_buff[0] = (byte) 255;
                    dst.put(row, col, d_buff);
                }
                    
            }
        }
        
        //  檢查是否為水平線
        for (int row = 0; row < src.rows(); row++) {
            for (int col = 0; col < src.cols(); col++) {
                if (src.get(row, col)[0] == 0)
                    continue;
         
                //  檢查上下是否都是黑色
                boolean up_black = false, down_black = false;

                if (row == 0 || src.get(row - 1, col)[0] == 0)
                    up_black = true;

                if (row == src.rows() - 1 || src.get(row + 1, col)[0] == 0)
                    down_black = true;

                if (!up_black || !down_black)
                    continue;
            
                //  檢查直線長度
                int length = row_length(src, row, col);
                if (length > min_length) {
                    byte[] d_buff = new byte[1]; 
                    d_buff[0] = (byte) 255;
                    dst.put(row, col, d_buff);
                }
            }
        }
        
        return dst;
    }
    
    private static int col_length(Mat src, int dst_row, int dst_col) {
        int length = 0;
        
        for (int row = dst_row; row >= 0; row--) {
            if (src.get(row, dst_col)[0] == 255)
                length++;
            else
                break;
	}
    
        for (int row = dst_row + 1; row < src.rows(); row++) {
            if (src.get(row, dst_col)[0] == 255)
                length++;
            else
                break;
	}
        
        //out.println(length);
        
        return length;
    }
    
    private static int row_length(Mat src, int dst_row, int dst_col) {
        
        int length = 0;
        
        for (int col = dst_col; col >= 0; col--) {
            if (src.get(dst_row, col)[0] == 255)
                length++;
            else
                break;
	}
        
        for (int col = dst_col + 1; col < src.cols(); col++) {
            if (src.get(dst_row, col)[0] == 255)
                length++;
            else
                break;
	}
        
        //out.println(length);
        
        return length;
    }
    
    /***
     * 刪除 src 當 dst_0 交集 dst_1
     * @param src
     * @param dst_0
     * @param dst_1 
     */
    private static void delete_target(Mat src, Mat dst_0, Mat dst_1) {
	for (int row = 0; row < src.rows(); row++) {
            for (int col = 0; col < src.cols(); col++) {
                if (dst_0.get(row, col)[0] == 255 && dst_1.get(row, col)[0] == 255) {
                    byte[] d_buff = new byte[1]; 
                    d_buff[0] = (byte) 0;
                    src.put(row, col, d_buff);
                }
            }
	}
    }
    
    /***
     * 刪除單一雜點
     * @param src 
     */
    private static void delete_point(Mat src) {
        for (int row = 0; row < src.rows(); row++) {
            for (int col = 0; col < src.cols(); col++) {
                if (src.get(row, col)[0] == 255) {
                    int count = 0;

                    if (row == 0 || src.get(row - 1, col)[0] == 0)
                        count++;
                    if (row == src.rows()-1 || src.get(row + 1, col)[0] == 0)
                        count++;
                    if (col == 0 || src.get(row, col - 1)[0] == 0)
                        count++;
                    if (col == src.cols()-1 || src.get(row, col + 1)[0] == 0)
                        count++;

                    if (count == 4){
                        byte[] d_buff = new byte[1]; 
                        d_buff[0] = (byte) 0;
                        src.put(row, col, d_buff);
                    }

                }   
            }
        }
    }
    
    /***
     * 依照圖像邊緣切割, 回傳ROI
     * @param src
     * @return 
     */
    private static List<Mat> find_number(Mat src) {
        Mat src_tmp = src.clone();
        
        //  膨脹
	Imgproc.dilate(src_tmp, src_tmp, new Mat());
        
        //  邊緣偵測
        Mat canny_edge = new Mat();
	Imgproc.blur(src_tmp, src_tmp, new Size(3, 3));
	Imgproc.Canny(src_tmp, canny_edge, 50, 150, 3, false);
        
        //  尋找邊緣
	List<MatOfPoint> contours = new ArrayList<>();
        Imgproc.findContours(canny_edge, contours, new Mat(), Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_SIMPLE);

        List<Rect> boundRect = new ArrayList<>();
        
        //  透過邊緣, 找出一定大小之範圍
	for (int i = 0; i < contours.size(); i++) {
            MatOfPoint2f tmp_mp2f_1 = new MatOfPoint2f();
            MatOfPoint2f tmp_mp2f_2 = new MatOfPoint2f();
            
            contours.get(i).convertTo(tmp_mp2f_1, CvType.CV_32FC2);
            
            Imgproc.approxPolyDP(tmp_mp2f_1, tmp_mp2f_2, 3, true);
            
            tmp_mp2f_2.convertTo(contours.get(i), CvType.CV_32S);
            
            Rect rect = Imgproc.boundingRect(contours.get(i));
            
            //if (rect.area() > 300)
                //out.println("h : " + rect.height + ", w : " + rect.width + ", aera :  " + rect.area());
            
            if (rect.height >= 21 && rect.width >= 21 && rect.area() >= 700)
                boundRect.add(rect);
        }
        
        //  將該範圍標住起來
        for (Rect rect : boundRect) {
            Scalar color = new Scalar(128);
            Imgproc.rectangle(src_tmp, rect.tl(), rect.br(), color, 2, 8, 0);
        }
        
        //  依中心點由左到右排序
        Collections.sort(boundRect, rectSort);
        
        List<Mat> numRoi = new ArrayList<>();
        for (Rect rect : boundRect) 
            numRoi.add(src.submat(rect));
        
        //for (Mat roi : numRoi) 
            //showResult(roi, "roi");
        
        return numRoi;
    }
    
    /***
     * 旋轉圖像
     * @param src
     * @param dst
     * @param degree 
     */
    private static void rotateImage(Mat src, Mat dst, int degree) { 
        Point center = new Point(src.cols() / 2.0 + 0.5, src.rows() / 2.0 + 0.5);
        Mat M = Imgproc.getRotationMatrix2D(center, degree, 1.0);
        Imgproc.warpAffine(src, dst, M, src.size());
    }
    
    /***
     * 依照樣本辨識數字
     * @param src
     * @return 
     */
    private static String dect_number(List<Mat> src) {
        String answer = "";
        
        for (Mat numRoi : src) {
            Mat zoomNum = new Mat(numRoi.rows() * 2, numRoi.cols() * 2, CvType.CV_8UC1, new Scalar(0));
            numRoi.copyTo( zoomNum.submat(new Rect(numRoi.cols() / 2, numRoi.rows() / 2, numRoi.cols(), numRoi.rows())) );
            
            double matchMin = Double.MAX_VALUE;
            int matchSample = 0;
            
            for (Map.Entry<Integer, List<Mat>> iter : sampleMat.entrySet()) {
                for (Mat sample : iter.getValue()) {
                    int result_cols = zoomNum.cols() - sample.cols() + 1;
                    int result_rows = zoomNum.rows() - sample.rows() + 1;
                    
                    Mat resultImg = new Mat(result_rows, result_cols, CvType.CV_32FC1);
                    
                    Imgproc.matchTemplate(zoomNum, sample, resultImg, Imgproc.TM_SQDIFF);

                    Core.MinMaxLocResult mmr = Core.minMaxLoc(resultImg);
                    
                    if (matchMin > mmr.minVal) {
                        matchMin = mmr.minVal;
                        matchSample = iter.getKey();
                    }
                }
            }
            answer += matchSample/2;
            out.println("NumRio\tmatch sample :  " + matchSample + "\tmatch value : " + matchMin);
        }
        
        out.println("Answer is : " + answer);
        return answer;
    }
    
    /***
     * Rect排序方法
     */
    private static class rect_cmp_by_center implements Comparator<Rect> {
        @Override
        public int compare(Rect a, Rect b) {
            int center_a = a.x + a.width / 2;
            int center_b = b.x + b.width / 2;
            
            return center_a - center_b;
        }
    }
    
}
