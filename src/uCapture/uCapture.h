#ifdef UCAPTURE_EXPORTS
#define UCAPTURE_API __declspec(dllexport)
#else
#define UCAPTURE_API __declspec(dllimport)
#endif

extern "C"
{
typedef void (*pfnOnCaptureFinish)(HBITMAP hBitmap);
typedef void (*pfnQueryCaptureSize)(long *pnWidth, long *pnHeight);

typedef struct CaptureCallBack
{
	pfnOnCaptureFinish  fn1;
	pfnQueryCaptureSize fn2;
}CAPTURECALLBACK;

UCAPTURE_API HRESULT Capture(HWND hWnd, LPTSTR szUrl, CAPTURECALLBACK *cb);
}
