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

#include <math.h>
#include "arith.h"
#include "zeta.h"

void
zeta_ui(fmprb_t x, ulong n, long prec)
{
    if (n == 0)
    {
        fmprb_set_si(x, -1);
        fmprb_mul_2exp_si(x, x, -1);
    }
    else if (n == 1)
    {
        printf("exception: zeta_ui(1)\n");
        abort();
    }
    /* fast detection of asymptotic case */
    else if (n > 0.7 * prec)
    {
        zeta_ui_asymp(x, n, prec);
    }
    else
    {
        /* even */
        if (n % 2 == 0)
        {
            if (((prec < 10000) && (n < 40 + 0.11*prec)) ||
                ((prec >= 10000) && (arith_bernoulli_number_size(n) * 0.9 < prec)))
            {
                zeta_ui_bernoulli(x, n, prec);
            }
            else
            {
                zeta_ui_euler_product(x, n, prec);
            }
        }
        else
        {
            if (n == 3)
            {
                zeta_apery_bsplit(x, prec);
            }
            else if (n < prec * 0.0006)
            {
                /* small odd n, extremely high precision */
                zeta_ui_borwein_bsplit(x, n, prec);
            }
            else if (prec > 20 && n > 6 && n > 0.4 * pow(prec, 0.8))
            {
                /* large n */
                zeta_ui_euler_product(x, n, prec);
            }
            else
            {
                /* fallback */
                zeta_ui_vec_borwein(x, n, 1, 0, prec);
            }
        }
    }
}

