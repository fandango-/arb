/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "fmprb_poly.h"

void
_fmprb_poly_evaluate_vec_iter(fmprb_ptr ys, fmprb_srcptr poly, long plen,
    fmprb_srcptr xs, long n, long prec)
{
    long i;

    for (i = 0; i < n; i++)
        _fmprb_poly_evaluate(ys + i, poly, plen, xs + i, prec);
}

void
fmprb_poly_evaluate_vec_iter(fmprb_ptr ys,
        const fmprb_poly_t poly, fmprb_srcptr xs, long n, long prec)
{
    _fmprb_poly_evaluate_vec_iter(ys, poly->coeffs,
                                        poly->length, xs, n, prec);
}
