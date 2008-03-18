#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <mgl/mgl_glut.h>

int sample(mglGraph *gr, void *)
{
	gr->NewFrame();				// first frame
	gr->SubPlot(2,2,0);			// just new axis without rotation and aspects
	gr->Box();
	gr->SubPlot(2,2,1);			// new axis with aspect and rotation
	gr->Aspect(1,1,2);
	gr->Rotate(60,120);
	gr->Box();
	gr->SubPlot(2,2,2);			// aspect in other direction
	gr->Aspect(1,2,2);
	gr->Rotate(60,120);
	gr->Box();
	gr->SubPlot(2,2,3);			// rotation before aspect. INCORRECT !!!
	gr->Rotate(60,120);
	gr->Aspect(1,1,2);
	gr->Box();
	gr->EndFrame();				// end of first frame
	gr->NewFrame();				// second frame
	gr->Clf();					// clear all
	gr->Rotate(60,120);
	gr->Box();
	gr->InPlot(0.6,1,0.6,1);	// new axis in upper right corner
	gr->Rotate(60,120);
	gr->Box();
	gr->EndFrame();				// end of second frame
	return 2;
}

int sample_1(mglGraph *gr, void *)
{
	mglData  a(50,15),d(50),d1(50),d2(50);
	d.Modify("0.7*sin(2*pi*x) + 0.5*cos(3*pi*x) + 0.2*sin(pi*x)");
	d1.Modify("cos(2*pi*x)");
	d2.Modify("sin(2*pi*x)");
	a.Modify("pow(x,4*y)");
	
	gr->NewFrame();
	gr->Box();	gr->Axis("xy");	gr->Label('x',"x");	gr->Label('y',"y");
	gr->Text(mglPoint(0,1.2,1),"Simple plot of one curve");
	gr->Plot(d);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Box();	gr->Axis("xy");	gr->Label('x',"x");	gr->Label('y',"y");
	gr->Text(mglPoint(0,1.2,1),"Three curves with manual styles");
	gr->Plot(d,"b");
	gr->Plot(d1,"ri");
	gr->Plot(d2,"m|^");
	gr->Plot(d,"l o");
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Box();	gr->Axis("xy");	gr->Label('x',"x");	gr->Label('y',"y");
	gr->Text(mglPoint(0,1.2,1),"Three curves with automatic styles");
	gr->Plot(d);
	gr->Plot(d1);
	gr->Plot(d2);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Box();	gr->Axis("xy");	gr->Label('x',"x");	gr->Label('y',"y");
	gr->Text(mglPoint(0,1.2,1),"Curves from matrix");
	gr->Plot(a);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Box();	gr->Axis("xy");	gr->Label('x',"x");	gr->Label('y',"y");
	gr->Text(mglPoint(0,1.2,1),"Parametrical curves in 2D");
	gr->Plot(d1,d2,"b");
	gr->Plot(d1,d,"ri");
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Parametrical curves in 3D");
	gr->Rotate(60,40);
	gr->Box();	gr->Axis();	gr->Label('x',"x");	gr->Label('y',"y");	gr->Label('z',"z");
	gr->Plot(d1,d2,d,"b");
	gr->EndFrame();
	
	gr->NewFrame();
	gr->SubPlot(2,2,0);
	gr->Box();	gr->Axis("xy");	gr->Label('x',"x");	gr->Label('y',"y");
	gr->Text(mglPoint(0,1.2,1),"Area plot");
	gr->Area(d);
	gr->SubPlot(2,2,1);
	gr->Box();	gr->Axis("xy");	gr->Label('x',"x");	gr->Label('y',"y");
	gr->Text(mglPoint(0,1.2,1),"Step plot");
	gr->Step(d);
	gr->SubPlot(2,2,2);
	gr->Box();	gr->Axis("xy");	gr->Label('x',"x");	gr->Label('y',"y");
	gr->Text(mglPoint(0,1.2,1),"Stem plot");
	gr->Stem(d);
	gr->SubPlot(2,2,3);
	gr->Box();	gr->Axis("xy");	gr->Label('x',"x");	gr->Label('y',"y");
	gr->Text(mglPoint(0,1.2,1),"Bars plot");
	gr->Bars(d);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->SubPlot(2,2,0);
	gr->Text(mglPoint(0,1.2,1),"Area plot in 3D");
	gr->Rotate(60,40);
	gr->Box();	gr->Axis();	gr->Label('x',"x");	gr->Label('y',"y");
	gr->Area(d1,d2,d);
	gr->SubPlot(2,2,1);
	gr->Text(mglPoint(0,1.2,1),"Step plot in 3D");
	gr->Rotate(60,40);
	gr->Box();	gr->Axis();	gr->Label('x',"x");	gr->Label('y',"y");
	gr->Step(d1,d2,d);
	gr->SubPlot(2,2,2);
	gr->Text(mglPoint(0,1.2,1),"Stem plot in 3D");
	gr->Rotate(60,40);
	gr->Box();	gr->Axis();	gr->Label('x',"x");	gr->Label('y',"y");
	gr->Stem(d1,d2,d);
	gr->SubPlot(2,2,3);
	gr->Text(mglPoint(0,1.2,1),"Bars plot in 3D");
	gr->Rotate(60,40);
	gr->Box();	gr->Axis();	gr->Label('x',"x");	gr->Label('y',"y");
	gr->Bars(d1,d2,d);
	gr->EndFrame();
	return gr->GetNumFrame();
}

