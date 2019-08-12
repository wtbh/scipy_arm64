/* idzr_rid.f -- translated by f2c (version 20100827).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/*       this file contains the following user-callable routines: */


/*       routine idzr_rid computes the ID, to a specified rank, */
/*       of a matrix specified by a routine for applying its adjoint */
/*       to arbitrary vectors. This routine is randomized. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int idzr_rid_(integer *m, integer *n, U_fp matveca, 
	doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, 
	doublecomplex *p4, integer *krank, integer *list, doublecomplex *proj)
{
    static integer ir, lr, ix, iy, lw, lx, ly;
    extern /* Subroutine */ int idzr_ridall0_(integer *, integer *, U_fp, 
	    doublecomplex *, doublecomplex *, doublecomplex *, doublecomplex *
	    , integer *, integer *, doublecomplex *, doublecomplex *, 
	    doublecomplex *);


/*       computes the ID of a matrix "a" specified by */
/*       the routine matveca -- matveca must apply the adjoint */
/*       of the matrix being ID'd to an arbitrary vector -- */
/*       i.e., the present routine lists in list the indices */
/*       of krank columns of a such that */

/*       a(j,list(k))  =  a(j,list(k)) */

/*       for all j = 1, ..., m; k = 1, ..., krank, and */

/*                       min(m,n,krank) */
/*       a(j,list(k))  =     Sigma      a(j,list(l)) * proj(l,k-krank)(*) */
/*                            l=1 */

/*                     +  epsilon(j,k-krank) */

/*       for all j = 1, ..., m; k = krank+1, ..., n, */

/*       for some matrix epsilon, dimensioned epsilon(m,n-krank), */
/*       whose norm is (hopefully) minimized by the pivoting procedure. */

/*       input: */
/*       m -- number of rows in the matrix to be ID'd */
/*       n -- number of columns in the matrix to be ID'd */
/*       matveca -- routine which applies the adjoint */
/*                  of the matrix to be ID'd to an arbitrary vector; */
/*                  this routine must have a calling sequence */
/*                  of the form */

/*                  matveca(m,x,n,y,p1,p2,p3,p4), */

/*                  where m is the length of x, */
/*                  x is the vector to which the adjoint */
/*                  of the matrix is to be applied, */
/*                  n is the length of y, */
/*                  y is the product of the adjoint of the matrix and x, */
/*                  and p1, p2, p3, and p4 are user-specified parameters */
/*       p1 -- parameter to be passed to routine matveca */
/*       p2 -- parameter to be passed to routine matveca */
/*       p3 -- parameter to be passed to routine matveca */
/*       p4 -- parameter to be passed to routine matveca */
/*       krank -- rank of the ID to be constructed */

/*       output: */
/*       list -- indices of the columns in the ID */
/*       proj -- matrix of coefficients needed to interpolate */
/*               from the selected columns to the other columns */
/*               in the original matrix being ID'd; */
/*               proj doubles as a work array in the present routine, so */
/*               proj must be at least m+(krank+3)*n complex*16 elements */
/*               long */

/*       _N.B._: The algorithm used by this routine is randomized. */
/*               proj must be at least m+(krank+3)*n complex*16 elements */
/*               long. */

/*       reference: */
/*       Halko, Martinsson, Tropp, "Finding structure with randomness: */
/*            probabilistic algorithms for constructing approximate */
/*            matrix decompositions," SIAM Review, 53 (2): 217-288, */
/*            2011. */



/*       Allocate memory in w. */

    /* Parameter adjustments */
    --list;
    --proj;

    /* Function Body */
    lw = 0;

    ir = lw + 1;
    lr = (*krank + 2) * *n;
    lw += lr;

    ix = lw + 1;
    lx = *m;
    lw += lx;

    iy = lw + 1;
    ly = *n;
    lw += ly;


    idzr_ridall0_(m, n, (U_fp)matveca, p1, p2, p3, p4, krank, &list[1], &
	    proj[ir], &proj[ix], &proj[iy]);


    return 0;
} /* idzr_rid__ */





/* Subroutine */ int idzr_ridall0_(integer *m, integer *n, S_fp matveca, 
	doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, 
	doublecomplex *p4, integer *krank, integer *list, doublecomplex *r__, 
	doublecomplex *x, doublecomplex *y)
{
    /* System generated locals */
    integer r_dim1, r_offset, i__1, i__2, i__3;
    doublecomplex z__1;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    extern /* Subroutine */ int id_srand_(integer *, doublecomplex *);
    static integer j, k, l, m2;
    extern /* Subroutine */ int idzr_id_(integer *, integer *, doublecomplex 
	    *, integer *, integer *, doublecomplex *);


/*       routine idzr_ridall serves as a memory wrapper */
/*       for the present routine */
/*       (see idzr_ridall for further documentation). */



/*       Set the number of random test vectors to 2 more than the rank. */

    /* Parameter adjustments */
    --x;
    --y;
    --list;
    r_dim1 = *krank + 2;
    r_offset = 1 + r_dim1;
    r__ -= r_offset;

    /* Function Body */
    l = *krank + 2;

/*       Apply the adjoint of the original matrix to l random vectors. */

    i__1 = l;
    for (j = 1; j <= i__1; ++j) {

/*         Generate a random vector. */

	m2 = *m << 1;
	id_srand_(&m2, &x[1]);

/*         Apply the adjoint of the matrix to x, obtaining y. */

	(*matveca)(m, &x[1], n, &y[1], p1, p2, p3, p4);

/*         Copy the conjugate of y into row j of r. */

	i__2 = *n;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = j + k * r_dim1;
	    d_cnjg(&z__1, &y[k]);
	    r__[i__3].r = z__1.r, r__[i__3].i = z__1.i;
	}

/* k */
    }


/*       ID r. */

/* j */
    idzr_id_(&l, n, &r__[r_offset], krank, &list[1], &y[1]);


    return 0;
} /* idzr_ridall0__ */

