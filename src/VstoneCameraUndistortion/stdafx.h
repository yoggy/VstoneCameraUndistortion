#pragma once

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>

// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#if _DEBUG
	#pragma comment(lib, "opencv_core248d.lib")
	#pragma comment(lib, "opencv_imgproc248d.lib")
	#pragma comment(lib, "opencv_highgui248d.lib")
#else
	#pragma comment(lib, "opencv_core248.lib")
	#pragma comment(lib, "opencv_imgproc248.lib")
	#pragma comment(lib, "opencv_highgui248.lib")
#endif
