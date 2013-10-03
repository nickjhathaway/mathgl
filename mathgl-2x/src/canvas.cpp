/***************************************************************************
 * canvas.cpp is part of Math Graphic Library
 * Copyright (C) 2007-2012 Alexey Balakin <mathgl.abalakin@gmail.ru>       *
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
#include <limits.h>
#include "mgl2/font.h"
#include "mgl2/canvas.h"
//-----------------------------------------------------------------------------
MGL_EXPORT std::string mglGlobalMess;	///< Buffer for receiving global messages
//-----------------------------------------------------------------------------
mglCanvas::mglCanvas(int w, int h) : mglBase()
{
	clr(MGL_DISABLE_SCALE);
	set(MGL_VECT_FRAME);	// NOTE: require a lot of memory!
	Z=0;	C=G=G4=0;	OI=0;	gif=0;
	CurFrameId=0;	Delay=0.5;
	Width=Height=Depth=0;	ObjId=-1;
	fscl=ftet=0;		PlotId = "frame";
	dr_nx1=dr_nx2=dr_ny1=dr_ny2=0;	// Allowed drawing region

	ac.ch='c';
	ax.dir = mglPoint(1,0,0);	ax.a = mglPoint(0,1,0);	ax.b = mglPoint(0,0,1);	ax.ch='x';
	ay.dir = mglPoint(0,1,0);	ay.a = mglPoint(1,0,0);	ay.b = mglPoint(0,0,1);	ay.ch='y';
	az.dir = mglPoint(0,0,1);	az.a = mglPoint(0,1,0);	az.b = mglPoint(1,0,0);	az.ch='z';

	SetSize(w,h);	SetQuality(MGL_DRAW_NORM);	DefaultPlotParam();
}
//-----------------------------------------------------------------------------
mglCanvas::~mglCanvas()
{
	if(G)	{	delete []G;	delete []C;	delete []Z;	delete []G4;delete []OI;	}
}
//-----------------------------------------------------------------------------
long mglCanvas::PushDrwDat()
{
	mglDrawDat d;
	d.Pnt=Pnt;	d.Prm=Prm;	d.Glf=Glf;	d.Ptx=Ptx;	d.Txt=Txt;
#pragma omp critical(drw)
	MGL_PUSH(DrwDat,d,mutexDrw);
	return DrwDat.size();
}
//-----------------------------------------------------------------------------
void mglCanvas::ResetFrames()	{	CurFrameId=0;	DrwDat.clear();	}
//-----------------------------------------------------------------------------
void mglCanvas::SetFrame(long i)
{
	if(get(MGL_VECT_FRAME) && i>=0 && i<long(DrwDat.size()))
	{
		Finish();	CurFrameId--;
		mglDrawDat d;
		d.Pnt=Pnt;	d.Prm=Prm;	d.Glf=Glf;	d.Ptx=Ptx;	d.Txt=Txt;
#if MGL_HAVE_PTHREAD
		pthread_mutex_lock(&mutexDrw);
#pragma omp critical(drw)
		DrwDat[i] = d;
		pthread_mutex_unlock(&mutexDrw);
#else
#pragma omp critical(drw)
		DrwDat[i] = d;
#endif
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::GetFrame(long k)
{
	if(k<0 || (size_t)k>=DrwDat.size())	return;
	Clf();
	const mglDrawDat &d=DrwDat[k];
#if MGL_HAVE_PTHREAD
	pthread_mutex_lock(&mutexPnt);
	pthread_mutex_lock(&mutexPrm);
	pthread_mutex_lock(&mutexGlf);
	pthread_mutex_lock(&mutexPtx);
	pthread_mutex_lock(&mutexTxt);
#endif
#pragma omp critical
	{	Pnt=d.Pnt;	Prm=d.Prm;	Glf=d.Glf;	Ptx=d.Ptx;	Txt=d.Txt;	}
#if MGL_HAVE_PTHREAD
	pthread_mutex_unlock(&mutexTxt);
	pthread_mutex_unlock(&mutexPtx);
	pthread_mutex_unlock(&mutexGlf);
	pthread_mutex_unlock(&mutexPrm);
	pthread_mutex_unlock(&mutexPnt);
#endif
}
//-----------------------------------------------------------------------------
void mglCanvas::ShowFrame(long k)
{
	if(k<0 || (size_t)k>=DrwDat.size())	return;
	ClfZB();
	size_t npnt=Pnt.size(), nglf=Glf.size(), nptx=Ptx.size(), ntxt=Txt.size();
#if MGL_HAVE_PTHREAD
	pthread_mutex_lock(&mutexPnt);
	pthread_mutex_lock(&mutexPrm);
	pthread_mutex_lock(&mutexGlf);
	pthread_mutex_lock(&mutexPtx);
	pthread_mutex_lock(&mutexTxt);
#endif
#pragma omp critical
	{
		const mglDrawDat &d=DrwDat[k];
		Glf.reserve(d.Glf.size());	for(size_t i=0;i<d.Glf.size();i++)	Glf.push_back(d.Glf[i]);
		Ptx.reserve(d.Ptx.size());	for(size_t i=0;i<d.Ptx.size();i++)	Ptx.push_back(d.Ptx[i]);
		Txt.reserve(d.Pnt.size());	for(size_t i=0;i<d.Txt.size();i++)	Txt.push_back(d.Txt[i]);
		Pnt.reserve(d.Pnt.size());
		for(size_t i=0;i<d.Pnt.size();i++)
		{
			mglPnt p = d.Pnt[i]; 	p.c += ntxt;
			Pnt.push_back(p);
		}
		Prm.reserve(d.Prm.size());
		for(size_t i=0;i<d.Prm.size();i++)
		{
			mglPrim p = d.Prm[i];
			p.n1 += npnt;

			switch(p.type)
			{
			case 1:	p.n2 += npnt;	break;
			case 2:	p.n2 += npnt;	p.n3 += npnt;	break;
			case 3:	p.n2 += npnt;	p.n3 += npnt;	p.n4 += npnt;	break;
			case 4: p.n4 += nglf;	break;
			case 5:	p.n2 += npnt;	break;
			case 6: p.n3 += nptx;	break;
			}
			Prm.push_back(p);
		}
	}
#if MGL_HAVE_PTHREAD
	pthread_mutex_unlock(&mutexPnt);
	pthread_mutex_unlock(&mutexPrm);
	pthread_mutex_unlock(&mutexGlf);
	pthread_mutex_unlock(&mutexPtx);
	pthread_mutex_unlock(&mutexTxt);
#endif
}
//-----------------------------------------------------------------------------
const unsigned char *mglCanvas::GetBits()	{	Finish();	return G;	}
//-----------------------------------------------------------------------------
mreal mglCanvas::GetRatio() const	{	return inW/inH;	}
//-----------------------------------------------------------------------------
void mglCanvas::add_prim(mglPrim &a)
{
	if(a.n1>=0)
	{
		a.z = Pnt[a.n1].z;	// this is a bit less accurate but simpler for transformation
		a.id = ObjId;
#pragma omp critical(prm)
		MGL_PUSH(Prm,a,mutexPrm);
		clr(MGL_FINISHED);
	}
}
//-----------------------------------------------------------------------------
extern uint64_t mgl_mask_def[16];
void mglCanvas::DefaultPlotParam()
{
/* NOTE: following variables and mutex will not be changed by DefaultPlotParam()
long InUse;			///< Smart pointer (number of users)
mglFont *fnt;		///< Class for printing vector text
int Quality;		///< Quality of plot (0x0-pure, 0x1-fast; 0x2-fine; 0x4 - low memory)
int Width;			///< Width of the image
int Height;			///< Height of the image
int Depth;			///< Depth of the image
int CurFrameId;		///< Number of automaticle created frames
GifFileType *gif;*/
	memcpy(mgl_mask_val, mgl_mask_def, 16*sizeof(uint64_t));	// should be > 16*8
	mgl_clear_fft();		ResetMask();
	SetTickRotate(true);	SetTickSkip(true);
	SetWarn(mglWarnNone,"");	mglGlobalMess = "";
	ObjId = -1;	HighId = INT_MIN;
	SetFunc(0,0);	Stop=false;	CutOff(0);	Ternary(0);
	SetRanges(mglPoint(-1,-1,-1,-1), mglPoint(1,1,1,1));
	SetOrigin(NAN,NAN,NAN,NAN);
	SetBarWidth(0.7);	SetMarkSize(1);	SetArrowSize(1);
	SetAlphaDef(0.5);		FontDef[0]=0;
	SetTranspType(0);		SetMeshNum(0);	// NOTE: default MeshNum=0
	SetRotatedText(true);	CurrPal = 0;
	SetLegendMarks();		SetFontSize(4);
	SetTuneTicks(-1);		SetAmbient();	SetDiffuse();
	clr(MGL_DISABLE_SCALE);
	clr(MGL_USE_GMTIME);	clr(MGL_NOSUBTICKS);
	SetDifLight(false);		SetReduceAcc(false);
	SetDefScheme(MGL_DEF_SCH);	SetPalette(MGL_DEF_PAL);
	SetPenPal("k-1");		Alpha(false);
	SetTicks('x');	SetTicks('y');	SetTicks('z');	SetTicks('c');
	stack.clear();	Restore();	DefColor('k');
	SetPlotFactor(0);	InPlot(0,1,0,1,false);
	SetTickLen(0);	SetCut(true);
	AdjustTicks("xyzc",true);	Clf('w');

	for(int i=0;i<10;i++)	{	AddLight(i, mglPoint(0,0,1));	Light(i,false);	}
	Light(0,true);	Light(false);	SetDifLight(true);
}
//-----------------------------------------------------------------------------
//	Optimal axis position
//-----------------------------------------------------------------------------
mreal mglCanvas::FindOptOrg(char dir, int ind) const
{
	static mglPoint px, py, pz;
	static mglMatrix bb;
	mglPoint nn[8]={mglPoint(0,0,0), mglPoint(0,0,1), mglPoint(0,1,0,0), mglPoint(0,1,1),
					mglPoint(1,0,0), mglPoint(1,0,1), mglPoint(1,1,0), mglPoint(1,1,1)}, pp[8];
	memcpy(pp, nn, 8*sizeof(mglPoint));
	// do nothing if transformation matrix is the same
	if(B!=bb)
	{
		bb = B;
#pragma omp parallel for
		for(long i=0;i<8;i++)	PostScale(&B,pp[i]);
		// find point with minimal y
		long j=0;
		for(long i=1;i<8;i++)	if(pp[i].y<pp[j].y)	j=i;
		pp[0]=pp[j];
		// first select 3 closest points
		pp[1].x=1-nn[j].x;	pp[1].y=nn[j].y;	pp[1].z=nn[j].z;	PostScale(&B,pp[1]);	pp[1]-=pp[0];
		pp[2].x=nn[j].x;	pp[2].y=1-nn[j].y;	pp[2].z=nn[j].z;	PostScale(&B,pp[2]);	pp[2]-=pp[0];
		pp[3].x=nn[j].x;	pp[3].y=nn[j].y;	pp[3].z=1-nn[j].z;	PostScale(&B,pp[3]);	pp[3]-=pp[0];
		// find cosine of axis projection
		register mreal tx=fabs(pp[1].x/pp[1].y), ty=fabs(pp[2].x/pp[2].y), tz=fabs(pp[3].x/pp[3].y);
		px=py=pz=nn[j];
		if(tz==0 && (ty==0 || tx==0))	// (x- & z-) or (y- & z-) axis are vertical
		{	if(pp[1].x>pp[2].x)	pz.y=1-pz.y;	else	pz.x=1-pz.x;	}
		else if(tx==0 && ty==0)	// x- && y-axis is vertical
		{
			py.x=1-py.x;
			if(pp[1].x>pp[3].x)
			{	px.z=1-px.z;	py.z=1-py.z;	}
		}
		else if(tz<tx && tz<ty)	// z-axis is vertical
		{	if(pp[1].x>pp[2].x)	pz.y=1-pz.y;	else	pz.x=1-pz.x;	}
		else if(ty<tx && ty<tz)	// y-axis is vertical
		{	if(pp[1].x>pp[3].x)	py.z=1-py.z;	else	py.x=1-py.x;	}
		else if(tx<tz && tx<tz)	// x-axis is vertical
		{	if(pp[3].x>pp[2].x)	px.y=1-px.y;	else	px.z=1-px.z;	}
	}
	// return to normal variables
	mglPoint rx = Min+(Max-Min)/px;
	mglPoint ry = Min+(Max-Min)/py;
	mglPoint rz = Min+(Max-Min)/pz;
	mreal res = rx.val(ind);
	if(dir=='y')	res = ry.val(ind);
	if(dir=='z')	res = rz.val(ind);
	return res;
}
//-----------------------------------------------------------------------------
mreal mglCanvas::GetOrgX(char dir, bool inv) const
{
	mreal res = Org.x;
	if(mgl_isnan(res))
	{
		if(strchr("xyz",dir))	res = FindOptOrg(dir,0);
		else if(dir=='t')		res = Min.x;
		else res = B.b[6]>0 ? Max.x:Min.x;
		if(inv)	res = Min.x+Max.x-res;
	}
	return res;
}
//-----------------------------------------------------------------------------
mreal mglCanvas::GetOrgY(char dir, bool inv) const
{
	mreal res = Org.y;
	if(mgl_isnan(res))
	{
		if(strchr("xyz",dir))	res = FindOptOrg(dir,1);
		else if(dir=='t')	res = Min.y;
		else res = B.b[7]>0 ? Max.y:Min.y;
		if(inv)	res = Min.y+Max.y-res;
	}
	return res;
}
//-----------------------------------------------------------------------------
mreal mglCanvas::GetOrgZ(char dir, bool inv) const
{
	mreal res = Org.z;
	if(mgl_isnan(res))
	{
		if(strchr("xyz",dir))	res = FindOptOrg(dir,2);
		else if(dir=='t')	res = Min.z;
		else res = B.b[8]>0 ? Max.z:Min.z;
		if(inv)	res = Min.z+Max.z-res;
	}
	return res;
}
//-----------------------------------------------------------------------------
//	Put primitives
//-----------------------------------------------------------------------------
#define MGL_MARK_PLOT	if(Quality&MGL_DRAW_LMEM)	mark_draw(Pnt[p],type,size,&d);else	\
						{	mglPrim a;	a.w = pw;	a.s = size;	\
							a.n1 = p;	a.n4 = type;	a.angl=0;	add_prim(a);	}
