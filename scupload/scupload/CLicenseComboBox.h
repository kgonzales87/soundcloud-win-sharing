// Copyright (c) 2012, SoundCloud Ltd.

#pragma once

// Specialized ComboBox control initialized with pre-defined
// license data. It will also extend the drop-down menu
// to fit the displayed strings, which is not the default behavior
// of the CComboBox MFC control!
class CLicenseComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CLicenseComboBox)

public:
	CLicenseComboBox();
	virtual ~CLicenseComboBox();
	bool Select(LPCTSTR dataValue);
	void Init();
	LPCTSTR CLicenseComboBox::GetSelected();
protected:
	DECLARE_MESSAGE_MAP()
private:
	void UpdateDroppedWidth();
	static const CString dataValues[];
	static const CString displayValues[];
	static const int numOfEntries;
};


