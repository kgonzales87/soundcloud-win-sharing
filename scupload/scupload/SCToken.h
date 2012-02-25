// Copyright (c) 2012, SoundCloud Ltd.

#pragma once

// The OAuth access token can be saved to a file.
class SCToken
{
public:
	SCToken(const CString& clientId);
	virtual ~SCToken(void);

	virtual CString GetToken(void);
	virtual void SetToken(const CString& token);
	virtual bool IsEmpty(void);
	virtual int Save(void);
	virtual int Reset(void);
protected:
	// Loads the access token from file
	virtual int Load(void);
private:
	CString m_sToken;
	CString GetTokenFilePath(void);
	// The ClientId is used to name the token storage file
	CString m_ClientId;
};
