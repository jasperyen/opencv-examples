#include "CascadeThread.h"


CascadeThread::CascadeThread(CaptureThread &capture, queue<Mat> &result_queue, mutex &queue_mutex) {
	this->capture = &capture;
	this->result_queue = &result_queue;
	this->queue_mutex = &queue_mutex;

	if (!cascade.load(cascade_name)) {
		cout << "Failed to load cascade classifier" << endl;
		exit(EXIT_FAILURE);
	}
}

CascadeThread::~CascadeThread() {
	keepRuning = false;
	cascade_thread->join();

}

void CascadeThread::start_thread() {
	cascade_thread = new thread(&CascadeThread::goCascade, this);
}

void CascadeThread::goCascade() {

	Mat frame, frame_gray;
	vector<Rect> rect;
	unique_lock<mutex> locker(*queue_mutex, defer_lock);
	keepRuning = true;

	while (keepRuning) {
		while (!capture->getFrame(frame)) {
			this_thread::sleep_for(chrono::duration<int, std::milli>(5));
			cout << "wait frame" << endl;
		}

		cout << "Get work in thread id : " << this_thread::get_id() << endl;


		cvtColor(frame, frame_gray, CV_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		cascade.detectMultiScale(
			frame_gray, rect, 1.25, 2, 0, Size(50, 50), Size(100, 100));

		for (size_t i = 0; i < rect.size(); i++)
			rectangle(frame, rect[i], Scalar(0, 0, 255), 2);

		//this_thread::sleep_for(chrono::duration<int, std::milli>(30));

		locker.lock();
		result_queue->push(frame);
		locker.unlock();
	}

}
