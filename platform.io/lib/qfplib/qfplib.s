@ Copyright 2015-2020 Mark Owen
@ http://www.quinapalus.com
@ E-mail: qfp@quinapalus.com
@
@ This file is free software: you can redistribute it and/or modify
@ it under the terms of version 2 of the GNU General Public License
@ as published by the Free Software Foundation.
@
@ This file is distributed in the hope that it will be useful,
@ but WITHOUT ANY WARRANTY; without even the implied warranty of
@ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@ GNU General Public License for more details.
@
@ You should have received a copy of the GNU General Public License
@ along with this file.  If not, see <http://www.gnu.org/licenses/> or
@ write to the Free Software Foundation, Inc., 51 Franklin Street,
@ Fifth Floor, Boston, MA  02110-1301, USA.

@ Thanks to Li Ling for optimisation suggestions and discussion.

@.equ include_faster,0        @ include fast divide and square root?
@.equ include_conversions,1   @ include float <-> fixed point conversion functions?
@.equ include_scientific,1    @ include trignometic, exponential etc. functions?

.ifndef include_faster
.equ include_faster,1
.endif

.ifndef include_conversions
.equ include_conversions,1
.endif

.ifndef include_scientific
.equ include_scientific,1
.endif

.if include_scientific
.equ include_conversions,1
.endif

.syntax unified
.cpu cortex-m0
.thumb

@ exported symbols

.global qfp_fadd
.global qfp_fsub
.global qfp_fmul
.global qfp_fdiv
.global qfp_fcmp
.if include_conversions
.global qfp_float2int
.global qfp_float2fix
.global qfp_float2uint
.global qfp_float2ufix
.global qfp_int2float
.global qfp_fix2float
.global qfp_uint2float
.global qfp_ufix2float
.endif
.if include_scientific
.global qfp_fcos
.global qfp_fsin
.global qfp_ftan
.global qfp_fatan2
.global qfp_fexp
.global qfp_fln
.global qfp_fsqrt
.endif

.if include_faster
.global qfp_fdiv_fast
.global qfp_fsqrt_fast
.endif

@ exchange r0<->r1, r2<->r3
xchxy:
 push {r0,r2,r14}
 mov r0,r1
 mov r2,r3
 pop {r1,r3,r15}

@ IEEE single precision floats in r0,r1-> mantissae in r1,r0 exponents in r3,r2 *respectively*
@ trashes r4
unpackxy:
 push {r14}
 bl unpackx
 bl xchxy
 pop {r4}
 mov r14,r4

