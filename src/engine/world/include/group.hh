/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_GROUP_HH_
#define BE_WORLD_GROUP_HH_
/*****************************************************************************/

namespace BugEngine { namespace World
{

class World;

class Group : public minitl::refcountable
{
    friend class World;
private:
    struct ComponentStorage
    {
        weak<const RTTI::Class> componentType;
        scoped<Storage>         storage;
    };
    minitl::array< weak<Rule> >         m_rules;
    minitl::array< ComponentStorage >   m_storages;
    scoped<ITask>                       m_updateTask;
public:
    Group(const minitl::vector< weak<const Rule> >& groupedRules);
    ~Group();

    weak<Task>   updateTask() const;
};

}}


/*****************************************************************************/
#endif

