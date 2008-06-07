/* mgl_1d.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <mathgl.abalakin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <stdio.h>
#include <string.h>
#include "mgl/mgl.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"
//-----------------------------------------------------------------------------
//
//	Primitives
//
//-----------------------------------------------------------------------------
void mglGraph::Cone(mglPoint p1, mglPoint p2, float r1, float r2, const char *stl, bool edge)
{
	if(r2<0)	r2 = r1;
	if(r1==0 && r2==0)	return;		// zero radius is invisible
	int n = edge ? 4:2;
	mglData x(n), y(n), z(n), r(n);
	if(edge)
	{
		float d;
		d = (p2.x-p1.x)/1e3;	x.a[0] = p1.x-d;	x.a[1] = p1.x;	x.a[2] = p2.x;	x.a[3] = p2.x+d;
		d = (p2.y-p1.y)/1e3;	y.a[0] = p1.y-d;	y.a[1] = p1.y;	y.a[2] = p2.y;	y.a[3] = p2.y+d;
		d = (p2.z-p1.z)/1e3;	z.a[0] = p1.z-d;	z.a[1] = p1.z;	z.a[2] = p2.z;	z.a[3] = p2.z+d;
		r.a[0] = 0;	r.a[1] = r1;	r.a[2] = r2;	r.a[3] = 0;
	}
	else
	{
		x.a[0] = p1.x;	x.a[1] = p2.x;
		y.a[0] = p1.y;	y.a[1] = p2.y;
		z.a[0] = p1.z;	z.a[1] = p2.z;
		r.a[0] = r1;	r.a[1] = r2;
	}
	Tube(x,y,z,r,stl);
}
//-----------------------------------------------------------------------------
void mglGraph::Curve(mglPoint p1, mglPoint d1, mglPoint p2, mglPoint d2,
					const char *stl, int n)
{
	register long i;
	n = (n<2) ? 2 : n;
	float *pp = new float[3*n],a[3],b[3],s;
	bool *tt = new bool[n];
	SelectPen(stl);
	a[0] = 3*(p2.x-p1.x) - (d2.x+2*d1.x);	b[0] = d1.x+d2.x - 2*(p2.x-p1.x);
	a[1] = 3*(p2.y-p1.y) - (d2.y+2*d1.y);	b[1] = d1.y+d2.y - 2*(p2.y-p1.y);
	a[2] = 3*(p2.z-p1.z) - (d2.z+2*d1.z);	b[2] = d1.z+d2.z - 2*(p2.z-p1.z);

	for(i=0;i<n;i++)
	{
		s = i/(n-1.);
		pp[3*i+0] = p1.x+s*(d1.x+s*(a[0]+s*b[0]));
		pp[3*i+1] = p1.y+s*(d1.y+s*(a[1]+s*b[1]));
		pp[3*i+2] = p1.z+s*(d1.z+s*(a[2]+s*b[2]));
		tt[i] = ScalePoint(pp[3*i+0],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(n,pp,tt);
	delete []tt;	delete []pp;
}
//-----------------------------------------------------------------------------
void mglGraph::Line(mglPoint p1, mglPoint p2, const char *stl, int n)
{
	register long i;
	n = (n<2) ? 2 : n;
	float *pp = new float[3*n],s;
	bool *tt = new bool[n];
	SelectPen(stl);

	for(i=0;i<n;i++)
	{
		s = i/(n-1.);
		pp[3*i+0] = p1.x+s*(p2.x-p1.x);
		pp[3*i+1] = p1.y+s*(p2.y-p1.y);
		pp[3*i+2] = p1.z+s*(p2.z-p1.z);
		tt[i] = ScalePoint(pp[3*i+0],pp[3*i+1],pp[3*i+2]);
	}
	if(tt[0] && tt[n-1])	memset(tt,true,n*sizeof(bool));
	curv_plot(n,pp,tt);
	delete []pp;	delete []tt;
}
//-----------------------------------------------------------------------------
void mglGraph::Error(mglPoint p,mglPoint e,const char *pen)
{
	char mk=SelectPen(pen);
	ArrowSize /=2;
	Line(mglPoint(p.x+e.x,p.y,p.z),mglPoint(p.x-e.x,p.y,p.z),"-II");
	Line(mglPoint(p.x,p.y+e.y,p.z),mglPoint(p.x,p.y-e.y,p.z),"-II");
	Line(mglPoint(p.x,p.y,p.z+e.z),mglPoint(p.x,p.y,p.z-e.z),"-II");
	if(mk)	Mark(p,mk);
	ArrowSize *=2;
}
//-----------------------------------------------------------------------------
//
//	Plot series
//
//-----------------------------------------------------------------------------
void mglGraph::Plot(mglData &x, mglData &y, mglData &z, const char *pen)
{
	long j,m,mx,my,mz,n=y.nx;
	Arrow1 = Arrow2 = '_';
	char mk=0;
	if(x.nx!=n || z.nx!=n)	{	SetWarn(mglWarnDim,"Plot");	return;	}
	if(n<2)					{	SetWarn(mglWarnLow,"Plot");	return;	}
	m = x.ny > y.ny ? x.ny : y.ny;	m = z.ny > m ? z.ny : m;
	float *pp = new float[3*n];
	bool *tt = new bool[n];
	for(j=0;j<m;j++)
	{
		if(pen && *pen)	mk=SelectPen(pen);
		else	Pen(Pal[(CurrPal = (CurrPal+1)%NumPal)],'-',BaseLineWidth);
		mx = j<x.ny ? j:0;	my = j<y.ny ? j:0;	mz = j<z.ny ? j:0;
		register long i,k;
		for(i=0;i<n;i++)
		{
			k = 3*i;
			pp[k+0] = x.a[i+mx*n];
			pp[k+1] = y.a[i+my*n];
			pp[k+2] = z.a[i+mz*n];
			tt[i] = ScalePoint(pp[k],pp[k+1],pp[k+2]);
			if(mk && tt[i])	Mark(pp[k],pp[k+1],pp[k+2],mk);
		}
		curv_plot(n,pp,tt);
	}
	delete []tt;	delete []pp;
}
//-----------------------------------------------------------------------------
void mglGraph::Plot(mglData &x, mglData &y, const char *pen,float zVal)
{
	mglData z(y.nx);
	if(isnan(zVal))	zVal = Min.z;
	z.Fill(zVal,zVal);
	Plot(x,y,z,pen);
}
//-----------------------------------------------------------------------------
void mglGraph::Plot(mglData &y, const char *pen,float zVal)
{
	if(y.nx<2)	{	SetWarn(mglWarnLow,"Plot");	return;	}
	mglData x(y.nx);
	x.Fill(Min.x,Max.x);
	Plot(x,y,pen,zVal);
}
//-----------------------------------------------------------------------------
//
//	Area series
//
//-----------------------------------------------------------------------------
void mglGraph::Area(mglData &x, mglData &y, mglData &z, const char *pen)
{
	long i,j,n=y.nx,m,mx,my,mz;
	char mk=0;
	if(x.nx!=n || z.nx!=n)	{	SetWarn(mglWarnDim,"Area");	return;	}
	if(n<2)					{	SetWarn(mglWarnLow,"Area");	return;	}
	m = x.ny > y.ny ? x.ny : y.ny;	m = z.ny > m ? z.ny : m;
	float *pp = new float[6*n],z0=GetOrgZ('x');
	bool *tt = new bool[2*n];
	for(j=0;j<m;j++)
	{
		if(pen && *pen)	mk=SelectPen(pen);
		else	Pen(Pal[(CurrPal = (CurrPal+1)%NumPal)],'-',BaseLineWidth);
		DefColor(NC, -1);
		mx = j<x.ny ? j:0;	my = j<y.ny ? j:0;	mz = j<z.ny ? j:0;

		for(i=0;i<y.nx;i++)
		{
			pp[6*i+3] = pp[6*i+0] = x.a[i+mx*n];
			pp[6*i+4] = pp[6*i+1] = y.a[i+my*n];
			pp[6*i+2] = z.a[i+mz*n];
			tt[2*i] = ScalePoint(pp[6*i+0],pp[6*i+1],pp[6*i+2]);
			if(mk && tt[2*i])	Mark(pp[6*i+0],pp[6*i+1],pp[6*i+2],mk);
			pp[6*i+5] = z0;
			tt[2*i+1] = ScalePoint(pp[6*i+3],pp[6*i+4],pp[6*i+5]);
		}
		surf_plot(2,n,pp,0,tt);
	}
	delete []pp;	delete []tt;
}
//-----------------------------------------------------------------------------
void mglGraph::Area(mglData &x, mglData &y, const char *pen,bool sum,float zVal)
{
	long i,j,n=y.nx,m=y.ny,mx;
	char mk=0;
	if(x.nx!=n)	{	SetWarn(mglWarnDim,"Area");	return;	}
	if(n<2)		{	SetWarn(mglWarnLow,"Area");	return;	}
	float *pp = new float[6*n],y0=GetOrgY('x');
	bool *tt = new bool[2*n];
	if(isnan(zVal))	zVal = Min.z;
	mglData f(y);
	if(sum)	f.CumSum("y");

	for(j=0;j<m;j++)
	{
		if(pen && *pen)	mk=SelectPen(pen);
		else	Pen(Pal[(CurrPal = (CurrPal+1)%NumPal)],'-',BaseLineWidth);
		DefColor(NC, -1);
		mx = j<x.ny ? j:0;	//my = j<y.ny ? j:0;
		for(i=0;i<n;i++)
		{
			pp[6*i+3] = pp[6*i+0] = x.a[i+mx*n];
			pp[6*i+1] = f.a[i+j*n];
			pp[6*i+2] = zVal;
			tt[2*i+0] = ScalePoint(pp[6*i+0],pp[6*i+1],pp[6*i+2]);
			if(mk && tt[2*i])	Mark(pp[6*i+0],pp[6*i+1],pp[6*i+2],mk);
			pp[6*i+4] = sum && j>0 ? f.a[i+j*n-n] : y0;
			pp[6*i+5] = zVal;
			tt[2*i+1] = ScalePoint(pp[6*i+3],pp[6*i+4],pp[6*i+5]);
		}
		surf_plot(2,n,pp,0,tt);
	}
	delete []pp;	delete []tt;
}
//-----------------------------------------------------------------------------
void mglGraph::Area(mglData &y, const char *pen,bool sum,float zVal)
{
	if(y.nx<2)		{	SetWarn(mglWarnLow,"Area");	return;	}
	mglData x(y.nx);
	x.Fill(Min.x,Max.x);
	Area(x,y,pen,sum,zVal);
}
//-----------------------------------------------------------------------------
//
//	Step series
//
//-----------------------------------------------------------------------------
void mglGraph::Step(mglData &x, mglData &y, mglData &z, const char *pen)
{
	long i,j,m,mx,my,mz,n=y.nx;
	char mk=0;
	Arrow1 = Arrow2 = '_';
	if(x.nx!=n || z.nx!=n)	{	SetWarn(mglWarnDim,"Step");	return;	}
	if(n<2)					{	SetWarn(mglWarnLow,"Step");	return;	}
	m = x.ny > y.ny ? x.ny : y.ny;	m = z.ny > m ? z.ny : m;
	float *pp = new float[6*n];
	bool *tt = new bool[2*n];
	for(j=0;j<m;j++)
	{
		if(pen && *pen)	mk=SelectPen(pen);
		else	Pen(Pal[(CurrPal = (CurrPal+1)%NumPal)],'-',BaseLineWidth);
		mx = j<x.ny ? j:0;	my = j<y.ny ? j:0;	mz = j<z.ny ? j:0;
		for(i=0;i<n;i++)
		{
			pp[6*i+0] = x.a[i+mx*n];
			pp[6*i+1] = y.a[i+my*n];
			pp[6*i+2] = z.a[i+mz*n];
			tt[2*i] = ScalePoint(pp[6*i+0],pp[6*i+1],pp[6*i+2]);
			if(mk && tt[2*i])	Mark(pp[6*i+0],pp[6*i+1],pp[6*i+2],mk);
			if(i<n-1)
			{
				pp[6*i+3] = x.a[i+1+mx*n];
				pp[6*i+4] = y.a[i+1+my*n];
				pp[6*i+5] = z.a[i+mz*n];
				tt[2*i+1] = ScalePoint(pp[6*i+3],pp[6*i+4],pp[6*i+5]);
			}
		}
		curv_plot(2*n-1,pp,tt);
	}
	delete []tt;	delete []pp;
}
//-----------------------------------------------------------------------------
void mglGraph::Step(mglData &x, mglData &y, const char *pen,float zVal)
{
	long i,j,m,mx,my,n=y.nx;
	char mk=0;
	Arrow1 = Arrow2 = '_';
	if(x.nx!=n)	{	SetWarn(mglWarnDim,"Step");	return;	}
	if(n<2)		{	SetWarn(mglWarnLow,"Step");	return;	}
	m = x.ny > y.ny ? x.ny : y.ny;
	float *pp = new float[6*n];
	bool *tt = new bool[2*n];
	if(isnan(zVal))	zVal = Min.z;

	for(j=0;j<m;j++)
	{
		if(pen && *pen)	mk=SelectPen(pen);
		else	Pen(Pal[(CurrPal = (CurrPal+1)%NumPal)],'-',BaseLineWidth);
		mx = j<x.ny ? j:0;	my = j<y.ny ? j:0;
		for(i=0;i<n;i++)
		{
			pp[6*i+0] = x.a[i+mx*n];
			pp[6*i+1] = y.a[i+my*n];
			pp[6*i+2] = zVal;
			tt[2*i] = ScalePoint(pp[6*i+0],pp[6*i+1],pp[6*i+2]);
			if(mk && tt[2*i])	Mark(pp[6*i+0],pp[6*i+1],pp[6*i+2],mk);
			if(i<n-1)
			{
				pp[6*i+3] = x.a[i+1+mx*n];
				pp[6*i+4] = y.a[i+my*n];
				pp[6*i+5] = zVal;
				tt[2*i+1] = ScalePoint(pp[6*i+3],pp[6*i+4],pp[6*i+5]);
			}
		}
		curv_plot(2*n-1,pp,tt);
	}
	delete []tt;	delete []pp;
}
//-----------------------------------------------------------------------------
void mglGraph::Step(mglData &y, const char *pen,float zVal)
{
	if(y.nx<2)	{	SetWarn(mglWarnLow,"Step");	return;	}
	mglData x(y.nx);
	x.Fill(Min.x,Max.x);
	Step(x,y,pen,zVal);
}
//-----------------------------------------------------------------------------
//
//	Stem series
//
//-----------------------------------------------------------------------------
void mglGraph::Stem(mglData &x, mglData &y, mglData &z, const char *pen)
{
	long i,j,m,mx,my,mz,n=y.nx;
	char mk=0;
	Arrow1 = Arrow2 = '_';
	if(x.nx!=n || z.nx!=n)	{	SetWarn(mglWarnDim,"Stem");	return;	}
	if(n<2)					{	SetWarn(mglWarnLow,"Stem");	return;	}
	m = x.ny > y.ny ? x.ny : y.ny;	m = z.ny > m ? z.ny : m;
	float *pp = new float[6*n],z0=GetOrgZ('x');
	long *nn = new long[2*n];
	bool *tt = new bool[2*n];
	for(j=0;j<m;j++)
	{
		if(pen && *pen)	mk=SelectPen(pen);
		else	Pen(Pal[(CurrPal = (CurrPal+1)%NumPal)],'-',BaseLineWidth);
		mx = j<x.ny ? j:0;	my = j<y.ny ? j:0;	mz = j<z.ny ? j:0;
		for(i=0;i<n;i++)
		{
			pp[6*i+3] = pp[6*i+0] = x.a[i+mx*n];
			pp[6*i+4] = pp[6*i+1] = y.a[i+my*n];
			pp[6*i+2] = z.a[i+mz*n];
			tt[2*i] = ScalePoint(pp[6*i+0],pp[6*i+1],pp[6*i+2]);
			if(mk && tt[2*i])	Mark(pp[6*i+0],pp[6*i+1],pp[6*i+2],mk);
			pp[6*i+5] = z0;
			tt[2*i+1] = ScalePoint(pp[6*i+3],pp[6*i+4],pp[6*i+5]);
			nn[2*i] = 2*i+1;	nn[2*i+1] = -1;
		}
		curv_plot(2*n,pp,tt,nn);
	}
	delete []tt;	delete []pp;	delete []nn;
}
//-----------------------------------------------------------------------------
void mglGraph::Stem(mglData &x, mglData &y, const char *pen,float zVal)
{
	long i,j,m,mx,my,n=y.nx;
	char mk=0;
	Arrow1 = Arrow2 = '_';
	if(x.nx!=n)	{	SetWarn(mglWarnDim,"Stem");	return;	}
	if(n<2)		{	SetWarn(mglWarnLow,"Stem");	return;	}
	m = x.ny > y.ny ? x.ny : y.ny;
	float *pp = new float[6*n],y0=GetOrgY('x');
	long *nn = new long[2*n];
	bool *tt = new bool[2*n];
	if(isnan(zVal))	zVal = Min.z;

	for(j=0;j<m;j++)
	{
		if(pen && *pen)	mk=SelectPen(pen);
		else	Pen(Pal[(CurrPal = (CurrPal+1)%NumPal)],'-',BaseLineWidth);
		mx = j<x.ny ? j:0;	my = j<y.ny ? j:0;
		for(i=0;i<n;i++)
		{
			pp[6*i+3] = pp[6*i+0] = x.a[i+mx*n];
			pp[6*i+1] = y.a[i+my*n];
			pp[6*i+5] = pp[6*i+2] = zVal;
			tt[2*i] = ScalePoint(pp[6*i+0],pp[6*i+1],pp[6*i+2]);
			if(mk && tt[2*i])	Mark(pp[6*i+0],pp[6*i+1],pp[6*i+2],mk);
			pp[6*i+4] = y0;
			tt[2*i+1] = ScalePoint(pp[6*i+3],pp[6*i+4],pp[6*i+5]);
			nn[2*i] = 2*i+1;	nn[2*i+1] = -1;
		}
		curv_plot(2*n,pp,tt,nn);
	}
	delete []tt;	delete []pp;	delete []nn;
}
//-----------------------------------------------------------------------------
void mglGraph::Stem(mglData &y, const char *pen,float zVal)
{
	if(y.nx<2)	{	SetWarn(mglWarnLow,"Stem");	return;	}
	mglData x(y.nx);
	x.Fill(Min.x,Max.x);
	Stem(x,y,pen,zVal);
}
//-----------------------------------------------------------------------------
//
//	Bars series
//
//-----------------------------------------------------------------------------
void mglGraph::Bars(mglData &x, mglData &y, mglData &z, const char *pen, bool above)
{
	long i,j,m,mx,my,mz,n=y.nx;
	char mk=0;
	if(x.nx!=n || z.nx!=n)	{	SetWarn(mglWarnDim,"Bars");	return;	}
	if(n<2)					{	SetWarn(mglWarnLow,"Bars");	return;	}
	m = x.ny > y.ny ? x.ny : y.ny;	m = z.ny > m ? z.ny : m;
	float *pp = new float[12*n],x1,x2,y1,y2,z0=GetOrgZ('x'),zz;
	bool *tt = new bool[4*n];
	mglData dd(above ? n : 1);

	for(j=0;j<m;j++)
	{
		if(pen && *pen)	mk=SelectPen(pen);
		else	Pen(Pal[(CurrPal = (CurrPal+1)%NumPal)],'-',BaseLineWidth);
		DefColor(NC, -1);
		mx = j<x.ny ? j:0;	my = j<y.ny ? j:0;	mz = j<z.ny ? j:0;
		for(i=0;i<n;i++)
		{
			if(i<n-1)
			{
				x2 = x.a[i+n*mx] + BarWidth*(x.a[i+1+n*mx]-x.a[i+n*mx])/2;
				y2 = y.a[i+n*my] + BarWidth*(y.a[i+1+n*my]-y.a[i+n*my])/2;
			}
			else	{	x2 = x.a[i+n*mx];	y2 = y.a[i+n*my];	}
			if(i>0)
			{
				x1 = x.a[i+n*mx] - BarWidth*(x.a[i+n*mx]-x.a[i-1+n*mx])/2;
				y1 = y.a[i+n*my] - BarWidth*(y.a[i+n*my]-y.a[i-1+n*my])/2;
			}
			else	{	x1 = x.a[i+n*mx];	y1 = y.a[i+n*my];	}
			zz = z.a[i+n*mz];
			if(!above)
			{
				x2 = (x2-x1)/m;		x1 += j*x2;		x2 += x1;
				y2 = (y2-y1)/m;		y1 += j*y2;		y2 += y1;
			}
			else
			{	z0 = GetOrgZ('x') + dd.a[i];	dd.a[i] += zz;	zz += z0;	}

			pp[12*i+0] = x1;	pp[12*i+1] = y1;	pp[12*i+2] = zz;
			pp[12*i+3] = x1;	pp[12*i+4] = y1;	pp[12*i+5] = z0;
			pp[12*i+6] = x2;	pp[12*i+7] = y2;	pp[12*i+8] = z0;
			pp[12*i+9] = x2;	pp[12*i+10]= y2;	pp[12*i+11]= zz;
			tt[4*i]   = ScalePoint(pp[12*i+0],pp[12*i+1],pp[12*i+2]);
			tt[4*i+1] = ScalePoint(pp[12*i+3],pp[12*i+4],pp[12*i+5]);
			tt[4*i+2] = ScalePoint(pp[12*i+6],pp[12*i+7],pp[12*i+8]);
			tt[4*i+3] = ScalePoint(pp[12*i+9],pp[12*i+10],pp[12*i+11]);
		}
		quads_plot(n,pp,0,tt);
	}
	delete []tt;	delete []pp;
}
//-----------------------------------------------------------------------------
void mglGraph::Bars(mglData &x, mglData &y, const char *pen,float zVal, bool above)
{
	long i,j,m,mx,my,n=y.nx;
	char mk=0;
	if(x.nx!=n)	{	SetWarn(mglWarnDim,"Bars");	return;	}
	if(n<2)		{	SetWarn(mglWarnLow,"Bars");	return;	}
	m = x.ny > y.ny ? x.ny : y.ny;
	float *pp = new float[12*n],x1,x2,yy,y0=GetOrgY('x');
	bool *tt = new bool[4*n];
	if(isnan(zVal))	zVal = Min.z;
	mglData dd(above ? n : 1);

	for(j=0;j<m;j++)
	{
		if(pen && *pen)	mk=SelectPen(pen);
		else	Pen(Pal[(CurrPal = (CurrPal+1)%NumPal)],'-',BaseLineWidth);
		DefColor(NC, -1);
		mx = j<x.ny ? j:0;	my = j<y.ny ? j:0;
		for(i=0;i<n;i++)
		{
			if(i<n-1)	x2 = x.a[i+n*mx] + BarWidth*(x.a[i+1+n*mx]-x.a[i+n*mx])/2;
			else		x2 = x.a[i+n*mx];
			if(i>0)		x1 = x.a[i+n*mx] - BarWidth*(x.a[i+n*mx]-x.a[i-1+n*mx])/2;
			else		x1 = x.a[i+n*mx];
			yy = y.a[i+n*my];
			if(!above)
			{	x2 = (x2-x1)/m;		x1 += j*x2;		x2 += x1;	}
			else
			{	y0 = GetOrgY('x') + dd.a[i];	dd.a[i] += yy;	yy += y0;	}
			pp[12*i+0] = x1;	pp[12*i+1] = yy;	pp[12*i+2] = zVal;
			pp[12*i+3] = x1;	pp[12*i+4] = y0;	pp[12*i+5] = zVal;
			pp[12*i+6] = x2;	pp[12*i+7] = y0;	pp[12*i+8] = zVal;
			pp[12*i+9] = x2;	pp[12*i+10]= yy;	pp[12*i+11]= zVal;
			tt[4*i]   = ScalePoint(pp[12*i+0],pp[12*i+1],pp[12*i+2]);
			tt[4*i+1] = ScalePoint(pp[12*i+3],pp[12*i+4],pp[12*i+5]);
			tt[4*i+2] = ScalePoint(pp[12*i+6],pp[12*i+7],pp[12*i+8]);
			tt[4*i+3] = ScalePoint(pp[12*i+9],pp[12*i+10],pp[12*i+11]);
		}
		quads_plot(n,pp,0,tt);
	}
	delete []tt;	delete []pp;
}
//-----------------------------------------------------------------------------
void mglGraph::Bars(mglData &y, const char *pen,float zVal, bool above)
{
	if(y.nx<2)	{	SetWarn(mglWarnLow,"Bars");	return;	}
	mglData x(y.nx);
	x.Fill(Min.x,Max.x);
	Bars(x,y,pen,zVal,above);
}
//-----------------------------------------------------------------------------
//
//		Torus series
//
//-----------------------------------------------------------------------------
void mglGraph::Torus(mglData &r, mglData &z, const char *sch)
{
	long i,j,n=r.nx;
	if(r.nx*r.ny!=z.nx*z.ny){	SetWarn(mglWarnDim,"Torus");	return;	}
	if(n<2)					{	SetWarn(mglWarnLow,"Torus");	return;	}
	float *pp = new float[3*n];
	long *nn = new long[n];
	SetScheme(sch);
	if(sch)
	{
		if(strchr(sch,'x'))	AxialDir = 'x';
//		if(strchr(sch,'y'))	AxialDir = 'y';
		else if(strchr(sch,'z'))	AxialDir = 'z';
		else AxialDir = 'y';
	}

	Color(Cmin);
	memset(pp,0,3*n*sizeof(float));
	for(j=0;j<r.ny;j++)
	{
		for(i=0;i<n;i++)
		{
			nn[i] = i<n-1 ? i+1 : -1;
			pp[3*i]  = r.a[i+n*j];	pp[3*i+1]= z.a[i+n*j];
		}
		axial_plot(n,pp,nn,99,sch && strchr(sch,'#'));
	}
	delete []nn;	delete []pp;
}
//-----------------------------------------------------------------------------
void mglGraph::Torus(mglData &y, const char *pen)
{
	if(y.nx<2)	{	SetWarn(mglWarnLow,"Torus");	return;	}
	mglData x(y.nx);
	x.Fill(Min.x,Max.x);
	Torus(x,y,pen);
}
//-----------------------------------------------------------------------------
//
//		Error series
//
//-----------------------------------------------------------------------------
void mglGraph::Error(mglData &x, mglData &y, mglData &ex, mglData &ey,
					const char *pen,float zVal)
{
	register long i,j,m=1,n=y.nx*y.ny*y.nz;
	if(ex.nx*ex.ny*ex.nz!=n || ey.nx*ey.ny*ey.nz!=n || y.nx!=x.nx)
	{	SetWarn(mglWarnDim,"Error");	return;	}
	char mk=SelectPen(pen);
	if(isnan(zVal))	zVal = Min.z;
	if(n!=x.nx*x.ny*x.nz)
	{	m = y.ny*y.nz;	n = y.nx;	}
	ArrowSize /=2;
	for(j=0;j<m;j++)
	{
		for(i=0;i<n;i++)
		{
			Line(mglPoint(x.a[i],y.a[i+n*j]+ey.a[i+n*j],zVal),
					mglPoint(x.a[i],y.a[i+n*j]-ey.a[i+n*j],zVal),"-II",100);
			Line(mglPoint(x.a[i]+ex.a[i+n*j],y.a[i+n*j],zVal),
					mglPoint(x.a[i]-ex.a[i+n*j],y.a[i+n*j],zVal),"-II",100);
			if(mk)	Mark(mglPoint(x.a[i],y.a[i+n*j],zVal),mk);
		}
	}
	ArrowSize *=2;
}
//-----------------------------------------------------------------------------
void mglGraph::Error(mglData &x, mglData &y, mglData &ey, const char *pen,
					float zVal)
{
	if(y.nx*y.ny*y.nz!=ey.nx*ey.ny*ey.nz || y.nx!=x.nx)
	{	SetWarn(mglWarnDim,"Error");	return;	}
	char mk=SelectPen(pen);
	if(isnan(zVal))	zVal = Min.z;
	register long i,j,m,n;
	if(y.nx*y.ny*y.nz!=x.nx*x.ny*x.nz)
	{	m = y.ny*y.nz;	n = y.nx;	}
	else
	{	m = 1;	n = y.nx*y.ny*y.nz;	}
	ArrowSize /=2;
	for(j=0;j<m;j++)
	{
		for(i=0;i<n;i++)
		{
			Line(mglPoint(x.a[i],y.a[i+n*j]+ey.a[i+n*j],zVal),
					mglPoint(x.a[i],y.a[i+n*j]-ey.a[i+n*j],zVal),"-II",100);
			if(mk)	Mark(mglPoint(x.a[i],y.a[i+n*j],zVal),mk);
		}
	}
	ArrowSize *=2;
}
//-----------------------------------------------------------------------------
void mglGraph::Error(mglData &y, mglData &ey, const char *pen,float zVal)
{
	mglData x(y.nx);
	x.Fill(Min.x,Max.x);
	Error(x,y,ey,pen,zVal);
}
//-----------------------------------------------------------------------------
//
//		Chart series
//
//-----------------------------------------------------------------------------
void mglGraph::Chart(mglData &a, const char *cols)
{
	if(a.Minimal()<0)	{	SetWarn(mglWarnNeg,"Chart");	return;	}
	bool wire = false;	// draw edges
	register long n=a.nx,i,j,k,l,m,i0;
	if(cols && !strcmp(cols,"#"))	{	wire = true;	cols = 0;	}

	mglColor *c = (cols && cols[0] && cols[1]) ? new mglColor[strlen(cols)+1] : 0,*cc=Pal;
	long nc=0;			// number of colors
	if(cols)			// fill colors
	{
		for(i=0;i<long(strlen(cols));i++)
		{
			if(strchr("wkrgbcymhRGBCYMHWlenuqpLENUQP",cols[i]))
			{	c[nc].Set(cols[i]);	nc++;	}
			else if(cols[i]=='#')	wire = true;
		}
		cc = c;
	}
	else	nc = NumPal;// get colors from palette

	float dy = (Max.y-Min.y)/a.ny, dx, ss, cs, x1, y1;
	float *pp = new float[3*40*81];
	for(j=0;j<a.ny;j++)
	{
		y1 = Min.y + dy*j;
		for(i=0,ss=0;i<n;i++)	ss += a.a[i+j*n];
		if(ss==0)	continue;
		for(cs=0,i=0;i<n;i++)
		{
			dx = a.a[i+j*n]/ss;	m = 2+long(38*dx+0.9);
			if(dx==0)	continue;
			x1 = Min.x + (Max.x-Min.x)*cs/ss;	dx *= (Max.x-Min.x);
			DefColor(cc[i%nc],Transparent ? AlphaDef : 1);
			for(k=0;k<40;k++)	for(l=0;l<m;l++)
			{
				i0 = 3*(m*k+l);
				pp[i0]=x1+dx*l/(m-1.);	pp[i0+1]=y1+dy*k/39.;	pp[i0+2]=Max.z;
				ScalePoint(pp[i0],pp[i0+1],pp[i0+2]);
				i0 = 3*(m*(79-k)+l);
				pp[i0]=x1+dx*l/(m-1.);	pp[i0+1]=y1+dy*k/39.;	pp[i0+2]=Min.z;
				ScalePoint(pp[i0],pp[i0+1],pp[i0+2]);
			}
			memcpy(pp+3*80*m,pp,3*m*sizeof(float));
			surf_plot(m,81,pp,0,0);
			for(k=0;k<40;k++)
			{
				i0 = 6*k;
				pp[i0] = x1;	pp[i0+1] = y1+dy*k/39.;	pp[i0+2] = Max.z;
				ScalePoint(pp[i0],pp[i0+1],pp[i0+2]);
				pp[i0+3] = x1;	pp[i0+4] = y1+dy*k/39.;	pp[i0+5] = Min.z;
				ScalePoint(pp[i0+3],pp[i0+4],pp[i0+5]);
			}
			surf_plot(2,40,pp,0,0);
			for(k=0;k<40;k++)
			{
				i0 = 6*k;
				pp[i0] = x1+dx;		pp[i0+1] = y1+dy*k/39.;	pp[i0+2] = Max.z;
				ScalePoint(pp[i0],pp[i0+1],pp[i0+2]);
				pp[i0+3] = x1+dx;	pp[i0+4] = y1+dy*k/39.;	pp[i0+5] = Min.z;
				ScalePoint(pp[i0+3],pp[i0+4],pp[i0+5]);
			}
			surf_plot(2,40,pp,0,0);
			if(wire)
			{
				Line(mglPoint(x1,y1,Max.z),mglPoint(x1,y1,Min.z),"k-",2);
				Line(mglPoint(x1+dx,y1,Max.z),mglPoint(x1+dx,y1,Min.z),"k-",2);
				Line(mglPoint(x1,y1+dy,Max.z),mglPoint(x1,y1+dy,Min.z),"k-",2);
				Line(mglPoint(x1+dx,y1+dy,Max.z),mglPoint(x1+dx,y1+dy,Min.z),"k-",2);

				Line(mglPoint(x1,y1,Max.z),mglPoint(x1+dx,y1,Max.z),"k-",40);
				Line(mglPoint(x1,y1,Max.z),mglPoint(x1,y1+dy,Max.z),"k-",40);
				Line(mglPoint(x1+dx,y1+dy,Max.z),mglPoint(x1+dx,y1,Max.z),"k-",40);
				Line(mglPoint(x1+dx,y1+dy,Max.z),mglPoint(x1,y1+dy,Max.z),"k-",40);

				Line(mglPoint(x1,y1,Min.z),mglPoint(x1+dx,y1,Min.z),"k-",40);
				Line(mglPoint(x1,y1,Min.z),mglPoint(x1,y1+dy,Min.z),"k-",40);
				Line(mglPoint(x1+dx,y1+dy,Min.z),mglPoint(x1+dx,y1,Min.z),"k-",40);
				Line(mglPoint(x1+dx,y1+dy,Min.z),mglPoint(x1,y1+dy,Min.z),"k-",40);
			}
			cs += a.a[i+j*n];
		}
	}
	delete []pp;	if(c)	delete []c;
}
//-----------------------------------------------------------------------------
//
//	Mark series
//
//-----------------------------------------------------------------------------
void mglGraph::Mark(mglData &x, mglData &y, mglData &z, mglData &r, const char *pen)
{
	long j,m,mx,my,mz,mr,n=y.nx;
	char mk=0;
	if(x.nx!=n || z.nx!=n || r.nx!=n)
	{	SetWarn(mglWarnDim,"Mark");	return;	}
	if(n<2)	{	SetWarn(mglWarnLow,"Mark");	return;	}
	m = x.ny > y.ny ? x.ny : y.ny;	m = z.ny > m ? z.ny : m;
	float ms = MarkSize, xx,yy,zz;
	bool tt;
	if(pen && *pen)	mk=SelectPen(pen);	else mk='o';
	if(mk==0)	return;

	for(j=0;j<m;j++)
	{
		if(!pen || *pen==0)	Pen(Pal[(CurrPal = (CurrPal+1)%NumPal)],'-',BaseLineWidth);
		mx = j<x.ny ? j:0;	my = j<y.ny ? j:0;
		mz = j<z.ny ? j:0;	mr = j<r.ny ? j:0;
		register long i;
		for(i=0;i<n;i++)
		{
			xx = x.a[i+mx*n];	yy = y.a[i+my*n];	zz = z.a[i+mz*n];
			tt = ScalePoint(xx,yy,zz);
			MarkSize = ms*fabs(r.a[i+mr*n]);
			if(tt)	Mark(xx,yy,zz,mk);
		}
	}
	MarkSize = ms;
}
//-----------------------------------------------------------------------------
void mglGraph::Mark(mglData &x, mglData &y, mglData &r, const char *pen,float zVal)
{
	mglData z(y.nx);
	if(isnan(zVal))	zVal = Min.z;
	z.Fill(zVal,zVal);
	Mark(x,y,z,r,pen);
}
//-----------------------------------------------------------------------------
void mglGraph::Mark(mglData &y, mglData &r, const char *pen,float zVal)
{
	if(y.nx<2)	{	SetWarn(mglWarnLow,"Mark");	return;	}
	mglData x(y.nx);
	x.Fill(Min.x,Max.x);
	Mark(x,y,r,pen,zVal);
}
//-----------------------------------------------------------------------------
//
//	Mark series
//
//-----------------------------------------------------------------------------
void mglGraph::TextMark(mglData &x, mglData &y, mglData &z, mglData &r, const wchar_t *text, const char *fnt)
{
	long j,m,mx,my,mz,mr,n=y.nx;
	if(x.nx!=n || z.nx!=n || r.nx!=n)
	{	SetWarn(mglWarnDim,"Mark");	return;	}
	if(n<2)	{	SetWarn(mglWarnLow,"Mark");	return;	}
	m = x.ny > y.ny ? x.ny : y.ny;	m = z.ny > m ? z.ny : m;
	float xx,yy,zz;
	bool tt;

	for(j=0;j<m;j++)
	{
		mx = j<x.ny ? j:0;	my = j<y.ny ? j:0;
		mz = j<z.ny ? j:0;	mr = j<r.ny ? j:0;
		register long i;
		for(i=0;i<n;i++)
			Putsw(mglPoint(x.a[i+mx*n],y.a[i+my*n],z.a[i+mz*n]),text,
				  fnt?fnt:"rC",-25*MarkSize*fabs(r.a[i+mr*n]));
	}
}
//-----------------------------------------------------------------------------
void mglGraph::TextMark(mglData &x, mglData &y, mglData &r, const wchar_t *text, const char *fnt,float zVal)
{
	mglData z(y.nx);
	if(isnan(zVal))	zVal = Min.z;
	z.Fill(zVal,zVal);
	TextMark(x,y,z,r,text,fnt);
}
//-----------------------------------------------------------------------------
void mglGraph::TextMark(mglData &y, mglData &r, const wchar_t *text, const char *fnt,float zVal)
{
	if(y.nx<2)	{	SetWarn(mglWarnLow,"Mark");	return;	}
	mglData x(y.nx);
	x.Fill(Min.x,Max.x);
	TextMark(x,y,r,text,fnt,zVal);
}
//-----------------------------------------------------------------------------
void mglGraph::TextMark(mglData &y, const wchar_t *text, const char *fnt,float zVal)
{
	mglData r(y.nx);	r.Fill(1,1);
	TextMark(y,r,text,fnt,zVal);
}
//-----------------------------------------------------------------------------
//
//	Tube series
//
//-----------------------------------------------------------------------------
void mglGraph::tube_plot(long n,float *q,float *c,float *r)
{
	if(n<2)	return;
	float *pp = new float[3*40*n], *cc = new float[4*40*n];
	float lx,ly,lz,len,gx=0,gy=0,gz=1,hx,hy,hz;
	float xx, yy, zz, rr;
	bool *tt = new bool[40*n],tl;
	register long i,k,i0;
	long m=n;

	for(i=0;i<n;i++)
	{
		xx = q[i*3]; yy = q[i*3+1]; zz = q[i*3+2];
		tl = ScalePoint(xx,yy,zz);
		xx = q[i*3]; yy = q[i*3+1]; zz = q[i*3+2];	rr = r[i];
		if(i>0)
		{	lx = xx-q[i*3-3];	ly = yy-q[i*3-2];	lz = zz-q[i*3-1];	}
		else
		{	lx = q[i*3+3]-xx;	ly = q[i*3+4]-yy;	lz = q[i*3+5]-zz;	}
		len = sqrt(lx*lx+ly*ly+lz*lz);
		if(len==0)	{	m--;	continue;	}
		lx /= len;	ly /= len;	lz /= len;
		if(i>0)
		{
			len = gx*lx+gy*ly+gz*lz;
			gx -= lx*len;	gy -= ly*len;	gz -= lz*len;
			len = sqrt(gx*gx+gy*gy+gz*gz);
			gx /= len;		gy /= len;		gz /= len;
		}
		else
		{
			if(lx==0 && ly==0)
			{	gx = -lz/hypot(lx,lz);	gz = lx/hypot(lx,lz);	gy=0;	}
			else
			{	gx = ly/hypot(lx,ly);	gy = -lx/hypot(lx,ly);	gz=0;	}
		}
		hx = ly*gz-lz*gy;	hy = lz*gx-lx*gz;	hz = lx*gy-ly*gx;
		for(k=0;k<CirclePnts;k++)
		{
			i0 = k+CirclePnts*i;
			lx = rr*cos(2*M_PI*k/(CirclePnts-1.));	ly = rr*sin(2*M_PI*k/(CirclePnts-1.));
			pp[3*i0] = xx + gx*lx + hx*ly;
			pp[3*i0+1]=yy + gy*lx + hy*ly;
			pp[3*i0+2]=zz + gz*lx + hz*ly;
			if(c)	memcpy(cc+4*i0,c+4*i,4*sizeof(float));
			tt[i0] = tl;
			ScalePoint(pp[3*i0],pp[3*i0+1],pp[3*i0+2]);
		}
	}
	surf_plot(CirclePnts,m,pp,c?cc:0,tt);
	delete []pp;	delete []cc;	delete []tt;
}
//-----------------------------------------------------------------------------
void mglGraph::Tube(mglData &x, mglData &y, mglData &z, mglData &r, const char *pen)
{
	long j,m,mx,my,mz,mr,n=y.nx;
	char mk=0;
	if(n<2)	{	SetWarn(mglWarnLow,"Tube");	return;	}
	if(x.nx!=n || z.nx!=n || r.nx!=n)
	{	SetWarn(mglWarnDim,"Tube");	return;	}
	m = x.ny > y.ny ? x.ny : y.ny;	m = z.ny > m ? z.ny : m;	m = r.ny > m ? r.ny : m;
	if(pen && *pen)	mk=SelectPen(pen);
	float *pp = new float[3*n], *rr = new float[n];

	for(j=0;j<m;j++)
	{
		if(!pen || *pen==0)	Pen(Pal[(CurrPal = (CurrPal+1)%NumPal)],'-',BaseLineWidth);
		DefColor(NC, -1);
		mx = j<x.ny ? j:0;	my = j<y.ny ? j:0;
		mz = j<z.ny ? j:0;	mr = j<r.ny ? j:0;
		register long i;

		for(i=0;i<n;i++)
		{
			pp[3*i] = x.a[i+mx*n];		pp[3*i+1] = y.a[i+my*n];
			pp[3*i+2] = z.a[i+mz*n];	rr[i] = r.a[i+mr*n];
		}
		tube_plot(n,pp,0,rr);
	}
	delete []pp;	delete []rr;
}
//-----------------------------------------------------------------------------
void mglGraph::Tube(mglData &x, mglData &y, mglData &r, const char *pen,float zVal)
{
	mglData z(y.nx);
	if(isnan(zVal))	zVal = Min.z;
	z.Fill(zVal,zVal);
	Tube(x,y,z,r,pen);
}
//-----------------------------------------------------------------------------
void mglGraph::Tube(mglData &y, mglData &r, const char *pen,float zVal)
{
	if(y.nx<2)	{	SetWarn(mglWarnLow,"Tube");	return;	}
	mglData x(y.nx);
	x.Fill(Min.x,Max.x);
	Tube(x,y,r,pen,zVal);
}
//-----------------------------------------------------------------------------
void mglGraph::Tube(mglData &y, float rr, const char *pen,float zVal)
{
	if(y.nx<2)	{	SetWarn(mglWarnLow,"Tube");	return;	}
	mglData x(y.nx), r(y.nx);
	x.Fill(Min.x,Max.x);
	r.Fill(rr,rr);
	Tube(x,y,r,pen,zVal);
}
//-----------------------------------------------------------------------------
void mglGraph::Tube(mglData &x, mglData &y, float rr, const char *pen,float zVal)
{
	if(y.nx<2)	{	SetWarn(mglWarnLow,"Tube");	return;	}
	mglData r(y.nx);
	r.Fill(rr,rr);
	Tube(x,y,r,pen,zVal);
}
//-----------------------------------------------------------------------------
void mglGraph::Tube(mglData &x, mglData &y, mglData &z, float rr, const char *pen)
{
	if(y.nx<2)	{	SetWarn(mglWarnLow,"Tube");	return;	}
	mglData r(y.nx);
	r.Fill(rr,rr);
	Tube(x,y,z,r,pen);
}
//-----------------------------------------------------------------------------
//
//		All in one :)
//
//-----------------------------------------------------------------------------
void mglGraph::Plot2(mglData &a, const char *pen,float zVal)
{
	if(a.nx<2 || a.ny<2)	{	SetWarn(mglWarnLow,"Plot2");	return;	}
	mglData x,y;
	for(long i=0;i<a.ny/2;i++)
	{
		x=a.SubData(-1,2*i);	y=a.SubData(-1,2*i+1);
		Plot(x,y,pen,zVal);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Plot3(mglData &a, const char *pen)
{
	if(a.nx<2 || a.ny<3)	{	SetWarn(mglWarnLow,"Plot3");	return;	}
	mglData x,y,z;
	for(long i=0;i<a.ny/2;i++)
	{
		x=a.SubData(-1,3*i);	y=a.SubData(-1,3*i+1);	z=a.SubData(-1,3*i+2);
		Plot(x,y,z,pen);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Area2(mglData &a, const char *pen,float zVal)
{
	if(a.nx<2 || a.ny<2)	{	SetWarn(mglWarnLow,"Area2");	return;	}
	mglData x,y;
	for(long i=0;i<a.ny/2;i++)
	{
		x=a.SubData(-1,2*i);	y=a.SubData(-1,2*i+1);
		Area(x,y,pen,false,zVal);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Area3(mglData &a, const char *pen)
{
	if(a.nx<2 || a.ny<3)	{	SetWarn(mglWarnLow,"Area3");	return;	}
	mglData x,y,z;
	for(long i=0;i<a.ny/2;i++)
	{
		x=a.SubData(-1,3*i);	y=a.SubData(-1,3*i+1);	z=a.SubData(-1,3*i+2);
		Area(x,y,z,pen);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Stem2(mglData &a, const char *pen,float zVal)
{
	if(a.nx<2 || a.ny<2)	{	SetWarn(mglWarnLow,"Stem2");	return;	}
	mglData x,y;
	for(long i=0;i<a.ny/2;i++)
	{
		x=a.SubData(-1,2*i);	y=a.SubData(-1,2*i+1);
		Stem(x,y,pen,zVal);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Stem3(mglData &a, const char *pen)
{
	if(a.nx<2 || a.ny<3)	{	SetWarn(mglWarnLow,"Stem3");	return;	}
	mglData x,y,z;
	for(long i=0;i<a.ny/2;i++)
	{
		x=a.SubData(-1,3*i);	y=a.SubData(-1,3*i+1);	z=a.SubData(-1,3*i+2);
		Stem(x,y,z,pen);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Step2(mglData &a, const char *pen,float zVal)
{
	if(a.nx<2 || a.ny<2)	{	SetWarn(mglWarnLow,"Step2");	return;	}
	mglData x,y;
	for(long i=0;i<a.ny/2;i++)
	{
		x=a.SubData(-1,2*i);	y=a.SubData(-1,2*i+1);
		Step(x,y,pen,zVal);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Step3(mglData &a, const char *pen)
{
	if(a.nx<2 || a.ny<3)	{	SetWarn(mglWarnLow,"Step3");	return;	}
	mglData x,y,z;
	for(long i=0;i<a.ny/2;i++)
	{
		x=a.SubData(-1,3*i);	y=a.SubData(-1,3*i+1);	z=a.SubData(-1,3*i+2);
		Step(x,y,z,pen);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Bars2(mglData &a, const char *pen,float zVal, bool above)
{
	if(a.nx<2 || a.ny<2)	{	SetWarn(mglWarnLow,"Bars2");	return;	}
	mglData x,y;
	for(long i=0;i<a.ny/2;i++)
	{
		x=a.SubData(-1,2*i);	y=a.SubData(-1,2*i+1);
		Bars(x,y,pen,zVal,above);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Bars3(mglData &a, const char *pen, bool above)
{
	if(a.nx<2 || a.ny<3)	{	SetWarn(mglWarnLow,"Bars3");	return;	}
	mglData x,y,z;
	for(long i=0;i<a.ny/2;i++)
	{
		x=a.SubData(-1,3*i);	y=a.SubData(-1,3*i+1);	z=a.SubData(-1,3*i+2);
		Bars(x,y,z,pen,above);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Torus2(mglData &a, const char *sch)
{
	if(a.nx<2 || a.ny<2)	{	SetWarn(mglWarnLow,"Torus2");	return;	}
	mglData x=a.SubData(-1,0);
	mglData z=a.SubData(-1,1);
	Torus(x,z,sch);
}
//-----------------------------------------------------------------------------
//		1D plotting functions
//-----------------------------------------------------------------------------
/// Draw line plot for points in arrays \a x, \a y, \a z.
void mgl_plot_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, const char *pen)
{	if(gr && x && y && z)	gr->Plot(*x,*y,*z,pen);	}
/// Draw line plot for points in arrays \a x, \a y.
void mgl_plot_xy(HMGL gr, HMDT x, HMDT y, const char *pen)
{	if(gr && x && y)	gr->Plot(*x,*y,pen);	}
/// Draw line plot for points in arrays \a y.
void mgl_plot(HMGL gr, HMDT y,	const char *pen)
{	if(gr && y)	gr->Plot(*y,pen);	}
/// Draw line plot for points in arrays \a a(0,:),\a a(1,:).
void mgl_plot_2(HMGL gr, HMDT a, const char *pen)
{	if(gr && a)	gr->Plot2(*a,pen);	}
/// Draw line plot for points in arrays \a a(0,:),\a a(1,:),\a a(2,:).
void mgl_plot_3(HMGL gr, HMDT a, const char *pen)
{	if(gr && a)	gr->Plot3(*a,pen);	}
/// Draw area plot for points in arrays \a x, \a y, \a z.
void mgl_area_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, const char *pen)
{	if(gr && x && y && z)	gr->Area(*x,*y,*z,pen);	}
/// Draw area plot for points in arrays \a x, \a y.
void mgl_area_xy(HMGL gr, HMDT x, HMDT y, const char *pen)
{	if(gr && x && y)	gr->Area(*x,*y,pen);	}
/// Draw area plot for points in arrays \a y.
void mgl_area(HMGL gr, HMDT y, const char *pen)
{	if(gr && y)	gr->Area(*y,pen);	}
/// Draw area plot for points in arrays \a a(0,:),\a a(1,:).
void mgl_area_2(HMGL gr, HMDT a, const char *pen)
{	if(gr && a)	gr->Area2(*a, pen);	}
/// Draw area plot for points in arrays \a a(0,:),\a a(1,:),\a a(2,:).
void mgl_area_3(HMGL gr, HMDT a, const char *pen)
{	if(gr && a)	gr->Area3(*a,pen);	}
/// draw mark with different type at position {x,y,z}
void mgl_mark(HMGL gr, float x,float y,float z,char mark)
{	gr->Mark(mglPoint(x,y,z),mark);	}
/// Draw vertical lines from points in arrays \a x, \a y, \a z to mglGraph::Org.
void mgl_stem_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, const char *pen)
{	if(gr && x && y && z)	gr->Stem(*x,*y,*z,pen);	}
/// Draw vertical lines from points in arrays \a x, \a y to mglGraph::Org.
void mgl_stem_xy(HMGL gr, HMDT x, HMDT y, const char *pen)
{	if(gr && x && y)	gr->Stem(*x,*y,pen);	}
/// Draw vertical lines from points in arrays \a y to mglGraph::Org.
void mgl_stem(HMGL gr, HMDT y,	const char *pen)
{	if(gr && y)	gr->Stem(*y,pen);	}
/// Draw vertical lines from points in arrays \a a(0,:),\a a(1,:) to mglGraph::Org.
void mgl_stem_2(HMGL gr, HMDT a, const char *pen)
{	if(gr && a)	gr->Stem2(*a, pen);	}
/// Draw vertical lines from points in arrays \a a(0,:),\a a(1,:),\a a(2,:) to mglGraph::Org.
void mgl_stem_3(HMGL gr, HMDT a, const char *pen)
{	if(gr && a)	gr->Stem3(*a, pen);	}
/// Draw stairs for points in arrays \a x, \a y, \a z.
void mgl_step_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, const char *pen)
{	if(gr && x && y && z)	gr->Step(*x,*y,*z,pen);	}
/// Draw stairs for points in arrays \a x, \a y.
void mgl_step_xy(HMGL gr, HMDT x, HMDT y, const char *pen)
{	if(gr && x && y)	gr->Step(*x,*y,pen);	}
/// Draw line plot for points in arrays \a y.
void mgl_step(HMGL gr, HMDT y,	const char *pen)
{	if(gr && y)	gr->Step(*y,pen);	}
/// Draw stairs for points in arrays \a a(0,:),\a a(1,:).
void mgl_step_2(HMGL gr, HMDT a, const char *pen)
{	if(gr && a)	gr->Step2(*a, pen);	}
/// Draw stairs for points in arrays \a a(0,:),\a a(1,:),\a a(2,:).
void mgl_step_3(HMGL gr, HMDT a, const char *pen)
{	if(gr && a)	gr->Step3(*a, pen);	}
/// Draw vertical bars from points in arrays \a x, \a y, \a z to mglGraph::Org.
void mgl_bars_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, const char *pen)
{	if(gr && x && y && z)	gr->Bars(*x,*y,*z,pen);	}
/// Draw vertical bars from points in arrays \a x, \a y to mglGraph::Org.
void mgl_bars_xy(HMGL gr, HMDT x, HMDT y, const char *pen)
{	if(gr && x && y)	gr->Bars(*x,*y,pen);	}
/// Draw vertical bars from points in arrays \a y to mglGraph::Org.
void mgl_bars(HMGL gr, HMDT y,	const char *pen)
{	if(gr && y)	gr->Bars(*y,pen);	}
/// Draw vertical bars from points in arrays \a a(0,:),\a a(1,:) to mglGraph::Org.
void mgl_bars_2(HMGL gr, HMDT a, const char *pen)
{	if(gr && a)	gr->Bars2(*a, pen);	}
/// Draw vertical bars from points in arrays \a a(0,:),\a a(1,:),\a a(2,:) to mglGraph::Org.
void mgl_bars_3(HMGL gr, HMDT a, const char *pen)
{	if(gr && a)	gr->Bars3(*a, pen);	}
/// Draw surface of curve {\a r,\a z} rotatation around Z axis
void mgl_torus(HMGL gr, HMDT r, HMDT z, const char *pen)
{	if(gr && r && z)	gr->Torus(*r, *z, pen);	}
/// Draw surface of curve {\a a(0,:),\a a(1,:)} rotatation around Z axis for
void mgl_torus_2(HMGL gr, HMDT a, const char *pen)
{	if(gr && a)	gr->Torus2(*a, pen);	}
//-----------------------------------------------------------------------------
/// Draw chart for data a
void mgl_chart(HMGL gr, HMDT a, const char *col)
{	if(gr&&a)	gr->Chart(*a, col);	}
/// Draw error boxes ey for data y
void mgl_error(HMGL gr, HMDT y, HMDT ey, const char *pen)
{	if(gr&&y&&ey)	gr->Error(*y,*ey,pen);	}
/// Draw error boxes ey for data {x,y}
void mgl_error_xy(HMGL gr, HMDT x, HMDT y, HMDT ey, const char *pen)
{	if(gr&&x&&y&&ey)	gr->Error(*x,*y,*ey,pen);	}
/// Draw error boxes {ex,ey} for data {x,y}
void mgl_error_exy(HMGL gr, HMDT x, HMDT y, HMDT ex, HMDT ey, const char *pen)
{	if(gr&&x&&ex&&y&&ey)	gr->Error(*x,*y,*ex,*ey,pen);	}
/// Draw marks with diffenernt sizes \a r for points in arrays \a x, \a y, \a z.
void mgl_mark_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, HMDT r, const char *pen)
{	if(gr&&x&&y&&z&&r)	gr->Mark(*x,*y,*z,*r,pen);	}
/// Draw marks with diffenernt sizes \a r for points in arrays \a x, \a y.
void mgl_mark_xy(HMGL gr, HMDT x, HMDT y, HMDT r, const char *pen)
{	if(gr&&x&&y&&r)	gr->Mark(*x,*y,*r,pen);	}
/// Draw marks with diffenernt sizes \a r for points in arrays \a y.
void mgl_mark_y(HMGL gr, HMDT y, HMDT r, const char *pen)
{	if(gr&&y&&r)	gr->Mark(*y,*r,pen);	}
/// Draw tube with radial sizes \a r for points in arrays \a x, \a y, \a z.
void mgl_tube_xyzr(HMGL gr, HMDT x, HMDT y, HMDT z, HMDT r, const char *pen)
{	if(gr&&x&&y&&z&&r)	gr->Tube(*x,*y,*z,*r,pen);	}
/// Draw tube with radial sizes \a r for points in arrays \a x, \a y.
void mgl_tube_xyr(HMGL gr, HMDT x, HMDT y, HMDT r, const char *pen)
{	if(gr&&x&&y&&r)	gr->Tube(*x,*y,*r,pen);	}
/// Draw tube with radial sizes \a r for points in arrays \a y.
void mgl_tube_r(HMGL gr, HMDT y, HMDT r, const char *pen)
{	if(gr&&y&&r)	gr->Tube(*y,*r,pen);	}
/// Draw tube with constant radial sizes \a r for points in arrays \a x, \a y, \a z.
void mgl_tube_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, float r, const char *pen)
{	if(gr&&x&&y&&z)	gr->Tube(*x,*y,*z,r,pen);	}
/// Draw tube with constant radial sizes \a r for points in arrays \a x, \a y.
void mgl_tube_xy(HMGL gr, HMDT x, HMDT y, float r, const char *pen)
{	if(gr&&x&&y)	gr->Tube(*x,*y,r,pen);	}
/// Draw tube with constant radial sizes \a r for points in arrays \a y.
void mgl_tube(HMGL gr, HMDT y, float r, const char *pen)
{	if(gr&&y)	gr->Tube(*y,r,pen);	}
//-----------------------------------------------------------------------------
//		1D plotting functions (Fortran)
//-----------------------------------------------------------------------------
/// Draw line plot for points in arrays \a x, \a y, \a z.
void mgl_plot_xyz_(long *gr, long *x, long *y, long *z, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && x && y && z)	_GR_->Plot(_D_(x),_D_(y),_D_(z),s);
	delete []s;
}
/// Draw line plot for points in arrays \a x, \a y.
void mgl_plot_xy_(long *gr, long *x, long *y, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && x && y)	_GR_->Plot(_D_(x),_D_(y),s);
	delete []s;
}
/// Draw line plot for points in arrays \a y.
void mgl_plot_(long *gr, long *y,	const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && y)	_GR_->Plot(_D_(y),s);
	delete []s;
}
/// Draw line plot for points in arrays \a a(0,:),\a a(1,:).
void mgl_plot_2_(long *gr, long *a, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && a)	_GR_->Plot2(_D_(a),s);
	delete []s;
}
/// Draw line plot for points in arrays \a a(0,:),\a a(1,:),\a a(2,:).
void mgl_plot_3_(long *gr, long *a, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && a)	_GR_->Plot3(_D_(a),s);
	delete []s;
}
/// Draw area plot for points in arrays \a x, \a y, \a z.
void mgl_area_xyz_(long *gr, long *x, long *y, long *z, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && x && y && z)	_GR_->Area(_D_(x),_D_(y),_D_(z),s);
	delete []s;
}
/// Draw area plot for points in arrays \a x, \a y.
void mgl_area_xy_(long *gr, long *x, long *y, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && x && y)	_GR_->Area(_D_(x),_D_(y),s);
	delete []s;
}
/// Draw area plot for points in arrays \a y.
void mgl_area_(long *gr, long *y, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && y)	_GR_->Area(_D_(y),s);
	delete []s;
}
/// Draw area plot for points in arrays \a a(0,:),\a a(1,:).
void mgl_area_2_(long *gr, long *a, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && a)	_GR_->Area2(_D_(a), s);
	delete []s;
}
/// Draw area plot for points in arrays \a a(0,:),\a a(1,:),\a a(2,:).
void mgl_area_3_(long *gr, long *a, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && a)	_GR_->Area3(_D_(a),s);
	delete []s;
}
/// draw mark with different type at position {x,y,z}
void mgl_mark_(long *gr, float *x,float *y,float *z,const char *mark,int)
{	_GR_->Mark(mglPoint(*x,*y,*z),*mark);	}
/// Draw vertical lines from points in arrays \a x, \a y, \a z to mglGraph::Org.
void mgl_stem_xyz_(long *gr, long *x, long *y, long *z, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && x && y && z)	_GR_->Stem(_D_(x),_D_(y),_D_(z),s);
	delete []s;
}
/// Draw vertical lines from points in arrays \a x, \a y to mglGraph::Org.
void mgl_stem_xy_(long *gr, long *x, long *y, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && x && y)	_GR_->Stem(_D_(x),_D_(y),s);
	delete []s;
}
/// Draw vertical lines from points in arrays \a y to mglGraph::Org.
void mgl_stem_(long *gr, long *y,	const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && y)	_GR_->Stem(_D_(y),s);
	delete []s;
}
/// Draw vertical lines from points in arrays \a a(0,:),\a a(1,:) to mglGraph::Org.
void mgl_stem_2_(long *gr, long *a, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && a)	_GR_->Stem2(_D_(a), s);
	delete []s;
}
/// Draw vertical lines from points in arrays \a a(0,:),\a a(1,:),\a a(2,:) to mglGraph::Org.
void mgl_stem_3_(long *gr, long *a, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && a)	_GR_->Stem3(_D_(a), s);
	delete []s;
}
/// Draw stairs for points in arrays \a x, \a y, \a z.
void mgl_step_xyz_(long *gr, long *x, long *y, long *z, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && x && y && z)	_GR_->Step(_D_(x),_D_(y),_D_(z),s);
	delete []s;
}
/// Draw stairs for points in arrays \a x, \a y.
void mgl_step_xy_(long *gr, long *x, long *y, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && x && y)	_GR_->Step(_D_(x),_D_(y),s);
	delete []s;
}
/// Draw line plot for points in arrays \a y.
void mgl_step_(long *gr, long *y,	const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && y)	_GR_->Step(_D_(y),s);
	delete []s;
}
/// Draw stairs for points in arrays \a a(0,:),\a a(1,:).
void mgl_step_2_(long *gr, long *a, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && a)	_GR_->Step2(_D_(a), s);
	delete []s;
}
/// Draw stairs for points in arrays \a a(0,:),\a a(1,:),\a a(2,:).
void mgl_step_3_(long *gr, long *a, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && a)	_GR_->Step3(_D_(a), s);
	delete []s;
}
/// Draw vertical bars from points in arrays \a x, \a y, \a z to mglGraph::Org.
void mgl_bars_xyz_(long *gr, long *x, long *y, long *z, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && x && y && z)	_GR_->Bars(_D_(x),_D_(y),_D_(z),s);
	delete []s;
}
/// Draw vertical bars from points in arrays \a x, \a y to mglGraph::Org.
void mgl_bars_xy_(long *gr, long *x, long *y, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && x && y)	_GR_->Bars(_D_(x),_D_(y),s);
	delete []s;
}
/// Draw vertical bars from points in arrays \a y to mglGraph::Org.
void mgl_bars_(long *gr, long *y,	const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && y)	_GR_->Bars(_D_(y),s);
	delete []s;
}
/// Draw vertical bars from points in arrays \a a(0,:),\a a(1,:) to mglGraph::Org.
void mgl_bars_2_(long *gr, long *a, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && a)	_GR_->Bars2(_D_(a), s);
	delete []s;
}
/// Draw vertical bars from points in arrays \a a(0,:),\a a(1,:),\a a(2,:) to mglGraph::Org.
void mgl_bars_3_(long *gr, long *a, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && a)	_GR_->Bars3(_D_(a), s);
	delete []s;
}
/// Draw surface of curve {\a r,\a z} rotatation around Z axis
void mgl_torus_(long *gr, long *r, long *z, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && r && z)	_GR_->Torus(_D_(r), _D_(z), s);
	delete []s;
}
/// Draw surface of curve {\a a(0,:),\a a(1,:)} rotatation around Z axis for
void mgl_torus_2_(long *gr, long *a, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr && a)	_GR_->Torus2(_D_(a), s);
	delete []s;
}
//-----------------------------------------------------------------------------
/// Draw chart for data a
void mgl_chart_(long *gr, long *a, const char *col,int l)
{
	char *s=new char[l+1];	memcpy(s,col,l);	s[l]=0;
	if(gr&&a)	_GR_->Chart(_D_(a), s);
	delete []s;
}
/// Draw error boxes ey for data y
void mgl_error_(long *gr, long *y, long *ey, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr&&y&&ey)	_GR_->Error(_D_(y),_D_(ey),s);
	delete []s;
}
/// Draw error boxes ey for data {x,y}
void mgl_error_xy_(long *gr, long *x, long *y, long *ey, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr&&x&&y&&ey)	_GR_->Error(_D_(x),_D_(y),_D_(ey),s);
	delete []s;
}
/// Draw error boxes {ex,ey} for data {x,y}
void mgl_error_exy_(long *gr, long *x, long *y, long *ex, long *ey, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr&&x&&ex&&y&&ey)	_GR_->Error(_D_(x),_D_(y),_D_(ex),_D_(ey),s);
	delete []s;
}
/// Draw marks with diffenernt sizes \a r for points in arrays \a x, \a y, \a z.
void mgl_mark_xyz_(long *gr, long *x, long *y, long *z, long *r, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr&&x&&y&&z&&r)	_GR_->Mark(_D_(x),_D_(y),_D_(z), _D_(r),s);
	delete []s;
}
/// Draw marks with diffenernt sizes \a r for points in arrays \a x, \a y.
void mgl_mark_xy_(long *gr, long *x, long *y, long *r, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr&&x&&y&&r)	_GR_->Mark(_D_(x), _D_(y), _D_(r),s);
	delete []s;
}
/// Draw marks with diffenernt sizes \a r for points in arrays \a y.
void mgl_mark_y_(long *gr, long *y, long *r, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr&&y&&r)	_GR_->Mark(_D_(y),_D_(r),s);
	delete []s;
}
/// Draw tube with radial sizes \a r for points in arrays \a x, \a y, \a z.
void mgl_tube_xyzr_(long *gr, long *x, long *y, long *z, long *r, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr&&x&&y&&z&&r)	_GR_->Tube(_D_(x),_D_(y),_D_(z), _D_(r),s);
	delete []s;
}
/// Draw tube with radial sizes \a r for points in arrays \a x, \a y.
void mgl_tube_xyr_(long *gr, long *x, long *y, long *r, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr&&x&&y&&r)	_GR_->Tube(_D_(x),_D_(y),_D_(r),s);
	delete []s;
}
/// Draw tube with radial sizes \a r for points in arrays \a y.
void mgl_tube_r_(long *gr, long *y, long *r, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr&&y&&r)	_GR_->Tube(_D_(y),_D_(r),s);
	delete []s;
}
/// Draw tube with constant radial sizes \a r for points in arrays \a x, \a y, \a z.
void mgl_tube_xyz_(long *gr, long *x, long *y, long *z, float *r, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr&&x&&y&&z)	_GR_->Tube(_D_(x),_D_(y),_D_(z),*r,s);
	delete []s;
}
/// Draw tube with constant radial sizes \a r for points in arrays \a x, \a y.
void mgl_tube_xy_(long *gr, long *x, long *y, float *r, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr&&x&&y)	_GR_->Tube(_D_(x),_D_(y),*r,s);
	delete []s;
}
/// Draw tube with constant radial sizes \a r for points in arrays \a y.
void mgl_tube_(long *gr, long *y, float *r, const char *pen,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	if(gr&&y)	_GR_->Tube(_D_(y),*r,s);
	delete []s;
}
//-----------------------------------------------------------------------------
void mgl_textmark_xyzr(HMGL gr, HMDT x, HMDT y, HMDT z, HMDT r, const char *text, const char *fnt)
{	if(gr&&z&&x&&y&&r&&text)	gr->TextMark(*x,*y,*z,*r,text,fnt);	}
void mgl_textmark_xyr(HMGL gr, HMDT x, HMDT y, HMDT r, const char *text, const char *fnt)
{	if(gr&&x&&y&&r&&text)	gr->TextMark(*x,*y,*r,text,fnt);	}
void mgl_textmark_yr(HMGL gr, HMDT y, HMDT r, const char *text, const char *fnt)
{	if(gr&&y&&r&&text)	gr->TextMark(*y,*r,text,fnt);	}
void mgl_textmark(HMGL gr, HMDT y, const char *text, const char *fnt)
{	if(gr&&y&&text)	gr->TextMark(*y,text,fnt);	}
void mgl_textmarkw_xyzr(HMGL gr, HMDT x, HMDT y, HMDT z, HMDT r, const wchar_t *text, const char *fnt)
{	if(gr&&z&&x&&y&&r&&text)	gr->TextMark(*x,*y,*z,*r,text,fnt);	}
void mgl_textmarkw_xyr(HMGL gr, HMDT x, HMDT y, HMDT r, const wchar_t *text, const char *fnt)
{	if(gr&&x&&y&&r&&text)	gr->TextMark(*x,*y,*r,text,fnt);	}
void mgl_textmarkw_yr(HMGL gr, HMDT y, HMDT r, const wchar_t *text, const char *fnt)
{	if(gr&&y&&r&&text)	gr->TextMark(*y,*r,text,fnt);	}
void mgl_textmarkw(HMGL gr, HMDT y, const wchar_t *text, const char *fnt)
{	if(gr&&y&&text)	gr->TextMark(*y,text,fnt);	}
//-----------------------------------------------------------------------------
void mgl_textmark_xyzr_(long *gr, long *x, long *y, long *z, long *r, const char *text, const char *fnt, int l,int n)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(s,fnt,n);	f[n]=0;
	if(gr&&y&&x&&z&&r)	_GR_->TextMark(_D_(x),_D_(y),_D_(z),_D_(r),s,f);
	delete []s;		delete []f;
}
void mgl_textmark_xyr_(long *gr, long *x, long *y, long *r, const char *text, const char *fnt, int l,int n)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(s,fnt,n);	f[n]=0;
	if(gr&&y&&x&&r)	_GR_->TextMark(_D_(x),_D_(y),_D_(r),s,f);
	delete []s;		delete []f;
}
void mgl_textmark_yr_(long *gr, long *y, long *r, const char *text, const char *fnt, int l,int n)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(s,fnt,n);	f[n]=0;
	if(gr&&y&&r)	_GR_->TextMark(_D_(y),_D_(r),s,f);
	delete []s;		delete []f;
}
void mgl_textmark_(long *gr, long *y, const char *text, const char *fnt, int l,int n)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(s,fnt,n);	f[n]=0;
	if(gr&&y)	_GR_->TextMark(_D_(y),s,f);
	delete []s;		delete []f;
}
//-----------------------------------------------------------------------------