@ IEEE single in r0-> signed (two's complemennt) mantissa in r0 9Q23 (24 significant bits), signed exponent (bias removed) in r2
@ trashes r4; zero, denormal -> mantissa=+/-1, exponent=-380; Inf, NaN -> mantissa=+/-1, exponent=+640
unpackx:
 lsrs r2,r0,#23 @ save exponent and sign
 lsls r0,#9     @ extract mantissa
 lsrs r0,#9
 movs r4,#1
 lsls r4,#23
 orrs r0,r4     @ reinstate implied leading 1
 cmp r2,#255    @ test sign bit
 uxtb r2,r2     @ clear it
 bls 1f         @ branch on positive
 rsbs r0,#0     @ negate mantissa
1:
 subs r2,#1
 cmp r2,#254    @ zero/denormal/Inf/NaN?
 bhs 2f
 subs r2,#126   @ remove exponent bias: can now be -126..+127
 bx r14

2:              @ here with special-case values
 cmp r0,#0
 mov r0,r4      @ set mantissa to +1
 bpl 3f
 rsbs r0,#0     @ zero/denormal/Inf/NaN: mantissa=+/-1
3:
 subs r2,#126   @ zero/denormal: exponent -> -127; Inf, NaN: exponent -> 128
 lsls r2,#2     @ zero/denormal: exponent -> -508; Inf, NaN: exponent -> 512
 adds r2,#128   @ zero/denormal: exponent -> -380; Inf, NaN: exponent -> 640
 bx r14

@ normalise and pack signed mantissa in r0 nominally 3Q29, signed exponent in r2-> IEEE single in r0
@ trashes r4, preserves r1,r3
@ r5: "sticky bits", must be zero iff all result bits below r0 are zero for correct rounding
packx:
 lsrs r4,r0,#31 @ save sign bit
 lsls r4,r4,#31 @ sign now in b31
 bpl 2f         @ skip if positive
 cmp r5,#0
 beq 11f
 adds r0,#1     @ fiddle carry in to following rsb if sticky bits are non-zero
11:
 rsbs r0,#0     @ can now treat r0 as unsigned
packx0:
 bmi 3f         @ catch r0=0x80000000 case
2:
 subs r2,#1     @ normalisation loop
 adds r0,r0
 beq 1f         @ zero? special case
 bpl 2b         @ normalise so leading "1" in bit 31
3:
 adds r2,#129   @ (mis-)offset exponent
 bne 12f        @ special case: highest denormal can round to lowest normal
 adds r0,#0x80  @ in special case, need to add 256 to r0 for rounding
 bcs 4f         @ tripped carry? then have leading 1 in C as required
12:
 adds r0,#0x80  @ rounding
 bcs 4f         @ tripped carry? then have leading 1 in C as required (and result is even so can ignore sticky bits)
 cmp r5,#0
 beq 7f         @ sticky bits zero?
8:
 lsls r0,#1     @ remove leading 1
9:
 subs r2,#1     @ compensate exponent on this path
4:
 cmp r2,#254
 bge 5f         @ overflow?
 adds r2,#1     @ correct exponent offset
 ble 10f        @ denormal/underflow?
 lsrs r0,#9     @ align mantissa
 lsls r2,#23    @ align exponent
 orrs r0,r2     @ assemble exponent and mantissa
6:
 orrs r0,r4     @ apply sign
1:
 bx r14

5:
 movs r0,#0xff  @ create infinity
 lsls r0,#23
 b 6b

10:
 movs r0,#0     @ create zero
 bx r14

7:              @ sticky bit rounding case
 lsls r5,r0,#24 @ check bottom 8 bits of r0
 bne 8b         @ in rounding-tie case?
 lsrs r0,#9     @ ensure even result
 lsls r0,#10
 b 9b

@ unpack two arguments (r0,r1) and shift one down to have common exponent, returned in r2; note that arguments are exchanged
@ sticky bits shifted off bottom of smaller argument in r5
@ following code is unnecessarily general for fadd, but is shared with atan2
unpackxyalign:
 push {r14}
 bl unpackxy
 lsls r0,r0,#6  @ Q29
 lsls r1,r1,#6  @ Q29
 subs r4,r2,r3  @ calculate shift
 bge 1f         @ x>=y?
 mov r2,r3      @ no: take common exponent from y
 mov r5,r0      @ potential sticky bits from x
 rsbs r4,#0     @ make shift positive
 asrs r0,r4
 cmp r4,#32
 blo 2f
 movs r0,#0    @ large shift, so all bits are sticky and result is zero
 pop {r15}
1:
 mov r5,r1     @ common exponent from x; potential sticky bits from y
 asrs r1,r4
 cmp r4,#32
 blo 2f
 movs r1,#0    @ large shift, so all bits are sticky and result is zero
 pop {r15}
2:
 rsbs r4,#0
 adds r4,#32
 lsls r5,r4    @ extract sticky bits
 pop {r15}

.thumb_func
qfp_fsub:
 movs r2,#1    @ subtract: flip sign bit of second argument and fall through to fadd
 lsls r2,#31
 eors r1,r2
.thumb_func
qfp_fadd:
 push {r4,r5,r14}
 bl unpackxyalign
 adds r0,r1    @ do addition
 bne 2f        @ not in Inf-Inf case?
 cmp r2,#200
 blt 2f
 movs r0,#1
 lsls r0,#29   @ for Inf-Inf, set mantissa to +1 to prevent zero result
2:
packret:       @ common return point: "pack and return"
 bl packx
 pop {r4,r5,r15}

@ signed multiply r0 1Q23 by r1 4Q23, result in r0 7Q25, sticky bits in r5
@ trashes r3,r4
mul0:
 uxth r3,r0      @ Q23
 asrs r4,r1,#16  @ Q7
 muls r3,r4      @ L*H, Q30 signed
 asrs r4,r0,#16  @ Q7
 uxth r5,r1      @ Q23
 muls r4,r5      @ H*L, Q30 signed
 adds r3,r4      @ sum of middle partial products
 uxth r4,r0
 muls r4,r5      @ L*L, Q46 unsigned
 lsls r5,r4,#16  @ initialise sticky bits from low half of low partial product
 lsrs r4,#16     @ Q25
 adds r3,r4      @ add high half of low partial product to sum of middle partial products
                 @ (cannot generate carry by limits on input arguments)
 asrs r0,#16     @ Q7
 asrs r1,#16     @ Q7
 muls r0,r1      @ H*H, Q14 signed
 lsls r0,#11     @ high partial product Q25
 lsls r1,r3,#27  @ sticky
 orrs r5,r1      @ collect further sticky bits
 asrs r1,r3,#5   @ middle partial products Q25
 adds r0,r1      @ final result
 bx r14

.thumb_func
qfp_fcmp:
 movs r2,#1      @ initialise result
 lsls r3,r2,#31  @ r3=0x80000000
 tst r0,r3       @ check sign of first argument
 beq 1f
 subs r0,r3,r0   @ convert to 2's complement form for direct comparison
1:
 tst r1,r3       @ repeat for second argument
 beq 2f
 subs r1,r3,r1
2:
 subs r0,r1     @ perform comparison
 beq 4f         @ equal? return 0
 bgt 3f         @ r0>r1? return +1
 rsbs r2,#0     @ r0<r1: return -1
3:
 mov r0,r2
4:
 bx r14

.thumb_func
qfp_fmul:
 push {r4,r5,r14}
 bl unpackxy
 add r2,r3      @ sum exponents
 adds r2,#4     @ adjust exponent for pack which expects Q29
fmul0:
 bl mul0
 b packret

.thumb_func
qfp_fdiv:
 push {r4,r5,r14}
fdiv_n:
 bl unpackxy
 movs r5,#1      @ result cannot fall exactly half-way between two representable numbers (exercise for reader - note that
                 @ we do not handle denormals) so there will always be sticky bits
 cmp r0,#0       @ check divisor sign
 bpl 1f
 rsbs r0,#0      @ ensure divisor positive
 rsbs r1,#0      @ preserve sign of result
