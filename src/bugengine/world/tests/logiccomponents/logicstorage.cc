#include <bugengine/world/component/logiccomponentstorage.script.hh>

int main()
{
    using namespace BugEngine;
    using namespace BugEngine::World::Component;

    ref< StorageConfiguration >  config = ref< StorageConfiguration >::create(Arena::debug());
    ref< LogicComponentStorage > storage
        = ref< LogicComponentStorage >::create(Arena::debug(), config, be_class< u32 >());

    return 0;
}
