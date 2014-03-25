#include "stdafx.h"
#include "VstoneCameraUndistortion.h"
#include "FPSCounter.h"

// undistortion parameters...
int center_x  = 357;
int center_y  = 264;
int r1        = 52;
int r2        = 240;
int offset_th = 0;

int main(int argc, char* argv[])
{
	cv::Mat src, dst, canvas;
	VstoneCameraUndistortion vstone_camera;
	FPSCounter fps;

	// load source image
	src = cv::imread("test.png");

	// create result image
	dst.create(cv::Size(960, 240), CV_8UC3);

	// set input & output image size
	vstone_camera.input_size(src.size());
	vstone_camera.output_size(dst.size());

	// setup gui
	cv::namedWindow("gui");
	cv::createTrackbar("center_x", "gui", &center_x, src.cols);
	cv::createTrackbar("center_y", "gui", &center_y, src.rows);
	cv::createTrackbar("r1", "gui", &r1, src.rows/2);
	cv::createTrackbar("r2", "gui", &r2, src.cols/2);
	cv::createTrackbar("offset_th", "gui", &offset_th, 360);

	while(true) {
		// set parameters
		vstone_camera.center(cv::Point(center_x, center_y));
		vstone_camera.r1((float)r1);
		vstone_camera.r2((float)r2);
		vstone_camera.offset_th(offset_th / 180.0f * (float)M_PI);

		// undistortion
		vstone_camera.undistortion(src, dst);

		// for debug draw
		src.copyTo(canvas);
		vstone_camera.debug_draw(canvas);

		cv::imshow("src", canvas);
		cv::imshow("dst", dst);

		int c = cv::waitKey(1);
		if (c == 27) break;

		fps.check();
	}
	cv::destroyAllWindows();

	return 0;
}