1:
 movs r4,#0
 cmp r1,#0       @ check sign of dividend
 bpl 2f
 rsbs r1,#0      @ result will be negative
 mvns r4,r4      @ save sign as 0x00000000 or 0xffffffff
2:               @ now do unsigned division on unpacked values {r1,r3}/{r0,r2}
 cmp r3,#200     @ dividend is an infinity? return it
 bge 3f
 rsbs r2,#0
 cmp r2,#200     @ divisor is zero? return infinity
 bge 3f
 adds r2,r3      @ difference of exponents
 movs r3,#0x40   @ termination marker (calculate enough bits to do rounding correctly)
2:               @ division loop
 subs r1,r0      @ trial subtraction
 bcs 1f
 add r1,r0       @ restore if failed
1:
 adcs r3,r3      @ shift in result bit
 add r1,r1       @ shift up dividend
 bcc 2b          @ loop until marker appears in carry
 lsls r0,r3,#4   @ align for packing
4:
 eors r0,r4      @ apply sign
 b packret
infret:
 movs r4,#0
3:
 mov r0,r1
 movs r2,#255    @ return infinity
 b 4b

.if include_faster

@ The fast divide routine uses an initial approximation to the reciprocal of the divisor based on the top four bits of the mantissa
@ followed by three Newton-Raphson iterations, resulting in about 27 bits of accuracy. This reciprocal is then multiplied by
@ the dividend.
@ The (fixed-point) reciprocal calculation is carefully implemented to preserve the necessary accuracy throughout. In places
@ the implied binary point is not within the stored value. For example where "Q47" is shown below it means that the least
@ significant bit of the value has significance 2^-47 and hence the most significant bit has significance 2^-16. In these
@ cases the value is known to be very close to an integer (usually 1) and so the bits of greater significance do not need
@ to be stored.
@ The reciprocal calculation has been tested for all possible input mantissa values.
.thumb_func
qfp_fdiv_fast:
 push {r4,r5,r14}
