#include "stdafx.h"
#include "SogouPlugIn.h"

CSogouPlugIn::CSogouPlugIn()
{

}

CSogouPlugIn::~CSogouPlugIn()
{

}

BOOL CSogouPlugIn::IsWorked()
{
	return TRUE;
}

int32 CSogouPlugIn::GetPluginVersion()
{
	return 1;
}

const wchar_t* CSogouPlugIn::GetBrowserName()
{
	return L"Sogou";
}

const wchar_t* CSogouPlugIn::GetInstallPath()
{
	return NULL;
}

const wchar_t* CSogouPlugIn::GetFavoriteDataPath()
{
	return NULL;
}

const wchar_t* CSogouPlugIn::GetHistoryDataPath()
{
	return NULL;
}

BOOL CSogouPlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	return TRUE;
}

BOOL CSogouPlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	return TRUE;
}

BOOL CSogouPlugIn::ImportFavoriteData( FAVORITELINEDATA stData )
{
	return TRUE;
}
