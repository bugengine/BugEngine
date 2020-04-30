/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <stdafx.h>
#include <bugengine/minitl/assert.hh>
#include <bugengine/plugin.debug.runtime/callstack.hh>
#include <bugengine/plugin.debug.runtime/module.hh>
#include <bugengine/plugin.debug.runtime/symbols.hh>

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <resource.h>

namespace BugEngine { namespace Debug {
HINSTANCE hDllInstance;

static INT_PTR CALLBACK AssertionDialogProc(HWND hwndDlg, UINT message, WPARAM wParam,
                                            LPARAM lParam)
{
    switch(message)
    {
    case WM_INITDIALOG:
    {
        const char** displayedtext = (const char**)lParam;
        (void)SendDlgItemMessage(hwndDlg, IDC_STATIC, WM_SETTEXT, 0, (LPARAM)displayedtext[0]);
        (void)SendDlgItemMessage(hwndDlg, IDC_EDITCALLSTACK, WM_SETTEXT, 0,
                                 (LPARAM)displayedtext[1]);
    }
        return TRUE;
    case WM_COMMAND:
        switch(wParam)
        {
        case IDC_BUTTONBREAK:
        case IDC_BUTTONIGNORE:
        case IDC_BUTTONIGNOREALL:
        case IDC_BUTTONABORT: (void)EndDialog(hwndDlg, (int)wParam); return TRUE;
        default: return FALSE;
        }
    default: return FALSE;
    }
}

static const int        BUFFER_SIZE = 4096 * 128;
static char             outmessage[BUFFER_SIZE];
static char             callstack[BUFFER_SIZE];
static char             buffer[BUFFER_SIZE];
minitl::AssertionResult AssertionCallback(const char* file, int line, const char* expr,
                                          const char* message)
{
    {
        (void)_snprintf(outmessage, BUFFER_SIZE - 1, "%s:%d : Assertion %s failed - %s\r\n", file,
                        line, expr, message);
        outmessage[BUFFER_SIZE - 1] = 0;
        OutputDebugString(outmessage);
    }

    *callstack = 0;

    char* dlgParams[2] = {outmessage, callstack};

    {
        Runtime::Callstack::Address address[128];
        size_t                      result = Runtime::Callstack::backtrace(address, 128, 1);

        ref< const Runtime::Module >                executable = Runtime::Module::self();
        static weak< const Runtime::Module >        last       = executable;
        static ref< const Runtime::SymbolResolver > s_symbols
           = Runtime::SymbolResolver::loadSymbols(executable->getSymbolInformation(),
                                                  ref< const Runtime::SymbolResolver >());
        while(last && last->next())
        {
            last                                              = last->next();
            Runtime::SymbolResolver::SymbolInformations infos = last->getSymbolInformation();
            s_symbols = Runtime::SymbolResolver::loadSymbols(infos, s_symbols);
        }
        for(Runtime::Callstack::Address* a = address; a < address + result; ++a)
        {
            Runtime::Symbol s;
            if(s_symbols)
            {
                s_symbols->resolve(*a, s);
            }
            (void)_snprintf(buffer, BUFFER_SIZE - 1, "[%s] %s(%d) : %s\r\n", s.module(),
                            s.filename(), s.line(), s.function());
            strcat(callstack, buffer);
            OutputDebugString(buffer);
        }
    }

    INT_PTR locr = DialogBoxParam(hDllInstance, MAKEINTRESOURCE(IDD_ASSERTDIALOG), 0,
                                  AssertionDialogProc, (LPARAM)&dlgParams[0]);
    if(locr == -1)
    {
        char* errorMessage;
        (void)::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
                              ::GetLastError(), 0, (LPTSTR)&errorMessage, 0, NULL);
        (void)MessageBox(0, outmessage, "Failed to open assertion dialog", MB_ICONERROR | MB_OK);
        (void)LocalFree(errorMessage);
        return minitl::Ignore;
    }
    else if(locr == IDC_BUTTONBREAK)
    {
        return minitl::Break;
    }
    else if(locr == IDC_BUTTONIGNORE)
        return minitl::Ignore;
    else if(locr == IDC_BUTTONIGNOREALL)
        return minitl::IgnoreAll;
    else if(locr == IDC_BUTTONABORT)
        return minitl::Abort;
    else
        return minitl::Ignore;
}

}}  // namespace BugEngine::Debug

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD /*reason*/, LPVOID /*reserved*/)
{
    BugEngine::Debug::hDllInstance = hInstance;
    return TRUE;
}
