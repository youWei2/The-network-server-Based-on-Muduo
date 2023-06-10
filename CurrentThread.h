#ifndef MYMUDUO_CURRENTTHREAD
#define MYMUDUO_CURRENTTHREAD

#include <unistd.h>
#include <sys/syscall.h>

namespace mymuduo
{

namespace CurrentThread
{
    extern __thread int t_cachedTid;

    void cacheTid();

    inline int tid()
    {
        if(__builtin_expect(t_cachedTid == 0, 0))
        {
            cacheTid();
        }
        return t_cachedTid;
    }
}

}

#endif 