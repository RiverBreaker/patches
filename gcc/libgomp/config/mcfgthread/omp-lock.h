/* This header is used during the build process to find the size and
   alignment of the public OpenMP locks, so that we can export data
   structures without polluting the namespace.

   In this MCF Gthread implementation, we use _MCF_mutex for the basic
   lock and _MCF_sem for the OpenMP 3.0 lock.  */

#include <mcfgthread/mutex.h>
#include <mcfgthread/sem.h>

typedef _MCF_mutex omp_lock_25_t;
typedef struct { _MCF_mutex lock; int count; void *owner; } omp_nest_lock_25_t;
typedef _MCF_sem omp_lock_t;
typedef struct { _MCF_sem lock; int count; void *owner; } omp_nest_lock_t;
