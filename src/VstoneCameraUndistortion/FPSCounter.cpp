#include "StdAfx.h"
#include "FPSCounter.h"

FPSCounter::FPSCounter()
{
	clear();
	check_count_ = 30;
}

FPSCounter::~FPSCounter()
{
}

void FPSCounter::clear()
{
	st_ = ::GetTickCount();
	count_ = 0;

	fps_ = 0.0f;
	t_ = 0.0f;
}

void FPSCounter::check() {
	count_ ++;
	if (count_ == check_count_) {
		float diff = (float)(::GetTickCount() - st_);
		t_ = diff / 1000.f / check_count_;
		fps_ = 1 / t_;

		printf("fps=%.2f, t=%.2f\n", fps_, t_);

		st_ = ::GetTickCount();
		count_ = 0;
	}
}
