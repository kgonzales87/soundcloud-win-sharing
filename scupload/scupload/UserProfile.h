// Copyright (c) 2012, SoundCloud Ltd.

#pragma once
#include "SharingConnection.h"

// Profile data of SoundCloud user, based on
// JSON data structure passed to the constructor.
class UserProfile
{
private:
	static const CString KEY_USERNAME;
	static const CString KEY_AVATAR;
	static const CStringA VALUE_TWITTER;
	static const CStringA VALUE_FBPROFILE;
	static const CStringA VALUE_FBPAGE;
	static const CStringA VALUE_TUMBLR;

	CString m_Username;
	CString m_AvatarUrl;
	CString m_AvatarFilePath;
	CString m_JsonProfile;
	CList<SharingConnection*>* m_Connections;

	CString ParseValue(LPCTSTR key);

public:
	UserProfile(CString*& json);
	virtual ~UserProfile(void);
	void SetConnections(CString*& json);
	void SetAvatarFilePath(LPCTSTR filePath);
	bool HasAvatar(void);
	LPCTSTR UserProfile::GetAvatarFilePath(void);
	CString GetName(void);
	CString GetAvatarUrl(void);
	CList<SharingConnection*>* GetConnections(void);
};
