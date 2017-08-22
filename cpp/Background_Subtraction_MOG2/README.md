# BackgroundSubtractorMOG2

- ### Chia-Chen's [GitHub](https://github.com/jasperyen) [HackMD.io](https://hackmd.io/EYNg7ALAhgrAjATgLQQCYDM4pAZgAxIJh7BIBMEeeZCIUeApgBxxA===) 

### 概要

這次要記錄的是 OpenCV 裡的 **BackgroundSubtractorMOG2** 演算法, 簡單的來說, 就是藉由前景與背景的相減, 計算出改變或者移動中的圖像, 除此之外還能透過學習新的影像, 不斷的改變背景。

在程式中, 主要透過 MOG2 演算法計算出移動圖像, 配合高斯模糊、二值化等等, 去除風吹草動的細微雜訊, 並且設定移動比率的閥值, 藉此實現簡易的移動偵測。

**輸入**

![](https://i.imgur.com/UNvpKJk.jpg)


**輸出**

![](https://i.imgur.com/ArjtAaz.png)



>參考資料
>1. [How to Use Background Subtraction Methods](http://docs.opencv.org/trunk/d1/dc5/tutorial_background_subtraction.html)
>2. [Background Subtraction Algorithm using OpenCV](https://ccw1986.blogspot.tw/2016/03/background-subtraction-algorithm-using.html)


* 完整程式碼 (Sample Code) : [GitHub](https://github.com/jasperyen/opencv-examples/blob/master/cpp/Background_Subtraction_MOG2/Background_Subtraction_MOG2/main.cpp)

---

#### BackgroundSubtractorMOG2

>建立演算法
>
>- history : 最多參考前幾張影像當作背景
>- varThreshold : 更新閥值 (兩像素的距離)
>- detectShadows : 是否偵測陰影
```
pPtr<BackgroundSubtractorMOG2> cv::createBackgroundSubtractorMOG2(
	int history = 500,
	double varThreshold = 16, 
	bool detectShadows = true
)
```

>執行MOG2計算
>
>- image : 當下欲計算的影像
>- fgmask : 輸出影像
>- learningRate : 學習速率 ( -1 : 自動), ( 0 ~ 1 : 不更新背景 ~ 完全取代背景 )
```
virtual void cv::BackgroundSubtractorMOG2::apply (
	InputArray image, 
	OutputArray fgmask, 
	double learningRate = -1
)
```
