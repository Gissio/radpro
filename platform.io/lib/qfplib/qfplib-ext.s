.syntax unified
.cpu cortex-m0
.thumb

@ exported symbols

.global cmath_fcmpeq
.global cmath_fcmplt
.global cmath_fcmple
.global cmath_fcmpge
.global cmath_fcmpgt
.global cmath_fcmpun

cmath_fcmpeq:
    push    {r3, lr}
    bl      qfp_fcmp
    negs	r3, r0
    adcs	r0, r3
    pop     {r3, pc}

cmath_fcmplt:
    push    {r3, lr}
    bl     qfp_fcmp
    lsrs    r0, r0, #31
    pop     {r3, pc}

cmath_fcmple:
    push    {r3, lr}
    bl      qfp_fcmp
    subs	r3, r0, #1
    orrs	r0, r3
    lsrs	r0, r0, #31
    pop     {r3, pc}

cmath_fcmpge:
    push    {r3, lr}
    bl      qfp_fcmp
    mvns	r0, r0
    lsrs	r0, r0, #31
    pop     {r3, pc}

cmath_fcmpgt:
    push    {r3, lr}
    bl      qfp_fcmp
    asrs	r3, r0, #31
    subs	r0, r3, r0
    lsrs	r0, r0, #31
    pop     {r3, pc}

cmath_fcmpun:
    push    {lr}
    movs    r0, #0
    pop     {pc}
