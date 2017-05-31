#include "CaptureThread.h"


CaptureThread::CaptureThread(VideoCapture &cap) {

	capture = &cap;

	if (!capture->isOpened()) {
		cout << "Could not open VideoCapture" << endl;
		exit(EXIT_FAILURE);
	}

	printLessCaptureSetting();
}


CaptureThread::~CaptureThread() {
	capturing = false;
	cap_thread->join();
}

void CaptureThread::printLessCaptureSetting() {

	cout << "CAP_PROP_FRAME_WIDTH" << "\t\t" << capture->get(CAP_PROP_FRAME_WIDTH) << endl;
	cout << "CAP_PROP_FRAME_HEIGHT" << "\t\t" << capture->get(CAP_PROP_FRAME_HEIGHT) << endl;
	cout << "CV_CAP_PROP_FPS" << "\t\t\t" << capture->get(CV_CAP_PROP_FPS) << endl;

}

Size CaptureThread::getFrameSize() {
	return Size(capture->get(CV_CAP_PROP_FRAME_WIDTH),
		capture->get(CV_CAP_PROP_FRAME_HEIGHT));
}

void CaptureThread::startCapture() {
	capturing = true;
	cap_thread = new thread(&CaptureThread::goCapture, this);
}


void CaptureThread::goCapture() {
	Mat frame;

	double sec = 0;
	int frame_sum = 0;
	unique_lock<mutex> locker(queue_mutex, defer_lock);

	while (capturing) {
		double t = (double)getTickCount();

		*capture >> frame;

		sec += ((double)getTickCount() - t) / getTickFrequency();
		frame_sum++;
		if (sec > 1.0) {
			cout << "Cpaturing frame rate : " << frame_sum / sec << endl;
			sec = 0;
			frame_sum = 0;
		}

		locker.lock();

		while (frame_queue.size() > max_queue_size) {
			cout << "loss frame" << endl;
			frame_queue.pop();
		}

		frame_queue.push(frame);

		locker.unlock();
	}

}

bool CaptureThread::getFrame(Mat &frame) {
	unique_lock<mutex> locker(queue_mutex);

	if (frame_queue.empty())
		return false;

	frame = frame_queue.front();
	frame_queue.pop();

	return true;
}

bool CaptureThread::isCapturing() {
	return capturing;
}