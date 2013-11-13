
// LikePannel.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LikePannel.h"
#include "LikePannelDlg.h"
#include <json/json.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLikePannelApp

BEGIN_MESSAGE_MAP(CLikePannelApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLikePannelApp construction

CLikePannelApp::CLikePannelApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CLikePannelApp object

CLikePannelApp theApp;


// CLikePannelApp initialization

BOOL CLikePannelApp::InitInstance()
{
	CWinApp::InitInstance();


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));



    std::string host = "127.0.0.1";
    std::string port = "8181";

    TCHAR path[MAX_PATH];
    ::GetModuleFileName(0, path, MAX_PATH);
    ::PathRemoveFileSpec(path);
    ::PathAppend(path, _T("LikePannel.json"));
    if (::PathFileExists(path) != FALSE) {
        CFile f(path, CFile::modeRead);
        const UINT size = static_cast<UINT>(f.GetLength());
        char* buf = new char[size];
        f.Read(buf, size);
        f.Close();

        Json::Value root(Json::objectValue);
        Json::Reader reader;
        if (reader.parse(buf, buf+size, root, false)) {
            Json::Value& param1 = root["host"];
            Json::Value& param2 = root["port"];
            if (param1.isString() && param2.isString()) {
                host = param1.asString();
                port = param2.asString();
            }
        }
        delete[] buf;
    }



	CLikePannelDlg dlg(host, port);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