fdiv_fast_n:
 bl unpackxy
 cmp r0,#0
 bpl 1f
 rsbs r0,#0
 rsbs r1,#0      @ ensure divisor positive
1:
 cmp r3,#200
 bge infret      @ dividend is an infinity? return it
 rsbs r2,#0
 cmp r2,#200     @ divisor is zero?
 bge infret      @ return infinity
 adds r2,r3      @ difference of exponents
 adr r4,rcpapp-8 @ the first 8 elements of the table are never accessed because of the mantissa's leading 1
 lsrs r3,r0,#20  @ y Q23; y>>20 Q7
 ldrb r4,[r4,r3] @ m=rcpapp[(y>>20)&7]; // Q8, .5<m<1

 lsls r3,r4,#2   @ m<<2         // Q10  first Newton-Raphson iteration
 muls r3,r0      @ s=y*(m<<2);  // Q33
 lsls r4,#8      @ m<<=8;       // Q16
 asrs r3,#21     @ s>>=21;      // Q12
 muls r3,r4      @ s*=m;        // Q28
 asrs r3,#12     @ s>>=12;      // Q16
 subs r4,r3      @ m=m-s;       // Q16

 mov r3,r4       @ s=y*m        // Q39 second Newton-Raphson iteration
 muls r4,r0      @ ...
 asrs r4,#16     @ s>>=16;      // Q23
 muls r4,r3      @ s*=m;        // Q39
 lsls r3,#8      @ m<<=8;       // Q24
 asrs r4,#15     @ s>>=15;      // Q24
 subs r3,r4      @ m=m-s;       // Q24

 lsls r4,r3,#7   @ \/ s=y*m;    // Q47 third Newton-Raphson iteration
 muls r3,r0      @ /\ m<<=7;    // Q31
 asrs r3,#15     @ s>>=15;      // Q32
 lsrs r0,r4,#16  @ s*=(m>>16);  // Q47
 muls r3,r0      @ ...
 asrs r3,#16     @ s>>=16;      // Q31
 subs r0,r4,r3   @ m=m-s;       // Q31
div0:
 adds r0,#7      @ rounding; reduce systematic error
 lsrs r0,#4      @ Q27
 b fmul0         @ drop into multiplication code to calculate result

@ The fast square root routine uses an initial approximation to the reciprocal of the square root of the argument based
@ on the top four bits of the mantissa (possibly shifted one place to make the exponent even). It then performs three
@ Newton-Raphson iterations, resulting in about 28-29 bits of accuracy. This reciprocal is then multiplied by
@ the original argument to produce the result.
@ Again, the fixed-point calculation is carefully implemented to preserve accuracy, and similar comments to those
@ made above on the fast division routine apply.
@ The reciprocal square root calculation has been tested for all possible (possibly shifted) input mantissa values.
.thumb_func
qfp_fsqrt_fast:
 push {r4,r5,r14}
 bl unpackx
 movs r1,r0
 bmi infret       @ negative? return -Inf
 asrs r0,r2,#1    @ check LSB of exponent
 bcc 1f
 lsls r1,#1       @ was odd: double mantissa; mantissa y now 1..4 Q23
