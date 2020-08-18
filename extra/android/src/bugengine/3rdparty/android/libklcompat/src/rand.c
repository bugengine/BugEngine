extern long lrand48(void);
extern void srand48(long);

__attribute__((visibility("default")))
int rand(void)
{
    return (int)lrand48();
}

__attribute__((visibility("default")))
void srand(unsigned int __s)
{
    srand48(__s);
}
