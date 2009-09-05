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

#ifndef BE_CORE_DEBUG_SYMBOLS_HH_
#define BE_CORE_DEBUG_SYMBOLS_HH_
/*****************************************************************************/
#include    <core/debug/callstack.hh>

namespace BugEngine { namespace Debug
{

class Symbols
{
public:
    class Symbol;
    class Module;
    class ISymbolResolver;
public:
    class Symbol
    {
        friend class Symbols;
        friend class Module;
    private:
        char    m_filename[4096];
        char    m_function[4096];
        int     m_line;
    public:
        Symbol();
        ~Symbol();

        const char *filename() const    { return m_filename; }
        int         line() const        { return m_line; }
        const char *function() const    { return m_function; }
    };
public:
    class ISymbolResolver
    {
    public:
        virtual bool resolve(const Callstack::Address& address, Symbol& result) const = 0;
    };
public:
    class Module
    {
    private:
        const ISymbolResolver* m_symbols;
    public:
        Module(const char *filename, u64 baseAddress);
        ~Module();

        static std::vector<Module> enumerate();
        bool resolve(const Callstack::Address& address, Symbol& result) const;
    };
private:
    enum _TargetSelf { Self };
    Symbols(_TargetSelf self);
public:
    Symbols();
    ~Symbols();

    void addModule(const Module& m);
    void resolve(const Callstack::Address& address, Symbol& result) const;

    static const Symbols& runningSymbols();
private:
    std::vector<Module> m_modules;
};

}}

/*****************************************************************************/
#endif

