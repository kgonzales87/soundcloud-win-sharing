// Copyright (c) 2012, SoundCloud Ltd.

#pragma once
#include "SCConnector.h"
#include "CWebBrowser2.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "UserProfile.h"
#include "CLicenseComboBox.h"

// The dialog UI hosting IE in case a sign-in or sign-up is required.
// UI updates are based on the current AppState (m_appState), modelling the
// sequential user actions required to complete a track upload.
class SCUploadDlg : public CDialog
{
public:
	SCUploadDlg(CWnd* pParent = NULL);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetCmdLineInfo(SCCommandLineInfo* cmdLineInfo);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void ShowStatusMessage(LPCTSTR newStatus);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	enum { IDD = IDD_SCUPLOAD_DIALOG, IDWB = 200 }; // Dialog Data
protected:
	HICON m_hIcon;
	SCConnector m_Connector;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg long OnConnectionCheckComplete(WPARAM wParam, LPARAM lParam);
	afx_msg long OnTokenVerificationComplete(WPARAM wParam, LPARAM lParam);
	afx_msg long OnUploadComplete(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedAction();
	afx_msg void OnOpenButtonClicked();
	afx_msg void OnLogOutClicked();
	afx_msg LRESULT OnCustomControlSiteMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedVisibility();
	afx_msg void OnNMClickLink(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedPicButton();
	afx_msg void OnEnChangeTitleeditor();
	afx_msg void OnStnClickedLicenseHelp();
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()

private:
	enum AppStates
	{
		Started,
		NotConnected,
		VerifyingToken,
		SUSI,
		SUSI_FB,
		UploadConfig,
		Uploading,
		UploadFailed,
		UploadSucceeded
	};

	bool m_isStartup;
	bool m_validArgs;
	AppStates m_appState;
	UserProfile* m_pUserProfile;

	CProgressCtrl m_ProgressCtrl;
	CStatic m_Status;
	CStatic m_Status2;
	CButton m_CancelButton;
	CButton m_ActionButton;
	CWebBrowser2* m_pBrowser;
	CButton m_OpenButton;
	CButton m_LogOutButton;
	CStatic m_GroupBox;
	CStatic m_Username;
	CStatic m_Avatar;
	CEdit m_TitleEditor;
	CEdit m_TagEditor;
	CButton m_FbCheck;
	CButton m_TwitterCheck;
	CButton m_TumblrCheck;
	CStatic m_TitleLabel;
	CStatic m_TagLabel;
	CStatic m_SharingLabel;
	CButton m_PublicRadio;
	CButton m_PrivateRadio;
	CLicenseComboBox m_LicenseCombo;
	CStatic m_LicenseHelp;
	CStatic m_PictureBox;
	CButton m_PictureButton;
	CStatic m_Usage;
	CStatic m_LicenseLabel;
	CStatic m_PictureLabel;
	CCheckListBox m_SharingList;
	CLinkCtrl m_ConnectionsLabel;
	CStatic m_PrivateInfo1;
	CStatic m_PrivateInfo2;
	CImage m_ArtworkImage;
	CLinkCtrl m_PermaLink;
	CBrush m_BackgroundBrush;
	COLORREF m_BackgroundColor;
	bool m_OverrideBackground;
	CSize m_Size1;
	CSize m_Size2;
	CSize m_Size3;

	long OnUploadProgress(WPARAM wParam, LPARAM lParam);
	void OnNavigateComplete(LPDISPATCH pDisp, VARIANT* szUrl);
	void OnNavigateError(LPDISPATCH pDisp,
								  VARIANT *URL,
								  VARIANT *TargetFrameName,
								  VARIANT *StatusCode,
								  VARIANT_BOOL *Cancel);
	void OnDocumentComplete(LPDISPATCH lpDisp, VARIANT FAR* URL);
	bool ShowAuthView(void);
	bool CloseAuthView(void);
	void ShowUploadConfig(bool show);
	static CString GetEditorText(CEdit& editor);
	void GetSelectedConnections(CArray<int, int&>& connections);
	void OpenBrowser(const LPCTSTR url);
	void InitUploadConfig(void);
	void ShowUsage(void);
	void SetImage(CString& filePath, CStatic& control);
	void UpdateWindowSize(void);
	void UpdateDisplay(CString statusMessage);
	void UpdateWindowTitle(const CString& title);
	void ShowAuth(void);
};
