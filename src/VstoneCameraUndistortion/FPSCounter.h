#pragma once

class FPSCounter
{
public:
	FPSCounter(void);
	virtual ~FPSCounter(void);

	void clear();
	void check();

protected:
	DWORD st_;
	int count_;

	float fps_;
	float t_;
	int check_count_;
};

