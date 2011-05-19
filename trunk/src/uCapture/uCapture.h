#ifdef UCAPTURE_EXPORTS
#define UCAPTURE_API __declspec(dllexport)
#else
#define UCAPTURE_API __declspec(dllimport)
#endif

extern "C"
{
UCAPTURE_API HRESULT Capture(HWND hWnd, LPTSTR szUrl);
}
