/* Copyright (C) 2005-2026 Free Software Foundation, Inc.
   Contributed by Richard Henderson <rth@redhat.com>.

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

/* This is the MCF Gthread implementation of the public OpenMP
   locking primitives.  */

#include "libgomp.h"


void
gomp_init_lock_30 (omp_lock_t *lock)
{
  _MCF_sem_init (lock, 1);
}

void
gomp_destroy_lock_30 (omp_lock_t *lock)
{
  (void) lock;
}

void
gomp_set_lock_30 (omp_lock_t *lock)
{
  while (_MCF_sem_wait (lock, NULL) != 0)
    ;
}

void
gomp_unset_lock_30 (omp_lock_t *lock)
{
  _MCF_sem_signal (lock);
}

int
gomp_test_lock_30 (omp_lock_t *lock)
{
  int64_t timeout = 0;
  return _MCF_sem_wait (lock, &timeout) == 0;
}

void
gomp_init_nest_lock_30 (omp_nest_lock_t *lock)
{
  _MCF_sem_init (&lock->lock, 1);
  lock->count = 0;
  lock->owner = NULL;
}

void
gomp_destroy_nest_lock_30 (omp_nest_lock_t *lock)
{
  (void) lock;
}

void
gomp_set_nest_lock_30 (omp_nest_lock_t *lock)
{
  void *me = gomp_icv (true);

  if (lock->owner != me)
    {
      while (_MCF_sem_wait (&lock->lock, NULL) != 0)
	;
      lock->owner = me;
    }
  lock->count++;
}

void
gomp_unset_nest_lock_30 (omp_nest_lock_t *lock)
{
  if (--lock->count == 0)
    {
      lock->owner = NULL;
      _MCF_sem_signal (&lock->lock);
    }
}

int
gomp_test_nest_lock_30 (omp_nest_lock_t *lock)
{
  void *me = gomp_icv (true);

  if (lock->owner != me)
    {
      int64_t timeout = 0;
      if (_MCF_sem_wait (&lock->lock, &timeout) != 0)
	return 0;
      lock->owner = me;
    }

  return ++lock->count;
}

#ifdef LIBGOMP_GNU_SYMBOL_VERSIONING
void
gomp_init_lock_25 (omp_lock_25_t *lock)
{
  _MCF_mutex_init (lock);
}

void
gomp_destroy_lock_25 (omp_lock_25_t *lock)
{
  (void) lock;
}

void
gomp_set_lock_25 (omp_lock_25_t *lock)
{
  _MCF_mutex_lock (lock, NULL);
}

void
gomp_unset_lock_25 (omp_lock_25_t *lock)
{
  _MCF_mutex_unlock (lock);
}

int
gomp_test_lock_25 (omp_lock_25_t *lock)
{
  int64_t timeout = 0;
  return _MCF_mutex_lock (lock, &timeout) == 0;
}

void
gomp_init_nest_lock_25 (omp_nest_lock_25_t *lock)
{
  _MCF_mutex_init (&lock->lock);
  lock->count = 0;
  lock->owner = NULL;
}

void
gomp_destroy_nest_lock_25 (omp_nest_lock_25_t *lock)
{
  (void) lock;
}

void
gomp_set_nest_lock_25 (omp_nest_lock_25_t *lock)
{
  void *me = gomp_icv (true);

  if (lock->owner != me)
    {
      _MCF_mutex_lock (&lock->lock, NULL);
      lock->owner = me;
    }
  lock->count++;
}

void
gomp_unset_nest_lock_25 (omp_nest_lock_25_t *lock)
{
  if (--lock->count == 0)
    {
      lock->owner = NULL;
      _MCF_mutex_unlock (&lock->lock);
    }
}

int
gomp_test_nest_lock_25 (omp_nest_lock_25_t *lock)
{
  void *me = gomp_icv (true);

  if (lock->owner != me)
    {
      int64_t timeout = 0;
      if (_MCF_mutex_lock (&lock->lock, &timeout) != 0)
	return 0;
      lock->owner = me;
    }
  return ++lock->count;
}

omp_lock_symver (omp_init_lock)
omp_lock_symver (omp_destroy_lock)
omp_lock_symver (omp_set_lock)
omp_lock_symver (omp_unset_lock)
omp_lock_symver (omp_test_lock)
omp_lock_symver (omp_init_nest_lock)
omp_lock_symver (omp_destroy_nest_lock)
omp_lock_symver (omp_set_nest_lock)
omp_lock_symver (omp_unset_nest_lock)
omp_lock_symver (omp_test_nest_lock)

#else

ialias (omp_init_lock)
ialias (omp_init_nest_lock)
ialias (omp_destroy_lock)
ialias (omp_destroy_nest_lock)
ialias (omp_set_lock)
ialias (omp_set_nest_lock)
ialias (omp_unset_lock)
ialias (omp_unset_nest_lock)
ialias (omp_test_lock)
ialias (omp_test_nest_lock)

#endif
