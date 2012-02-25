// Copyright (c) 2012, SoundCloud Ltd.

#pragma once

// Functions to validate urls. 
class WebUtility
{
public:
	static bool IsSusiCancelled(const CString& url);
	static bool IsSusiError(const CString& url, CString*& errorInfo);
	static CString UrlDecode(const CString strEncodedText);
	static CString GetFieldValueFromUrl(const CString& url, const CString& field);
	static bool IsFacebook(const CString& url);
	static void CopyToClipboard(HWND owner, const CString& s);
	static CString FileNameFromUrl(const CString& url);
	static bool IsOfflineMode();
};
