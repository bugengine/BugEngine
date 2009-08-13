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
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#include    <core/stdafx.h>

#if defined(BE_COMPILER_MSVC) || (defined(BE_COMPILER_INTEL) && defined(BE_PLATFORM_WIN32))


#include    <core/debug/callstack.hh>

#include    <winnt.h>
#include    <tlhelp32.h>
#include    <psapi.h>
#include    <DbgHelp.h>

namespace BugEngine { namespace Debug
{

static size_t   g_loadedModules = 0;

static void loadModules()
{
    HANDLE handle = GetCurrentProcess();
    HMODULE modules[1024];
    DWORD requiredSize;
    EnumProcessModules(handle, modules, sizeof(modules), &requiredSize);
    size_t countOfModulesLoaded = (requiredSize / sizeof(HMODULE));

    for(size_t i = g_loadedModules; i < countOfModulesLoaded; ++i)
    {
        char moduleName[MAX_PATH];
        if(! GetModuleBaseName(handle, modules[i], moduleName, sizeof(moduleName)))
        continue;

        MODULEINFO info;
        if(! GetModuleInformation(handle, modules[i], &info, sizeof(MODULEINFO)))
        continue;

        #ifdef _X86_
        if (! SymLoadModule(handle, NULL, (PSTR)moduleName, NULL,(DWORD)info.lpBaseOfDll , info.SizeOfImage) )
        #else
        if (! SymLoadModule(handle, NULL, (PSTR)moduleName, NULL,(DWORD64)info.lpBaseOfDll , info.SizeOfImage) )
        #endif
        {
            char buffer[255]; buffer[0] = '\0';
            strcat(buffer,"Failed to load symbols for module: ");
            strcat(buffer,moduleName);
            OutputDebugString(buffer);
        }
    }
    g_loadedModules = countOfModulesLoaded;
}

static void initSymbols()
{
    HANDLE handle = GetCurrentProcess();
    minitl::format<> symPath(".\\;");

    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS); // load line number information as well
    SymInitialize(handle, symPath, false);
    loadModules();

    g_symbolsInitalized = true;
}

SymbolResolver::SymbolResolver()
{
    initSymbols();
}

SymbolResolver::~SymbolResolver()
{
}

void SymbolResolver::fill(const Adress& a)
{
    if(!a.m_line)
    {
        DWORD64 Displacement;
        DWORD64 address = reinterpret_cast<DWORD64>(a.m_address);
        ULONG64 buffer[(sizeof(SYMBOL_INFO) + MAX_SYM_NAME*sizeof(CHAR) + sizeof(ULONG64) - 1) / sizeof(ULONG64)];
        PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYM_NAME;

        HANDLE handle = GetCurrentProcess();
        if (SymFromAddr(GetCurrentProcess(), address, &Displacement, symbol))
        {
            strncpy(a.m_function, symbol->Name, sizeof(a.m_function));
        }
        else
        {
            loadModules();
            if (SymFromAddr(GetCurrentProcess(), address, &Displacement, symbol))
            {
                strncpy(a.m_function, symbol->Name, sizeof(a.m_function));
            }
            else
            {
                strcpy(a.m_function, "???");
            }
        }

        DWORD disp = 0;
        IMAGEHLP_LINE lineInfo;
        memset(&lineInfo, 0, sizeof(lineInfo));
        lineInfo.SizeOfStruct = sizeof(lineInfo);

#ifdef _X86_
        if (SymGetLineFromAddr(handle, static_cast<DWORD>(address), &disp, &lineInfo))
#else
        if (SymGetLineFromAddr64(handle, static_cast<DWORD64>(address), &disp, &lineInfo))
#endif
        {
            a.m_line = lineInfo.LineNumber;
            strncpy(a.m_filename, lineInfo.FileName, sizeof(a.m_filename));
        }
        else
        {
            strcpy(a.m_filename, "???");
            a.m_line = 1;
        }
    }
}

}}

#endif

