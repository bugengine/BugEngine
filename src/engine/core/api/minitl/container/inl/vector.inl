/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_VECTOR_INL_
#define BE_MINITL_CONTAINER_VECTOR_INL_
/*****************************************************************************/

namespace minitl
{

vector::vector()
{
}

vector::~vector()
{
}

size_t vector::size() const
{
    return m_end - m_start;
}




}

/*****************************************************************************/
#endif
