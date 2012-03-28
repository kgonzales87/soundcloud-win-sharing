// Copyright (c) 2012, SoundCloud Ltd.

#pragma once
#include "afxcoll.h"

typedef void (*PROGRESSCALLBACK) (DWORD, DWORD);

// Sends text and binary data as a multi-part POST request to a specified resource.
// Not multi-threading safe and always blocking.
// However, during an upload, a progress callback is called after each chunk,
// so the UI thread can be updated if this class is used from a non-UI thread.
class MultipartPostMethod
{
private:
	PROGRESSCALLBACK m_fProgressCallback;
	CString textParts;
	CMapStringToPtr fileParts;
	UINT totalLength;
	UINT progress;
	CString ctHeader;
	CString uaHeader;
	CString endPart;
	static const CString END_TOKEN;
	static const DWORD CHUNKLENGTH;
	static const CString MULTIPART_BOUNDARY;
	void Init(void);

public:
	MultipartPostMethod(PROGRESSCALLBACK f, LPCTSTR userAgent);
	~MultipartPostMethod(void);
	void AddTextPart(const CString& name, const CString& data);
	int AddBinaryPart(const CString& name, const CString& filePath);
	void AddTextParts(const CMapStringToString* nameDataPairs);
	DWORD SendRequest(CHttpConnection* connection, const CString& url, DWORD dwFlags, CString*& pResponse);
	void WriteBinaryData(CHttpFile* pHttpFile, const CString& header, CFile* file);
	void Close(void);

	static const DWORD ENCODING_ERROR;
};
