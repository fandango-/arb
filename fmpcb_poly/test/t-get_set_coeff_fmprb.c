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

    Copyright (C) 2009 William Hart
    Copyright (C) 2013 Fredrik Johansson

******************************************************************************/

#include "fmpcb_poly.h"

int
main(void)
{
    int i, j, result;
    flint_rand_t state;

    printf("get/set_coeff_fmpcb....");
    fflush(stdout);

    flint_randinit(state);

    for (i = 0; i < 100; i++)
    {
        fmpcb_poly_t a;
        fmpcb_t x1, x2;
        slong coeff, len;

        fmpcb_poly_init(a);
        fmpcb_init(x1);
        fmpcb_init(x2);
        len = n_randint(state, 100) + 1;

        for (j = 0; j < 100; j++)
        {
            fmpcb_randtest(x1, state, 2 + n_randint(state, 200), 10);
            coeff = n_randint(state, len);
            fmpcb_poly_set_coeff_fmpcb(a, coeff, x1);
            fmpcb_poly_get_coeff_fmpcb(x2, a, coeff);

            result = (fmpcb_equal(x1, x2));
            if (!result)
            {
                printf("FAIL:\n");
                printf("x1 = "), fmpcb_print(x1), printf("\n");
                printf("x2 = "), fmpcb_print(x2), printf("\n");
                printf("coeff = %ld, length = %ld\n", coeff, len);
                abort();
            }
        }

        fmpcb_clear(x1);
        fmpcb_clear(x2);
        fmpcb_poly_clear(a);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return 0;
}

