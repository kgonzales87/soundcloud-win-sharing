// Copyright (c) 2012, SoundCloud Ltd.

#include "stdafx.h"
#include "MultipartPostMethod.h"

const CString MultipartPostMethod::MULTIPART_BOUNDARY = _T("QQQSPACETHEFINALFRONTIERQQQ");
const DWORD MultipartPostMethod::CHUNKLENGTH = 1024;
const CString MultipartPostMethod::END_TOKEN = _T("\r\n");
const DWORD MultipartPostMethod::ENCODING_ERROR = 10;

MultipartPostMethod::MultipartPostMethod(PROGRESSCALLBACK f, LPCTSTR userAgent)
{
	m_fProgressCallback = f;
	totalLength = 0;
	progress = 0;
	uaHeader = userAgent;
	Init();
}

MultipartPostMethod::~MultipartPostMethod(void)
{
}

void MultipartPostMethod::Init(void)
{
	CString format;

	// Set header
	format = _T("Content-Type: multipart/form-data; boundary=%s\r\n");
	ctHeader.Format(format, MULTIPART_BOUNDARY);

	// Set end part which is appended at the end of a request
	format = _T("--%s--\r\n");
	endPart.Format(format, MULTIPART_BOUNDARY);
	totalLength += endPart.GetLength();
}

DWORD MultipartPostMethod::SendRequest(CHttpConnection* pCon, const CString& url, DWORD dwFlags, CString*& pResponse)
{
	DWORD dwRet = 0;
	CHttpFile* pHttpFile = NULL;
	CString response;
	CString debugMsg;

	try
	{
		pHttpFile = pCon->OpenRequest(CHttpConnection::HTTP_VERB_POST, url, 0, 1, 0, 0, dwFlags);
		pHttpFile->AddRequestHeaders(ctHeader);
		pHttpFile->AddRequestHeaders(uaHeader, HTTP_ADDREQ_FLAG_REPLACE);
		
		if(!pHttpFile->SendRequestEx(totalLength, HSR_SYNC | HSR_INITIATE))
			AfxThrowInternetException(pHttpFile->GetContext());

		// Send track properties UTF-8 encoded
		CStringA text;
		CStringW wideText = textParts;
		int utf8Length = ::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wideText, -1, NULL, 0, NULL, NULL);
		LPSTR utf8Bytes = text.GetBuffer(utf8Length);
		if(::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wideText, -1, utf8Bytes, utf8Length, NULL, NULL) == 0)
		{
			DWORD err = ::GetLastError();
			text.ReleaseBuffer();
			CString log;
			log.Format(_T("Failed to encode metadata as UTF-8: %d"), err);
			OutputDebugString(log + _T("\n"));
			pResponse = new CString(log);
			if(pHttpFile != NULL)
			{
				pHttpFile->Abort();
				delete pHttpFile;
			}
			return ENCODING_ERROR;
		}
		text.ReleaseBuffer();
		ASSERT(text.GetLength() + 1 == utf8Length); //utf8Length includes terminating \0 char
		pHttpFile->Write(text, text.GetLength());
		pHttpFile->Flush();

		progress += text.GetLength();
		m_fProgressCallback(progress, totalLength);
		
		// Send file data 
		POSITION pos = fileParts.GetStartPosition();
		while(pos != NULL)
		{
			CString fileHeader;
			CFile* pFile = NULL;
			fileParts.GetNextAssoc(pos, fileHeader, reinterpret_cast<void*&>(pFile));
			WriteBinaryData(pHttpFile, fileHeader, pFile);
		}

		// Send request closure
		CT2CA endPartBuffer (endPart);
		pHttpFile->Write((LPSTR)endPartBuffer, endPart.GetLength());
		pHttpFile->Flush();
		progress += endPart.GetLength();

		debugMsg.Format(_T("MultipartPostMethod::SendRequest: Uploaded %d / %d bytes\n"),
			progress, totalLength); 
		OutputDebugString(debugMsg);

		try
		{
			pHttpFile->EndRequest(HSR_SYNC);
			pHttpFile->QueryInfoStatusCode(dwRet);
		}
		catch (CInternetException* ex)
		{
			OutputDebugString(_T("MultipartPostMethod::SendRequest: Exception on EndRequest!\n"));
			if(progress == totalLength)
			{
				dwRet = 201; // Recover from timeout with completed upload
				ex->Delete();
			}
			else
				throw;
		}
		
		ASSERT(progress == totalLength);
		m_fProgressCallback(totalLength, totalLength);
		OutputDebugString(_T("MultipartPostMethod::SendRequest: Upload complete!\n"));

		// Receive response
		LPSTR szResponse;
		DWORD responseLength = pHttpFile->GetLength();
		while (0 != responseLength )
		{
			szResponse = (LPSTR)malloc(responseLength + 1);
			szResponse[responseLength] = '\0';
			pHttpFile->Read(szResponse, responseLength);
			response += szResponse;
			free(szResponse);
			responseLength = pHttpFile->GetLength();
		}

		OutputDebugString(_T("MultipartPostMethod::SendRequest: Response:\n"));
		OutputDebugString(CString(response));
		OutputDebugString(_T("\n"));
	}
	catch(CException* ex)
	{
		debugMsg.Format(_T("MultipartPostMethod::SendRequest: Exception at %d / %d bytes\n"),
			progress, totalLength); 
		OutputDebugString(debugMsg);

		if(pHttpFile != NULL)
		{
			pHttpFile->QueryInfoStatusCode(dwRet);
			debugMsg.Format(_T("MultipartPostMethod::SendRequest: Exception with http code %d\n"), dwRet); 
			OutputDebugString(debugMsg);

			pHttpFile->Abort();
			delete pHttpFile;
		}
		throw;
	}

	if(pHttpFile != NULL)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}

	pResponse = new CString(response);
	return dwRet;
}

