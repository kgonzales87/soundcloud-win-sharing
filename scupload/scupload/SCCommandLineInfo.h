// Copyright (c) 2012, SoundCloud Ltd.

#pragma once
#include "afxcoll.h"

// SC specific command line argument parser,
// handling individual track properties
class SCCommandLineInfo : public CCommandLineInfo
{
public:
	SCCommandLineInfo(LPTSTR args);

	CString m_sAccessToken;
	CString m_sClientId;
	CString m_sAssetData;
	CString m_sArtworkData;
	CMapStringToString m_mTrackProperties;

	void ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);
	bool IsValid(void);
private:
	CString m_sArgs;
	static const CString SCCommandLineInfo::OAUTH_TOKEN;
	static const CString SCCommandLineInfo::ASSET_DATA;
	static const CString SCCommandLineInfo::ARTWORK_DATA;
	static const CString SCCommandLineInfo::CLIENT_ID;
};