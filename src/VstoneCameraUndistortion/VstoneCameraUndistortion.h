#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <opencv2/core/core.hpp>

class VstoneCameraUndistortionParam {
public:
	VstoneCameraUndistortionParam();
	virtual ~VstoneCameraUndistortionParam();

	void copyParams(VstoneCameraUndistortionParam &dst) const;

	cv::Size input_size() const;
	void input_size(const cv::Size &val);

	cv::Size output_size() const;
	void output_size(const cv::Size &val);

	cv::Point center() const;
	void center(const cv::Point &val);

	float r1() const;
	void r1(const float &val);

	float r2() const;
	void r2(const float &val);

	float offset_th() const;
	void offset_th(const float &val);

	cv::Point2f convert_polar_(const cv::Point &src) const;
	cv::Point2f convert_polar_(const int &x, const int &y) const;

protected:
	cv::Size input_size_;
	cv::Size output_size_;
	cv::Point center_;
	float r1_;
	float r2_;
	float offset_th_;
};

class VstoneCameraUndistortion : public VstoneCameraUndistortionParam
{
public:
	VstoneCameraUndistortion();
	~VstoneCameraUndistortion();

	bool undistortion(const cv::Mat &src, cv::Mat &dst);
	bool debug_draw(cv::Mat &canvas);
};

class VstoneCameraUndistortionPLB : public cv::ParallelLoopBody, public VstoneCameraUndistortionParam
{
public:
	VstoneCameraUndistortionPLB(const cv::Mat &src, cv::Mat &dst);
	void operator ()(const cv::Range& range) const;

private:
	const cv::Mat &src_;
	cv::Mat &dst_;
};

