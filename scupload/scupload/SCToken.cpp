// Copyright (c) 2012, SoundCloud Ltd.

#include "StdAfx.h"
#include "SCToken.h"
#include "FileUtility.h"

SCToken::SCToken(const CString& clientId)
: m_sToken(_T(""))
, m_ClientId(clientId)
{
	Load();
}

SCToken::~SCToken(void)
{
}

// Persists the current access token
// to a file in the user's application data folder.
// e.g. C:\Users\{username}\AppData\Local\SoundCloud\{client-id}.scx
int SCToken::Save(void)
{
	CStdioFile input;

	try
	{
		input.Open(GetTokenFilePath(),
			CFile::modeCreate|CFile::shareDenyNone|CFile::modeWrite);
		input.WriteString(m_sToken);
	}
	catch(CFileException* ex)
	{
		TCHAR   szCause[255];
		ex->GetErrorMessage(szCause, 255);

		CString errorMsg = _T("Failed to save access token: ");
		OutputDebugString(errorMsg + szCause);
		
		delete ex;
	}
	
	input.Close();

	return 0;
}

// Loads the access token for current windows user from file.
int SCToken::Load(void)
{
	CFileStatus status;
	CString filePath = GetTokenFilePath();
	if(CFile::GetStatus(filePath, status))
	{
		CStdioFile tokenFile;
		try
		{
			tokenFile.Open(filePath, CFile::shareDenyWrite|CFile::modeRead);
			tokenFile.ReadString(m_sToken);
		}
		catch(CFileException*)
		{
			return -1;
		}
		tokenFile.Close();
	}
	return 0;
}

// Generates the fully qualified path to the file used to store the access token 
CString SCToken::GetTokenFilePath(void)
{
	TCHAR userDataPath[MAX_PATH];
	GetUserDataPath(userDataPath);

	CString fileName;
	fileName.Format(_T("%s.scx"), m_ClientId);
	PathAppend(userDataPath, fileName);

	return CString(userDataPath);
}

CString SCToken::GetToken(void)
{
	return m_sToken;
}

void SCToken::SetToken(const CString& token)
{
	m_sToken = CString(token);
}

bool SCToken::IsEmpty(void)
{
	return m_sToken.IsEmpty();
}

int SCToken::Reset(void)
{
	m_sToken.Empty();
	Save();
	return 0;
}
