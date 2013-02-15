/***************************************************************************
 * data_cf.h is part of Math Graphic Library
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
#ifndef _MGL_DATA_CF_H_
#define _MGL_DATA_CF_H_
//-----------------------------------------------------------------------------
#include "mgl2/base.h"
//-----------------------------------------------------------------------------
#if MGL_HAVE_GSL
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#else
typedef void gsl_vector;
typedef void gsl_matrix;
#endif
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

/// Set seed for random numbers
void MGL_EXPORT mgl_srnd(long seed);
void MGL_EXPORT mgl_srnd_(int *seed);
/// Get random number
double MGL_EXPORT mgl_rnd();
double MGL_EXPORT mgl_rnd_();
/// Get integer power of x
double MGL_EXPORT mgl_ipow(double x,int n);
double MGL_EXPORT mgl_ipow_(mreal *x,int *n);
/// Get number of seconds since 1970 for given string
double MGL_EXPORT mgl_get_time(const char *time, const char *fmt);
double MGL_EXPORT mgl_get_time_(const char *time, const char *fmt,int,int);

/// Create HMDT object
HMDT MGL_EXPORT mgl_create_data();
uintptr_t MGL_EXPORT mgl_create_data_();
/// Create HMDT object with specified sizes
HMDT MGL_EXPORT mgl_create_data_size(long nx, long ny, long nz);
uintptr_t MGL_EXPORT mgl_create_data_size_(int *nx, int *ny, int *nz);
/// Create HMDT object with data from file
HMDT MGL_EXPORT mgl_create_data_file(const char *fname);
uintptr_t MGL_EXPORT mgl_create_data_file_(const char *fname, int len);
/// Delete HMDT object
void MGL_EXPORT mgl_delete_data(HMDT dat);
void MGL_EXPORT mgl_delete_data_(uintptr_t *dat);
/// Get information about the data (sizes and momentum) to string
MGL_EXPORT const char *mgl_data_info(HCDT dat);

/// Rearange data dimensions
void MGL_EXPORT mgl_data_rearrange(HMDT dat, long mx,long my,long mz);
void MGL_EXPORT mgl_data_rearrange_(uintptr_t *dat, int *mx, int *my, int *mz);
/// Link external data array (don't delete it at exit)
void MGL_EXPORT mgl_data_link(HMDT dat, mreal *A,long mx,long my,long mz);
void MGL_EXPORT mgl_data_link_(uintptr_t *d, mreal *A, int *nx,int *ny,int *nz);
/// Allocate memory and copy the data from the (float *) array
void MGL_EXPORT mgl_data_set_float(HMDT dat, const float *A,long mx,long my,long mz);
void MGL_EXPORT mgl_data_set_float_(uintptr_t *dat, const float *A,int *NX,int *NY,int *NZ);
void MGL_EXPORT mgl_data_set_float1_(uintptr_t *d, const float *A,int *N1);
/// Allocate memory and copy the data from the (double *) array
void MGL_EXPORT mgl_data_set_double(HMDT dat, const double *A,long mx,long my,long mz);
void MGL_EXPORT mgl_data_set_double_(uintptr_t *dat, const double *A,int *NX,int *NY,int *NZ);
void MGL_EXPORT mgl_data_set_double1_(uintptr_t *d, const double *A,int *N1);
/// Allocate memory and copy the data from the (float **) array
void MGL_EXPORT mgl_data_set_float2(HMDT d, const float **A,long N1,long N2);
void MGL_EXPORT mgl_data_set_float2_(uintptr_t *d, const float *A,int *N1,int *N2);
/// Allocate memory and copy the data from the (double **) array
void MGL_EXPORT mgl_data_set_double2(HMDT d, const double **A,long N1,long N2);
void MGL_EXPORT mgl_data_set_double2_(uintptr_t *d, const double *A,int *N1,int *N2);
/// Allocate memory and copy the data from the (float ***) array
void MGL_EXPORT mgl_data_set_float3(HMDT d, const float ***A,long N1,long N2,long N3);
void MGL_EXPORT mgl_data_set_float3_(uintptr_t *d, const float *A,int *N1,int *N2,int *N3);
/// Allocate memory and copy the data from the (double ***) array
void MGL_EXPORT mgl_data_set_double3(HMDT d, const double ***A,long N1,long N2,long N3);
void MGL_EXPORT mgl_data_set_double3_(uintptr_t *d, const double *A,int *N1,int *N2,int *N3);
/// Import data from abstract type
void MGL_EXPORT mgl_data_set(HMDT dat, HCDT a);
void MGL_EXPORT mgl_data_set_(uintptr_t *dat, uintptr_t *a);
/// Allocate memory and copy the data from the gsl_vector
void MGL_EXPORT mgl_data_set_vector(HMDT dat, gsl_vector *v);
/// Allocate memory and copy the data from the gsl_matrix
void MGL_EXPORT mgl_data_set_matrix(HMDT dat, gsl_matrix *m);
/// Set value of data element [i,j,k]
void MGL_EXPORT mgl_data_set_value(HMDT dat, mreal v, long i, long j, long k);
void MGL_EXPORT mgl_data_set_value_(uintptr_t *d, mreal *v, int *i, int *j, int *k);
/// Get value of data element [i,j,k]
mreal MGL_EXPORT mgl_data_get_value(HCDT dat, long i, long j, long k);
mreal MGL_EXPORT mgl_data_get_value_(uintptr_t *d, int *i, int *j, int *k);
/// Allocate memory and scanf the data from the string
void MGL_EXPORT mgl_data_set_values(HMDT dat, const char *val, long nx, long ny, long nz);
void MGL_EXPORT mgl_data_set_values_(uintptr_t *d, const char *val, int *nx, int *ny, int *nz, int l);

/// Read data array from HDF file (parse HDF4 and HDF5 files)
int MGL_EXPORT mgl_data_read_hdf(HMDT d,const char *fname,const char *data);
int MGL_EXPORT mgl_data_read_hdf_(uintptr_t *d, const char *fname, const char *data,int l,int n);
/// Save data to HDF file
void MGL_EXPORT mgl_data_save_hdf(HCDT d,const char *fname,const char *data,int rewrite);
void MGL_EXPORT mgl_data_save_hdf_(uintptr_t *d, const char *fname, const char *data, int *rewrite,int l,int n);
/// Put HDF data names into buf as '\t' separated.
int MGL_EXPORT mgl_datas_hdf(const char *fname, char *buf, long size);
/// Read data from tab-separated text file with auto determining size
int MGL_EXPORT mgl_data_read(HMDT dat, const char *fname);
int MGL_EXPORT mgl_data_read_(uintptr_t *d, const char *fname,int l);
/// Read data from text file with size specified at beginning of the file
int MGL_EXPORT mgl_data_read_mat(HMDT dat, const char *fname, long dim);
int MGL_EXPORT mgl_data_read_mat_(uintptr_t *dat, const char *fname, int *dim, int);
/// Read data from text file with specifeid size
int MGL_EXPORT mgl_data_read_dim(HMDT dat, const char *fname,long mx,long my,long mz);
int MGL_EXPORT mgl_data_read_dim_(uintptr_t *dat, const char *fname,int *mx,int *my,int *mz,int);
/// Read data from tab-separated text files with auto determining size which filenames are result of sprintf(fname,templ,t) where t=from:step:to
int MGL_EXPORT mgl_data_read_range(HMDT d, const char *templ, double from, double to, double step, int as_slice);
int MGL_EXPORT mgl_data_read_range_(uintptr_t *d, const char *fname, mreal *from, mreal *to, mreal *step, int *as_slice,int l);
/// Read data from tab-separated text files with auto determining size which filenames are satisfied to template (like "t_*.dat")
int MGL_EXPORT mgl_data_read_all(HMDT dat, const char *templ, int as_slice);
int MGL_EXPORT mgl_data_read_all_(uintptr_t *d, const char *fname, int *as_slice,int l);
/// Save whole data array (for ns=-1) or only ns-th slice to text file
void MGL_EXPORT mgl_data_save(HCDT dat, const char *fname,long ns);
void MGL_EXPORT mgl_data_save_(uintptr_t *dat, const char *fname,int *ns,int);
/// Export data array (for ns=-1) or only ns-th slice to PNG file according color scheme
void MGL_EXPORT mgl_data_export(HCDT dat, const char *fname, const char *scheme,mreal v1,mreal v2,long ns);
void MGL_EXPORT mgl_data_export_(uintptr_t *dat, const char *fname, const char *scheme,mreal *v1,mreal *v2,int *ns,int,int);
/// Import data array from PNG file according color scheme
void MGL_EXPORT mgl_data_import(HMDT dat, const char *fname, const char *scheme,mreal v1,mreal v2);
void MGL_EXPORT mgl_data_import_(uintptr_t *dat, const char *fname, const char *scheme,mreal *v1,mreal *v2,int,int);

/// Create or recreate the array with specified size and fill it by zero
void MGL_EXPORT mgl_data_create(HMDT dat, long nx,long ny,long nz);
void MGL_EXPORT mgl_data_create_(uintptr_t *dat, int *nx,int *ny,int *nz);
/// Transpose dimensions of the data (generalization of Transpose)
void MGL_EXPORT mgl_data_transpose(HMDT dat, const char *dim);
void MGL_EXPORT mgl_data_transpose_(uintptr_t *dat, const char *dim,int);
/// Normalize the data to range [v1,v2]
void MGL_EXPORT mgl_data_norm(HMDT dat, mreal v1,mreal v2,long sym,long dim);
void MGL_EXPORT mgl_data_norm_(uintptr_t *dat, mreal *v1,mreal *v2,int *sym,int *dim);
/// Normalize the data to range [v1,v2] slice by slice
void MGL_EXPORT mgl_data_norm_slice(HMDT dat, mreal v1,mreal v2,char dir,long keep_en,long sym);
void MGL_EXPORT mgl_data_norm_slice_(uintptr_t *dat, mreal *v1,mreal *v2,char *dir,int *keep_en,int *sym,int l);
/// Get sub-array of the data with given fixed indexes
HMDT MGL_EXPORT mgl_data_subdata(HCDT dat, long xx,long yy,long zz);
uintptr_t MGL_EXPORT mgl_data_subdata_(uintptr_t *dat, int *xx,int *yy,int *zz);
/// Get sub-array of the data with given fixed indexes (like indirect access)
HMDT MGL_EXPORT mgl_data_subdata_ext(HCDT dat, HCDT xx, HCDT yy, HCDT zz);
uintptr_t MGL_EXPORT mgl_data_subdata_ext_(uintptr_t *dat, uintptr_t *xx,uintptr_t *yy,uintptr_t *zz);
/// Get column (or slice) of the data filled by formulas of named columns
HMDT MGL_EXPORT mgl_data_column(HCDT dat, const char *eq);
uintptr_t MGL_EXPORT mgl_data_column_(uintptr_t *dat, const char *eq,int l);
/// Set names for columns (slices)
void MGL_EXPORT mgl_data_set_id(HMDT d, const char *id);
void MGL_EXPORT mgl_data_set_id_(uintptr_t *dat, const char *id,int l);
/// Equidistantly fill the data to range [x1,x2] in direction dir
void MGL_EXPORT mgl_data_fill(HMDT dat, mreal x1,mreal x2,char dir);
void MGL_EXPORT mgl_data_fill_(uintptr_t *dat, mreal *x1,mreal *x2,const char *dir,int);
/// Modify the data by specified formula assuming x,y,z in range [r1,r2]
void MGL_EXPORT mgl_data_fill_eq(HMGL gr, HMDT dat, const char *eq, HCDT vdat, HCDT wdat,const char *opt);
void MGL_EXPORT mgl_data_fill_eq_(uintptr_t *gr, uintptr_t *dat, const char *eq, uintptr_t *vdat, uintptr_t *wdat,const char *opt, int, int);
/// Set the data by triangulated surface values assuming x,y,z in range [r1,r2]
void MGL_EXPORT mgl_data_grid(HMGL gr, HMDT d, HCDT xdat, HCDT ydat, HCDT zdat,const char *opt);
void MGL_EXPORT mgl_data_grid_(uintptr_t *gr, uintptr_t *dat, uintptr_t *xdat, uintptr_t *ydat, uintptr_t *zdat, const char *opt,int);
/// Put value to data element(s)
void MGL_EXPORT mgl_data_put_val(HMDT dat, mreal val, long i, long j, long k);
void MGL_EXPORT mgl_data_put_val_(uintptr_t *dat, mreal *val, int *i, int *j, int *k);
/// Put array to data element(s)
void MGL_EXPORT mgl_data_put_dat(HMDT dat, HCDT val, long i, long j, long k);
void MGL_EXPORT mgl_data_put_dat_(uintptr_t *dat, uintptr_t *val, int *i, int *j, int *k);
/// Modify the data by specified formula
void MGL_EXPORT mgl_data_modify(HMDT dat, const char *eq,long dim);
void MGL_EXPORT mgl_data_modify_(uintptr_t *dat, const char *eq,int *dim,int);
/// Modify the data by specified formula
void MGL_EXPORT mgl_data_modify_vw(HMDT dat, const char *eq,HCDT vdat,HCDT wdat);
void MGL_EXPORT mgl_data_modify_vw_(uintptr_t *dat, const char *eq, uintptr_t *vdat, uintptr_t *wdat,int);
/// Reduce size of the data
void MGL_EXPORT mgl_data_squeeze(HMDT dat, long rx,long ry,long rz,long smooth);
void MGL_EXPORT mgl_data_squeeze_(uintptr_t *dat, int *rx,int *ry,int *rz,int *smooth);

/// Get maximal value of the data
mreal MGL_EXPORT mgl_data_max(HCDT dat);
mreal MGL_EXPORT mgl_data_max_(uintptr_t *dat);
/// Get minimal value of the data
mreal MGL_EXPORT mgl_data_min(HCDT dat);
mreal MGL_EXPORT mgl_data_min_(uintptr_t *dat);
/// Returns pointer to data element [i,j,k]
MGL_EXPORT mreal *mgl_data_value(HMDT dat, long i,long j,long k);
/// Returns pointer to internal data array
MGL_EXPORT mreal *mgl_data_data(HMDT dat);

/// Gets the x-size of the data.
long MGL_EXPORT mgl_data_get_nx(HCDT d);
long MGL_EXPORT mgl_data_get_nx_(uintptr_t *d);
/// Gets the y-size of the data.
long MGL_EXPORT mgl_data_get_ny(HCDT d);
long MGL_EXPORT mgl_data_get_ny_(uintptr_t *d);
/// Gets the z-size of the data.
long MGL_EXPORT mgl_data_get_nz(HCDT d);
long MGL_EXPORT mgl_data_get_nz_(uintptr_t *d);

/// Find position (after specified in i,j,k) of first nonzero value of formula
mreal MGL_EXPORT mgl_data_first(HCDT dat, const char *cond, long *i, long *j, long *k);
mreal MGL_EXPORT mgl_data_first_(uintptr_t *dat, const char *cond, int *i, int *j, int *k, int);
/// Find position (before specified in i,j,k) of last nonzero value of formula
mreal MGL_EXPORT mgl_data_last(HCDT dat, const char *cond, long *i, long *j, long *k);
mreal MGL_EXPORT mgl_data_last_(uintptr_t *dat, const char *cond, int *i, int *j, int *k, int);
/// Find position of first in direction 'dir' nonzero value of formula
long MGL_EXPORT mgl_data_find(HCDT dat, const char *cond, char dir, long i, long j, long k);
int MGL_EXPORT mgl_data_find_(uintptr_t *dat, const char *cond, char *dir, int *i, int *j, int *k, int,int);
/// Find if any nonzero value of formula
int MGL_EXPORT mgl_data_find_any(HCDT dat, const char *cond);
int MGL_EXPORT mgl_data_find_any_(uintptr_t *dat, const char *cond, int);
/// Get maximal value of the data and its position
mreal MGL_EXPORT mgl_data_max_int(HCDT dat, long *i, long *j, long *k);
mreal MGL_EXPORT mgl_data_max_int_(uintptr_t *dat, int *i, int *j, int *k);
/// Get maximal value of the data and its approximated position
mreal MGL_EXPORT mgl_data_max_real(HCDT dat, mreal *x, mreal *y, mreal *z);
mreal MGL_EXPORT mgl_data_max_real_(uintptr_t *dat, mreal *x, mreal *y, mreal *z);
/// Get minimal value of the data and its position
mreal MGL_EXPORT mgl_data_min_int(HCDT dat, long *i, long *j, long *k);
mreal MGL_EXPORT mgl_data_min_int_(uintptr_t *dat, int *i, int *j, int *k);
/// Get minimal value of the data and its approximated position
mreal MGL_EXPORT mgl_data_min_real(HCDT dat, mreal *x, mreal *y, mreal *z);
mreal MGL_EXPORT mgl_data_min_real_(uintptr_t *dat, mreal *x, mreal *y, mreal *z);
/// Get "energy and find 4 momenta of data: median, width, skewness, kurtosis
mreal MGL_EXPORT mgl_data_momentum_val(HCDT d, char dir, mreal *m, mreal *w, mreal *s, mreal *k);
mreal MGL_EXPORT mgl_data_momentum_val_(uintptr_t *dat, char *dir, mreal *m, mreal *w, mreal *s, mreal *k,int);

/// Get the data which is direct multiplication (like, d[i,j] = this[i]*a[j] and so on)
HMDT MGL_EXPORT mgl_data_combine(HCDT dat1, HCDT dat2);
uintptr_t MGL_EXPORT mgl_data_combine_(uintptr_t *dat1, uintptr_t *dat2);
/// Extend data dimensions
void MGL_EXPORT mgl_data_extend(HMDT dat, long n1, long n2);
void MGL_EXPORT mgl_data_extend_(uintptr_t *dat, int *n1, int *n2);
/// Insert data rows/columns/slices
void MGL_EXPORT mgl_data_insert(HMDT dat, char dir, long at, long num);
void MGL_EXPORT mgl_data_insert_(uintptr_t *dat, const char *dir, int *at, int *num, int);
/// Delete data rows/columns/slices
void MGL_EXPORT mgl_data_delete(HMDT dat, char dir, long at, long num);
void MGL_EXPORT mgl_data_delete_(uintptr_t *dat, const char *dir, int *at, int *num, int);

/// Smooth the data on specified direction or directions
void MGL_EXPORT mgl_data_smooth(HMDT d, const char *dirs, mreal delta);
void MGL_EXPORT mgl_data_smooth_(uintptr_t *dat, const char *dirs, mreal *delta,int);
/// Get array which is result of summation in given direction or directions
HMDT MGL_EXPORT mgl_data_sum(HCDT dat, const char *dir);
uintptr_t MGL_EXPORT mgl_data_sum_(uintptr_t *dat, const char *dir,int);
/// Get array which is result of maximal values in given direction or directions
HMDT MGL_EXPORT mgl_data_max_dir(HCDT dat, const char *dir);
uintptr_t MGL_EXPORT mgl_data_max_dir_(uintptr_t *dat, const char *dir,int);
/// Get array which is result of minimal values in given direction or directions
HMDT MGL_EXPORT mgl_data_min_dir(HCDT dat, const char *dir);
uintptr_t MGL_EXPORT mgl_data_min_dir_(uintptr_t *dat, const char *dir,int);
/// Cumulative summation the data in given direction or directions
void MGL_EXPORT mgl_data_cumsum(HMDT dat, const char *dir);
void MGL_EXPORT mgl_data_cumsum_(uintptr_t *dat, const char *dir,int);
/// Integrate (cumulative summation) the data in given direction or directions
void MGL_EXPORT mgl_data_integral(HMDT dat, const char *dir);
void MGL_EXPORT mgl_data_integral_(uintptr_t *dat, const char *dir,int);
/// Differentiate the data in given direction or directions
void MGL_EXPORT mgl_data_diff(HMDT dat, const char *dir);
void MGL_EXPORT mgl_data_diff_(uintptr_t *dat, const char *dir,int);
/// Differentiate the parametrically specified data along direction v1 with v2,v3=const (v3 can be NULL)
void MGL_EXPORT mgl_data_diff_par(HMDT dat, HCDT v1, HCDT v2, HCDT v3);
void MGL_EXPORT mgl_data_diff_par_(uintptr_t *dat, uintptr_t *v1, uintptr_t *v2, uintptr_t *v3);
/// Double-differentiate (like Laplace operator) the data in given direction
void MGL_EXPORT mgl_data_diff2(HMDT dat, const char *dir);
void MGL_EXPORT mgl_data_diff2_(uintptr_t *dat, const char *dir,int);
/// Swap left and right part of the data in given direction (useful for Fourier spectrum)
void MGL_EXPORT mgl_data_swap(HMDT dat, const char *dir);
void MGL_EXPORT mgl_data_swap_(uintptr_t *dat, const char *dir,int);
/// Roll data along direction dir by num slices
void MGL_EXPORT mgl_data_roll(HMDT dat, char dir, long num);
void MGL_EXPORT mgl_data_roll_(uintptr_t *dat, const char *dir, int *num, int);
/// Mirror the data in given direction (useful for Fourier spectrum)
void MGL_EXPORT mgl_data_mirror(HMDT dat, const char *dir);
void MGL_EXPORT mgl_data_mirror_(uintptr_t *dat, const char *dir,int);
/// Sort rows (or slices) by values of specified column
void MGL_EXPORT mgl_data_sort(HMDT dat, long idx, long idy);
void MGL_EXPORT mgl_data_sort_(uintptr_t *dat, int *idx, int *idy);

/// Apply Hankel transform
void MGL_EXPORT mgl_data_hankel(HMDT dat, const char *dir);
void MGL_EXPORT mgl_data_hankel_(uintptr_t *dat, const char *dir,int);
/// Apply Sin-Fourier transform
void MGL_EXPORT mgl_data_sinfft(HMDT dat, const char *dir);
void MGL_EXPORT mgl_data_sinfft_(uintptr_t *dat, const char *dir,int);
/// Apply Cos-Fourier transform
void MGL_EXPORT mgl_data_cosfft(HMDT dat, const char *dir);
void MGL_EXPORT mgl_data_cosfft_(uintptr_t *dat, const char *dir,int);
/// Fill data by 'x'/'k' samples for Hankel ('h') or Fourier ('f') transform
void MGL_EXPORT mgl_data_fill_sample(HMDT dat, const char *how);
void MGL_EXPORT mgl_data_fill_sample_(uintptr_t *dat, const char *how,int);

/// Interpolate by cubic spline the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
mreal MGL_EXPORT mgl_data_spline(HCDT dat, mreal x,mreal y,mreal z);
mreal MGL_EXPORT mgl_data_spline_(uintptr_t *dat, mreal *x,mreal *y,mreal *z);
/// Interpolate by linear function the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
mreal MGL_EXPORT mgl_data_linear(HCDT dat, mreal x,mreal y,mreal z);
mreal MGL_EXPORT mgl_data_linear_(uintptr_t *dat, mreal *x,mreal *y,mreal *z);
/// Interpolate by cubic spline the data and return its derivatives at given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
mreal MGL_EXPORT mgl_data_spline_ext(HCDT dat, mreal x,mreal y,mreal z, mreal *dx,mreal *dy,mreal *dz);
mreal MGL_EXPORT mgl_data_spline_ext_(uintptr_t *dat, mreal *x,mreal *y,mreal *z, mreal *dx,mreal *dy,mreal *dz);
/// Interpolate by linear function the data and return its derivatives at given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
mreal MGL_EXPORT mgl_data_linear_ext(HCDT dat, mreal x,mreal y,mreal z, mreal *dx,mreal *dy,mreal *dz);
mreal MGL_EXPORT mgl_data_linear_ext_(uintptr_t *dat, mreal *x,mreal *y,mreal *z, mreal *dx,mreal *dy,mreal *dz);
/// Return an approximated x-value (root) when dat(x) = val
mreal MGL_EXPORT mgl_data_solve_1d(HCDT dat, mreal val, int spl, long i0);
mreal MGL_EXPORT mgl_data_solve_1d_(uintptr_t *dat, mreal *val, int *spl, int *i0);
/// Return an approximated value (root) when dat(x) = val
HMDT MGL_EXPORT mgl_data_solve(HCDT dat, mreal val, char dir, HCDT i0, int norm);
uintptr_t MGL_EXPORT mgl_data_solve_(uintptr_t *dat, mreal *val, const char *dir, uintptr_t *i0, int *norm,int);

/// Get trace of the data array
HMDT MGL_EXPORT mgl_data_trace(HCDT d);
uintptr_t MGL_EXPORT mgl_data_trace_(uintptr_t *d);
/// Resize the data to new sizes
HMDT MGL_EXPORT mgl_data_resize(HCDT dat, long mx,long my,long mz);
uintptr_t MGL_EXPORT mgl_data_resize_(uintptr_t *dat, int *mx,int *my,int *mz);
/// Resize the data to new sizes of box [x1,x2]*[y1,y2]*[z1,z2]
HMDT MGL_EXPORT mgl_data_resize_box(HCDT dat, long mx,long my,long mz,mreal x1,mreal x2,mreal y1,mreal y2,mreal z1,mreal z2);
uintptr_t MGL_EXPORT mgl_data_resize_box_(uintptr_t *dat, int *mx,int *my,int *mz,mreal *x1,mreal *x2,mreal *y1,mreal *y2,mreal *z1,mreal *z2);
/// Create n-th points distribution of this data values in range [v1, v2]
HMDT MGL_EXPORT mgl_data_hist(HCDT dat, long n, mreal v1, mreal v2, long nsub);
uintptr_t MGL_EXPORT mgl_data_momentum_(uintptr_t *dat, char *dir, const char *how, int,int);
/// Create n-th points distribution of this data values in range [v1, v2] with weight w
HMDT MGL_EXPORT mgl_data_hist_w(HCDT dat, HCDT weight, long n, mreal v1, mreal v2, long nsub);
uintptr_t MGL_EXPORT mgl_data_hist_(uintptr_t *dat, int *n, mreal *v1, mreal *v2, int *nsub);
/// Get momentum (1D-array) of data along direction 'dir'. String looks like "x1" for median in x-direction, "x2" for width in x-dir and so on.
HMDT MGL_EXPORT mgl_data_momentum(HCDT dat, char dir, const char *how);
uintptr_t MGL_EXPORT mgl_data_hist_w_(uintptr_t *dat, uintptr_t *weight, int *n, mreal *v1, mreal *v2, int *nsub);
/// Get array which values is result of interpolation this for coordinates from other arrays
HMDT MGL_EXPORT mgl_data_evaluate(HCDT dat, HCDT idat, HCDT jdat, HCDT kdat, int norm);
uintptr_t MGL_EXPORT mgl_data_evaluate_(uintptr_t *dat, uintptr_t *idat, uintptr_t *jdat, uintptr_t *kdat, int *norm);
/// Set as the data envelop
void MGL_EXPORT mgl_data_envelop(HMDT dat, char dir);
void MGL_EXPORT mgl_data_envelop_(uintptr_t *dat, const char *dir, int);
/// Remove phase jump
void MGL_EXPORT mgl_data_sew(HMDT dat, const char *dirs, mreal da);
void MGL_EXPORT mgl_data_sew_(uintptr_t *dat, const char *dirs, mreal *da, int);
/// Crop the data
void MGL_EXPORT mgl_data_crop(HMDT dat, long n1, long n2, char dir);
void MGL_EXPORT mgl_data_crop_(uintptr_t *dat, int *n1, int *n2, const char *dir,int);
/// Remove rows with duplicate values in column id
void MGL_EXPORT mgl_data_clean(HMDT dat, long id);
void MGL_EXPORT mgl_data_clean_(uintptr_t *dat, int *id);

/// Multiply the data by other one for each element
void MGL_EXPORT mgl_data_mul_dat(HMDT dat, HCDT d);
void MGL_EXPORT mgl_data_mul_dat_(uintptr_t *dat, uintptr_t *d);
/// Divide the data by other one for each element
void MGL_EXPORT mgl_data_div_dat(HMDT dat, HCDT d);
void MGL_EXPORT mgl_data_div_dat_(uintptr_t *dat, uintptr_t *d);
/// Add the other data
void MGL_EXPORT mgl_data_add_dat(HMDT dat, HCDT d);
void MGL_EXPORT mgl_data_add_dat_(uintptr_t *dat, uintptr_t *d);
/// Subtract the other data
void MGL_EXPORT mgl_data_sub_dat(HMDT dat, HCDT d);
void MGL_EXPORT mgl_data_sub_dat_(uintptr_t *dat, uintptr_t *d);
/// Multiply each element by the number
void MGL_EXPORT mgl_data_mul_num(HMDT dat, mreal d);
void MGL_EXPORT mgl_data_mul_num_(uintptr_t *dat, mreal *d);
/// Divide each element by the number
void MGL_EXPORT mgl_data_div_num(HMDT dat, mreal d);
void MGL_EXPORT mgl_data_div_num_(uintptr_t *dat, mreal *d);
/// Add the number
void MGL_EXPORT mgl_data_add_num(HMDT dat, mreal d);
void MGL_EXPORT mgl_data_add_num_(uintptr_t *dat, mreal *d);
/// Subtract the number
void MGL_EXPORT mgl_data_sub_num(HMDT dat, mreal d);
void MGL_EXPORT mgl_data_sub_num_(uintptr_t *dat, mreal *d);

/// Saves result of PDE solving (|u|^2) for "Hamiltonian" ham with initial conditions ini
HMDT MGL_EXPORT mgl_pde_solve(HMGL gr, const char *ham, HCDT ini_re, HCDT ini_im, mreal dz, mreal k0,const char *opt);
uintptr_t MGL_EXPORT mgl_pde_solve_(uintptr_t* gr, const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, mreal *dz, mreal *k0,const char *opt,int,int);
/// Saves result of PDE solving for "Hamiltonian" ham with initial conditions ini along a curve ray (must have nx>=7 - x,y,z,px,py,pz,tau or nx=5 - x,y,px,py,tau)
HMDT MGL_EXPORT mgl_qo2d_solve(const char *ham, HCDT ini_re, HCDT ini_im, HCDT ray, mreal r, mreal k0, HMDT xx, HMDT yy);
HMDT MGL_EXPORT mgl_qo2d_func(dual (*ham)(mreal u, mreal x, mreal y, mreal px, mreal py, void *par), void *par, HCDT ini_re, HCDT ini_im, HCDT ray, mreal r, mreal k0, HMDT xx, HMDT yy);
uintptr_t MGL_EXPORT mgl_qo2d_solve_(const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, uintptr_t* ray, mreal *r, mreal *k0, uintptr_t* xx, uintptr_t* yy, int);
/// Saves result of PDE solving for "Hamiltonian" ham with initial conditions ini along a curve ray (must have nx>=7 - x,y,z,px,py,pz,tau or nx=5 - x,y,px,py,tau)
HMDT MGL_EXPORT mgl_qo3d_solve(const char *ham, HCDT ini_re, HCDT ini_im, HCDT ray, mreal r, mreal k0, HMDT xx, HMDT yy, HMDT zz);
HMDT MGL_EXPORT mgl_qo3d_func(dual (*ham)(mreal u, mreal x, mreal y, mreal z, mreal px, mreal py, mreal pz, void *par), void *par, HCDT ini_re, HCDT ini_im, HCDT ray, mreal r, mreal k0, HMDT xx, HMDT yy, HMDT zz);
uintptr_t MGL_EXPORT mgl_qo3d_solve_(const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, uintptr_t* ray, mreal *r, mreal *k0, uintptr_t* xx, uintptr_t* yy, uintptr_t* zz, int);
/// Finds ray with starting point r0, p0 (and prepares ray data for mgl_qo2d_solve)
HMDT MGL_EXPORT mgl_ray_trace(const char *ham, mreal x0, mreal y0, mreal z0, mreal px, mreal py, mreal pz, mreal dt, mreal tmax);
uintptr_t MGL_EXPORT mgl_ray_trace_(const char *ham, mreal *x0, mreal *y0, mreal *z0, mreal *px, mreal *py, mreal *pz, mreal *dt, mreal *tmax,int);
/// Calculate Jacobian determinant for D{x(u,v), y(u,v)} = dx/du*dy/dv-dx/dv*dy/du
HMDT MGL_EXPORT mgl_jacobian_2d(HCDT x, HCDT y);
uintptr_t MGL_EXPORT mgl_jacobian_2d_(uintptr_t* x, uintptr_t* y);
/// Calculate Jacobian determinant for D{x(u,v,w), y(u,v,w), z(u,v,w)}
HMDT MGL_EXPORT mgl_jacobian_3d(HCDT x, HCDT y, HCDT z);
uintptr_t MGL_EXPORT mgl_jacobian_3d_(uintptr_t* x, uintptr_t* y, uintptr_t* z);
/// Integral data transformation (like Fourier 'f' or 'i', Hankel 'h' or None 'n') for amplitude and phase
HMDT MGL_EXPORT mgl_transform_a(HCDT am, HCDT ph, const char *tr);
uintptr_t MGL_EXPORT mgl_transform_a_(uintptr_t *am, uintptr_t *ph, const char *tr, int);
/// Integral data transformation (like Fourier 'f' or 'i', Hankel 'h' or None 'n') for real and imaginary parts
HMDT MGL_EXPORT mgl_transform(HCDT re, HCDT im, const char *tr);
uintptr_t MGL_EXPORT mgl_transform_(uintptr_t *re, uintptr_t *im, const char *tr, int);
/// Apply Fourier transform for the data and save result into it
void MGL_EXPORT mgl_data_fourier(HMDT re, HMDT im, const char *dir);
void MGL_EXPORT mgl_data_fourier_(uintptr_t *re, uintptr_t *im, const char *dir, int l);
/// Short time Fourier analysis for real and imaginary parts. Output is amplitude of partial Fourier (result will have size {dn, floor(nx/dn), ny} for dir='x'
HMDT MGL_EXPORT mgl_data_stfa(HCDT re, HCDT im, long dn, char dir);
uintptr_t MGL_EXPORT mgl_data_stfa_(uintptr_t *re, uintptr_t *im, int *dn, char *dir, int);

/// Do something like Delone triangulation for 3d points
HMDT MGL_EXPORT mgl_triangulation_3d(HCDT x, HCDT y, HCDT z);
uintptr_t MGL_EXPORT mgl_triangulation_3d_(uintptr_t *x, uintptr_t *y, uintptr_t *z);
/// Do Delone triangulation for 2d points
HMDT MGL_EXPORT mgl_triangulation_2d(HCDT x, HCDT y);
uintptr_t MGL_EXPORT mgl_triangulation_2d_(uintptr_t *x, uintptr_t *y);

#ifdef __cplusplus
}
#endif
//-----------------------------------------------------------------------------
#endif
