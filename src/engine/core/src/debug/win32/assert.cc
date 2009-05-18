/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/

#include    <core/stdafx.h>
#include    <core/debug/assert.hh>
#include    <core/debug/callstack.hh>

#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>
#include    <malloc.h>

#include    "resource.h"


namespace BugEngine
{
    extern HINSTANCE hDllInstance;


    static INT_PTR CALLBACK AssertionDialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
    { 
        switch (message) 
        { 
        case WM_INITDIALOG:
            {
                const char **displayedtext = (const char **)lParam;
                (void)SendDlgItemMessage(hwndDlg,IDC_STATIC, WM_SETTEXT,0,(LPARAM)displayedtext[0]);
                (void)SendDlgItemMessage(hwndDlg,IDC_EDITCALLSTACK, WM_SETTEXT,0,(LPARAM)displayedtext[1]);
            }
            return TRUE;
        case WM_COMMAND: 
            switch (wParam)
            {
            case IDC_BUTTONBREAK:
            case IDC_BUTTONIGNORE:
            case IDC_BUTTONIGNOREALL:
            case IDC_BUTTONABORT:
                (void)EndDialog(hwndDlg, (int)wParam); 
                return TRUE; 
            default:
                return FALSE;
            }
        default:
            return FALSE;
        }
    } 


    static const int BUFFER_SIZE = 4096*128; 
    static char outmessage[BUFFER_SIZE];
    static char callstack[BUFFER_SIZE];
    static char buffer[BUFFER_SIZE];
    AssertionResult defaultAssertionCallback( const char *file,
                                              int        line,
                                              const char *message,
                                              ...)
    {
        {
            va_list l;
            va_start(l,message);
            char assertmsg[BUFFER_SIZE];
            (void)_vsnprintf(assertmsg,sizeof(outmessage)-1,message,l);
            assertmsg[sizeof(assertmsg)-1] = 0;
            va_end(l);

            (void)_snprintf(outmessage,BUFFER_SIZE-1,"%s:%d : %s\r\n", file, line, assertmsg);
            outmessage[BUFFER_SIZE-1] = 0;
            OutputDebugString(outmessage);
        }

        *callstack = 0;

        char *dlgParams[2] = { outmessage, callstack };

        {
            Callstack::Address address[128];
            size_t result = Callstack::backtrace(address, 128, 1);
            for(Callstack::Address* a = address; a < address+result; ++a)
            {
                (void)_snprintf(buffer, BUFFER_SIZE-1, "%s:%d : %s\r\n", a->filename(), a->line(), a->function());
                strcat(callstack, buffer);
                OutputDebugString(buffer);
            }
        }

        INT_PTR locr = DialogBoxParam( hDllInstance,
                                       MAKEINTRESOURCE(IDD_ASSERTDIALOG),
                                       0, 
                                       AssertionDialogProc,
                                       (LPARAM)&dlgParams[0]);
        if(locr == -1)
        {
            char *errorMessage;
            (void)::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                             NULL,
                             ::GetLastError(),
                             0,
                             (LPTSTR)&errorMessage,
                             0,
                             NULL);
            (void)MessageBox(0, outmessage, "Failed to open assertion dialog", MB_ICONERROR | MB_OK);
            (void)LocalFree(errorMessage);
            return Ignore;
        }
        else if (locr == IDC_BUTTONBREAK)
        {
            return Break;
        }
        else if (locr == IDC_BUTTONIGNORE)
            return Ignore;
        else if (locr == IDC_BUTTONIGNOREALL)
            return IgnoreAll;      
        else if (locr == IDC_BUTTONABORT)
            return Abort;
        else
            return Ignore;
    }

static AssertionCallback_t g_callback = defaultAssertionCallback;

AssertionCallback_t COREEXPORT setAssertionCallback(AssertionCallback_t callback)
{
    AssertionCallback_t previous = g_callback;
    g_callback = callback;
    return previous;
}

AssertionCallback_t getAssertionCallback()
{
    return g_callback;
}

}
