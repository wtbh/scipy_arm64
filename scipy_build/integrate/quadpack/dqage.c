/* dqage.f -- translated by f2c (version 20190311).
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

/* Table of constant values */

static integer c__4 = 4;
static integer c__1 = 1;

/* Subroutine */ int dqage_(D_fp f, doublereal *a, doublereal *b, doublereal *
	epsabs, doublereal *epsrel, integer *key, integer *limit, doublereal *
	result, doublereal *abserr, integer *neval, integer *ier, doublereal *
	alist__, doublereal *blist, doublereal *rlist, doublereal *elist, 
	integer *iord, integer *last)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2;

    /* Local variables */
    static integer k;
    static doublereal a1, a2, b1, b2, area;
    extern /* Subroutine */ int dqk21_(D_fp, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), dqk31_(
	    D_fp, doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *), dqk41_(D_fp, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *), dqk15_(D_fp, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), dqk51_(
	    D_fp, doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *), dqk61_(D_fp, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *);
    static integer keyf;
    static doublereal area1, area2, area12, erro12, defab1, defab2;
    static integer nrmax;
    static doublereal uflow;
    extern doublereal d1mach_(integer *);
    static integer iroff1, iroff2;
    static doublereal error1, error2, defabs, epmach, errbnd, resabs, errmax;
    static integer maxerr;
    static doublereal errsum;
    extern /* Subroutine */ int dqpsrt_(integer *, integer *, integer *, 
	    doublereal *, doublereal *, integer *, integer *);

/* ***begin prologue  dqage */
/* ***date written   800101   (yymmdd) */
/* ***revision date  830518   (yymmdd) */
/* ***category no.  h2a1a1 */
/* ***keywords  automatic integrator, general-purpose, */
/*             integrand examinator, globally adaptive, */
/*             gauss-kronrod */
/* ***author  piessens,robert,appl. math. & progr. div. - k.u.leuven */
/*           de doncker,elise,appl. math. & progr. div. - k.u.leuven */
/* ***purpose  the routine calculates an approximation result to a given */
/*            definite integral   i = integral of f over (a,b), */
/*            hopefully satisfying following claim for accuracy */
/*            abs(i-reslt).le.max(epsabs,epsrel*abs(i)). */
/* ***description */

/*        computation of a definite integral */
/*        standard fortran subroutine */
/*        double precision version */

/*        parameters */
/*         on entry */
/*            f      - double precision */
/*                     function subprogram defining the integrand */
/*                     function f(x). the actual name for f needs to be */
/*                     declared e x t e r n a l in the driver program. */

/*            a      - double precision */
/*                     lower limit of integration */

/*            b      - double precision */
/*                     upper limit of integration */

/*            epsabs - double precision */
/*                     absolute accuracy requested */
/*            epsrel - double precision */
/*                     relative accuracy requested */
/*                     if  epsabs.le.0 */
/*                     and epsrel.lt.max(50*rel.mach.acc.,0.5d-28), */
/*                     the routine will end with ier = 6. */

/*            key    - integer */
/*                     key for choice of local integration rule */
/*                     a gauss-kronrod pair is used with */
/*                          7 - 15 points if key.lt.2, */
/*                         10 - 21 points if key = 2, */
/*                         15 - 31 points if key = 3, */
/*                         20 - 41 points if key = 4, */
/*                         25 - 51 points if key = 5, */
/*                         30 - 61 points if key.gt.5. */

/*            limit  - integer */
/*                     gives an upperbound on the number of subintervals */
/*                     in the partition of (a,b), limit.ge.1. */

/*         on return */
/*            result - double precision */
/*                     approximation to the integral */

/*            abserr - double precision */
/*                     estimate of the modulus of the absolute error, */
/*                     which should equal or exceed abs(i-result) */

/*            neval  - integer */
/*                     number of integrand evaluations */

