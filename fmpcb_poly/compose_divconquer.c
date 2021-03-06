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

    Copyright (C) 2010 William Hart
    Copyright (C) 2012 Sebastian Pancratz
    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "fmpcb_poly.h"

void
_fmpcb_poly_compose_divconquer(fmpcb_ptr res, fmpcb_srcptr poly1, long len1,
                                          fmpcb_srcptr poly2, long len2, long prec)
{
    long i, j, k, n;
    long *hlen, alloc, powlen;
    fmpcb_ptr v, pow, temp;
    fmpcb_ptr * h;

    if (len1 == 1)
    {
        fmpcb_set(res, poly1);
        return;
    }
    if (len2 == 1)
    {
        _fmpcb_poly_evaluate(res, poly1, len1, poly2, prec);
        return;
    }
    if (len1 == 2)
    {
        _fmpcb_poly_compose_horner(res, poly1, len1, poly2, len2, prec);
        return;
    }

    /* Initialisation */
    
    hlen = (long *) flint_malloc(((len1 + 1) / 2) * sizeof(long));
    
    for (k = 1; (2 << k) < len1; k++) ;
    
    hlen[0] = hlen[1] = ((1 << k) - 1) * (len2 - 1) + 1;
    for (i = k - 1; i > 0; i--)
    {
        long hi = (len1 + (1 << i) - 1) / (1 << i);
        for (n = (hi + 1) / 2; n < hi; n++)
            hlen[n] = ((1 << i) - 1) * (len2 - 1) + 1;
    }
    powlen = (1 << k) * (len2 - 1) + 1;
    
    alloc = 0;
    for (i = 0; i < (len1 + 1) / 2; i++)
        alloc += hlen[i];

    v = _fmpcb_vec_init(alloc + 2 * powlen);
    h = (fmpcb_ptr *) flint_malloc(((len1 + 1) / 2) * sizeof(fmpcb_ptr));
    h[0] = v;
    for (i = 0; i < (len1 - 1) / 2; i++)
    {
        h[i + 1] = h[i] + hlen[i];
        hlen[i] = 0;
    }
    hlen[(len1 - 1) / 2] = 0;
    pow = v + alloc;
    temp = pow + powlen;
    
    /* Let's start the actual work */
    
    for (i = 0, j = 0; i < len1 / 2; i++, j += 2)
    {
        if (!fmpcb_is_zero(poly1 + j + 1))
        {
            _fmpcb_vec_scalar_mul(h[i], poly2, len2, poly1 + j + 1, prec);
            fmpcb_add(h[i], h[i], poly1 + j, prec);
            hlen[i] = len2;
        }
        else if (!fmpcb_is_zero(poly1 + j))
        {
            fmpcb_set(h[i], poly1 + j);
            hlen[i] = 1;
        }
    }
    if ((len1 & 1L))
    {
        if (!fmpcb_is_zero(poly1 + j))
        {
            fmpcb_set(h[i], poly1 + j);
            hlen[i] = 1;
        }
    }
    
    _fmpcb_poly_mul(pow, poly2, len2, poly2, len2, prec);
    powlen = 2 * len2 - 1;
    
    for (n = (len1 + 1) / 2; n > 2; n = (n + 1) / 2)
    {
        if (hlen[1] > 0)
        {
            long templen = powlen + hlen[1] - 1;
            _fmpcb_poly_mul(temp, pow, powlen, h[1], hlen[1], prec);
            _fmpcb_poly_add(h[0], temp, templen, h[0], hlen[0], prec);
            hlen[0] = FLINT_MAX(hlen[0], templen);
        }
        
        for (i = 1; i < n / 2; i++)
        {
            if (hlen[2*i + 1] > 0)
            {
                _fmpcb_poly_mul(h[i], pow, powlen, h[2*i + 1], hlen[2*i + 1], prec);
                hlen[i] = hlen[2*i + 1] + powlen - 1;
            } else
                hlen[i] = 0;
            _fmpcb_poly_add(h[i], h[i], hlen[i], h[2*i], hlen[2*i], prec);
            hlen[i] = FLINT_MAX(hlen[i], hlen[2*i]);
        }
        if ((n & 1L))
        {
            _fmpcb_vec_set(h[i], h[2*i], hlen[2*i]);
            hlen[i] = hlen[2*i];
        }
        
        _fmpcb_poly_mul(temp, pow, powlen, pow, powlen, prec);
        powlen += powlen - 1;
        {
            fmpcb_ptr t = pow;
            pow = temp;
            temp = t;
        }
    }

    _fmpcb_poly_mul(res, pow, powlen, h[1], hlen[1], prec);
    _fmpcb_vec_add(res, res, h[0], hlen[0], prec);
    
    _fmpcb_vec_clear(v, alloc + 2 * powlen);
    flint_free(h);
    flint_free(hlen);
}

void
fmpcb_poly_compose_divconquer(fmpcb_poly_t res,
                             const fmpcb_poly_t poly1, const fmpcb_poly_t poly2, long prec)
{
    const long len1 = poly1->length;
    const long len2 = poly2->length;
    
    if (len1 == 0)
    {
        fmpcb_poly_zero(res);
    }
    else if (len1 == 1 || len2 == 0)
    {
        fmpcb_poly_set_fmpcb(res, poly1->coeffs);
    }
    else
    {
        const long lenr = (len1 - 1) * (len2 - 1) + 1;
        
        if (res != poly1 && res != poly2)
        {
            fmpcb_poly_fit_length(res, lenr);
            _fmpcb_poly_compose_divconquer(res->coeffs, poly1->coeffs, len1,
                                                   poly2->coeffs, len2, prec);
        }
        else
        {
            fmpcb_poly_t t;
            fmpcb_poly_init2(t, lenr);
            _fmpcb_poly_compose_divconquer(t->coeffs, poly1->coeffs, len1,
                                                 poly2->coeffs, len2, prec);
            fmpcb_poly_swap(res, t);
            fmpcb_poly_clear(t);
        }

        _fmpcb_poly_set_length(res, lenr);
        _fmpcb_poly_normalise(res);
    }
}
