#pragma once


class UTILITY_API ImageHelper
{
private:
	ImageHelper();
	~ImageHelper();
public:
	static HICON CreateIconFromBuffer( const LPBYTE lpBuf, unsigned int nSize, int nIconSize);
	static HICON Convert32x32IconTo16x16(HICON h32x32Icon);

};