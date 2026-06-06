/* Copyright (C) 2005-2026 Free Software Foundation, Inc.

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

/* This file contains system specific routines related to counting
   online processors and dynamic load balancing.  */

#include "libgomp.h"
#include <mcfgthread/fwd.h>
#include <windows.h>


/* At startup, determine the default number of threads.  */

void
gomp_init_num_threads (void)
{
  gomp_global_icv.nthreads_var = _MCF_get_processor_count ();
}

/* When OMP_DYNAMIC is set, at thread launch determine the number of
   threads we should spawn for this team.  */

unsigned
gomp_dynamic_max_threads (void)
{
  unsigned n_onln;
  unsigned nthreads_var = gomp_icv (false)->nthreads_var;

  n_onln = _MCF_get_processor_count ();
  if (n_onln > nthreads_var)
    n_onln = nthreads_var;

  /* Windows does not provide getloadavg().  Use the system load
     information via GetSystemTimes() to approximate CPU utilization.
     idle_frac = idle_time / total_time; busy cores ~= n_onln * (1 - idle_frac).  */
  FILETIME idle_ft, kernel_ft, user_ft;
  if (GetSystemTimes (&idle_ft, &kernel_ft, &user_ft))
    {
      ULARGE_INTEGER idle, kernel, user;
      idle.LowPart = idle_ft.dwLowDateTime;
      idle.HighPart = idle_ft.dwHighDateTime;
      kernel.LowPart = kernel_ft.dwLowDateTime;
      kernel.HighPart = kernel_ft.dwHighDateTime;
      user.LowPart = user_ft.dwLowDateTime;
      user.HighPart = user_ft.dwHighDateTime;
      ULONGLONG total = kernel.QuadPart + user.QuadPart;
      if (total > 0)
	{
	  unsigned busy = (unsigned) ((n_onln * (total - idle.QuadPart)
				      + total / 2) / total);
	  if (busy >= n_onln)
	    busy = n_onln > 0 ? n_onln - 1 : 0;
	  n_onln -= busy;
	  /* Ensure at least 1 thread.  */
	  if (n_onln < 1)
	    n_onln = 1;
	}
    }

  return n_onln;
}

int
omp_get_num_procs (void)
{
  return _MCF_get_processor_count ();
}

ialias (omp_get_num_procs)
