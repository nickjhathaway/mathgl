/* mgl_exec.cpp is part of Math Graphic Library
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
#include <ctype.h>
#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#include <unistd.h>
#else
#include <direct.h>
#endif
//#include <unistd.h>
#include "mgl/mgl_parse.h"
char *mgl_strdup(const char *s);
wchar_t *mgl_str_copy(const char *s);
//-----------------------------------------------------------------------------
//	{"addlegend","Add legend entry","addlegend txt fmt", mgls_addlegend, mglc_addlegend}
int mgls_addlegend(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==2 && k[1]==2)	gr->AddLegend(a[0].w,a[1].s);
	else	return 1;
	return 0;
}
void mglc_addlegend(wchar_t out[1024], long n, mglArg *a, int k[10])
{	if(k[0]==2 && k[1]==2)	swprintf(out,1024,L"gr->AddLegend(\"%s\", \"%s\");",a[0].s,a[1].s);	}
//-----------------------------------------------------------------------------
//	{"addto","Add data or number","addto var|num", mgls_addto, mglc_addto}
int mgls_addto(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)		*(a[0].d) += *(a[1].d);
	else if(k[0]==1 && k[1]==3)	*(a[0].d) += a[1].v;
	else	return 1;
	return 0;
}
void mglc_addto(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)		swprintf(out,1024,L"%s += %s;",a[0].s, a[1].s);
	else if(k[0]==1 && k[1]==3)	swprintf(out,1024,L"%s += %g;",a[0].s, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"alpha","Switch on/off transparency","addto [num]", mgls_alpha, mglc_alpha}
int mgls_alpha(mglGraph *gr, long n, mglArg *a, int k[10])
{
	gr->Alpha(k[0]==3 ? a[0].v!=0 : true);	return 0;
}
void mglc_alpha(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	swprintf(out,1024,L"gr->Alpha(%s);",(k[0]==3&&a[0].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"alphadef","Set default transparency","alphadef num", mgls_alphadef, mglc_alphadef}
int mgls_alphadef(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)	gr->AlphaDef = a[0].v;
	else	return 1;
	return 0;
}
void mglc_alphadef(wchar_t out[1024], long n, mglArg *a, int k[10])
{	if(k[0]==3)	swprintf(out,1024,L"gr->AlphaDef = %g;",a[0].v);	}
//-----------------------------------------------------------------------------
//	{"ambient","Set ambient light brightness","ambient txt fmt", mgls_ambient, mglc_ambient}
int mgls_ambient(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)	gr->Ambient(a[0].v);
	else	return 1;
	return 0;
}
void mglc_ambient(wchar_t out[1024], long n, mglArg *a, int k[10])
{	if(k[0]==3)	swprintf(out,1024,L"gr->Ambient(%g);",a[0].v);	}
//-----------------------------------------------------------------------------
//	{"area","Draw area plot for 1D data","area {xvar} yvar {{zvar}} [fmt num]", mgls_area, mglc_area}
int mgls_area(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Area(*(a[0].d),k[1]==2?a[1].s:0,false,k[2]==3 ? a[2].v : NAN);
	else if(k[2]!=1)	gr->Area(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,false,k[3]==3 ? a[3].v : NAN);
	else 				gr->Area(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	return 0;
}
void mglc_area(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Area(%s, \"%s\", false, %g);",a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
	else if(k[2]!=1)
		swprintf(out,1024,L"gr->Area(%s, %s, \"%s\", false, %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else 	swprintf(out,1024,L"gr->Area(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"aspect","Set aspect ration","aspect numx numy numz", mgls_aspect, mglc_aspect}
int mgls_aspect(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)	gr->Aspect(a[0].v,a[1].v,a[2].v);
	else	return 1;
	return 0;
}
void mglc_aspect(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		swprintf(out,1024,L"gr->Aspect(%g, %g, %g);",a[0].v,a[1].v,a[2].v);
}
//-----------------------------------------------------------------------------
//	{"axial","Draw surfaces of contour lines rotation","axial {xvar yvar} zvar [fmt num]", mgls_axial, mglc_axial}
int mgls_axial(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Axial(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3 ? int(a[2].v) : 3);
	else if(k[1]==1 && k[2]==1)
		gr->Axial(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3 ? int(a[4].v) : 3);
	else	return 1;
	return 0;
}
void mglc_axial(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Axial(%s, \"%s\", %d);", a[0].s,k[1]==2?a[1].s:"",k[2]==3 ? int(a[2].v) : 3);
	else if(k[1]==1 && k[2]==1)
		swprintf(out,1024,L"gr->Axial(%s, %s, %s, \"%s\", %d);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?int(a[4].v):3);
}
//-----------------------------------------------------------------------------
//	{"axialdir","Set axis of rotation","axialdir dir", mgls_axialdir, mglc_axialdir}
int mgls_axialdir(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==2)	gr->AxialDir = a[0].s[0];	else	return 1;
	return 0;
}
void mglc_axialdir(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==2)	swprintf(out,1024,L"gr->AxialDir = '%c';", a[0].s[0]);
}
//-----------------------------------------------------------------------------
//	{"axis","Setup or draw axis",0, mgls_axis, mglc_axis}
int mgls_axis(mglGraph *gr, long n, mglArg *a, int k[10])
{
	register int i;
	if(k[0]==2 && k[1]==2 && k[2]==2)	gr->Axis(a[0].s,a[1].s,a[2].s);
	else if(n==6)
	{
		bool ok=true;
		for(i=0;i<6;i++)	if(k[i]!=3)	ok = false;
		if(ok)	gr->Axis(mglPoint(a[0].v,a[1].v,a[2].v), mglPoint(a[3].v,a[4].v,a[5].v),gr->Org);
		else return 1;
	}
	else if(n==4)
	{
		bool ok=true;
		for(i=0;i<4;i++)	if(k[i]!=3)	ok = false;
		if(ok)	gr->Axis(mglPoint(a[0].v,a[1].v), mglPoint(a[2].v,a[3].v),gr->Org);
		else return 1;
	}
	else if(k[0]==2)	gr->Axis(a[0].s);
	else if(k[0]==0)	gr->Axis("xyz");
	else return 1;
	return 0;
}
void mglc_axis(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	register int i;
	if(k[0]==2 && k[1]==2 && k[2]==2)
		swprintf(out,1024,L"gr->Axis(\"%s\", \"%s\", \"%s\");", a[0].s,a[1].s,a[2].s);
	else if(n==6)
	{
		bool ok=true;
		for(i=0;i<6;i++)	if(k[i]!=3)	ok = false;
		if(ok)	swprintf(out,1024,L"gr->Axis(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), gr->Org);", a[0].v,a[1].v,a[2].v,a[3].v,a[4].v,a[5].v);
	}
	else if(n==4)
	{
		bool ok=true;
		for(i=0;i<4;i++)	if(k[i]!=3)	ok = false;
		if(ok)	swprintf(out,1024,L"gr->Axis(mglPoint(%g, %g), mglPoint(%g, %g), gr->Org);", a[0].v,a[1].v,a[2].v,a[3].v);
	}
	else if(k[0]==2)	swprintf(out,1024,L"gr->Axis(\"%s\");", a[0].s);
	else if(k[0]==0)	swprintf(out,1024,L"gr->Axis(\"xyz\");");
}
//-----------------------------------------------------------------------------
//	{"ball","Draw point (ball)","ball posx posy {posz} [fmt]", mgls_ball, mglc_ball}
int mgls_ball(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		gr->Ball(mglPoint(a[0].v,a[1].v,a[2].v),k[3]==2 ? a[3].s[0]:'r');
	else if(k[0]==3 && k[1]==3)
		gr->Ball(mglPoint(a[0].v,a[1].v),k[2]==2 ? a[2].s[0]:'r');
	else	return 1;
	return 0;
}
void mglc_ball(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		swprintf(out,1024,L"gr->Ball(mglPoint(%g, %g, %g), '%c');", a[0].v,a[1].v,a[2].v,k[3]==2 ? a[3].s[0]:'r');
	else if(k[0]==3 && k[1]==3)
		swprintf(out,1024,L"gr->Ball(mglPoint(%g, %g), '%c');", a[0].v, a[1].v, k[2]==2 ? a[2].s[0]:'r');
}
//-----------------------------------------------------------------------------
//	{"box","Draw bounding box","box [fmt]", mgls_box, mglc_box}
int mgls_box(mglGraph *gr, long n, mglArg *a, int k[10])
{
	gr->Box(k[0]==2 ? a[0].s : (gr->TranspType!=2 ?"k-":"w-"));
	return 0;
}
void mglc_box(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	swprintf(out,1024,L"gr->Box(\"%s\");", k[0]==2 ? a[0].s : "");
}
//-----------------------------------------------------------------------------
//	{"bars","Draw bars for 1D data","bars {xvar} yvar {{zvar}} [fmt num num]", mgls_bars, mglc_bars}
int mgls_bars(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Bars(*(a[0].d), k[1]==2?a[1].s:0, k[2]==3?a[2].v:NAN, k[3]==3?(a[3].v!=0):false);
	else if(k[2]!=1)	gr->Bars(*(a[0].d), *(a[1].d), k[2]==2?a[2].s:0, k[3]==3?a[3].v:NAN, k[4]==3?(a[4].v!=0):false);
	else 				gr->Bars(*(a[0].d), *(a[1].d), *(a[2].d), k[3]==2?a[3].s:0, k[4]==3?(a[4].v!=0):false);
	return 0;
}
void mglc_bars(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)	
		swprintf(out,1024,L"gr->Bars(%s, \"%s\", %g, %s);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, (k[3]==3&&a[3].v!=0)?"true":"false");
	else if(k[2]!=1)
		swprintf(out,1024,L"gr->Bars(%s, %s, \"%s\", %g, %s);",a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN, (k[4]==3&&a[4].v!=0)?"true":"false");
	else
		swprintf(out,1024,L"gr->Bars(%s, %s, %s, \"%s\", %s);",a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", (k[4]==3&&a[4].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"belt","Draw belts","belt {xvar yvar} zvar [fmt]", mgls_belt, mglc_belt}
int mgls_belt(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)			gr->Belt(*(a[0].d),k[1]==2?a[1].s:0);
	else if(k[1]==1 && k[2]==1)	gr->Belt(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else	return 1;
	return 0;
}
void mglc_belt(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Belt(%s, \"%s\");", a[0].s,k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]==1)
		swprintf(out,1024,L"gr->Belt(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"boxs","Draw boxes","boxs {xvar yvar} zvar [fmt]", mgls_boxs, mglc_boxs}
int mgls_boxs(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)			gr->Boxs(*(a[0].d),k[1]==2?a[1].s:0);
	else if(k[1]==1 && k[2]==1)	gr->Boxs(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else	return 1;
	return 0;
}
void mglc_boxs(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Boxs(%s, \"%s\");", a[0].s,k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]==1)
		swprintf(out,1024,L"gr->Boxs(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"beam","Draw quasioptical beam",0, mgls_beam, mglc_beam}
int mgls_beam(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
		gr->Beam(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].v,
				k[5]==2? a[5].s:0,k[6]==3?int(a[6].v):0, int(k[7]==3?a[7].v:3));
	else	return 1;
	return 0;
}
void mglc_beam(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
		swprintf(out,1024,L"gr->Beam(%s, %s, %s, %s, %g, \"%s\", %d, %d);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].v, k[5]==2? a[5].s:"",k[6]==3?int(a[6].v):0, int(k[7]==3?a[7].v:3));
}
//-----------------------------------------------------------------------------
//	{"caxis","Set color range","caxis c1 c2", mgls_caxis, mglc_caxis}
int mgls_caxis(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3)	gr->CAxis(a[0].v,a[1].v);
	else	return 1;
	return 0;
}
void mglc_caxis(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3)	swprintf(out,1024,L"gr->CAxis(%g, %g);", a[0].v,a[1].v);
}
//-----------------------------------------------------------------------------
//	{"clearlegend","Clear legend antries","clearlegend", mgls_clearlegend, mglc_clearlegend}
int mgls_clearlegend(mglGraph *gr, long n, mglArg *a, int k[10])
{	gr->ClearLegend();	return 0;	}
void mglc_clearlegend(wchar_t out[1024], long n, mglArg *a, int k[10])
{	swprintf(out,1024,L"gr->ClearLegend();");	}
//-----------------------------------------------------------------------------
//	{"clf","Clear picture","clf", mgls_clf, mglc_clf}
int mgls_clf(mglGraph *gr, long n, mglArg *a, int k[10])
{	gr->Clf();	return 0;	}
void mglc_clf(wchar_t out[1024], long n, mglArg *a, int k[10])
{	swprintf(out,1024,L"gr->Clf();");	}
//-----------------------------------------------------------------------------
//	{"chart","Draw chart","chart var [fmt]", mgls_chart, mglc_chart}
int mgls_chart(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->Chart(*(a[0].d), k[1]==2?a[1].s:0);
	else	return 1;
	return 0;
}
void mglc_chart(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3)	swprintf(out,1024,L"gr->Chart(%s, \"%s\");", a[0].s, k[1]==2?a[1].s:"");
}
//-----------------------------------------------------------------------------
//	{"cloud","Draw cloud","cloud {xvar yvar zvar} avar [fmt]", mgls_cloud, mglc_cloud}
int mgls_cloud(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->CloudQ(*(a[0].d),k[1]==2?a[1].s:0);
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->CloudQ(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	return 0;
}
void mglc_cloud(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->CloudQ(%s,\"%s\");",a[0].s, k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->CloudQ(%s, %s, %s, %s, \"%s\");", a[0].s,a[1].s,a[2].s,a[3].s,k[4]==2?a[4].s:"");
}
//-----------------------------------------------------------------------------
//	{"crange","Set color range","crange {var [sym] | c1 c2}", mgls_crange, mglc_crange}
int mgls_crange(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->CRange(*(a[0].d),k[1]==3?(a[1].v!=0):false);
	else if(k[0]==3 && k[1]==3)	{	gr->Cmin = a[0].v;	gr->Cmax = a[1].v;	}
	else	return 1;
	return 0;
}
void mglc_crange(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)	swprintf(out,1024,L"gr->CRange(%s, %s);",a[0].s, (k[1]==3 && a[1].v!=0)?"true":"false");
	else if(k[0]==3 && k[1]==3)	swprintf(out,1024,L"gr->Cmin = %g;\tgr->Cmax = %g;",a[0].v,a[1].v);
}
//-----------------------------------------------------------------------------
//	{"crop","Crop edge of data","crop var n1 n2 dir", mgls_crop, mglc_crop}
int mgls_crop(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3 && k[2]==3 && k[3]==2)
		a[0].d->Crop(int(a[1].v),int(a[2].v),a[3].s[0]);
	else	return 1;
	return 0;
}
void mglc_crop(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3 && k[2]==3 && k[3]==2)
		swprintf(out,1024,L"%s.Crop(%d, %d, '%c');",a[0].s, int(a[1].v), int(a[2].v), a[3].s[0]);
}
//-----------------------------------------------------------------------------
//	{"cumsum","Crop edge of data","cumsum var dir", mgls_cumsum, mglc_cumsum}
int mgls_cumsum(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	a[0].d->CumSum(a[1].s);
	else	return 1;
	return 0;
}
void mglc_cumsum(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	swprintf(out,1024,L"%s.CumSum(\"%s\");",a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"curve","Draw curve",0, mgls_curve, mglc_curve}
int mgls_curve(mglGraph *gr, long n, mglArg *a, int k[10])
{
	register long i;
	for(i=0;i<12;i++)	if(a[i].type!=2 || i>=n)	break;
	if(i==12)
		gr->Curve(mglPoint(a[0].v,a[1].v,a[2].v),
			mglPoint(a[3].v,a[4].v,a[5].v),
			mglPoint(a[6].v,a[7].v,a[8].v),
			mglPoint(a[9].v,a[10].v,a[11].v),
			(n>12 && a[12].type==1) ? a[12].s : 0);
	else if(i==8)
		gr->Curve(mglPoint(a[0].v,a[1].v),
			mglPoint(a[2].v,a[3].v),
			mglPoint(a[4].v,a[5].v),
			mglPoint(a[6].v,a[7].v),
			(n>8 && a[8].type==1) ? a[8].s : 0);
	return 0;
}
void mglc_curve(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	register long i;
	for(i=0;i<12;i++)	if(a[i].type!=2 || i>=n)	break;
	if(i==12)
		swprintf(out,1024,L"gr->Curve(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), \"%s\");", a[0].v, a[1].v, a[2].v, a[3].v, a[4].v, a[5].v, a[6].v, a[7].v, a[8].v, a[9].v, a[10].v, a[11].v, (n>12 && a[12].type==1) ? a[12].s : "");
	else if(i==8)
		swprintf(out,1024,L"gr->Curve(mglPoint(%g, %g), mglPoint(%g, %g), mglPoint(%g, %g), mglPoint(%g, %g), \"%s\");", a[0].v, a[1].v, a[2].v, a[3].v, a[4].v, a[5].v, a[6].v, a[7].v, (n>8 && a[8].type==1) ? a[8].s : "");
}
//-----------------------------------------------------------------------------
//	{"cut","Setup plot points cutting",0, mgls_cut, mglc_cut}
int mgls_cut(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && n == 1)	gr->Cut = (a[0].v != 0);
	else if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3 && k[4]==3 && k[5]==3)
	{
		gr->CutMin = mglPoint(a[0].v,a[1].v,a[2].v);
		gr->CutMax = mglPoint(a[3].v,a[4].v,a[5].v);
	}
	else if(k[0]==2 && n == 1)	gr->CutOff(a[0].s);
	else	return 1;
	return 0;
}
void mglc_cut(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && n == 1)	swprintf(out,1024,L"gr->Cut = %s;",(a[0].v != 0)?"true":"false");
	else if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3 && k[4]==3 && k[5]==3)
		swprintf(out,1024,L"gr->CutMin = mglPoint(%g, %g, %g);\tgr->CutMax = mglPoint(%g, %g, %g);", a[0].v, a[1].v, a[2].v, a[3].v, a[4].v, a[5].v);
	else if(k[0]==2 && n == 1)
		swprintf(out,1024,L"gr->CutOff(\"%s\");", a[0].s);
}
//-----------------------------------------------------------------------------
//	{"crust","Draw reconstructed surface for arbitrary data points","crust {xvar yvar zvar} | var [fmt]", mgls_crust, mglc_crust}
int mgls_crust(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1)
		gr->Crust(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else if(k[0]==1)
		gr->Crust(*(a[0].d),k[1]==2?a[1].s:0);
	else	return 1;
	return 0;
}
void mglc_crust(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1)
		swprintf(out,1024,L"gr->Crust(%s, %s, %s, \"%s\");",a[0].s,a[1].s,a[2].s,k[3]==2?a[3].s:"");
	else if(k[0]==1)
		swprintf(out,1024,L"gr->Crust(%s, \"%s\");",a[0].s, k[1]==2?a[1].s:"");
}
//-----------------------------------------------------------------------------
//	{"colorbar","Draw colorbar","colorbar [fmt pos]", mgls_colorbar, mglc_colorbar}
int mgls_colorbar(mglGraph *gr, long n, mglArg *a, int k[10])
{
	gr->Colorbar(k[0]==2 ? a[0].s:0, k[1]==3 ? int(a[1].v):0);
	return 0;
}
void mglc_colorbar(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	swprintf(out,1024,L"gr->Colorbar(\"%s\", %d);",k[0]==2 ? a[0].s:"", k[1]==3 ? int(a[1].v):0);
}
//-----------------------------------------------------------------------------
//	{"copy","Copy data from another variable","copy var1 var2", mgls_copy, mglc_copy}
int mgls_copy(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)	a[0].d->Set(*(a[1].d));
	return 0;
}
void mglc_copy(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)	swprintf(out,1024,L"%s.Set(%s);",a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"cont","Draw contour lines","cont {vvar} {xvar yvar} zvar [fmt num zpos]", mgls_cont, mglc_cont}
int mgls_cont(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Cont(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?int(a[2].v):7,k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]!=1)
		gr->Cont(*(a[0].d), *(a[1].d), k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		gr->Cont(*(a[0].d), *(a[1].d), *(a[2].d), k[3]==2?a[3].s:0,
			k[4]==3?int(a[4].v):7, k[5]==3?a[5].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]!=1)
		gr->Cont(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), k[4]==2?a[4].s:0,
			k[5]==3?a[5].v:NAN);
	else	return 1;
	return 0;
}
void mglc_cont(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Cont(%s, \"%s\", %d, %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?int(a[2].v):7, k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]!=1)
		swprintf(out,1024,L"gr->Cont(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		swprintf(out,1024,L"gr->Cont(%s, %s, %s, \"%s\", %d, %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?int(a[4].v):7, k[5]==3?a[5].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]!=1)
		swprintf(out,1024,L"gr->Cont(%s, %s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"contf","Draw solid contour lines","contf {vvar} {xvar yvar} zvar [fmt num zpos]", mgls_contf, mglc_contf}
int mgls_contf(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->ContF(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?int(a[2].v):7,k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]!=1)
		gr->ContF(*(a[0].d), *(a[1].d), k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		gr->ContF(*(a[0].d), *(a[1].d), *(a[2].d), k[3]==2?a[3].s:0,
			k[4]==3?int(a[4].v):7, k[5]==3?a[5].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]!=1)
		gr->ContF(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), k[4]==2?a[4].s:0,
			k[5]==3?a[5].v:NAN);
	else	return 1;
	return 0;
}
void mglc_contf(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->ContF(%s, \"%s\", %d, %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?int(a[2].v):7, k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]!=1)
		swprintf(out,1024,L"gr->ContF(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		swprintf(out,1024,L"gr->ContF(%s, %s, %s, \"%s\", %d, %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?int(a[4].v):7, k[5]==3?a[5].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]!=1)
		swprintf(out,1024,L"gr->ContF(%s, %s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"cont3","Draw contour lines for 3D data","cont3 {xvar yvar zvar} avar dir [pos fmt num]", mgls_cont3, mglc_cont3}
int mgls_cont3(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==2)
		gr->Cont3(*(a[0].d), a[1].s[0], k[2]==3?int(a[2].v):-1, k[3]==2?a[3].s:0, k[4]==3?int(a[4].v):7);
	else if(k[1]==1 && k[2]==2)
		gr->Cont3(*(a[0].d), *(a[1].d), a[2].s[0], k[3]==3?int(a[3].v):-1, k[4]==2?a[4].s:0);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		gr->Cont3(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), a[4].s[0], k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0, k[7]==3?int(a[7].v):7);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==1 && k[5]==2)
		gr->Cont3(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), *(a[4].d), a[5].s[0], k[6]==3?int(a[6].v):-1, k[7]==2?a[7].s:0);
	else	return 1;
	return 0;
}
void mglc_cont3(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]==2)
		swprintf(out,1024,L"gr->Cont3(%s, '%c', %d, \"%s\", %d);", a[0].s, a[1].s[0], k[2]==3?int(a[2].v):-1, k[3]==2?a[3].s:"", k[4]==3?int(a[4].v):7);
	else if(k[1]==1 && k[2]==2)
		swprintf(out,1024,L"gr->Cont3(%s, %s, '%c', %d, \"%s\");", a[0].s, a[1].s, a[2].s[0], k[3]==3?int(a[3].v):-1, k[4]==2?a[4].s:"");
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		swprintf(out,1024,L"gr->Cont3(%s, %s, %s, %s, '%c', %d, \"%s\", %d);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s[0], k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0, k[7]==3?int(a[7].v):7);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==1 && k[5]==2)
		swprintf(out,1024,L"gr->Cont3(%s, %s, %s, %s, %s, '%c', %d, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s[0], k[6]==3?int(a[6].v):-1, k[7]==2?a[7].s:0);
}
//-----------------------------------------------------------------------------
//	{"contf3","Draw solid contour lines for 3D data","contf3 {xvar yvar zvar} avar dir [pos fmt num]", mgls_contf3, mglc_contf3}
int mgls_contf3(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==2)
		gr->ContF3(*(a[0].d), a[1].s[0], k[2]==3?int(a[2].v):-1, k[3]==2?a[3].s:0, k[4]==3?int(a[4].v):7);
	else if(k[1]==1 && k[2]==2)
		gr->ContF3(*(a[0].d), *(a[1].d), a[2].s[0], k[3]==3?int(a[3].v):-1, k[4]==2?a[4].s:0);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		gr->ContF3(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), a[4].s[0], k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0, k[7]==3?int(a[7].v):7);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==1 && k[5]==2)
		gr->ContF3(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), *(a[4].d), a[5].s[0], k[6]==3?int(a[6].v):-1, k[7]==2?a[7].s:0);
	else	return 1;
	return 0;
}
void mglc_contf3(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]==2)
		swprintf(out,1024,L"gr->ContF3(%s, '%c', %d, \"%s\", %d);", a[0].s, a[1].s[0], k[2]==3?int(a[2].v):-1, k[3]==2?a[3].s:"", k[4]==3?int(a[4].v):7);
	else if(k[1]==1 && k[2]==2)
		swprintf(out,1024,L"gr->ContF3(%s, %s, '%c', %d, \"%s\");", a[0].s, a[1].s, a[2].s[0], k[3]==3?int(a[3].v):-1, k[4]==2?a[4].s:"");
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		swprintf(out,1024,L"gr->ContF3(%s, %s, %s, %s, '%c', %d, \"%s\", %d);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s[0], k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0, k[7]==3?int(a[7].v):7);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==1 && k[5]==2)
		swprintf(out,1024,L"gr->ContF3(%s, %s, %s, %s, %s, '%c', %d, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s[0], k[6]==3?int(a[6].v):-1, k[7]==2?a[7].s:0);
}
//-----------------------------------------------------------------------------
//	{"conta","Draw contour lines at central slices of 3D data","conta {xvar yvar zvar} avar [fmt num]", mgls_conta, mglc_conta}
int mgls_conta(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->ContA(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), k[4]==2?a[4].s:0, k[5]==3?int(a[5].v):7);
	else	gr->ContA(*(a[0].d), k[1]==2?a[1].s:0, k[2]==3?int(a[2].v):7);
	return 0;
}
void mglc_conta(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->ContA(%s, %s, %s, %s, \"%s\", %d);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?int(a[5].v):7);
	else
		swprintf(out,1024,L"gr->ContA(%s, \"%s\", %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?int(a[2].v):7);
}
//-----------------------------------------------------------------------------
//	{"contfa","Draw solid contour lines at central slices of 3D data","contfa {xvar yvar zvar} avar [fmt num]", mgls_contfa, mglc_contfa}
int mgls_contfa(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->ContFA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,
				k[5]==3?int(a[5].v):7);
	else	gr->ContFA(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?int(a[2].v):7);
	return 0;
}
void mglc_contfa(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->ContFA(%s, %s, %s, %s, \"%s\", %d);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?int(a[5].v):7);
	else
		swprintf(out,1024,L"gr->ContFA(%s, \"%s\", %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?int(a[2].v):7);
}
//-----------------------------------------------------------------------------
//	{"contx","Draw contour lines at x-slice (or x-plane)","contx avar [fmt pos num]", mgls_contx, mglc_contx}
int mgls_contx(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->ContX(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
	else	return 1;
	return 0;
}
void mglc_contx(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)	swprintf(out,1024,L"gr->ContX(%s, \"%s\", %g, %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, k[3]==3?int(a[3].v):7);
}
//-----------------------------------------------------------------------------
//	{"contfx","Draw solid contour lines at x-slice (or x-plane)","contfx avar [fmt pos num]", mgls_contfx, mglc_contfx}
int mgls_contfx(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->ContFX(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
	else	return 1;
	return 0;
}
void mglc_contfx(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)	swprintf(out,1024,L"gr->ContFX(%s, \"%s\", %g, %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, k[3]==3?int(a[3].v):7);
}
//-----------------------------------------------------------------------------
//	{"conty","Draw contour lines at y-slice (or y-plane)","conty avar [fmt pos num]", mgls_conty, mglc_conty}
int mgls_conty(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->ContY(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
	else	return 1;
	return 0;
}
void mglc_conty(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)	swprintf(out,1024,L"gr->ContY(%s, \"%s\", %g, %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, k[3]==3?int(a[3].v):7);
}
//-----------------------------------------------------------------------------
//	{"contfy","Draw solid contour lines at y-slice (or y-plane)","contfy avar [fmt pos num]", mgls_contfy, mglc_contfy}
int mgls_contfy(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->ContFY(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
	else	return 1;
	return 0;
}
void mglc_contfy(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)	swprintf(out,1024,L"gr->ContFY(%s, \"%s\", %g, %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, k[3]==3?int(a[3].v):7);
}
//-----------------------------------------------------------------------------
//	{"contz","Draw contour lines at z-slice (or z-plane)","contz avar [fmt pos num]", mgls_contz, mglc_contz}
int mgls_contz(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->ContZ(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
	else	return 1;
	return 0;
}
void mglc_contz(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)	swprintf(out,1024,L"gr->ContZ(%s, \"%s\", %g, %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, k[3]==3?int(a[3].v):7);
}
//-----------------------------------------------------------------------------
//	{"contfy","Draw solid contour lines at z-slice (or z-plane)","contfz avar [fmt pos num]", mgls_contfz, mglc_contfz}
int mgls_contfz(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->ContFZ(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
	else	return 1;
	return 0;
}
void mglc_contfz(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)	swprintf(out,1024,L"gr->ContFZ(%s, \"%s\", %g, %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, k[3]==3?int(a[3].v):7);
}
//-----------------------------------------------------------------------------
//	{"cone","Draw cone","cone avar [fmt pos num]", mgls_cone, mglc_cone}
int mgls_cone(mglGraph *gr, long n, mglArg *a, int k[10])
{
	bool ok=true;
	for(int i=0;i<7;i++)	if(a[i].type!=2)	ok=false;
	if(ok)
	{
		if(a[7].type==2)
			gr->Cone(mglPoint(a[0].v,a[1].v,a[2].v), mglPoint(a[3].v,a[4].v,a[5].v),
				a[6].v, a[7].v, (a[8].type==1) ? a[8].s : 0, (a[9].type==2) ? a[9].v!=0 : false);
		else
			gr->Cone(mglPoint(a[0].v,a[1].v,a[2].v), mglPoint(a[3].v,a[4].v,a[5].v),
				a[6].v, -1, (a[7].type==1) ? a[7].s : 0, (a[8].type==2) ? a[8].v!=0 : false);
	}
	else	return 1;
	return 0;
}
void mglc_cone(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	bool ok=true;
	for(int i=0;i<7;i++)	if(a[i].type!=2)	ok=false;
	if(ok)
	{
		if(a[7].type==2)
			swprintf(out,1024,L"gr->Cone(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), %g, %g, \"%s\", %s);", a[0].v,a[1].v,a[2].v, a[3].v,a[4].v,a[5].v, a[6].v, a[7].v, (a[8].type==1) ? a[8].s : "", (a[9].type==2 && a[9].v!=0) ? "true" : "false");
		else
			swprintf(out,1024,L"gr->Cone(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), %g, -1, \"%s\", %s);", a[0].v,a[1].v,a[2].v, a[3].v,a[4].v,a[5].v, a[6].v, (a[7].type==1) ? a[7].s : "", (a[8].type==2 && a[8].v!=0)? "true" : "false");
	}
}
//-----------------------------------------------------------------------------
//	{"dens","Draw density plot","dens {xvar yvar} zvar [fmt zpos]", mgls_dens, mglc_dens}
int mgls_dens(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Dens(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		gr->Dens(*(a[0].d), *(a[1].d), *(a[2].d), k[3]==2?a[3].s:0, k[4]==3?a[4].v:NAN);
	else	return 1;
	return 0;
}
void mglc_dens(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Dens(%s,\"%s\", %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		swprintf(out,1024,L"gr->Dens(%s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"dens3","Draw density plot at slices of 3D data","dens3 {xvar yvar zvar} avar dir [pos fmt]", mgls_dens3, mglc_dens3}
int mgls_dens3(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==2)
		gr->Dens3(*(a[0].d),a[1].s[0],k[2]==3?int(a[2].v):-1,
				k[3]==2?a[3].s:0);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		gr->Dens3(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].s[0],
				k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0);
	else	return 1;
	return 0;
}
void mglc_dens3(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]==2)
		swprintf(out,1024,L"gr->Dens3(%s, '%c', %d, \"%s\");", a[0].s, a[1].s[0], k[2]==3?int(a[2].v):-1, k[3]==2?a[3].s:"");
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		swprintf(out,1024,L"gr->Dens3(%s, %s, %s, %s, '%c', %d, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s[0], k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0);
}
//-----------------------------------------------------------------------------
//	{"densa","Draw density plot at central slices of 3D data","densa {xvar yvar zvar} avar [fmt num]", mgls_densa, mglc_densa}
int mgls_densa(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->DensA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	else	gr->DensA(*(a[0].d),k[1]==2?a[1].s:0);
	return 0;
}
void mglc_densa(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->DensA(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
	else
		swprintf(out,1024,L"gr->DensA(%s, \"%s\");", a[0].s, k[1]==2?a[1].s:"");
}
//-----------------------------------------------------------------------------
//	{"densx","Draw density plot at x-slice (or x-plane)","densx avar [fmt pos]", mgls_densx, mglc_densx}
int mgls_densx(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->DensX(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN);
	else	return 1;
	return 0;
}
void mglc_densx(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)	swprintf(out,1024,L"gr->DensX(%s, \"%s\", %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"densy","Draw density plot at y-slice (or y-plane)","densy avar [fmt pos]", mgls_densy, mglc_densy}
int mgls_densy(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->DensY(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN);
	else	return 1;
	return 0;
}
void mglc_densy(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)	swprintf(out,1024,L"gr->DensY(%s, \"%s\", %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"densz","Draw density plot at z-slice (or z-plane)","densz avar [fmt pos]", mgls_densz, mglc_densz}
int mgls_densz(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->DensZ(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN);
	else	return 1;
	return 0;
}
void mglc_densz(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)	swprintf(out,1024,L"gr->DensZ(%s, \"%s\", %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"divto","Divide by data or number","divto var|num", mgls_divto, mglc_divto}
int mgls_divto(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)		*(a[0].d) /= *(a[1].d);
	else if(k[0]==1 && k[1]==3)	*(a[0].d) /= a[1].v;
	else	return 1;
	return 0;
}
void mglc_divto(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)		swprintf(out,1024,L"%s /= %s;",a[0].s, a[1].s);
	else if(k[0]==1 && k[1]==3)	swprintf(out,1024,L"%s /= %g;",a[0].s, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"multo","Multiply by data or number","multo var|num", mgls_multo, mglc_multo}
int mgls_multo(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)		*(a[0].d) *= *(a[1].d);
	else if(k[0]==1 && k[1]==3)	*(a[0].d) *= a[1].v;
	else	return 1;
	return 0;
}
void mglc_multo(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)		swprintf(out,1024,L"%s *= %s;",a[0].s, a[1].s);
	else if(k[0]==1 && k[1]==3)	swprintf(out,1024,L"%s *= %g;",a[0].s, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"subto","Subtract data or number","subto var|num", mgls_subto, mglc_subto}
int mgls_subto(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)		*(a[0].d) -= *(a[1].d);
	else if(k[0]==1 && k[1]==3)	*(a[0].d) -= a[1].v;
	else	return 1;
	return 0;
}
void mglc_subto(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)		swprintf(out,1024,L"%s -= %s;",a[0].s, a[1].s);
	else if(k[0]==1 && k[1]==3)	swprintf(out,1024,L"%s -= %g;",a[0].s, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"dots","Draw dots for arbitrary data points","dots {xvar yvar zvar} | var [fmt]", mgls_dots, mglc_dots}
int mgls_dots(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1)
		gr->Dots(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else if(k[0]==1)
		gr->Dots(*(a[0].d),k[1]==2?a[1].s:0);
	else	return 1;
	return 0;
}
void mglc_dots(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1)
		swprintf(out,1024,L"gr->Dots(%s, %s, %s, \"%s\");",a[0].s,a[1].s,a[2].s,k[3]==2?a[3].s:"");
	else if(k[0]==1)
		swprintf(out,1024,L"gr->Dots(%s, \"%s\");",a[0].s, k[1]==2?a[1].s:"");
}
//-----------------------------------------------------------------------------
//	{"diff","Numerically differentiate data","diff var dir", mgls_diff, mglc_diff}
int mgls_diff(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	a[0].d->Diff(a[1].s);
	else	return 1;
	return 0;
}
void mglc_diff(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	swprintf(out,1024,L"%s.Diff(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"diff2","Numerically double differentiate data","diff2 var dir", mgls_diff2, mglc_diff2}
int mgls_diff2(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	a[0].d->Diff2(a[1].s);
	else	return 1;
	return 0;
}
void mglc_diff2(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	swprintf(out,1024,L"%s.Diff2(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"drop","Draw drop",0, mgls_drop, mglc_drop}
int mgls_drop(mglGraph *gr, long n, mglArg *a, int k[10])
{
	register int i;
	for(i=0;i<7;i++)	if(k[i]!=3)	break;
	if(i==7)
		gr->Drop(mglPoint(a[0].v,a[1].v,a[2].v), mglPoint(a[3].v,a[4].v,a[5].v), a[6].v,
				k[7]==2 ? a[7].s:"b", k[8]==3 ? a[8].v:1.,  (n>9 && a[9].type==2) ? a[9].v:1.);
	else if(i==5)
		gr->Drop(mglPoint(a[0].v,a[1].v), mglPoint(a[2].v,a[3].v), a[4].v,
				k[5]==2 ? a[5].s:"b", k[6]==3 ? a[6].v:1., k[7]==3 ? a[7].v:1.);
	else	return 1;
	return 0;
}
void mglc_drop(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	register int i;
	for(i=0;i<7;i++)	if(k[i]!=3)	break;
	if(i==7)
		swprintf(out,1024,L"gr->Drop(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), %g, \"%s\", %g, %g);", a[0].v,a[1].v,a[2].v, a[3].v,a[4].v,a[5].v, a[6].v, k[7]==2 ? a[7].s:"b", k[8]==3 ? a[8].v:1.,  (n>9 && a[9].type==2) ? a[9].v:1.);
	else if(i==5)
		swprintf(out,1024,L"gr->Drop(mglPoint(%g, %g), mglPoint(%g, %g), %g, \"%s\", %g, %g);", a[0].v,a[1].v, a[2].v,a[3].v, a[4].v, k[5]==2 ? a[5].s:"b", k[6]==3 ? a[6].v:1., k[7]==3 ? a[7].v:1.);
}
//-----------------------------------------------------------------------------
//	{"dew","Draw dew plot","dew {xvae yvar} uvar vvar [fmt]", mgls_dew, mglc_dew}
int mgls_dew(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		gr->Dew(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==1)	gr->Dew(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else	return 1;
	return 0;
}
void mglc_dew(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->Dew(%s, %s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==1)
		swprintf(out,1024,L"gr->Dew(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"fall","Draw waterfalls","fall {xvar yvar} zvar [fmt]", mgls_fall, mglc_fall}
int mgls_fall(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)			gr->Fall(*(a[0].d),k[1]==2?a[1].s:0);
	else if(k[1]==1 && k[2]==1)	gr->Fall(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else	return 1;
	return 0;
}
void mglc_fall(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Fall(%s, \"%s\");", a[0].s,k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]==1)
		swprintf(out,1024,L"gr->Fall(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"mesh","Draw mesh surface","mesh {xvar yvar} zvar [fmt]", mgls_mesh, mglc_mesh}
int mgls_mesh(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)			gr->Mesh(*(a[0].d),k[1]==2?a[1].s:0);
	else if(k[1]==1 && k[2]==1)	gr->Mesh(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else	return 1;
	return 0;
}
void mglc_mesh(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Mesh(%s, \"%s\");", a[0].s,k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]==1)
		swprintf(out,1024,L"gr->Mesh(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"surf","Draw solid surface","surf {xvar yvar} zvar [fmt]", mgls_surf, mglc_surf}
int mgls_surf(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)			gr->Surf(*(a[0].d),k[1]==2?a[1].s:0);
	else if(k[1]==1 && k[2]==1)	gr->Surf(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else	return 1;
	return 0;
}
void mglc_surf(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Surf(%s, \"%s\");", a[0].s,k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]==1)
		swprintf(out,1024,L"gr->Surf(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"surfc","Draw solid surface colored by other data","surfc {xvar yvar} zvar cvar [fmt]", mgls_surfc, mglc_surfc}
int mgls_surfc(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1 || k[1]!=1)	return 1;
	else if(k[2]!=1)			gr->SurfC(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0);
	else if(k[2]==1 && k[3]==1)	gr->SurfC(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	else	return 1;
	return 0;
}
void mglc_surfc(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1 || k[1]!=1)	return;
	else if(k[2]!=1)
		swprintf(out,1024,L"gr->SurfC(%s, %s, \"%s\");", a[0].s,a[1].s,k[2]==2?a[2].s:"");
	else if(k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->SurfC(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
}
//-----------------------------------------------------------------------------
//	{"surfa","Draw solid surface transpared by other data","surfa {xvar yvar} zvar cvar [fmt]", mgls_surfa, mglc_surfa}
int mgls_surfa(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1 || k[1]!=1)	return 1;
	else if(k[2]!=1)			gr->SurfA(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0);
	else if(k[2]==1 && k[3]==1)	gr->SurfA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	else	return 1;
	return 0;
}
void mglc_surfa(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1 || k[1]!=1)	return;
	else if(k[2]!=1)
		swprintf(out,1024,L"gr->SurfA(%s, %s, \"%s\");", a[0].s,a[1].s,k[2]==2?a[2].s:"");
	else if(k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->SurfA(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
}
//-----------------------------------------------------------------------------
//	{"flow","Draw flow threads for vector field",0, mgls_flow, mglc_flow}
int mgls_flow(mglGraph *gr, long n, mglArg *a, int k[10])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		gr->Flow(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?int(a[3].v):5,
			true,k[4]==3?a[4].v:NAN);
	else if(i==4)
		gr->Flow(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,
			k[5]==3?int(a[5].v):5,true,k[6]==3?a[6].v:NAN);
	else if(i==3)
		gr->Flow(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?int(a[4].v):3);
	else if(i==6)
		gr->Flow(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),*(a[5].d),
			k[6]==2?a[6].s:0,k[7]==3?int(a[7].v):3);
	else	return 1;
	return 0;
}
void mglc_flow(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		swprintf(out,1024,L"gr->Flow(%s, %s, \"%s\", %d, true, %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?int(a[3].v):5, k[4]==3?a[4].v:NAN);
	else if(i==4)
		swprintf(out,1024,L"gr->Flow(%s, %s, %s, %s, \"%s\", %d, true, %g);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?int(a[5].v):5, k[6]==3?a[6].v:NAN);
	else if(i==3)
		swprintf(out,1024,L"gr->Flow(%s, %s, %s, \"%s\", %d);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:0, k[4]==3?int(a[4].v):3);
	else if(i==6)
		swprintf(out,1024,L"gr->Flow(%s, %s, %s, %s, %s, %s, \"%s\", %d);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, k[6]==2?a[6].s:0, k[7]==3?int(a[7].v):3);
}
//-----------------------------------------------------------------------------
//	{"fill","Fill data linearly in range [v1, v2]","fill var v1 v2 [dir]", mgls_fill, mglc_fill}
int mgls_fill(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3 && k[2]==3)	a[0].d->Fill(a[1].v,a[2].v,k[3]==2?a[3].s[0]:'x');
	else	return 1;
	return 0;
}
void mglc_fill(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3 && k[2]==3)
		swprintf(out,1024,L"%s.Fill(%g, %g, '%c');", a[0].s, a[1].v,a[2].v, k[3]==2?a[3].s[0]:'x');
}
//-----------------------------------------------------------------------------
//	{"fog","Switch on/off fog","fog var val [pos]", mgls_fog, mglc_fog}
int mgls_fog(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)	gr->Fog(a[0].v,k[1]==3?a[1].v:0.25);
	else	return 1;
	return 0;
}
void mglc_fog(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3)	swprintf(out,1024,L"gr->Fog(%g, %g);", a[0].v,k[1]==3?a[1].v:0.25);
}
//-----------------------------------------------------------------------------
//	{"font","Setup font","font fmt [size]", mgls_font, mglc_font}
int mgls_font(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==2)
	{	strcpy(gr->FontDef,a[0].s);	if(k[1]==3)	gr->FontSize = a[1].v;	}
	else	return 1;
	return 0;
}
void mglc_font(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==2 && k[1]==3)
		swprintf(out,1024,L"strcpy(gr->FontDef,\"%s\");\tgr->FontSize = %g;", a[0].s, a[1].v);
	else if(k[0]==2)
		swprintf(out,1024,L"strcpy(gr->FontDef,\"%s\");",a[0].s);
}
//-----------------------------------------------------------------------------
//	{"grid","Draw grid",0, mgls_grid, mglc_grid}
int mgls_grid(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)			gr->Grid(k[0]==2?a[0].s:"xyz", k[1]==2?a[1].s:"B-");
	else if(k[1]!=1)	gr->Grid(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		gr->Grid(*(a[0].d), *(a[1].d), *(a[2].d), k[3]==2?a[3].s:0, k[4]==3?a[4].v:NAN);
	else	return 1;
	return 0;
}
void mglc_grid(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	swprintf(out,1024,L"gr->Grid(\"%s\", \"%s\");", k[0]==2?a[0].s:"xyz", k[1]==2?a[1].s:"B-");
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Grid(%s,\"%s\", %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		swprintf(out,1024,L"gr->Grid(%s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"grid3","Draw grid at slices of 3D data","grid3 {xvar yvar zvar} avar dir [pos fmt]", mgls_grid3, mglc_grid3}
int mgls_grid3(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==2)
		gr->Grid3(*(a[0].d),a[1].s[0],k[2]==3?int(a[2].v):-1,
				k[3]==2?a[3].s:0);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		gr->Grid3(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].s[0],
				k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0);
	else	return 1;
	return 0;
}
void mglc_grid3(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]==2)
		swprintf(out,1024,L"gr->Grid3(%s, '%c', %d, \"%s\");", a[0].s, a[1].s[0], k[2]==3?int(a[2].v):-1, k[3]==2?a[3].s:"");
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		swprintf(out,1024,L"gr->Grid3(%s, %s, %s, %s, '%c', %d, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s[0], k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0);
}
//-----------------------------------------------------------------------------
//	{"grida","Draw grid at central slices of 3D data","grida {xvar yvar zvar} avar [fmt num]", mgls_grida, mglc_grida}
int mgls_grida(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->GridA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	else	gr->GridA(*(a[0].d),k[1]==2?a[1].s:0);
	return 0;
}
void mglc_grida(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->GridA(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
	else
		swprintf(out,1024,L"gr->GridA(%s, \"%s\");", a[0].s, k[1]==2?a[1].s:"");
}
//-----------------------------------------------------------------------------
//	{"light","Setup light",0, mgls_light, mglc_light}
int mgls_light(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==0)			gr->Light(true);
	else if(k[0]!=3)	return 1;
	else if(k[1]!=3)	gr->Light(a[0].v!=0);
	else if(k[2]==3 && k[3]==3)
		gr->Light(int(a[0].v),mglPoint(a[1].v,a[2].v,a[3].v),
			k[4]==2?a[4].s[0]:'w',k[5]==3?a[5].v:0.5,true);
	else	gr->Light(int(a[0].v),a[1].v!=0);
	return 0;
}
void mglc_light(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==0)			swprintf(out,1024,L"gr->Light(true);");
	else if(k[0]!=3)	return;
	else if(k[1]!=3)	swprintf(out,1024,L"gr->Light(%s);", a[0].v!=0 ? "true":"false");
	else if(k[2]==3 && k[3]==3)
		swprintf(out,1024,L"gr->Light(%d, mglPoint(%g, %g, %g), '%c', %g, true);", int(a[0].v), a[1].v, a[2].v, a[3].v, k[4]==2?a[4].s[0]:'w', k[5]==3?a[5].v:0.5);
	else	swprintf(out,1024,L"gr->Light(%d, %s);", int(a[0].v), a[1].v!=0 ? "true":"false");
}
//-----------------------------------------------------------------------------
//	{"line","Draw line",0, mgls_line, mglc_line}
int mgls_line(mglGraph *gr, long n, mglArg *a, int k[10])
{
	int i;
	if(n>5)
	{
		bool ok=true;
		for(i=0;i<6;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
			gr->Line(mglPoint(a[0].v,a[1].v,a[2].v),
				mglPoint(a[3].v,a[4].v,a[5].v),
				(n==7 && a[6].type==1) ? a[6].s : 0, 100);
		else	return 1;
	}
	else if(n>3)
	{
		bool ok=true;
		for(i=0;i<4;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
			gr->Line(mglPoint(a[0].v,a[1].v,gr->Min.z), mglPoint(a[2].v,a[3].v,gr->Min.z),
				(n==5 && a[4].type==1) ? a[4].s : 0, 100);
		else	return 1;
	}
	else	return 1;
	return 0;
}
void mglc_line(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	int i;
	if(n>5)
	{
		bool ok=true;
		for(i=0;i<6;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
			swprintf(out,1024,L"gr->Line(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), \"%s\", 100);", a[0].v,a[1].v,a[2].v, a[3].v,a[4].v,a[5].v, (n==7 && a[6].type==1) ? a[6].s : "");
	}
	else if(n>3)
	{
		bool ok=true;
		for(i=0;i<4;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
			swprintf(out,1024,L"gr->Line(mglPoint(%g, %g, gr->Min.z), mglPoint(%g, %g, gr->Min.z), \"%s\", 100);", a[0].v,a[1].v,a[2].v, a[3].v, (n==5 && a[4].type==1) ? a[4].s : "");
	}
}
//-----------------------------------------------------------------------------
//	{"legend","Draw legend","legend {xpos ypos} | pos [fmt size]", mgls_legend, mglc_legend}
int mgls_legend(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3)
		gr->Legend(a[0].v, a[1].v, k[2]==2?a[2].s:"rL", k[3]==3?a[3].v:-1);
	else
		gr->Legend(k[0]==3?int(a[0].v):3, k[1]==2?a[1].s:"rL", k[2]==3?a[2].v:-1);
	return 0;
}
void mglc_legend(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3)
		swprintf(out,1024,L"gr->Legend(%g, %g, \"%s\", %g);", a[0].v, a[1].v, k[2]==2?a[2].s:"rL", k[3]==3?a[3].v:-1);
	else
		swprintf(out,1024,L"gr->Legend(%d, \"%s\", %g);", k[0]==3?int(a[0].v):3, k[1]==2?a[1].s:"rL", k[2]==3?a[2].v:-1);
}
//-----------------------------------------------------------------------------
//	{"linewidth","Set default line width","linewidth val", mgls_linewidth, mglc_linewidth}
int mgls_linewidth(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && a[0].v>=1)	gr->BaseLineWidth = a[0].v;
	else	return 1;
	return 0;
}
void mglc_linewidth(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && a[0].v>=1)	swprintf(out,1024,L"gr->BaseLineWidth = %g;", a[0].v);
}
//-----------------------------------------------------------------------------
//	{"legendbox","Switch on/off drawing box in legend","legendbox val", mgls_legendbox, mglc_legendbox}
int mgls_legendbox(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)	gr->LegendBox = (a[0].v!=0);
	else	return 1;
	return 0;
}
void mglc_legendbox(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3)	swprintf(out,1024,L"gr->LegendBox = %s;", (a[0].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"modify","Modify data values by formula","modify var formula [var1 var2]", mgls_modify, mglc_modify}
int mgls_modify(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
		a[0].d->Modify(a[1].s,*(a[2].d),*(a[3].d));
	else if(k[0]==1 && k[1]==2 && k[2]==1)
		a[0].d->Modify(a[1].s,*(a[2].d));
	else if(k[0]==1 && k[1]==2)
		a[0].d->Modify(a[1].s, k[2]==3 ? int(a[2].v):0);
	else	return 1;
	return 0;
}
void mglc_modify(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"%s.Modify(\"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s);
	else if(k[0]==1 && k[1]==2 && k[2]==1)
		swprintf(out,1024,L"%s.Modify(\"%s\", %s);", a[0].s, a[1].s, a[2].s);
	else if(k[0]==1 && k[1]==2)
		swprintf(out,1024,L"%s.Modify(\"%s\", %d);", a[0].s, a[1].s, k[2]==3 ? int(a[2].v):0);
}
//-----------------------------------------------------------------------------
//	{"max","Find maximal value over direction","max ovar ivar dir", mgls_max, mglc_max}
int mgls_max(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)	*(a[0].d) = a[1].d->Max(a[2].s);
	else	return 1;
	return 0;
}
void mglc_max(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)
		swprintf(out,1024,L"%s = %s.Max(\"%s\");", a[0].s, a[1].s, a[2].s);
}
//-----------------------------------------------------------------------------
//	{"min","Find minimal value over direction","min ovar ivar dir", mgls_min, mglc_min}
int mgls_min(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)	*(a[0].d) = a[1].d->Min(a[2].s);
	else	return 1;
	return 0;
}
void mglc_min(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)
		swprintf(out,1024,L"%s = %s.Min(\"%s\");", a[0].s, a[1].s, a[2].s);
}
//-----------------------------------------------------------------------------
//	{"sum","Find summation over direction","sum ovar ivar dir", mgls_sum, mglc_sum}
int mgls_sum(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)	*(a[0].d) = a[1].d->Sum(a[2].s);
	else	return 1;
	return 0;
}
void mglc_sum(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)
		swprintf(out,1024,L"%s = %s.Sum(\"%s\");", a[0].s, a[1].s, a[2].s);
}
//-----------------------------------------------------------------------------
//	{"meshnum","Set number of lines in mesh/fall/vect and so on","meshnum val", mgls_meshnum, mglc_meshnum}
int mgls_meshnum(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)	gr->MeshNum = int(a[0].v);
	else	return 1;
	return 0;
}
void mglc_meshnum(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3)	swprintf(out,1024,L"gr->MeshNum = %d;", int(a[0].v));
}
//-----------------------------------------------------------------------------
//	{"marksize","Set size of markers","marksize val", mgls_marksize, mglc_marksize}
int mgls_marksize(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)	gr->MarkSize = a[0].v/50;
	else	return 1;
	return 0;
}
void mglc_marksize(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3)	swprintf(out,1024,L"gr->MarkSize = %g;", a[0].v/50);
}
//-----------------------------------------------------------------------------
//	{"mark","Draw mark plot for 1D data","mark {xvar} yvar {{zvar}} rvar [fmt num]", mgls_mark, mglc_mark}
int mgls_mark(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1 || k[1]!=1)	return 1;
	else if(k[2]!=1)	gr->Mark(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else if(k[3]!=1)	gr->Mark(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
	else 				gr->Mark(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	return 0;
}
void mglc_mark(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1 || k[1]!=1)	return;
	else if(k[2]!=1)
		swprintf(out,1024,L"gr->Mark(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else if(k[3]!=1)
		swprintf(out,1024,L"gr->Mark(%s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
	else
		swprintf(out,1024,L"gr->Mark(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
}
//-----------------------------------------------------------------------------
//	{"map","Draw mapping plot","map {xvar yvar} uvar vvar [fmt]", mgls_map, mglc_map}
int mgls_map(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		gr->Map(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0, 0,k[5]==3?(a[5].v!=0):true);
	else if(k[0]==1 && k[1]==1)
		gr->Map(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,0,k[3]==3?(a[3].v!=0):true);
	else	return 1;
	return 0;
}
void mglc_map(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->Map(%s, %s, %s, %s, \"%s\", 0, %s);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", (k[5]!=3 || a[5].v!=0)?"true":"false");
	else if(k[0]==1 && k[1]==1)
		swprintf(out,1024,L"gr->Map(%s, %s, \"%s\", 0, %s);", a[0].s, a[1].s, k[2]==2?a[2].s:"", (k[3]!=3 || a[3].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"read","Read data from file","read var file [nx ny nz]", mgls_read, mglc_read}
int mgls_read(mglGraph *gr, long n, mglArg *a, int k[10])
{
	bool rr=true;
	if(k[0]!=1 || k[1]!=2)	return 1;
	else if(n==2)	rr=a[0].d->Read(a[1].s);
	else rr=a[0].d->Read(a[1].s, k[2]==3?int(a[2].v):1,
			k[3]==3?int(a[3].v):1, k[4]==3?int(a[4].v):1);
	if(!rr)	gr->SetWarn(mglWarnFile);
	return 0;
}
void mglc_read(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1 || k[1]!=2)	return;
	else if(n==2)	swprintf(out,1024,L"%s.Read(\"%s\");", a[0].s, a[1].s);
	else
		swprintf(out,1024,L"%s.Read(\"%s\", %d, %d, %d);", a[0].s, a[1].s, k[2]==3?int(a[2].v):1, k[3]==3?int(a[3].v):1, k[4]==3?int(a[4].v):1);
}
//-----------------------------------------------------------------------------
//	{"readmat","Read data from file with sizes specified in first row","readmat var file [dim=2]", mgls_readmat, mglc_readmat}
int mgls_readmat(mglGraph *gr, long n, mglArg *a, int k[10])
{
	bool rr=true;
	if(k[0]!=1 || k[1]!=2)	return 1;
	else	rr=a[0].d->ReadMat(a[1].s, k[2]==3?int(a[2].v):2);
	if(!rr)	gr->SetWarn(mglWarnFile);
	return 0;
}
void mglc_readmat(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)
		swprintf(out,1024,L"%s.ReadMat(\"%s\", %d);", a[0].s, a[1].s, k[2]==3?int(a[2].v):2);
}
//-----------------------------------------------------------------------------
//	{"readall","Read and join data from several files",0, mgls_readall, mglc_readall}
int mgls_readall(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1 || k[1]!=2)	return 1;
	else if(k[2]==3 && k[3]==3)
		a[0].d->ReadRange(a[1].s, a[2].v, a[3].v, k[4]==3?a[4].v:1.f, k[5]==3?(a[5].v!=0) : false);
	else a[0].d->ReadAll(a[1].s, k[2]==3?(a[2].v!=0) : false);
	return 0;
}
void mglc_readall(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1 || k[1]!=2)	return;
	else if(k[2]==3 && k[3]==3)
		swprintf(out,1024,L"%s.ReadRange(\"%s\", %g, %g, %g, %s);", a[0].s, a[1].s, a[2].v, a[3].v, k[4]==3?a[4].v:1.f, (k[5]==3&&a[5].v!=0)?"true":"false");
	else
		swprintf(out,1024,L"%s.ReadAll(\"%s\", %s);", a[0].s, a[1].s, (k[2]==3&&a[2].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"readhdf","Read data from HDF5 file","readhdf var file id", mgls_readhdf, mglc_readhdf}
int mgls_readhdf(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2 && k[2]==2)	a[0].d->ReadHDF(a[1].s, a[2].s);
	else	return 1;
	return 0;
}
void mglc_readhdf(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2 && k[2]==2)	swprintf(out,1024,L"%s.ReadHDF(\"%s\", \"%s\");", a[0].s, a[1].s, a[2].s);
}
//-----------------------------------------------------------------------------
//	{"rect","Draw rectangle",0, mgls_rect, mglc_rect}
int mgls_rect(mglGraph *gr, long n, mglArg *a, int k[10])
{
	long i;
	if(n>5)
	{
		bool ok=true;
		for(i=0;i<6;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
		{
			if(a[0].v==a[3].v)	gr->Face(mglPoint(a[0].v,a[1].v,a[2].v),
				mglPoint(a[0].v,a[4].v,a[2].v),
				mglPoint(a[3].v,a[1].v,a[5].v),
				mglPoint(a[3].v,a[4].v,a[5].v),
				(n==7 && a[6].type==1) ? a[6].s : 0, 2);
			else	gr->Face(mglPoint(a[0].v,a[1].v,a[2].v),
				mglPoint(a[0].v,a[4].v,a[5].v),
				mglPoint(a[3].v,a[1].v,a[2].v),
				mglPoint(a[3].v,a[4].v,a[5].v),
				(n==7 && a[6].type==1) ? a[6].s : 0, 2);
		}
		else	return 1;
	}
	else if(n>3)
	{
		bool ok=true;
		float p = (gr->Min.z+gr->Max.z)/2;
		for(i=0;i<4;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
			gr->Face(mglPoint(a[0].v,a[1].v,p), mglPoint(a[0].v,a[3].v,p),
				mglPoint(a[2].v,a[1].v,p), mglPoint(a[2].v,a[3].v,p),
				(n==5 && a[4].type==1) ? a[4].s : 0, 2);
		else	return 1;
	}
	else	return 1;
	return 0;
}
void mglc_rect(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	long i;
	if(n>5)
	{
		bool ok=true;
		for(i=0;i<6;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
		{
			if(a[0].v==a[3].v)
				swprintf(out,1024,L"gr->Face(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), \"%s\", 2);", a[0].v,a[1].v,a[2].v, a[0].v,a[4].v,a[2].v, a[3].v,a[1].v,a[5].v, a[3].v,a[4].v,a[5].v, (n==7 && a[6].type==1) ? a[6].s : "");
			else
				swprintf(out,1024,L"gr->Face(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), \"%s\", 2);", a[0].v,a[1].v,a[2].v, a[0].v,a[4].v,a[5].v, a[3].v,a[1].v,a[2].v, a[3].v,a[4].v,a[5].v, (n==7 && a[6].type==1) ? a[6].s : "");
		}
	}
	else if(n>3)
	{
		bool ok=true;
		for(i=0;i<4;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
			swprintf(out,1024,L"gr->Face(mglPoint(%g, %g, (gr->Min.z+gr->Max.z)/2), mglPoint(%g, %g, (gr->Min.z+gr->Max.z)/2), mglPoint(%g, %g, (gr->Min.z+gr->Max.z)/2), mglPoint(%g, %g, (gr->Min.z+gr->Max.z)/2), \"%s\", 2);", a[0].v,a[1].v, a[0].v,a[3].v, a[2].v,a[1].v, a[2].v,a[3].v, (n==5 && a[4].type==1) ? a[4].s : "");
	}
}
//-----------------------------------------------------------------------------
//	{"resize","Resize data","resize ovar ivar [nx ny nz]", mgls_resize, mglc_resize}
int mgls_resize(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)
		*(a[0].d) = a[1].d->Resize(k[2]==3?int(a[2].v):1,
				k[3]==3?int(a[3].v):1, k[4]==3?int(a[4].v):1);
	else	return 1;
	return 0;
}
void mglc_resize(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)
		swprintf(out,1024,L"%s = %s.Resize(%d, %d, %d);", a[0].s, a[1].s, k[2]==3?int(a[2].v):1, k[3]==3?int(a[3].v):1, k[4]==3?int(a[4].v):1);
}
//-----------------------------------------------------------------------------
//	{"rotate","Rotate plot",0, mgls_rotate, mglc_rotate}
int mgls_rotate(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)
		gr->RotateN(a[0].v, a[1].v, a[2].v, a[3].v);
	else if(k[0]==3 && k[1]==3)
		gr->Rotate(a[0].v, a[1].v, k[2]==3?a[2].v:0);
	else	return 1;
	return 0;
}
void mglc_rotate(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)
		swprintf(out,1024,L"gr->RotateN(%g, %g, %g, %g);", a[0].v, a[1].v, a[2].v, a[3].v);
	else if(k[0]==3 && k[1]==3)
		swprintf(out,1024,L"gr->Rotate(%g, %g, %g);", a[0].v, a[1].v, k[2]==3?a[2].v:0);
}
//-----------------------------------------------------------------------------
//	{"rotatetext","Set to auto rotate text or not",0, mgls_rotatetext, mglc_rotatetext}
int mgls_rotatetext(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)	gr->RotatedText = (a[0].v!=0);
	else	return 1;
	return 0;
}
void mglc_rotatetext(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3)	swprintf(out,1024,L"gr->RotatedText = %s;", (a[0].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"save","Save data to file","save var file", mgls_save, mglc_save}
int mgls_save(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	a[0].d->Save(a[1].s);
	else	return 1;
	return 0;
}
void mglc_save(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	swprintf(out,1024,L"%s.Save(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"savehdf","Save data to HDF5 file","savehdf var file id", mgls_savehdf, mglc_savehdf}
int mgls_savehdf(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2 && k[2]==2)	a[0].d->SaveHDF(a[1].s, a[2].s);
	else	return 1;
	return 0;
}
void mglc_savehdf(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2 && k[2]==2)
		swprintf(out,1024,L"%s.SaveHDF(\"%s\", \"%s\");", a[0].s, a[1].s, a[2].s);
}
//-----------------------------------------------------------------------------
//	{"smooth","Smooth data","smooth var kind dir", mgls_smooth, mglc_smooth}
int mgls_smooth(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)		a[0].d->Smooth(k[1]==3?int(a[1].v):2,k[2]==2?a[2].s:"xyz");
	else	return 1;
	return 0;
}
void mglc_smooth(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)
		swprintf(out,1024,L"%s.Smooth(%d, \"%s\");", a[0].s, k[1]==3?int(a[1].v):2, k[2]==2?a[2].s:"xyz");
}
//-----------------------------------------------------------------------------
//	{"swap","Swap data (usefull after Fourier transform)","swap var dir", mgls_swap, mglc_swap}
int mgls_swap(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	a[0].d->Swap(a[1].s);
	else	return 1;
	return 0;
}
void mglc_swap(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	swprintf(out,1024,L"%s.Swap(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"set_id","Set column id for data","set_id var id", mgls_set_id, mglc_set_id}
int mgls_set_id(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	a[0].d->SetColumnId(a[1].s);
	else	return 1;
	return 0;
}
void mglc_set_id(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	swprintf(out,1024,L"%s.SetColumnId(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"stem","Draw stem plot for 1D data","stem {xvar} yvar {{zvar}} [fmt num]", mgls_stem, mglc_stem}
int mgls_stem(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Stem(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3 ? a[2].v : NAN);
	else if(k[2]!=1)	gr->Stem(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3 ? a[3].v : NAN);
	else 				gr->Stem(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	return 0;
}
void mglc_stem(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Stem(%s, \"%s\", %g);",a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
	else if(k[2]!=1)
		swprintf(out,1024,L"gr->Stem(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else 	swprintf(out,1024,L"gr->Stem(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"step","Draw step plot for 1D data","step {xvar} yvar {{zvar}} [fmt num]", mgls_step, mglc_step}
int mgls_step(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Step(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3 ? a[2].v : NAN);
	else if(k[2]!=1)	gr->Step(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3 ? a[3].v : NAN);
	else 				gr->Step(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	return 0;
}
void mglc_step(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Step(%s, \"%s\", %g);",a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
	else if(k[2]!=1)
		swprintf(out,1024,L"gr->Step(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else 	swprintf(out,1024,L"gr->Step(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"plot","Draw usual plot for 1D data","plot {xvar} yvar {{zvar}} [fmt num]", mgls_plot, mglc_plot}
int mgls_plot(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Plot(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3 ? a[2].v : NAN);
	else if(k[2]!=1)	gr->Plot(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3 ? a[3].v : NAN);
	else 				gr->Plot(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	return 0;
}
void mglc_plot(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Plot(%s, \"%s\", %g);",a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
	else if(k[2]!=1)
		swprintf(out,1024,L"gr->Plot(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else 	swprintf(out,1024,L"gr->Plot(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"squeeze","Squeeze data","squeeze var kx [ky kz]", mgls_squeeze, mglc_squeeze}
int mgls_squeeze(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3)
		a[0].d->Squeeze(int(a[1].v), k[2]==3?int(a[2].v):1, k[3]==3?int(a[3].v):1);
	else	return 1;
	return 0;
}
void mglc_squeeze(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3)
		swprintf(out,1024,L"%s.Squeeze(%d, %d, %d);", a[0].s, int(a[1].v), k[2]==3?int(a[2].v):1, k[3]==3?int(a[3].v):1);
}
//-----------------------------------------------------------------------------
//	{"stfad","Do STFA transform","", mgls_stfad, mglc_stfad}
int mgls_stfad(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3)
		*(a[0].d) = ::STFA(*(a[1].d),*(a[2].d), int(a[3].v), k[4]==2?a[4].s[0]:'x');
	else	return 1;
	return 0;
}
void mglc_stfad(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3)
		swprintf(out,1024,L"%s = ::STFA(%s, %s, %d, '%c');", a[0].s, a[1].s, a[2].s, int(a[3].v), k[4]==2?a[4].s[0]:'x');
}
//-----------------------------------------------------------------------------
//	{"setsize","Set picture size","setsize wval hval", mgls_setsize, mglc_setsize}
int mgls_setsize(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && a[1].v>1 && a[0].v>1)
		gr->SetSize(int(a[0].v), int(a[1].v));
	else	return 1;
	return 0;
}
void mglc_setsize(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && a[1].v>1 && a[0].v>1)
		swprintf(out,1024,L"gr->SetSize(%d, %d);", int(a[0].v), int(a[1].v));
}
//-----------------------------------------------------------------------------
//	{"sphere","Draw sphere",0, mgls_sphere, mglc_sphere}
int mgls_sphere(mglGraph *gr, long n, mglArg *a, int k[10])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=3)	break;
	if(i==4)
		gr->Sphere(mglPoint(a[0].v,a[1].v,a[2].v), a[3].v, k[4]==2 ? a[4].s:"r");
	else if(i==3)
		gr->Sphere(mglPoint(a[0].v,a[1].v), a[2].v, k[3]==2 ? a[3].s:"r");
	else	return 1;
	return 0;
}
void mglc_sphere(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=3)	break;
	if(i==4)
		swprintf(out,1024,L"gr->Sphere(mglPoint(%g, %g, %g), %g, \"%s\");", a[0].v,a[1].v,a[2].v, a[3].v, k[4]==2 ? a[4].s:"r");
	else if(i==3)
		swprintf(out,1024,L"gr->Sphere(mglPoint(%g, %g), %g, \"%s\");", a[0].v,a[1].v, a[2].v, k[3]==2 ? a[3].s:"r");
}
//-----------------------------------------------------------------------------
//	{"stfa","Draw STFA diagram","stfa {xvar yvar} uvar vvar num [fmt]", mgls_stfa, mglc_stfa}
int mgls_stfa(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==3)
		gr->STFA(*(a[0].d),*(a[1].d), int(a[2].v), k[3]==2?a[3].s:"");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
		gr->STFA(*(a[0].d),*(a[1].d), *(a[2].d),*(a[3].d), int(a[4].v), k[5]==2?a[5].s:"");
	else	return 1;
	return 0;
}
void mglc_stfa(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==3)
		swprintf(out,1024,L"gr->STFA(%s, %s, %d, \"%s\");", a[0].s, a[1].s, int(a[2].v), k[3]==2?a[3].s:"");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
		swprintf(out,1024,L"gr->STFA(%s, %s, %s, %s, %d, \"%s\");", a[0].s, a[1].s,  a[2].s, a[3].s, int(a[4].v), k[5]==2?a[5].s:"");
}
//-----------------------------------------------------------------------------
//	{"surf3","Draw isosurface for 3D data","surf3 {xvar yvar zvar} avar {num} [fmt]", mgls_surf3, mglc_surf3}
int mgls_surf3(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==0)	gr->Surf3(*(a[0].d));
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
		gr->Surf3(a[4].v,*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),
				k[5]==2? a[5].s:0);
	else if(k[1]==3)	gr->Surf3(a[1].v,*(a[0].d),k[2]==2? a[2].s:0);
	else if(k[1]==2)	gr->Surf3(*(a[0].d),a[1].s,int(k[2]==3?a[2].v:3));
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->Surf3(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),
				k[4]==2? a[4].s:0,int(k[5]==3?a[5].v:3));
	else	return 1;
	return 0;
}
void mglc_surf3(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]==0)	swprintf(out,1024,L"gr->Surf3(%s);", a[0].s);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
		swprintf(out,1024,L"gr->Surf3(%g, %s, %s, %s, %s, \"%s\");", a[4].v, a[0].s, a[1].s, a[2].s, a[3].s, k[5]==2?a[5].s:"");
	else if(k[1]==3)
		swprintf(out,1024,L"gr->Surf3(%g, %s, \"%s\");", a[1].v, a[0].s, k[2]==2? a[2].s:"");
	else if(k[1]==2)
		swprintf(out,1024,L"gr->Surf3(%s, \"%s\", %d);",  a[0].s, a[1].s, int(k[2]==3?a[2].v:3));
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->Surf3(%s, %s, %s, %s, \"%s\", %d);",  a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", int(k[5]==3?a[5].v:3));
}
//-----------------------------------------------------------------------------
//	{"surf3c","Draw isosurface for 3D data colored by other data","surf3c {xvar yvar zvar} avar cvar {num} [fmt]", mgls_surf3c, mglc_surf3c}
int mgls_surf3c(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1 || k[1]!=1)	return 1;
	else if(k[2]==0)	gr->Surf3C(*(a[0].d),*(a[1].d));
	else if(k[2]==1 && k[3]==1 && k[4]==1 && k[5]==3)
		gr->Surf3C(a[5].v,*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),
				k[6]==2? a[6].s:0);
	else if(k[2]==3)	gr->Surf3C(a[2].v,*(a[0].d),*(a[1].d),k[3]==2? a[3].s:0);
	else if(k[2]==2)	gr->Surf3C(*(a[0].d),*(a[1].d),a[2].s,int(k[3]==3?a[3].v:3));
	else if(k[2]==1 && k[3]==1 && k[4]==1)
		gr->Surf3C(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),
				k[5]==2? a[5].s:0,int(k[6]==3?a[6].v:3));
	else	return 1;
	return 0;
}
void mglc_surf3c(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1 || k[1]!=1)	return;
	else if(k[2]==0)	swprintf(out,1024,L"gr->Surf3C(%s, %s);",  a[0].s, a[1].s);
	else if(k[2]==1 && k[3]==1 && k[4]==1 && k[5]==3)
		swprintf(out,1024,L"gr->Surf3C(%g, %s, %s, %s, %s, %s, \"%s\");", a[5].v, a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, k[6]==2? a[6].s:"");
	else if(k[2]==3)
		swprintf(out,1024,L"gr->Surf3C(%g, %s, %s, \"%s\");", a[2].v, a[0].s, a[1].s, k[3]==2?a[3].s:0);
	else if(k[2]==2)
		swprintf(out,1024,L"gr->Surf3C(%s, %s, \"%s\", %d);",  a[0].s, a[1].s, a[2].s, int(k[3]==3?a[3].v:3));
	else if(k[2]==1 && k[3]==1 && k[4]==1)
		swprintf(out,1024,L"gr->Surf3C(%s, %s, %s, %s, %s, \"%s\", %d);",  a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, k[5]==2?a[5].s:"", int(k[6]==3?a[6].v:3));
}
//-----------------------------------------------------------------------------
//	{"surf3a","Draw isosurface for 3D data transpared by other data","surf3a {xvar yvar zvar} avar cvar {num} [fmt]", mgls_surf3a, mglc_surf3a}
int mgls_surf3a(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1 || k[1]!=1)	return 1;
	else if(k[2]==0)	gr->Surf3A(*(a[0].d),*(a[1].d));
	else if(k[2]==1 && k[3]==1 && k[4]==1 && k[5]==3)
		gr->Surf3A(a[5].v,*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),
				k[6]==2? a[6].s:0);
	else if(k[2]==3)	gr->Surf3A(a[2].v,*(a[0].d),*(a[1].d),k[3]==2? a[3].s:0);
	else if(k[2]==2)	gr->Surf3A(*(a[0].d),*(a[1].d),a[2].s,int(k[3]==3?a[3].v:3));
	else if(k[2]==1 && k[3]==1 && k[4]==1)
		gr->Surf3A(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),
				k[5]==2? a[5].s:0,int(k[6]==3?a[6].v:3));
	else	return 1;
	return 0;
}
void mglc_surf3a(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1 || k[1]!=1)	return;
	else if(k[2]==0)	swprintf(out,1024,L"gr->Surf3A(%s, %s);",  a[0].s, a[1].s);
	else if(k[2]==1 && k[3]==1 && k[4]==1 && k[5]==3)
		swprintf(out,1024,L"gr->Surf3A(%g, %s, %s, %s, %s, %s, \"%s\");", a[5].v, a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, k[6]==2? a[6].s:"");
	else if(k[2]==3)
		swprintf(out,1024,L"gr->Surf3A(%g, %s, %s, \"%s\");", a[2].v, a[0].s, a[1].s, k[3]==2?a[3].s:0);
	else if(k[2]==2)
		swprintf(out,1024,L"gr->Surf3A(%s, %s, \"%s\", %d);",  a[0].s, a[1].s, a[2].s, int(k[3]==3?a[3].v:3));
	else if(k[2]==1 && k[3]==1 && k[4]==1)
		swprintf(out,1024,L"gr->Surf3A(%s, %s, %s, %s, %s, \"%s\", %d);",  a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, k[5]==2?a[5].s:"", int(k[6]==3?a[6].v:3));
}
//-----------------------------------------------------------------------------
//	{"subplot","Set position of plot","subplot mval nval ival", mgls_subplot, mglc_subplot}
int mgls_subplot(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		gr->SubPlot(int(a[0].v), int(a[1].v), int(a[2].v), k[3]==3?a[3].v:0, k[4]==3?a[4].v:0);
	else	return 1;
	return 0;
}
void mglc_subplot(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		swprintf(out,1024,L"gr->SubPlot(%d, %d, %d, %g, %g);", int(a[0].v), int(a[1].v), int(a[2].v), k[3]==3?a[3].v:0, k[4]==3?a[4].v:0);
}
//-----------------------------------------------------------------------------
//	{"subdata","Extract sub-array","subdata ovar ivar nx [ny nz]", mgls_subdata, mglc_subdata}
int mgls_subdata(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==3)
		*(a[0].d) = a[1].d->SubData(int(a[2].v), k[3]==3?int(a[3].v):-1, k[4]==3?int(a[4].v):-1);
	else	return 1;
	return 0;
}
void mglc_subdata(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==3)
		swprintf(out,1024,L"%s = %s.SubData(%d, %d, %d);",  a[0].s, a[1].s, int(a[2].v), k[3]==3?int(a[3].v):-1, k[4]==3?int(a[4].v):-1);
}
//-----------------------------------------------------------------------------
//	{"tile","Draw horizontal tiles","tile {xvar yvar} zvar {rvar} [fmt]", mgls_tile, mglc_tile}
int mgls_tile(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Tile(*(a[0].d), k[1]==2? a[1].s:0);
	else if(k[1]==1 && k[2]!=1)
		gr->Tile(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0);
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->Tile(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2? a[4].s:0);
	else if(k[1]==1 && k[2]==1)
		gr->Tile(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2? a[3].s:0);
	else	return 1;
	return 0;
}
void mglc_tile(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		swprintf(out,1024,L"gr->Tile(%s, \"%s\");", a[0].s, k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]!=1)
		swprintf(out,1024,L"gr->Tile(%s, %s, \"%s\");", a[0].s, a[1].s, k[2]==2?a[2].s:"");
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->Tile(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
	else if(k[1]==1 && k[2]==1)
		swprintf(out,1024,L"gr->Tile(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"text","Draw text at some position or along curve",0, mgls_text, mglc_text}
int mgls_text(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==2)
		gr->Text(mglPoint(a[0].v,a[1].v,a[2].v),a[3].w,
			(k[4]==2 && a[4].s[0]!=0)?a[4].s:gr->FontDef,k[5]==3?a[5].v:-1.4);
	else if(k[0]==3 && k[1]==3 && k[2]==2)
		gr->Text(mglPoint(a[0].v,a[1].v),a[2].w,
			(k[3]==2 && a[3].s[0]!=0)?a[3].s:gr->FontDef,k[4]==3?a[4].v:-1.4);
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2)
		gr->Text(*(a[0].d),*(a[1].d),*(a[2].d),a[3].w,k[4]==2?a[4].s:0,k[5]==3?a[5].v:-1);
	else if(k[0]==1 && k[1]==1 && k[2]==2)
		gr->Text(*(a[0].d),*(a[1].d),a[2].w,k[3]==2?a[3].s:0,
			k[4]==3?a[4].v:-1,k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==2)
		gr->Text(*(a[0].d),a[1].w,k[2]==2?a[2].s:0,k[3]==3?a[3].v:-1,k[4]==3?a[4].v:NAN);
	else	return 1;
	return 0;
}
void mglc_text(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==2)
		swprintf(out,1024,L"gr->Text(mglPoint(%g, %g, %g), \"%ls\", \"%s\", %g);", a[0].v, a[1].v, a[2].v, a[3].w, (k[4]==2 && a[4].s[0]!=0)?a[4].s:"", k[5]==3?a[5].v:-1.4);
	else if(k[0]==3 && k[1]==3 && k[2]==2)
		swprintf(out,1024,L"gr->Text(mglPoint(%g, %g), \"%ls\", \"%s\", %g);", a[0].v, a[1].v, a[2].w, (k[3]==2 && a[3].s[0]!=0)?a[3].s:"", k[4]==3?a[4].v:-1.4);
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2)
		swprintf(out,1024,L"gr->Text(%s, %s, %s, \"%ls\", \"%s\", %g);",  a[0].s, a[1].s, a[2].s, a[3].w, k[4]==2?a[4].s:"", k[5]==3?a[5].v:-1);
	else if(k[0]==1 && k[1]==1 && k[2]==2)
		swprintf(out,1024,L"gr->Text(%s, %s, \"%ls\", \"%s\", %g, %g);",  a[0].s, a[1].s, a[2].w, k[3]==2?a[3].s:"", k[4]==3?a[4].v:-1, k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==2)
		swprintf(out,1024,L"gr->Text(%s, \"%ls\", \"%s\", %g, %g);",  a[0].s, a[1].w, k[2]==2?a[2].s:"", k[3]==3?a[3].v:-1, k[4]==3?a[4].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"torus","Draw surface of curve rotation","torus {zvar} rvar [fmt]", mgls_torus, mglc_torus}
int mgls_torus(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)	gr->Torus(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0);
	else if(k[0]==1)		gr->Torus(*(a[0].d),k[1]==2?a[1].s:0);
	else	return 1;
	return 0;
}
void mglc_torus(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1)	swprintf(out,1024,L"gr->Torus(%s, %s, \"%s\");", a[0].s, a[1].s, k[2]==2?a[2].s:"");
	else if(k[0]==1)		swprintf(out,1024,L"gr->Torus(%s, \"%s\");", a[0].s, k[1]==2?a[1].s:"");
}
//-----------------------------------------------------------------------------
//	{"transparent","Switch off transparency","transparent val", mgls_transparent, mglc_transparent} //!!! OLD !!!
int mgls_transparent(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)	gr->Transparent = (a[0].v!=0);
	else	return 1;
	return 0;
}
void mglc_transparent(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3)	swprintf(out,1024,L"gr->Transparent = %s;", (a[0].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"transptype","Set type transparency","transptype val", mgls_transptype, mglc_transptype}
int mgls_transptype(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)	gr->TranspType = int(a[0].v);
	else	return 1;
	return 0;
}
void mglc_transptype(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3)	swprintf(out,1024,L"gr->TranspType = %d;", int(a[0].v));
}
//-----------------------------------------------------------------------------
//	{"transform","Do integral transform of data","transform ovar how rvar ivar", mgls_transform, mglc_transform}
int mgls_transform(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
		*(a[0].d) = Transform(*(a[2].d),*(a[3].d),a[1].s);
	else	return 1;
	return 0;
}
void mglc_transform(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"%s = Transform(%s, %s, \"%s\");",a[0].s,  a[2].s, a[3].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"transforma","Do integral transform of data","transforma ovar how avar fvar", mgls_transforma, mglc_transforma}
int mgls_transforma(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
		*(a[0].d) = TransformA(*(a[2].d),*(a[3].d),a[1].s);
	else	return 1;
	return 0;
}
void mglc_transforma(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"%s = TransformA(%s, %s, \"%s\");",a[0].s,  a[2].s, a[3].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"tube","Draw curve by tube",0, mgls_tube, mglc_tube}
int mgls_tube(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		gr->Tube(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3)
		gr->Tube(*(a[0].d),*(a[1].d),*(a[2].d),a[3].v,k[4]==2?a[4].s:0);
	else if(k[0]==1 && k[1]==1 && k[2]==1)
		gr->Tube(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==1 && k[2]==3)
		gr->Tube(*(a[0].d),*(a[1].d),a[2].v,k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==1)
		gr->Tube(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else if(k[0]==1 && k[1]==3)
		gr->Tube(*(a[0].d),a[1].v,k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else	return 1;
	return 0;
}
void mglc_tube(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->Tube(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3)
		swprintf(out,1024,L"gr->Tube(%s, %s, %s, %g, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].v, k[4]==2?a[4].s:"");
	else if(k[0]==1 && k[1]==1 && k[2]==1)
		swprintf(out,1024,L"gr->Tube(%s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==1 && k[2]==3)
		swprintf(out,1024,L"gr->Tube(%s, %s, %g, \"%s\", %g);", a[0].s, a[1].s, a[2].v, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==1)
		swprintf(out,1024,L"gr->Tube(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else if(k[0]==1 && k[1]==3)
		swprintf(out,1024,L"gr->Tube(%s, %g, \"%s\", %g);", a[0].s, a[1].v, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"textmark","Draw TeX mark at point position",0, mgls_textmark, mglc_textmark}
int mgls_textmark(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		gr->TextMark(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].w,k[5]==2?a[5].s:0);
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2)
		gr->TextMark(*(a[0].d),*(a[1].d),*(a[2].d),a[3].w,k[4]==2?a[4].s:0,k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==1 && k[2]==2)
		gr->TextMark(*(a[0].d),*(a[1].d),a[2].w,k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==2)
		gr->TextMark(*(a[0].d),a[1].w,k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else	return 1;
	return 0;
}
void mglc_textmark(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		swprintf(out,1024,L"gr->TextMark(%s, %s, %s, %s, \"%ls\", \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, a[4].w, k[5]==2?a[5].s:"");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2)
		swprintf(out,1024,L"gr->TextMark(%s, %s, %s, \"%ls\", \"%s\", %g);", a[0].s, a[1].s, a[2].s, a[3].w, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==1 && k[2]==2)
		swprintf(out,1024,L"gr->TextMark(%s, %s, \"%ls\", \"%s\", %g);", a[0].s, a[1].s, a[2].w, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==2)
		swprintf(out,1024,L"gr->TextMark(%s, \"%ls\", \"%s\", %g);", a[0].s, a[1].w, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"triplot","Draw surface of triangles",0, mgls_triplot, mglc_triplot}
int mgls_triplot(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		gr->TriPlot(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	else if(k[0]==1 && k[1]==1 && k[2]==1)
		gr->TriPlot(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
	else	return 1;
	return 0;
}
void mglc_triplot(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->TriPlot(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
	else if(k[0]==1 && k[1]==1 && k[2]==1)
		swprintf(out,1024,L"gr->TriPlot(%s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"ternary","Switch on/off to use ternary axis","ternary val", mgls_ternary, mglc_ternary}
int mgls_ternary(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)	gr->Ternary(a[0].v!=0);
	else	return 1;
	return 0;
}
void mglc_ternary(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3)	swprintf(out,1024,L"gr->Ternary(%s);", a[0].v!=0?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"transpose","Transpose data array","transpose var [dir]", mgls_transpose, mglc_transpose}
int mgls_transpose(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	a[0].d->Transpose(k[1]==2?a[1].s:"yxz");
	else	return 1;
	return 0;
}
void mglc_transpose(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)	swprintf(out,1024,L"%s.Transpose(\"%s\");", a[0].s, k[1]==2?a[1].s:"yxz");
}
//-----------------------------------------------------------------------------
//	{"vectc","Draw vector field with coloring",0, mgls_vectc, mglc_vectc}
int mgls_vectc(mglGraph *gr, long n, mglArg *a, int k[10])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		gr->VectC(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else if(i==4)
		gr->VectC(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,k[5]==3?a[5].v:NAN);
	else if(i==3)
		gr->VectC(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else if(i==6)
		gr->VectC(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),*(a[5].d),k[6]==2?a[6].s:0);
	else	return 1;
	return 0;
}
void mglc_vectc(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		swprintf(out,1024,L"gr->VectC(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else if(i==4)
		swprintf(out,1024,L"gr->VectC(%s, %s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
	else if(i==3)
		swprintf(out,1024,L"gr->VectC(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:0);
	else if(i==6)
		swprintf(out,1024,L"gr->VectC(%s, %s, %s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, k[6]==2?a[6].s:"");
}
//-----------------------------------------------------------------------------
//	{"vect","Draw vector field",0, mgls_vect, mglc_vect}
int mgls_vect(mglGraph *gr, long n, mglArg *a, int k[10])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		gr->Vect(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else if(i==4)
		gr->Vect(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,k[5]==3?a[5].v:NAN);
	else if(i==3)
		gr->Vect(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else if(i==6)
		gr->Vect(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),*(a[5].d),k[6]==2?a[6].s:0);
	else	return 1;
	return 0;
}
void mglc_vect(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		swprintf(out,1024,L"gr->Vect(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else if(i==4)
		swprintf(out,1024,L"gr->Vect(%s, %s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
	else if(i==3)
		swprintf(out,1024,L"gr->Vect(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:0);
	else if(i==6)
		swprintf(out,1024,L"gr->Vect(%s, %s, %s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, k[6]==2?a[6].s:"");
}
//-----------------------------------------------------------------------------
//	{"xlabel","Draw label for x-axis","xlabel txt [fnt pos]", mgls_xlabel, mglc_xlabel}
int mgls_xlabel(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==2)	gr->Label('x',a[0].s,k[1]==3?int(a[1].v):1);
	else	return 1;
	return 0;
}
void mglc_xlabel(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==2)	swprintf(out,1024,L"gr->Label('x',\"%s\",%d);", a[0].s, k[1]==3?int(a[1].v):1);
}
//-----------------------------------------------------------------------------
//	{"ylabel","Draw label for y-axis","ylabel txt [fnt pos]", mgls_ylabel, mglc_ylabel}
int mgls_ylabel(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==2)	gr->Label('y',a[0].s,k[1]==3?int(a[1].v):1);
	else	return 1;
	return 0;
}
void mglc_ylabel(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==2)	swprintf(out,1024,L"gr->Label('y',\"%s\",%d);", a[0].s, k[1]==3?int(a[1].v):1);
}
//-----------------------------------------------------------------------------
//	{"zlabel","Draw label for z-axis","zlabel txt [fnt pos]", mgls_zlabel, mglc_zlabel}
int mgls_zlabel(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==2)	gr->Label('z',a[0].s,k[1]==3?int(a[1].v):1);
	else	return 1;
	return 0;
}
void mglc_zlabel(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==2)	swprintf(out,1024,L"gr->Label('z',\"%s\",%d);", a[0].s, k[1]==3?int(a[1].v):1);
}
//-----------------------------------------------------------------------------
//	{"xrange","Set range for x-axis","xrange {var [add]} | {x1 x2}", mgls_xrange, mglc_xrange}
int mgls_xrange(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->XRange(*(a[0].d),k[1]==3?(a[1].v!=0):false);
	else if(k[0]==3 && k[1]==3)
	{	gr->Min.x = a[0].v;	gr->Max.x = a[1].v;	gr->RecalcBorder();	}
	else	return 1;
	return 0;
}
void mglc_xrange(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)
		swprintf(out,1024,L"gr->XRange(%s, %s);", a[0].s, (k[1]==3&&a[1].v!=0)?"true":"false");
	else if(k[0]==3 && k[1]==3)
		swprintf(out,1024,L"gr->Min.x = %g;\tgr->Max.x = %g;\tgr->RecalcBorder();", a[0].v, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"yrange","Set range for y-axis","yrange {var [add]} | {x1 x2}", mgls_yrange, mglc_yrange}
int mgls_yrange(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->YRange(*(a[0].d),k[1]==3?(a[1].v!=0):false);
	else if(k[0]==3 && k[1]==3)
	{	gr->Min.y = a[0].v;	gr->Max.y = a[1].v;	gr->RecalcBorder();	}
	else	return 1;
	return 0;
}
void mglc_yrange(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)
		swprintf(out,1024,L"gr->YRange(%s, %s);", a[0].s, (k[1]==3&&a[1].v!=0)?"true":"false");
	else if(k[0]==3 && k[1]==3)
		swprintf(out,1024,L"gr->Min.y = %g;\tgr->Max.y = %g;\tgr->RecalcBorder();", a[0].v, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"zrange","Set range for z-axis","yrange {var [add]} | {x1 x2}", mgls_zrange, mglc_zrange}
int mgls_zrange(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	gr->ZRange(*(a[0].d),k[1]==3?(a[1].v!=0):false);
	else if(k[0]==3 && k[1]==3)
	{	gr->Min.z = a[0].v;	gr->Max.z = a[1].v;	gr->RecalcBorder();	}
	else	return 1;
	return 0;
}
void mglc_zrange(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)
		swprintf(out,1024,L"gr->ZRange(%s, %s);", a[0].s, (k[1]==3&&a[1].v!=0)?"true":"false");
	else if(k[0]==3 && k[1]==3)
		swprintf(out,1024,L"gr->Min.z = %g;\tgr->Max.z = %g;\tgr->RecalcBorder();", a[0].v, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"xtick","Set ticks for x-axis","xtick {val [sub]} | tmpl", mgls_xtick, mglc_xtick}
int mgls_xtick(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)
	{
		gr->dx = a[0].v;
		if(k[1]==3)	gr->NSx = int(a[1].v);
		if(k[2]==3)	gr->OrgT.x = a[2].v;
	}
	else if(k[0]==2)
	{
		if(gr->xtt)	delete []gr->xtt;
		gr->xtt = a[0].s[0] ? mgl_str_copy(a[0].s) : 0;
	}
	else	return 1;
	return 0;
}
void mglc_xtick(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		swprintf(out,1024,L"gr->dx = %g;\tgr->NSx = %d;\tgr->OrgT.x = %g;", a[0].v, int(a[1].v),a[2].v);
	else if(k[0]==3 && k[1]==3)	swprintf(out,1024,L"gr->dx = %g;\tgr->NSx = %d;", a[0].v, int(a[1].v));
	else if(k[0]==3)		swprintf(out,1024,L"gr->dx = %g;", a[0].v);
	else if(k[0]==2)
		swprintf(out,1024,L"if(gr->xtt) delete []gr->xtt;\tgr->xtt = '%c' ? mgl_str_copy(\"%s\") : 0;", a[0].s[0], a[0].s);
}
//-----------------------------------------------------------------------------
//	{"ytick","Set ticks for y-axis","ytick {val [sub]} | tmpl", mgls_ytick, mglc_ytick}
int mgls_ytick(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)
	{
		gr->dy = a[0].v;
		if(k[1]==3)	gr->NSy = int(a[1].v);
		if(k[2]==3)	gr->OrgT.y = a[2].v;
	}
	else if(k[0]==2)
	{
		if(gr->ytt)	delete []gr->ytt;
		gr->ytt = a[0].s[0] ? mgl_str_copy(a[0].s) : 0;
	}
	else	return 1;
	return 0;
}
void mglc_ytick(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		swprintf(out,1024,L"gr->dy = %g;\tgr->NSy = %d;\tgr->OrgT.y = %g;", a[0].v, int(a[1].v),a[2].v);
	else if(k[0]==3 && k[1]==3)	swprintf(out,1024,L"gr->dy = %g;\tgr->NSy = %d;", a[0].v, int(a[1].v));
	else if(k[0]==3)		swprintf(out,1024,L"gr->dy = %g;", a[0].v);
	else if(k[0]==2)
		swprintf(out,1024,L"if(gr->ytt) delete []gr->ytt;\tgr->ytt = '%c' ? mgl_str_copy(\"%s\") : 0;", a[0].s[0], a[0].s);
}
//-----------------------------------------------------------------------------
//	{"ztick","Set ticks for z-axis","ztick {val [sub]} | tmpl", mgls_ztick, mglc_ztick}
int mgls_ztick(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)
	{
		gr->dz = a[0].v;
		if(k[1]==3)	gr->NSz = int(a[1].v);
		if(k[2]==3)	gr->OrgT.z = a[2].v;
	}
	else if(k[0]==2)
	{
		if(gr->ztt)	delete []gr->ztt;
		gr->ztt = a[0].s[0] ? mgl_str_copy(a[0].s) : 0;
	}
	else	return 1;
	return 0;
}
void mglc_ztick(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		swprintf(out,1024,L"gr->dz = %g;\tgr->NSz = %d;\tgr->OrgT.z = %g;", a[0].v, int(a[1].v),a[2].v);
	else if(k[0]==3 && k[1]==3)	swprintf(out,1024,L"gr->dz = %g;\tgr->NSz = %d;", a[0].v, int(a[1].v));
	else if(k[0]==3)		swprintf(out,1024,L"gr->dz = %g;", a[0].v);
	else if(k[0]==2)
		swprintf(out,1024,L"if(gr->ztt) delete []gr->ztt;\tgr->ztt = '%c' ? mgl_str_copy(\"%s\") : 0;", a[0].s[0], a[0].s);
}
//-----------------------------------------------------------------------------
//	{"zoom","Zoom in/out plot","zoom x1 y1 x2 y2", mgls_zoom, mglc_zoom}
int mgls_zoom(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)
		gr->Zoom(a[0].v, a[1].v, a[2].v, a[3].v);
	else	return 1;
	return 0;
}
void mglc_zoom(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)
		swprintf(out,1024,L"gr->Zoom(%g, %g, %g, %g);", a[0].v, a[1].v, a[2].v, a[3].v);
}
//-----------------------------------------------------------------------------
//	{"error","Draw error boxes",0, mgls_error, mglc_error}
int mgls_error(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		gr->Error(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),
			k[4]==2?a[4].s:0, k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==1 && k[2]==1)
		gr->Error(*(a[0].d),*(a[1].d),*(a[2].d), k[3]==2?a[3].s:0, k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==1)
		gr->Error(*(a[0].d),*(a[1].d), k[2]==2?a[2].s:0, k[3]==3?a[3].v:NAN);
	else	return 1;
	return 0;
}
void mglc_error(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		swprintf(out,1024,L"gr->Error(%s, %s, %s, %s, \"%s\", %g);",  a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==1 && k[2]==1)
		swprintf(out,1024,L"gr->Error(%s, %s, %s, \"%s\", %g);",  a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==1)
		swprintf(out,1024,L"gr->Error(%s, %s, \"%s\", %g);",  a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"extend","Extend data array",0, mgls_extend, mglc_extend}
int mgls_extend(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3)	a[0].d->Extend(int(a[1].v),k[2]==3?int(a[2].v):0);
	else	return 1;
	return 0;
}
void mglc_extend(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3)
		swprintf(out,1024,L"%s.Extend(%d, %d);", a[0].s, int(a[1].v), k[2]==3?int(a[2].v):0);
}
//-----------------------------------------------------------------------------
//	{"info","Print information about data","info var", mgls_info, mglc_info}
int mgls_info(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1)	a[0].d->PrintInfo(gr->Message);
	else	return 1;
	return 0;
}
void mglc_info(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1)	swprintf(out,1024,L"%s.PrintInfo(gr->Message);", a[0].s);
}
//-----------------------------------------------------------------------------
//	{"integrate","Integrate data","integrate var dir", mgls_integrate, mglc_integrate}
int mgls_integrate(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	a[0].d->Integral(a[1].s);
	else	return 1;
	return 0;
}
void mglc_integrate(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	swprintf(out,1024,L"%s.Integral(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"inplot","Set position of plot in picture",0, mgls_inplot, mglc_inplot}
int mgls_inplot(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)	gr->InPlot(a[0].v,a[1].v,a[2].v,a[3].v);
	else	return 1;
	return 0;
}
void mglc_inplot(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)
		swprintf(out,1024,L"gr->InPlot(%g, %g, %g, %g);", a[0].v, a[1].v, a[2].v, a[3].v);
}
//-----------------------------------------------------------------------------
//	{"pipe","Draw flow pipes for vector field",0, mgls_pipe, mglc_pipe}
int mgls_pipe(mglGraph *gr, long n, mglArg *a, int k[10])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		gr->Pipe(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:0.05,k[4]==3?int(a[4].v):5,
			true,k[5]==3?a[5].v:NAN);
	else if(i==4)
		gr->Pipe(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,k[5]==3?a[5].v:0.05,
			k[6]==3?int(a[6].v):5,true,k[7]==3?a[7].v:NAN);
	else if(i==3)
		gr->Pipe(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?a[4].v:0.05,k[5]==3?int(a[5].v):3);
	else if(i==6)
		gr->Pipe(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),*(a[5].d),
			k[6]==2?a[6].s:0,k[7]==3?a[7].v:0.05,k[8]==3?int(a[8].v):3);
	else	return 1;
	return 0;
}
void mglc_pipe(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		swprintf(out,1024,L"gr->Pipe(%s, %s, \"%s\", %g, %d, true, %g);",  a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:0.05, k[4]==3?int(a[4].v):5, k[5]==3?a[5].v:NAN);
	else if(i==4)
		swprintf(out,1024,L"gr->Pipe(%s, %s, %s, %s, \"%s\", %g, %d, true, %g);",  a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:0.05, k[6]==3?int(a[6].v):5, k[7]==3?a[7].v:NAN);
	else if(i==3)
		swprintf(out,1024,L"gr->Pipe(%s, %s, %s, \"%s\", %g, %d);",  a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:0.05, k[5]==3?int(a[5].v):3);
	else if(i==6)
		swprintf(out,1024,L"gr->Pipe(%s, %s, %s, %s, %s, %s, \"%s\", %g, %d);",  a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, k[6]==2?a[6].s:"", k[7]==3?a[7].v:0.05, k[8]==3?int(a[8].v):3);
}
//-----------------------------------------------------------------------------
//	{"origin","Set axis origin",0, mgls_origin, mglc_origin}
int mgls_origin(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3)	gr->Org = mglPoint(a[0].v,a[1].v,k[2]==3?a[2].v:gr->Min.z);
	else	return 1;
	return 0;
}
void mglc_origin(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3)
		swprintf(out,1024,L"gr->Org = mglPoint(%g, %g, %g);", a[0].v, a[1].v, k[2]==3?a[2].v:0);
}
//-----------------------------------------------------------------------------
//	{"plotfactor","Set plotfactor","plotfactor val", mgls_plotfactor, mglc_plotfactor}
int mgls_plotfactor(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)
	{
		gr->PlotFactor = a[0].v<0 ? gr->PlotFactor:a[0].v;
		gr->AutoPlotFactor = (a[0].v<0);
	}
	else	return 1;
	return 0;
}
void mglc_plotfactor(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3)	swprintf(out,1024,L"gr->PlotFactor = %g<0 ? gr->PlotFactor:%g;\tgr->AutoPlotFactor = (%g<0);", a[0].v, a[0].v, a[0].v);
}
//-----------------------------------------------------------------------------
//	{"norm","Normalize data",0, mgls_norm, mglc_norm}
int mgls_norm(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3 && k[2]==3)
		a[0].d->Norm(a[1].v,a[2].v,k[3]==3?a[3].v!=0:false,k[4]==3?int(a[4].v):0);
	else	return 1;
	return 0;
}
void mglc_norm(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3 && k[2]==3)
		swprintf(out,1024,L"%s.Norm(%g, %g, %s, %d);", a[0].s, a[1].v, a[2].v, (k[3]==3&&a[3].v!=0)?"true":"false", k[4]==3?int(a[4].v):0);
}
//-----------------------------------------------------------------------------
//	{"hist","Create histogram (distribution) of data values",0, mgls_hist, mglc_hist}
int mgls_hist(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3 && k[4]==3 && k[5]==3)
		*(a[0].d) = a[1].d->Hist(*(a[2].d),int(a[3].v),a[4].v,a[5].v);
	else if(k[0]==1 && k[1]==1 && k[2]==3 && k[3]==3 && k[4]==3)
		*(a[0].d) = a[1].d->Hist(int(a[2].v),a[3].v,a[4].v);
	else	return 1;
	return 0;
}
void mglc_hist(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3 && k[4]==3 && k[5]==3)
		swprintf(out,1024,L"%s = %s.Hist(%s, %d, %g, %g);", a[0].s, a[1].s, a[2].s, int(a[3].v), a[4].v, a[5].v);
	else if(k[0]==1 && k[1]==1 && k[2]==3 && k[3]==3 && k[4]==3)
		swprintf(out,1024,L"%s = %s.Hist(%d, %g, %g);", a[0].s, a[1].s, int(a[2].v), a[3].v, a[4].v);
}
//-----------------------------------------------------------------------------
//	{"mirror","Mirror data at some direction",0, mgls_mirror, mglc_mirror}
int mgls_mirror(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	a[0].d->Mirror(a[1].s);
	else	return 1;
	return 0;
}
void mglc_mirror(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==2)	swprintf(out,1024,L"%s.Mirror(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"new","Create new data","new var nx [ny nz]", mgls_new, mglc_new}
int mgls_new(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3)	a[0].d->Create(int(a[1].v), k[2]==3?int(a[2].v):1, k[3]==3?int(a[3].v):1);
	else	return 1;
	return 0;
}
void mglc_new(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3)
		swprintf(out,1024,L"%s.Create(%d, %d, %d);", a[0].s, int(a[1].v), k[2]==3?int(a[2].v):1, k[3]==3?int(a[3].v):1);
}
//-----------------------------------------------------------------------------
//	{"var","Create new 1D data and fill it in range","var var nx x1 x2", mgls_var, mglc_var}
int mgls_var(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3 && k[2]==3 && k[3]==3)
	{	a[0].d->Create(int(a[1].v));	a[0].d->Fill(a[2].v, a[3].v);	}
	else	return 1;
	return 0;
}
void mglc_var(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3 && k[2]==3 && k[3]==3)
	swprintf(out,1024,L"%s.Create(%d);\t%s.Fill(%g,%g);",a[0].s, int(a[1].v), a[0].s, a[2].v, a[3].v);
}
//-----------------------------------------------------------------------------
//	{"chdir","Change current directory","chdir dir", mgls_chdir, mglc_chdir}
int mgls_chdir(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==2)	chdir(a[0].s);
	else	return 1;
	return 0;
}
void mglc_chdir(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==2)	swprintf(out,1024,L"chdir(\"%s\");", a[0].s);
}
//-----------------------------------------------------------------------------
//	{"perspective","Set perspective","perspective val", mgls_perspective, mglc_perspective}
int mgls_perspective(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3)	gr->Perspective(a[0].v);
	else	return 1;
	return 0;
}
void mglc_perspective(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3)	swprintf(out,1024,L"gr->Perspective(%g);", a[0].v);
}
//-----------------------------------------------------------------------------
//	{"facex","Draw face perpendicular to x-axis","facex x0 y0 z0 wy wz [fmt]", mgls_facex, mglc_facex}
int mgls_facex(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3 && k[4]==3)
		gr->FaceX(a[0].v, a[1].v, a[2].v, a[3].v, a[4].v, k[5]==2?a[5].s:0, k[6]==3?a[6].v:0, k[7]==3?a[7].v:0);
	else	return 1;
	return 0;
}
void mglc_facex(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3 && k[4]==3)
		swprintf(out,1024,L"gr->FaceX(%g, %g, %g, %g, %g, \"%s\", %g, %g);", a[0].v, a[1].v, a[2].v, a[3].v, a[4].v, k[5]==2?a[5].s:"", k[6]==3?a[6].v:0, k[7]==3?a[7].v:0);
}
//-----------------------------------------------------------------------------
//	{"facey","Draw face perpendicular to y-axis","facex x0 y0 z0 wx wz [fmt]", mgls_facey, mglc_facey}
int mgls_facey(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3 && k[4]==3)
		gr->FaceY(a[0].v, a[1].v, a[2].v, a[3].v, a[4].v, k[5]==2?a[5].s:0, k[6]==3?a[6].v:0, k[7]==3?a[7].v:0);
	else	return 1;
	return 0;
}
void mglc_facey(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3 && k[4]==3)
		swprintf(out,1024,L"gr->FaceY(%g, %g, %g, %g, %g, \"%s\", %g, %g);", a[0].v, a[1].v, a[2].v, a[3].v, a[4].v, k[5]==2?a[5].s:"", k[6]==3?a[6].v:0, k[7]==3?a[7].v:0);
}
//-----------------------------------------------------------------------------
//	{"facez","Draw face perpendicular to z-axis","facex x0 y0 z0 wy wz [fmt]", mgls_facez, mglc_facez}
int mgls_facez(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3 && k[4]==3)
		gr->FaceZ(a[0].v, a[1].v, a[2].v, a[3].v, a[4].v, k[5]==2?a[5].s:0, k[6]==3?a[6].v:0, k[7]==3?a[7].v:0);
	else	return 1;
	return 0;
}
void mglc_facez(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3 && k[4]==3)
		swprintf(out,1024,L"gr->FaceZ(%g, %g, %g, %g, %g, \"%s\", %g, %g);", a[0].v, a[1].v, a[2].v, a[3].v, a[4].v, k[5]==2?a[5].s:"", k[6]==3?a[6].v:0, k[7]==3?a[7].v:0);
}
//-----------------------------------------------------------------------------
//	{"normsl","Normalize data slice by slice",0, mgls_normsl, mglc_normsl}
int mgls_normsl(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3 && k[2]==3)
		a[0].d->NormSl(a[1].v, a[2].v, k[3]==2?a[3].s[0]:'z', k[4]==3?a[4].v!=0:true, k[5]==3?a[5].v!=0:false);
	else	return 1;
	return 0;
}
void mglc_normsl(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==3 && k[2]==3)
		swprintf(out,1024,L"%s.NormSl(%g, %g, '%c', %s, %s);", a[0].s, a[1].v, a[2].v, k[3]==2?a[3].s[0]:'z', (k[4]!=3||a[4].v!=0)?"true":"false", (k[5]==3&&a[5].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"momentum","Get momentum along direction","momentum odat idat how dir", mgls_momentum, mglc_momentum}
int mgls_momentum(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)
		*(a[0].d) = a[1].d->Momentum(k[3]==2?a[3].s[0]:'z', a[2].s);
	else	return 1;
	return 0;
}
void mglc_momentum(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)
		swprintf(out,1024,L"%s = %s.Momentum('%c', \"%s\");", a[0].s, a[1].s, k[3]==2?a[3].s[0]:'z', a[2].s);
}
//-----------------------------------------------------------------------------
//	{"fit","Fit data by formula","fit res {x {y {z}}} a eq var [ini print]", mgls_fit, mglc_fit}
int mgls_fit(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==1&& k[5]==2 && k[6]==2)
		gr->Fit(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), *(a[4].d), a[5].s, a[6].s, (k[7]==1 && a[7].d->nx>=strlen(a[6].s))?a[7].d->a:0, k[8]==3?a[8].v!=0:false);
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2 && k[5]==2)
		gr->Fit(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), a[4].s, a[5].s, (k[6]==1 && a[6].d->nx>=strlen(a[5].s))?a[6].d->a:0, k[7]==3?a[7].v!=0:false);
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2 && k[4]==2)
		gr->Fit(*(a[0].d), *(a[1].d), *(a[2].d), a[3].s, a[4].s, (k[5]==1 && a[5].d->nx>=strlen(a[4].s))?a[5].d->a:0, k[6]==3?a[6].v!=0:false);
	else if(k[0]==1 && k[1]==1 && k[2]==2 && k[3]==2)
		gr->Fit(*(a[0].d), *(a[1].d), a[2].s, a[3].s, (k[4]==1 && a[4].d->nx>=strlen(a[3].s))?a[4].d->a:0, k[5]==3?a[5].v!=0:false);
	else	return 1;
	return 0;
}
void mglc_fit(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==1&& k[5]==2 && k[6]==2)
		swprintf(out,1024,L"gr->Fit(%s, %s, %s, %s, %s, \"%s\", \"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, a[6].s, (k[7]==1 && a[7].d->nx>=strlen(a[6].s))?a[7].s:"NULL", (k[8]==3&&a[8].v!=0)?"true":"false");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2 && k[5]==2)
		swprintf(out,1024,L"gr->Fit(%s, %s, %s, %s, \"%s\", \"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, (k[6]==1 && a[6].d->nx>=strlen(a[5].s))?a[6].s:"NULL", (k[7]==3&&a[7].v!=0)?"true":"false");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2 && k[4]==2)
		swprintf(out,1024,L"gr->Fit(%s, %s, %s, \"%s\", \"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, (k[5]==1 && a[5].d->nx>=strlen(a[4].s))?a[5].s:"NULL", (k[6]==3&&a[6].v!=0)?"true":"false");
	else if(k[0]==1 && k[1]==1 && k[2]==2 && k[3]==2)
		swprintf(out,1024,L"gr->Fit(%s, %s, \"%s\", \"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s, (k[5]==1 && a[5].d->nx>=strlen(a[4].s))?a[5].s:"NULL", (k[6]==3&&a[6].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"fits","Fit data by formula","fits res {x {y {z}}} a s eq var [ini print]", mgls_fits, mglc_fits}
int mgls_fits(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==1 && k[5]==1 && k[6]==2 && k[7]==2)
		gr->FitS(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), *(a[4].d), *(a[5].d), a[6].s, a[7].s, (k[8]==1 && a[8].d->nx>=strlen(a[7].s))?a[8].d->a:0, k[9]==3?a[9].v!=0:false);
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==1 && k[5]==2 && k[6]==2)
		gr->FitS(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), *(a[4].d), a[5].s, a[6].s, (k[7]==1 && a[7].d->nx>=strlen(a[6].s))?a[7].d->a:0, k[8]==3?a[8].v!=0:false);
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2 && k[5]==2)
		gr->FitS(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), a[4].s, a[5].s, (k[6]==1 && a[6].d->nx>=strlen(a[5].s))?a[6].d->a:0, k[7]==3?a[7].v!=0:false);
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2 && k[4]==2)
		gr->FitS(*(a[0].d), *(a[1].d), *(a[2].d), a[3].s, a[4].s, (k[5]==1 && a[5].d->nx>=strlen(a[4].s))?a[5].d->a:0, k[6]==3?a[6].v!=0:false);
	else	return 1;
	return 0;
}
void mglc_fits(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==1 && k[5]==1 && k[6]==2 && k[7]==2)
		swprintf(out,1024,L"gr->FitS(%s, %s, %s, %s, %s, %s, \"%s\", \"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, a[6].s, a[7].s, (k[8]==1 && a[8].d->nx>=strlen(a[7].s))?a[8].s:"NULL", (k[9]==3&&a[9].v!=0)?"true":"false");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==1 && k[5]==2 && k[6]==2)
		swprintf(out,1024,L"gr->FitS(%s, %s, %s, %s, %s, \"%s\", \"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, a[6].s, (k[7]==1 && a[7].d->nx>=strlen(a[6].s))?a[7].s:"NULL", (k[8]==3&&a[8].v!=0)?"true":"false");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2 && k[5]==2)
		swprintf(out,1024,L"gr->FitS(%s, %s, %s, %s, \"%s\", \"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, (k[6]==1 && a[6].d->nx>=strlen(a[5].s))?a[6].s:"NULL", (k[7]==3&&a[7].v!=0)?"true":"false");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2 && k[4]==2)
		swprintf(out,1024,L"gr->FitS(%s, %s, %s, \"%s\", \"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, (k[5]==1 && a[5].d->nx>=strlen(a[4].s))?a[5].s:"NULL", (k[6]==3&&a[6].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"putsfit","Print fitted formula","putsfit x y {z} [pre font size]", mgls_putsfit, mglc_putsfit}
int mgls_putsfit(mglGraph *gr, long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		gr->PutsFit(mglPoint(a[0].v,a[1].v,a[2].v), k[3]==2?a[3].s:0, k[4]==2?a[4].s:0, k[5]==3?a[5].v:-1);
	else if(k[0]==3 && k[1]==3)
		gr->PutsFit(mglPoint(a[0].v,a[1].v), k[2]==2?a[2].s:0, k[3]==2?a[3].s:0, k[4]==3?a[4].v:-1);
	else	return 1;
	return 0;
}
void mglc_putsfit(wchar_t out[1024], long n, mglArg *a, int k[10])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		swprintf(out,1024,L"gr->PutsFit(mglPoint(%g, %g, %g), \"%s\", \"%s\", %g);", a[0].v,a[1].v,a[2].v, k[3]==2?a[3].s:"", k[4]==2?a[4].s:"", k[5]==3?a[5].v:-1);
	else if(k[0]==3 && k[1]==3)
		swprintf(out,1024,L"gr->PutsFit(mglPoint(%g, %g), \"%s\", \"%s\", %g);", a[0].v,a[1].v, k[2]==2?a[2].s:"", k[3]==2?a[3].s:"", k[4]==3?a[4].v:-1);
	
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==1 && k[5]==1 && k[6]==2 && k[7]==2)
		swprintf(out,1024,L"gr->FitS(%s, %s, %s, %s, %s, %s, \"%s\", \"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, a[6].s, a[7].s, (k[8]==1 && a[8].d->nx>=strlen(a[7].s))?a[8].s:"NULL", (k[9]==3&&a[9].v!=0)?"true":"false");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==1 && k[5]==2 && k[6]==2)
		swprintf(out,1024,L"gr->FitS(%s, %s, %s, %s, %s, \"%s\", \"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, a[6].s, (k[7]==1 && a[7].d->nx>=strlen(a[6].s))?a[7].s:"NULL", (k[8]==3&&a[8].v!=0)?"true":"false");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2 && k[5]==2)
		swprintf(out,1024,L"gr->FitS(%s, %s, %s, %s, \"%s\", \"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, (k[6]==1 && a[6].d->nx>=strlen(a[5].s))?a[6].s:"NULL", (k[7]==3&&a[7].v!=0)?"true":"false");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2 && k[4]==2)
		swprintf(out,1024,L"gr->FitS(%s, %s, %s, \"%s\", \"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, (k[5]==1 && a[5].d->nx>=strlen(a[4].s))?a[5].s:"NULL", (k[6]==3&&a[6].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
mglCommand mgls_base_cmd[] = {
	{L"addlegend",L"Add legend entry",L"addlegend txt fmt", mgls_addlegend, mglc_addlegend},
	{L"addto",L"Add data or number",L"addto var|num", mgls_addto, mglc_addto},
	{L"alpha",L"Switch on/off transparency",L"addto [num]", mgls_alpha, mglc_alpha},
	{L"alphadef",L"Set default transparency",L"alphadef num", mgls_alphadef, mglc_alphadef},
	{L"ambient",L"Set ambient light brightness",L"ambient txt fmt", mgls_ambient, mglc_ambient},
	{L"area",L"Draw area plot for 1D data",L"area {xvar} yvar {{zvar}} [fmt num]", mgls_area, mglc_area},
	{L"aspect",L"Set aspect ration",L"aspect numx numy numz", mgls_aspect, mglc_aspect},
	{L"axial",L"Draw surfaces of contour lines rotation",L"axial {xvar yvar} zvar [fmt num]", mgls_axial, mglc_axial},
	{L"axialdir",L"Set axis of rotation",L"axialdir dir", mgls_axialdir, mglc_axialdir},
	{L"axis",L"Setup or draw axis",L"", mgls_axis, mglc_axis},
	{L"ball",L"Draw point (ball)",L"ball posx posy {posz} [fmt]", mgls_ball, mglc_ball},
	{L"bars",L"Draw bars for 1D data",L"bars {xvar} yvar {{zvar}} [fmt num num]", mgls_bars, mglc_bars},
	{L"beam",L"Draw quasioptical beam",L"", mgls_beam, mglc_beam},
	{L"belt",L"Draw belts",L"belt {xvar yvar} zvar [fmt]", mgls_belt, mglc_belt},
	{L"box",L"Draw bounding box",L"box [fmt]", mgls_box, mglc_box},
	{L"boxs",L"Draw boxes",L"boxs {xvar yvar} zvar [fmt]", mgls_boxs, mglc_boxs},
	{L"caxis",L"Set color range",L"caxis c1 c2", mgls_caxis, mglc_caxis},
	{L"chart",L"Draw chart",L"chart var [fmt]", mgls_chart, mglc_chart},
	{L"chdir",L"Change current directory",L"chdir dir", mgls_chdir, mglc_chdir},
	{L"clearlegend",L"Clear legend antries",L"clearlegend", mgls_clearlegend, mglc_clearlegend},
	{L"clf",L"Clear picture",L"clf", mgls_clf, mglc_clf},
	{L"cloud",L"Draw cloud",L"cloud {xvar yvar zvar} avar [fmt]", mgls_cloud, mglc_cloud},
	{L"colorbar",L"Draw colorbar",L"colorbar [fmt pos]", mgls_colorbar, mglc_colorbar},
	{L"cone",L"Draw cone",L"cone avar [fmt pos num]", mgls_cone, mglc_cone},
	{L"cont",L"Draw contour lines",L"cont {vvar} {xvar yvar} zvar [fmt num zpos]", mgls_cont, mglc_cont},
	{L"cont3",L"Draw contour lines for 3D data",L"cont3 {xvar yvar zvar} avar dir [pos fmt num]", mgls_cont3, mglc_cont3},
	{L"conta",L"Draw contour lines at central slices of 3D data",L"conta {xvar yvar zvar} avar [fmt num]", mgls_conta, mglc_conta},
	{L"contf",L"Draw solid contour lines",L"contf {vvar} {xvar yvar} zvar [fmt num zpos]", mgls_contf, mglc_contf},
	{L"contf3",L"Draw solid contour lines for 3D data",L"contf3 {xvar yvar zvar} avar dir [pos fmt num]", mgls_contf3, mglc_contf3},
	{L"contfa",L"Draw solid contour lines at central slices of 3D data",L"contfa {xvar yvar zvar} avar [fmt num]", mgls_contfa, mglc_contfa},
	{L"contfx",L"Draw solid contour lines at x-slice (or x-plane)",L"contfx avar [fmt pos num]", mgls_contfx, mglc_contfx},
	{L"contfy",L"Draw solid contour lines at y-slice (or y-plane)",L"contfy avar [fmt pos num]", mgls_contfy, mglc_contfy},
	{L"contfy",L"Draw solid contour lines at z-slice (or z-plane)",L"contfz avar [fmt pos num]", mgls_contfz, mglc_contfz},
	{L"contx",L"Draw contour lines at x-slice (or x-plane)",L"contx avar [fmt pos num]", mgls_contx, mglc_contx},
	{L"conty",L"Draw contour lines at y-slice (or y-plane)",L"conty avar [fmt pos num]", mgls_conty, mglc_conty},
	{L"contz",L"Draw contour lines at z-slice (or z-plane)",L"contz avar [fmt pos num]", mgls_contz, mglc_contz},
	{L"copy",L"Copy data from another variable",L"copy var1 var2", mgls_copy, mglc_copy},
	{L"crange",L"Set color range",L"crange {var [sym] | c1 c2}", mgls_crange, mglc_crange},
	{L"crop",L"Crop edge of data",L"crop var n1 n2 dir", mgls_crop, mglc_crop},
	{L"crust",L"Draw reconstructed surface for arbitrary data points",L"crust {xvar yvar zvar} | var [fmt]", mgls_crust, mglc_crust},
	{L"cumsum",L"Crop edge of data",L"cumsum var dir", mgls_cumsum, mglc_cumsum},
	{L"curve",L"Draw curve",L"", mgls_curve, mglc_curve},
	{L"cut",L"Setup plot points cutting",L"", mgls_cut, mglc_cut},
	{L"dens",L"Draw density plot",L"dens {xvar yvar} zvar [fmt zpos]", mgls_dens, mglc_dens},
	{L"dens3",L"Draw density plot at slices of 3D data",L"dens3 {xvar yvar zvar} avar dir [pos fmt]", mgls_dens3, mglc_dens3},
	{L"densa",L"Draw density plot at central slices of 3D data",L"densa {xvar yvar zvar} avar [fmt num]", mgls_densa, mglc_densa},
	{L"densx",L"Draw density plot at x-slice (or x-plane)",L"densx avar [fmt pos]", mgls_densx, mglc_densx},
	{L"densy",L"Draw density plot at y-slice (or y-plane)",L"densy avar [fmt pos]", mgls_densy, mglc_densy},
	{L"densz",L"Draw density plot at z-slice (or z-plane)",L"densz avar [fmt pos]", mgls_densz, mglc_densz},
	{L"dew",L"Draw dew plot",L"dew {xvae yvar} uvar vvar [fmt]", mgls_dew, mglc_dew},
	{L"diff",L"Numerically differentiate data",L"diff var dir", mgls_diff, mglc_diff},
	{L"diff2",L"Numerically double differentiate data",L"diff2 var dir", mgls_diff2, mglc_diff2},
	{L"divto",L"Divide by data or number",L"divto var|num", mgls_divto, mglc_divto},
	{L"dots",L"Draw dots for arbitrary data points",L"dots {xvar yvar zvar} | var [fmt]", mgls_dots, mglc_dots},
	{L"drop",L"Draw drop",L"", mgls_drop, mglc_drop},
	{L"error",L"Draw error boxes",L"", mgls_error, mglc_error},
	{L"extend",L"Extend data array",L"", mgls_extend, mglc_extend},
	{L"facex",L"Draw face perpendicular to x-axis",L"facex x0 y0 z0 wy wz [fmt]", mgls_facex, mglc_facex},
	{L"facey",L"Draw face perpendicular to y-axis",L"facex x0 y0 z0 wx wz [fmt]", mgls_facey, mglc_facey},
	{L"facez",L"Draw face perpendicular to z-axis",L"facex x0 y0 z0 wy wz [fmt]", mgls_facez, mglc_facez},
	{L"fall",L"Draw waterfalls",L"fall {xvar yvar} zvar [fmt]", mgls_fall, mglc_fall},
	{L"fill",L"Fill data linearly in range [v1, v2]",L"fill var v1 v2 [dir]", mgls_fill, mglc_fill},
	{L"fit",L"Fit data to formula",L"fit res {x {y {z}}} a eq var [ini print]", mgls_fit, mglc_fit},
	{L"fits",L"Fit data to formula",L"fits res {x {y {z}}} a s eq var [ini print]", mgls_fits, mglc_fits},
	{L"flow",L"Draw flow threads for vector field",L"", mgls_flow, mglc_flow},
	{L"fog",L"Switch on/off fog",L"fog var val [pos]", mgls_fog, mglc_fog},
	{L"font",L"Setup font",L"font fmt [size]", mgls_font, mglc_font},
	{L"grid",L"Draw grid",L"", mgls_grid, mglc_grid},
	{L"grid3",L"Draw grid at slices of 3D data",L"grid3 {xvar yvar zvar} avar dir [pos fmt]", mgls_grid3, mglc_grid3},
	{L"grida",L"Draw grid at central slices of 3D data",L"grida {xvar yvar zvar} avar [fmt num]", mgls_grida, mglc_grida},
	{L"hist",L"Create histogram (distribution) of data values",L"", mgls_hist, mglc_hist},
	{L"info",L"Print information about data",L"info var", mgls_info, mglc_info},
	{L"inplot",L"Set position of plot in picture",L"", mgls_inplot, mglc_inplot},
	{L"integrate",L"Integrate data",L"integrate var dir", mgls_integrate, mglc_integrate},
	{L"legend",L"Draw legend",L"legend {xpos ypos} | pos [fmt size]", mgls_legend, mglc_legend},
	{L"legendbox",L"Switch on/off drawing box in legend",L"legendbox val", mgls_legendbox, mglc_legendbox},
	{L"light",L"Setup light",L"", mgls_light, mglc_light},
	{L"line",L"Draw line",L"", mgls_line, mglc_line},
	{L"linewidth",L"Set default line width",L"linewidth val", mgls_linewidth, mglc_linewidth},
	{L"map",L"Draw mapping plot",L"map {xvar yvar} uvar vvar [fmt]", mgls_map, mglc_map},
	{L"mark",L"Draw mark plot for 1D data",L"mark {xvar} yvar {{zvar}} rvar [fmt num]", mgls_mark, mglc_mark},
	{L"marksize",L"Set size of markers",L"marksize val", mgls_marksize, mglc_marksize},
	{L"max",L"Find maximal value over direction",L"max ovar ivar dir", mgls_max, mglc_max},
	{L"mesh",L"Draw mesh surface",L"mesh {xvar yvar} zvar [fmt]", mgls_mesh, mglc_mesh},
	{L"meshnum",L"Set number of lines in mesh/fall/vect and so on",L"meshnum val", mgls_meshnum, mglc_meshnum},
	{L"min",L"Find minimal value over direction",L"min ovar ivar dir", mgls_min, mglc_min},
	{L"mirror",L"Mirror data at some direction",L"", mgls_mirror, mglc_mirror},
	{L"modify",L"Modify data values by formula",L"modify var formula [var1 var2]", mgls_modify, mglc_modify},
	{L"momentum",L"Get momentum along direction",L"momentum odat idat dir how", mgls_momentum, mglc_momentum},
	{L"multo",L"Multiply by data or number",L"multo var|num", mgls_multo, mglc_multo},
	{L"new",L"Create new data",L"new var nx [ny nz]", mgls_new, mglc_new},
	{L"norm",L"Normalize data",L"", mgls_norm, mglc_norm},
	{L"normsl",L"Normalize data slice by slice",L"", mgls_normsl, mglc_normsl},
	{L"origin",L"Set axis origin",L"", mgls_origin, mglc_origin},
	{L"perspective",L"Set perspective",L"perspective val", mgls_perspective, mglc_perspective},
	{L"pipe",L"Draw flow pipes for vector field",L"", mgls_pipe, mglc_pipe},
	{L"plot",L"Draw usual plot for 1D data",L"plot {xvar} yvar {{zvar}} [fmt num]", mgls_plot, mglc_plot},
	{L"plotfactor",L"Set plotfactor",L"plotfactor val", mgls_plotfactor, mglc_plotfactor},
	{L"putsfit",L"Print fitted formula",L"putsfit x y {z} [pre font size]", mgls_putsfit, mglc_putsfit},
	{L"read",L"Read data from file",L"read var file [nx ny nz]", mgls_read, mglc_read},
	{L"readall",L"Read and join data from several files",L"", mgls_readall, mglc_readall},
	{L"readhdf",L"Read data from HDF5 file",L"readhdf var file id", mgls_readhdf, mglc_readhdf},
	{L"readmat",L"Read data from file with sizes specified in first row",L"readmat var file [dim=2]", mgls_readmat, mglc_readmat},
	{L"rect",L"Draw rectangle",L"", mgls_rect, mglc_rect},
	{L"resize",L"Resize data",L"resize ovar ivar [nx ny nz]", mgls_resize, mglc_resize},
	{L"rotate",L"Rotate plot",L"", mgls_rotate, mglc_rotate},
	{L"rotatetext",L"Set to auto rotate text or not",L"", mgls_rotatetext, mglc_rotatetext},
	{L"save",L"Save data to file",L"save var file", mgls_save, mglc_save},
	{L"savehdf",L"Save data to HDF5 file",L"savehdf var file id", mgls_savehdf, mglc_savehdf},
	{L"set_id",L"Set column id for data",L"set_id var id", mgls_set_id, mglc_set_id},
	{L"setsize",L"Set picture size",L"setsize wval hval", mgls_setsize, mglc_setsize},
	{L"smooth",L"Smooth data",L"smooth var kind dir", mgls_smooth, mglc_smooth},
	{L"sphere",L"Draw sphere",L"", mgls_sphere, mglc_sphere},
	{L"squeeze",L"Squeeze data",L"squeeze var kx [ky kz]", mgls_squeeze, mglc_squeeze},
	{L"stem",L"Draw stem plot for 1D data",L"stem {xvar} yvar {{zvar}} [fmt num]", mgls_stem, mglc_stem},
	{L"step",L"Draw step plot for 1D data",L"step {xvar} yvar {{zvar}} [fmt num]", mgls_step, mglc_step},
	{L"stfa",L"Draw STFA diagram",L"stfa {xvar yvar} uvar vvar num [fmt]", mgls_stfa, mglc_stfa},
	{L"stfad",L"Do STFA transform",L"", mgls_stfad, mglc_stfad},
	{L"subdata",L"Extract sub-array",L"subdata ovar ivar nx [ny nz]", mgls_subdata, mglc_subdata},
	{L"subplot",L"Set position of plot",L"subplot mval nval ival", mgls_subplot, mglc_subplot},
	{L"subto",L"Subtract data or number",L"subto var|num", mgls_subto, mglc_subto},
	{L"sum",L"Find summation over direction",L"sum ovar ivar dir", mgls_sum, mglc_sum},
	{L"surf",L"Draw solid surface",L"surf {xvar yvar} zvar [fmt]", mgls_surf, mglc_surf},
	{L"surf3",L"Draw isosurface for 3D data",L"surf3 {xvar yvar zvar} avar {num} [fmt]", mgls_surf3, mglc_surf3},
	{L"surf3a",L"Draw isosurface for 3D data transpared by other data",L"surf3a {xvar yvar zvar} avar cvar {num} [fmt]", mgls_surf3a, mglc_surf3a},
	{L"surf3c",L"Draw isosurface for 3D data colored by other data",L"surf3c {xvar yvar zvar} avar cvar {num} [fmt]", mgls_surf3c, mglc_surf3c},
	{L"surfa",L"Draw solid surface transpared by other data",L"surfa {xvar yvar} zvar cvar [fmt]", mgls_surfa, mglc_surfa},
	{L"surfc",L"Draw solid surface colored by other data",L"surfc {xvar yvar} zvar cvar [fmt]", mgls_surfc, mglc_surfc},
	{L"swap",L"Swap data (usefull after Fourier transform)",L"swap var dir", mgls_swap, mglc_swap},
	{L"ternary",L"Switch on/off to use ternary axis",L"ternary val", mgls_ternary, mglc_ternary},
	{L"text",L"Draw text at some position or along curve",L"", mgls_text, mglc_text},
	{L"textmark",L"Draw TeX mark at point position",L"", mgls_textmark, mglc_textmark},
	{L"tile",L"Draw horizontal tiles",L"tile {xvar yvar} zvar {rvar} [fmt]", mgls_tile, mglc_tile},
	{L"torus",L"Draw surface of curve rotation",L"torus {zvar} rvar [fmt]", mgls_torus, mglc_torus},
	{L"transform",L"Do integral transform of data",L"transform ovar how rvar ivar", mgls_transform, mglc_transform},
	{L"transforma",L"Do integral transform of data",L"transforma ovar how avar fvar", mgls_transforma, mglc_transforma},
	{L"transparent",L"Switch off transparency",L"transparent val", mgls_transparent, mglc_transparent}, //!!! OLD !!!
	{L"transpose",L"Transpose data array",L"transpose var [dir]", mgls_transpose, mglc_transpose},
	{L"transptype",L"Set type transparency",L"transptype val", mgls_transptype, mglc_transptype},
	{L"triplot",L"Draw surface of triangles",L"", mgls_triplot, mglc_triplot},
	{L"tube",L"Draw curve by tube",L"", mgls_tube, mglc_tube},
	{L"var",L"Create new 1D data and fill it in range",L"var var nx x1 x2", mgls_var, mglc_var},
	{L"vect",L"Draw vector field",L"", mgls_vect, mglc_vect},
	{L"vectc",L"Draw vector field with coloring",L"", mgls_vectc, mglc_vectc},
	{L"xlabel",L"Draw label for x-axis",L"xlabel txt [fnt pos]", mgls_xlabel, mglc_xlabel},
	{L"xrange",L"Set range for x-axis",L"xrange {var [add]} | {x1 x2}", mgls_xrange, mglc_xrange},
	{L"xtick",L"Set ticks for x-axis",L"xtick {val [sub]} | tmpl", mgls_xtick, mglc_xtick},
	{L"ylabel",L"Draw label for y-axis",L"ylabel txt [fnt pos]", mgls_ylabel, mglc_ylabel},
	{L"yrange",L"Set range for y-axis",L"yrange {var [add]} | {x1 x2}", mgls_yrange, mglc_yrange},
	{L"ytick",L"Set ticks for y-axis",L"ytick {val [sub]} | tmpl", mgls_ytick, mglc_ytick},
	{L"zlabel",L"Draw label for z-axis",L"zlabel txt [fnt pos]", mgls_zlabel, mglc_zlabel},
	{L"zoom",L"Zoom in/out plot",L"zoom x1 y1 x2 y2", mgls_zoom, mglc_zoom},
	{L"zrange",L"Set range for z-axis",L"yrange {var [add]} | {x1 x2}", mgls_zrange, mglc_zrange},
	{L"ztick",L"Set ticks for z-axis",L"ztick {val [sub]} | tmpl", mgls_ztick, mglc_ztick},
{L"",0,0,0,0}};
//-----------------------------------------------------------------------------
