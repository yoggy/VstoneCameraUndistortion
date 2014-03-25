#include "StdAfx.h"
#include "VstoneCameraUndistortion.h"


VstoneCameraUndistortionParam::VstoneCameraUndistortionParam()
	: input_size_(640, 480), output_size_(720, 360),
	center_(320, 240), r1_(100.0f), r2_(200.0f), offset_th_(0.0f)
{

}

VstoneCameraUndistortionParam::~VstoneCameraUndistortionParam()
{

}

void VstoneCameraUndistortionParam::copyParams(VstoneCameraUndistortionParam &dst) const
{
	dst.input_size(input_size());
	dst.output_size(output_size());
	dst.center(center());
	dst.r1(r1());
	dst.r2(r2());
	dst.offset_th(offset_th());
}

cv::Point VstoneCameraUndistortionParam::center() const
{
	return center_;
}

void VstoneCameraUndistortionParam::center(const cv::Point &val)
{
	center_ = val;
}

cv::Size VstoneCameraUndistortionParam::input_size() const
{
	return input_size_;
}

void VstoneCameraUndistortionParam::input_size(const cv::Size &val)
{
	input_size_ = val;
}

cv::Size VstoneCameraUndistortionParam::output_size() const
{
	return output_size_;
}

void VstoneCameraUndistortionParam::output_size(const cv::Size &val)
{
	output_size_ = val;
}

float VstoneCameraUndistortionParam::r1() const
{
	return r1_;
}

void VstoneCameraUndistortionParam::r1(const float &val)
{
	if (val <= 0.0f) return;
	r1_ = val;
}

float VstoneCameraUndistortionParam::r2() const
{
	return r2_;
}

void VstoneCameraUndistortionParam::r2(const float &val)
{
	if (val <= 0.0f) return;
	r2_ = val;
}

float VstoneCameraUndistortionParam::offset_th() const
{
	return offset_th_;
}

void VstoneCameraUndistortionParam::offset_th(const float &val)
{
	offset_th_ = val;
}

cv::Point2f VstoneCameraUndistortionParam::convert_polar_(const cv::Point &src) const
{
	// normalized coordinate
	cv::Point2f n;
	n.x = src.x / (float)output_size_.width;  //[0.0, 1.0]
	n.y = src.y / (float)output_size_.height; //[0.0, 1.0]

	// polar coordinate
	float r_range = r2_ - r1_;
	float r =  r1_ + r_range * (1.0f - n.y);  // [r1_, r2_]
	float th = (2 * (float)M_PI) * (1.0f - n.x) + offset_th_;     // [0,M_PI]

	//
	cv::Point2f result;
	result.x =  r * sin(th) + center_.x;
	result.y =  r * cos(th) + center_.y;

	return result;
}

cv::Point2f VstoneCameraUndistortionParam::convert_polar_(const int &x, const int &y) const
{
	return convert_polar_(cv::Point(x, y));
}

VstoneCameraUndistortion::VstoneCameraUndistortion()
	: VstoneCameraUndistortionParam()
{
}

VstoneCameraUndistortion::~VstoneCameraUndistortion()
{

}

bool VstoneCameraUndistortion::undistortion(const cv::Mat &src, cv::Mat &dst)
{
	if (src.empty()) {
		printf("error : VstoneCameraUndistortion::undistortion() src image is null...\n");
		return false;
	}
	if (dst.empty()) {
		printf("error : VstoneCameraUndistortion::undistortion() dst image is null...\n");
		return false;
	}

	VstoneCameraUndistortionPLB body(src, dst);
	copyParams(body);
	cv::parallel_for_(cv::Range(0, dst.rows), body);

	return true;
}

bool VstoneCameraUndistortion::debug_draw(cv::Mat &canvas)
{
	if (canvas.empty()) {
		printf("error : VstoneCameraUndistortion::debug_draw() canvas image is null...\n");
		return false;
	}

	cv::circle(canvas, center_, 3, CV_RGB(0, 255, 0), CV_FILLED);
	cv::circle(canvas, center_, (int)r1_, CV_RGB(0, 255, 255), 2);
	cv::circle(canvas, center_, (int)r2_, CV_RGB(255, 255, 0), 2);

	cv::Point p1 = convert_polar_(0, 0);
	cv::Point p2 = convert_polar_(0, output_size_.height);
	cv::line(canvas, p1, p2, CV_RGB(0, 255, 0), 2);

	return true;
}

VstoneCameraUndistortionPLB::VstoneCameraUndistortionPLB(const cv::Mat &src, cv::Mat &dst)
	: ParallelLoopBody(), VstoneCameraUndistortionParam(), src_(src), dst_(dst)
{

}

void VstoneCameraUndistortionPLB::operator ()(const cv::Range& range) const
{
	cv::Mat patch;
	for (int y = range.start; y < range.end; ++y) {
		for (int x = 0; x < dst_.cols; ++x) {
			cv::Point2f src_p =convert_polar_ (x, y);
			cv::getRectSubPix(src_, cv::Size(1,1), src_p, patch);
			cv::Vec3b &color = patch.at<cv::Vec3b>(0,0);
			dst_.at<cv::Vec3b>(y, x) = color;
		}
	}
}