1:
 adds r2,#4       @ correction for packing
 adr r4,rsqrtapp-4@ first four table entries are never accessed because of the mantissa's leading 1
 lsrs r3,r1,#21   @ y>>21 Q2
 ldrb r4,[r4,r3]  @ initial approximation to reciprocal square root m Q8

 lsrs r0,r1,#7    @ y>>7             // Q16 first Newton-Raphson iteration
 muls r0,r4       @ m*y
 muls r0,r4       @ s=m*y*y          // Q32
 asrs r0,#12      @ s>>12
 muls r0,r4       @ m*s              // Q28
 asrs r0,#13      @ m*s              // Q15
 lsls r4,#8       @ m                // Q16
 subs r4,r0       @ m=(m<<8)-(s>>13) // Q16-Q15/2 -> Q16

 mov r0,r4        @                  // second Newton-Raphson iteration
 muls r0,r0       @ u=m*m            // Q32
 lsrs r0,#16      @ u>>16            // Q16
 lsrs r3,r1,#7    @ y>>7             // Q16
 muls r0,r3       @ s=u*(y>>7)       // Q32
 asrs r0,#12      @ s>>12            // Q20
 muls r0,r4       @ s*m              // Q36
 asrs r0,#21      @ s*m              // Q15
 subs r4,r0       @ m=m-s            // Q16-Q15/2

 mov r0,r4        @                  // third Newton-Raphson iteration
 muls r0,r0       @ u=m*m            // Q32
 lsrs r3,r0,#12   @ now multiply u and y in two parts: u>>12
 muls r3,r1       @ first partial product (u>>12)*y Q43
 lsls r0,#20
 lsrs r0,#20      @ u&0xfff
 lsrs r5,r1,#12   @ y>>12
 muls r0,r5       @ second partial product (u&0xfff)*(y>>12) Q43
 add r0,r3        @ s=u*y            // Q43
 asrs r0,#15      @ s>>15            // Q28
 muls r0,r4       @ (s>>15)*m        // Q44
 lsls r4,#13      @ m<<13            // Q29
 asrs r0,#16      @ s>>16            // Q28
 subs r0,r4,r0    @                  // Q29-Q28/2

 asrs r2,#1       @ halve exponent
 bcc div0         @ was y shifted?
 lsrs r0,#1
 lsls r1,#1       @ shift y back
 b div0           @ round and complete with multiplication

.align 2

@ round(2^15./[136:16:248])
rcpapp:
.byte 0xf1,0xd8,0xc3,0xb2, 0xa4,0x98,0x8d,0x84

@ round(sqrt(2^22./[72:16:248]))
rsqrtapp:
.byte 0xf1,0xda,0xc9,0xbb, 0xb0,0xa6,0x9e,0x97, 0x91,0x8b,0x86,0x82

.endif

.if include_conversions

@ convert float to signed int, rounding towards -Inf, clamping
.thumb_func
qfp_float2int:
 movs r1,#0      @ fall through

@ convert float in r0 to signed fixed point in r0, clamping
.thumb_func
qfp_float2fix:
 push {r4,r14}
 bl unpackx
 add r2,r1       @ incorporate binary point position into exponent
 subs r2,#23     @ r2 is now amount of left shift required
 blt 1f          @ requires right shift?
 cmp r2,#7       @ overflow?
 ble 4f
3:               @ overflow
 asrs r1,r0,#31  @ +ve:0 -ve:0xffffffff
 mvns r1,r1      @ +ve:0xffffffff -ve:0
 movs r0,#1
 lsls r0,#31
5:
 eors r0,r1      @ +ve:0x7fffffff -ve:0x80000000 (unsigned path: 0xffffffff)
 pop {r4,r15}
1:
 rsbs r2,#0      @ right shift for r0, >0
 cmp r2,#32
 blt 2f          @ more than 32 bits of right shift?
 movs r2,#32
2:
 asrs r0,r0,r2
 pop {r4,r15}

@ unsigned version
.thumb_func
qfp_float2uint:
 movs r1,#0      @ fall through