/*            ier    - integer */
/*                     ier = 0 normal and reliable termination of the */
/*                             routine. it is assumed that the requested */
/*                             accuracy has been achieved. */
/*                     ier.gt.0 abnormal termination of the routine */
/*                             the estimates for result and error are */
/*                             less reliable. it is assumed that the */
/*                             requested accuracy has not been achieved. */
/*            error messages */
/*                     ier = 1 maximum number of subdivisions allowed */
/*                             has been achieved. one can allow more */
/*                             subdivisions by increasing the value */
/*                             of limit. */
/*                             however, if this yields no improvement it */
/*                             is rather advised to analyze the integrand */
/*                             in order to determine the integration */
/*                             difficulties. if the position of a local */
/*                             difficulty can be determined(e.g. */
/*                             singularity, discontinuity within the */
/*                             interval) one will probably gain from */
/*                             splitting up the interval at this point */
/*                             and calling the integrator on the */
/*                             subranges. if possible, an appropriate */
/*                             special-purpose integrator should be used */
/*                             which is designed for handling the type of */
/*                             difficulty involved. */
/*                         = 2 the occurrence of roundoff error is */
/*                             detected, which prevents the requested */
/*                             tolerance from being achieved. */
/*                         = 3 extremely bad integrand behaviour occurs */
/*                             at some points of the integration */
/*                             interval. */
/*                         = 6 the input is invalid, because */
/*                             (epsabs.le.0 and */
/*                              epsrel.lt.max(50*rel.mach.acc.,0.5d-28), */
/*                             result, abserr, neval, last, rlist(1) , */
/*                             elist(1) and iord(1) are set to zero. */
/*                             alist(1) and blist(1) are set to a and b */
/*                             respectively. */

/*            alist   - double precision */
/*                      vector of dimension at least limit, the first */
/*                       last  elements of which are the left */
/*                      end points of the subintervals in the partition */
/*                      of the given integration range (a,b) */

/*            blist   - double precision */
/*                      vector of dimension at least limit, the first */
/*                       last  elements of which are the right */
/*                      end points of the subintervals in the partition */
/*                      of the given integration range (a,b) */

/*            rlist   - double precision */
/*                      vector of dimension at least limit, the first */
/*                       last  elements of which are the */
/*                      integral approximations on the subintervals */

/*            elist   - double precision */
/*                      vector of dimension at least limit, the first */
/*                       last  elements of which are the moduli of the */
/*                      absolute error estimates on the subintervals */

/*            iord    - integer */
/*                      vector of dimension at least limit, the first k */
/*                      elements of which are pointers to the */
/*                      error estimates over the subintervals, */
/*                      such that elist(iord(1)), ..., */
/*                      elist(iord(k)) form a decreasing sequence, */
/*                      with k = last if last.le.(limit/2+2), and */
/*                      k = limit+1-last otherwise */

/*            last    - integer */
/*                      number of subintervals actually produced in the */
/*                      subdivision process */

/* ***references  (none) */
/* ***routines called  d1mach,dqk15,dqk21,dqk31, */
/*                    dqk41,dqk51,dqk61,dqpsrt */
/* ***end prologue  dqage */




/*            list of major variables */
/*            ----------------------- */

/*           alist     - list of left end points of all subintervals */
/*                       considered up to now */
/*           blist     - list of right end points of all subintervals */
/*                       considered up to now */
/*           rlist(i)  - approximation to the integral over */
/*                      (alist(i),blist(i)) */
/*           elist(i)  - error estimate applying to rlist(i) */
/*           maxerr    - pointer to the interval with largest */
/*                       error estimate */
/*           errmax    - elist(maxerr) */
/*           area      - sum of the integrals over the subintervals */
/*           errsum    - sum of the errors over the subintervals */
/*           errbnd    - requested accuracy max(epsabs,epsrel* */
/*                       abs(result)) */
/*           *****1    - variable for the left subinterval */
/*           *****2    - variable for the right subinterval */
/*           last      - index for subdivision */


/*           machine dependent constants */
/*           --------------------------- */

