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

    Copyright (C) 2012, 2013 Fredrik Johansson

******************************************************************************/

#include "fmprb_poly.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("riemann_siegel_theta_series....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 300; iter++)
    {
        long m, n1, n2, rbits1, rbits2, rbits3;
        fmprb_poly_t a, b, c, d;

        rbits1 = 2 + n_randint(state, 150);
        rbits2 = 2 + n_randint(state, 150);
        rbits3 = 2 + n_randint(state, 150);

        m = 1 + n_randint(state, 15);
        n1 = 1 + n_randint(state, 15);
        n2 = 1 + n_randint(state, 15);

        fmprb_poly_init(a);
        fmprb_poly_init(b);
        fmprb_poly_init(c);
        fmprb_poly_init(d);

        fmprb_poly_randtest(a, state, m, rbits1, 10);
        /* if (a->length != 0)
            fmprb_randtest_precise(a->coeffs, state, rbits1, 1); */
        fmprb_poly_randtest(b, state, m, rbits1, 10);
        fmprb_poly_randtest(c, state, m, rbits1, 10);

        fmprb_poly_riemann_siegel_theta_series(b, a, n1, rbits2);
        fmprb_poly_riemann_siegel_theta_series(c, a, n2, rbits3);

        fmprb_poly_set(d, b);
        fmprb_poly_truncate(d, FLINT_MIN(n1, n2));
        fmprb_poly_truncate(c, FLINT_MIN(n1, n2));

        if (!fmprb_poly_overlaps(c, d))
        {
            printf("FAIL\n\n");
            printf("n1 = %ld, n2 = %ld, bits2 = %ld, bits3 = %ld\n", n1, n2, rbits2, rbits3);

            printf("a = "); fmprb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); fmprb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); fmprb_poly_printd(c, 15); printf("\n\n");

            abort();
        }

        /* check theta(a) = -theta(-a) */
        fmprb_poly_neg(d, a);
        fmprb_poly_riemann_siegel_theta_series(c, d, n1, rbits2);
        fmprb_poly_neg(c, c);

        if (!fmprb_poly_overlaps(b, c))
        {
            printf("FAIL (symmetry, n1 = %ld)\n\n", n1);

            printf("a = "); fmprb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); fmprb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); fmprb_poly_printd(c, 15); printf("\n\n");
            printf("d = "); fmprb_poly_printd(d, 15); printf("\n\n");

            abort();
        }

        fmprb_poly_riemann_siegel_theta_series(a, a, n1, rbits2);
        if (!fmprb_poly_overlaps(a, b))
        {
            printf("FAIL (aliasing)\n\n");
            abort();
        }

        fmprb_poly_clear(a);
        fmprb_poly_clear(b);
        fmprb_poly_clear(c);
        fmprb_poly_clear(d);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}

