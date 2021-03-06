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

    Copyright (C) 2013 Fredrik Johansson

******************************************************************************/

#include "fmprb_poly.h"

void
_fmprb_poly_pow_ui(fmprb_ptr res, fmprb_srcptr f, long flen, ulong exp, long prec)
{
    _fmprb_poly_pow_ui_trunc_binexp(res, f, flen, exp, exp * (flen - 1) + 1, prec);
}

void
fmprb_poly_pow_ui(fmprb_poly_t res,
    const fmprb_poly_t poly, ulong exp, long prec)
{
    long flen, rlen;

    flen = poly->length;

    if (exp == 0)
    {
        fmprb_poly_one(res);
    }
    else if (flen == 0)
    {
        fmprb_poly_zero(res);
    }
    else
    {
        rlen = exp * (flen - 1) + 1;

        if (res != poly)
        {
            fmprb_poly_fit_length(res, rlen);
            _fmprb_poly_pow_ui(res->coeffs,
                poly->coeffs, flen, exp, prec);
            _fmprb_poly_set_length(res, rlen);
            _fmprb_poly_normalise(res);
        }
        else
        {
            fmprb_poly_t t;
            fmprb_poly_init2(t, rlen);
            _fmprb_poly_pow_ui(t->coeffs,
                poly->coeffs, flen, exp, prec);
            _fmprb_poly_set_length(t, rlen);
            _fmprb_poly_normalise(t);
            fmprb_poly_swap(res, t);
            fmprb_poly_clear(t);
        }
    }
}