/*           epmach  is the largest relative spacing. */
/*           uflow  is the smallest positive magnitude. */

/* ***first executable statement  dqage */
    /* Parameter adjustments */
    --iord;
    --elist;
    --rlist;
    --blist;
    --alist__;

    /* Function Body */
    epmach = d1mach_(&c__4);
    uflow = d1mach_(&c__1);

/*           test on validity of parameters */
/*           ------------------------------ */

    *ier = 0;
    *neval = 0;
    *last = 0;
    *result = 0.;
    *abserr = 0.;
    alist__[1] = *a;
    blist[1] = *b;
    rlist[1] = 0.;
    elist[1] = 0.;
    iord[1] = 0;
/* Computing MAX */
    d__1 = epmach * 50.;
    if (*epsabs <= 0. && *epsrel < max(d__1,5e-29)) {
	*ier = 6;
    }
    if (*ier == 6) {
	goto L999;
    }

/*           first approximation to the integral */
/*           ----------------------------------- */

    keyf = *key;
    if (*key <= 0) {
	keyf = 1;
    }
    if (*key >= 7) {
	keyf = 6;
    }
    *neval = 0;
    if (keyf == 1) {
	dqk15_((D_fp)f, a, b, result, abserr, &defabs, &resabs);
    }
    if (keyf == 2) {
	dqk21_((D_fp)f, a, b, result, abserr, &defabs, &resabs);
    }
    if (keyf == 3) {
	dqk31_((D_fp)f, a, b, result, abserr, &defabs, &resabs);
    }
    if (keyf == 4) {
	dqk41_((D_fp)f, a, b, result, abserr, &defabs, &resabs);
    }
    if (keyf == 5) {
	dqk51_((D_fp)f, a, b, result, abserr, &defabs, &resabs);
    }
    if (keyf == 6) {
	dqk61_((D_fp)f, a, b, result, abserr, &defabs, &resabs);
    }
    *last = 1;
    rlist[1] = *result;
    elist[1] = *abserr;
    iord[1] = 1;

/*           test on accuracy. */

/* Computing MAX */
    d__1 = *epsabs, d__2 = *epsrel * abs(*result);
    errbnd = max(d__1,d__2);
    if (*abserr <= epmach * 50. * defabs && *abserr > errbnd) {
	*ier = 2;
    }
    if (*limit == 1) {
	*ier = 1;
    }
    if (*ier != 0 || *abserr <= errbnd && *abserr != resabs || *abserr == 0.) 
	    {
	goto L60;
    }

/*           initialization */
/*           -------------- */


    errmax = *abserr;
    maxerr = 1;
    area = *result;
    errsum = *abserr;
    nrmax = 1;
    iroff1 = 0;
    iroff2 = 0;

