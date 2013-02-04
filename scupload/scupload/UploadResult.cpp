// Copyright (c) 2013, SoundCloud Ltd.
#include "StdAfx.h"
#include "WebUtility.h"
#include "UploadResult.h"

using namespace Json;

UploadResult::UploadResult(CString*& json)
{
	m_IsPrivate = false;
	m_HasError = Parse(json);
}

UploadResult::~UploadResult(void)
{

}

bool UploadResult::HasError(void)
{
	return m_HasError;
}

CString* UploadResult::GetPermaLink(void)
{
	CString* url = new CString(m_PermaLink);
	if(m_IsPrivate)
	{
		url->Append(_T("/"));
		url->Append(m_SecretToken);
	}
	return url;
}

CString* UploadResult::GetErrorMessage(void)
{
	return new CString(m_ErrorMessage);
}

//Parse the permalink_url from the response
//e.g. "permalink_url": "http://soundcloud.com/jwagener/a-nice-track-title-1"
bool UploadResult::Parse(CString*& json)
{
	CStringA jsonA = CStringA(*json);
	Value root;
	Reader reader;
	const char* start = jsonA.GetBuffer();
	const char* end = start + jsonA.GetLength();

	if (!reader.parse(start, end, root))
	{
		m_ErrorMessage = _T("Failed to parse upload result!");
		OutputDebugString(m_ErrorMessage);
		OutputDebugStringA(reader.getFormattedErrorMessages().c_str());
		return true;
	}

	ASSERT(root != NULL && root.isObject());

	const Json::Value errorValue = root.get("error_message", Json::Value::null);
	if(!errorValue.empty())
	{
		m_ErrorMessage = WebUtility::FromUTF8MultiByte(errorValue.asCString());
		OutputDebugString(m_ErrorMessage);
		return true; // There won't be more to parse
	}

	const Json::Value permalink = root.get("permalink_url", Json::Value::null);
	if(!permalink.empty())
	{
		m_PermaLink = WebUtility::FromUTF8MultiByte(permalink.asCString());

		Json::Value sharing = root.get("sharing", Json::Value::null);
		if(!sharing.empty())
		{
			m_Sharing = WebUtility::FromUTF8MultiByte(sharing.asCString());
			m_IsPrivate = m_Sharing.CompareNoCase(_T("private")) == 0;
		}

		if(m_IsPrivate)
		{
			Json::Value secretToken = root.get("secret_token", Json::Value::null);
			if(!secretToken.empty())
			{
				m_SecretToken = WebUtility::FromUTF8MultiByte(secretToken.asCString());
			}
		}
	}
	return false;
}