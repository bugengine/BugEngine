/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_DESCRIPTION_SCRIPT_HH_
#define BE_RESOURCE_DESCRIPTION_SCRIPT_HH_
/*****************************************************************************/
#include    <minitl/refcountable.hh>
#include    <minitl/intrusive_list.hh>
#include    <resource/resource.hh>

namespace BugEngine { namespace Resource
{

class ILoader;
class ResourceManager;

class be_api(RESOURCE) Description : public minitl::refcountable, public minitl::intrusive_list<const Description, 2>::item
{
    friend class ResourceManager;
    BE_NOCOPY(Description);
private:
    enum
    {
        MaxResourceCount = 4
    };
    mutable Resource m_resources[MaxResourceCount];
private:
    void load(weak<ILoader> loader) const;
    void unload(weak<ILoader> loader) const;
protected:
    Description();
    ~Description();
public:
    const Resource& getResource(weak<const ILoader> owner) const;
    Resource& getResourceForWriting(weak<const ILoader> owner) const;
};

}}

/*****************************************************************************/
#endif