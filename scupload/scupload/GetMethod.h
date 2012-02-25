// Copyright (c) 2012, SoundCloud Ltd.

#pragma once

// Encapsulates an HTTP GET request to a specified resource or
// allows the download of a file.
// Thread-safe and blocking.
class GetMethod
{
public:
	GetMethod(void);
	virtual ~GetMethod(void);
	DWORD SendRequest(CHttpConnection* pCon, const CString& url, CString*& pResponse);
	bool DownloadFile(CInternetSession* pSession, const CString& url, CString*& pFilePath);
};
