// Copyright 2015 Mark Owen
// http://www.quinapalus.com
// E-mail: qfp@quinapalus.com
//
// Thanks to Bill Westfield
//
// This file is free software: you can redistribute it and/or modify
// it under the terms of version 2 of the GNU General Public License
// as published by the Free Software Foundation.
//
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this file.  If not, see <http://www.gnu.org/licenses/> or
// write to the Free Software Foundation, Inc., 51 Franklin Street,
// Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef _QFPLIB_H_
#define _QFPLIB_H_

#ifdef __cplusplus
  extern "C" {
#endif

extern          float qfp_fadd(float x,float y);
extern          float qfp_fsub(float x,float y);
extern          float qfp_fmul(float x,float y);
extern          float qfp_fdiv(float x,float y);
extern          float qfp_fdiv_fast(float x,float y);
extern          int   qfp_float2int(float x);
extern          int   qfp_float2fix(float x,int y);
extern unsigned int   qfp_float2uint(float x);
extern unsigned int   qfp_float2ufix(float x,int y);
extern          float qfp_int2float(int x);
extern          float qfp_fix2float(int x,int y);
extern          float qfp_uint2float(unsigned int x);
extern          float qfp_ufix2float(unsigned int x,int y);
extern          int   qfp_fcmp(float x,float y);
extern          float qfp_fcos(float x);
extern          float qfp_fsin(float x);
extern          float qfp_ftan(float x);
extern          float qfp_fatan2(float y,float x);
extern          float qfp_fexp(float x);
extern          float qfp_fln(float x);
extern          float qfp_fsqrt(float x);
extern          float qfp_fsqrt_fast(float x);

#ifdef __cplusplus
  } // extern "C"
#endif
#endif
