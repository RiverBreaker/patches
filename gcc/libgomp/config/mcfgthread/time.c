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

/* This file contains system specific timer routines.  */

#include "libgomp.h"
#include <mcfgthread/clock.h>
#include <windows.h>


double
omp_get_wtime (void)
{
  /* _MCF_hires_steady_now returns milliseconds as a double.  */
  return _MCF_hires_steady_now () / 1000.0;
}

double
omp_get_wtick (void)
{
  /* MCF Gthread uses Windows QueryPerformanceCounter internally.
     Query the actual frequency to compute the true tick resolution.  */
  LARGE_INTEGER freq;
  if (QueryPerformanceFrequency (&freq) && freq.QuadPart > 0)
    return 1.0 / (double) freq.QuadPart;
  /* Fallback: 100 ns (typical Windows timer resolution).  */
  return 1e-7;
}

ialias (omp_get_wtime)
ialias (omp_get_wtick)
