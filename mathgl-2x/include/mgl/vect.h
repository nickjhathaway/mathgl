/***************************************************************************
 * vect.h is part of Math Graphic Library
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
#ifndef _MGL_VECT_H_
#define _MGL_VECT_H_
#include <stdint.h>
#include "mgl/base.h"
/*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
void mgl_traj_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch,float zVal,float len);
void mgl_traj_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch,float len);

void mgl_vect_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch,float zVal,int flag);
void mgl_vect_2d(HMGL gr, HCDT ax, HCDT ay, const char *sch,float zVal,int flag);
void mgl_vect_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch,int flag);
void mgl_vect_3d(HMGL gr, HCDT ax, HCDT ay, HCDT az, const char *sch,int flag);

void mgl_flow_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch, int num, float zVal);
void mgl_flow_2d(HMGL gr, HCDT ax, HCDT ay, const char *sch, int num, float zVal);
void mgl_flow_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch, int num);
void mgl_flow_3d(HMGL gr, HCDT ax, HCDT ay, HCDT az, const char *sch, int num);

void mgl_flowp_xy(HMGL gr, float x0, float y0, float z0, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch);
void mgl_flowp_2d(HMGL gr, float x0, float y0, float z0, HCDT ax, HCDT ay, const char *sch);
void mgl_flowp_xyz(HMGL gr, float x0, float y0, float z0, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch);
void mgl_flowp_3d(HMGL gr, float x0, float y0, float z0, HCDT ax, HCDT ay, HCDT az, const char *sch);

void mgl_pipe_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch, float r0, int num, float zVal);
void mgl_pipe_2d(HMGL gr, HCDT ax, HCDT ay, const char *sch, float r0, int num, float zVal);
void mgl_pipe_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch, float r0, int num);
void mgl_pipe_3d(HMGL gr, HCDT ax, HCDT ay, HCDT az, const char *sch, float r0, int num);

void mgl_grad_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ph, const char *sch, int num);
void mgl_grad_xy(HMGL gr, HCDT x, HCDT y, HCDT ph, const char *sch, int num, float zVal);
void mgl_grad(HMGL gr, HCDT ph, const char *sch, int num, float zVal);

/*****************************************************************************/
void mgl_traj_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch,float *zVal,float *len,int);
void mgl_traj_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch,float *len,int);

void mgl_vect_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch,float *zVal,int *flag,int);
void mgl_vect_2d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, const char *sch,float *zVal,int *flag,int);
void mgl_vect_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch,int *flag,int);
void mgl_vect_3d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch,int *flag,int);

void mgl_flow_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, int *num, float *zVal,int);
void mgl_flow_2d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, const char *sch, int *num, float *zVal,int);
void mgl_flow_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, int *num,int);
void mgl_flow_3d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, int *num,int);

void mgl_flowp_xy_(uintptr_t *gr, float *x0, float *y0, float *z0, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, int);
void mgl_flowp_2d_(uintptr_t *gr, float *x0, float *y0, float *z0, uintptr_t *ax, uintptr_t *ay, const char *sch, int);
void mgl_flowp_xyz_(uintptr_t *gr, float *x0, float *y0, float *z0, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, int);
void mgl_flowp_3d_(uintptr_t *gr, float *x0, float *y0, float *z0, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch,int);

void mgl_pipe_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, float *r0, int *num, float *zVal,int);
void mgl_pipe_2d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, const char *sch, float *r0, int *num, float *zVal,int);
void mgl_pipe_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, float *r0, int *num,int);
void mgl_pipe_3d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, float *r0, int *num,int);

void mgl_grad_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ph, const char *sch, int *num, int);
void mgl_grad_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ph, const char *sch, int *num, float *zVal,int);
void mgl_grad_(uintptr_t *gr, uintptr_t *ph, const char *sch, int *num, float *zVal,int);

/*****************************************************************************/
#ifdef __cplusplus
}
#endif
/*****************************************************************************/
#endif