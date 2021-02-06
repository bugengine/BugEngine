/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/core/stdafx.h>
#include <bugengine/core/environment.hh>

#include <bugengine/core/string/istring.hh>

#include <cstring>

namespace BugEngine {

Environment& Environment::getEnvironment()
{
    static Environment s_environment;
    return s_environment;
}

ipath Environment::canonicalPath(const char* path, const char* pathSeparators)
{
    ipath result;

    if(*path)
    {
        be_assert_recover(1 + strlen(pathSeparators) <= 4, "too many different path separators",
                          return result);
        char separators[4] = {0, 0, 0, 0};
        strcpy(separators, pathSeparators);

        const char* sep = path;
        while(*sep != separators[0] && *sep != separators[1] && *sep != separators[2]
              && *sep != separators[3])
            sep++;
        result.push_back(istring(path, sep));
        path = sep;
        if(*path) path++;
        while(*path)
        {
            sep = path;
            while(*sep != separators[0] && *sep != separators[1] && *sep != separators[2]
                  && *sep != separators[3])
                sep++;
            if(sep != path)
            {
                if(sep - path == 1 && path[0] == '.')
                {
                    // skip
                }
                else if(sep == path)
                {
                    // skip
                }
                else if(sep - path == 2 && path[0] == '.' && path[1] == '.' && result.size())
                {
                    result.pop_back();
                }
                else
                {
                    result.push_back(istring(path, sep));
                }
            }
            path = sep;
            if(*path) path++;
        }
    }
    return result;
}

}  // namespace BugEngine