void mglCanvas::mark_plot(long p, char type, mreal size)
{
	if(p<0 || mgl_isnan(Pnt[p].x) || mgl_isnan(size))	return;
	long pp=p;
//	mreal pw = fabs(PenWidth)*0.15/sqrt(font_factor);
	mreal pw = 0.15/sqrt(font_factor);
	mglDrawReg d;	d.set(this,1,1,0);
	d.PDef = PDef;	d.pPos = pPos;	d.PenWidth=pw;
	if(size>=0)	size *= MarkSize;
	if(size==0)	size = MarkSize;
	size *= 0.35*font_factor;
	if(type=='.')	size = fabs(PenWidth)*sqrt(font_factor/400);
	if(TernAxis&4) for(int i=0;i<4;i++)
	{	p = ProjScale(i, pp);	MGL_MARK_PLOT	}
	else	{	MGL_MARK_PLOT	}
}
//-----------------------------------------------------------------------------
#define MGL_LINE_PLOT	if(Quality&MGL_DRAW_LMEM)	line_draw(Pnt[p1],Pnt[p2],&dd);else	\
						{	mglPrim a(1);	a.n3=PDef;	a.s = pPos;	\
							a.n1 = p1;	a.n2 = p2;	a.w = pw;	a.angl=0;	add_prim(a);	}
