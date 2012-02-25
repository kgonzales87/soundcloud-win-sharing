// Copyright (c) 2012, SoundCloud Ltd.

#pragma once
#include "Windows.h"
#include "Shlobj.h"

// Helper function to get the user data path for the SC app
inline void GetUserDataPath(TCHAR* szPath)
{
	if(SUCCEEDED(SHGetFolderPath(NULL, 
								 CSIDL_LOCAL_APPDATA|CSIDL_FLAG_CREATE, 
								 NULL, 
								 0, 
								 szPath))) 
	{
		PathAppend(szPath, _T("SoundCloud"));

		if(GetFileAttributes(szPath) == INVALID_FILE_ATTRIBUTES)
		{
			SECURITY_ATTRIBUTES saPermissions;
			saPermissions.nLength = sizeof(SECURITY_ATTRIBUTES);
			saPermissions.lpSecurityDescriptor = NULL;
			saPermissions.bInheritHandle = TRUE;

			if(CreateDirectory(szPath, &saPermissions) == FALSE)
				OutputDebugString(_T("Failed to create SoundCloud directory."));
		}
	}
}

inline void OutputExceptionMessage(CException* pEx)
{
	if(pEx == NULL)
		return;
	TCHAR pError[256];
	pEx->GetErrorMessage(pError, 256);
	OutputDebugString(pError);
}