void MultipartPostMethod::AddTextPart(const CString& name, const CString& data)
{
	CString format;
	CString partHeader;

	format =  _T("--%s");
	format += _T("\r\n");
	format += _T("Content-Disposition: form-data; name=\"%s\"");
	format += _T("\r\n\r\n");
	format += _T("%s");
	format += _T("\r\n");

	partHeader.Format(format, MULTIPART_BOUNDARY, name, data);
	int mbLength = ::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)partHeader, -1, NULL, 0, NULL, NULL);
	totalLength += (mbLength - 1); //mbLength includes terminating \0 char!
	textParts += partHeader;
}

int MultipartPostMethod::AddBinaryPart(const CString& name, const CString& filePath)
{
	if(filePath.IsEmpty())
		return CFileException::badPath;

	CFile* file = new CFile();
	CFileException* ex = NULL;
	if(!file->Open(filePath, CFile::modeRead, ex)) // failure is expected
	{
		file->Abort();
		return ex != NULL ? ex->m_cause : CFileException::genericException;
	}
	
	CString format;
	CString partHeader;

	format =  _T("--%s");
	format += _T("\r\n");
	format += _T("Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"");
	format += _T("\r\n");
	format += _T("Content-Type: application/octet-stream");
	format += _T("\r\n");
	format += _T("Content-Transfer-Encoding: binary");
	format += _T("\r\n\r\n");

	partHeader.Format(format, MULTIPART_BOUNDARY, name, file->GetFileName());

	totalLength += partHeader.GetLength();
	totalLength += file->GetLength();
	totalLength += END_TOKEN.GetLength();

	fileParts.SetAt(partHeader, file);

	return 0;
}

void MultipartPostMethod::AddTextParts(const CMapStringToString* nameDataPairs)
{
	POSITION pos = nameDataPairs->GetStartPosition();
	while(pos != NULL)
	{
		CString name, data;
		nameDataPairs->GetNextAssoc(pos, name, data);
		AddTextPart(const_cast<CString&>(name), data);
	}
}

void MultipartPostMethod::WriteBinaryData(CHttpFile* pHttpFile, const CString& header, CFile* file)
{
	//Send file data section
	CT2CA pHeaderBuffer (header);
	progress += header.GetLength();
	pHttpFile->Write((LPSTR)pHeaderBuffer, header.GetLength());
	pHttpFile->Flush();
	m_fProgressCallback(progress, totalLength);

	//Send file data
	void* pBuffer = NULL;
	DWORD dwReadLength = -1;
	
	try
	{
		pBuffer = malloc(CHUNKLENGTH);
		if(pBuffer == NULL)
			AfxThrowMemoryException();

		CString debugMsg;
		debugMsg.Format(_T("Starting to read/write %s"), file->GetFileName());
		OutputDebugString(debugMsg);

		while(0 != dwReadLength)
		{
			dwReadLength = file->Read(pBuffer, CHUNKLENGTH);
			
			debugMsg.Format(_T("Read %d bytes\n"), dwReadLength);
			OutputDebugString(debugMsg);

			if (0 != dwReadLength)
			{
				pHttpFile->Write(pBuffer, dwReadLength);
				
				debugMsg.Format(_T("Wrote %d bytes to http stream\n"), dwReadLength);
				OutputDebugString(debugMsg);
				
				pHttpFile->Flush();
				progress += (UINT)dwReadLength;

				debugMsg.Format(_T("MultipartPostMethod::WriteBinaryData: Uploaded %d / %d bytes\n"),
					progress, totalLength, dwReadLength);
				OutputDebugString(debugMsg);

				m_fProgressCallback(progress, totalLength);
			}
		}
		file->Close();
	}
	catch(CException* ex)
	{
		CString debugMsg;
		debugMsg.Format(_T("MultipartPostMethod::WriteBinaryData: Exception at %d / %d (buffer %d)\n"),
			progress, totalLength, dwReadLength);
		OutputDebugString(debugMsg);

		if (pBuffer != NULL)
			free(pBuffer);
		throw;
	}

	if (pBuffer != NULL)
		free(pBuffer);

	//Send end of part
	CT2CA endTokenBuffer (END_TOKEN);
	pHttpFile->Write((LPSTR)endTokenBuffer, END_TOKEN.GetLength());
	pHttpFile->Flush();
	progress += END_TOKEN.GetLength();
}

void MultipartPostMethod::Close(void)
{
	POSITION pos = fileParts.GetStartPosition();
	while(pos != NULL)
	{
		CString fileHeader;
		CFile* pFile = NULL;
		fileParts.GetNextAssoc(pos, fileHeader, reinterpret_cast<void*&>(pFile));
		if(pFile != NULL)
		{
			pFile->Abort();
			delete pFile;
		}
	}
	fileParts.RemoveAll();
}
