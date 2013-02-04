// Copyright (c) 2012, SoundCloud Ltd.

#include "stdafx.h"
#include <afxtempl.h>
#include "UserProfile.h"
#include "SharingConnection.h"
#include "FileUtility.h"
#include "WebUtility.h"

using namespace boost;
using namespace Json;

const CString UserProfile::KEY_USERNAME = _T("username");
const CString UserProfile::KEY_AVATAR = _T("avatar_url");
const CStringA UserProfile::VALUE_TWITTER = "twitter";
const CStringA UserProfile::VALUE_FBPROFILE = "facebook_profile";
const CStringA UserProfile::VALUE_FBPAGE = "facebook_page";
const CStringA UserProfile::VALUE_TUMBLR = "tumblr";

UserProfile::UserProfile(CString*& json)
{
	m_JsonProfile = CString(*json);
	m_Username = ParseValue(KEY_USERNAME);
	m_Username = WebUtility::UnicodeEntityDecode(m_Username);
	m_AvatarUrl = ParseValue(KEY_AVATAR);
	m_Connections = new CList<SharingConnection*>();
}

UserProfile::~UserProfile(void)
{
	for(POSITION i = m_Connections->GetHeadPosition(); i != NULL;)
        delete m_Connections->GetNext(i);
	delete m_Connections;
}

void UserProfile::SetConnections(CString*& json)
{
	for(POSITION i = m_Connections->GetHeadPosition(); i != NULL;)
        delete m_Connections->GetNext(i);
	m_Connections->RemoveAll();

	CStringA jsonA = CStringA(*json);
	Value root;
	Reader reader;
	const char* start = jsonA.GetBuffer();
	const char* end = start + jsonA.GetLength();

	if (!reader.parse(start, end, root))
	{
		OutputDebugString(_T("Failed to parse connections from json:\n"));
		OutputDebugStringA(reader.getFormattedErrorMessages().c_str());
		return;
	}
	ASSERT(root != NULL && root.isArray());

	for(unsigned int i = 0; i < root.size(); i++)
	{
		ASSERT(root[i].isObject());
		
		CStringA type = root[i].get("type", "").asCString();
		Json::Value displayNameValue = root[i].get("display_name", "");

		// jsoncpp retains the original encoding in raw char buffer
		// we assume UTF-8 multibyte encoding and transform the buffer accordingly
		CString displayName = WebUtility::FromUTF8MultiByte(displayNameValue.asCString());

		//bool publish = root[i].get("post_publish", false).asBool();
		int cid = root[i].get("id", -1).asInt();
		if(cid == -1)
		{
			CString debugMessage;
			debugMessage.Format(
				_T("Failed to parse valid id for '%s' connection (%s)\n"),
				CString(type), displayName);
			OutputDebugString(debugMessage);
			continue;
		}

		SharingConnection::ShareType connectionType = SharingConnection::unknown;
		if(type == VALUE_TWITTER)
			connectionType = SharingConnection::twitter;
		else if (type == VALUE_FBPROFILE)
			connectionType = SharingConnection::fbprofile;
		else if(type == VALUE_TUMBLR)
			connectionType = SharingConnection::tumblr;
		else if(type == VALUE_FBPAGE)
			connectionType = SharingConnection::fbpage;
		else
		{
			// TODO: 'SoundCloud' connection type?
			ASSERT(false);
			continue;
		}

		SharingConnection* connection = new SharingConnection(
			cid,
			displayName,
			connectionType);
		m_Connections->AddTail(connection);
	}

	jsonA.ReleaseBuffer();
}

CString UserProfile::ParseValue(LPCTSTR key)
{
	CString patternStr;
	CString patternFormat = _T("\"%s\"\\s*?:\\s*?\"(.*?)\"");
	patternStr.Format(patternFormat, key);

	tregex pattern(patternStr);
	tmatch match;
	if(regex_search(m_JsonProfile, match, pattern))
		return CString(match[1].first, match.length(1));
	return CString();
}

CString UserProfile::GetName(void)
{
	return m_Username;
}

CString UserProfile::GetAvatarUrl(void)
{
	return m_AvatarUrl;
}

void UserProfile::SetAvatarFilePath(LPCTSTR filePath)
{
	m_AvatarFilePath = CString(filePath);
}

LPCTSTR UserProfile::GetAvatarFilePath(void)
{
	return m_AvatarFilePath;
}

bool UserProfile::HasAvatar(void)
{
	return !m_AvatarFilePath.IsEmpty();
}

CList<SharingConnection*>* UserProfile::GetConnections(void)
{
	return m_Connections;
}