int sample_2(mglGraph *gr, void *)
{
	mglData  a(50,50),b(50,50),c(50,50),d(50,50),m(50,50),c1(50,50,7),
		d1(50),d2(50);
	d1.Modify("2*x*x-1");	d2.Modify("sin(pi*(x-0.5))");
	a.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	b.Modify("(2+sin(2*pi*x))*cos(2*pi*y)/3");
	c.Modify("(2+sin(2*pi*x))*sin(2*pi*y)/3");
	d.Modify("cos(2*pi*x)");
	m.Modify("cos(pi*x)");
	c1.Modify("(2-z)*(2*x-1)^2 + (z+1)*(2*y-1)^2");
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Simple surface");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Surf(a,"BbcyrR");
//	gr->Colorbar();
	gr->EndFrame();
	
	gr->NewFrame();
	gr->SubPlot(2,2,0);
	gr->Text(mglPoint(0,1.2,1),"Gray color scheme 'kw'");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Surf(a,"kw");
	gr->SubPlot(2,2,1);
	gr->Text(mglPoint(0,1.2,1),"Hot color scheme 'wyrRk'");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Surf(a,"wyrRk");
	gr->SubPlot(2,2,2);		
	gr->Text(mglPoint(0,1.2,1),"Along coordiantes 'rgbd'");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Surf(a,"rgbd");
	gr->SubPlot(2,2,3);		
	gr->Text(mglPoint(0,1.2,1),"Bicolor scheme 'BbwrR'");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Surf(a,"BbwrR");
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Box();	gr->Axis();	
	gr->Text(mglPoint(0,1.2,1),"Density plot");
	gr->Dens(a,"BbcyrR");
	gr->InPlot(0.6,1,0.6,1);	// new axis in upper right corner
	gr->Box();	gr->Axis();	
	gr->Text(mglPoint(0,1.2,1),"... with bicolor");
	gr->Dens(a,"BbwrR");
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Mesh lines (previous scheme by default)");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Mesh(a);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Surface of boxes");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Boxs(a,"BbcyrR");
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Contour plot");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Cont(a);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Contour isosurface y-rotation");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Axial(a,":y");
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Contour isosurface x-rotation");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Axial(a,"x");
	a.Transpose();
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Surface and contours");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Cont(a,"BbcyrR",7,gr->Min.z);
	gr->Surf(a);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Parametrical surface (1)");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Mesh(d1,d2,b);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Parametrical surface (vase)");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Surf(b,c,m);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Parametrical surface (torus)");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Surf(b,c,d);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Contours for 3-tensor");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Cont(c1);
	gr->EndFrame();
	return gr->GetNumFrame();
}

