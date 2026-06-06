/* Copyright (C) 2025-2026 Free Software Foundation, Inc.

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

/* This is the MCF Gthread implementation of a semaphore synchronization
   mechanism for libgomp.  This type is private to the library.  */

#ifndef GOMP_SEM_H
#define GOMP_SEM_H 1

#ifdef HAVE_ATTRIBUTE_VISIBILITY
# pragma GCC visibility push(default)
#endif

#include <mcfgthread/sem.h>

#ifdef HAVE_ATTRIBUTE_VISIBILITY
# pragma GCC visibility pop
#endif

typedef _MCF_sem gomp_sem_t;

static inline void gomp_sem_init (gomp_sem_t *sem, int value)
{
  _MCF_sem_init (sem, (intptr_t) value);
}

extern void gomp_sem_wait (gomp_sem_t *sem);

static inline void gomp_sem_post (gomp_sem_t *sem)
{
  _MCF_sem_signal (sem);
}

static inline void gomp_sem_destroy (gomp_sem_t *sem)
{
  (void) sem;
}

static inline int gomp_sem_getcount (gomp_sem_t *sem)
{
  return (int) _MCF_sem_get (sem);
}

#endif /* GOMP_SEM_H */