void mglCanvas::line_plot(long p1, long p2)
{
	if(PDef==0)	return;
	if(p1<0 || p2<0 || mgl_isnan(Pnt[p1].x) || mgl_isnan(Pnt[p2].x))	return;
	if(p1>p2)	{	long kk=p1;	p1=p2;	p2=kk;	}	// rearrange start/end for proper dashing
	long pp1=p1,pp2=p2;
	mreal pw = fabs(PenWidth)*sqrt(font_factor/400), d;
	d = hypot(Pnt[p1].x-Pnt[p2].x, Pnt[p1].y-Pnt[p2].y);

	mglDrawReg dd;	dd.set(this,1,1,0);
	dd.PDef = PDef;	dd.pPos = pPos;	dd.PenWidth=pw;

	if(TernAxis&4) for(int i=0;i<4;i++)
	{	p1 = ProjScale(i, pp1);	p2 = ProjScale(i, pp2);
		MGL_LINE_PLOT	}
	else	{	MGL_LINE_PLOT	}
	pPos = fmod(pPos+d/pw/1.5, 16);
}
//-----------------------------------------------------------------------------
#define MGL_TRIG_PLOT	if(Quality&MGL_DRAW_LMEM)	trig_draw(Pnt[p1],Pnt[p2],Pnt[p3],true,&d);else	\
						{	mglPrim a(2);	a.n1 = p1;	a.n2 = p2;	a.n3 = p3;	\
							a.m=mask;	a.angl=mask_an;	a.w = pw;	add_prim(a);}
void mglCanvas::trig_plot(long p1, long p2, long p3)
{
	if(p1<0 || p2<0 || p3<0 || mgl_isnan(Pnt[p1].x) || mgl_isnan(Pnt[p2].x) || mgl_isnan(Pnt[p3].x))	return;
	long pp1=p1,pp2=p2,pp3=p3;
	mreal pw = fabs(PenWidth)*sqrt(font_factor/400);
	mglDrawReg d;	d.set(this,1,1,0);	d.PenWidth=pw;
	if(TernAxis&4) for(int i=0;i<4;i++)
	{	p1 = ProjScale(i, pp1);	p2 = ProjScale(i, pp2);
		p3 = ProjScale(i, pp3);	MGL_TRIG_PLOT	}
	else	{	MGL_TRIG_PLOT	}
}
//-----------------------------------------------------------------------------
#define MGL_QUAD_PLOT	if(Quality&MGL_DRAW_LMEM)	quad_draw(Pnt[p1],Pnt[p2],Pnt[p3],Pnt[p4],&d);else	\
						{	mglPrim a(3);	a.n1 = p1;	a.n2 = p2;	a.n3 = p3;	a.n4 = p4;	\
							a.m=mask;	a.angl=mask_an;	a.w = pw;	add_prim(a);	}
