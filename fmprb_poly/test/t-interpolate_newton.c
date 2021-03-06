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

int main()
{
    long iter;
    flint_rand_t state;

    printf("interpolate_newton....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 10000; iter++)
    {
        long i, n, qbits1, qbits2, rbits1, rbits2, rbits3;
        fmpq_poly_t P;
        fmprb_poly_t R, S;
        fmpq_t t, u;
        fmprb_ptr xs, ys;

        fmpq_poly_init(P);
        fmprb_poly_init(R);
        fmprb_poly_init(S);
        fmpq_init(t);
        fmpq_init(u);

        qbits1 = 2 + n_randint(state, 200);
        qbits2 = 2 + n_randint(state, 5);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);
        rbits3 = 2 + n_randint(state, 200);

        fmpq_poly_randtest(P, state, 1 + n_randint(state, 20), qbits1);
        n = P->length;

        xs = _fmprb_vec_init(n);
        ys = _fmprb_vec_init(n);

        fmprb_poly_set_fmpq_poly(R, P, rbits1);

        if (n > 0)
        {
            fmpq_randtest(t, state, qbits2);
            fmprb_set_fmpq(xs, t, rbits2);

            for (i = 1; i < n; i++)
            {
                fmpq_randtest_not_zero(u, state, qbits2);
                fmpq_abs(u, u);
                fmpq_add(t, t, u);
                fmprb_set_fmpq(xs + i, t, rbits2);
            }
        }

        for (i = 0; i < n; i++)
            fmprb_poly_evaluate(ys + i, R, xs + i, rbits2);

        fmprb_poly_interpolate_newton(S, xs, ys, n, rbits3);

        if (!fmprb_poly_contains_fmpq_poly(S, P))
        {
            printf("FAIL:\n");
            printf("P = "); fmpq_poly_print(P); printf("\n\n");
            printf("R = "); fmprb_poly_printd(R, 15); printf("\n\n");
            printf("S = "); fmprb_poly_printd(S, 15); printf("\n\n");
            abort();
        }

        fmpq_poly_clear(P);
        fmprb_poly_clear(R);
        fmprb_poly_clear(S);
        fmpq_clear(t);
        fmpq_clear(u);
        _fmprb_vec_clear(xs, n);
        _fmprb_vec_clear(ys, n);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}
