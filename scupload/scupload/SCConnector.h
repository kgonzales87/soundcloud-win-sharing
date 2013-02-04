// Copyright (c) 2012, SoundCloud Ltd.

#pragma once
#include "SCCommandLineInfo.h"
#include "SCToken.h"

#define WM_USER_SCME_COMPLETE WM_USER+0x100
#define WM_USER_SCPOST_PROGRESS WM_USER+0x101
#define WM_USER_SCPOST_COMPLETE WM_USER+0x102
#define WM_USER_CONCHECK_COMPLETE WM_USER+0x103
#define SC_CONNECTION_FAILED 999
#define SC_FAIL_ERROR 998

typedef struct scHeadThreadParam
{
	CString resx;
	HWND hWnd;
	CInternetSession* session;
} URL_THREADPARAM;

typedef struct scPostThreadParam
{
	CString assetData;
	CString artworkData;
	CString token;
	CMapStringToString* trackProperties;
	HWND hWnd;
	CInternetSession* session;
	CString* userAgent;
} UPLOAD_THREADPARAM;

// Offers a range of asynchronous requests to SoundCloud web services.
// Is able to persist or reset the currently assigned access token.
// A web service request can only be completed if the necessary data
// has been provided, e.g. an access token must be provided before it can
// be validated, or track properties are required before a track can be uploaded.
class SCConnector : public CObject
{
public:
	static const CString TRACK_TITLE;
	static const CString TRACK_TAGS;
	static const CString TRACK_SHARING;
	static const CString TRACK_SHAREDTO;
	static const CString TRACK_LICENSE;
	static const CString ARTWORK_DATA;
	static const CString PRIVATE;
	static const CString PUBLIC;
	static const CString DEFAULT_TITLE;
	static const CString LICENSE_HELP_URL;

	SCConnector();
	virtual ~SCConnector();
	bool VerifyToken();
	void IsConnected();
	void SetCommandLineInfo(SCCommandLineInfo* cmdLineInfo);
	void SetCallbackHandle(HWND hWnd);
	void UploadFile();
	int SetAccessToken(const CString& accessToken);
	bool IsSusiUrl(const CString& url);
	CString GetSusiUrl(void);
	void KeepToken(void);
	void ResetToken(void);
	void SetTrackProperty(LPCTSTR key, LPCTSTR value);
	CString GetTrackProperty(LPCTSTR key);
	void RemoveTrackProperty(LPCTSTR key);
	void SetTrackProperties(CString title, CString tags, CString license, bool isPublic, const CArray<int, int&>& connections);
protected:
	SCCommandLineInfo* m_CmdLineInfo;
	SCToken* m_PersistentToken;
	static HWND m_hCallbackWnd;

private:
	CInternetSession* m_pSession;
	CString m_UserAgent;
	CString GetOsId(void);
	static const CString PROFILE_URL;
	static UINT RequestHead(LPVOID pParam);
	static UINT RequestProfile(LPVOID pParam);
	static UINT PostFile(LPVOID pParam);
	static void UploadProgress(DWORD progress, DWORD total);
	static CString* GetMessageFromResponse(DWORD resultCode, CString* pResponse);
	
	static const INTERNET_PORT PORT;
	static const CString SERVER;
	static const CString OAUTH_TOKEN;
	static const CString ASSET_DATA;
	static const CString AUTH_URL_TEMPLATE;
	static const CString REDIRECT_URL;
	static const CString TRACKS_URL;
	static const CString CONNECTIONS_URL;
	static const CString AVATAR_URL;
};
