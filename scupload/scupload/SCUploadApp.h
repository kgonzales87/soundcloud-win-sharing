// Copyright (c) 2012, SoundCloud Ltd.

// Main header file for the SoundCloud Upload app

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"

// SC Windows app for upload of a track specified by command-line arguments.
// The user must have a valid SC account and is asked to sign-in before starting the upload. 
class SCUploadApp : public CWinAppEx
{
public:
	SCUploadApp();
	~SCUploadApp();
	virtual BOOL InitInstance();

	class CImpIDispatch* m_pDispOM;

	DECLARE_MESSAGE_MAP()
};

extern SCUploadApp theApp;