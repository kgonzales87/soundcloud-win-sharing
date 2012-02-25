// This source file was published as part of an article describing IE web browser customization:
// http://www.codeproject.com/script/Articles/ViewDownloads.aspx?aid=4758
// It is distributed under The Code Project Open License (CPOL) 1.02:
// http://www.codeproject.com/info/cpol10.aspx

#ifndef __CUSTOMMENUSH__
#define __CUSTOMMENUSH__

#ifndef USE_MFC7_HTMLVIEW_FEATURES
#include "Custsite.h"
#endif

enum WebContextMenuMode
{
	kDefaultMenuSupport = 0,
	kNoContextMenu,
	kTextSelectionOnly,
	kAllowAllButViewSource,
	kCustomMenuSupport,
	kWebContextMenuModeLimit
};

HRESULT CustomShowContextMenu(UINT mode, DWORD dwID, POINT *pptPosition,
					IUnknown *pCommandTarget, IDispatch *pDispatchObjectHit);

#endif
