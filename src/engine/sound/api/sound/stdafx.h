/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SOUND_STDAFX_H_
#define BE_SOUND_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <system/stdafx.h>

#include    <rtti/helper.hh>



#if defined(building_sound) || defined(SOUND_EXPORTS)
# define    SOUNDEXPORT         BE_EXPORT
#elif defined(sound_dll)
# define    SOUNDEXPORT         BE_IMPORT
#else
# define    SOUNDEXPORT
#endif

/*****************************************************************************/
#endif
