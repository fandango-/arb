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

#include "fmpcb.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("sqrt....");
    fflush(stdout);

    flint_randinit(state);

    /* check (a^(1/2))^2 = a */
    for (iter = 0; iter < 10000; iter++)
    {
        fmpcb_t a, b, c;
        long prec;

        fmpcb_init(a);
        fmpcb_init(b);
        fmpcb_init(c);

        fmpcb_randtest(a, state, 1 + n_randint(state, 2000), 10);
        fmpcb_randtest(b, state, 1 + n_randint(state, 2000), 10);

        prec = 2 + n_randint(state, 2000);

        fmpcb_sqrt(b, a, prec);
        fmpcb_mul(c, b, b, prec);

        if (!fmpcb_contains(c, a))
        {
            printf("FAIL: containment\n\n");
            printf("a = "); fmpcb_print(a); printf("\n\n");
            printf("b = "); fmpcb_print(b); printf("\n\n");
            printf("c = "); fmpcb_print(c); printf("\n\n");
            abort();
        }

        fmpcb_sqrt(a, a, prec);
        if (!fmpcb_equal(a, b))
        {
            printf("FAIL: aliasing\n\n");
            printf("a = "); fmpcb_print(a); printf("\n\n");
            printf("b = "); fmpcb_print(b); printf("\n\n");
            abort();
        }

        fmpcb_clear(a);
        fmpcb_clear(b);
        fmpcb_clear(c);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}

