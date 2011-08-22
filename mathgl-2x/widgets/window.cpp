/***************************************************************************
 * window.cpp is part of Math Graphic Library
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
#include "mgl/window.h"
//-----------------------------------------------------------------------------
mglCanvasW::mglCanvasW() : mglCanvas()
{
	Setup(true,false,true);	Delay=0.5;
	LoadFunc=0;	FuncPar=0;	DrawFunc=0;
	GG = 0;		NumFig = 0;	CurFig = -1;
}
//-----------------------------------------------------------------------------
mglCanvasW::~mglCanvasW()	{	if(GG) free(GG);	}
//-----------------------------------------------------------------------------
void mglCanvasW::Clf(mglColor Back)	{	if(get(MGL_AUTO_CLF))	mglCanvas::Clf(Back);	}
//-----------------------------------------------------------------------------
void mglCanvasW::SetSize(int w,int h)
{
	if(GG)	free(GG);	GG = 0;
	mglCanvas::SetSize(w,h);
	//	if(Wnd)	Wnd->size(w,h);
}
//-----------------------------------------------------------------------------
void mglCanvasW::EndFrame()
{
	CurFig = CurFrameId-1;
	if(!GG)
	{
		GG = (unsigned char *)malloc(3*Width*Height);
		NumFig = 1;		CurFig = 0;
	}
	else if(CurFig>NumFig-1)
	{
		GG = (unsigned char *)realloc(GG,3*(NumFig+1)*Width*Height);
		NumFig++;
	}
	mglCanvas::EndFrame();
	memcpy(GG + CurFig*Width*Height*3,G,3*Width*Height);
	CurFig++;
}
//-----------------------------------------------------------------------------
void mglCanvasW::SetDrawFunc(int (*draw)(mglBase *gr, void *p), void *par, void (*reload)(void *p))
{
	NumFig=0;	CurFig=0;
	CurFrameId = 0;
	int n = draw ? draw(this,par) : 0;
	if(n<NumFig && n>=0)	NumFig = n;
	DrawFunc = draw;		FuncPar = par;
	LoadFunc = reload;
}
//-----------------------------------------------------------------------------
const unsigned char *mglCanvasW::GetBits()
{
	const unsigned char *g = mglCanvas::GetBits();
	if(GG && NumFig>0 && CurFig<NumFig && CurFig>=0)
		g = GG + CurFig*Width*Height*3;
	return g;
}
//-----------------------------------------------------------------------------
void mglCanvasW::ReLoad()
{	if(LoadFunc)	{	LoadFunc(FuncPar);	Update();	}	}
//-----------------------------------------------------------------------------
void mgl_wnd_set_delay(HMGL gr, mreal dt)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->SetDelay(dt);	}
void mgl_wnd_toggle_alpha(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ToggleAlpha();	}
void mgl_wnd_toggle_light(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ToggleLight();	}
//void mgl_wnd_toggle_zoom(HMGL gr)
//{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ToggleZoom();	}
void mgl_wnd_toggle_rotate(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ToggleRotate();	}
void mgl_wnd_toggle_no(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ToggleNo();	}
void mgl_wnd_update(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->Update();	}
void mgl_wnd_reload(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ReLoad();	}
void mgl_wnd_adjust(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->Adjust();	}
void mgl_wnd_next_frame(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->NextFrame();	}
void mgl_wnd_prev_frame(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->PrevFrame();	}
void mgl_wnd_animation(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->Animation();	}
//-----------------------------------------------------------------------------
void mgl_wnd_set_delay_(uintptr_t *gr, mreal *dt)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->SetDelay(*dt);	}
void mgl_wnd_toggle_alpha_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->ToggleAlpha();	}
void mgl_wnd_toggle_light_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->ToggleLight();	}
//void mgl_wnd_toggle_zoom_(uintptr_t *gr)
//{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
//	if(g)	g->ToggleZoom();	}
void mgl_wnd_toggle_rotate_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->ToggleRotate();	}
void mgl_wnd_toggle_no_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->ToggleNo();	}
void mgl_wnd_update_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->Update();	}
void mgl_wnd_reload_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->ReLoad();	}
void mgl_wnd_adjust_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->Adjust();	}
void mgl_wnd_next_frame_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->NextFrame();	}
void mgl_wnd_prev_frame_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->PrevFrame();	}
void mgl_wnd_animation_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->Animation();	}
//-----------------------------------------------------------------------------
#ifndef HAVE_FLTK
HMGL mgl_create_graph_fltk(int (*)(HMGL gr, void *p), const char *, void *)
{	return NULL;	}
void mgl_fltk_run(){}
//-----------------------------------------------------------------------------
uintptr_t mgl_create_graph_fltk_(const char *title, int l)
{
	char *s = new char[l+1];	memcpy(s,title,l);	s[l]=0;
	uintptr_t t = uintptr_t(mgl_create_graph_fltk(0,s,0));
	delete []s;	return t;
}
void mgl_fltk_run_()	{	mgl_fltk_run();	}
#endif
//-----------------------------------------------------------------------------
#ifndef HAVE_QT
HMGL mgl_create_graph_qt(int (*)(HMGL gr, void *p), const char *, void *)
{	return NULL;	}
void mgl_qt_run(){}
//-----------------------------------------------------------------------------
uintptr_t mgl_create_graph_qt_(const char *title, int l)
{
	char *s = new char[l+1];	memcpy(s,title,l);	s[l]=0;
	uintptr_t t = uintptr_t(mgl_create_graph_qt(0,s,0));
	delete []s;	return t;
}
void mgl_qt_run_()	{	mgl_qt_run();	}
#endif
//-----------------------------------------------------------------------------