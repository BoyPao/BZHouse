#include "BZLog.h"

using namespace std;

void LogWithInfo(const char* str, ...)
{
	char strBuffer[256];
	va_list(va);
	__crt_va_start(va, str);
	vsnprintf(strBuffer, 254, str, va);
	strBuffer[254] = '\0';
	strBuffer[255] = '\n';
#if IS_WINDOWS()
	cout << strBuffer << endl;
#endif

#if IS_LINUX()
	//need to develop
#endif
	__crt_va_end(va);
}