/*           main do-loop */
/*           ------------ */

    i__1 = *limit;
    for (*last = 2; *last <= i__1; ++(*last)) {

/*           bisect the subinterval with the largest error estimate. */

	a1 = alist__[maxerr];
	b1 = (alist__[maxerr] + blist[maxerr]) * .5;
	a2 = b1;
	b2 = blist[maxerr];
	if (keyf == 1) {
	    dqk15_((D_fp)f, &a1, &b1, &area1, &error1, &resabs, &defab1);
	}
	if (keyf == 2) {
	    dqk21_((D_fp)f, &a1, &b1, &area1, &error1, &resabs, &defab1);
	}
	if (keyf == 3) {
	    dqk31_((D_fp)f, &a1, &b1, &area1, &error1, &resabs, &defab1);
	}
	if (keyf == 4) {
	    dqk41_((D_fp)f, &a1, &b1, &area1, &error1, &resabs, &defab1);
	}
	if (keyf == 5) {
	    dqk51_((D_fp)f, &a1, &b1, &area1, &error1, &resabs, &defab1);
	}
	if (keyf == 6) {
	    dqk61_((D_fp)f, &a1, &b1, &area1, &error1, &resabs, &defab1);
	}
	if (keyf == 1) {
	    dqk15_((D_fp)f, &a2, &b2, &area2, &error2, &resabs, &defab2);
	}
	if (keyf == 2) {
	    dqk21_((D_fp)f, &a2, &b2, &area2, &error2, &resabs, &defab2);
	}
	if (keyf == 3) {
	    dqk31_((D_fp)f, &a2, &b2, &area2, &error2, &resabs, &defab2);
	}
	if (keyf == 4) {
	    dqk41_((D_fp)f, &a2, &b2, &area2, &error2, &resabs, &defab2);
	}
	if (keyf == 5) {
	    dqk51_((D_fp)f, &a2, &b2, &area2, &error2, &resabs, &defab2);
	}
	if (keyf == 6) {
	    dqk61_((D_fp)f, &a2, &b2, &area2, &error2, &resabs, &defab2);
	}

/*           improve previous approximations to integral */
/*           and error and test for accuracy. */

	++(*neval);
	area12 = area1 + area2;
	erro12 = error1 + error2;
	errsum = errsum + erro12 - errmax;
	area = area + area12 - rlist[maxerr];
	if (defab1 == error1 || defab2 == error2) {
	    goto L5;
	}
	if ((d__1 = rlist[maxerr] - area12, abs(d__1)) <= abs(area12) * 1e-5 
		&& erro12 >= errmax * .99) {
	    ++iroff1;
	}
	if (*last > 10 && erro12 > errmax) {
	    ++iroff2;
	}
L5:
	rlist[maxerr] = area1;
	rlist[*last] = area2;
/* Computing MAX */
	d__1 = *epsabs, d__2 = *epsrel * abs(area);
	errbnd = max(d__1,d__2);
	if (errsum <= errbnd) {
	    goto L8;
	}

/*           test for roundoff error and eventually set error flag. */

	if (iroff1 >= 6 || iroff2 >= 20) {
	    *ier = 2;
	}

/*           set error flag in the case that the number of subintervals */
/*           equals limit. */

	if (*last == *limit) {
	    *ier = 1;
	}

/*           set error flag in the case of bad integrand behaviour */
/*           at a point of the integration range. */

/* Computing MAX */
	d__1 = abs(a1), d__2 = abs(b2);
	if (max(d__1,d__2) <= (epmach * 100. + 1.) * (abs(a2) + uflow * 1e3)) 
		{
	    *ier = 3;
	}

/*           append the newly-created intervals to the list. */

L8:
	if (error2 > error1) {
	    goto L10;
	}
	alist__[*last] = a2;
	blist[maxerr] = b1;
	blist[*last] = b2;
	elist[maxerr] = error1;
	elist[*last] = error2;
	goto L20;
L10:
	alist__[maxerr] = a2;
	alist__[*last] = a1;
	blist[*last] = b1;
	rlist[maxerr] = area2;
	rlist[*last] = area1;
	elist[maxerr] = error2;
	elist[*last] = error1;

/*           call subroutine dqpsrt to maintain the descending ordering */
/*           in the list of error estimates and select the subinterval */
/*           with the largest error estimate (to be bisected next). */

L20:
	dqpsrt_(limit, last, &maxerr, &errmax, &elist[1], &iord[1], &nrmax);
/* ***jump out of do-loop */
	if (*ier != 0 || errsum <= errbnd) {
	    goto L40;
	}
/* L30: */
    }

/*           compute final result. */
/*           --------------------- */

L40:
    *result = 0.;
    i__1 = *last;
    for (k = 1; k <= i__1; ++k) {
	*result += rlist[k];
/* L50: */
    }
    *abserr = errsum;
L60:
    if (keyf != 1) {
	*neval = (keyf * 10 + 1) * ((*neval << 1) + 1);
    }
    if (keyf == 1) {
	*neval = *neval * 30 + 15;
    }
L999:
    return 0;
} /* dqage_ */

