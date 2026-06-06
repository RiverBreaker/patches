/* Copyright (C) 2005-2026 Free Software Foundation, Inc.
   Contributed by Sebastian Huber <sebastian.huber@embedded-brains.de>.

   This file is part of the GNU Offloading and Multi Processing Library
   (libgomp).

   Libgomp is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* This is the MCF Gthread implementation of the thread pool management
   for libgomp.  This type is private to the library.  */

#ifndef GOMP_POOL_H
#define GOMP_POOL_H 1

#include "libgomp.h"
#include <mcfgthread/thread.h>

/* Get the thread pool, allocate and initialize it on demand.  */

static inline struct gomp_thread_pool *
gomp_get_thread_pool (struct gomp_thread *thr, unsigned nthreads)
{
  struct gomp_thread_pool *pool = thr->thread_pool;
  if (__builtin_expect (pool == NULL, 0))
    {
      pool = gomp_malloc (sizeof (*pool));
      pool->threads = NULL;
      pool->threads_size = 0;
      pool->threads_used = 0;
      pool->last_team = NULL;
      pool->threads_busy = nthreads;
      thr->thread_pool = pool;
      _MCF_tls_set (gomp_tls_destructor_key, thr);
    }
  return pool;
}

static inline void
gomp_release_thread_pool (struct gomp_thread_pool *pool)
{
  (void) pool;
}

static inline void *
gomp_adjust_thread_attr (void *attr, void *mutable_attr)
{
  (void) mutable_attr;
  return attr;
}

#endif /* GOMP_POOL_H */
