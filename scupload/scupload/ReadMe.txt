// Copyright (c) 2012, SoundCloud Ltd.

Purpose of this document
------------------------
Describes how the SoundCloud app for Windows is built using Visual Studio C++ 2008.
and lists its dependencies to 3rd party open source components.

The main projects
-----------------

The source code of this app is organized in three folders:

scupload
    The Visual C++ Project which includes the main components
    for the SoundCloud app. It includes the project file (scupload/scupload.vcproj)
    which defines the build configurations (debug and release).

boost_1_47
    Some of the boost C++ libraries are linked by the SoundCloud app
    for regular expressions when parsing command-line arguments,
    HTTP responses or URL strings.
    The required boost libraries are not compiled from source. Instead,
    the compiled libraries are statically linked.
    The source and documentation of the boost library can be found here:
    http://www.boost.org/users/history/version_1_47_0.html
    It is distributed under the Boost Software License v1.0:
    http://www.boost.org/LICENSE_1_0.txt
 
jsoncpp-src-0.6.0-rc2
	JsonCpp is a JSON parser for C++. It is used to parse user profile data.
	JsonCpp is built from source and statically linked.
	The source and documentation of the JSON C++ library can be found here:
    http://jsoncpp.sourceforge.net/
    It is distributed as open source software (Public Domain or MIT License)
    as described here: http://jsoncpp.sourceforge.net/LICENSE 

Building the app
----------------
Open the solution file (scupload\scupload.sln) with Visual Studio C++ 2008.
This solution includes both the scupload project with the SoundCloud app source code
and the jsoncpp source code. Use the Configuration Manager to choose a Debug or Release
build configuration. The build configuration uses the multi-threaded C/C++ run-time library
and statically links the MFC library to create a single executable that depends only on
Windows platform features, e.g. Internet Explorer 6 SP2 or newer, see scupload/targetver.h.
Build and run, done.

Some important files
-------------------- 

scupload.vcproj
    This is the main project file for VC++ projects generated using an application wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    application wizard.

SCUploadApp.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    SCUploadApp application class.

SCUploadApp.cpp
    This is the main application source file that contains the application
    class SCUploadApp.

scupload.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in 1033.

scupload.rc2
    This file contains resources that are not edited by Microsoft
    Visual C++. You should place all resources not editable by
    the resource editor in this file.

scuploadDlg.h, scuploadDlg.cpp - the dialog
    These files contain your CscuploadDlg class.  This class defines
    the behavior of your application's main dialog.  The dialog's template is
    in scupload.rc, which can be edited in Microsoft Visual C++.

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named scupload.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

scupload.manifest
	Application manifest files are used by Windows XP to describe an applications
	dependency on specific versions of Side-by-Side assemblies. The loader uses this
	information to load the appropriate assembly from the assembly cache or private
	from the application. The Application manifest  maybe included for redistribution
	as an external .manifest file that is installed in the same folder as the application
	executable or it may be included in the executable in the form of a resource.

