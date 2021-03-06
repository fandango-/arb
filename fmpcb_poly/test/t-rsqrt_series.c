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

#include "fmpcb_poly.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("rsqrt_series....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 5000; iter++)
    {
        long m, n, qbits, rbits1, rbits2;
        fmpq_poly_t A;
        fmpcb_poly_t a, b, c;

        qbits = 2 + n_randint(state, 200);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);

        m = 1 + n_randint(state, 30);
        n = 1 + n_randint(state, 30);

        fmpq_poly_init(A);
        fmpcb_poly_init(a);
        fmpcb_poly_init(b);
        fmpcb_poly_init(c);

        fmpq_poly_randtest_not_zero(A, state, m, qbits);
        fmpcb_poly_set_fmpq_poly(a, A, rbits1);

        fmpcb_poly_randtest(b, state, 1 + n_randint(state, 30), rbits1, 5);

        fmpcb_poly_rsqrt_series(b, a, n, rbits2);

        /* Check 1/((1/sqrt(a))^2) = a */
        fmpcb_poly_mullow(c, b, b, n, rbits2);
        fmpcb_poly_inv_series(c, c, n, rbits2);

        fmpq_poly_truncate(A, n);
        if (!fmpcb_poly_contains_fmpq_poly(c, A))
        {
            printf("FAIL\n\n");
            printf("bits2 = %ld\n", rbits2);

            printf("A = "); fmpq_poly_print(A); printf("\n\n");
            printf("a = "); fmpcb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); fmpcb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); fmpcb_poly_printd(c, 15); printf("\n\n");

            abort();
        }

        fmpcb_poly_rsqrt_series(a, a, n, rbits2);
        if (!fmpcb_poly_equal(a, b))
        {
            printf("FAIL (aliasing)\n\n");
            abort();
        }

        fmpq_poly_clear(A);
        fmpcb_poly_clear(a);
        fmpcb_poly_clear(b);
        fmpcb_poly_clear(c);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}

