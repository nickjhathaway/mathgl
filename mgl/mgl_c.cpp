/* mgl_c.cpp is part of Math Graphic Library
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
#include "mgl/mgl_c.h"
#include "mgl/mgl.h"
#include "mgl/mgl_ab.h"
#include "mgl/mgl_data.h"
//-----------------------------------------------------------------------------
/// Delete mglGraph object. MUST be called for each mgl_create_* call.
void mgl_delete_graph(HMGL gr)
{	if(gr)	delete gr;	}
//-----------------------------------------------------------------------------
/// Create mglData object.
HMDT mgl_create_data()
{	return new mglData();	};
/// Create mglData object with specified sizes.
HMDT mgl_create_data_size(int nx, int ny, int nz)
{	return new mglData(nx,ny,nz);	};
/// Create mglData object from data file.
HMDT mgl_create_data_file(const char *fname)
{	return new mglData(fname);	};
/// Delete mglData object. MUST be called for each mgl_create_data_* call.
void mgl_delete_data(HMDT dat)
{	if(dat)	delete dat;	};
//-----------------------------------------------------------------------------
//		Setup mglGraph
//-----------------------------------------------------------------------------
/// Set color in mglGraph::Pal array at index \a n.
void mgl_set_palette(HMGL gr, const char *colors)
{	if(gr)	gr->SetPalette(colors);	}
/// Set color in mglGraph::Pal array at index \a n.
void mgl_set_pal_color(HMGL gr, int n, float r, float g, float b)
{	if(gr && n<100)	gr->Pal[n] = mglColor(r,g,b);	}
/// Set number of colors in mglGraph::Pal array.
void mgl_set_pal_num(HMGL gr, int num)
{	if(gr && num<100)	gr->NumPal = num;	}
/// Set mglGraph::RotatedText.
void mgl_set_rotated_text(HMGL gr, int rotated)
{	gr->RotatedText = rotated;	}
/// Set mglGraph::Cut.
void mgl_set_cut(HMGL gr, int cut)
{	gr->Cut = cut;	}
/// Set mglGraph::CutMin, mglGraph::CutMax.
void mgl_set_cut_box(HMGL gr, float x1,float y1,float z1,float x2,float y2,float z2)
{	gr->CutMin = mglPoint(x1,y1,z1);	gr->CutMax = mglPoint(x2,y2,z2);	}
/// Set type of transparency mglGraph::TranspType.
void mgl_set_transp_type(HMGL gr, int type)
{	gr->TranspType = type;	}
/// Switch on/off transparency mglGraph::Transparent.
void mgl_set_transp(HMGL gr, int enable)
{	gr->Transparent = enable;	}
/// Set width of rectangles in mglGraph::Bars().
void mgl_set_bar_width(HMGL gr, float width)
{	gr->BarWidth = width;	}
/// Set size of marks mglGraph::BaseLineWidth.
void mgl_set_base_line_width(HMGL gr, float size)
{	gr->BaseLineWidth = size;	}
/// Set size of marks mglGraph::MarkSize.
void mgl_set_mark_size(HMGL gr, float size)
{	gr->MarkSize = size;	}
/// Set size of arrows mglGraph::ArrowSize.
void mgl_set_arrow_size(HMGL gr, float size)
{	gr->ArrowSize = size;	}
/// Set default font size mglGraph::FontSize.
void mgl_set_font_size(HMGL gr, float size)
{	gr->FontSize = size;	}
/// Set default alpha value mglGraph::AlphaDef.
void mgl_set_alpha_default(HMGL gr, float alpha)
{	gr->AlphaDef = alpha;	}
/// Set size of frame in pixels. Normally this function is called internaly.
void mgl_set_size(HMGL gr, int width, int height)
{	gr->SetSize(width,height);	}
/// Set rotation direction
void mgl_set_axial_dir(HMGL gr, char dir)
{	gr->AxialDir = dir;	}
/// Set number of lines in mesh
void mgl_set_meshnum(HMGL gr, int num)
{	gr->MeshNum = num;	}
/// Switch on/off face drawing
void mgl_set_draw_face(HMGL gr, int enable)
{	gr->DrawFace = enable;	}
/// Set color scheme
void mgl_set_scheme(HMGL gr, const char *sch)
{	gr->SetScheme((sch && sch[0]) ? sch : "BbcyrR");	}
/// Set font facename
void mgl_load_font(HMGL gr, const char *name, const char *path)
{	gr->GetFont()->Load(name,path);	}
/// Copy font data from another HMGL object
void mgl_copy_font(HMGL gr, HMGL gr_from)
{	gr->GetFont()->Copy(gr_from->GetFont());	}
/// Restore font data
void mgl_restore_font(HMGL gr)
{	gr->GetFont()->Restore();	}
//-----------------------------------------------------------------------------
//		Export to file
//-----------------------------------------------------------------------------
/// Write the frame in file using PostScript format
void mgl_write_eps(HMGL gr, const char *fname,const char *descr)
{	gr->WriteEPS(fname,descr);	}
/// Write the frame in file using ScalableVectorGraphics format
void mgl_write_svg(HMGL gr, const char *fname,const char *descr)
{	gr->WriteSVG(fname,descr);	}
void mgl_show_image(HMGL gr, const char *viewer, int keep)
{	gr->ShowImage(viewer,keep);	}
//-----------------------------------------------------------------------------
//		Setup frames transparency (alpha) and lightning
//-----------------------------------------------------------------------------
/// Create new frame.
int mgl_new_frame(HMGL gr, int id)
{	return gr->NewFrame(id);	}
/// Finish frame drawing
void mgl_end_frame(HMGL gr)
{	gr->EndFrame();	}
/// Get the number of created frames
int mgl_get_num_frame(HMGL gr)
{	return gr->GetNumFrame();	}
/// Set the transparency on/off.
void mgl_set_alpha(HMGL gr, int enable)
{	gr->Alpha(enable);	}
/// Set the fog distance.
void mgl_set_fog(HMGL gr, float d, float dz)
{	gr->Fog(d,dz);	}
/// Set the using of light on/off.
void mgl_set_light(HMGL gr, int enable)
{	gr->Light(enable);	}
/// Set the using of n-th light on/off.
void mgl_set_light_n(HMGL gr, int n, int enable)
{	gr->Light(n, enable);	}
/// Add white light source.
void mgl_add_light(HMGL gr, int n, float x, float y, float z, char c)
{	gr->Light(n,mglPoint(x,y,z),c,0.5);	}
/// Add a light source with color {r,g,b}.
void mgl_add_light_rgb(HMGL gr, int n, float x, float y, float z, int infty,
						float r, float g, float b,float i)
{	gr->Light(n,mglPoint(x,y,z),mglColor(r,g,b),i,infty);	}
// Set ambient light brightness
void mgl_set_ambbr(HMGL gr, float i)
{	gr->Ambient(i);	}
//-----------------------------------------------------------------------------
//		Scale and rotate
//-----------------------------------------------------------------------------
/// Clear transformation matrix.
void mgl_identity(HMGL gr)
{	gr->Identity();	}
/// Clear up the frame
void mgl_clf(HMGL gr)
{	gr->Clf();	}
/// Clear up the frame
void mgl_clf_rgb(HMGL gr, float r, float g, float b)
{	gr->Clf(mglColor(r,g,b));	}
/// Put further plotting in some region of whole frame surface.
void mgl_subplot(HMGL gr, int nx,int ny,int m)
{	gr->SubPlot(nx,ny,m);	}
/// Put further plotting in some region of whole frame surface.
void mgl_subplot_d(HMGL gr, int nx,int ny,int m,float dx,float dy)
{	gr->SubPlot(nx,ny,m,dx,dy);	}
/// Put further plotting in some region of whole frame surface.
void mgl_inplot(HMGL gr, float x1,float x2,float y1,float y2)
{	gr->InPlot(x1,x2,y1,y2);	}
/// Set aspect ratio for further plotting.
void mgl_aspect(HMGL gr, float Ax,float Ay,float Az)
{	gr->Aspect(Ax,Ay,Az);	}
/// Rotate a further plotting.
void mgl_rotate(HMGL gr, float TetX,float TetZ,float TetY)
{	gr->Rotate(TetX,TetZ,TetY);	}
/// Rotate a further plotting around vector {x,y,z}.
void mgl_rotate_vector(HMGL gr, float Tet,float x,float y,float z)
{	gr->RotateN(Tet,x,y,z);	}
void mgl_perspective(HMGL gr, float val)
{	gr->Perspective(val);	}
//-----------------------------------------------------------------------------
//		Axis functions
//-----------------------------------------------------------------------------
/// Switch on/off ticks tunning and set factor position for tunned ticks.
void mgl_tune_ticks(HMGL gr, int tune, float fact_pos)
{	gr->TuneTicks = tune;	gr->FactorPos = fact_pos;	}
void mgl_set_ticks_dir(HMGL gr, char dir, float d, int ns, float org)
{	gr->SetTicks(dir, d, ns, org);	}
/// Set ticks interval mglGraph::dx, mglGraph::dy, mglGraph::dz.
void mgl_set_ticks(HMGL gr, float DX, float DY, float DZ)
{	gr->dx=DX;	gr->dy=DY;	gr->dz=DZ;	}
/// Set number of subticks mglGraph::NSx, mglGraph::NSy, mglGraph::NSz.
void mgl_set_subticks(HMGL gr, int NX, int NY, int NZ)
{	gr->NSx=NX;	gr->NSy=NY;	gr->NSz=NZ;	}
/// Set the values of mglGraph::Cmin and mglGraph::Cmax
void mgl_set_caxis(HMGL gr, float C1,float C2)
{	gr->CAxis(C1,C2);	}
void mgl_set_crange(HMGL gr, const HMDT a, int add)	{	gr->CRange(*a,add);	}
void mgl_set_xrange(HMGL gr, const HMDT a, int add)	{	gr->XRange(*a,add);	}
void mgl_set_yrange(HMGL gr, const HMDT a, int add)	{	gr->YRange(*a,add);	}
void mgl_set_zrange(HMGL gr, const HMDT a, int add)	{	gr->ZRange(*a,add);	}
/// Safetly set the value for mglGraph::Min, mglGraph::Max and mglGraph::Org members of the class.
void mgl_set_axis(HMGL gr, float x1, float y1, float z1, float x2, float y2, float z2, float x0, float y0, float z0)
{	gr->Axis(mglPoint(x1,y1,z1),mglPoint(x2,y2,z2),mglPoint(x0,y0,z0));	}
/// Safetly set the transformation formulas for coordinate.
void mgl_set_func(HMGL gr, const char *EqX,const char *EqY,const char *EqZ)
{	gr->Axis(EqX,EqY,EqZ);	}
void mgl_set_cutoff(HMGL gr, const char *EqC)
{	gr->CutOff(EqC);	}
/// Draws bounding box outside the plotting volume by default color.
void mgl_box(HMGL gr, int ticks)
{	gr->Box(NC,ticks);	}
/// Draws bounding box outside the plotting volume with color \a c.
void mgl_box_str(HMGL gr, const char *col, int ticks)
{	gr->Box(col,ticks);	}
/// Draws bounding box outside the plotting volume.
void mgl_box_rgb(HMGL gr, float r, float g, float b, int ticks)
{	gr->Box(mglColor(r,g,b),ticks);	}
/// Draw axises with ticks in directions determined by string parameter \a dir.
void mgl_axis(HMGL gr, const char *dir)
{	gr->Axis(dir);	}
/// Draw grid lines perpendicular to direction determined by string parameter \a dir.
void mgl_axis_grid(HMGL gr, const char *dir,const char *pen)
{	gr->Grid(dir,pen);	}
/// Print the label \a text for axis \a dir.
void mgl_label(HMGL gr, char dir, const char *text)
{	gr->Label(dir, text);	}
/// Print the label \a text for axis \a dir.
void mgl_label_ext(HMGL gr, char dir, const char *text, int pos, float size, float shift)
{	gr->Label(dir, text, pos, size, shift);	}
void mgl_set_xtt(HMGL gr, const wchar_t *templ)	{gr->xtt = templ;}
void mgl_set_ytt(HMGL gr, const wchar_t *templ)	{gr->ytt = templ;}
void mgl_set_ztt(HMGL gr, const wchar_t *templ)	{gr->ztt = templ;}
void mgl_set_ctt(HMGL gr, const wchar_t *templ)	{gr->ctt = templ;}
//-----------------------------------------------------------------------------
//		Simple drawing
//-----------------------------------------------------------------------------
/// Draws the red point (ball) at position \a {x,y,z}.
void mgl_ball(HMGL gr, float x,float y,float z)
{	gr->Ball(mglPoint(x,y,z));	}
/// Draws the point (ball) at position \a {x,y,z} with color {r,g,b}.
void mgl_ball_rgb(HMGL gr, float x, float y, float z, float r, float g, float b, float alpha)
{	gr->Ball(x,y,z,mglColor(r,g,b),alpha);	}
/// Draws the point (ball) at position \a p with color \a col.
void mgl_ball_str(HMGL gr, float x, float y, float z, const char *col)
{	gr->Ball(mglPoint(x,y,z),col[0]);	}
/// Draws the line between points with style \a sch.
void mgl_line(HMGL gr, float x1, float y1, float z1, float x2, float y2, float z2, const char *pen,int n)
{	gr->Line(mglPoint(x1,y1,z1),mglPoint(x2,y2,z2),pen,n);	}

void mgl_facex(HMGL gr, float x0, float y0, float z0, float wy, float wz, const char *stl, float dx, float dy)
{	gr->FaceX(x0,y0,z0,wy,wz,stl,dx,dy);	}
void mgl_facey(HMGL gr, float x0, float y0, float z0, float wx, float wz, const char *stl, float dx, float dy)
{	gr->FaceY(x0,y0,z0,wx,wz,stl,dx,dy);	}
void mgl_facez(HMGL gr, float x0, float y0, float z0, float wx, float wy, const char *stl, float dx, float dy)
{	gr->FaceZ(x0,y0,z0,wx,wy,stl,dx,dy);	}
void mgl_curve(HMGL gr, float x1, float y1, float z1, float dx1, float dy1, float dz1, float x2, float y2, float z2, float dx2, float dy2, float dz2, const char *pen,int n)
{	gr->Curve(mglPoint(x1,y1,z1), mglPoint(dx1,dy1,dz1), mglPoint(x2,y2,z2), mglPoint(dx2,dy2,dz2), pen, n);	}

/// Print string \a str in position \a p with font size \a size.
void mgl_puts(HMGL gr, float x, float y, float z,const char *text)
{	gr->Puts(mglPoint(x,y,z),text);	}
/// Print string \a str in position \a p with font size \a size.
void mgl_putsw(HMGL gr, float x, float y, float z,const wchar_t *text)
{	gr->Putsw(mglPoint(x,y,z),text);	}
/// Print string \a str in position \a p along direction \a d with font size \a size.
void mgl_puts_dir(HMGL gr, float x, float y, float z, float dx, float dy, float dz, const char *text, float size)
{	gr->Puts(mglPoint(x,y,z), mglPoint(dx,dy,dz), text, 't', size);	}
/// Print string \a str in position \a p along direction \a d with font size \a size.
void mgl_putsw_dir(HMGL gr, float x, float y, float z, float dx, float dy, float dz, const wchar_t *text, float size)
{	gr->Putsw(mglPoint(x,y,z), mglPoint(dx,dy,dz), text, 't', size);	}
/// Print unrotated string \a str in position \a p with font size \a size.
void mgl_text(HMGL gr, float x, float y, float z,const char *text)
{	gr->Text(mglPoint(x,y,z),text);	}
/// Print string \a str in position \a p with font size \a size.
void mgl_puts_ext(HMGL gr, float x, float y, float z,const char *text,const char *font,float size,char dir)
{	gr->Puts(mglPoint(x,y,z),text,font,size,dir);	}
/// Print string \a str in position \a p with font size \a size.
void mgl_putsw_ext(HMGL gr, float x, float y, float z,const wchar_t *text,const char *font,float size,char dir)
{	gr->Putsw(mglPoint(x,y,z),text,font,size,dir);	}
/// Print unrotated string \a str in position \a p with font size \a size.
void mgl_text_ext(HMGL gr, float x, float y, float z,const char *text,const char *font,float size,char dir)
{	gr->Text(mglPoint(x,y,z),text,font,size,dir);	}
/// Draw colorbar at edge of axis
void mgl_colorbar(HMGL gr, const char *sch,int where)
{	gr->Colorbar(sch,where);	}
/// Plot data depending on its dimensions and \a type parameter
void mgl_simple_plot(HMGL gr, const HMDT a, int type, const char *sch)
{	if(gr && a)	gr->SimplePlot(*a,type,sch);	}
//-----------------------------------------------------------------------------
//		Plot legend drawing
//-----------------------------------------------------------------------------
/// Add string to legend
void mgl_add_legend(HMGL gr, const char *text,const char *style)
{	if(gr)	gr->AddLegend(text,style);	}
/// Add string to legend
void mgl_add_legendw(HMGL gr, const wchar_t *text,const char *style)
{	if(gr)	gr->AddLegend(text,style);	}
/// Clear saved legend string
void mgl_clear_legend(HMGL gr)
{	if(gr)	gr->ClearLegend();	}
/// Draw legend of accumulated strings at position (x, y) by \a font with \a size
void mgl_legend_xy(HMGL gr, float x, float y, const char *font, float size, float llen)
{	if(gr)	gr->Legend(x, y, font, size, llen);	}
/// Draw legend of accumulated strings by \a font with \a size
void mgl_legend(HMGL gr, int where, const char *font, float size, float llen)
{	if(gr)	gr->Legend(where, font, size, llen);	}
void mgl_set_legend_box(HMGL gr, int enable)
{	gr->LegendBox = enable;	}
//-----------------------------------------------------------------------------
const unsigned char *mgl_get_rgb(HMGL graph)
{
	mglGraphAB *g = dynamic_cast<mglGraphAB *>(graph);
	return g ? g->GetBits():0;
}
const unsigned char *mgl_get_rgba(HMGL graph)
{
	mglGraphAB *g = dynamic_cast<mglGraphAB *>(graph);
	return g ? g->GetRGBA():0;
}
int mgl_get_width(HMGL graph)
{
	mglGraphAB *g = dynamic_cast<mglGraphAB *>(graph);
	return g ? g->GetWidth():0;
}
int mgl_get_height(HMGL graph)
{
	mglGraphAB *g = dynamic_cast<mglGraphAB *>(graph);
	return g ? g->GetHeight():0;
}
void mgl_update(HMGL graph)
{
	mglGraphAB *g = dynamic_cast<mglGraphAB *>(graph);
	if(g)	g->Update();
}
//-----------------------------------------------------------------------------
float mgl_data_get_value(const HMDT d, int i, int j, int k)
{	return	d->a[i+d->nx*(j+d->ny*k)];	}
void mgl_data_set_value(HMDT d, float v, int i, int j, int k)
{	d->a[i+d->nx*(j+d->ny*k)] = v;	}
//-----------------------------------------------------------------------------
/// Zoom in/out a part of picture
void mgl_set_zoom(HMGL gr, float x1, float y1, float x2, float y2)
{	gr->Zoom(x1,y1,x2,y2);	}
void mgl_set_plotfactor(HMGL gr, float val)
{
	if(val>0)	{	gr->PlotFactor = val;	gr->AutoPlotFactor=false;	}
	else	gr->AutoPlotFactor=true;
}
void mgl_set_axis_3d(HMGL gr, float x1, float y1, float z1, float x2, float y2, float z2)
{	gr->Axis(mglPoint(x1,y1,z1),mglPoint(x2,y2,z2));	}
void mgl_set_axis_2d(HMGL gr, float x1, float y1, float x2, float y2)
{	gr->Axis(mglPoint(x1,y1),mglPoint(x2,y2));	}
void mgl_set_origin(HMGL gr, float x0, float y0, float z0)
{	gr->Org = mglPoint(x0,y0,z0);	}
void mgl_set_tick_origin(HMGL gr, float x0, float y0, float z0)
{	gr->OrgT = mglPoint(x0,y0,z0);	}
//-----------------------------------------------------------------------------
void mgl_title(HMGL gr, const char *text, const char *fnt, float size)
{	gr->Title(text, fnt,size);	}
void mgl_titlew(HMGL gr, const wchar_t *text, const char *fnt, float size)
{	gr->Title(text, fnt,size);	}
//-----------------------------------------------------------------------------
void mgl_set_ternary(HMGL gr, int enable)
{	gr->Ternary(enable);	}
void mgl_sphere(HMGL gr, float x, float y, float z, float r, const char *stl)
{	gr->Sphere(mglPoint(x,y,z),r,stl);	}
void mgl_drop(HMGL gr, float x1, float y1, float z1, float x2, float y2, float z2, float r, const char *stl, float shift, float ap)
{	gr->Drop(mglPoint(x1,y1,z1),mglPoint(x2,y2,z2),r,stl,shift,ap);	}
void mgl_cone(HMGL gr, float x1, float y1, float z1, float x2, float y2, float z2, float r1, float r2, const char *stl, int edge)
{	gr->Cone(mglPoint(x1,y1,z1),mglPoint(x2,y2,z2),r1,r2,stl,edge);	}
void mgl_set_def_param(HMGL gr)	{	gr->DefaultPlotParam();	}
void mgl_set_font_def(HMGL gr, const char *fnt)
{	strncpy(gr->FontDef, fnt, 31);	}
void mgl_flush(HMGL gr)	{	gr->Flush();	}
void mgl_data_fill_eq(HMGL gr, HMDT dat, const char *eq, const HMDT vdat, const HMDT wdat)
{	dat->Fill(eq, gr->Min, gr->Max, vdat, wdat);	}
void mgl_set_ticks_vals(HMGL gr, char dir, int n, float *val, const char **lbl)
{	gr->SetTicksVal(dir,n,val,lbl);	}
//-----------------------------------------------------------------------------
#include <stdint.h>
int mgl_fortran_func(HMGL gr, void *f)
{
	typedef int (*func_draw)(uintptr_t *gr);
	func_draw draw = func_draw(f);
	uintptr_t p = uintptr_t(gr);
	int res = draw(&p);	return res;
}
//-----------------------------------------------------------------------------