.thumb_func
qfp_float2ufix:
 push {r4,r14}
 bl unpackx
 add r2,r1       @ incorporate binary point position into exponent
 movs r1,r0
 bmi 5b          @ negative? return zero
 subs r2,#23     @ r2 is now amount of left shift required
 blt 1b          @ requires right shift?
 mvns r1,r0      @ ready to return 0xffffffff
 cmp r2,#8       @ overflow?
 bgt 5b
4:
 lsls r0,r0,r2   @ result fits, left shifted
 pop {r4,r15}

@ convert signed int to float, rounding
.thumb_func
qfp_int2float:
 movs r1,#0      @ fall through

@ convert signed fix to float, rounding; number of r0 bits after point in r1
.thumb_func
qfp_fix2float:
 push {r4,r5,r14}
1:
 movs r2,#29
 subs r2,r1      @ fix exponent
packretns:       @ pack and return, sticky bits=0
 movs r5,#0
 b packret

@ unsigned version
.thumb_func
qfp_uint2float:
 movs r1,#0      @ fall through
.thumb_func
qfp_ufix2float:
 push {r4,r5,r14}
 cmp r0,#0
 bge 1b          @ treat <2^31 as signed
 movs r2,#30
 subs r2,r1      @ fix exponent
 lsls r5,r0,#31  @ one sticky bit
 lsrs r0,#1
 b packret

.endif

.if include_scientific

@ All the scientific functions are implemented using the CORDIC algorithm. For notation,
@ details not explained in the comments below, and a good overall survey see
@ "50 Years of CORDIC: Algorithms, Architectures, and Applications" by Meher et al.,
@ IEEE Transactions on Circuits and Systems Part I, Volume 56 Issue 9.

@ Register use:
@ r0: x
@ r1: y
@ r2: z/omega
@ r3: coefficient pointer
@ r4,r12: m
@ r5: i (shift)

cordic_start: @ initialisation
 movs r5,#0   @ initial shift=0
 mov r12,r4
 b 5f

cordic_vstep: @ one step of algorithm in vector mode
 cmp r1,#0    @ check sign of y
 bgt 4f
 b 1f
cordic_rstep: @ one step of algorithm in rotation mode
 cmp r2,#0    @ check sign of angle
 bge 1f
4:
 subs r1,r6   @ negative rotation: y=y-(x>>i)
 rsbs r7,#0
 adds r2,r4   @ accumulate angle
 b 2f
1:
 adds r1,r6   @ positive rotation: y=y+(x>>i)
 subs r2,r4   @ accumulate angle
2:
 mov r4,r12
 muls r7,r4   @ apply sign from m
 subs r0,r7   @ finish rotation: x=x{+/-}(y>>i)
5:
 ldmia r3!,{r4}   @ fetch next angle from table and bump pointer
 lsrs r4,#1   @ repeated angle?
 bcs 3f
 adds r5,#1   @ adjust shift if not
3:
 mov r6,r0
 asrs r6,r5   @ x>>i
 mov r7,r1
 asrs r7,r5   @ y>>i
 lsrs r4,#1   @ shift end flag into carry
 bx r14

@ CORDIC rotation mode
cordic_rot:
 push {r6,r7,r14}
 bl cordic_start   @ initialise
1:
 bl cordic_rstep
 bcc 1b            @ step until table finished
 asrs r6,r0,#14    @ remaining small rotations can be linearised: see IV.B of paper referenced above
 asrs r7,r1,#14
 asrs r2,#3
 muls r6,r2        @ all remaining CORDIC steps in a multiplication
 muls r7,r2
 mov r4,r12
 muls r7,r4
 asrs r6,#12
 asrs r7,#12
 subs r0,r7        @ x=x{+/-}(yz>>k)
 adds r1,r6        @ y=y+(xz>>k)
cordic_exit:
 pop {r6,r7,r15}

@ CORDIC vector mode
cordic_vec:
 push {r6,r7,r14}
 bl cordic_start   @ initialise
1:
 bl cordic_vstep
 bcc 1b            @ step until table finished
