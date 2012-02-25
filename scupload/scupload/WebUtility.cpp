// Copyright (c) 2012, SoundCloud Ltd.

#include "StdAfx.h"
#include "Shlwapi.h"
#include "WebUtility.h"

using namespace boost;

// Checks if the SUSI flow was cancelled
bool WebUtility::IsSusiCancelled(const CString& url)
{
	// The user cancelled when following url redirect occurs:
	// http://soundcloud.com/login?return_to=%2Fdashboard%3Ferror%3Daccess_denied%26error_description%3DThe%2Bend-user%2Bdenied%2Bthe%2Brequest
	
	CString decodedUrl = UrlDecode(url);
	tregex accessDeniedPattern(_T("error=access_denied"));
	tregex errorDescPattern(_T("error_description=The\\+end-user\\+denied\\+the\\+request"));
	tmatch match;
	if(regex_search(decodedUrl, match, accessDeniedPattern) &&
		regex_search(decodedUrl, match, errorDescPattern))
		return true;

	return false;
}

// Checks if SC returned a SUSI error
bool WebUtility::IsSusiError(const CString& url, CString*& errorInfo)
{
	// The error should come from
	tregex p(_T("^https?://(www\\.)?soundcloud\\.com"));
	tmatch m;
	if(!regex_search(url, m, p))
		return false;	

	CString errorParams = GetFieldValueFromUrl(url, _T("return_to"));
	if(errorParams.IsEmpty())
		errorParams = url;

	CString decodedUrl = UrlDecode(errorParams);
	CString errorDesc = GetFieldValueFromUrl(decodedUrl, _T("error_description"));
	if(errorDesc.IsEmpty())
		errorDesc = GetFieldValueFromUrl(decodedUrl, _T("error"));
	if(errorDesc.IsEmpty())
		return false;

	errorDesc.Replace(_T('+'), _T(' ')); // From URL encoding!
	errorInfo = new CString(errorDesc);
	return true;
}

bool WebUtility::IsFacebook(const CString& url)
{
	tregex p(_T("^https?://(www\\.)?facebook\\.com"));
	tmatch m;
	return regex_search(url, m, p);
}

// Simplistic url decoding. Intended to be sufficient only when dealing
// with SC servers. Will keep '+' in the query component of the URL as is.
CString WebUtility::UrlDecode(const CString encoded)
{
	CString decoded;

	for(int i = 0; i < encoded.GetLength(); i++)
	{
		wchar_t c = encoded.GetAt(i);
		if(c == _T('%'))
		{
			wchar_t encodedChar[3];		
			encodedChar[0] = encoded.GetAt(++i);
			encodedChar[1] = encoded.GetAt(++i);
			encodedChar[2] = '\0';		
			
			CString decodedChar;
			decodedChar.Format(_T("%c"), _tcstoul(encodedChar, NULL, 16));
			decoded += decodedChar;
		}
		else
		{
			decoded += c;
		}
	}
	
	return decoded;
}
// Extracts a field value from the url query string
CString WebUtility::GetFieldValueFromUrl(const CString& url, const CString& field)
{
	CString fieldValue;
	CString param = field + _T("=");
	CString separators = _T("?#&");
	int i = 0;
	CString token = url.Tokenize(separators, i);
	while(!token.IsEmpty())
	{
		if(token.Left(param.GetLength()) == param)
		{
			fieldValue = token.Right(token.GetLength() - param.GetLength());
			break;
		}
		token = url.Tokenize(separators, i);
	}

	return fieldValue;
}

// Adds the given string to the Windows clipboard
void WebUtility::CopyToClipboard(HWND owner, const CString& s)
{
	HGLOBAL hGlob = NULL;
	if(!OpenClipboard(owner))
	{
		OutputDebugString(_T("Cannot open the Clipboard!"));
		return;
	}
	
	EmptyClipboard();
	hGlob = GlobalAlloc(GMEM_FIXED, s.GetLength() + 1);
	CT2A chars(s, CP_UTF8);
	strcpy_s((char*)hGlob, s.GetLength() + 1, chars.m_psz);
	if(::SetClipboardData(CF_TEXT, hGlob) == NULL)
	{
		CString msg;
		msg.Format(_T("Sorry, can't copy to clipboard! (error: %d)"), GetLastError());
		AfxMessageBox(msg);
		CloseClipboard();
		GlobalFree(hGlob);
		return;
	}
	CloseClipboard();
}

// Parses the file from the url
CString WebUtility::FileNameFromUrl(const CString& url)
{
	int i = 0;
	CString fileUrl = url.Tokenize(_T("?"), i);
	CString filePart = PathFindFileName(fileUrl);
	return filePart;
}

// Determines if IE offline mode is currently enabled.
bool WebUtility::IsOfflineMode()
{
	DWORD flags = 0;
	if(InternetGetConnectedState(&flags, 0))
		if((flags & INTERNET_CONNECTION_OFFLINE) > 0)
			return true;
	
	return false;
}
