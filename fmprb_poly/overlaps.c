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

int
_fmprb_poly_overlaps(fmprb_srcptr poly1, long len1,
        fmprb_srcptr poly2, long len2)
{
    long i;

    for (i = 0; i < len2; i++)
        if (!fmprb_overlaps(poly1 + i, poly2 + i))
            return 0;

    for (i = len2; i < len1; i++)
        if (!fmprb_contains_zero(poly1 + i))
            return 0;

    return 1;
}

int
fmprb_poly_overlaps(const fmprb_poly_t poly1, const fmprb_poly_t poly2)
{
    long len1 = poly1->length;
    long len2 = poly2->length;

    if (len1 >= len2)
        return _fmprb_poly_overlaps(poly1->coeffs, len1, poly2->coeffs, len2);
    else
        return _fmprb_poly_overlaps(poly2->coeffs, len2, poly1->coeffs, len1);
}

