/***************************************************************************
 * mgl_define.h is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
//-----------------------------------------------------------------------------
#ifndef _MGL_DEFINE_H_
#define _MGL_DEFINE_H_
//-----------------------------------------------------------------------------
#ifdef WIN32 //_MSC_VER needs this before math.h 
#define	_USE_MATH_DEFINES
#endif

#include <math.h>

#ifdef WIN32
#include <float.h>
const unsigned long mgl_nan[2] = {0xffffffff, 0x7fffffff};
#define NANd	(*(double*)mgl_nan)
#define NANf	(*(float*)&(mgl_nan[1]))
#define NAN		NANd
#if defined(_MSC_VER)
#define chdir	_chdir // BORLAND has chdir
#endif
#endif

#ifndef M_PI
#define M_PI	3.14159265358979323846  /* pi */
#endif
//-----------------------------------------------------------------------------
#ifdef WIN32
#define mglprintf    _snwprintf
#else
#define mglprintf    swprintf
#endif
//-----------------------------------------------------------------------------
#ifndef isnan
#define isnan(a)		((a)!=(a))
#endif
//-----------------------------------------------------------------------------
#include <mgl/config.h>
//-----------------------------------------------------------------------------
#if(MGL_USE_DOUBLE==1)
typedef double mreal;
#else
typedef float mreal;
#endif
//-----------------------------------------------------------------------------
#define mgl_min(a,b)	(((a)>(b)) ? (b) : (a))
#define mgl_max(a,b)	(((a)>(b)) ? (a) : (b))
//-----------------------------------------------------------------------------
#define SMOOTH_NONE		0
#define SMOOTH_LINE_3	1
#define SMOOTH_LINE_5	2
#define SMOOTH_QUAD_5	3
//-----------------------------------------------------------------------------
#define MGL_HIST_IN		0
#define MGL_HIST_SUM	1
#define MGL_HIST_UP		2
#define MGL_HIST_DOWN	3
//-----------------------------------------------------------------------------
enum{	// types of predefined curvelinear coordinate systems
	mglCartesian = 0,	// no transformation
	mglPolar,
	mglSpherical,
	mglParabolic,
	mglParaboloidal,
	mglOblate,
	mglProlate,
	mglElliptic,
	mglToroidal,
	mglBispherical,
	mglBipolar
};
//-----------------------------------------------------------------------------
// types of vector plot
#define MGL_VEC_COL	1	// use 2 colors only
#define MGL_VEC_LEN	2	// use fixed length
#define MGL_VEC_END	4	// draw vector to point
#define MGL_VEC_MID	8	// draw vector with point at center
#define MGL_VEC_DOT	16	// draw dot instead of vector
//-----------------------------------------------------------------------------
enum{	// Codes for warnings/messages
	mglWarnNone = 0,// Everything OK
	mglWarnDim,		// Data dimension(s) is incompatible
	mglWarnLow, 	// Data dimension(s) is too small
	mglWarnNeg, 	// Minimal data value is negative
	mglWarnFile,	// No file or wrong data dimensions
	mglWarnMem,		// Not enough memory
	mglWarnZero,	// Data values are zero
	mglWarnLegA,	// Too many legend entries
	mglWarnLeg,		// No legend entries
	mglWarnSlc,		// Slice value is out of range
	mglWarnCnt,		// Number of contours is zero or negative
	mglWarnOpen,	// Couldn't open file
	mglWarnLId,		// Light: ID is out of range
	mglWarnSize,	// Setsize: size(s) is zero or negative
	mglWarnFmt,		// Format is not supported for that build
	mglWarnEnd		// Maximal number of warnings (must be last)
};
//-----------------------------------------------------------------------------
#ifndef MGL_DEF_PAL
#define MGL_DEF_PAL	"bgrcmyhlnqeupH"	// default palette
#endif
//-----------------------------------------------------------------------------
struct mglThread
{
	mreal *z;		// float* array with parameters
	long *p;		// long* array with parameters
	void *v;		// pointer to data/grapher
	int id;			// thread id
	int n;		// total number of iteration
//	unsigned char r[4];	// default color
	mreal res;		// result of operation
	mglThread();
	~mglThread();
};
/// Start several thread for the task
mreal mglStartThread(void *(*func)(void *), mreal (*fres)(mreal,mreal), int n, mreal *z, long *p, void *v);
#ifndef MGL_NTH_DEF
#define MGL_NTH_DEF		2
#endif
#ifndef HAVE_PTHREAD
#undef MGL_NTH_DEF
#define MGL_NTH_DEF		1
#endif
extern int mglNumThr;		///< Number of thread for plotting and data handling
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------