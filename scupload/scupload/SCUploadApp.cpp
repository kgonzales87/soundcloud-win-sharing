// Copyright (c) 2012, SoundCloud Ltd.

#include "stdafx.h"
#include "SCUploadApp.h"
#include "SCUploadDlg.h"
#include "SCCommandLineInfo.h"
#include "Custsite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(SCUploadApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


SCUploadApp::SCUploadApp()
{
	m_pDispOM = NULL;
}

SCUploadApp::~SCUploadApp()
{
	if ( m_pDispOM != NULL)
	{
		//This is the way that the driller sample disposes 
		//the m_pDispOM object.
		//Note that the CImpIDispatch::Release() method 
		//is (intentionally?) never disposing its object! 
		delete m_pDispOM;
	}
}


// The one and only SCUploadApp object
SCUploadApp theApp;

BOOL SCUploadApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	
	// Common control classes
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// New IDispatch implementation extending the IE HTML object model
	m_pDispOM = new CImpIDispatch;

	// Setup control container for IE customization
	AfxEnableControlContainer(new CCustomOccManager);
	//AfxEnableControlContainer(); // Standard MFC

	// Parse command line ars
	SCCommandLineInfo cmdInfo = SCCommandLineInfo(m_lpCmdLine);
	ParseCommandLine(cmdInfo);

	SCUploadDlg dlg;
	dlg.SetCmdLineInfo(&cmdInfo);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDCANCEL)
	{
		// Any cleanup before shutdown...
	}

	// Returning FALSE so that we exit the
	// application, rather than start the application's message pump.
	return FALSE;
}
