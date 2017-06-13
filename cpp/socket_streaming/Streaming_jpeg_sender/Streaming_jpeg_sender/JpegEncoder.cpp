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

JpegEncoder::~JpegEncoder() {
	encoding = false;
	encode_thread->join();
}

void JpegEncoder::startJpegEncode() {
	encoding = true;
	encode_thread = new thread(&JpegEncoder::goEncode, this);
}

void JpegEncoder::goEncode() {
	Mat frame;
	vector<unsigned char> *data;

	double sec = 0;
	int frame_sum = 0;
	unique_lock<mutex> locker(queue_mutex, defer_lock);

	while (encoding) {
		while (!capture->getFrame(frame)) {
			this_thread::sleep_for(chrono::duration<int, std::milli>(5));
			//cout << "wait frame" << endl;
		}

		if (showCapture) {
			imshow("Sender capture", frame);
			waitKey(1);
		}

		double t = (double)getTickCount();

		encodeJpegPackage(frame, &data);

		sec += ((double)getTickCount() - t) / getTickFrequency();
		frame_sum++;
		if (sec > 1.0) {
			cout << "Encode jpeg rate : " << frame_sum / sec << endl;
			sec = 0;
			frame_sum = 0;
		}

		locker.lock();

		while (jpeg_queue.size() > max_queue_size) {
			cout << "loss jpeg package" << endl;
			vector<unsigned char> *loss_data = jpeg_queue.front();
			jpeg_queue.pop();
			delete loss_data;
		}

		//double s = (double)getTickCount();
		jpeg_queue.push(data);
		//cout << "push time" << ((double)getTickCount() - s) / getTickFrequency() << endl;

		locker.unlock();
	}

}

void JpegEncoder::encodeJpegPackage(Mat &frame, vector<unsigned char>  **data_ptr) {
	vector<unsigned char> *data = new vector<unsigned char>;

	if (scaleSize != originalSize)
		resize(frame, frame, scaleSize);

	imencode(".jpg", frame, *data, compression_params);


	for (int i = 0; i < sizeof(unsigned int); i++)
		data->insert(data->begin(), 0);

	unsigned int i[1];
	i[0] = data->size();
	unsigned char *ptr = data->data();

	memcpy(ptr, i, sizeof(unsigned int));

	*data_ptr = data;
}

bool JpegEncoder::getJpegPackage(vector<unsigned char> **data_ptr) {

	unique_lock<mutex> locker(queue_mutex);

	if (jpeg_queue.empty())
		return false;

	*data_ptr = jpeg_queue.front();
	jpeg_queue.pop();

	return true;
}

bool JpegEncoder::isEncoding() {
	return encoding;
}
