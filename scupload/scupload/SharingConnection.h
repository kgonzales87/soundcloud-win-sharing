// Copyright (c) 2012, SoundCloud Ltd.

#pragma once

// Sharing connection as defined by a SoundCloud user profile.
// As of this writing, users can share uploads on the services
// listed in the enum ShareType. This impementation should be extended
// to support more sharing types as they become available.
class SharingConnection
{
public:
	enum ShareType { unknown, soundcloud, tumblr, fbprofile, fbpage, twitter };
private:
	int m_Id;
	CStringA m_DisplayName;
	ShareType m_Type;
	static CString GetTypeLabel(ShareType type);

public:
	SharingConnection(void);
	SharingConnection(int id, CStringA displayName, ShareType type);
	virtual ~SharingConnection(void);
	CString GetLabel(void);
	int GetId(void);
};
