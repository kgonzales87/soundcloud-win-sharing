// Copyright (c) 2012, SoundCloud Ltd.

#include "stdafx.h"
#include "SCCommandLineInfo.h"

using namespace boost;

const CString SCCommandLineInfo::ASSET_DATA = _T("track[asset_data]:");
const CString SCCommandLineInfo::ARTWORK_DATA = _T("track[artwork_data]:");
const CString SCCommandLineInfo::OAUTH_TOKEN = _T("oauth_token:");
const CString SCCommandLineInfo::CLIENT_ID = _T("client_id:");

SCCommandLineInfo::SCCommandLineInfo(LPTSTR args)
{
	m_sArgs = CString(args);
}

void SCCommandLineInfo::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	if(bFlag)
	{
		CString sParam(lpszParam);
		if (sParam.Left(OAUTH_TOKEN.GetLength()) == OAUTH_TOKEN)
		{
			m_sAccessToken = sParam.Right(sParam.GetLength() - OAUTH_TOKEN.GetLength());
			return;
		}
		else if (sParam.Left(CLIENT_ID.GetLength()) == CLIENT_ID)
		{
			m_sClientId = sParam.Right(sParam.GetLength() - CLIENT_ID.GetLength());
			return;
		}
		else if (sParam.Left(ASSET_DATA.GetLength()) == ASSET_DATA)
		{
			m_sAssetData = sParam.Right(sParam.GetLength() - ASSET_DATA.GetLength());
			return;
		}
		else if (sParam.Left(ARTWORK_DATA.GetLength()) == ARTWORK_DATA)
		{
			m_sArtworkData = sParam.Right(sParam.GetLength() - ARTWORK_DATA.GetLength());
			return;
		}
		else
		{
			// Parsing all track properties
			tregex propertyPattern(_T("^(track\\[.+?]):(.*)"));
			tmatch match;
			if(regex_match(sParam, match, propertyPattern))
			{
				CString propertyName = CString(match[1].first, match.length(1));
				m_mTrackProperties[propertyName] = CString(match[2].first, match.length(2));
			}
		}
	}
	else
	{
		// check if this is a valid asset file parameter
		//CFileStatus status;
		//if(CFile::GetStatus(lpszParam, status))
		//   if((status.m_attribute & CFile::Attribute::directory) == 0)
				m_sAssetData = CString(lpszParam);
	}

	// Call the base class to ensure proper command line processing
	CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
}

bool SCCommandLineInfo::IsValid(void)
{
	return !m_sAssetData.IsEmpty() && !m_sClientId.IsEmpty();
}
