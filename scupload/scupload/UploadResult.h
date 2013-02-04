// Copyright (c) 2013, SoundCloud Ltd.
#pragma once

class UploadResult
{
private:
	bool Parse(CString*& json);
	CString m_PermaLink;
	CString m_SecretToken;
	CString m_ErrorMessage;
	CString m_Sharing;
	bool m_IsPrivate;
	bool m_HasError;
public:
	UploadResult(CString*& json);
	virtual ~UploadResult(void);
	CString* GetPermaLink(void);
	CString* GetErrorMessage(void);
	bool HasError(void);
};
