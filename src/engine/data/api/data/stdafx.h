/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DATA_STDAFX_H_
#define BE_DATA_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <system/stdafx.h>

#include    <rtti/helper.hh>

#if defined(building_data) || defined(DATA_EXPORTS)
# define    DATAEXPORT          BE_EXPORT
#elif defined(data_dll)
# define    DATAEXPORT          BE_IMPORT
#else
# define    DATAEXPORT
#endif

/*****************************************************************************/
#endif
