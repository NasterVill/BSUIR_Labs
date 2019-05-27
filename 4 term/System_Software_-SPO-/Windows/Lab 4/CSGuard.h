#pragma once
#include <windows.h>

class CSGuard
{
public:
	void LeaveSection();
	void EnterSection();

	CSGuard(LPCRITICAL_SECTION criticalSectionPointer);
	~CSGuard();
	CSGuard(const CSGuard&) = delete;
	CSGuard& operator=(const CSGuard&) = delete;
private:
	LPCRITICAL_SECTION criticalSectionPointer_;
	bool in_;
};
