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
    int i, result;
    flint_rand_t state;

    printf("shift_left/right....");
    fflush(stdout);

    flint_randinit(state);

    /* Check aliasing of a and b for left shift */
    for (i = 0; i < 1000; i++)
    {
        fmpcb_poly_t a, b;
        long shift = n_randint(state, 100);

        fmpcb_poly_init(a);
        fmpcb_poly_init(b);
        fmpcb_poly_randtest(a, state, n_randint(state, 100), 2 + n_randint(state, 200), 10);

        fmpcb_poly_shift_left(b, a, shift);
        fmpcb_poly_shift_left(a, a, shift);

        result = (fmpcb_poly_equal(a, b));
        if (!result)
        {
            printf("FAIL:\n");
            fmpcb_poly_printd(a, 10), printf("\n\n");
            fmpcb_poly_printd(b, 10), printf("\n\n");
            abort();
        }

        fmpcb_poly_clear(a);
        fmpcb_poly_clear(b);
    }

    /* Check aliasing of a and b for right shift */
    for (i = 0; i < 1000; i++)
    {
        fmpcb_poly_t a, b;
        long shift = n_randint(state, 100);

        fmpcb_poly_init(a);
        fmpcb_poly_init(b);
        fmpcb_poly_randtest(a, state, n_randint(state, 100), 2 + n_randint(state, 200), 10);

        fmpcb_poly_shift_right(b, a, shift);
        fmpcb_poly_shift_right(a, a, shift);

        result = (fmpcb_poly_equal(a, b));
        if (!result)
        {
            printf("FAIL:\n");
            fmpcb_poly_printd(a, 10), printf("\n\n");
            fmpcb_poly_printd(b, 10), printf("\n\n");
            abort();
        }

        fmpcb_poly_clear(a);
        fmpcb_poly_clear(b);
    }

    /* Check shift left then right does nothing */
    for (i = 0; i < 1000; i++)
    {
        fmpcb_poly_t a, b, c;
        long shift = n_randint(state, 100);

        fmpcb_poly_init(a);
        fmpcb_poly_init(b);
        fmpcb_poly_init(c);
        fmpcb_poly_randtest(a, state, n_randint(state, 100), 2 + n_randint(state, 200), 10);

        fmpcb_poly_shift_left(b, a, shift);
        fmpcb_poly_shift_right(c, b, shift);

        result = (fmpcb_poly_equal(c, a));
        if (!result)
        {
            printf("FAIL:\n");
            fmpcb_poly_printd(a, 10), printf("\n\n");
            fmpcb_poly_printd(b, 10), printf("\n\n");
            fmpcb_poly_printd(c, 10), printf("\n\n");
            abort();
        }

        fmpcb_poly_clear(a);
        fmpcb_poly_clear(b);
        fmpcb_poly_clear(c);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return 0;
}

