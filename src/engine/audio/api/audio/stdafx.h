/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_AUDIO_STDAFX_H_
#define BE_AUDIO_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <system/stdafx.h>



#if defined(building_audio) || defined(AUDIO_EXPORTS)
# define    AUDIOEXPORT         BE_EXPORT
#elif defined(audio_dll)
# define    AUDIOEXPORT         BE_IMPORT
#else
# define    AUDIOEXPORT
#endif

/*****************************************************************************/
#endif