int sample_3(mglGraph *gr, void *)
{
	mglData  a(50,50,50),b(50,50,50), c(50,50,50),d(50,50,50), d1(50),d2(50),d3(50);
	d1.Modify("cos(2*pi*x)");
	d2.Modify("sin(2*pi*x)");
	d3.Modify("2*x*x-1");
	a.Modify("(-2*((2*x-1)^2 + (2*y-1)^2 + (2*z-1)^4 - (2*z-1)^2 + 0.1))");
	b.Modify("-2*((2*x-1)^2 + (2*y-1)^2)");
	c.Modify("exp(-8*((2*x-1)^2+(2*y-1)^2)/(1+z*z*4))");
	d.Modify("cos(32*z*((2*x-1)^2+(2*y-1)^2)/(1+z*z*4))");
//	c.Modify("0.5*sin(2*pi*x)*sin(3*pi*y)*sin(3*pi*z) + 0.5*cos(4*pi*(x*y*z-(x*y+y*z+z*x)/2))");

	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Isosurface (try lightning!)");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Surf3(a);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Isosurface with 'rgbd' scheme");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Surf3(a,"rgbd");
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Cloud plot (switch alpha !!!)");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->CloudQ(a,"BbcyrR");
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Density at central slices");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->DensA(a);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Text(mglPoint(0,1.2,1),"Contours at central slices");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->ContA(a);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->SubPlot(2,1,0);
	gr->Text(mglPoint(0,1.2,1),"Gauss difraction");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->CAxis(0,1);
	gr->Surf3(0.5,c,"g");
	gr->SubPlot(2,1,1);
	gr->Text(mglPoint(0,1.2,1),"and its phase");
	gr->Rotate(40,60);
	gr->Box();	gr->Axis();	
	gr->Surf3A(sin(M_PI/4),d,c,"q");
	gr->Surf3A(-sin(M_PI/4),d,c,"q");
	gr->EndFrame();

	return gr->GetNumFrame();
}

int sample_d(mglGraph *gr, void *)
{
	mglData  a(50,50),b(50,50);
	mglData cx(50,50,50),cy(50,50,50),cz(50,50,50);
	a.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	b.Modify("0.6*cos(2*pi*x)*cos(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	cx.Modify("0.01*(x-0.3)/pow((x-0.3)^2+(y-0.5)^2+(z-0.5)^2,1.5) - 0.01*(x-0.7)/pow((x-0.7)^2+(y-0.5)^2+(z-0.5)^2,1.5)");
	cy.Modify("0.01*(y-0.5)/pow((x-0.3)^2+(y-0.5)^2+(z-0.5)^2,1.5) - 0.01*(y-0.5)/pow((x-0.7)^2+(y-0.5)^2+(z-0.5)^2,1.5)");
	cz.Modify("0.01*(z-0.5)/pow((x-0.3)^2+(y-0.5)^2+(z-0.5)^2,1.5) - 0.01*(z-0.5)/pow((x-0.7)^2+(y-0.5)^2+(z-0.5)^2,1.5)");

	gr->NewFrame();
	gr->Box();	gr->Axis("xy");	
	gr->Text(mglPoint(0,1.2,1),"Vector field (color ~ \\sqrt{a^2})","rC",8);
	gr->VectC(a,b);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Box();	gr->Axis("xy");	
	gr->Text(mglPoint(0,1.2,1),"Vector field (length ~ \\sqrt{a^2})","rC",8);
	gr->Vect(a,b);
	gr->EndFrame();
	
	gr->NewFrame();
	gr->Box();	gr->Axis("xy");	
	gr->Text(mglPoint(0,1.2,1),"Flow chart (blue - source)","rC",8);
	gr->Flow(a,b);
	gr->EndFrame();

	return gr->GetNumFrame();
}

int main(int argc,char **argv)
{
	mglGraphGLUT gr;
	char key = 0;
	if(argc>1 && argv[1][0]!='-')	key = argv[1][0];
	else	printf("You may specify argument '1', '2', '3' or 'd' for viewing examples of 1d, 2d, 3d or dual plotting");
	switch(key)
	{
	case '1':	gr.Window(0,0,sample_1,"1D plots",NULL);	break;
	case '2':	gr.Window(0,0,sample_2,"2D plots",NULL);	break;
	case '3':	gr.Window(0,0,sample_3,"3D plots",NULL);	break;
	case 'd':	gr.Window(0,0,sample_d,"Dual plots",NULL);	break;
	default:	gr.Window(0,0,sample,"Subplot, and rotations",NULL);	break;
	}
	return 0;
}