void mglCanvas::quad_plot(long p1, long p2, long p3, long p4)
{
	if(p1<0 || mgl_isnan(Pnt[p1].x))	{	trig_plot(p4,p2,p3);	return;	}
	if(p2<0 || mgl_isnan(Pnt[p2].x))	{	trig_plot(p1,p4,p3);	return;	}
	if(p3<0 || mgl_isnan(Pnt[p3].x))	{	trig_plot(p1,p2,p4);	return;	}
	if(p4<0 || mgl_isnan(Pnt[p4].x))	{	trig_plot(p1,p2,p3);	return;	}
	long pp1=p1,pp2=p2,pp3=p3,pp4=p4;
	mreal pw = fabs(PenWidth)*sqrt(font_factor/400);
	mglDrawReg d;	d.set(this,1,1,0);	d.PenWidth=pw;
	if(TernAxis&4) for(int i=0;i<4;i++)
	{	p1 = ProjScale(i, pp1);	p2 = ProjScale(i, pp2);
		p3 = ProjScale(i, pp3);	p4 = ProjScale(i, pp4);
		MGL_QUAD_PLOT	}
	else	{	MGL_QUAD_PLOT	}
}
//-----------------------------------------------------------------------------
mreal mglCanvas::text_plot(long p,const wchar_t *text,const char *font,mreal size,mreal sh,mreal col,bool rot)
{
	if(p<0 || mgl_isnan(Pnt[p].x) || !text || *text==0)	return 0;
	if(size<0)	size *= -FontSize;
	if(!font)	font="";

	if(TernAxis&4)	// text at projections
	{
		mreal res;
		TernAxis = TernAxis&(~4);
		for(int i=0;i<4;i++)
			res = text_plot(ProjScale(i,p,true),text,font,size/2,sh,col);
		TernAxis = TernAxis|4;
		return res;
	}

	mglPnt q=Pnt[p];
	mreal ll = q.u*q.u+q.v*q.v;
	bool inv=false;
	if(rot && (q.u<0 || (q.u==0 && q.v<0)))
	{	q.u=-q.u;	q.v=-q.v;	q.w=-q.w;	inv=true;	}

	mreal fsize=size/6.5*font_factor, h = fnt->Height(font)*fsize, w, shift = -(sh+0.02)*h;
	// text drawing itself

#if MGL_HAVE_PTHREAD
pthread_mutex_lock(&mutexPtx);
#endif
#pragma omp critical(ptx)
	{
		Bt = B;
		inv = inv ^ (strchr(font,'T')!=0);
		if(inv)	shift = 0.2*h-shift;
		shift += 0.015*h;	// Correction for glyph rotation around proper point
	//		shift *= h;

		int align;	mglGetStyle(font,0,&align);	align = align&3;
		Bt.x = q.x;	Bt.y = q.y - shift;	Bt.z = q.z;
		if(ll>0)
		{
			Bt.x += shift*q.v/sqrt(ll);	Bt.y += shift*(1-q.u/sqrt(ll));
			if(q.u==0 && !get(MGL_ENABLE_RTEXT))	Bt.y -= 0.1*h;
		}
		fscl = fsize;	forg = p;

		if(mgl_isnan(ll) || !get(MGL_ENABLE_RTEXT))	ftet = 0;
		else if(ll)	ftet = -180*atan2(q.v,q.u)/M_PI;
		else 	ftet = NAN;

		if(!(Quality&MGL_DRAW_LMEM))	// add text itself
		{
			char ch = mglGetStyle(font,0,0);
			mglColor mc(ch);
			if(!ch)	mc = col<0 ? mglColor(char(0.5-col)):Txt[long(col)].GetC(col);

			mglPrim a(6);	a.n1 = p;
			a.n2 = int(255*mc.r) + 256*(int(255*mc.g) + 256*int(255*mc.b));
			mglText txt(text,font);
			Ptx.push_back(txt);	a.n3 = Ptx.size()-1;
			a.s = size;	a.w = shift;	a.p=ftet;
			add_prim(a);
		}

		memset(Bt.b,0,9*sizeof(mreal));
		Bt.b[0] = Bt.b[4] = Bt.b[8] = fscl;
		register mreal opf = Bt.pf;
		Bt.RotateN(ftet,0,0,1);	Bt.pf = opf;
		if(strchr(font,'@'))	// draw box around text
		{
			long k1,k2,k3,k4;	mglPnt pt;	mglPoint pp;
			w = fnt->Width(text,font);	h = fnt->Height(font);
//			int align;	mglGetStyle(font,0,&align);	align = align&3;
			mreal d=-w*align/2.-h*0.2;	w+=h*0.4;
			pt = q;	pp = mglPoint(d,-h*0.4);		PostScale(&Bt,pp);
			pt.x=pt.xx=pp.x;	pt.y=pt.yy=pp.y;
#pragma omp critical(pnt)
			{MGL_PUSH(Pnt,pt,mutexPnt);	k1=Pnt.size()-1;}
			pt = q;	pp = mglPoint(w+d,-h*0.4);		PostScale(&Bt,pp);
			pt.x=pt.xx=pp.x;	pt.y=pt.yy=pp.y;
#pragma omp critical(pnt)
			{MGL_PUSH(Pnt,pt,mutexPnt);	k2=Pnt.size()-1;}
			pt = q;	pp = mglPoint(d,h*1.2);			PostScale(&Bt,pp);
			pt.x=pt.xx=pp.x;	pt.y=pt.yy=pp.y;
#pragma omp critical(pnt)
			{MGL_PUSH(Pnt,pt,mutexPnt);	k3=Pnt.size()-1;}
			pt = q;	pp = mglPoint(w+d,h*1.2);		PostScale(&Bt,pp);
			pt.x=pt.xx=pp.x;	pt.y=pt.yy=pp.y;
#pragma omp critical(pnt)
			{MGL_PUSH(Pnt,pt,mutexPnt);	k4=Pnt.size()-1;}
			line_plot(k1,k2);	line_plot(k1,k3);
			line_plot(k4,k2);	line_plot(k4,k3);
		}
		fsize *= fnt->Puts(text,font,col)/2;
	}
#if MGL_HAVE_PTHREAD
	pthread_mutex_unlock(&mutexPtx);
#endif
	return fsize;
}
//-----------------------------------------------------------------------------
void mglCanvas::Glyph(mreal x, mreal y, mreal f, int s, long j, mreal col)
{
	mglPrim a(4);	// NOTE: no projection since text_plot() did it
	a.s = fscl/Bt.pf;
	a.w = get(MGL_ENABLE_RTEXT)?ftet:1e5;
	a.p = f/fnt->GetFact(s&3);
	mreal cc = col<0 ? AddTexture(char(0.5-col)):col;
	if(cc<0)	cc = CDef;
	a.n1 = AddPnt(&Bt, mglPoint(Bt.x,Bt.y,Bt.z), cc, mglPoint(x,y,NAN), -1, -1);
	a.n2 = forg; 	a.n3 = s;	a.n4 = AddGlyph(s,j);
	if(a.n1<0)	return;

	mglDrawReg d;	d.set(this,1,1,0);
	d.PDef = s;		d.pPos = a.s;	d.PenWidth=a.w;

	if(Quality&MGL_DRAW_LMEM)	glyph_draw(a,&d);
	else	add_prim(a);
}
//-----------------------------------------------------------------------------
//	Plot positioning functions
//-----------------------------------------------------------------------------
void mglCanvas::InPlot(mreal x1,mreal x2,mreal y1,mreal y2, const char *st)
{
	if(Width<=0 || Height<=0 || Depth<=0)	return;
	if(!st)		{	InPlot(x1,x2,y1,y2,false);	return;	}
	inW = Width*(x2-x1);	inH = Height*(y2-y1);
	inX=Width*x1;	inY=Height*y1;	ZMin=1;
	mglPrim p;	p.id = ObjId;
	p.n1=x1*Width;	p.n2=x2*Width;	p.n3=y1*Height;	p.n4=y2*Height;
#pragma omp critical(sub)
	MGL_PUSH(Sub,p,mutexSub);

	if(strchr(st,'T'))	{	y1*=0.9;	y2*=0.9;	}	// general title
	bool r = !(strchr(st,'r') || strchr(st,'R') || strchr(st,'>') || strchr(st,'g'));
	bool l = !(strchr(st,'l') || strchr(st,'L') || strchr(st,'<') || strchr(st,'g'));
	bool u = !(strchr(st,'u') || strchr(st,'U') || strchr(st,'_') || strchr(st,'g'));
	bool a = !(strchr(st,'a') || strchr(st,'A') || strchr(st,'^') || strchr(st,'g') || strchr(st,'t'));
	// let use simplified scheme -- i.e. no differences between axis, colorbar and/or title
	register mreal xs=(x1+x2)/2, ys=(y1+y2)/2, f1 = 1.3, f2 = 1.1;
	if(strchr(st,'#'))	f1=f2=1.55;
	if(r && l)	{	x2=xs+(x2-xs)*f1;	x1=xs+(x1-xs)*f1;	}
	else if(r)	{	x2=xs+(x2-xs)*f1;	x1=xs+(x1-xs)*f2;	}
	else if(l)	{	x2=xs+(x2-xs)*f2;	x1=xs+(x1-xs)*f1;	}
	if(a && u)	{	y2=ys+(y2-ys)*f1;	y1=ys+(y1-ys)*f1;	}
	else if(a)	{	y2=ys+(y2-ys)*f1;	y1=ys+(y1-ys)*f2;	}
	else if(u)	{	y2=ys+(y2-ys)*f2;	y1=ys+(y1-ys)*f1;	}

	B.clear();
	if(get(MGL_AUTO_FACTOR)) B.pf = 1.55;	// Automatically change plot factor !!!
	B.x = (x1+x2)/2*Width;
	B.y = (y1+y2)/2*Height;
	B.b[0] = Width*(x2-x1);	B.b[4] = Height*(y2-y1);
	B.b[8] = sqrt(B.b[0]*B.b[4]);
	B.z = (1.f-B.b[8]/(2*Depth))*Depth;
	B1=B;	font_factor = B.b[0] < B.b[4] ? B.b[0] : B.b[4];
}
//-----------------------------------------------------------------------------
void mglCanvas::InPlot(mglMatrix &M,mreal x1,mreal x2,mreal y1,mreal y2, bool rel)
{
	if(Width<=0 || Height<=0 || Depth<=0)	return;
	M.clear();
	if(get(MGL_AUTO_FACTOR)) M.pf = 1.55;	// Automatically change plot factor !!!
	if(rel)
	{
		M.x = B1.x + (x1+x2-1)/2*B1.b[0]/1.55;
		M.y = B1.y + (y1+y2-1)/2*B1.b[4]/1.55;
		M.b[0] = B1.b[0]*(x2-x1);	M.b[4] = B1.b[4]*(y2-y1);
		M.b[8] = sqrt(M.b[0]*M.b[4]);
		M.z = B1.z + (1.f-M.b[8]/(2*Depth))*B1.b[8];
	}
	else
	{
		M.x = (x1+x2)/2*Width;
		M.y = (y1+y2)/2*Height;
		M.b[0] = Width*(x2-x1);	M.b[4] = Height*(y2-y1);
		M.b[8] = sqrt(M.b[0]*M.b[4]);
		M.z = (1.f-M.b[8]/(2*Depth))*Depth;
		B1=B;
	}
	inW=M.b[0];	inH=M.b[4];	ZMin=1;
	inX=Width*x1;	inY=Height*y1;
	font_factor = M.b[0] < M.b[4] ? M.b[0] : M.b[4];
	mglPrim p;	p.id = ObjId;
	p.n1=x1*Width;	p.n2=x2*Width;	p.n3=y1*Height;	p.n4=y2*Height;
#pragma omp critical(sub)
	MGL_PUSH(Sub,p,mutexSub);
}
//-----------------------------------------------------------------------------
void mglCanvas::StickPlot(int num, int id, mreal tet, mreal phi)
{
	mreal dx,dy,wx,wy,x1,y1,f1,f2;
	mglPoint p1(-1,0,0), p2(1,0,0);
	// first iteration
	InPlot(0,1,0,1,true);	Rotate(tet, phi);
	PostScale(GetB(),p1);	PostScale(GetB(),p2);	f1 = B.pf;
	dx=(p2.x-p1.x)*1.55/B1.b[0];	dy=(p2.y-p1.y)*1.55/B1.b[4];
	wx=1/(1+(num-1)*fabs(dx));		wy=1/(1+(num-1)*fabs(dy));
	x1=dx>0?dx*id:dx*(id-num+1);	y1=dy>0?dy*id:dy*(id-num+1);
	InPlot(x1*wx,(x1+1)*wx,y1*wy,(y1+1)*wy,true);	Rotate(tet,phi);
	f2 = B.pf;	dx*=f1/f2;	dy*=f1/f2;	// add correction due to PlotFactor
	wx=1/(1+(num-1)*fabs(dx));		wy=1/(1+(num-1)*fabs(dy));
	x1=dx>0?dx*id:dx*(id-num+1);	y1=dy>0?dy*id:dy*(id-num+1);
	InPlot(x1*wx,(x1+1)*wx,y1*wy,(y1+1)*wy,true);	Rotate(tet,phi);
	f1=f2;	f2 = B.pf;	dx*=f1/f2;	dy*=f1/f2;	// add correction due to PlotFactor
	wx=1/(1+(num-1)*fabs(dx));		wy=1/(1+(num-1)*fabs(dy));
	x1=dx>0?dx*id:dx*(id-num+1);	y1=dy>0?dy*id:dy*(id-num+1);
	InPlot(x1*wx,(x1+1)*wx,y1*wy,(y1+1)*wy,true);	Rotate(tet,phi);
}
//-----------------------------------------------------------------------------
void mglCanvas::Rotate(mreal tetz,mreal tetx,mreal tety)
{
	B.Rotate(tetz,tetx,tety);
	if(get(MGL_AUTO_FACTOR))
	{
		mreal w=(fabs(B.b[3])+fabs(B.b[4])+fabs(B.b[5]))/B1.b[4];
		mreal h=(fabs(B.b[0])+fabs(B.b[1])+fabs(B.b[2]))/B1.b[0];
		B.pf = 1.55+0.6147*(w<h ? (h-1):(w-1));
	}
}
//-----------------------------------------------------------------------------
void mglMatrix::Rotate(mreal tetz,mreal tetx,mreal tety)
{
//	RotateN(TetX,1.,0.,0.);
//	RotateN(TetY,0.,1.,0.);
//	RotateN(TetZ,0.,0.,1.);
	mreal R[9], O[9];
	mreal cx=cos(tetx*M_PI/180), sx=-sin(tetx*M_PI/180), cy=cos(tety*M_PI/180), sy=-sin(tety*M_PI/180), cz=cos(tetz*M_PI/180), sz=-sin(tetz*M_PI/180);
	R[0] = cx*cy;			R[1] = -cy*sx;			R[2] = sy;
	R[3] = cx*sy*sz+cz*sx;	R[4] = cx*cz-sx*sy*sz;	R[5] =-cy*sz;
	R[6] = sx*sz-cx*cz*sy;	R[7] = cx*sz+cz*sx*sy;	R[8] = cy*cz;
	memcpy(O,b,9*sizeof(mreal));
	b[0] = R[0]*O[0] + R[3]*O[1] + R[6]*O[2];
	b[1] = R[1]*O[0] + R[4]*O[1] + R[7]*O[2];
	b[2] = R[2]*O[0] + R[5]*O[1] + R[8]*O[2];
	b[3] = R[0]*O[3] + R[3]*O[4] + R[6]*O[5];
	b[4] = R[1]*O[3] + R[4]*O[4] + R[7]*O[5];
	b[5] = R[2]*O[3] + R[5]*O[4] + R[8]*O[5];
	b[6] = R[0]*O[6] + R[3]*O[7] + R[6]*O[8];
	b[7] = R[1]*O[6] + R[4]*O[7] + R[7]*O[8];
	b[8] = R[2]*O[6] + R[5]*O[7] + R[8]*O[8];
}
//-----------------------------------------------------------------------------
void mglCanvas::RotateN(mreal Tet,mreal x,mreal y,mreal z)
{
	B.RotateN(Tet,x,y,z);
	if(get(MGL_AUTO_FACTOR))
	{
		mreal w=(fabs(B.b[3])+fabs(B.b[4])+fabs(B.b[5]))/B1.b[4];
		mreal h=(fabs(B.b[0])+fabs(B.b[1])+fabs(B.b[2]))/B1.b[0];
		B.pf = 1.55+0.6147*(w<h ? (h-1):(w-1));
	}
}
//-----------------------------------------------------------------------------
void mglMatrix::RotateN(mreal Tet,mreal x,mreal y,mreal z)
{
	mreal R[9],T[9],c=cos(Tet*M_PI/180),s=-sin(Tet*M_PI/180),r=1-c,n=sqrt(x*x+y*y+z*z);
	x/=n;	y/=n;	z/=n;
	T[0] = x*x*r+c;		T[1] = x*y*r-z*s;	T[2] = x*z*r+y*s;
	T[3] = x*y*r+z*s;	T[4] = y*y*r+c;		T[5] = y*z*r-x*s;
	T[6] = x*z*r-y*s;	T[7] = y*z*r+x*s;	T[8] = z*z*r+c;
	memcpy(R,b,9*sizeof(mreal));
	b[0] = T[0]*R[0] + T[3]*R[1] + T[6]*R[2];
	b[1] = T[1]*R[0] + T[4]*R[1] + T[7]*R[2];
	b[2] = T[2]*R[0] + T[5]*R[1] + T[8]*R[2];
	b[3] = T[0]*R[3] + T[3]*R[4] + T[6]*R[5];
	b[4] = T[1]*R[3] + T[4]*R[4] + T[7]*R[5];
	b[5] = T[2]*R[3] + T[5]*R[4] + T[8]*R[5];
	b[6] = T[0]*R[6] + T[3]*R[7] + T[6]*R[8];
	b[7] = T[1]*R[6] + T[4]*R[7] + T[7]*R[8];
	b[8] = T[2]*R[6] + T[5]*R[7] + T[8]*R[8];
}
//-----------------------------------------------------------------------------
void mglCanvas::View(mreal tetx,mreal tetz,mreal tety)
{	Bp.Rotate(-tetz,-tetx,-tety);	}
//-----------------------------------------------------------------------------
void mglCanvas::Zoom(mreal x1, mreal y1, mreal x2, mreal y2)
{
	Bp.pf=0;	Bp.clear();		ClfZB();
	if(x1==x2 || y1==y2)	{	x1=y1=0;	x2=y2=1;	}
	x1=2*x1-1;	x2=2*x2-1;	y1=2*y1-1;	y2=2*y2-1;
	Bp.b[0]=2/fabs(x2-x1);	Bp.b[4]=2/fabs(y2-y1);
	Bp.x=(x1+x2)/fabs(x2-x1);Bp.y=(y1+y2)/fabs(y2-y1);
}
//-----------------------------------------------------------------------------
int mglCanvas::GetSplId(long x,long y) const
{
	register long i,id=-1;
	for(i=Sub.size()-1;i>=0;i--)
	{
		const mglPrim &p = Sub[i];
		if(p.n1<=x && p.n2>=x && p.n3<=y && p.n4>=y)
		{	id=p.id;	break;	}
	}
	return id;
}
//-----------------------------------------------------------------------------
#define islog(a, b) (((a)>0 && (b)>10*(a)) || ((b)<0 && (a)<10*(b)))
void mglCanvas::Aspect(mreal Ax,mreal Ay,mreal Az)
{
	if(mgl_isnan(Ax))
	{
		mreal dy = (Max.y-Min.y), dx = (Max.x-Min.x);
		if(islog(Min.x,Max.x) && fx)	dx = log10(Max.x/Min.x);
		if(islog(Min.y,Max.y) && fy)	dy = log10(Max.y/Min.y);
		mreal f=exp(M_LN10*floor(0.5+log10(fabs(dy/dx))));
		if(mgl_isnum(Ay))	f=Ay;
		Ax = Height*dx*f;	Ay = Width*dy;	Az = Depth;
	}
	mreal a = fabs(Ax) > fabs(Ay) ? fabs(Ax) : fabs(Ay);
	a = a > fabs(Az) ? a : fabs(Az);
	if(a==0)	{	SetWarn(mglWarnZero,"Aspect");	return;	}
	Ax/=a;	Ay/=a;	Az/=a;
	B.b[0] *= Ax;		B.b[3] *= Ax;		B.b[6] *= Ax;
	B.b[1] *= Ay;		B.b[4] *= Ay;		B.b[7] *= Ay;
	B.b[2] *= Az;		B.b[5] *= Az;		B.b[8] *= Az;
}
//-----------------------------------------------------------------------------
//	Lighting and transparency
//-----------------------------------------------------------------------------
void mglCanvas::Fog(mreal d, mreal dz)	{	FogDist=d;	FogDz = dz;	}
//-----------------------------------------------------------------------------
void mglCanvas::Light(int n, bool enable)
{
	if(n<0 || n>9)	{	SetWarn(mglWarnLId,"Light");	return;	}
	light[n].n = enable;
}
//-----------------------------------------------------------------------------
void mglCanvas::AddLight(int n, mglPoint r, mglPoint d, char col, mreal br, mreal ap)
{
	if(n<0 || n>9)	{	SetWarn(mglWarnLId,"AddLight");	return;	}
	light[n].n = true;	light[n].a = ap>0?ap*ap:3;
	light[n].b = br;	light[n].r = r;
	light[n].d = d;		light[n].c = mglColor(col);
}
//-----------------------------------------------------------------------------
void mglCanvas::arrow_plot(long n1, long n2, char st)
{
	if(n1<0 || n2<0 || !strchr("AVKSDTIO",st))	return;
	float ll = PenWidth*ArrowSize*0.35*font_factor;
	uint64_t m=mask;	int ma=mask_an;
	ResetMask();
	if((Quality&3)==3)
		arrow_plot_3d(n1, n2, st, ll);
	else
		arrow_draw(n1, n2, st, ll);
	mask=m;	mask_an=ma;
}
//-----------------------------------------------------------------------------
void mglCanvas::Legend(const std::vector<mglText> &leg, mreal x, mreal y, const char *font, const char *opt)
{
	long n=leg.size();
	mreal iw, ih;
	if(n<1)	{	SetWarn(mglWarnLeg,"Legend");	return;	}
	mreal ll = SaveState(opt);	if(mgl_isnan(ll))	ll=0.1;
	if(saved)	MarkSize=MSS;	// restore back size of marks
	static int cgid=1;	StartGroup("Legend",cgid++);
	if(ll<=0 || mgl_isnan(ll))	ll=0.1;
	ll *=font_factor;
	mreal size = 0.8*FontSize;
	// setup font and parse absolute coordinates
	if(!font)	font="#";
	char *pA, *ff = new char[strlen(font)+3];
	strcpy(ff,font);	strcat(ff,":L");	Push();
	if((pA=strchr(ff,'A')))
	{	*pA = ' ';	InPlot(0,1,0,1,false);	iw=B1.b[0];	ih=B1.b[4];	}
	else	{	iw=B1.b[0]/B1.pf;	ih=B1.b[4]/B1.pf;	}
	// find sizes
	mreal h=TextHeight(font,size);
	mreal dx = 0.03*iw, dy = 0.03*ih, w=0, t, sp=TextWidth(" ",font,size);
	register long i,j;
	for(i=0;i<n;i++)		// find text length
	{
		t = TextWidth(leg[i].text.c_str(),font,size)+sp;
		if(leg[i].stl.empty())	t -= ll;
		w = w>t ? w:t;
	}
	w += ll+0.01*iw;	// add space for lines
	j = long((ih*0.95)/h);
	long ncol = 1+(n-1)/j, nrow = (n+ncol-1)/ncol;
	if(strchr(font,'-'))	// horizontal legend
	{
		j = long((iw*0.95)/w);
		nrow = 1+(n-1)/j;
		ncol = (n+nrow-1)/nrow;
	}
	if(mglchr(font,'^'))	// use "external" positioning
	{
		x = x>=0.5 ? x*iw : x*iw-w*ncol-2*dx;
		y = y>=0.5 ? y*ih : y*ih-h*nrow-2*dy;
	}
	else
	{
		x *= iw-w*ncol-2*dx;
		y *= ih-h*nrow-2*dy;
	}
	x += B.x-iw/2+dx;	y += B.y-ih/2+dy;
	// draw it
	long k1=0,k2=0,k3=0,k4=0;
	mglPoint p,q=mglPoint(NAN,NAN);

	for(i=0;ff[i] && ff[i]!=':';i++)	if(strchr(MGL_COLORS,ff[i]))
	{

		if(k1 && k2)	{	k3=ff[i];	k4++;	}	// NOTE: keep k3 for future usage
		if(k1 && !k2)	{	k2=ff[i];	k4++;	}
		if(!k1)	{	k1=ff[i];	k4++;	}
	}
	if(k4==2)	k2=0;
	if(k4==1)	k1=k2=0;
	mreal c1=AddTexture(char(k1?k1:'w')), c2=AddTexture(char(k2?k2:'k'));
	if((Flag&3)==2)	{	mreal cc=c1;	c2=c1;	c1=cc;	}

	if(strchr(ff,'#'))	// draw bounding box
	{
		SetPenPal("k-");
		k1=AddPnt(&B,mglPoint(x,y,Depth/MGL_FEPSILON),c1,q,-1,0);
		k2=AddPnt(&B,mglPoint(x+w*ncol,y,Depth/MGL_FEPSILON),c1,q,-1,0);
		k3=AddPnt(&B,mglPoint(x,y+h*nrow,Depth/MGL_FEPSILON),c1,q,-1,0);
		k4=AddPnt(&B,mglPoint(x+w*ncol,y+h*nrow,Depth/MGL_FEPSILON),c1,q,-1,0);
		quad_plot(k1,k2,k3,k4);
		k1=CopyNtoC(k1,c2);	k2=CopyNtoC(k2,c2);
		k3=CopyNtoC(k3,c2);	k4=CopyNtoC(k4,c2);
		line_plot(k1,k2);	line_plot(k2,k4);
		line_plot(k4,k3);	line_plot(k3,k1);
	}
	for(i=0;i<n;i++)	// draw lines and legend
	{
		register long iy=nrow-(i%nrow)-1,ix=i/nrow;
		char m=SetPenPal(leg[i].stl.c_str());
		k1=AddPnt(&B,mglPoint(x+ix*w+0.1*ll,y+iy*h+0.45*h,Depth),CDef,q,-1,0);
		k2=AddPnt(&B,mglPoint(x+ix*w+0.9*ll,y+iy*h+0.45*h,Depth),CDef,q,-1,0);	pPos=0;
		if(!leg[i].stl.empty())	line_plot(k1,k2);
		if(m)	for(j=0;j<LegendMarks;j++)
		{
			p = mglPoint(x+ix*w+0.1f*ll + (j+1)*0.8f*ll/(1.+LegendMarks),y+iy*h+0.45*h,Depth);
			mark_plot(AddPnt(&B,p,CDef,q,-1,0),m);
		}
		p = mglPoint(x+ix*w+((!leg[i].stl.empty())?ll:0.01*iw), y+iy*h+0.15*h, Depth);
		text_plot(AddPnt(&B,p,-1,q,-1,0), leg[i].text.c_str(), ff, size);
	}
	Pop();	EndGroup();	delete []ff;
}
//-----------------------------------------------------------------------------
void mglCanvas::Table(mreal x, mreal y, HCDT val, const wchar_t *text, const char *frm, const char *opt)
{
//	if(x>=1) 	{	SetWarn(mglWarnSpc,"Table");	return;	}
	long i,j,m=val->GetNy(),n=val->GetNx();
//	mreal pos=SaveState(opt);
	mreal vw = SaveState(opt);
	static int cgid=1;	StartGroup("Table",cgid++);
	bool grid = mglchr(frm,'#'), eqd = mglchr(frm,'='), lim = mglchr(frm,'|');
	if(mgl_isnan(vw))	vw=1;	else 	lim = true;
	if(!text)	text=L"";
	if(x<0)	x=0; 	if(y<0)	y=0; 	if(y>1)	y=1;
//	if(vw>1-x)	vw=1-x;

	wchar_t *buf = new wchar_t[m*32], sng[32];
	std::vector<std::wstring> str;
	for(i=0;i<n;i++)		// prepare list of strings first
	{
		*buf=0;
		for(j=0;j+1<m;j++)
		{
			mglprintf(sng,32,L"%.3g\n",val->v(i,j));
			wcscat(buf,sng);
		}
		mglprintf(sng,32,L"%.3g",val->v(i,m-1));
		wcscat(buf,sng);		str.push_back(buf);
	}
	delete []buf;

	mreal sp=2*TextWidth(" ",frm,-1), w=*text ? sp+TextWidth(text,frm,-1):0, w1=0, ww, h;
	for(i=0;i<n;i++)		// find width for given font size
	{
		ww = TextWidth(str[i].c_str(),frm,-1)+sp;
		w1 = w1<ww?ww:w1;
		if(!eqd)	w += ww;
	}
	if(eqd)	w += n*w1;
	// reduce font size if table have to be inside inplot
	mreal fsize=FontSize;
	if(lim && w>vw*inW)
	{	h=vw*inW/w;	SetFontSize(-h); 	w*=h; 	w1*=h;	sp*=h;	}
	h = TextHeight(frm,-1);	// now we can determine text height

	x = x*(inW-w)+B.x-inW/2;
	y = y*(inH-h*m)+B.y-inH/2;

	mglPoint p,q=mglPoint(NAN,NAN);
	long k1,k2;
	mreal xx,yy;
	if(grid)	// draw bounding box
	{
		SetPenPal("k-");
		k1=AddPnt(&B,mglPoint(x,y,Depth),-1,q,-1,0);
		k2=AddPnt(&B,mglPoint(x,y+m*h,Depth),-1,q,-1,0);
		line_plot(k1,k2);
		ww = *text ? TextWidth(text,frm,-1)+sp:0;
		k1=AddPnt(&B,mglPoint(x+ww,y,Depth),-1,q,-1,0);
		k2=AddPnt(&B,mglPoint(x+ww,y+m*h,Depth),-1,q,-1,0);
		line_plot(k1,k2);
		for(i=0,xx=x+ww,yy=y;i<n;i++)
		{
			xx += eqd ? w1:(TextWidth(str[i].c_str(),frm,-1)+sp);
			k1=AddPnt(&B,mglPoint(xx,yy,Depth),-1,q,-1,0);
			k2=AddPnt(&B,mglPoint(xx,yy+m*h,Depth),-1,q,-1,0);
			line_plot(k1,k2);
		}
		for(i=0,xx=x,yy=y;i<=m;i++)
		{
			k1=AddPnt(&B,mglPoint(xx,yy,Depth),-1,q,-1,0);
			k2=AddPnt(&B,mglPoint(xx+w,yy,Depth),-1,q,-1,0);
			line_plot(k1,k2);	yy += h;
		}
	}
	int align;	mglGetStyle(frm, 0, &align);
	if(*text)
	{
		ww = TextWidth(text,frm,-1)+sp;
		k1=AddPnt(&B,mglPoint(x+ww*align/2.,y+h*(m-0.99),Depth),-1,q,-1,0);
		text_plot(k1,text,frm);
	}
	else 	ww = 0;
	for(i=0,xx=x+ww,yy=y+h*(m-0.99);i<n;i++)	// draw lines and legend
	{
		ww = eqd ? w1:(TextWidth(str[i].c_str(),frm,-1)+sp);
		k1=AddPnt(&B,mglPoint(xx+ww*align/2.,yy,Depth),-1,q,-1,0);
		text_plot(k1,str[i].c_str(),frm);	xx += ww;
	}
	FontSize = fsize;	EndGroup();
}
//-----------------------------------------------------------------------------
void mglCanvas::Title(const char *title,const char *stl,mreal size)
{
	if(!title)	title="";
	MGL_TO_WCS(title,Title(wcs, stl,size));
}
//-----------------------------------------------------------------------------
void mglCanvas::Title(const wchar_t *title,const char *stl,mreal size)
{
	mreal s = size>0 ? size/FontSize:-size, h=TextHeight(stl,size)*s/2;
	if(h>=inH)	{	SetWarn(mglWarnSpc,"Title");	return;	}
	static int cgid=1;	StartGroup("Title",cgid++);
	bool box=mglchr(stl,'#');
	int align;	mglGetStyle(stl,0,&align);	align = align&3;
	mreal y=inY+inH-h;
	mglPoint p(inX + inW*align/2.,y,Depth),q(NAN);
	if(title)	text_plot(AddPnt(p,-1,q,-1,0),title,stl,size);
	if(box)	//	draw boungind box
	{
		mreal c1=AddTexture('w'), c2=AddTexture('k');
		if((Flag&3)==2)	{	mreal cc=c1;	c2=c1;	c1=cc;	}
		long k1,k2,k3,k4;
		k1=AddPnt(&B,mglPoint(inX,y,Depth),c1,q,-1,0);
		k2=AddPnt(&B,mglPoint(inX+inW,y,Depth),c1,q,-1,0);
		k3=AddPnt(&B,mglPoint(inX,y+h,Depth),c1,q,-1,0);
		k4=AddPnt(&B,mglPoint(inX+inW,y+h,Depth),c1,q,-1,0);
		quad_plot(k1,k2,k3,k4);
		k1=CopyNtoC(k1,c2);	k2=CopyNtoC(k2,c2);
		k3=CopyNtoC(k3,c2);	k4=CopyNtoC(k4,c2);
		line_plot(k1,k2);	line_plot(k2,k4);
		line_plot(k4,k3);	line_plot(k3,k1);
	}
	B1.y -= h/2;	B1.b[4] -= h;	B=B1;
	inH-=h;	font_factor = B.b[0] < B.b[4] ? B.b[0] : B.b[4];
	EndGroup();
}
//-----------------------------------------------------------------------------
void mglCanvas::StartAutoGroup (const char *lbl)
{
	static int id=1;
	if(lbl==NULL)	{	id=1;	grp_counter=0;	return;	}
	grp_counter++;
	if(grp_counter>1)	return;	// do nothing in "subgroups"
	if(ObjId<0)	{	ObjId = -id;	id++;	}
	register size_t len = Grp.size();
	if(ObjId>=0 && len>0 && ObjId!=Grp[len-1].Id)
#pragma omp critical(grp)
	{	MGL_PUSH(Grp,mglGroup(lbl,ObjId),mutexGrp);}
	else if(ObjId<0)
#pragma omp critical(grp)
	{	MGL_PUSH(Grp,mglGroup(lbl,ObjId),mutexGrp);}
}
//-----------------------------------------------------------------------------
void mglCanvas::EndGroup()
{
	LoadState();
	if(Quality&MGL_DRAW_LMEM)
	{
		Pnt.clear();		Prm.clear();		Ptx.clear();
		Glf.clear();		Act.clear(); 	Grp.clear();
	}
	if(grp_counter>0)	grp_counter--;
}
//-----------------------------------------------------------------------------
int mglCanvas::IsActive(int xs, int ys,int &n)
{
	long i, h = (Width>Height ? Height:Width)/100;
	for(i=0;i<(long)Act.size();i++)
	{
		const mglActivePos &p=Act[i];
		if(abs(xs-p.x)<=h && abs(ys-p.y)<=h)
		{	n=p.n;	return p.id;		}
	}
	n=-1;	return GetObjId(xs,ys);
}
//-----------------------------------------------------------------------------
void mglCanvas::Push()
{
#pragma omp critical(stk)
	{MGL_PUSH(stack,B,mutexStk);}
}
//-----------------------------------------------------------------------------
