// Copyright (c) 2012, SoundCloud Ltd.

#include "StdAfx.h"
#include "SharingConnection.h"

SharingConnection::SharingConnection(void)
{
}

SharingConnection::SharingConnection(int id, CString displayName, ShareType type)
{
	m_Id = id;
	m_DisplayName = displayName;
	m_Type = type;
}

SharingConnection::~SharingConnection(void)
{
}

int SharingConnection::GetId(void)
{
	return m_Id;
}

CString SharingConnection::GetLabel(void)
{
	CString label;
	label.Format(_T("%s (%s)"), m_DisplayName, GetTypeLabel(m_Type));
	return label;
}

CString SharingConnection::GetTypeLabel(ShareType type)
{
	switch(type)
	{
	case twitter:
		return _T("Twitter");
	case fbprofile:
		return _T("FB Profile");
	case fbpage:
		return _T("FB Page");
	case tumblr:
		return _T("Tumblr");
	case soundcloud:
		return _T("SoundCloud");
	default:
		return _T("");
	}
}