4:
 cmp r1,#0         @ continue as in cordic_vstep but without using table; x is not affected as y is small
 bgt 2f            @ check sign of y
 adds r1,r6        @ positive rotation: y=y+(x>>i)
 subs r2,r4        @ accumulate angle
 b 3f
2:
 subs r1,r6        @ negative rotation: y=y-(x>>i)
 adds r2,r4        @ accumulate angle
3:
 asrs r6,#1
 asrs r4,#1        @ next "table entry"
 bne 4b
 b cordic_exit

.thumb_func
qfp_fsin:            @ calculate sin and cos using CORDIC rotation method
 push {r4,r5,r14}
 movs r1,#24
 bl qfp_float2fix    @ range reduction by repeated subtraction/addition in fixed point
 ldr r4,pi_q29
 lsrs r4,#4          @ 2pi Q24
1:
 subs r0,r4
 bge 1b
1:
 adds r0,r4
 bmi 1b              @ now in range 0..2pi
 lsls r2,r0,#2       @ z Q26
 lsls r5,r4,#1       @ pi Q26 (r4=pi/2 Q26)
 ldr r0,=#0x136e9db4 @ initialise CORDIC x,y with scaling
 movs r1,#0
1:
 cmp r2,r4           @ >pi/2?
 blt 2f
 subs r2,r5          @ reduce range to -pi/2..pi/2
 rsbs r0,#0          @ rotate vector by pi
 b 1b
2:
 lsls r2,#3          @ Q29
 adr r3,tab_cc       @ circular coefficients
 movs r4,#1          @ m=1
 bl cordic_rot
 adds r1,#9          @ fiddle factor to make sin(0)==0
 movs r2,#0          @ exponents to zero
 movs r3,#0
 movs r5,#0          @ no sticky bits
 bl packx            @ pack cosine
 bl xchxy
 b packretns         @ pack sine

.thumb_func
qfp_fcos:
 push {r14}
 bl qfp_fsin
 mov r0,r1           @ extract cosine result
 pop {r15}


.thumb_func
qfp_ftan:
 push {r4,r5,r14}
 bl qfp_fsin         @ sine in r0/r2, cosine in r1/r3
.if include_faster
 b fdiv_fast_n       @ sin/cos
.else
 b fdiv_n

.endif

.thumb_func
qfp_fexp:            @ calculate cosh and sinh using rotation method; add to obtain exp
 push {r4,r5,r14}
 movs r1,#24
 bl qfp_float2fix    @ Q24: covers entire valid input range
 asrs r1,r0,#16      @ Q8
 ldr r2,=#5909       @ log_2(e) Q12
 muls r1,r2          @ estimate exponent of result Q20
 asrs r1,#19         @ Q1
 adds r1,#1          @ rounding
 asrs r1,#1          @ rounded estimate of exponent of result
 push {r1}           @ save for later
 lsls r2,r0,#5       @ Q29
 ldr r0,=#0x162e42ff @ ln(2) Q29
 muls r1,r0          @ accurate contribution of estimated exponent
 subs r2,r1          @ residual to be exponentiated, approximately -.5..+.5 Q29
 ldr r0,=#0x2c9e15ca @ initialise CORDIC x,y with scaling
 movs r1,#0
 adr r3,tab_ch       @ hyperbolic coefficients
 mvns r4,r1          @ m=-1
 bl cordic_rot       @ calculate cosh and sinh
 add r0,r1           @ exp=cosh+sinh
 pop {r2}            @ recover exponent
 b packretns         @ pack result

.thumb_func
qfp_fsqrt:           @ calculate sqrt and ln using vector method
 push {r4,r5,r14}
 bl unpackx
 movs r1,r0          @ -ve argument?
 bmi 3f              @ return -Inf, -Inf
 ldr r1,=#0x0593C2B9 @ scale factor for CORDIC
 bl mul0             @ Q29
 asrs r1,r2,#1       @ halve exponent
 bcc 1f
 adds r1,#1          @ was odd: add 1 and shift mantissa
 asrs r0,#1
