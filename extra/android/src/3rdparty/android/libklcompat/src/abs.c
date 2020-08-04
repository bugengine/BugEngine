__attribute__((visibility("default")))
int abs(int __s)
{
    return __s < 0 ? -__s : __s;
}
