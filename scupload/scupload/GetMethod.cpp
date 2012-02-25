// Copyright (c) 2012, SoundCloud Ltd.

#include "StdAfx.h"
#include "GetMethod.h"
#include "FileUtility.h"
#include "WebUtility.h"

GetMethod::GetMethod(void)
{
}

GetMethod::~GetMethod(void)
{
}

DWORD GetMethod::SendRequest(CHttpConnection* pCon, const CString& url, CString*& pResponse)
{
	DWORD resultCode = 0;
	CString response;
	CHttpFile* pHttpFile = NULL;

	try
	{
		pHttpFile = pCon->OpenRequest(
			CHttpConnection::HTTP_VERB_GET,
			url, 0, 1, 0, 0, INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD);
		pHttpFile->SendRequest();
		pHttpFile->QueryInfoStatusCode(resultCode);

		// Receive response
		LPSTR szResponse;
		DWORD responseLength = (DWORD)pHttpFile->GetLength();
		while (0 != responseLength )
		{
			szResponse = (LPSTR)malloc(responseLength + 1);
			szResponse[responseLength] = '\0';
			pHttpFile->Read(szResponse, responseLength);
			response += szResponse;
			free(szResponse);
			responseLength = (DWORD)pHttpFile->GetLength();
		}
	}
	catch(CException*)
	{
		if(pHttpFile != NULL)
		{
			pHttpFile->Abort();
			delete pHttpFile;
		}
		throw;
	}

	if(pHttpFile != NULL)
	{
		pHttpFile->Abort();
		delete pHttpFile;
	}

	pResponse = new CString(response);
	return resultCode;
}

bool GetMethod::DownloadFile(CInternetSession* pSession, const CString& url, CString*& pFilePath)
{
	bool result = FALSE;
	CStdioFile* pHttpFile = NULL;

	try
	{
		LPCTSTR acceptHeader = _T("Accept: image/jpg, image/jpeg, image/png, image/gif\r\n");
		pHttpFile = pSession->OpenURL(url, 1,
			INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_DONT_CACHE,
			acceptHeader, -1L);

		// Save file
		BYTE buffer[2048];
		CString filename = WebUtility::FileNameFromUrl(url);
		TCHAR filePath[MAX_PATH];
		GetUserDataPath(filePath);
		PathAppend(filePath, filename);
		CFile file = CFile(filePath,
			CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		int read = 0;
		while ((read = pHttpFile->Read(buffer, sizeof(buffer))) > 0)
			file.Write(buffer, read);
		file.Close();

		pFilePath = new CString(filePath);
		result = TRUE;
	}
	catch(CException* ex)
	{
		OutputDebugString(_T("Failed to download file from " + url));
		OutputExceptionMessage(ex);
		if(pHttpFile != NULL)
		{
			pHttpFile->Abort();
			delete pHttpFile;
		}
		ex->Delete();
	}

	if(pHttpFile != NULL)
	{
		pHttpFile->Abort();
		delete pHttpFile;
	}

	return result;
}
