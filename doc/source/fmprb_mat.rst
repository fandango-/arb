fmprb_mat.h -- matrices of real number balls
===============================================================================

An *fmprb_mat_t* represents a dense matrix over the real numbers,
implemented as an array of entries of type *fmprb_struct*.

The dimension (number of rows and columns) of a matrix is fixed at
initialization, and the user must ensure that inputs and outputs to
an operation have compatible dimensions. The number of rows or columns
in a matrix can be zero.


Types, macros and constants
-------------------------------------------------------------------------------

.. type:: fmprb_mat_struct

.. type:: fmprb_mat_t

    Contains a pointer to a flat array of the entries (entries), an array of
    pointers to the start of each row (rows), and the number of rows (r)
    and columns (c).

    An *fmprb_mat_t* is defined as an array of length one of type
    *fmprb_mat_struct*, permitting an *fmprb_mat_t* to
    be passed by reference.

.. macro:: fmprb_mat_entry(mat, i, j)

    Macro giving a pointer to the entry at row *i* and column *j*.

.. macro:: fmprb_mat_nrows(mat)

    Returns the number of rows of the matrix.

.. macro:: fmprb_mat_ncols(mat)

    Returns the number of columns of the matrix.


Memory management
-------------------------------------------------------------------------------

.. function:: void fmprb_mat_init(fmprb_mat_t mat, long r, long c)

    Initializes the matrix, setting it to the zero matrix with *r* rows
    and *c* columns.

.. function:: void fmprb_mat_clear(fmprb_mat_t mat)

    Clears the matrix, deallocating all entries.


Conversions
-------------------------------------------------------------------------------

.. function:: void fmprb_mat_set(fmprb_mat_t dest, const fmprb_mat_t src)

.. function:: void fmprb_mat_set_fmpz_mat(fmprb_mat_t dest, const fmpz_mat_t src)

.. function:: void fmprb_mat_set_fmpq_mat(fmprb_mat_t dest, const fmpq_mat_t src, long prec)

    Sets *dest* to *src*. The operands must have identical dimensions.


Input and output
-------------------------------------------------------------------------------

.. function:: void fmprb_mat_printd(const fmprb_mat_t mat, long digits)

    Prints each entry in the matrix with the specified number of decimal digits.

Comparisons
-------------------------------------------------------------------------------

.. function:: int fmprb_mat_equal(const fmprb_mat_t mat1, const fmprb_mat_t mat2)

    Returns nonzero iff the matrices have the same dimensions
    and identical entries.

.. function:: int fmprb_mat_contains_fmpq_mat(const fmprb_mat_t mat1, const fmpq_mat_t mat2)

    Returns nonzero iff the matrices have the same dimensions and each entry
    in *mat2* is contained in the corresponding entry in *mat1*.


Special matrices
-------------------------------------------------------------------------------

.. function:: void fmprb_mat_zero(fmprb_mat_t mat)

    Sets all entries in mat to zero.

.. function:: void fmprb_mat_one(fmprb_mat_t mat)

    Sets the entries on the main diagonal to ones,
    and all other entries to zero.


Arithmetic
-------------------------------------------------------------------------------

.. function:: void fmprb_mat_neg(fmprb_mat_t dest, const fmprb_mat_t src)

    Sets *dest* to the exact negation of *src*. The operands must have
    the same dimensions.

.. function:: void fmprb_mat_add(fmprb_mat_t res, const fmprb_mat_t mat1, const fmprb_mat_t mat2, long prec)

    Sets res to the sum of *mat1* and *mat2*. The operands must have the same dimensions.

.. function:: void fmprb_mat_sub(fmprb_mat_t res, const fmprb_mat_t mat1, const fmprb_mat_t mat2, long prec)

    Sets *res* to the difference of *mat1* and *mat2*. The operands must have
    the same dimensions.

.. function:: void fmprb_mat_mul(fmprb_mat_t res, const fmprb_mat_t mat1, const fmprb_mat_t mat2, long prec)

    Sets *res* to the matrix product of *mat1* and *mat2*. The operands must have
    compatible dimensions for matrix multiplication.


Gaussian elimination and solving
-------------------------------------------------------------------------------

.. function:: int fmprb_mat_lu(long * perm, fmprb_mat_t LU, const fmprb_mat_t A, long prec)

    Given an `n \times n` matrix `A`, computes an LU decomposition `PLU = A`
    using Gaussian elimination with partial pivoting.
    The input and output matrices can be the same, performing the
    decomposition in-place.

    Entry `i` in the permutation vector perm is set to the row index in
    the input matrix corresponding to row `i` in the output matrix.

    The algorithm succeeds and returns nonzero if it can find `n` invertible
    (i.e. not containing zero) pivot entries. This guarantees that the matrix
    is invertible.

    The algorithm fails and returns zero, leaving the entries in `P` and `LU`
    undefined, if it cannot find `n` invertible pivot elements.
    In this case, either the matrix is singular, the input matrix was
    computed to insufficient precision, or the LU decomposition was
    attempted at insufficient precision.

.. function:: void fmprb_mat_solve_lu_precomp(fmprb_mat_t X, const long * perm, const fmprb_mat_t LU, const fmprb_mat_t B, long prec)

    Solves `AX = B` given the precomputed nonsingular LU decomposition `A = PLU`.
    The matrices `X` and `B` are allowed to be aliased with each other,
    but `X` is not allowed to be aliased with `LU`.

.. function:: int fmprb_mat_solve(fmprb_mat_t X, const fmprb_mat_t A, const fmprb_mat_t B, long prec)

    Solves `AX = B` where `A` is a nonsingular `n \times n` matrix
    and `X` and `B` are `n \times m` matrices, using LU decomposition.

    If `m > 0` and `A` cannot be inverted numerically (indicating either that
    `A` is singular or that the precision is insufficient), the values in the
    output matrix are left undefined and zero is returned. A nonzero return
    value guarantees that `A` is invertible and that the exact solution
    matrix is contained in the output.

.. function:: int fmprb_mat_inv(fmprb_mat_t X, const fmprb_mat_t A, long prec)

    Sets `X = A^{-1}` where `A` is a square matrix, computed by solving
    the system `AX = I`.

    If `A` cannot be inverted numerically (indicating either that
    `A` is singular or that the precision is insufficient), the values in the
    output matrix are left undefined and zero is returned.
    A nonzero return value guarantees that the matrix is invertible
    and that the exact inverse is contained in the output.

.. function:: void fmprb_mat_det(fmprb_t det, const fmprb_mat_t A, long prec)

    Computes the determinant of the matrix, using Gaussian elimination
    with partial pivoting. If at some point an invertible pivot element
    cannot be found, the elimination is stopped and the magnitude of the
    determinant of the remaining submatrix is bounded using
    Hadamard's inequality.
