#include "CaptureThread.h"


CaptureThread::CaptureThread(VideoCapture &cap) {

	spec = VIDEOCAPTURE;
	videocapture = &cap;

	if (!videocapture->isOpened()) {
		cout << "Could not open VideoCapture" << endl;
		exit(EXIT_FAILURE);
	}

	printLessCaptureSetting();
}

CaptureThread::CaptureThread(raspicam::RaspiCam_Cv &cap) {

	spec = RASPICAM;
	raspicam_cv = &cap;

	if (!raspicam_cv->isOpened()) {
		cout << "Could not open RaspiCam" << endl;
		exit(EXIT_FAILURE);
	}

	printLessCaptureSetting();
}


CaptureThread::~CaptureThread() {
	capturing = false;
	cap_thread->join();
}

void CaptureThread::printLessCaptureSetting() {
	switch (spec) {

		case VIDEOCAPTURE :
			cout << "CAP_PROP_FRAME_WIDTH" << "\t\t" << videocapture->get(CAP_PROP_FRAME_WIDTH) << endl;
			cout << "CAP_PROP_FRAME_HEIGHT" << "\t\t" << videocapture->get(CAP_PROP_FRAME_HEIGHT) << endl;
			cout << "CV_CAP_PROP_FPS" << "\t\t\t" << videocapture->get(CV_CAP_PROP_FPS) << endl;
			break;

		case RASPICAM :
			cout << "CAP_PROP_FRAME_WIDTH" << "\t\t" << raspicam_cv->get(CAP_PROP_FRAME_WIDTH) << endl;
			cout << "CAP_PROP_FRAME_HEIGHT" << "\t\t" << raspicam_cv->get(CAP_PROP_FRAME_HEIGHT) << endl;
			cout << "CV_CAP_PROP_FPS" << "\t\t\t" << raspicam_cv->get(CV_CAP_PROP_FPS) << endl;
			break;
	}
}

Size CaptureThread::getFrameSize() {
	Size frame_size;

	switch (spec) {
		case VIDEOCAPTURE :
			frame_size = Size(videocapture->get(CV_CAP_PROP_FRAME_WIDTH),
									videocapture->get(CV_CAP_PROP_FRAME_HEIGHT));
			break;

		case RASPICAM :
			frame_size = Size(raspicam_cv->get(CV_CAP_PROP_FRAME_WIDTH),
									raspicam_cv->get(CV_CAP_PROP_FRAME_HEIGHT));
			break;
	}

	return frame_size;
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

		switch (spec) {
			case VIDEOCAPTURE :
				*videocapture >> frame;
				break;

			case RASPICAM :
				raspicam_cv->grab();
				raspicam_cv->retrieve(frame);
				break;
		}

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
