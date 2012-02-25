// Copyright (c) 2012, SoundCloud Ltd.

// CLicenseComboBox.cpp : implementation file

#include "stdafx.h"
#include "CLicenseComboBox.h"

IMPLEMENT_DYNAMIC(CLicenseComboBox, CComboBox)

const int CLicenseComboBox::numOfEntries = 9;
const CString CLicenseComboBox::dataValues[numOfEntries] = 
	{
		_T(""), // Default
		_T("no-rights-reserved"),
		_T("all-rights-reserved"),
		_T("cc-by"),
		_T("cc-by-nc"),
		_T("cc-by-nd"),
		_T("cc-by-sa"),
		_T("cc-by-nc-nd"),
		_T("cc-by-nc-sa")
	};

const CString CLicenseComboBox::displayValues[numOfEntries] = 
	{
		_T("Select your license"),
		_T("No rights reserved"),
		_T("All rights reserved"),
		_T("Creative Commons BY"),
		_T("Creative Commons BY-NC"),
		_T("Creative Commons BY-ND"),
		_T("Creative Commons BY-SA"),
		_T("Creative Commons BY-NC-ND"),
		_T("Creative Commons BY-NC-SA")
	};

CLicenseComboBox::CLicenseComboBox()
{
}

CLicenseComboBox::~CLicenseComboBox()
{
}

void CLicenseComboBox::Init() 
{
	for(int i = 0; i < numOfEntries; i++)
		AddString(displayValues[i]);

	UpdateDroppedWidth();
}

/**
 * Selects the given license or "All rights reserved" if the license is NULL or unknown.
 */
bool CLicenseComboBox::Select(LPCTSTR dataValue)
{
	if(dataValue != NULL)
	{
		for(int i = 0; i < numOfEntries; i++)
		{
			if(dataValue == dataValues[i])
			{
				SetCurSel(i);
				return true;
			}
		}
	}
	SetCurSel(0); // Default license
	return false;
}

LPCTSTR CLicenseComboBox::GetSelected()
{
	return dataValues[GetCurSel()];
}

BEGIN_MESSAGE_MAP(CLicenseComboBox, CComboBox)
END_MESSAGE_MAP()

void CLicenseComboBox::UpdateDroppedWidth()
{
    int maxW = 0;
	CString s;
    CSize size;
    CDC* pDc = GetDC();
	CFont* pOriginalFont = pDc->GetCurrentFont();
	pDc->SelectObject(GetFont());
    for (int i = 0; i < GetCount(); i++)
    {
        GetLBText(i, s);
        size = pDc->GetTextExtent(s);
        if(size.cx > maxW)
            maxW = size.cx;
    }
    ReleaseDC(pDc);

    // Width for vertical scroll bar and borders
    maxW += 2 * ::GetSystemMetrics(SM_CXEDGE) + ::GetSystemMetrics(SM_CXVSCROLL);
    SetDroppedWidth(maxW);

	//Reset the device context's original font
	pDc->SelectObject(pOriginalFont);
}




