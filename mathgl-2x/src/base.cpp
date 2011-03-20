/***************************************************************************
 * base.cpp is part of Math Graphic Library
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
#include <stdlib.h>
#include "mgl/base.h"
#include "mgl/define.h"
//-----------------------------------------------------------------------------
mglBase::mglBase()
{
	posC=posN=0;	numC=numN=1024;
	pntC=(float *)malloc(4*numC*sizeof(float));
	pntN=(float *)malloc(8*numC*sizeof(float));
	txt = (mglTexture *)malloc(2*sizeof(mglTexture));
	// Always create default palette txt[0] and default scheme txt[1]
	txt[0].Set(MGL_DEF_PAL,-1);		txt[1].Set("BbcyrR",1);	numT=2;
	last_style[0]='k';	last_style[1]='-';	last_style[2]='0';	last_style[3]=last_style[4]=0;
	MinS=mglPoint(-1,-1,-1);	MaxS=mglPoint(1,1,1);
}
mglBase::~mglBase()
{
	for(long i=0;i<numT;i++)	txt[i].Clear();
	free(pntC);	free(pntN);	free(txt);	ClearEq();
}
//-----------------------------------------------------------------------------
void mglBase::StartGroup(const char *name, int id)
{
	LightScale();
	char buf[128];
	sprintf(buf,"%s_%d",name,id);
	StartAutoGroup(buf);
}
//-----------------------------------------------------------------------------
const char *mglWarn[mglWarnEnd] = {"%s: data dimension(s) is incompatible",
								"%s: data dimension(s) is too small",
								"%s: minimal data value is negative",
								"No file or wrong data dimensions",
								"Not enough memory",
								"%s: data values are zero",
								"Too many legend entries",
								"No legend entries",
								"%s: slice value is out of range",
								"%s: number of contours is zero or negative",
								"Couldn't open file %s",
								"Light: ID is out of range",
								"Setsize: size(s) is zero or negative",
								"Format %s is not supported for that build"};
//-----------------------------------------------------------------------------
void mglBase::SetWarn(int code, const char *who)
{
	WarnCode = code;
	if(Message && code>0 && code<mglWarnEnd)
		sprintf(Message,mglWarn[code-1],who?who:"UNKNOWN");
	else if(Message)	Message[0]=0;
}
//-----------------------------------------------------------------------------
//		Add points to the buffer
//-----------------------------------------------------------------------------
long mglBase::AddPntC(mglPoint p, float c)	// NOTE: this is not-thread-safe!!!
{
	float pp[4]={p.x,p.y,p.z,c};
	if(posC>numC-2)
	{	numC+=1024;	pntC=(float *)realloc(pntC,4*numC*sizeof(float));	}
	memcpy(pntC+posC*4,pp,4*sizeof(float));	posC++;
	return posC-1;
}
//-----------------------------------------------------------------------------
long mglBase::AddPntN(mglPoint p, float c, mglPoint n, float a, bool scl)	// NOTE: this is not-thread-safe!!!
{
	if(scl)	NormScale(n);	// Usually p was scaled before, but n should be scaled now!
	a = (a>=0 && a<=1) ? a : AlphaDef;
	float pp[8]={p.x,p.y,p.z,c,a,n.x,n.y,n.z};
	if(posN>numN-2)
	{	numN+=1024;	pntN=(float *)realloc(pntN,8*numN*sizeof(float));	}
	memcpy(pntN+posN*8,pp,8*sizeof(float));	posN++;
	return posN-1;
}
//-----------------------------------------------------------------------------
long mglBase::ReserveC(long n)
{
	if(posC+n>numC)
	{	numC=posC+n;	pntC=(float *)realloc(pntC,4*numC*sizeof(float));	}
	return posC;
}
//-----------------------------------------------------------------------------
long mglBase::ReserveN(long n)
{
	if(posN+n>numN)
	{	numN=posN+n;	pntN=(float *)realloc(pntN,8*numN*sizeof(float));	}
	return posN;
}
//-----------------------------------------------------------------------------
long mglBase::CopyNtoC(long from, long size, float c)
{
	if(size<1)	return -1;
	ReserveC(size);
	register long i,j;
	for(i=from;i<numN && i<from+size;i++)
	{
		j = 4*(posC+i-from);
		memcpy(pntC+j, pntN+8*i, 4*sizeof(float));
		if(c>=0)	pntC[j+3] = c;
	}
	posC+=size;
	return posC-size;
}
//-----------------------------------------------------------------------------
//		Boundaries and scaling
//---------------------------------------------------------------------------
void mglBase::RecalcCRange()
{
	if(!fa)
	{	FMin.c = Min.c;	FMax.c = Max.c;	}
	else
	{
		FMin.c = 1e30;	FMax.c = -1e30;
		register int i;
		float a;
		int n=30;
		for(i=0;i<=n;i++)
		{
			a = fa->Calc(0,0,0,Min.c+i*(Max.c-Min.c)/n);
			if(a<FMin.c)	FMin.c=a;
			if(a>FMax.c)	FMax.c=a;
		}
	}
}
//-----------------------------------------------------------------------------
void mglBase::RecalcBorder()
{
	if(!fx &&	!fy &&	!fz)
	{	FMin = Min;	FMax = Max;	}
	else
	{
		FMin = mglPoint( 1e30, 1e30, 1e30);
		FMax = mglPoint(-1e30,-1e30,-1e30);
		register int i,j;
		int n=30;
		for(i=0;i<=n;i++)	for(j=0;j<=n;j++)	// x range
		{
			SetFBord(Min.x, Min.y+i*(Max.y-Min.y)/n, Min.z+j*(Max.z-Min.z)/n);
			SetFBord(Max.x, Min.y+i*(Max.y-Min.y)/n, Min.z+j*(Max.z-Min.z)/n);
		}
		for(i=0;i<=n;i++)	for(j=0;j<=n;j++)	// y range
		{
			SetFBord(Min.x+i*(Max.x-Min.x)/n, Min.y, Min.z+j*(Max.z-Min.z)/n);
			SetFBord(Min.x+i*(Max.x-Min.x)/n, Max.y, Min.z+j*(Max.z-Min.z)/n);
		}
		for(i=0;i<=n;i++)	for(j=0;j<=n;j++)	// x range
		{
			SetFBord(Min.x+i*(Max.x-Min.x)/n, Min.y+j*(Max.y-Min.y)/n, Min.x);
			SetFBord(Min.x+i*(Max.x-Min.x)/n, Min.y+j*(Max.y-Min.y)/n, Max.z);
		}
		if(!fx)	{	FMin.x = Min.x;	FMax.x = Max.x;	}
		if(!fy)	{	FMin.y = Min.y;	FMax.y = Max.y;	}
		if(!fz)	{	FMin.z = Min.z;	FMax.z = Max.z;	}
	}
	RecalcCRange();
}
//-----------------------------------------------------------------------------
void mglBase::SetFBord(float x,float y,float z)
{
	if(fx)
	{
		float v = fx->Calc(x,y,z);
		if(FMax.x < v)	FMax.x = v;
		if(FMin.x > v)	FMin.x = v;
	}
	if(fy)
	{
		float v = fy->Calc(x,y,z);
		if(FMax.y < v)	FMax.y = v;
		if(FMin.y > v)	FMin.y = v;
	}
	if(fz)
	{
		float v = fz->Calc(x,y,z);
		if(FMax.z < v)	FMax.z = v;
		if(FMin.z > v)	FMin.z = v;
	}
}
//-----------------------------------------------------------------------------
bool mglBase::ScalePoint(mglPoint &p, bool use_nan)
{
	float &x=p.x, &y=p.y, &z=p.z;
	if(isnan(x) || isnan(y) || isnan(z))	{	x=NAN;	return false;	}
	float x1,y1,z1,x2,y2,z2;
	x1 = x>0?x*MGL_FLT_EPS:x/MGL_FLT_EPS;	x2 = x<0?x*MGL_FLT_EPS:x/MGL_FLT_EPS;
	y1 = y>0?y*MGL_FLT_EPS:y/MGL_FLT_EPS;	y2 = y<0?y*MGL_FLT_EPS:y/MGL_FLT_EPS;
	z1 = z>0?z*MGL_FLT_EPS:z/MGL_FLT_EPS;	z2 = z<0?z*MGL_FLT_EPS:z/MGL_FLT_EPS;
	bool res = true;
	if(x2>CutMin.x && x1<CutMax.x && y2>CutMin.y && y1<CutMax.y &&
		z2>CutMin.z && z1<CutMax.z)	res = false;
	if(fc && fc->Calc(x,y,z))	res = false;

	if(Cut || !use_nan)
	{
//		if(x1<Min.x || x2>Max.x || y1<Min.y || y2>Max.y || z1<Min.z || z2>Max.z)	res = false;
		if((x1-Min.x)*(x1-Max.x)>0 && (x2-Min.x)*(x2-Max.x)>0)	res = false;
		if((y1-Min.y)*(y1-Max.y)>0 && (y2-Min.y)*(y2-Max.y)>0)	res = false;
		if((z1-Min.z)*(z1-Max.z)>0 && (z2-Min.z)*(z2-Max.z)>0)	res = false;
	}
	else
	{
		if(x1<Min.x)	x=Min.x;	if(x2>Max.x)	x=Max.x;
		if(y1<Min.y)	y=Min.y;	if(y2>Max.y)	y=Max.y;
		if(z1<Min.z)	z=Min.z;	if(z2>Max.z)	z=Max.z;
	}

	if(fx)	x1 = fx->Calc(x,y,z);	else	x1=x;
	if(fy)	y1 = fy->Calc(x,y,z);	else	y1=y;
	if(fz)	z1 = fz->Calc(x,y,z);	else	z1=z;
	if(isnan(x1) || isnan(y1) || isnan(z1))	{	x=NAN;	return false;	}

	x = (2*x1 - FMin.x - FMax.x)/(FMax.x - FMin.x);
	y = (2*y1 - FMin.y - FMax.y)/(FMax.y - FMin.y);
	z = (2*z1 - FMin.z - FMax.z)/(FMax.z - FMin.z);
	if(TernAxis)
	{
		if(x+y>0)
		{
			if(Cut)	res = false;
			else	y = -x;
		}
		x = x + (y+1)/2;
	}
	if(fabs(x)>MGL_FLT_EPS)	res = false;
	if(fabs(y)>MGL_FLT_EPS)	res = false;
	if(fabs(z)>MGL_FLT_EPS)	res = false;
	if(!res && use_nan)	x = NAN;	// extra sign that point shouldn't be plotted
	return res;
}
//-----------------------------------------------------------------------------
//		Ranges
//-----------------------------------------------------------------------------
void mglBase::SetRanges(mglPoint m1, mglPoint m2)
{
	if(m1.x<m2.x)	{	Min.x=m1.x;	Max.x = m2.x;	}
	if(m1.x>m2.x)	{	Min.x=m2.x;	Max.x = m1.x;	}
	if(m1.y<m2.y)	{	Min.y=m1.y;	Max.y = m2.y;	}
	if(m1.y>m2.y)	{	Min.y=m2.y;	Max.y = m1.y;	}
	if(m1.z<m2.z)	{	Min.z=m1.z;	Max.z = m2.z;	}
	if(m1.z>m2.z)	{	Min.z=m2.z;	Max.z = m1.z;	}
	if(m1.c<m2.c)	{	Min.c=m1.c;	Max.c = m2.c;	}
	if(m1.c>m2.c)	{	Min.c=m2.c;	Max.c = m1.c;	}
	if(Min.c==Max.c)	{	Min.c=Min.z;	Max.c=Max.z;	}
//	if(AutoOrg)
	{
		if(Org.x<Min.x && !isnan(Org.x))	Org.x = Min.x;
		if(Org.x>Max.x && !isnan(Org.x))	Org.x = Max.x;
		if(Org.y<Min.y && !isnan(Org.y))	Org.y = Min.y;
		if(Org.y>Max.y && !isnan(Org.y))	Org.y = Max.y;
		if(Org.z<Min.z && !isnan(Org.z))	Org.z = Min.z;
		if(Org.z>Max.z && !isnan(Org.z))	Org.z = Max.z;
	}
	CutMin = mglPoint(0,0,0);	CutMax = mglPoint(0,0,0);
	RecalcBorder();
}
//-----------------------------------------------------------------------------
void mglBase::CRange(const mglDataA &a,bool add, float fact)
{
	float v1=a.Minimal(), v2=a.Maximal(), dv;
	dv=(v2-v1)*fact;	v1 -= dv;	v2 += dv;
	if(!add)	{	Min.c = v1;	Max.c = v2;	}
	else if(Min.c<Max.c)
	{
		if(Min.c>v1)	Min.c=v1;
		if(Max.c<v2)	Max.c=v2;
	}
	else
	{
		dv = Min.c;
		Min.c = v1<Max.c ? v1:Max.c;
		Max.c = v2>dv ? v2:dv;
	}
	if(Org.c<Min.c && !isnan(Org.c))	Org.c = Min.c;
	if(Org.c>Max.c && !isnan(Org.c))	Org.c = Max.c;
	RecalcCRange();
}
//-----------------------------------------------------------------------------
void mglBase::XRange(const mglDataA &a,bool add,float fact)
{
	float v1=a.Minimal(), v2=a.Maximal(), dv;
	dv=(v2-v1)*fact;	v1 -= dv;	v2 += dv;
	if(!add)	{	Min.x = v1;	Max.x = v2;	}
	else if(Min.x<Max.x)
	{
		if(Min.x>v1)	Min.x=v1;
		if(Max.x<v2)	Max.x=v2;
	}
	else
	{
		dv = Min.x;
		Min.x = v1<Max.x ? v1:Max.x;
		Max.x = v2>dv ? v2:dv;
	}
	if(Org.x<Min.x && !isnan(Org.x))	Org.x = Min.x;
	if(Org.x>Max.x && !isnan(Org.x))	Org.x = Max.x;
	RecalcBorder();
}
//-----------------------------------------------------------------------------
void mglBase::YRange(const mglDataA &a,bool add,float fact)
{
	float v1=a.Minimal(), v2=a.Maximal(), dv;
	dv=(v2-v1)*fact;	v1 -= dv;	v2 += dv;
	if(!add)	{	Min.y = v1;	Max.y = v2;	}
	else if(Min.y<Max.y)
	{
		if(Min.y>v1)	Min.y=v1;
		if(Max.y<v2)	Max.y=v2;
	}
	else
	{
		dv = Min.y;
		Min.y = v1<Max.y ? v1:Max.y;
		Max.y = v2>dv ? v2:dv;
	}
	if(Org.y<Min.y && !isnan(Org.y))	Org.y = Min.y;
	if(Org.y>Max.y && !isnan(Org.y))	Org.y = Max.y;
	RecalcBorder();
}
//-----------------------------------------------------------------------------
void mglBase::ZRange(const mglDataA &a,bool add,float fact)
{
	float v1=a.Minimal(), v2=a.Maximal(), dv;
	dv=(v2-v1)*fact;	v1 -= dv;	v2 += dv;
	if(!add)	{	Min.z = v1;	Max.z = v2;	}
	else if(Min.z<Max.z)
	{
		if(Min.z>v1)	Min.z=v1;
		if(Max.z<v2)	Max.z=v2;
	}
	else
	{
		dv = Min.z;
		Min.z = v1<Max.z ? v1:Max.z;
		Max.z = v2>dv ? v2:dv;
	}
	if(Org.z<Min.z && !isnan(Org.z))	Org.z = Min.z;
	if(Org.z>Max.z && !isnan(Org.z))	Org.z = Max.z;
	RecalcBorder();
}
//-----------------------------------------------------------------------------
void mglBase::SetAutoRanges(float x1, float x2, float y1, float y2, float z1, float z2, float c1, float c2)
{
	if(x1!=x2)	{	Min.x = x1;	Max.x = x2;	}
	if(y1!=y2)	{	Min.y = y1;	Max.y = y2;	}
	if(z1!=z2)	{	Min.z = z1;	Max.z = z2;	}
	if(c1!=c2)	{	Min.c = c1;	Max.c = c2;	}
}
//-----------------------------------------------------------------------------
void mglBase::Ternary(int t)
{
	static mglPoint x1(-1,-1,-1),x2(1,1,1),o(NAN,NAN,NAN);
	static bool c = true;
	TernAxis = t;
	if(t)
	{
		x1 = Min;	x2 = Max;	o = Org;	c = Cut;
		Cut = false;
		SetRanges(mglPoint(0,0,0),mglPoint(1,1,t==1?0:1));
		Org=mglPoint(0,0,0);
	}
	else	{	SetRanges(x1,x2);	Org=o;	Cut=c;	}
}
//-----------------------------------------------------------------------------
//		Transformation functions
//-----------------------------------------------------------------------------
void mglBase::SetFunc(const char *EqX,const char *EqY,const char *EqZ,const char *EqA)
{
	if(fa)	delete fa;	if(fx)	delete fx;
	if(fy)	delete fy;	if(fz)	delete fz;
	if(EqX && EqX[0] && (EqX[0]!='x' || EqX[1]!=0))
		fx = new mglFormula(EqX);
	else	fx = 0;
	if(EqY && EqY[0] && (EqY[0]!='y' || EqY[1]!=0))
		fy = new mglFormula(EqY);
	else	fy = 0;
	if(EqZ && EqZ[0] && (EqZ[0]!='z' || EqZ[1]!=0))
		fz = new mglFormula(EqZ);
	else	fz = 0;
	if(EqA && EqA[0] && ((EqA[0]!='c' && EqA[0]!='a') || EqA[1]!=0))
		fa = new mglFormula(EqA);
	else	fa = 0;
	RecalcBorder();
}
//-----------------------------------------------------------------------------
void mglBase::CutOff(const char *EqC)
{
	if(fc)	delete fc;
	if(EqC && EqC[0])	fc = new mglFormula(EqC);	else	fc = 0;
}
//-----------------------------------------------------------------------------
void mglBase::SetCoor(int how)
{
	switch(how)
	{
	case mglCartesian:	SetFunc(0,0);	break;
	case mglPolar:
		SetFunc("x*cos(y)","x*sin(y)");	break;
	case mglSpherical:
		SetFunc("x*sin(y)*cos(z)","x*sin(y)*sin(z)","x*cos(y)");	break;
	case mglParabolic:
		SetFunc("x*y","(x*x-y*y)/2");	break;
	case mglParaboloidal:
		SetFunc("(x*x-y*y)*cos(z)/2","(x*x-y*y)*sin(z)/2","x*y");	break;
	case mglOblate:
		SetFunc("cosh(x)*cos(y)*cos(z)","cosh(x)*cos(y)*sin(z)","sinh(x)*sin(y)");	break;
//		SetFunc("x*y*cos(z)","x*y*sin(z)","(x*x-1)*(1-y*y)");	break;
	case mglProlate:
		SetFunc("sinh(x)*sin(y)*cos(z)","sinh(x)*sin(y)*sin(z)","cosh(x)*cos(y)");	break;
	case mglElliptic:
		SetFunc("cosh(x)*cos(y)","sinh(x)*sin(y)");	break;
	case mglToroidal:
		SetFunc("sinh(x)*cos(z)/(cosh(x)-cos(y))","sinh(x)*sin(z)/(cosh(x)-cos(y))",
			"sin(y)/(cosh(x)-cos(y))");	break;
	case mglBispherical:
		SetFunc("sin(y)*cos(z)/(cosh(x)-cos(y))","sin(y)*sin(z)/(cosh(x)-cos(y))",
			"sinh(x)/(cosh(x)-cos(y))");	break;
	case mglBipolar:
		SetFunc("sinh(x)/(cosh(x)-cos(y))","sin(y)/(cosh(x)-cos(y))");	break;
	default:	SetFunc(0,0);	break;
	}
}
//-----------------------------------------------------------------------------
void mglBase::ClearEq()
{
	if(fx)	delete fx;	if(fy)	delete fy;	if(fz)	delete fz;
	if(fa)	delete fa;	if(fc)	delete fc;
	fx = fy = fz = fc = fa = 0;
	RecalcBorder();
}
//-----------------------------------------------------------------------------
//		Colors ids
//-----------------------------------------------------------------------------
mglColorID mglColorIds[] = {{'k', mglColor(0,0,0)},
	{'r', mglColor(1,0,0)},		{'R', mglColor(0.5,0,0)},
	{'g', mglColor(0,1,0)},		{'G', mglColor(0,0.5,0)},
	{'b', mglColor(0,0,1)},		{'B', mglColor(0,0,0.5)},
	{'w', mglColor(1,1,1)},		{'W', mglColor(0.7,0.7,0.7)},
	{'c', mglColor(0,1,1)},		{'C', mglColor(0,0.5,0.5)},
	{'m', mglColor(1,0,1)},		{'M', mglColor(0.5,0,0.5)},
	{'y', mglColor(1,1,0)},		{'Y', mglColor(0.5,0.5,0)},
	{'h', mglColor(0.5,0.5,0.5)},	{'H', mglColor(0.3,0.3,0.3)},
	{'l', mglColor(0,1,0.5)},	{'L', mglColor(0,0.5,0.25)},
	{'e', mglColor(0.5,1,0)},	{'E', mglColor(0.25,0.5,0)},
	{'n', mglColor(0,0.5,1)},	{'N', mglColor(0,0.25,0.5)},
	{'u', mglColor(0.5,0,1)},	{'U', mglColor(0.25,0,0.5)},
	{'q', mglColor(1,0.5,0)},	{'Q', mglColor(0.5,0.25,0)},
	{'p', mglColor(1,0,0.5)},	{'P', mglColor(0.5,0,0.25)},
	{' ', mglColor(-1,-1,-1)},	{0, mglColor(-1,-1,-1)}	// the last one MUST have id=0
};
//-----------------------------------------------------------------------------
void mglColor::Set(mglColor c, float br)
{
	if(br<0)	br=0;	if(br>2.f)	br=2.f;
	r = br<=1.f ? c.r*br : 1 - (1-c.r)*(2-br);
	g = br<=1.f ? c.g*br : 1 - (1-c.g)*(2-br);
	b = br<=1.f ? c.b*br : 1 - (1-c.b)*(2-br);
	a = 1;
}
//-----------------------------------------------------------------------------
void mglColor::Set(char p, float bright)
{
	Set(-1,-1,-1);
	for(long i=0; mglColorIds[i].id; i++)
		if(mglColorIds[i].id==p)
		{	Set(mglColorIds[i].col, bright);	break;	}
}
//-----------------------------------------------------------------------------
void mglTexture::Set(const char *s, int smooth, float alpha)
{
	bool sm;		///< Use smoothed color
	mglColor *c;	///< Colors itself

	// NOTE: New syntax -- colors are CCCCC or {CNCNCCCN}; options inside []
	if(!s || !s[0])	return;
	if(strchr(s,'|') && !smooth)	smooth = -1;
	sm = smooth>=0;
	register long i,j=0,m=0,l=strlen(s);
	const char *cols = "kwrgbcymhWRGBCYMHlenpquLENPQU";
	for(i=0;i<l;i++)		// find number of colors
	{
		if(s[i]=='[')	j++;	if(s[i]==']')	j--;
		if(strchr(cols,s[i]) && j<1)	n++;
	}
	if(!n)	return;
	c = new mglColor[2*n];
	bool map = (smooth==2);
	for(i=j=n=0;i<l;i++)	// fill colors
	{
		if(s[i]=='[')	j++;	if(s[i]==']')	j--;
		if(j>0 && s[i]=='M')	map=true;
		if(s[i]=='{')	m++;	if(s[i]=='}')	m--;
		if(strchr(cols,s[i]) && j<1)		// this is color
		{
			if(m>0 && s[i+1]>'0' && s[i+1]<='9')// ext color
			{	c[2*n] = mglColor(s[i],(s[i+1]-'0')/5.f);	i++;	}
			else	c[2*n] = mglColor(s[i]);	// usual color
			n++;
		}
		// NOTE: User can change alpha if it placed like {AN}
		if(s[i]=='A' && j<1 && m>0 && s[i+1]>'0' && s[i+1]<='9')
		{	alpha = 0.1*(s[i+1]-'0');	i++;	}
	}
	for(i=0;i<n;i++)	// default texture
	{	c[2*i+1]=c[2*i];	c[2*i].a=alpha;	c[2*i+1].a=sm?0:alpha;	}
	if(map && sm)		// map texture
	{
		if(n==2)
		{	c[1]=c[2];	c[2]=c[0];	c[0]=BC;	c[3]=c[1]+c[2];	}
		else if(n==3)
		{	c[1]=c[2];	c[2]=c[0];	c[0]=BC;	c[3]=c[4];	n=2;}
		else
		{	c[1]=c[4];	c[3]=c[6];	n=2;	}
	}
	register float u;
	for(i=0;i<256;i++)
	{
		u = (n-1)*i/256.;	j = long(u);	u-=j;
		if(!sm || j>=n-1)
		{	col[2*i] = c[2*j];	col[2*i+1] = c[2*j+1];	}
		else
		{
			col[2*i] = c[2*j]*(1-u)+c[2*j+2]*u;
			col[2*i+1]=c[2*j+1]*(1-u)+c[2*j+3]*u;
		}
	}
	col[512]=col[510];	col[513]=col[511];
	delete []c;
}
//-----------------------------------------------------------------------------
void mglTexture::GetC(float u,float v,float cc[4])
{
	u -= long(u);
	register long i=long(256*u);
	mglColor *s=col+2*i;
	u = u*256-i;
	cc[0] = (s[0].r*(1-u)+s[2].r*u)*(1-v) + (s[1].r*(1-u)+s[3].r*u)*v;
	cc[1] = (s[0].g*(1-u)+s[2].g*u)*(1-v) + (s[1].g*(1-u)+s[3].g*u)*v;
	cc[2] = (s[0].b*(1-u)+s[2].b*u)*(1-v) + (s[1].b*(1-u)+s[3].b*u)*v;
	cc[3] = (s[0].a*(1-u)+s[2].a*u)*(1-v) + (s[1].a*(1-u)+s[3].a*u)*v;
}
//-----------------------------------------------------------------------------
bool mglTexture::IsSame(mglTexture &t)
{	return !memcmp(col,t.col,514*sizeof(mglColor));	}
//-----------------------------------------------------------------------------
long mglBase::AddTexture(const char *cols, int smooth)
{
	mglTexture *t=new mglTexture;	t->Set(cols,smooth);
	if(t->n==0)	return smooth<0 ? 0:1;
	// check if already exist
	for(long i=0;i<numT;i++)	if(t->IsSame(txt[i]))	return i;
	// create new one
	txt = (mglTexture *)realloc(txt,(numT+1)*sizeof(mglTexture));
	memcpy(txt+numT, t, sizeof(mglTexture));	numT++;
	delete t;
	return numT-1;
}
//-----------------------------------------------------------------------------
float mglBase::AddTexture(char col)
{
	mglColor c(col);
	register long i,j;
	if(!c.Valid())	return -1;
	// first lets try an existed one
	for(i=0;i<numT;i++)	for(j=0;j<256;j++)
		if(c==txt[i].col[2*j])	return i+j/255.;
	// add new texture
	txt = (mglTexture *)realloc(txt,(numT+1)*sizeof(mglTexture));
	mglTexture *t=txt+numT;
	for(i=0;i<514;i++)	t->col[i]=c;	numT++;
	return numT-1;
}
//-----------------------------------------------------------------------------
//		Coloring and palette
//-----------------------------------------------------------------------------
float mglBase::NextColor(long &id)
{	// NOTE: I suppose that texture contain not more than 256 colors!
	long i=abs(id)/256, n=txt[i].n, p=abs(id)&0xff;
	if(id>=0)	{	p=(p+1)%n;	id = 256*i+p;	}
	last_style[0]=MGL_DEF_PAL[p%strlen(MGL_DEF_PAL)];	// TODO: last_style correctly !!!
	CDef = i + (n>0 ? 0.99*p/(n-1.) : 0);
	return CDef;
}
//-----------------------------------------------------------------------------
char mglBase::SetPenPal(const char *p, long *Id)
{
	char mk=0, pp=0;

	Arrow1 = Arrow2 = 0;
	if(p && *p!=0)
	{
//		const char *col = "wkrgbcymhRGBCYMHWlenuqpLENUQP";
		const char *stl = " -|;:ji=";
		const char *mrk = "*o+xsd.^v<>";
		const char *wdh = "123456789";
		const char *arr = "AKDTVISO_";
		for(unsigned i=0;i<strlen(p);i++)
		{
			if(strchr(stl,p[i]))
			{
				switch(p[i])
				{
				case '-': PDef = 0xffff;	break;
				case '|': PDef = 0x00ff;	break;
				case ';': PDef = 0x0f0f;	break;
				case '=': PDef = 0x3333;	break;
				case ':': PDef = 0x1111;	break;
				case 'j': PDef = 0x087f;	break;
				case 'i': PDef = 0x2727;	break;
				case ' ': PDef = 0x0000;	break;
				}
				pp = last_style[1]=p[i];
			}
			else if(strchr(mrk,p[i]))	mk = p[i];
			else if(strchr(wdh,p[i]))
			{	last_style[2] = p[i];	PenWidth = p[i]-'0';	}
			else if(strchr(arr,p[i]))
			{
				if(!Arrow1)	Arrow1 = p[i];
				else	Arrow2 = p[i];
			}
		}
		if(Arrow1=='_')	Arrow1=0;	if(Arrow2=='_')	Arrow2=0;
		if(strchr(p,'#'))
		{
			if(mk=='.')	mk = 'C';
			if(mk=='+')	mk = 'P';
			if(mk=='x')	mk = 'X';
			if(mk=='o')	mk = 'O';
			if(mk=='d')	mk = 'D';
			if(mk=='s')	mk = 'S';
			if(mk=='^')	mk = 'T';
			if(mk=='v')	mk = 'V';
			if(mk=='<')	mk = 'L';
			if(mk=='>')	mk = 'R';
			if(mk=='*')	mk = 'Y';
		}
	}
	last_style[3]=mk;	SetPen(pp, PenWidth);
	long tt;
	tt = AddTexture(p,-1);	CDef = tt;
	if(Id)	*Id=long(tt)*256+txt[tt].n-1;
	return mk;
}
//-----------------------------------------------------------------------------
void mglBase::SetPen(char style,float width)	{}
//-----------------------------------------------------------------------------
float mglBase::GetA(float a)
{
	if(fa)		a = fa->Calc(0,0,0,a);
	a = (a-FMin.c)/(FMax.c-FMin.c);
	a = a<1?(a>0?a:0):1/MGL_FLT_EPS;	// for texture a must be <1 always!!!
	return a;
}
//-----------------------------------------------------------------------------
mglPoint GetX(const mglDataA *x, int i, int j, int k)
{
	k = k<x->GetNz() ? k : 0;
	if(x->GetNy()>j && x->GetNx()>i && x->GetNy()>1)
		return mglPoint(x->v(i,j,k),x->dvx(i,j,k),x->dvy(i,j,k));
	else if(x->GetNx()>i)
		return mglPoint(x->v(i),x->dvx(i),0);
	return mglPoint(0);
}
//-----------------------------------------------------------------------------
mglPoint GetY(const mglDataA *y, int i, int j, int k)
{
	k = k<y->GetNz() ? k : 0;
	if(y->GetNy()>j && y->GetNx()>i && y->GetNy()>1)
		return mglPoint(y->v(i,j,k),y->dvx(i,j,k),y->dvy(i,j,k));
	else if(y->GetNx()>j)
		return mglPoint(y->v(j),0,y->dvx(j));
	return mglPoint(0);
}
//-----------------------------------------------------------------------------
mglPoint GetZ(const mglDataA *z, int i, int j, int k)
{
	k = k<z->GetNz() ? k : 0;
	if(z->GetNy()>j && z->GetNx()>i && z->GetNy()>1)
		return mglPoint(z->v(i,j,k),z->dvx(i,j,k),z->dvy(i,j,k));
	else if(z->GetNx()>k)
		return mglPoint(z->v(k),0,0);
	return mglPoint(0);
}
//-----------------------------------------------------------------------------
void mglBase::vect_plot(long p1, long p2)	// position in pntC
{
	float pp[14], *pp1=pntC+7*p1, *pp2=pntC+7*p2;
	memcpy(pp,pp2,7*sizeof(float));
	memcpy(pp+7,pp2,7*sizeof(float));
	pp[0] = pp1[0]+0.8*(pp2[0]-pp1[0]) + 0.1*(pp2[1]-pp1[1]);
	pp[7] = pp1[0]+0.8*(pp2[0]-pp1[0]) - 0.1*(pp2[1]-pp1[1]);
	pp[1] = pp1[1]+0.8*(pp2[1]-pp1[1]) - 0.1*(pp2[0]-pp1[0]);
	pp[8] = pp1[1]+0.8*(pp2[1]-pp1[1]) + 0.1*(pp2[0]-pp1[0]);
	pp[2] = pp[9] = pp1[2]+0.8*(pp2[2]-pp1[2]);

	if(posC>numC-3)
	{	numC+=1024;	pntC=(float *)realloc(pntC,7*numC*sizeof(float));	}
	memcpy(pntC+posC*7,pp,14*sizeof(float));
	line_plot(p1,p2);
	line_plot(posC,p2);
	line_plot(p2,posC+1);
	posC+=2;
}
//-----------------------------------------------------------------------------
void mglBase::SaveState()
{
	MSS=MarkSize;	ASS=ArrowSize;
	FSS=FontSize;	ADS=AlphaDef;
	MNS=MeshNum;	CSS=Cut;
	MinS=Min;	MaxS=Max;
}
void mglBase::LoadState()
{
	MarkSize=MSS;	ArrowSize=ASS;
	FontSize=FSS;	AlphaDef=ADS;
	MeshNum=MNS;	Cut=CSS;
	Min=MinS;	Max=MaxS;
}