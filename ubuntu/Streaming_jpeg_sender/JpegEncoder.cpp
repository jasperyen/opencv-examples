
#include "JpegEncoder.h"

using namespace std;
using namespace cv;

JpegEncoder::JpegEncoder(CaptureThread &cap, const bool show, const int quality, const float scale) {

	capture = &cap;

	showCapture = show;

	if (showCapture)
		namedWindow("Sender capture", WINDOW_AUTOSIZE);


	originalSize = capture->getFrameSize();

	if (scale < 1.0 && scale > 0.0)
		scaleSize = Size((int)(originalSize.width * scale), (int)(originalSize.height * scale));
	else
		scaleSize = originalSize;


	compression_params = {
		IMWRITE_JPEG_QUALITY, quality
	};
}

bool JpegEncoder::getJpegData(vector<unsigned char> &data) {
	Mat frame;
	
	if (capture->isCapturing()) {
		while (!capture->getFrame(frame)) {
			this_thread::sleep_for(chrono::duration<int, std::milli>(5));
			//cout << "wait frame" << endl;
		}
	}
	else
		return false;

	if (showCapture) {
		imshow("Sender capture", frame);
		waitKey(1);
	}

	if (scaleSize != originalSize) 
		resize(frame, frame, scaleSize);


	imencode(".jpg", frame, data, compression_params);
	

	return true;
}

bool JpegEncoder::getJpegPackage(vector<unsigned char> &data) {

	if (!getJpegData(data))
		return false;


	for (int i = 0; i < sizeof(unsigned int); i++)
		data.insert(data.begin(), 0);
	

	unsigned int i[1];
	i[0] = data.size();
	unsigned char *ptr = data.data();

	memcpy(ptr, i, sizeof(unsigned int));

	return true;
}