1:
 push {r1}           @ save exponent/2 for later
 mov r1,r0
 ldr r3,=#0x0593C2B9 @ re-use constant
 lsls r3,#2
 adds r0,r3          @ "a+1"
 subs r1,r3          @ "a-1"
 movs r2,#0
 adr r3,tab_ch       @ hyperbolic coefficients
 mvns r4,r2          @ m=-1
 bl cordic_vec
 mov r1,r2           @ keep ln result
 pop {r2}            @ retrieve exponent/2
2:
 movs r3,r2
 b packretns         @ pack sqrt result

3:
 movs r2,#255
 b 2b

.thumb_func
qfp_fln:
 push {r4,r5,r14}
 bl qfp_fsqrt            @ get unpacked ln in r1/r3; exponent has been halved
 cmp r3,#70              @ ln(Inf)?
 bgt 3b                  @ return Inf
 rsbs r3,#0
 cmp r3,#70
 bgt 1f                  @ ln(0)? return -Inf
 ldr r0,=#0x0162e430     @ ln(4) Q24
 muls r0,r3              @ contribution from negated, halved exponent
 adds r1,#8              @ round result of ln
 asrs r1,#4              @ Q24
 subs r0,r1,r0           @ add in contribution from (negated) exponent
 movs r2,#5              @ pack expects Q29
 b packretns
1:
 mvns r0,r0              @ make result -Inf
 b 3b

.thumb_func
qfp_fatan2:
 push {r4,r5,r14}
 bl unpackxyalign        @ convert to fixed point (ensure common exponent, which is discarded)
 asrs r0,#1
 asrs r1,#1
 movs r2,#0              @ initial angle
 cmp r0,#0               @ x negative
 bge 5f
 rsbs r0,#0              @ rotate to 1st/4th quadrants
 rsbs r1,#0
 ldr r2,pi_q29           @ pi Q29
5:
 adr r3,tab_cc           @ circular coefficients
 movs r4,#1              @ m=1
 bl cordic_vec           @ also produces magnitude (with scaling factor 1.646760119), which is discarded
 mov r0,r2               @ result here is -pi/2..3pi/2 Q29
 ldr r2,pi_q29           @ pi Q29
 adds r4,r0,r2           @ attempt to fix -3pi/2..-pi case
 bcs 6f                  @ -pi/2..0? leave result as is
 subs r4,r0,r2           @ <pi? leave as is
 bmi 6f
 subs r0,r4,r2           @ >pi: take off 2pi
6:
 subs r0,#1              @ fiddle factor so atan2(0,1)==0
 movs r2,#0              @ exponent for pack
 b packretns

.align 2
.ltorg

@ first entry in following table is pi Q29
pi_q29:
@ circular CORDIC coefficients: atan(2^-i), b0=flag for preventing shift, b1=flag for end of table
tab_cc:
.word 0x1921fb54*4+1     @ no shift before first iteration
.word 0x0ed63383*4+0
.word 0x07d6dd7e*4+0
.word 0x03fab753*4+0
.word 0x01ff55bb*4+0
.word 0x00ffeaae*4+0
.word 0x007ffd55*4+0
.word 0x003fffab*4+0
.word 0x001ffff5*4+0
.word 0x000fffff*4+0
.word 0x0007ffff*4+0
.word 0x00040000*4+0
.word 0x00020000*4+0+2   @ +2 marks end

@ hyperbolic CORDIC coefficients: atanh(2^-i), flags as above
tab_ch:
.word 0x1193ea7b*4+0
.word 0x1193ea7b*4+1   @ repeat i=1
.word 0x082c577d*4+0
.word 0x04056247*4+0
.word 0x0200ab11*4+0
.word 0x0200ab11*4+1   @ repeat i=4
.word 0x01001559*4+0
.word 0x008002ab*4+0
.word 0x00400055*4+0
.word 0x0020000b*4+0
.word 0x00100001*4+0
.word 0x00080001*4+0
.word 0x00040000*4+0
.word 0x00020000*4+0
.word 0x00020000*4+1+2 @ repeat i=12

.endif

qfp_lib_end:
