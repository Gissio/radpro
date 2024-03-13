
.pio/build/fs2011-gd32f150c8/firmware.elf:     file format elf32-littlearm


Disassembly of section .isr_vector:

08000000 <g_pfnVectors>:
 8000000:	20002000 	andcs	r2, r0, r0
 8000004:	080005d1 	stmdaeq	r0, {r0, r4, r6, r7, r8, sl}
 8000008:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 800000c:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
	...
 800002c:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
	...
 8000038:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 800003c:	08004765 	stmdaeq	r0, {r0, r2, r5, r6, r8, r9, sl, lr}
 8000040:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000044:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000048:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 800004c:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000050:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000054:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000058:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 800005c:	08004cb5 	stmdaeq	r0, {r0, r2, r4, r5, r7, sl, fp, lr}
 8000060:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000064:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000068:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 800006c:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000070:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000074:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000078:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 800007c:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000080:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000084:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000088:	00000000 	andeq	r0, r0, r0
 800008c:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000090:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000094:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 8000098:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 800009c:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 80000a0:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 80000a4:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 80000a8:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 80000ac:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 80000b0:	080045f9 	stmdaeq	r0, {r0, r3, r4, r5, r6, r7, r8, sl, lr}
 80000b4:	00000000 	andeq	r0, r0, r0
 80000b8:	08000621 	stmdaeq	r0, {r0, r5, r9, sl}
 80000bc:	00000000 	andeq	r0, r0, r0

Disassembly of section .text:

080000c0 <__do_global_dtors_aux>:
 80000c0:	b510      	push	{r4, lr}
 80000c2:	4c05      	ldr	r4, [pc, #20]	; (80000d8 <__do_global_dtors_aux+0x18>)
 80000c4:	7823      	ldrb	r3, [r4, #0]
 80000c6:	b933      	cbnz	r3, 80000d6 <__do_global_dtors_aux+0x16>
 80000c8:	4b04      	ldr	r3, [pc, #16]	; (80000dc <__do_global_dtors_aux+0x1c>)
 80000ca:	b113      	cbz	r3, 80000d2 <__do_global_dtors_aux+0x12>
 80000cc:	4804      	ldr	r0, [pc, #16]	; (80000e0 <__do_global_dtors_aux+0x20>)
 80000ce:	f3af 8000 	nop.w
 80000d2:	2301      	movs	r3, #1
 80000d4:	7023      	strb	r3, [r4, #0]
 80000d6:	bd10      	pop	{r4, pc}
 80000d8:	20000070 	andcs	r0, r0, r0, ror r0
 80000dc:	00000000 	andeq	r0, r0, r0
 80000e0:	0800606c 	stmdaeq	r0, {r2, r3, r5, r6, sp, lr}

080000e4 <frame_dummy>:
 80000e4:	b508      	push	{r3, lr}
 80000e6:	4b03      	ldr	r3, [pc, #12]	; (80000f4 <frame_dummy+0x10>)
 80000e8:	b11b      	cbz	r3, 80000f2 <frame_dummy+0xe>
 80000ea:	4903      	ldr	r1, [pc, #12]	; (80000f8 <frame_dummy+0x14>)
 80000ec:	4803      	ldr	r0, [pc, #12]	; (80000fc <frame_dummy+0x18>)
 80000ee:	f3af 8000 	nop.w
 80000f2:	bd08      	pop	{r3, pc}
 80000f4:	00000000 	andeq	r0, r0, r0
 80000f8:	20000074 	andcs	r0, r0, r4, ror r0
 80000fc:	0800606c 	stmdaeq	r0, {r2, r3, r5, r6, sp, lr}

08000100 <xchxy>:
 8000100:	b505      	push	{r0, r2, lr}
 8000102:	4608      	mov	r0, r1
 8000104:	461a      	mov	r2, r3
 8000106:	bd0a      	pop	{r1, r3, pc}

08000108 <unpackxy>:
 8000108:	b500      	push	{lr}
 800010a:	f000 f804 	bl	8000116 <unpackx>
 800010e:	f7ff fff7 	bl	8000100 <xchxy>
 8000112:	bc10      	pop	{r4}
 8000114:	46a6      	mov	lr, r4

08000116 <unpackx>:
 8000116:	0dc2      	lsrs	r2, r0, #23
 8000118:	0240      	lsls	r0, r0, #9
 800011a:	0a40      	lsrs	r0, r0, #9
 800011c:	2401      	movs	r4, #1
 800011e:	05e4      	lsls	r4, r4, #23
 8000120:	4320      	orrs	r0, r4
 8000122:	2aff      	cmp	r2, #255	; 0xff
 8000124:	b2d2      	uxtb	r2, r2
 8000126:	d900      	bls.n	800012a <unpackx+0x14>
 8000128:	4240      	negs	r0, r0
 800012a:	3a01      	subs	r2, #1
 800012c:	2afe      	cmp	r2, #254	; 0xfe
 800012e:	d201      	bcs.n	8000134 <unpackx+0x1e>
 8000130:	3a7e      	subs	r2, #126	; 0x7e
 8000132:	4770      	bx	lr
 8000134:	2800      	cmp	r0, #0
 8000136:	4620      	mov	r0, r4
 8000138:	d500      	bpl.n	800013c <unpackx+0x26>
 800013a:	4240      	negs	r0, r0
 800013c:	3a7e      	subs	r2, #126	; 0x7e
 800013e:	0092      	lsls	r2, r2, #2
 8000140:	3280      	adds	r2, #128	; 0x80
 8000142:	4770      	bx	lr

08000144 <packx>:
 8000144:	0fc4      	lsrs	r4, r0, #31
 8000146:	07e4      	lsls	r4, r4, #31
 8000148:	d504      	bpl.n	8000154 <packx0+0x2>
 800014a:	2d00      	cmp	r5, #0
 800014c:	d000      	beq.n	8000150 <packx+0xc>
 800014e:	3001      	adds	r0, #1
 8000150:	4240      	negs	r0, r0

08000152 <packx0>:
 8000152:	d403      	bmi.n	800015c <packx0+0xa>
 8000154:	3a01      	subs	r2, #1
 8000156:	1800      	adds	r0, r0, r0
 8000158:	d012      	beq.n	8000180 <packx0+0x2e>
 800015a:	d5fb      	bpl.n	8000154 <packx0+0x2>
 800015c:	3281      	adds	r2, #129	; 0x81
 800015e:	d101      	bne.n	8000164 <packx0+0x12>
 8000160:	3080      	adds	r0, #128	; 0x80
 8000162:	d205      	bcs.n	8000170 <packx0+0x1e>
 8000164:	3080      	adds	r0, #128	; 0x80
 8000166:	d203      	bcs.n	8000170 <packx0+0x1e>
 8000168:	2d00      	cmp	r5, #0
 800016a:	d00f      	beq.n	800018c <packx0+0x3a>
 800016c:	0040      	lsls	r0, r0, #1
 800016e:	3a01      	subs	r2, #1
 8000170:	2afe      	cmp	r2, #254	; 0xfe
 8000172:	da06      	bge.n	8000182 <packx0+0x30>
 8000174:	3201      	adds	r2, #1
 8000176:	dd07      	ble.n	8000188 <packx0+0x36>
 8000178:	0a40      	lsrs	r0, r0, #9
 800017a:	05d2      	lsls	r2, r2, #23
 800017c:	4310      	orrs	r0, r2
 800017e:	4320      	orrs	r0, r4
 8000180:	4770      	bx	lr
 8000182:	20ff      	movs	r0, #255	; 0xff
 8000184:	05c0      	lsls	r0, r0, #23
 8000186:	e7fa      	b.n	800017e <packx0+0x2c>
 8000188:	2000      	movs	r0, #0
 800018a:	4770      	bx	lr
 800018c:	0605      	lsls	r5, r0, #24
 800018e:	d1ed      	bne.n	800016c <packx0+0x1a>
 8000190:	0a40      	lsrs	r0, r0, #9
 8000192:	0280      	lsls	r0, r0, #10
 8000194:	e7eb      	b.n	800016e <packx0+0x1c>

08000196 <unpackxyalign>:
 8000196:	b500      	push	{lr}
 8000198:	f7ff ffb6 	bl	8000108 <unpackxy>
 800019c:	0180      	lsls	r0, r0, #6
 800019e:	0189      	lsls	r1, r1, #6
 80001a0:	1ad4      	subs	r4, r2, r3
 80001a2:	da07      	bge.n	80001b4 <unpackxyalign+0x1e>
 80001a4:	461a      	mov	r2, r3
 80001a6:	4605      	mov	r5, r0
 80001a8:	4264      	negs	r4, r4
 80001aa:	4120      	asrs	r0, r4
 80001ac:	2c20      	cmp	r4, #32
 80001ae:	d307      	bcc.n	80001c0 <unpackxyalign+0x2a>
 80001b0:	2000      	movs	r0, #0
 80001b2:	bd00      	pop	{pc}
 80001b4:	460d      	mov	r5, r1
 80001b6:	4121      	asrs	r1, r4
 80001b8:	2c20      	cmp	r4, #32
 80001ba:	d301      	bcc.n	80001c0 <unpackxyalign+0x2a>
 80001bc:	2100      	movs	r1, #0
 80001be:	bd00      	pop	{pc}
 80001c0:	4264      	negs	r4, r4
 80001c2:	3420      	adds	r4, #32
 80001c4:	40a5      	lsls	r5, r4
 80001c6:	bd00      	pop	{pc}

080001c8 <qfp_fsub>:
 80001c8:	2201      	movs	r2, #1
 80001ca:	07d2      	lsls	r2, r2, #31
 80001cc:	4051      	eors	r1, r2

080001ce <qfp_fadd>:
 80001ce:	b530      	push	{r4, r5, lr}
 80001d0:	f7ff ffe1 	bl	8000196 <unpackxyalign>
 80001d4:	1840      	adds	r0, r0, r1
 80001d6:	d103      	bne.n	80001e0 <packret>
 80001d8:	2ac8      	cmp	r2, #200	; 0xc8
 80001da:	db01      	blt.n	80001e0 <packret>
 80001dc:	2001      	movs	r0, #1
 80001de:	0740      	lsls	r0, r0, #29

080001e0 <packret>:
 80001e0:	f7ff ffb0 	bl	8000144 <packx>
 80001e4:	bd30      	pop	{r4, r5, pc}

080001e6 <mul0>:
 80001e6:	b283      	uxth	r3, r0
 80001e8:	140c      	asrs	r4, r1, #16
 80001ea:	4363      	muls	r3, r4
 80001ec:	1404      	asrs	r4, r0, #16
 80001ee:	b28d      	uxth	r5, r1
 80001f0:	436c      	muls	r4, r5
 80001f2:	191b      	adds	r3, r3, r4
 80001f4:	b284      	uxth	r4, r0
 80001f6:	436c      	muls	r4, r5
 80001f8:	0425      	lsls	r5, r4, #16
 80001fa:	0c24      	lsrs	r4, r4, #16
 80001fc:	191b      	adds	r3, r3, r4
 80001fe:	1400      	asrs	r0, r0, #16
 8000200:	1409      	asrs	r1, r1, #16
 8000202:	4348      	muls	r0, r1
 8000204:	02c0      	lsls	r0, r0, #11
 8000206:	06d9      	lsls	r1, r3, #27
 8000208:	430d      	orrs	r5, r1
 800020a:	1159      	asrs	r1, r3, #5
 800020c:	1840      	adds	r0, r0, r1
 800020e:	4770      	bx	lr

08000210 <qfp_fcmp>:
 8000210:	2201      	movs	r2, #1
 8000212:	07d3      	lsls	r3, r2, #31
 8000214:	4218      	tst	r0, r3
 8000216:	d000      	beq.n	800021a <qfp_fcmp+0xa>
 8000218:	1a18      	subs	r0, r3, r0
 800021a:	4219      	tst	r1, r3
 800021c:	d000      	beq.n	8000220 <qfp_fcmp+0x10>
 800021e:	1a59      	subs	r1, r3, r1
 8000220:	1a40      	subs	r0, r0, r1
 8000222:	d002      	beq.n	800022a <qfp_fcmp+0x1a>
 8000224:	dc00      	bgt.n	8000228 <qfp_fcmp+0x18>
 8000226:	4252      	negs	r2, r2
 8000228:	4610      	mov	r0, r2
 800022a:	4770      	bx	lr

0800022c <qfp_fmul>:
 800022c:	b530      	push	{r4, r5, lr}
 800022e:	f7ff ff6b 	bl	8000108 <unpackxy>
 8000232:	441a      	add	r2, r3
 8000234:	3204      	adds	r2, #4

08000236 <fmul0>:
 8000236:	f7ff ffd6 	bl	80001e6 <mul0>
 800023a:	e7d1      	b.n	80001e0 <packret>

0800023c <qfp_fdiv>:
 800023c:	b530      	push	{r4, r5, lr}

0800023e <fdiv_n>:
 800023e:	f7ff ff63 	bl	8000108 <unpackxy>
 8000242:	2501      	movs	r5, #1
 8000244:	2800      	cmp	r0, #0
 8000246:	d501      	bpl.n	800024c <fdiv_n+0xe>
 8000248:	4240      	negs	r0, r0
 800024a:	4249      	negs	r1, r1
 800024c:	2400      	movs	r4, #0
 800024e:	2900      	cmp	r1, #0
 8000250:	d501      	bpl.n	8000256 <fdiv_n+0x18>
 8000252:	4249      	negs	r1, r1
 8000254:	43e4      	mvns	r4, r4
 8000256:	2bc8      	cmp	r3, #200	; 0xc8
 8000258:	da0e      	bge.n	8000278 <infret+0x2>
 800025a:	4252      	negs	r2, r2
 800025c:	2ac8      	cmp	r2, #200	; 0xc8
 800025e:	da0b      	bge.n	8000278 <infret+0x2>
 8000260:	18d2      	adds	r2, r2, r3
 8000262:	2340      	movs	r3, #64	; 0x40
 8000264:	1a09      	subs	r1, r1, r0
 8000266:	d200      	bcs.n	800026a <fdiv_n+0x2c>
 8000268:	4401      	add	r1, r0
 800026a:	415b      	adcs	r3, r3
 800026c:	4409      	add	r1, r1
 800026e:	d3f9      	bcc.n	8000264 <fdiv_n+0x26>
 8000270:	0118      	lsls	r0, r3, #4
 8000272:	4060      	eors	r0, r4
 8000274:	e7b4      	b.n	80001e0 <packret>

08000276 <infret>:
 8000276:	2400      	movs	r4, #0
 8000278:	4608      	mov	r0, r1
 800027a:	22ff      	movs	r2, #255	; 0xff
 800027c:	e7f9      	b.n	8000272 <fdiv_n+0x34>

0800027e <qfp_fdiv_fast>:
 800027e:	b530      	push	{r4, r5, lr}

08000280 <fdiv_fast_n>:
 8000280:	f7ff ff42 	bl	8000108 <unpackxy>
 8000284:	2800      	cmp	r0, #0
 8000286:	d501      	bpl.n	800028c <fdiv_fast_n+0xc>
 8000288:	4240      	negs	r0, r0
 800028a:	4249      	negs	r1, r1
 800028c:	2bc8      	cmp	r3, #200	; 0xc8
 800028e:	daf2      	bge.n	8000276 <infret>
 8000290:	4252      	negs	r2, r2
 8000292:	2ac8      	cmp	r2, #200	; 0xc8
 8000294:	daef      	bge.n	8000276 <infret>
 8000296:	18d2      	adds	r2, r2, r3
 8000298:	a423      	add	r4, pc, #140	; (adr r4, 8000328 <qfp_fsqrt_fast+0x5a>)
 800029a:	0d03      	lsrs	r3, r0, #20
 800029c:	5ce4      	ldrb	r4, [r4, r3]
 800029e:	00a3      	lsls	r3, r4, #2
 80002a0:	4343      	muls	r3, r0
 80002a2:	0224      	lsls	r4, r4, #8
 80002a4:	155b      	asrs	r3, r3, #21
 80002a6:	4363      	muls	r3, r4
 80002a8:	131b      	asrs	r3, r3, #12
 80002aa:	1ae4      	subs	r4, r4, r3
 80002ac:	4623      	mov	r3, r4
 80002ae:	4344      	muls	r4, r0
 80002b0:	1424      	asrs	r4, r4, #16
 80002b2:	435c      	muls	r4, r3
 80002b4:	021b      	lsls	r3, r3, #8
 80002b6:	13e4      	asrs	r4, r4, #15
 80002b8:	1b1b      	subs	r3, r3, r4
 80002ba:	01dc      	lsls	r4, r3, #7
 80002bc:	4343      	muls	r3, r0
 80002be:	13db      	asrs	r3, r3, #15
 80002c0:	0c20      	lsrs	r0, r4, #16
 80002c2:	4343      	muls	r3, r0
 80002c4:	141b      	asrs	r3, r3, #16
 80002c6:	1ae0      	subs	r0, r4, r3

080002c8 <div0>:
 80002c8:	3007      	adds	r0, #7
 80002ca:	0900      	lsrs	r0, r0, #4
 80002cc:	e7b3      	b.n	8000236 <fmul0>

080002ce <qfp_fsqrt_fast>:
 80002ce:	b530      	push	{r4, r5, lr}
 80002d0:	f7ff ff21 	bl	8000116 <unpackx>
 80002d4:	0001      	movs	r1, r0
 80002d6:	d4ce      	bmi.n	8000276 <infret>
 80002d8:	1050      	asrs	r0, r2, #1
 80002da:	d300      	bcc.n	80002de <qfp_fsqrt_fast+0x10>
 80002dc:	0049      	lsls	r1, r1, #1
 80002de:	3204      	adds	r2, #4
 80002e0:	a414      	add	r4, pc, #80	; (adr r4, 8000334 <rcpapp+0x4>)
 80002e2:	0d4b      	lsrs	r3, r1, #21
 80002e4:	5ce4      	ldrb	r4, [r4, r3]
 80002e6:	09c8      	lsrs	r0, r1, #7
 80002e8:	4360      	muls	r0, r4
 80002ea:	4360      	muls	r0, r4
 80002ec:	1300      	asrs	r0, r0, #12
 80002ee:	4360      	muls	r0, r4
 80002f0:	1340      	asrs	r0, r0, #13
 80002f2:	0224      	lsls	r4, r4, #8
 80002f4:	1a24      	subs	r4, r4, r0
 80002f6:	4620      	mov	r0, r4
 80002f8:	4340      	muls	r0, r0
 80002fa:	0c00      	lsrs	r0, r0, #16
 80002fc:	09cb      	lsrs	r3, r1, #7
 80002fe:	4358      	muls	r0, r3
 8000300:	1300      	asrs	r0, r0, #12
 8000302:	4360      	muls	r0, r4
 8000304:	1540      	asrs	r0, r0, #21
 8000306:	1a24      	subs	r4, r4, r0
 8000308:	4620      	mov	r0, r4
 800030a:	4340      	muls	r0, r0
 800030c:	0b03      	lsrs	r3, r0, #12
 800030e:	434b      	muls	r3, r1
 8000310:	0500      	lsls	r0, r0, #20
 8000312:	0d00      	lsrs	r0, r0, #20
 8000314:	0b0d      	lsrs	r5, r1, #12
 8000316:	4368      	muls	r0, r5
 8000318:	4418      	add	r0, r3
 800031a:	13c0      	asrs	r0, r0, #15
 800031c:	4360      	muls	r0, r4
 800031e:	0364      	lsls	r4, r4, #13
 8000320:	1400      	asrs	r0, r0, #16
 8000322:	1a20      	subs	r0, r4, r0
 8000324:	1052      	asrs	r2, r2, #1
 8000326:	d3cf      	bcc.n	80002c8 <div0>
 8000328:	0840      	lsrs	r0, r0, #1
 800032a:	0049      	lsls	r1, r1, #1
 800032c:	e7cc      	b.n	80002c8 <div0>
 800032e:	46c0      	nop			; (mov r8, r8)

08000330 <rcpapp>:
 8000330:	b2c3d8f1 	sbclt	sp, r3, #15794176	; 0xf10000
 8000334:	848d98a4 	strhi	r9, [sp], #2212	; 0x8a4

08000338 <rsqrtapp>:
 8000338:	bbc9daf1 	bllt	7276f04 <_Min_Stack_Size+0x7276b04>
 800033c:	979ea6b0 			; <UNDEFINED> instruction: 0x979ea6b0
 8000340:	82868b91 	addhi	r8, r6, #148480	; 0x24400

08000344 <qfp_float2int>:
 8000344:	2100      	movs	r1, #0

08000346 <qfp_float2fix>:
 8000346:	b510      	push	{r4, lr}
 8000348:	f7ff fee5 	bl	8000116 <unpackx>
 800034c:	440a      	add	r2, r1
 800034e:	3a17      	subs	r2, #23
 8000350:	db07      	blt.n	8000362 <qfp_float2fix+0x1c>
 8000352:	2a07      	cmp	r2, #7
 8000354:	dd17      	ble.n	8000386 <qfp_float2ufix+0x16>
 8000356:	17c1      	asrs	r1, r0, #31
 8000358:	43c9      	mvns	r1, r1
 800035a:	2001      	movs	r0, #1
 800035c:	07c0      	lsls	r0, r0, #31
 800035e:	4048      	eors	r0, r1
 8000360:	bd10      	pop	{r4, pc}
 8000362:	4252      	negs	r2, r2
 8000364:	2a20      	cmp	r2, #32
 8000366:	db00      	blt.n	800036a <qfp_float2fix+0x24>
 8000368:	2220      	movs	r2, #32
 800036a:	4110      	asrs	r0, r2
 800036c:	bd10      	pop	{r4, pc}

0800036e <qfp_float2uint>:
 800036e:	2100      	movs	r1, #0

08000370 <qfp_float2ufix>:
 8000370:	b510      	push	{r4, lr}
 8000372:	f7ff fed0 	bl	8000116 <unpackx>
 8000376:	440a      	add	r2, r1
 8000378:	0001      	movs	r1, r0
 800037a:	d4f0      	bmi.n	800035e <qfp_float2fix+0x18>
 800037c:	3a17      	subs	r2, #23
 800037e:	dbf0      	blt.n	8000362 <qfp_float2fix+0x1c>
 8000380:	43c1      	mvns	r1, r0
 8000382:	2a08      	cmp	r2, #8
 8000384:	dceb      	bgt.n	800035e <qfp_float2fix+0x18>
 8000386:	4090      	lsls	r0, r2
 8000388:	bd10      	pop	{r4, pc}

0800038a <qfp_int2float>:
 800038a:	2100      	movs	r1, #0

0800038c <qfp_fix2float>:
 800038c:	b530      	push	{r4, r5, lr}
 800038e:	221d      	movs	r2, #29
 8000390:	1a52      	subs	r2, r2, r1

08000392 <packretns>:
 8000392:	2500      	movs	r5, #0
 8000394:	e724      	b.n	80001e0 <packret>

08000396 <qfp_uint2float>:
 8000396:	2100      	movs	r1, #0

08000398 <qfp_ufix2float>:
 8000398:	b530      	push	{r4, r5, lr}
 800039a:	2800      	cmp	r0, #0
 800039c:	daf7      	bge.n	800038e <qfp_fix2float+0x2>
 800039e:	221e      	movs	r2, #30
 80003a0:	1a52      	subs	r2, r2, r1
 80003a2:	07c5      	lsls	r5, r0, #31
 80003a4:	0840      	lsrs	r0, r0, #1
 80003a6:	e71b      	b.n	80001e0 <packret>

080003a8 <cordic_start>:
 80003a8:	2500      	movs	r5, #0
 80003aa:	46a4      	mov	ip, r4
 80003ac:	e00d      	b.n	80003ca <cordic_rstep+0x16>

080003ae <cordic_vstep>:
 80003ae:	2900      	cmp	r1, #0
 80003b0:	dc02      	bgt.n	80003b8 <cordic_rstep+0x4>
 80003b2:	e005      	b.n	80003c0 <cordic_rstep+0xc>

080003b4 <cordic_rstep>:
 80003b4:	2a00      	cmp	r2, #0
 80003b6:	da03      	bge.n	80003c0 <cordic_rstep+0xc>
 80003b8:	1b89      	subs	r1, r1, r6
 80003ba:	427f      	negs	r7, r7
 80003bc:	1912      	adds	r2, r2, r4
 80003be:	e001      	b.n	80003c4 <cordic_rstep+0x10>
 80003c0:	1989      	adds	r1, r1, r6
 80003c2:	1b12      	subs	r2, r2, r4
 80003c4:	4664      	mov	r4, ip
 80003c6:	4367      	muls	r7, r4
 80003c8:	1bc0      	subs	r0, r0, r7
 80003ca:	cb10      	ldmia	r3!, {r4}
 80003cc:	0864      	lsrs	r4, r4, #1
 80003ce:	d200      	bcs.n	80003d2 <cordic_rstep+0x1e>
 80003d0:	3501      	adds	r5, #1
 80003d2:	4606      	mov	r6, r0
 80003d4:	412e      	asrs	r6, r5
 80003d6:	460f      	mov	r7, r1
 80003d8:	412f      	asrs	r7, r5
 80003da:	0864      	lsrs	r4, r4, #1
 80003dc:	4770      	bx	lr

080003de <cordic_rot>:
 80003de:	b5c0      	push	{r6, r7, lr}
 80003e0:	f7ff ffe2 	bl	80003a8 <cordic_start>
 80003e4:	f7ff ffe6 	bl	80003b4 <cordic_rstep>
 80003e8:	d3fc      	bcc.n	80003e4 <cordic_rot+0x6>
 80003ea:	1386      	asrs	r6, r0, #14
 80003ec:	138f      	asrs	r7, r1, #14
 80003ee:	10d2      	asrs	r2, r2, #3
 80003f0:	4356      	muls	r6, r2
 80003f2:	4357      	muls	r7, r2
 80003f4:	4664      	mov	r4, ip
 80003f6:	4367      	muls	r7, r4
 80003f8:	1336      	asrs	r6, r6, #12
 80003fa:	133f      	asrs	r7, r7, #12
 80003fc:	1bc0      	subs	r0, r0, r7
 80003fe:	1989      	adds	r1, r1, r6

08000400 <cordic_exit>:
 8000400:	bdc0      	pop	{r6, r7, pc}

08000402 <cordic_vec>:
 8000402:	b5c0      	push	{r6, r7, lr}
 8000404:	f7ff ffd0 	bl	80003a8 <cordic_start>
 8000408:	f7ff ffd1 	bl	80003ae <cordic_vstep>
 800040c:	d3fc      	bcc.n	8000408 <cordic_vec+0x6>
 800040e:	2900      	cmp	r1, #0
 8000410:	dc02      	bgt.n	8000418 <cordic_vec+0x16>
 8000412:	1989      	adds	r1, r1, r6
 8000414:	1b12      	subs	r2, r2, r4
 8000416:	e001      	b.n	800041c <cordic_vec+0x1a>
 8000418:	1b89      	subs	r1, r1, r6
 800041a:	1912      	adds	r2, r2, r4
 800041c:	1076      	asrs	r6, r6, #1
 800041e:	1064      	asrs	r4, r4, #1
 8000420:	d1f5      	bne.n	800040e <cordic_vec+0xc>
 8000422:	e7ed      	b.n	8000400 <cordic_exit>

08000424 <qfp_fsin>:
 8000424:	b530      	push	{r4, r5, lr}
 8000426:	2118      	movs	r1, #24
 8000428:	f7ff ff8d 	bl	8000346 <qfp_float2fix>
 800042c:	4c48      	ldr	r4, [pc, #288]	; (8000550 <pi_q29>)
 800042e:	0924      	lsrs	r4, r4, #4
 8000430:	1b00      	subs	r0, r0, r4
 8000432:	dafd      	bge.n	8000430 <qfp_fsin+0xc>
 8000434:	1900      	adds	r0, r0, r4
 8000436:	d4fd      	bmi.n	8000434 <qfp_fsin+0x10>
 8000438:	0082      	lsls	r2, r0, #2
 800043a:	0065      	lsls	r5, r4, #1
 800043c:	483e      	ldr	r0, [pc, #248]	; (8000538 <qfp_fatan2+0x34>)
 800043e:	2100      	movs	r1, #0
 8000440:	42a2      	cmp	r2, r4
 8000442:	db02      	blt.n	800044a <qfp_fsin+0x26>
 8000444:	1b52      	subs	r2, r2, r5
 8000446:	4240      	negs	r0, r0
 8000448:	e7fa      	b.n	8000440 <qfp_fsin+0x1c>
 800044a:	00d2      	lsls	r2, r2, #3
 800044c:	a340      	add	r3, pc, #256	; (adr r3, 8000550 <pi_q29>)
 800044e:	2401      	movs	r4, #1
 8000450:	f7ff ffc5 	bl	80003de <cordic_rot>
 8000454:	3109      	adds	r1, #9
 8000456:	2200      	movs	r2, #0
 8000458:	2300      	movs	r3, #0
 800045a:	2500      	movs	r5, #0
 800045c:	f7ff fe72 	bl	8000144 <packx>
 8000460:	f7ff fe4e 	bl	8000100 <xchxy>
 8000464:	e795      	b.n	8000392 <packretns>

08000466 <qfp_fcos>:
 8000466:	b500      	push	{lr}
 8000468:	f7ff ffdc 	bl	8000424 <qfp_fsin>
 800046c:	4608      	mov	r0, r1
 800046e:	bd00      	pop	{pc}

08000470 <qfp_ftan>:
 8000470:	b530      	push	{r4, r5, lr}
 8000472:	f7ff ffd7 	bl	8000424 <qfp_fsin>
 8000476:	e703      	b.n	8000280 <fdiv_fast_n>

08000478 <qfp_fexp>:
 8000478:	b530      	push	{r4, r5, lr}
 800047a:	2118      	movs	r1, #24
 800047c:	f7ff ff63 	bl	8000346 <qfp_float2fix>
 8000480:	1401      	asrs	r1, r0, #16
 8000482:	4a2e      	ldr	r2, [pc, #184]	; (800053c <qfp_fatan2+0x38>)
 8000484:	4351      	muls	r1, r2
 8000486:	14c9      	asrs	r1, r1, #19
 8000488:	3101      	adds	r1, #1
 800048a:	1049      	asrs	r1, r1, #1
 800048c:	b402      	push	{r1}
 800048e:	0142      	lsls	r2, r0, #5
 8000490:	482b      	ldr	r0, [pc, #172]	; (8000540 <qfp_fatan2+0x3c>)
 8000492:	4341      	muls	r1, r0
 8000494:	1a52      	subs	r2, r2, r1
 8000496:	482b      	ldr	r0, [pc, #172]	; (8000544 <qfp_fatan2+0x40>)
 8000498:	2100      	movs	r1, #0
 800049a:	a33a      	add	r3, pc, #232	; (adr r3, 8000584 <tab_ch>)
 800049c:	43cc      	mvns	r4, r1
 800049e:	f7ff ff9e 	bl	80003de <cordic_rot>
 80004a2:	4408      	add	r0, r1
 80004a4:	bc04      	pop	{r2}
 80004a6:	e774      	b.n	8000392 <packretns>

080004a8 <qfp_fsqrt>:
 80004a8:	b530      	push	{r4, r5, lr}
 80004aa:	f7ff fe34 	bl	8000116 <unpackx>
 80004ae:	0001      	movs	r1, r0
 80004b0:	d415      	bmi.n	80004de <qfp_fsqrt+0x36>
 80004b2:	4925      	ldr	r1, [pc, #148]	; (8000548 <qfp_fatan2+0x44>)
 80004b4:	f7ff fe97 	bl	80001e6 <mul0>
 80004b8:	1051      	asrs	r1, r2, #1
 80004ba:	d301      	bcc.n	80004c0 <qfp_fsqrt+0x18>
 80004bc:	3101      	adds	r1, #1
 80004be:	1040      	asrs	r0, r0, #1
 80004c0:	b402      	push	{r1}
 80004c2:	4601      	mov	r1, r0
 80004c4:	4b20      	ldr	r3, [pc, #128]	; (8000548 <qfp_fatan2+0x44>)
 80004c6:	009b      	lsls	r3, r3, #2
 80004c8:	18c0      	adds	r0, r0, r3
 80004ca:	1ac9      	subs	r1, r1, r3
 80004cc:	2200      	movs	r2, #0
 80004ce:	a32d      	add	r3, pc, #180	; (adr r3, 8000584 <tab_ch>)
 80004d0:	43d4      	mvns	r4, r2
 80004d2:	f7ff ff96 	bl	8000402 <cordic_vec>
 80004d6:	4611      	mov	r1, r2
 80004d8:	bc04      	pop	{r2}
 80004da:	0013      	movs	r3, r2
 80004dc:	e759      	b.n	8000392 <packretns>
 80004de:	22ff      	movs	r2, #255	; 0xff
 80004e0:	e7fb      	b.n	80004da <qfp_fsqrt+0x32>

080004e2 <qfp_fln>:
 80004e2:	b530      	push	{r4, r5, lr}
 80004e4:	f7ff ffe0 	bl	80004a8 <qfp_fsqrt>
 80004e8:	2b46      	cmp	r3, #70	; 0x46
 80004ea:	dcf8      	bgt.n	80004de <qfp_fsqrt+0x36>
 80004ec:	425b      	negs	r3, r3
 80004ee:	2b46      	cmp	r3, #70	; 0x46
 80004f0:	dc06      	bgt.n	8000500 <qfp_fln+0x1e>
 80004f2:	4816      	ldr	r0, [pc, #88]	; (800054c <qfp_fatan2+0x48>)
 80004f4:	4358      	muls	r0, r3
 80004f6:	3108      	adds	r1, #8
 80004f8:	1109      	asrs	r1, r1, #4
 80004fa:	1a08      	subs	r0, r1, r0
 80004fc:	2205      	movs	r2, #5
 80004fe:	e748      	b.n	8000392 <packretns>
 8000500:	43c0      	mvns	r0, r0
 8000502:	e7ec      	b.n	80004de <qfp_fsqrt+0x36>

08000504 <qfp_fatan2>:
 8000504:	b530      	push	{r4, r5, lr}
 8000506:	f7ff fe46 	bl	8000196 <unpackxyalign>
 800050a:	1040      	asrs	r0, r0, #1
 800050c:	1049      	asrs	r1, r1, #1
 800050e:	2200      	movs	r2, #0
 8000510:	2800      	cmp	r0, #0
 8000512:	da02      	bge.n	800051a <qfp_fatan2+0x16>
 8000514:	4240      	negs	r0, r0
 8000516:	4249      	negs	r1, r1
 8000518:	4a0d      	ldr	r2, [pc, #52]	; (8000550 <pi_q29>)
 800051a:	a30d      	add	r3, pc, #52	; (adr r3, 8000550 <pi_q29>)
 800051c:	2401      	movs	r4, #1
 800051e:	f7ff ff70 	bl	8000402 <cordic_vec>
 8000522:	4610      	mov	r0, r2
 8000524:	4a0a      	ldr	r2, [pc, #40]	; (8000550 <pi_q29>)
 8000526:	1884      	adds	r4, r0, r2
 8000528:	d202      	bcs.n	8000530 <qfp_fatan2+0x2c>
 800052a:	1a84      	subs	r4, r0, r2
 800052c:	d400      	bmi.n	8000530 <qfp_fatan2+0x2c>
 800052e:	1aa0      	subs	r0, r4, r2
 8000530:	3801      	subs	r0, #1
 8000532:	2200      	movs	r2, #0
 8000534:	e72d      	b.n	8000392 <packretns>
 8000536:	46c0      	nop			; (mov r8, r8)
 8000538:	136e9db4 	cmnne	lr, #180, 26	; 0x2d00
 800053c:	00001715 	andeq	r1, r0, r5, lsl r7
 8000540:	162e42ff 			; <UNDEFINED> instruction: 0x162e42ff
 8000544:	2c9e15ca 	cfldr32cs	mvfx1, [lr], {202}	; 0xca
 8000548:	0593c2b9 	ldreq	ip, [r3, #697]	; 0x2b9
 800054c:	0162e430 	cmneq	r2, r0, lsr r4

08000550 <pi_q29>:
 8000550:	6487ed51 	strvs	lr, [r7], #3409	; 0xd51
 8000554:	3b58ce0c 	blcc	9633d8c <_sidata+0x162c53c>
 8000558:	1f5b75f8 	svcne	0x005b75f8
 800055c:	0feadd4c 	svceq	0x00eadd4c
 8000560:	07fd56ec 	ldrbeq	r5, [sp, ip, ror #13]!
 8000564:	03ffaab8 	mvnseq	sl, #184, 20	; 0xb8000
 8000568:	01fff554 	mvnseq	pc, r4, asr r5	; <UNPREDICTABLE>
 800056c:	00fffeac 	rscseq	pc, pc, ip, lsr #29
 8000570:	007fffd4 	ldrsbteq	pc, [pc], #-244	; <UNPREDICTABLE>
 8000574:	003ffffc 	ldrshteq	pc, [pc], -ip	; <UNPREDICTABLE>
 8000578:	001ffffc 			; <UNDEFINED> instruction: 0x001ffffc
 800057c:	00100000 	andseq	r0, r0, r0
 8000580:	00080002 	andeq	r0, r8, r2

08000584 <tab_ch>:
 8000584:	464fa9ec 	strbmi	sl, [pc], -ip, ror #19
 8000588:	464fa9ed 	strbmi	sl, [pc], -sp, ror #19
 800058c:	20b15df4 	ldrshtcs	r5, [r1], r4
 8000590:	1015891c 	andsne	r8, r5, ip, lsl r9
 8000594:	0802ac44 	stmdaeq	r2, {r2, r6, sl, fp, sp, pc}
 8000598:	0802ac45 	stmdaeq	r2, {r0, r2, r6, sl, fp, sp, pc}
 800059c:	04005564 	streq	r5, [r0], #-1380	; 0xfffffa9c
 80005a0:	02000aac 	andeq	r0, r0, #172, 20	; 0xac000
 80005a4:	01000154 	tsteq	r0, r4, asr r1
 80005a8:	0080002c 	addeq	r0, r0, ip, lsr #32
 80005ac:	00400004 	subeq	r0, r0, r4
 80005b0:	00200004 	eoreq	r0, r0, r4
 80005b4:	00100000 	andseq	r0, r0, r0
 80005b8:	00080000 	andeq	r0, r8, r0
 80005bc:	00080003 	andeq	r0, r8, r3

080005c0 <strlen>:
 80005c0:	4603      	mov	r3, r0
 80005c2:	f813 2b01 	ldrb.w	r2, [r3], #1
 80005c6:	2a00      	cmp	r2, #0
 80005c8:	d1fb      	bne.n	80005c2 <strlen+0x2>
 80005ca:	1a18      	subs	r0, r3, r0
 80005cc:	3801      	subs	r0, #1
 80005ce:	4770      	bx	lr

080005d0 <Reset_Handler>:
 80005d0:	480d      	ldr	r0, [pc, #52]	; (8000608 <LoopForever+0x2>)
 80005d2:	4685      	mov	sp, r0
 80005d4:	480d      	ldr	r0, [pc, #52]	; (800060c <LoopForever+0x6>)
 80005d6:	490e      	ldr	r1, [pc, #56]	; (8000610 <LoopForever+0xa>)
 80005d8:	4a0e      	ldr	r2, [pc, #56]	; (8000614 <LoopForever+0xe>)
 80005da:	2300      	movs	r3, #0
 80005dc:	e002      	b.n	80005e4 <LoopCopyDataInit>

080005de <CopyDataInit>:
 80005de:	58d4      	ldr	r4, [r2, r3]
 80005e0:	50c4      	str	r4, [r0, r3]
 80005e2:	3304      	adds	r3, #4

080005e4 <LoopCopyDataInit>:
 80005e4:	18c4      	adds	r4, r0, r3
 80005e6:	428c      	cmp	r4, r1
 80005e8:	d3f9      	bcc.n	80005de <CopyDataInit>
 80005ea:	4a0b      	ldr	r2, [pc, #44]	; (8000618 <LoopForever+0x12>)
 80005ec:	4c0b      	ldr	r4, [pc, #44]	; (800061c <LoopForever+0x16>)
 80005ee:	2300      	movs	r3, #0
 80005f0:	e001      	b.n	80005f6 <LoopFillZerobss>

080005f2 <FillZerobss>:
 80005f2:	6013      	str	r3, [r2, #0]
 80005f4:	3204      	adds	r2, #4

080005f6 <LoopFillZerobss>:
 80005f6:	42a2      	cmp	r2, r4
 80005f8:	d3fb      	bcc.n	80005f2 <FillZerobss>
 80005fa:	f000 f812 	bl	8000622 <SystemInit>
 80005fe:	f005 fce7 	bl	8005fd0 <__libc_init_array>
 8000602:	f002 f8a5 	bl	8002750 <main>

08000606 <LoopForever>:
 8000606:	e7fe      	b.n	8000606 <LoopForever>
 8000608:	20002000 	andcs	r2, r0, r0
 800060c:	20000000 	andcs	r0, r0, r0
 8000610:	20000070 	andcs	r0, r0, r0, ror r0
 8000614:	08007850 	stmdaeq	r0, {r4, r6, fp, ip, sp, lr}
 8000618:	20000070 	andcs	r0, r0, r0, ror r0
 800061c:	20000e64 	andcs	r0, r0, r4, ror #28

08000620 <ADC1_COMP_IRQHandler>:
 8000620:	e7fe      	b.n	8000620 <ADC1_COMP_IRQHandler>

08000622 <SystemInit>:
 8000622:	4770      	bx	lr

08000624 <onPulseClicksMenuGetOption>:
 8000624:	4b05      	ldr	r3, [pc, #20]	; (800063c <onPulseClicksMenuGetOption+0x18>)
 8000626:	685b      	ldr	r3, [r3, #4]
 8000628:	f3c3 33c1 	ubfx	r3, r3, #15, #2
 800062c:	1a58      	subs	r0, r3, r1
 800062e:	4243      	negs	r3, r0
 8000630:	4143      	adcs	r3, r0
 8000632:	6013      	str	r3, [r2, #0]
 8000634:	4b02      	ldr	r3, [pc, #8]	; (8000640 <onPulseClicksMenuGetOption+0x1c>)
 8000636:	f853 0021 	ldr.w	r0, [r3, r1, lsl #2]
 800063a:	4770      	bx	lr
 800063c:	20000da0 	andcs	r0, r0, r0, lsr #27
 8000640:	0800616c 	stmdaeq	r0, {r2, r3, r5, r6, r8, sp, lr}

08000644 <onPulseClicksMenuSelect>:
 8000644:	4b03      	ldr	r3, [pc, #12]	; (8000654 <onPulseClicksMenuSelect+0x10>)
 8000646:	6841      	ldr	r1, [r0, #4]
 8000648:	685a      	ldr	r2, [r3, #4]
 800064a:	8849      	ldrh	r1, [r1, #2]
 800064c:	f361 32d0 	bfi	r2, r1, #15, #2
 8000650:	605a      	str	r2, [r3, #4]
 8000652:	4770      	bx	lr
 8000654:	20000da0 	andcs	r0, r0, r0, lsr #27

08000658 <initBuzzer>:
 8000658:	b508      	push	{r3, lr}
 800065a:	f003 fef1 	bl	8004440 <initBuzzerController>
 800065e:	4b05      	ldr	r3, [pc, #20]	; (8000674 <initBuzzer+0x1c>)
 8000660:	2203      	movs	r2, #3
 8000662:	6859      	ldr	r1, [r3, #4]
 8000664:	4804      	ldr	r0, [pc, #16]	; (8000678 <initBuzzer+0x20>)
 8000666:	f3c1 31c1 	ubfx	r1, r1, #15, #2
 800066a:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 800066e:	f002 bffd 	b.w	800366c <selectMenuItem>
 8000672:	bf00      	nop
 8000674:	20000da0 	andcs	r0, r0, r0, lsr #27
 8000678:	08006158 	stmdaeq	r0, {r3, r4, r6, r8, sp, lr}

0800067c <playSystemAlert>:
 800067c:	b510      	push	{r4, lr}
 800067e:	240a      	movs	r4, #10
 8000680:	2001      	movs	r0, #1
 8000682:	f003 fef7 	bl	8004474 <setBuzzer>
 8000686:	2032      	movs	r0, #50	; 0x32
 8000688:	f004 f876 	bl	8004778 <sleep>
 800068c:	2000      	movs	r0, #0
 800068e:	f003 fef1 	bl	8004474 <setBuzzer>
 8000692:	2032      	movs	r0, #50	; 0x32
 8000694:	f004 f870 	bl	8004778 <sleep>
 8000698:	3c01      	subs	r4, #1
 800069a:	d1f1      	bne.n	8000680 <playSystemAlert+0x4>
 800069c:	bd10      	pop	{r4, pc}

0800069e <addClamped>:
 800069e:	43ca      	mvns	r2, r1
 80006a0:	6803      	ldr	r3, [r0, #0]
 80006a2:	4293      	cmp	r3, r2
 80006a4:	bf8c      	ite	hi
 80006a6:	f04f 33ff 	movhi.w	r3, #4294967295
 80006aa:	185b      	addls	r3, r3, r1
 80006ac:	6003      	str	r3, [r0, #0]
 80006ae:	4770      	bx	lr

080006b0 <getConfidenceInterval>:
 80006b0:	2801      	cmp	r0, #1
 80006b2:	b538      	push	{r3, r4, r5, lr}
 80006b4:	d919      	bls.n	80006ea <getConfidenceInterval+0x3a>
 80006b6:	2802      	cmp	r0, #2
 80006b8:	d019      	beq.n	80006ee <getConfidenceInterval+0x3e>
 80006ba:	f003 feef 	bl	800449c <__aeabi_ui2f>
 80006be:	4604      	mov	r4, r0
 80006c0:	f003 ff31 	bl	8004526 <sqrtf>
 80006c4:	490b      	ldr	r1, [pc, #44]	; (80006f4 <getConfidenceInterval+0x44>)
 80006c6:	4605      	mov	r5, r0
 80006c8:	4620      	mov	r0, r4
 80006ca:	f003 fedf 	bl	800448c <__aeabi_fadd>
 80006ce:	4601      	mov	r1, r0
 80006d0:	4809      	ldr	r0, [pc, #36]	; (80006f8 <getConfidenceInterval+0x48>)
 80006d2:	f003 fee1 	bl	8004498 <__aeabi_fdiv>
 80006d6:	4629      	mov	r1, r5
 80006d8:	4604      	mov	r4, r0
 80006da:	4808      	ldr	r0, [pc, #32]	; (80006fc <getConfidenceInterval+0x4c>)
 80006dc:	f003 fedc 	bl	8004498 <__aeabi_fdiv>
 80006e0:	4601      	mov	r1, r0
 80006e2:	4620      	mov	r0, r4
 80006e4:	f003 fed2 	bl	800448c <__aeabi_fadd>
 80006e8:	bd38      	pop	{r3, r4, r5, pc}
 80006ea:	4805      	ldr	r0, [pc, #20]	; (8000700 <getConfidenceInterval+0x50>)
 80006ec:	e7fc      	b.n	80006e8 <getConfidenceInterval+0x38>
 80006ee:	4805      	ldr	r0, [pc, #20]	; (8000704 <getConfidenceInterval+0x54>)
 80006f0:	e7fa      	b.n	80006e8 <getConfidenceInterval+0x38>
 80006f2:	bf00      	nop
 80006f4:	3eae7642 	cdpcc	6, 10, cr7, cr14, cr2, {2}
 80006f8:	3f6bc37a 	svccc	0x006bc37a
 80006fc:	3ffae01a 	svccc	0x00fae01a
 8000700:	402c169a 	mlami	ip, sl, r6, r1
 8000704:	3fe495ce 	svccc	0x00e495ce

08000708 <getRandomBit>:
 8000708:	4a05      	ldr	r2, [pc, #20]	; (8000720 <getRandomBit+0x18>)
 800070a:	6811      	ldr	r1, [r2, #0]
 800070c:	f001 0001 	and.w	r0, r1, #1
 8000710:	4243      	negs	r3, r0
 8000712:	f403 4334 	and.w	r3, r3, #46080	; 0xb400
 8000716:	ea83 0351 	eor.w	r3, r3, r1, lsr #1
 800071a:	6013      	str	r3, [r2, #0]
 800071c:	4770      	bx	lr
 800071e:	bf00      	nop
 8000720:	20000000 	andcs	r0, r0, r0

08000724 <matchCommCommand>:
 8000724:	4a07      	ldr	r2, [pc, #28]	; (8000744 <matchCommCommand+0x20>)
 8000726:	1e41      	subs	r1, r0, #1
 8000728:	f811 3f01 	ldrb.w	r3, [r1, #1]!
 800072c:	f812 0f01 	ldrb.w	r0, [r2, #1]!
 8000730:	b923      	cbnz	r3, 800073c <matchCommCommand+0x18>
 8000732:	2820      	cmp	r0, #32
 8000734:	bf8c      	ite	hi
 8000736:	2000      	movhi	r0, #0
 8000738:	2001      	movls	r0, #1
 800073a:	4770      	bx	lr
 800073c:	4298      	cmp	r0, r3
 800073e:	d0f3      	beq.n	8000728 <matchCommCommand+0x4>
 8000740:	2000      	movs	r0, #0
 8000742:	4770      	bx	lr
 8000744:	20000c83 	andcs	r0, r0, r3, lsl #25

08000748 <sendCommOk>:
 8000748:	4901      	ldr	r1, [pc, #4]	; (8000750 <sendCommOk+0x8>)
 800074a:	4802      	ldr	r0, [pc, #8]	; (8000754 <sendCommOk+0xc>)
 800074c:	f005 bc86 	b.w	800605c <strcpy>
 8000750:	080062dd 	stmdaeq	r0, {r0, r2, r3, r4, r6, r7, r9, sp, lr}
 8000754:	20000c84 	andcs	r0, r0, r4, lsl #25

08000758 <sendCommOkWithUInt32>:
 8000758:	b510      	push	{r4, lr}
 800075a:	4604      	mov	r4, r0
 800075c:	f7ff fff4 	bl	8000748 <sendCommOk>
 8000760:	4b05      	ldr	r3, [pc, #20]	; (8000778 <sendCommOkWithUInt32+0x20>)
 8000762:	4906      	ldr	r1, [pc, #24]	; (800077c <sendCommOkWithUInt32+0x24>)
 8000764:	4618      	mov	r0, r3
 8000766:	f005 fc6a 	bl	800603e <strcat>
 800076a:	4621      	mov	r1, r4
 800076c:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8000770:	2200      	movs	r2, #0
 8000772:	f000 b99b 	b.w	8000aac <strcatUInt32>
 8000776:	bf00      	nop
 8000778:	20000c84 	andcs	r0, r0, r4, lsl #25
 800077c:	08006506 	stmdaeq	r0, {r1, r2, r8, sl, sp, lr}

08000780 <sendCommOkWithFloat>:
 8000780:	b538      	push	{r3, r4, r5, lr}
 8000782:	4604      	mov	r4, r0
 8000784:	460d      	mov	r5, r1
 8000786:	f7ff ffdf 	bl	8000748 <sendCommOk>
 800078a:	4b05      	ldr	r3, [pc, #20]	; (80007a0 <sendCommOkWithFloat+0x20>)
 800078c:	4905      	ldr	r1, [pc, #20]	; (80007a4 <sendCommOkWithFloat+0x24>)
 800078e:	4618      	mov	r0, r3
 8000790:	f005 fc55 	bl	800603e <strcat>
 8000794:	462a      	mov	r2, r5
 8000796:	4621      	mov	r1, r4
 8000798:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 800079c:	f000 b9ea 	b.w	8000b74 <strcatFloat>
 80007a0:	20000c84 	andcs	r0, r0, r4, lsl #25
 80007a4:	08006506 	stmdaeq	r0, {r1, r2, r8, sl, sp, lr}

080007a8 <startDatalogDumpTemp>:
 80007a8:	b510      	push	{r4, lr}
 80007aa:	4c08      	ldr	r4, [pc, #32]	; (80007cc <startDatalogDumpTemp+0x24>)
 80007ac:	f7ff ffcc 	bl	8000748 <sendCommOk>
 80007b0:	4907      	ldr	r1, [pc, #28]	; (80007d0 <startDatalogDumpTemp+0x28>)
 80007b2:	4620      	mov	r0, r4
 80007b4:	f005 fc43 	bl	800603e <strcat>
 80007b8:	f000 fca0 	bl	80010fc <startDatalogDownload>
 80007bc:	2301      	movs	r3, #1
 80007be:	f884 3041 	strb.w	r3, [r4, #65]	; 0x41
 80007c2:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 80007c6:	f003 bf05 	b.w	80045d4 <transmitComm>
 80007ca:	bf00      	nop
 80007cc:	20000c84 	andcs	r0, r0, r4, lsl #25
 80007d0:	080062e0 	stmdaeq	r0, {r5, r6, r7, r9, sp, lr}

080007d4 <matchCommCommandWithUInt32>:
 80007d4:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 80007d6:	4607      	mov	r7, r0
 80007d8:	460d      	mov	r5, r1
 80007da:	f7ff ffa3 	bl	8000724 <matchCommCommand>
 80007de:	b180      	cbz	r0, 8000802 <matchCommCommandWithUInt32+0x2e>
 80007e0:	4c09      	ldr	r4, [pc, #36]	; (8000808 <matchCommCommandWithUInt32+0x34>)
 80007e2:	4620      	mov	r0, r4
 80007e4:	f7ff feec 	bl	80005c0 <strlen>
 80007e8:	4606      	mov	r6, r0
 80007ea:	4638      	mov	r0, r7
 80007ec:	f7ff fee8 	bl	80005c0 <strlen>
 80007f0:	3001      	adds	r0, #1
 80007f2:	4286      	cmp	r6, r0
 80007f4:	d906      	bls.n	8000804 <matchCommCommandWithUInt32+0x30>
 80007f6:	4629      	mov	r1, r5
 80007f8:	4420      	add	r0, r4
 80007fa:	f000 f979 	bl	8000af0 <parseUInt32>
 80007fe:	f000 0001 	and.w	r0, r0, #1
 8000802:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 8000804:	2000      	movs	r0, #0
 8000806:	e7fa      	b.n	80007fe <matchCommCommandWithUInt32+0x2a>
 8000808:	20000c84 	andcs	r0, r0, r4, lsl #25

0800080c <setCommEnabled>:
 800080c:	4b01      	ldr	r3, [pc, #4]	; (8000814 <setCommEnabled+0x8>)
 800080e:	f883 0048 	strb.w	r0, [r3, #72]	; 0x48
 8000812:	4770      	bx	lr
 8000814:	20000c7c 	andcs	r0, r0, ip, ror ip

08000818 <dispatchCommEvents>:
 8000818:	e92d 43f7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
 800081c:	4c87      	ldr	r4, [pc, #540]	; (8000a3c <dispatchCommEvents+0x224>)
 800081e:	f003 ff2d 	bl	800467c <updateCommController>
 8000822:	7866      	ldrb	r6, [r4, #1]
 8000824:	4625      	mov	r5, r4
 8000826:	b2f6      	uxtb	r6, r6
 8000828:	2e01      	cmp	r6, #1
 800082a:	f040 80cc 	bne.w	80009c6 <dispatchCommEvents+0x1ae>
 800082e:	4884      	ldr	r0, [pc, #528]	; (8000a40 <dispatchCommEvents+0x228>)
 8000830:	f7ff ff78 	bl	8000724 <matchCommCommand>
 8000834:	b1f8      	cbz	r0, 8000876 <dispatchCommEvents+0x5e>
 8000836:	4d83      	ldr	r5, [pc, #524]	; (8000a44 <dispatchCommEvents+0x22c>)
 8000838:	f7ff ff86 	bl	8000748 <sendCommOk>
 800083c:	4982      	ldr	r1, [pc, #520]	; (8000a48 <dispatchCommEvents+0x230>)
 800083e:	f104 0008 	add.w	r0, r4, #8
 8000842:	f005 fbfc 	bl	800603e <strcat>
 8000846:	6829      	ldr	r1, [r5, #0]
 8000848:	f104 0008 	add.w	r0, r4, #8
 800084c:	f005 fbf7 	bl	800603e <strcat>
 8000850:	497e      	ldr	r1, [pc, #504]	; (8000a4c <dispatchCommEvents+0x234>)
 8000852:	f104 0008 	add.w	r0, r4, #8
 8000856:	f005 fbf2 	bl	800603e <strcat>
 800085a:	f004 f965 	bl	8004b28 <getDeviceId>
 800085e:	4601      	mov	r1, r0
 8000860:	f104 0008 	add.w	r0, r4, #8
 8000864:	f000 fa89 	bl	8000d7a <strcatUInt32Hex>
 8000868:	4979      	ldr	r1, [pc, #484]	; (8000a50 <dispatchCommEvents+0x238>)
 800086a:	487a      	ldr	r0, [pc, #488]	; (8000a54 <dispatchCommEvents+0x23c>)
 800086c:	f005 fbe7 	bl	800603e <strcat>
 8000870:	f003 feb0 	bl	80045d4 <transmitComm>
 8000874:	e07e      	b.n	8000974 <dispatchCommEvents+0x15c>
 8000876:	4878      	ldr	r0, [pc, #480]	; (8000a58 <dispatchCommEvents+0x240>)
 8000878:	f7ff ff54 	bl	8000724 <matchCommCommand>
 800087c:	b118      	cbz	r0, 8000886 <dispatchCommEvents+0x6e>
 800087e:	f003 fdb7 	bl	80043f0 <getDeviceBatteryVoltage>
 8000882:	2103      	movs	r1, #3
 8000884:	e006      	b.n	8000894 <dispatchCommEvents+0x7c>
 8000886:	4875      	ldr	r0, [pc, #468]	; (8000a5c <dispatchCommEvents+0x244>)
 8000888:	f7ff ff4c 	bl	8000724 <matchCommCommand>
 800088c:	b128      	cbz	r0, 800089a <dispatchCommEvents+0x82>
 800088e:	f003 fdbb 	bl	8004408 <getDeviceTemperature>
 8000892:	4631      	mov	r1, r6
 8000894:	f7ff ff74 	bl	8000780 <sendCommOkWithFloat>
 8000898:	e7e6      	b.n	8000868 <dispatchCommEvents+0x50>
 800089a:	4871      	ldr	r0, [pc, #452]	; (8000a60 <dispatchCommEvents+0x248>)
 800089c:	f7ff ff42 	bl	8000724 <matchCommCommand>
 80008a0:	b120      	cbz	r0, 80008ac <dispatchCommEvents+0x94>
 80008a2:	f004 f8f7 	bl	8004a94 <getDeviceTime>
 80008a6:	f7ff ff57 	bl	8000758 <sendCommOkWithUInt32>
 80008aa:	e7dd      	b.n	8000868 <dispatchCommEvents+0x50>
 80008ac:	4669      	mov	r1, sp
 80008ae:	486d      	ldr	r0, [pc, #436]	; (8000a64 <dispatchCommEvents+0x24c>)
 80008b0:	f7ff ff90 	bl	80007d4 <matchCommCommandWithUInt32>
 80008b4:	b128      	cbz	r0, 80008c2 <dispatchCommEvents+0xaa>
 80008b6:	9800      	ldr	r0, [sp, #0]
 80008b8:	f004 f852 	bl	8004960 <setDeviceTime>
 80008bc:	f7ff ff44 	bl	8000748 <sendCommOk>
 80008c0:	e7d2      	b.n	8000868 <dispatchCommEvents+0x50>
 80008c2:	4869      	ldr	r0, [pc, #420]	; (8000a68 <dispatchCommEvents+0x250>)
 80008c4:	f7ff ff2e 	bl	8000724 <matchCommCommand>
 80008c8:	b110      	cbz	r0, 80008d0 <dispatchCommEvents+0xb8>
 80008ca:	f002 febd 	bl	8003648 <getTubeTime>
 80008ce:	e7ea      	b.n	80008a6 <dispatchCommEvents+0x8e>
 80008d0:	4669      	mov	r1, sp
 80008d2:	4866      	ldr	r0, [pc, #408]	; (8000a6c <dispatchCommEvents+0x254>)
 80008d4:	f7ff ff7e 	bl	80007d4 <matchCommCommandWithUInt32>
 80008d8:	b118      	cbz	r0, 80008e2 <dispatchCommEvents+0xca>
 80008da:	9800      	ldr	r0, [sp, #0]
 80008dc:	f002 feae 	bl	800363c <setTubeTime>
 80008e0:	e7ec      	b.n	80008bc <dispatchCommEvents+0xa4>
 80008e2:	4863      	ldr	r0, [pc, #396]	; (8000a70 <dispatchCommEvents+0x258>)
 80008e4:	f7ff ff1e 	bl	8000724 <matchCommCommand>
 80008e8:	b110      	cbz	r0, 80008f0 <dispatchCommEvents+0xd8>
 80008ea:	f002 feb9 	bl	8003660 <getTubePulseCount>
 80008ee:	e7da      	b.n	80008a6 <dispatchCommEvents+0x8e>
 80008f0:	4669      	mov	r1, sp
 80008f2:	4860      	ldr	r0, [pc, #384]	; (8000a74 <dispatchCommEvents+0x25c>)
 80008f4:	f7ff ff6e 	bl	80007d4 <matchCommCommandWithUInt32>
 80008f8:	b118      	cbz	r0, 8000902 <dispatchCommEvents+0xea>
 80008fa:	9800      	ldr	r0, [sp, #0]
 80008fc:	f002 feaa 	bl	8003654 <setTubePulseCount>
 8000900:	e7dc      	b.n	80008bc <dispatchCommEvents+0xa4>
 8000902:	485d      	ldr	r0, [pc, #372]	; (8000a78 <dispatchCommEvents+0x260>)
 8000904:	f7ff ff0e 	bl	8000724 <matchCommCommand>
 8000908:	b128      	cbz	r0, 8000916 <dispatchCommEvents+0xfe>
 800090a:	f002 fe79 	bl	8003600 <getInstantaneousRate>
 800090e:	495b      	ldr	r1, [pc, #364]	; (8000a7c <dispatchCommEvents+0x264>)
 8000910:	f003 fdc0 	bl	8004494 <__aeabi_fmul>
 8000914:	e7b5      	b.n	8000882 <dispatchCommEvents+0x6a>
 8000916:	485a      	ldr	r0, [pc, #360]	; (8000a80 <dispatchCommEvents+0x268>)
 8000918:	f7ff ff04 	bl	8000724 <matchCommCommand>
 800091c:	b118      	cbz	r0, 8000926 <dispatchCommEvents+0x10e>
 800091e:	f001 fad7 	bl	8001ed0 <getTubeDeadTime>
 8000922:	2107      	movs	r1, #7
 8000924:	e7b6      	b.n	8000894 <dispatchCommEvents+0x7c>
 8000926:	4857      	ldr	r0, [pc, #348]	; (8000a84 <dispatchCommEvents+0x26c>)
 8000928:	f7ff fefc 	bl	8000724 <matchCommCommand>
 800092c:	b110      	cbz	r0, 8000934 <dispatchCommEvents+0x11c>
 800092e:	f004 fbf3 	bl	8005118 <getTubeConversionFactor>
 8000932:	e7a6      	b.n	8000882 <dispatchCommEvents+0x6a>
 8000934:	4854      	ldr	r0, [pc, #336]	; (8000a88 <dispatchCommEvents+0x270>)
 8000936:	f7ff fef5 	bl	8000724 <matchCommCommand>
 800093a:	4604      	mov	r4, r0
 800093c:	b110      	cbz	r0, 8000944 <dispatchCommEvents+0x12c>
 800093e:	f004 fc09 	bl	8005154 <getTubeDeadTimeCompensation>
 8000942:	e7ee      	b.n	8000922 <dispatchCommEvents+0x10a>
 8000944:	4851      	ldr	r0, [pc, #324]	; (8000a8c <dispatchCommEvents+0x274>)
 8000946:	f7ff feed 	bl	8000724 <matchCommCommand>
 800094a:	b118      	cbz	r0, 8000954 <dispatchCommEvents+0x13c>
 800094c:	f004 fc1c 	bl	8005188 <getTubeHVFrequency>
 8000950:	4621      	mov	r1, r4
 8000952:	e79f      	b.n	8000894 <dispatchCommEvents+0x7c>
 8000954:	484e      	ldr	r0, [pc, #312]	; (8000a90 <dispatchCommEvents+0x278>)
 8000956:	f7ff fee5 	bl	8000724 <matchCommCommand>
 800095a:	b118      	cbz	r0, 8000964 <dispatchCommEvents+0x14c>
 800095c:	f004 fc36 	bl	80051cc <getTubeHVDutyCycle>
 8000960:	2104      	movs	r1, #4
 8000962:	e797      	b.n	8000894 <dispatchCommEvents+0x7c>
 8000964:	494b      	ldr	r1, [pc, #300]	; (8000a94 <dispatchCommEvents+0x27c>)
 8000966:	484c      	ldr	r0, [pc, #304]	; (8000a98 <dispatchCommEvents+0x280>)
 8000968:	f7ff ff34 	bl	80007d4 <matchCommCommandWithUInt32>
 800096c:	4606      	mov	r6, r0
 800096e:	b120      	cbz	r0, 800097a <dispatchCommEvents+0x162>
 8000970:	f7ff ff1a 	bl	80007a8 <startDatalogDumpTemp>
 8000974:	b003      	add	sp, #12
 8000976:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 800097a:	4847      	ldr	r0, [pc, #284]	; (8000a98 <dispatchCommEvents+0x280>)
 800097c:	f7ff fed2 	bl	8000724 <matchCommCommand>
 8000980:	4604      	mov	r4, r0
 8000982:	b108      	cbz	r0, 8000988 <dispatchCommEvents+0x170>
 8000984:	64ee      	str	r6, [r5, #76]	; 0x4c
 8000986:	e7f3      	b.n	8000970 <dispatchCommEvents+0x158>
 8000988:	4844      	ldr	r0, [pc, #272]	; (8000a9c <dispatchCommEvents+0x284>)
 800098a:	f7ff fecb 	bl	8000724 <matchCommCommand>
 800098e:	b1a8      	cbz	r0, 80009bc <dispatchCommEvents+0x1a4>
 8000990:	f7ff feda 	bl	8000748 <sendCommOk>
 8000994:	4f2c      	ldr	r7, [pc, #176]	; (8000a48 <dispatchCommEvents+0x230>)
 8000996:	4d2f      	ldr	r5, [pc, #188]	; (8000a54 <dispatchCommEvents+0x23c>)
 8000998:	f003 f842 	bl	8003a20 <getRandomData>
 800099c:	1e06      	subs	r6, r0, #0
 800099e:	f6ff af63 	blt.w	8000868 <dispatchCommEvents+0x50>
 80009a2:	b91c      	cbnz	r4, 80009ac <dispatchCommEvents+0x194>
 80009a4:	4639      	mov	r1, r7
 80009a6:	4628      	mov	r0, r5
 80009a8:	f005 fb49 	bl	800603e <strcat>
 80009ac:	b2f1      	uxtb	r1, r6
 80009ae:	4628      	mov	r0, r5
 80009b0:	3401      	adds	r4, #1
 80009b2:	f000 f9bf 	bl	8000d34 <strcatUInt8Hex>
 80009b6:	2c10      	cmp	r4, #16
 80009b8:	d1ee      	bne.n	8000998 <dispatchCommEvents+0x180>
 80009ba:	e755      	b.n	8000868 <dispatchCommEvents+0x50>
 80009bc:	4938      	ldr	r1, [pc, #224]	; (8000aa0 <dispatchCommEvents+0x288>)
 80009be:	4825      	ldr	r0, [pc, #148]	; (8000a54 <dispatchCommEvents+0x23c>)
 80009c0:	f005 fb4c 	bl	800605c <strcpy>
 80009c4:	e750      	b.n	8000868 <dispatchCommEvents+0x50>
 80009c6:	7863      	ldrb	r3, [r4, #1]
 80009c8:	2b03      	cmp	r3, #3
 80009ca:	d1d3      	bne.n	8000974 <dispatchCommEvents+0x15c>
 80009cc:	f894 3049 	ldrb.w	r3, [r4, #73]	; 0x49
 80009d0:	b383      	cbz	r3, 8000a34 <dispatchCommEvents+0x21c>
 80009d2:	f104 0008 	add.w	r0, r4, #8
 80009d6:	4933      	ldr	r1, [pc, #204]	; (8000aa4 <dispatchCommEvents+0x28c>)
 80009d8:	f005 fb40 	bl	800605c <strcpy>
 80009dc:	2600      	movs	r6, #0
 80009de:	f8df 906c 	ldr.w	r9, [pc, #108]	; 8000a4c <dispatchCommEvents+0x234>
 80009e2:	f8df 80c4 	ldr.w	r8, [pc, #196]	; 8000aa8 <dispatchCommEvents+0x290>
 80009e6:	3408      	adds	r4, #8
 80009e8:	4668      	mov	r0, sp
 80009ea:	f000 fb97 	bl	800111c <getDatalogDownloadEntry>
 80009ee:	4607      	mov	r7, r0
 80009f0:	b930      	cbnz	r0, 8000a00 <dispatchCommEvents+0x1e8>
 80009f2:	4917      	ldr	r1, [pc, #92]	; (8000a50 <dispatchCommEvents+0x238>)
 80009f4:	4817      	ldr	r0, [pc, #92]	; (8000a54 <dispatchCommEvents+0x23c>)
 80009f6:	f005 fb22 	bl	800603e <strcat>
 80009fa:	f885 7049 	strb.w	r7, [r5, #73]	; 0x49
 80009fe:	e737      	b.n	8000870 <dispatchCommEvents+0x58>
 8000a00:	9f00      	ldr	r7, [sp, #0]
 8000a02:	6ceb      	ldr	r3, [r5, #76]	; 0x4c
 8000a04:	429f      	cmp	r7, r3
 8000a06:	d3ef      	bcc.n	80009e8 <dispatchCommEvents+0x1d0>
 8000a08:	4649      	mov	r1, r9
 8000a0a:	4620      	mov	r0, r4
 8000a0c:	f005 fb17 	bl	800603e <strcat>
 8000a10:	2200      	movs	r2, #0
 8000a12:	4639      	mov	r1, r7
 8000a14:	4620      	mov	r0, r4
 8000a16:	f000 f849 	bl	8000aac <strcatUInt32>
 8000a1a:	4641      	mov	r1, r8
 8000a1c:	4620      	mov	r0, r4
 8000a1e:	f005 fb0e 	bl	800603e <strcat>
 8000a22:	3601      	adds	r6, #1
 8000a24:	2200      	movs	r2, #0
 8000a26:	9901      	ldr	r1, [sp, #4]
 8000a28:	4620      	mov	r0, r4
 8000a2a:	f000 f83f 	bl	8000aac <strcatUInt32>
 8000a2e:	2e02      	cmp	r6, #2
 8000a30:	d1da      	bne.n	80009e8 <dispatchCommEvents+0x1d0>
 8000a32:	e71d      	b.n	8000870 <dispatchCommEvents+0x58>
 8000a34:	6063      	str	r3, [r4, #4]
 8000a36:	7063      	strb	r3, [r4, #1]
 8000a38:	e79c      	b.n	8000974 <dispatchCommEvents+0x15c>
 8000a3a:	bf00      	nop
 8000a3c:	20000c7c 	andcs	r0, r0, ip, ror ip
 8000a40:	080061a0 	stmdaeq	r0, {r5, r7, r8, sp, lr}
 8000a44:	0800755c 	stmdaeq	r0, {r2, r3, r4, r6, r8, sl, ip, sp, lr}
 8000a48:	08006506 	stmdaeq	r0, {r1, r2, r8, sl, sp, lr}
 8000a4c:	080061ad 	stmdaeq	r0, {r0, r2, r3, r5, r7, r8, sp, lr}
 8000a50:	080062d9 	stmdaeq	r0, {r0, r3, r4, r6, r7, r9, sp, lr}
 8000a54:	20000c84 	andcs	r0, r0, r4, lsl #25
 8000a58:	080061af 	stmdaeq	r0, {r0, r1, r2, r3, r5, r7, r8, sp, lr}
 8000a5c:	080061c8 	stmdaeq	r0, {r3, r6, r7, r8, sp, lr}
 8000a60:	080061de 	stmdaeq	r0, {r1, r2, r3, r4, r6, r7, r8, sp, lr}
 8000a64:	080061ed 	stmdaeq	r0, {r0, r2, r3, r5, r6, r7, r8, sp, lr}
 8000a68:	080061fc 	stmdaeq	r0, {r2, r3, r4, r5, r6, r7, r8, sp, lr}
 8000a6c:	08006209 	stmdaeq	r0, {r0, r3, r9, sp, lr}
 8000a70:	08006216 	stmdaeq	r0, {r1, r2, r4, r9, sp, lr}
 8000a74:	08006229 	stmdaeq	r0, {r0, r3, r5, r9, sp, lr}
 8000a78:	0800623c 	stmdaeq	r0, {r2, r3, r4, r5, r9, sp, lr}
 8000a7c:	42700000 	rsbsmi	r0, r0, #0
 8000a80:	08006249 	stmdaeq	r0, {r0, r3, r6, r9, sp, lr}
 8000a84:	0800625a 	stmdaeq	r0, {r1, r3, r4, r6, r9, sp, lr}
 8000a88:	08006273 	stmdaeq	r0, {r0, r1, r4, r5, r6, r9, sp, lr}
 8000a8c:	08006290 	stmdaeq	r0, {r4, r7, r9, sp, lr}
 8000a90:	080062a4 	stmdaeq	r0, {r2, r5, r7, r9, sp, lr}
 8000a94:	20000cc8 	andcs	r0, r0, r8, asr #25
 8000a98:	080062b8 	stmdaeq	r0, {r3, r4, r5, r7, r9, sp, lr}
 8000a9c:	080062c4 	stmdaeq	r0, {r2, r6, r7, r9, sp, lr}
 8000aa0:	080062d3 	stmdaeq	r0, {r0, r1, r4, r6, r7, r9, sp, lr}
 8000aa4:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}
 8000aa8:	080062db 	stmdaeq	r0, {r0, r1, r3, r4, r6, r7, r9, sp, lr}

08000aac <strcatUInt32>:
 8000aac:	b570      	push	{r4, r5, r6, lr}
 8000aae:	460c      	mov	r4, r1
 8000ab0:	4605      	mov	r5, r0
 8000ab2:	4616      	mov	r6, r2
 8000ab4:	f7ff fd84 	bl	80005c0 <strlen>
 8000ab8:	4621      	mov	r1, r4
 8000aba:	2300      	movs	r3, #0
 8000abc:	220a      	movs	r2, #10
 8000abe:	4428      	add	r0, r5
 8000ac0:	fbb1 f1f2 	udiv	r1, r1, r2
 8000ac4:	3301      	adds	r3, #1
 8000ac6:	2900      	cmp	r1, #0
 8000ac8:	d1fa      	bne.n	8000ac0 <strcatUInt32+0x14>
 8000aca:	42b3      	cmp	r3, r6
 8000acc:	bf38      	it	cc
 8000ace:	4633      	movcc	r3, r6
 8000ad0:	210a      	movs	r1, #10
 8000ad2:	1e5a      	subs	r2, r3, #1
 8000ad4:	2a00      	cmp	r2, #0
 8000ad6:	da02      	bge.n	8000ade <strcatUInt32+0x32>
 8000ad8:	2200      	movs	r2, #0
 8000ada:	54c2      	strb	r2, [r0, r3]
 8000adc:	bd70      	pop	{r4, r5, r6, pc}
 8000ade:	fbb4 f5f1 	udiv	r5, r4, r1
 8000ae2:	fb01 4415 	mls	r4, r1, r5, r4
 8000ae6:	3430      	adds	r4, #48	; 0x30
 8000ae8:	5484      	strb	r4, [r0, r2]
 8000aea:	462c      	mov	r4, r5
 8000aec:	3a01      	subs	r2, #1
 8000aee:	e7f1      	b.n	8000ad4 <strcatUInt32+0x28>

08000af0 <parseUInt32>:
 8000af0:	b570      	push	{r4, r5, r6, lr}
 8000af2:	2200      	movs	r2, #0
 8000af4:	240a      	movs	r4, #10
 8000af6:	4d09      	ldr	r5, [pc, #36]	; (8000b1c <parseUInt32+0x2c>)
 8000af8:	f810 3b01 	ldrb.w	r3, [r0], #1
 8000afc:	2b1f      	cmp	r3, #31
 8000afe:	d802      	bhi.n	8000b06 <parseUInt32+0x16>
 8000b00:	2001      	movs	r0, #1
 8000b02:	600a      	str	r2, [r1, #0]
 8000b04:	bd70      	pop	{r4, r5, r6, pc}
 8000b06:	3b30      	subs	r3, #48	; 0x30
 8000b08:	b2de      	uxtb	r6, r3
 8000b0a:	2e09      	cmp	r6, #9
 8000b0c:	d804      	bhi.n	8000b18 <parseUInt32+0x28>
 8000b0e:	42aa      	cmp	r2, r5
 8000b10:	d802      	bhi.n	8000b18 <parseUInt32+0x28>
 8000b12:	fb04 3202 	mla	r2, r4, r2, r3
 8000b16:	e7ef      	b.n	8000af8 <parseUInt32+0x8>
 8000b18:	2000      	movs	r0, #0
 8000b1a:	e7f3      	b.n	8000b04 <parseUInt32+0x14>
 8000b1c:	19999999 	ldmibne	r9, {r0, r3, r4, r7, r8, fp, ip, pc}

08000b20 <strcatTime>:
 8000b20:	233c      	movs	r3, #60	; 0x3c
 8000b22:	fbb1 f2f3 	udiv	r2, r1, r3
 8000b26:	b570      	push	{r4, r5, r6, lr}
 8000b28:	fbb2 f5f3 	udiv	r5, r2, r3
 8000b2c:	fb03 1612 	mls	r6, r3, r2, r1
 8000b30:	fb03 2515 	mls	r5, r3, r5, r2
 8000b34:	f44f 6361 	mov.w	r3, #3600	; 0xe10
 8000b38:	fbb1 f1f3 	udiv	r1, r1, r3
 8000b3c:	4604      	mov	r4, r0
 8000b3e:	b131      	cbz	r1, 8000b4e <strcatTime+0x2e>
 8000b40:	2200      	movs	r2, #0
 8000b42:	f7ff ffb3 	bl	8000aac <strcatUInt32>
 8000b46:	490a      	ldr	r1, [pc, #40]	; (8000b70 <strcatTime+0x50>)
 8000b48:	4620      	mov	r0, r4
 8000b4a:	f005 fa78 	bl	800603e <strcat>
 8000b4e:	2202      	movs	r2, #2
 8000b50:	4629      	mov	r1, r5
 8000b52:	4620      	mov	r0, r4
 8000b54:	f7ff ffaa 	bl	8000aac <strcatUInt32>
 8000b58:	4905      	ldr	r1, [pc, #20]	; (8000b70 <strcatTime+0x50>)
 8000b5a:	4620      	mov	r0, r4
 8000b5c:	f005 fa6f 	bl	800603e <strcat>
 8000b60:	4631      	mov	r1, r6
 8000b62:	4620      	mov	r0, r4
 8000b64:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 8000b68:	2202      	movs	r2, #2
 8000b6a:	f7ff bf9f 	b.w	8000aac <strcatUInt32>
 8000b6e:	bf00      	nop
 8000b70:	0800634f 	stmdaeq	r0, {r0, r1, r2, r3, r6, r8, r9, sp, lr}

08000b74 <strcatFloat>:
 8000b74:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
 8000b78:	460c      	mov	r4, r1
 8000b7a:	4605      	mov	r5, r0
 8000b7c:	2100      	movs	r1, #0
 8000b7e:	4620      	mov	r0, r4
 8000b80:	4617      	mov	r7, r2
 8000b82:	f003 fc9d 	bl	80044c0 <__aeabi_fcmplt>
 8000b86:	b128      	cbz	r0, 8000b94 <strcatFloat+0x20>
 8000b88:	491d      	ldr	r1, [pc, #116]	; (8000c00 <strcatFloat+0x8c>)
 8000b8a:	4628      	mov	r0, r5
 8000b8c:	f104 4400 	add.w	r4, r4, #2147483648	; 0x80000000
 8000b90:	f005 fa55 	bl	800603e <strcat>
 8000b94:	463e      	mov	r6, r7
 8000b96:	2001      	movs	r0, #1
 8000b98:	230a      	movs	r3, #10
 8000b9a:	bb5e      	cbnz	r6, 8000bf4 <strcatFloat+0x80>
 8000b9c:	f003 fc7e 	bl	800449c <__aeabi_ui2f>
 8000ba0:	4601      	mov	r1, r0
 8000ba2:	4680      	mov	r8, r0
 8000ba4:	f04f 507c 	mov.w	r0, #1056964608	; 0x3f000000
 8000ba8:	f003 fc76 	bl	8004498 <__aeabi_fdiv>
 8000bac:	4621      	mov	r1, r4
 8000bae:	f003 fc6d 	bl	800448c <__aeabi_fadd>
 8000bb2:	4681      	mov	r9, r0
 8000bb4:	f003 fc7b 	bl	80044ae <__aeabi_f2uiz>
 8000bb8:	4632      	mov	r2, r6
 8000bba:	4604      	mov	r4, r0
 8000bbc:	4601      	mov	r1, r0
 8000bbe:	4628      	mov	r0, r5
 8000bc0:	f7ff ff74 	bl	8000aac <strcatUInt32>
 8000bc4:	b1cf      	cbz	r7, 8000bfa <strcatFloat+0x86>
 8000bc6:	490f      	ldr	r1, [pc, #60]	; (8000c04 <strcatFloat+0x90>)
 8000bc8:	4628      	mov	r0, r5
 8000bca:	f005 fa38 	bl	800603e <strcat>
 8000bce:	4620      	mov	r0, r4
 8000bd0:	f003 fc64 	bl	800449c <__aeabi_ui2f>
 8000bd4:	4601      	mov	r1, r0
 8000bd6:	4648      	mov	r0, r9
 8000bd8:	f003 fc5a 	bl	8004490 <__aeabi_fsub>
 8000bdc:	4641      	mov	r1, r8
 8000bde:	f003 fc59 	bl	8004494 <__aeabi_fmul>
 8000be2:	f003 fc64 	bl	80044ae <__aeabi_f2uiz>
 8000be6:	463a      	mov	r2, r7
 8000be8:	4601      	mov	r1, r0
 8000bea:	4628      	mov	r0, r5
 8000bec:	e8bd 43f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
 8000bf0:	f7ff bf5c 	b.w	8000aac <strcatUInt32>
 8000bf4:	4358      	muls	r0, r3
 8000bf6:	3e01      	subs	r6, #1
 8000bf8:	e7cf      	b.n	8000b9a <strcatFloat+0x26>
 8000bfa:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 8000bfe:	bf00      	nop
 8000c00:	0800737b 	stmdaeq	r0, {r0, r1, r3, r4, r5, r6, r8, r9, ip, sp, lr}
 8000c04:	080072e7 	stmdaeq	r0, {r0, r1, r2, r5, r6, r7, r9, ip, sp, lr}

08000c08 <strcatFloatWithMetricPrefix>:
 8000c08:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8000c0c:	f04f 080c 	mov.w	r8, #12
 8000c10:	4606      	mov	r6, r0
 8000c12:	460f      	mov	r7, r1
 8000c14:	4615      	mov	r5, r2
 8000c16:	4c1c      	ldr	r4, [pc, #112]	; (8000c88 <strcatFloatWithMetricPrefix+0x80>)
 8000c18:	fb08 4802 	mla	r8, r8, r2, r4
 8000c1c:	2d06      	cmp	r5, #6
 8000c1e:	d902      	bls.n	8000c26 <strcatFloatWithMetricPrefix+0x1e>
 8000c20:	2400      	movs	r4, #0
 8000c22:	2100      	movs	r1, #0
 8000c24:	e00d      	b.n	8000c42 <strcatFloatWithMetricPrefix+0x3a>
 8000c26:	f108 080c 	add.w	r8, r8, #12
 8000c2a:	4639      	mov	r1, r7
 8000c2c:	f858 0c08 	ldr.w	r0, [r8, #-8]
 8000c30:	f003 fc51 	bl	80044d6 <__aeabi_fcmpgt>
 8000c34:	b318      	cbz	r0, 8000c7e <strcatFloatWithMetricPrefix+0x76>
 8000c36:	230c      	movs	r3, #12
 8000c38:	fb03 f205 	mul.w	r2, r3, r5
 8000c3c:	58a1      	ldr	r1, [r4, r2]
 8000c3e:	3208      	adds	r2, #8
 8000c40:	4414      	add	r4, r2
 8000c42:	4638      	mov	r0, r7
 8000c44:	f003 fc28 	bl	8004498 <__aeabi_fdiv>
 8000c48:	4910      	ldr	r1, [pc, #64]	; (8000c8c <strcatFloatWithMetricPrefix+0x84>)
 8000c4a:	4605      	mov	r5, r0
 8000c4c:	f003 fc3d 	bl	80044ca <__aeabi_fcmpge>
 8000c50:	b9b8      	cbnz	r0, 8000c82 <strcatFloatWithMetricPrefix+0x7a>
 8000c52:	490f      	ldr	r1, [pc, #60]	; (8000c90 <strcatFloatWithMetricPrefix+0x88>)
 8000c54:	4628      	mov	r0, r5
 8000c56:	f003 fc38 	bl	80044ca <__aeabi_fcmpge>
 8000c5a:	2800      	cmp	r0, #0
 8000c5c:	bf14      	ite	ne
 8000c5e:	2202      	movne	r2, #2
 8000c60:	2203      	moveq	r2, #3
 8000c62:	4629      	mov	r1, r5
 8000c64:	4630      	mov	r0, r6
 8000c66:	f7ff ff85 	bl	8000b74 <strcatFloat>
 8000c6a:	490a      	ldr	r1, [pc, #40]	; (8000c94 <strcatFloatWithMetricPrefix+0x8c>)
 8000c6c:	4630      	mov	r0, r6
 8000c6e:	f005 f9e6 	bl	800603e <strcat>
 8000c72:	4621      	mov	r1, r4
 8000c74:	4630      	mov	r0, r6
 8000c76:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
 8000c7a:	f005 b9e0 	b.w	800603e <strcat>
 8000c7e:	3501      	adds	r5, #1
 8000c80:	e7cc      	b.n	8000c1c <strcatFloatWithMetricPrefix+0x14>
 8000c82:	2201      	movs	r2, #1
 8000c84:	e7ed      	b.n	8000c62 <strcatFloatWithMetricPrefix+0x5a>
 8000c86:	bf00      	nop
 8000c88:	080062f8 	stmdaeq	r0, {r3, r4, r5, r6, r7, r9, sp, lr}
 8000c8c:	42c7fd71 	sbcmi	pc, r7, #7232	; 0x1c40
 8000c90:	411ffdf4 			; <UNDEFINED> instruction: 0x411ffdf4
 8000c94:	08006506 	stmdaeq	r0, {r1, r2, r8, sl, sp, lr}

08000c98 <strcatDecimalPowerWithMetricPrefix>:
 8000c98:	1d8b      	adds	r3, r1, #6
 8000c9a:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8000c9e:	4606      	mov	r6, r0
 8000ca0:	460c      	mov	r4, r1
 8000ca2:	da24      	bge.n	8000cee <strcatDecimalPowerWithMetricPrefix+0x56>
 8000ca4:	491e      	ldr	r1, [pc, #120]	; (8000d20 <strcatDecimalPowerWithMetricPrefix+0x88>)
 8000ca6:	f005 f9ca 	bl	800603e <strcat>
 8000caa:	2500      	movs	r5, #0
 8000cac:	f1c4 24ff 	rsb	r4, r4, #4278255360	; 0xff00ff00
 8000cb0:	f504 047f 	add.w	r4, r4, #16711680	; 0xff0000
 8000cb4:	4f1b      	ldr	r7, [pc, #108]	; (8000d24 <strcatDecimalPowerWithMetricPrefix+0x8c>)
 8000cb6:	34f9      	adds	r4, #249	; 0xf9
 8000cb8:	42ac      	cmp	r4, r5
 8000cba:	dc12      	bgt.n	8000ce2 <strcatDecimalPowerWithMetricPrefix+0x4a>
 8000cbc:	491a      	ldr	r1, [pc, #104]	; (8000d28 <strcatDecimalPowerWithMetricPrefix+0x90>)
 8000cbe:	4630      	mov	r0, r6
 8000cc0:	f005 f9bd 	bl	800603e <strcat>
 8000cc4:	2400      	movs	r4, #0
 8000cc6:	4919      	ldr	r1, [pc, #100]	; (8000d2c <strcatDecimalPowerWithMetricPrefix+0x94>)
 8000cc8:	4630      	mov	r0, r6
 8000cca:	f005 f9b8 	bl	800603e <strcat>
 8000cce:	230c      	movs	r3, #12
 8000cd0:	4917      	ldr	r1, [pc, #92]	; (8000d30 <strcatDecimalPowerWithMetricPrefix+0x98>)
 8000cd2:	4630      	mov	r0, r6
 8000cd4:	fb03 1104 	mla	r1, r3, r4, r1
 8000cd8:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
 8000cdc:	3108      	adds	r1, #8
 8000cde:	f005 b9ae 	b.w	800603e <strcat>
 8000ce2:	4639      	mov	r1, r7
 8000ce4:	4630      	mov	r0, r6
 8000ce6:	f005 f9aa 	bl	800603e <strcat>
 8000cea:	3501      	adds	r5, #1
 8000cec:	e7e4      	b.n	8000cb8 <strcatDecimalPowerWithMetricPrefix+0x20>
 8000cee:	2403      	movs	r4, #3
 8000cf0:	1d8b      	adds	r3, r1, #6
 8000cf2:	fbb3 f4f4 	udiv	r4, r3, r4
 8000cf6:	2c06      	cmp	r4, #6
 8000cf8:	bf28      	it	cs
 8000cfa:	2406      	movcs	r4, #6
 8000cfc:	490a      	ldr	r1, [pc, #40]	; (8000d28 <strcatDecimalPowerWithMetricPrefix+0x90>)
 8000cfe:	eb04 0544 	add.w	r5, r4, r4, lsl #1
 8000d02:	1b5d      	subs	r5, r3, r5
 8000d04:	2700      	movs	r7, #0
 8000d06:	f005 f99a 	bl	800603e <strcat>
 8000d0a:	f8df 8018 	ldr.w	r8, [pc, #24]	; 8000d24 <strcatDecimalPowerWithMetricPrefix+0x8c>
 8000d0e:	42af      	cmp	r7, r5
 8000d10:	dad9      	bge.n	8000cc6 <strcatDecimalPowerWithMetricPrefix+0x2e>
 8000d12:	4641      	mov	r1, r8
 8000d14:	4630      	mov	r0, r6
 8000d16:	f005 f992 	bl	800603e <strcat>
 8000d1a:	3701      	adds	r7, #1
 8000d1c:	e7f7      	b.n	8000d0e <strcatDecimalPowerWithMetricPrefix+0x76>
 8000d1e:	bf00      	nop
 8000d20:	0800634c 	stmdaeq	r0, {r2, r3, r6, r8, r9, sp, lr}
 8000d24:	0800766c 	stmdaeq	r0, {r2, r3, r5, r6, r9, sl, ip, sp, lr}
 8000d28:	08007665 	stmdaeq	r0, {r0, r2, r5, r6, r9, sl, ip, sp, lr}
 8000d2c:	08006506 	stmdaeq	r0, {r1, r2, r8, sl, sp, lr}
 8000d30:	080062f8 	stmdaeq	r0, {r3, r4, r5, r6, r7, r9, sp, lr}

08000d34 <strcatUInt8Hex>:
 8000d34:	b538      	push	{r3, r4, r5, lr}
 8000d36:	460c      	mov	r4, r1
 8000d38:	4605      	mov	r5, r0
 8000d3a:	f7ff fc41 	bl	80005c0 <strlen>
 8000d3e:	0923      	lsrs	r3, r4, #4
 8000d40:	2b09      	cmp	r3, #9
 8000d42:	bf94      	ite	ls
 8000d44:	3330      	addls	r3, #48	; 0x30
 8000d46:	3357      	addhi	r3, #87	; 0x57
 8000d48:	542b      	strb	r3, [r5, r0]
 8000d4a:	2300      	movs	r3, #0
 8000d4c:	f004 010f 	and.w	r1, r4, #15
 8000d50:	2909      	cmp	r1, #9
 8000d52:	eb05 0200 	add.w	r2, r5, r0
 8000d56:	bf94      	ite	ls
 8000d58:	3130      	addls	r1, #48	; 0x30
 8000d5a:	3157      	addhi	r1, #87	; 0x57
 8000d5c:	7051      	strb	r1, [r2, #1]
 8000d5e:	7093      	strb	r3, [r2, #2]
 8000d60:	bd38      	pop	{r3, r4, r5, pc}

08000d62 <strcatUInt16Hex>:
 8000d62:	b538      	push	{r3, r4, r5, lr}
 8000d64:	4605      	mov	r5, r0
 8000d66:	460c      	mov	r4, r1
 8000d68:	0a09      	lsrs	r1, r1, #8
 8000d6a:	f7ff ffe3 	bl	8000d34 <strcatUInt8Hex>
 8000d6e:	b2e1      	uxtb	r1, r4
 8000d70:	4628      	mov	r0, r5
 8000d72:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8000d76:	f7ff bfdd 	b.w	8000d34 <strcatUInt8Hex>

08000d7a <strcatUInt32Hex>:
 8000d7a:	b538      	push	{r3, r4, r5, lr}
 8000d7c:	4605      	mov	r5, r0
 8000d7e:	460c      	mov	r4, r1
 8000d80:	0c09      	lsrs	r1, r1, #16
 8000d82:	f7ff ffee 	bl	8000d62 <strcatUInt16Hex>
 8000d86:	b2a1      	uxth	r1, r4
 8000d88:	4628      	mov	r0, r5
 8000d8a:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8000d8e:	f7ff bfe8 	b.w	8000d62 <strcatUInt16Hex>

08000d92 <encodeDatalogValue>:
 8000d92:	b530      	push	{r4, r5, lr}
 8000d94:	2401      	movs	r4, #1
 8000d96:	f102 5200 	add.w	r2, r2, #536870912	; 0x20000000
 8000d9a:	3a01      	subs	r2, #1
 8000d9c:	fa04 f303 	lsl.w	r3, r4, r3
 8000da0:	00d2      	lsls	r2, r2, #3
 8000da2:	1e5d      	subs	r5, r3, #1
 8000da4:	fa40 f402 	asr.w	r4, r0, r2
 8000da8:	402c      	ands	r4, r5
 8000daa:	425b      	negs	r3, r3
 8000dac:	ea44 0343 	orr.w	r3, r4, r3, lsl #1
 8000db0:	700b      	strb	r3, [r1, #0]
 8000db2:	3a08      	subs	r2, #8
 8000db4:	f112 0f08 	cmn.w	r2, #8
 8000db8:	d100      	bne.n	8000dbc <encodeDatalogValue+0x2a>
 8000dba:	bd30      	pop	{r4, r5, pc}
 8000dbc:	fa40 f302 	asr.w	r3, r0, r2
 8000dc0:	f801 3f01 	strb.w	r3, [r1, #1]!
 8000dc4:	e7f5      	b.n	8000db2 <encodeDatalogValue+0x20>
	...

08000dc8 <onDatalogMenuGetOption>:
 8000dc8:	4b05      	ldr	r3, [pc, #20]	; (8000de0 <onDatalogMenuGetOption+0x18>)
 8000dca:	681b      	ldr	r3, [r3, #0]
 8000dcc:	f3c3 3382 	ubfx	r3, r3, #14, #3
 8000dd0:	1a58      	subs	r0, r3, r1
 8000dd2:	4243      	negs	r3, r0
 8000dd4:	4143      	adcs	r3, r0
 8000dd6:	6013      	str	r3, [r2, #0]
 8000dd8:	4b02      	ldr	r3, [pc, #8]	; (8000de4 <onDatalogMenuGetOption+0x1c>)
 8000dda:	f853 0021 	ldr.w	r0, [r3, r1, lsl #2]
 8000dde:	4770      	bx	lr
 8000de0:	20000da0 	andcs	r0, r0, r0, lsr #27
 8000de4:	08006368 	stmdaeq	r0, {r3, r5, r6, r8, r9, sp, lr}

08000de8 <writeDatalogBuffer>:
 8000de8:	b570      	push	{r4, r5, r6, lr}
 8000dea:	4b0c      	ldr	r3, [pc, #48]	; (8000e1c <writeDatalogBuffer+0x34>)
 8000dec:	4d0c      	ldr	r5, [pc, #48]	; (8000e20 <writeDatalogBuffer+0x38>)
 8000dee:	681b      	ldr	r3, [r3, #0]
 8000df0:	6b28      	ldr	r0, [r5, #48]	; 0x30
 8000df2:	f105 0634 	add.w	r6, r5, #52	; 0x34
 8000df6:	18c4      	adds	r4, r0, r3
 8000df8:	3c01      	subs	r4, #1
 8000dfa:	fbb4 f4f3 	udiv	r4, r4, r3
 8000dfe:	435c      	muls	r4, r3
 8000e00:	21fe      	movs	r1, #254	; 0xfe
 8000e02:	1a22      	subs	r2, r4, r0
 8000e04:	4430      	add	r0, r6
 8000e06:	f005 f912 	bl	800602e <memset>
 8000e0a:	4622      	mov	r2, r4
 8000e0c:	4631      	mov	r1, r6
 8000e0e:	4628      	mov	r0, r5
 8000e10:	f001 f922 	bl	8002058 <writeFlashPage>
 8000e14:	2300      	movs	r3, #0
 8000e16:	632b      	str	r3, [r5, #48]	; 0x30
 8000e18:	bd70      	pop	{r4, r5, r6, pc}
 8000e1a:	bf00      	nop
 8000e1c:	08007594 	stmdaeq	r0, {r2, r4, r7, r8, sl, ip, sp, lr}
 8000e20:	20000090 	mulcs	r0, r0, r0

08000e24 <writeDatalogEntry>:
 8000e24:	e92d 41ff 	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
 8000e28:	4b45      	ldr	r3, [pc, #276]	; (8000f40 <writeDatalogEntry+0x11c>)
 8000e2a:	4605      	mov	r5, r0
 8000e2c:	681a      	ldr	r2, [r3, #0]
 8000e2e:	4698      	mov	r8, r3
 8000e30:	f412 3fe0 	tst.w	r2, #114688	; 0x1c000
 8000e34:	d072      	beq.n	8000f1c <writeDatalogEntry+0xf8>
 8000e36:	4c43      	ldr	r4, [pc, #268]	; (8000f44 <writeDatalogEntry+0x120>)
 8000e38:	f894 3044 	ldrb.w	r3, [r4, #68]	; 0x44
 8000e3c:	2b00      	cmp	r3, #0
 8000e3e:	d16d      	bne.n	8000f1c <writeDatalogEntry+0xf8>
 8000e40:	f003 fe28 	bl	8004a94 <getDeviceTime>
 8000e44:	4607      	mov	r7, r0
 8000e46:	f002 fc0b 	bl	8003660 <getTubePulseCount>
 8000e4a:	4606      	mov	r6, r0
 8000e4c:	b16d      	cbz	r5, 8000e6a <writeDatalogEntry+0x46>
 8000e4e:	f8d8 2000 	ldr.w	r2, [r8]
 8000e52:	483d      	ldr	r0, [pc, #244]	; (8000f48 <writeDatalogEntry+0x124>)
 8000e54:	6923      	ldr	r3, [r4, #16]
 8000e56:	f3c2 3282 	ubfx	r2, r2, #14, #3
 8000e5a:	f830 2012 	ldrh.w	r2, [r0, r2, lsl #1]
 8000e5e:	1af9      	subs	r1, r7, r3
 8000e60:	4291      	cmp	r1, r2
 8000e62:	d35b      	bcc.n	8000f1c <writeDatalogEntry+0xf8>
 8000e64:	1c50      	adds	r0, r2, #1
 8000e66:	4288      	cmp	r0, r1
 8000e68:	d21a      	bcs.n	8000ea0 <writeDatalogEntry+0x7c>
 8000e6a:	e9c4 7604 	strd	r7, r6, [r4, #16]
 8000e6e:	f8d8 3000 	ldr.w	r3, [r8]
 8000e72:	2204      	movs	r2, #4
 8000e74:	f3c3 3382 	ubfx	r3, r3, #14, #3
 8000e78:	3b10      	subs	r3, #16
 8000e7a:	f88d 3004 	strb.w	r3, [sp, #4]
 8000e7e:	f10d 0105 	add.w	r1, sp, #5
 8000e82:	2308      	movs	r3, #8
 8000e84:	4638      	mov	r0, r7
 8000e86:	f7ff ff84 	bl	8000d92 <encodeDatalogValue>
 8000e8a:	2308      	movs	r3, #8
 8000e8c:	2204      	movs	r2, #4
 8000e8e:	f10d 0109 	add.w	r1, sp, #9
 8000e92:	4630      	mov	r0, r6
 8000e94:	f7ff ff7d 	bl	8000d92 <encodeDatalogValue>
 8000e98:	2509      	movs	r5, #9
 8000e9a:	f7ff ffa5 	bl	8000de8 <writeDatalogBuffer>
 8000e9e:	e02d      	b.n	8000efc <writeDatalogEntry+0xd8>
 8000ea0:	6960      	ldr	r0, [r4, #20]
 8000ea2:	4413      	add	r3, r2
 8000ea4:	1a30      	subs	r0, r6, r0
 8000ea6:	e9c4 3604 	strd	r3, r6, [r4, #16]
 8000eaa:	f100 6300 	add.w	r3, r0, #134217728	; 0x8000000
 8000eae:	f1b3 5f80 	cmp.w	r3, #268435456	; 0x10000000
 8000eb2:	d23a      	bcs.n	8000f2a <writeDatalogEntry+0x106>
 8000eb4:	f100 0340 	add.w	r3, r0, #64	; 0x40
 8000eb8:	2b7f      	cmp	r3, #127	; 0x7f
 8000eba:	d932      	bls.n	8000f22 <writeDatalogEntry+0xfe>
 8000ebc:	f500 5300 	add.w	r3, r0, #8192	; 0x2000
 8000ec0:	f5b3 4f80 	cmp.w	r3, #16384	; 0x4000
 8000ec4:	d32f      	bcc.n	8000f26 <writeDatalogEntry+0x102>
 8000ec6:	f500 1380 	add.w	r3, r0, #1048576	; 0x100000
 8000eca:	f5b3 1f00 	cmp.w	r3, #2097152	; 0x200000
 8000ece:	bf34      	ite	cc
 8000ed0:	2503      	movcc	r5, #3
 8000ed2:	2504      	movcs	r5, #4
 8000ed4:	f1c5 0308 	rsb	r3, r5, #8
 8000ed8:	462a      	mov	r2, r5
 8000eda:	a901      	add	r1, sp, #4
 8000edc:	f7ff ff59 	bl	8000d92 <encodeDatalogValue>
 8000ee0:	4a1a      	ldr	r2, [pc, #104]	; (8000f4c <writeDatalogEntry+0x128>)
 8000ee2:	6b23      	ldr	r3, [r4, #48]	; 0x30
 8000ee4:	6812      	ldr	r2, [r2, #0]
 8000ee6:	442b      	add	r3, r5
 8000ee8:	4293      	cmp	r3, r2
 8000eea:	d901      	bls.n	8000ef0 <writeDatalogEntry+0xcc>
 8000eec:	f7ff ff7c 	bl	8000de8 <writeDatalogBuffer>
 8000ef0:	4a17      	ldr	r2, [pc, #92]	; (8000f50 <writeDatalogEntry+0x12c>)
 8000ef2:	68a3      	ldr	r3, [r4, #8]
 8000ef4:	6812      	ldr	r2, [r2, #0]
 8000ef6:	442b      	add	r3, r5
 8000ef8:	4293      	cmp	r3, r2
 8000efa:	d8b8      	bhi.n	8000e6e <writeDatalogEntry+0x4a>
 8000efc:	6b23      	ldr	r3, [r4, #48]	; 0x30
 8000efe:	4815      	ldr	r0, [pc, #84]	; (8000f54 <writeDatalogEntry+0x130>)
 8000f00:	462a      	mov	r2, r5
 8000f02:	4418      	add	r0, r3
 8000f04:	a901      	add	r1, sp, #4
 8000f06:	f005 f887 	bl	8006018 <memcpy>
 8000f0a:	6b23      	ldr	r3, [r4, #48]	; 0x30
 8000f0c:	441d      	add	r5, r3
 8000f0e:	4b0f      	ldr	r3, [pc, #60]	; (8000f4c <writeDatalogEntry+0x128>)
 8000f10:	6325      	str	r5, [r4, #48]	; 0x30
 8000f12:	681b      	ldr	r3, [r3, #0]
 8000f14:	429d      	cmp	r5, r3
 8000f16:	d301      	bcc.n	8000f1c <writeDatalogEntry+0xf8>
 8000f18:	f7ff ff66 	bl	8000de8 <writeDatalogBuffer>
 8000f1c:	b004      	add	sp, #16
 8000f1e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8000f22:	2501      	movs	r5, #1
 8000f24:	e7d6      	b.n	8000ed4 <writeDatalogEntry+0xb0>
 8000f26:	2502      	movs	r5, #2
 8000f28:	e7d4      	b.n	8000ed4 <writeDatalogEntry+0xb0>
 8000f2a:	23f0      	movs	r3, #240	; 0xf0
 8000f2c:	2204      	movs	r2, #4
 8000f2e:	f88d 3004 	strb.w	r3, [sp, #4]
 8000f32:	f10d 0105 	add.w	r1, sp, #5
 8000f36:	2308      	movs	r3, #8
 8000f38:	f7ff ff2b 	bl	8000d92 <encodeDatalogValue>
 8000f3c:	2505      	movs	r5, #5
 8000f3e:	e7cf      	b.n	8000ee0 <writeDatalogEntry+0xbc>
 8000f40:	20000da0 	andcs	r0, r0, r0, lsr #27
 8000f44:	20000090 	mulcs	r0, r0, r0
 8000f48:	0800638c 	stmdaeq	r0, {r2, r3, r7, r8, r9, sp, lr}
 8000f4c:	08007594 	stmdaeq	r0, {r2, r4, r7, r8, sl, ip, sp, lr}
 8000f50:	08007598 	stmdaeq	r0, {r3, r4, r7, r8, sl, ip, sp, lr}
 8000f54:	200000c4 	andcs	r0, r0, r4, asr #1

08000f58 <onDatalogMenuSelect>:
 8000f58:	b570      	push	{r4, r5, r6, lr}
 8000f5a:	6843      	ldr	r3, [r0, #4]
 8000f5c:	4c09      	ldr	r4, [pc, #36]	; (8000f84 <onDatalogMenuSelect+0x2c>)
 8000f5e:	885e      	ldrh	r6, [r3, #2]
 8000f60:	6825      	ldr	r5, [r4, #0]
 8000f62:	f3c5 3582 	ubfx	r5, r5, #14, #3
 8000f66:	b916      	cbnz	r6, 8000f6e <onDatalogMenuSelect+0x16>
 8000f68:	4630      	mov	r0, r6
 8000f6a:	f7ff ff5b 	bl	8000e24 <writeDatalogEntry>
 8000f6e:	6823      	ldr	r3, [r4, #0]
 8000f70:	f366 3390 	bfi	r3, r6, #14, #3
 8000f74:	6023      	str	r3, [r4, #0]
 8000f76:	b925      	cbnz	r5, 8000f82 <onDatalogMenuSelect+0x2a>
 8000f78:	4628      	mov	r0, r5
 8000f7a:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 8000f7e:	f7ff bf51 	b.w	8000e24 <writeDatalogEntry>
 8000f82:	bd70      	pop	{r4, r5, r6, pc}
 8000f84:	20000da0 	andcs	r0, r0, r0, lsr #27

08000f88 <decodeDatalogValue>:
 8000f88:	e92d 41f3 	stmdb	sp!, {r0, r1, r4, r5, r6, r7, r8, lr}
 8000f8c:	2600      	movs	r6, #0
 8000f8e:	4680      	mov	r8, r0
 8000f90:	460f      	mov	r7, r1
 8000f92:	4634      	mov	r4, r6
 8000f94:	f1c2 0228 	rsb	r2, r2, #40	; 0x28
 8000f98:	eba2 05c1 	sub.w	r5, r2, r1, lsl #3
 8000f9c:	2201      	movs	r2, #1
 8000f9e:	f10d 0107 	add.w	r1, sp, #7
 8000fa2:	4640      	mov	r0, r8
 8000fa4:	f003 fc2a 	bl	80047fc <readFlash>
 8000fa8:	3601      	adds	r6, #1
 8000faa:	f89d 3007 	ldrb.w	r3, [sp, #7]
 8000fae:	42b7      	cmp	r7, r6
 8000fb0:	ea43 2404 	orr.w	r4, r3, r4, lsl #8
 8000fb4:	d1f2      	bne.n	8000f9c <decodeDatalogValue+0x14>
 8000fb6:	fa04 f005 	lsl.w	r0, r4, r5
 8000fba:	4128      	asrs	r0, r5
 8000fbc:	b002      	add	sp, #8
 8000fbe:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
	...

08000fc4 <decodeDatalogEntry>:
 8000fc4:	b573      	push	{r0, r1, r4, r5, r6, lr}
 8000fc6:	4604      	mov	r4, r0
 8000fc8:	4b2d      	ldr	r3, [pc, #180]	; (8001080 <decodeDatalogEntry+0xbc>)
 8000fca:	4e2e      	ldr	r6, [pc, #184]	; (8001084 <decodeDatalogEntry+0xc0>)
 8000fcc:	681d      	ldr	r5, [r3, #0]
 8000fce:	68a3      	ldr	r3, [r4, #8]
 8000fd0:	42ab      	cmp	r3, r5
 8000fd2:	d306      	bcc.n	8000fe2 <decodeDatalogEntry+0x1e>
 8000fd4:	4630      	mov	r0, r6
 8000fd6:	f000 ffe3 	bl	8001fa0 <isFlashPageFull>
 8000fda:	b340      	cbz	r0, 800102e <decodeDatalogEntry+0x6a>
 8000fdc:	4630      	mov	r0, r6
 8000fde:	f001 f80c 	bl	8001ffa <setFlashPageNext>
 8000fe2:	2201      	movs	r2, #1
 8000fe4:	f10d 0107 	add.w	r1, sp, #7
 8000fe8:	4620      	mov	r0, r4
 8000fea:	f003 fc07 	bl	80047fc <readFlash>
 8000fee:	f89d 3007 	ldrb.w	r3, [sp, #7]
 8000ff2:	2bef      	cmp	r3, #239	; 0xef
 8000ff4:	d821      	bhi.n	800103a <decodeDatalogEntry+0x76>
 8000ff6:	061a      	lsls	r2, r3, #24
 8000ff8:	d51b      	bpl.n	8001032 <decodeDatalogEntry+0x6e>
 8000ffa:	f003 02c0 	and.w	r2, r3, #192	; 0xc0
 8000ffe:	2a80      	cmp	r2, #128	; 0x80
 8001000:	d019      	beq.n	8001036 <decodeDatalogEntry+0x72>
 8001002:	f003 03e0 	and.w	r3, r3, #224	; 0xe0
 8001006:	2bc0      	cmp	r3, #192	; 0xc0
 8001008:	bf0c      	ite	eq
 800100a:	2103      	moveq	r1, #3
 800100c:	2104      	movne	r1, #4
 800100e:	e9d4 2303 	ldrd	r2, r3, [r4, #12]
 8001012:	4413      	add	r3, r2
 8001014:	6123      	str	r3, [r4, #16]
 8001016:	68a3      	ldr	r3, [r4, #8]
 8001018:	f1c1 0208 	rsb	r2, r1, #8
 800101c:	3b01      	subs	r3, #1
 800101e:	60a3      	str	r3, [r4, #8]
 8001020:	4620      	mov	r0, r4
 8001022:	f7ff ffb1 	bl	8000f88 <decodeDatalogValue>
 8001026:	6963      	ldr	r3, [r4, #20]
 8001028:	4418      	add	r0, r3
 800102a:	6160      	str	r0, [r4, #20]
 800102c:	2001      	movs	r0, #1
 800102e:	b002      	add	sp, #8
 8001030:	bd70      	pop	{r4, r5, r6, pc}
 8001032:	2101      	movs	r1, #1
 8001034:	e7eb      	b.n	800100e <decodeDatalogEntry+0x4a>
 8001036:	2102      	movs	r1, #2
 8001038:	e7e9      	b.n	800100e <decodeDatalogEntry+0x4a>
 800103a:	2bf0      	cmp	r3, #240	; 0xf0
 800103c:	d106      	bne.n	800104c <decodeDatalogEntry+0x88>
 800103e:	e9d4 2303 	ldrd	r2, r3, [r4, #12]
 8001042:	4413      	add	r3, r2
 8001044:	6123      	str	r3, [r4, #16]
 8001046:	2208      	movs	r2, #8
 8001048:	2104      	movs	r1, #4
 800104a:	e7e9      	b.n	8001020 <decodeDatalogEntry+0x5c>
 800104c:	2bf5      	cmp	r3, #245	; 0xf5
 800104e:	d810      	bhi.n	8001072 <decodeDatalogEntry+0xae>
 8001050:	4a0d      	ldr	r2, [pc, #52]	; (8001088 <decodeDatalogEntry+0xc4>)
 8001052:	3bf0      	subs	r3, #240	; 0xf0
 8001054:	f832 3013 	ldrh.w	r3, [r2, r3, lsl #1]
 8001058:	2104      	movs	r1, #4
 800105a:	60e3      	str	r3, [r4, #12]
 800105c:	2208      	movs	r2, #8
 800105e:	4620      	mov	r0, r4
 8001060:	f7ff ff92 	bl	8000f88 <decodeDatalogValue>
 8001064:	2208      	movs	r2, #8
 8001066:	6120      	str	r0, [r4, #16]
 8001068:	2104      	movs	r1, #4
 800106a:	4620      	mov	r0, r4
 800106c:	f7ff ff8c 	bl	8000f88 <decodeDatalogValue>
 8001070:	e7db      	b.n	800102a <decodeDatalogEntry+0x66>
 8001072:	2bff      	cmp	r3, #255	; 0xff
 8001074:	d1ab      	bne.n	8000fce <decodeDatalogEntry+0xa>
 8001076:	68a3      	ldr	r3, [r4, #8]
 8001078:	2000      	movs	r0, #0
 800107a:	3b01      	subs	r3, #1
 800107c:	60a3      	str	r3, [r4, #8]
 800107e:	e7d6      	b.n	800102e <decodeDatalogEntry+0x6a>
 8001080:	08007598 	stmdaeq	r0, {r3, r4, r7, r8, sl, ip, sp, lr}
 8001084:	200000a8 	andcs	r0, r0, r8, lsr #1
 8001088:	0800638c 	stmdaeq	r0, {r2, r3, r7, r8, r9, sp, lr}

0800108c <initDatalog>:
 800108c:	4b0a      	ldr	r3, [pc, #40]	; (80010b8 <initDatalog+0x2c>)
 800108e:	b510      	push	{r4, lr}
 8001090:	6819      	ldr	r1, [r3, #0]
 8001092:	480a      	ldr	r0, [pc, #40]	; (80010bc <initDatalog+0x30>)
 8001094:	4c0a      	ldr	r4, [pc, #40]	; (80010c0 <initDatalog+0x34>)
 8001096:	2206      	movs	r2, #6
 8001098:	f3c1 3182 	ubfx	r1, r1, #14, #3
 800109c:	f002 fae6 	bl	800366c <selectMenuItem>
 80010a0:	4b08      	ldr	r3, [pc, #32]	; (80010c4 <initDatalog+0x38>)
 80010a2:	4620      	mov	r0, r4
 80010a4:	6023      	str	r3, [r4, #0]
 80010a6:	f000 ff91 	bl	8001fcc <setFlashPageHead>
 80010aa:	4620      	mov	r0, r4
 80010ac:	f7ff ff8a 	bl	8000fc4 <decodeDatalogEntry>
 80010b0:	2800      	cmp	r0, #0
 80010b2:	d1fa      	bne.n	80010aa <initDatalog+0x1e>
 80010b4:	bd10      	pop	{r4, pc}
 80010b6:	bf00      	nop
 80010b8:	20000da0 	andcs	r0, r0, r0, lsr #27
 80010bc:	08006354 	stmdaeq	r0, {r2, r4, r6, r8, r9, sp, lr}
 80010c0:	20000090 	mulcs	r0, r0, r0
 80010c4:	08007569 	stmdaeq	r0, {r0, r3, r5, r6, r8, sl, ip, sp, lr}

080010c8 <updateDatalog>:
 80010c8:	2001      	movs	r0, #1
 80010ca:	f7ff beab 	b.w	8000e24 <writeDatalogEntry>

080010ce <writeDatalog>:
 80010ce:	2000      	movs	r0, #0
 80010d0:	f7ff bea8 	b.w	8000e24 <writeDatalogEntry>

080010d4 <lockDatalog>:
 80010d4:	4a03      	ldr	r2, [pc, #12]	; (80010e4 <lockDatalog+0x10>)
 80010d6:	f892 3044 	ldrb.w	r3, [r2, #68]	; 0x44
 80010da:	3301      	adds	r3, #1
 80010dc:	f882 3044 	strb.w	r3, [r2, #68]	; 0x44
 80010e0:	4770      	bx	lr
 80010e2:	bf00      	nop
 80010e4:	20000090 	mulcs	r0, r0, r0

080010e8 <unlockDatalog>:
 80010e8:	4a03      	ldr	r2, [pc, #12]	; (80010f8 <unlockDatalog+0x10>)
 80010ea:	f892 3044 	ldrb.w	r3, [r2, #68]	; 0x44
 80010ee:	3b01      	subs	r3, #1
 80010f0:	f882 3044 	strb.w	r3, [r2, #68]	; 0x44
 80010f4:	4770      	bx	lr
 80010f6:	bf00      	nop
 80010f8:	20000090 	mulcs	r0, r0, r0

080010fc <startDatalogDownload>:
 80010fc:	b508      	push	{r3, lr}
 80010fe:	f7ff ffe9 	bl	80010d4 <lockDatalog>
 8001102:	4b04      	ldr	r3, [pc, #16]	; (8001114 <startDatalogDownload+0x18>)
 8001104:	4804      	ldr	r0, [pc, #16]	; (8001118 <startDatalogDownload+0x1c>)
 8001106:	f840 3f18 	str.w	r3, [r0, #24]!
 800110a:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 800110e:	f000 bf90 	b.w	8002032 <setFlashPageTail>
 8001112:	bf00      	nop
 8001114:	08007569 	stmdaeq	r0, {r0, r3, r5, r6, r8, sl, ip, sp, lr}
 8001118:	20000090 	mulcs	r0, r0, r0

0800111c <getDatalogDownloadEntry>:
 800111c:	b538      	push	{r3, r4, r5, lr}
 800111e:	4605      	mov	r5, r0
 8001120:	4807      	ldr	r0, [pc, #28]	; (8001140 <getDatalogDownloadEntry+0x24>)
 8001122:	f7ff ff4f 	bl	8000fc4 <decodeDatalogEntry>
 8001126:	4604      	mov	r4, r0
 8001128:	b130      	cbz	r0, 8001138 <getDatalogDownloadEntry+0x1c>
 800112a:	4b06      	ldr	r3, [pc, #24]	; (8001144 <getDatalogDownloadEntry+0x28>)
 800112c:	e893 0003 	ldmia.w	r3, {r0, r1}
 8001130:	e885 0003 	stmia.w	r5, {r0, r1}
 8001134:	4620      	mov	r0, r4
 8001136:	bd38      	pop	{r3, r4, r5, pc}
 8001138:	f7ff ffd6 	bl	80010e8 <unlockDatalog>
 800113c:	e7fa      	b.n	8001134 <getDatalogDownloadEntry+0x18>
 800113e:	bf00      	nop
 8001140:	200000a8 	andcs	r0, r0, r8, lsr #1
 8001144:	200000b8 	strhcs	r0, [r0], -r8

08001148 <onDisplayMenuGetOption>:
 8001148:	2302      	movs	r3, #2
 800114a:	6013      	str	r3, [r2, #0]
 800114c:	4b01      	ldr	r3, [pc, #4]	; (8001154 <onDisplayMenuGetOption+0xc>)
 800114e:	f853 0031 	ldr.w	r0, [r3, r1, lsl #3]
 8001152:	4770      	bx	lr
 8001154:	08006458 	stmdaeq	r0, {r3, r4, r6, sl, sp, lr}

08001158 <onDisplayBrightnessMenuGetOption>:
 8001158:	4b05      	ldr	r3, [pc, #20]	; (8001170 <onDisplayBrightnessMenuGetOption+0x18>)
 800115a:	799b      	ldrb	r3, [r3, #6]
 800115c:	f3c3 1301 	ubfx	r3, r3, #4, #2
 8001160:	1a58      	subs	r0, r3, r1
 8001162:	4243      	negs	r3, r0
 8001164:	4143      	adcs	r3, r0
 8001166:	6013      	str	r3, [r2, #0]
 8001168:	4b02      	ldr	r3, [pc, #8]	; (8001174 <onDisplayBrightnessMenuGetOption+0x1c>)
 800116a:	f853 0021 	ldr.w	r0, [r3, r1, lsl #2]
 800116e:	4770      	bx	lr
 8001170:	20000da0 	andcs	r0, r0, r0, lsr #27
 8001174:	0800640c 	stmdaeq	r0, {r2, r3, sl, sp, lr}

08001178 <onDisplaySleepMenuGetOption>:
 8001178:	4b05      	ldr	r3, [pc, #20]	; (8001190 <onDisplaySleepMenuGetOption+0x18>)
 800117a:	88db      	ldrh	r3, [r3, #6]
 800117c:	f3c3 1382 	ubfx	r3, r3, #6, #3
 8001180:	1a58      	subs	r0, r3, r1
 8001182:	4243      	negs	r3, r0
 8001184:	4143      	adcs	r3, r0
 8001186:	6013      	str	r3, [r2, #0]
 8001188:	4b02      	ldr	r3, [pc, #8]	; (8001194 <onDisplaySleepMenuGetOption+0x1c>)
 800118a:	f853 0021 	ldr.w	r0, [r3, r1, lsl #2]
 800118e:	4770      	bx	lr
 8001190:	20000da0 	andcs	r0, r0, r0, lsr #27
 8001194:	08006494 	stmdaeq	r0, {r2, r4, r7, sl, sp, lr}

08001198 <onDisplaySubMenuBack>:
 8001198:	4801      	ldr	r0, [pc, #4]	; (80011a0 <onDisplaySubMenuBack+0x8>)
 800119a:	f004 b875 	b.w	8005288 <setView>
 800119e:	bf00      	nop
 80011a0:	08006478 	stmdaeq	r0, {r3, r4, r5, r6, sl, sp, lr}

080011a4 <onDisplayMenuSelect>:
 80011a4:	6843      	ldr	r3, [r0, #4]
 80011a6:	885a      	ldrh	r2, [r3, #2]
 80011a8:	4b02      	ldr	r3, [pc, #8]	; (80011b4 <onDisplayMenuSelect+0x10>)
 80011aa:	eb03 03c2 	add.w	r3, r3, r2, lsl #3
 80011ae:	6858      	ldr	r0, [r3, #4]
 80011b0:	f004 b86a 	b.w	8005288 <setView>
 80011b4:	08006458 	stmdaeq	r0, {r3, r4, r6, sl, sp, lr}

080011b8 <onDisplayBrightnessMenuSelect>:
 80011b8:	6841      	ldr	r1, [r0, #4]
 80011ba:	4b04      	ldr	r3, [pc, #16]	; (80011cc <onDisplayBrightnessMenuSelect+0x14>)
 80011bc:	8849      	ldrh	r1, [r1, #2]
 80011be:	799a      	ldrb	r2, [r3, #6]
 80011c0:	2001      	movs	r0, #1
 80011c2:	f361 1205 	bfi	r2, r1, #4, #2
 80011c6:	719a      	strb	r2, [r3, #6]
 80011c8:	f003 ba92 	b.w	80046f0 <setDisplayBacklight>
 80011cc:	20000da0 	andcs	r0, r0, r0, lsr #27

080011d0 <onDisplaySleepMenuSelect>:
 80011d0:	4b04      	ldr	r3, [pc, #16]	; (80011e4 <onDisplaySleepMenuSelect+0x14>)
 80011d2:	6841      	ldr	r1, [r0, #4]
 80011d4:	88da      	ldrh	r2, [r3, #6]
 80011d6:	8849      	ldrh	r1, [r1, #2]
 80011d8:	f361 1288 	bfi	r2, r1, #6, #3
 80011dc:	80da      	strh	r2, [r3, #6]
 80011de:	f000 be85 	b.w	8001eec <triggerDisplay>
 80011e2:	bf00      	nop
 80011e4:	20000da0 	andcs	r0, r0, r0, lsr #27

080011e8 <onDisplayContrastMenuSelect>:
 80011e8:	4b04      	ldr	r3, [pc, #16]	; (80011fc <onDisplayContrastMenuSelect+0x14>)
 80011ea:	6841      	ldr	r1, [r0, #4]
 80011ec:	799a      	ldrb	r2, [r3, #6]
 80011ee:	8849      	ldrh	r1, [r1, #2]
 80011f0:	f361 0243 	bfi	r2, r1, #1, #3
 80011f4:	719a      	strb	r2, [r3, #6]
 80011f6:	f002 bfbf 	b.w	8004178 <updateDisplayContrast>
 80011fa:	bf00      	nop
 80011fc:	20000da0 	andcs	r0, r0, r0, lsr #27

08001200 <onDisplayContrastMenuGetOption>:
 8001200:	2907      	cmp	r1, #7
 8001202:	b510      	push	{r4, lr}
 8001204:	460c      	mov	r4, r1
 8001206:	d812      	bhi.n	800122e <onDisplayContrastMenuGetOption+0x2e>
 8001208:	4b0a      	ldr	r3, [pc, #40]	; (8001234 <onDisplayContrastMenuGetOption+0x34>)
 800120a:	480b      	ldr	r0, [pc, #44]	; (8001238 <onDisplayContrastMenuGetOption+0x38>)
 800120c:	799b      	ldrb	r3, [r3, #6]
 800120e:	f3c3 0342 	ubfx	r3, r3, #1, #3
 8001212:	1a59      	subs	r1, r3, r1
 8001214:	424b      	negs	r3, r1
 8001216:	414b      	adcs	r3, r1
 8001218:	6013      	str	r3, [r2, #0]
 800121a:	4908      	ldr	r1, [pc, #32]	; (800123c <onDisplayContrastMenuGetOption+0x3c>)
 800121c:	f004 ff1e 	bl	800605c <strcpy>
 8001220:	4805      	ldr	r0, [pc, #20]	; (8001238 <onDisplayContrastMenuGetOption+0x38>)
 8001222:	2200      	movs	r2, #0
 8001224:	1c61      	adds	r1, r4, #1
 8001226:	f7ff fc41 	bl	8000aac <strcatUInt32>
 800122a:	4803      	ldr	r0, [pc, #12]	; (8001238 <onDisplayContrastMenuGetOption+0x38>)
 800122c:	bd10      	pop	{r4, pc}
 800122e:	2000      	movs	r0, #0
 8001230:	e7fc      	b.n	800122c <onDisplayContrastMenuGetOption+0x2c>
 8001232:	bf00      	nop
 8001234:	20000da0 	andcs	r0, r0, r0, lsr #27
 8001238:	20000d7c 	andcs	r0, r0, ip, ror sp
 800123c:	08006e58 	stmdaeq	r0, {r3, r4, r6, r9, sl, fp, sp, lr}

08001240 <setFillColor>:
 8001240:	2810      	cmp	r0, #16
 8001242:	b510      	push	{r4, lr}
 8001244:	d016      	beq.n	8001274 <setFillColor+0x34>
 8001246:	f648 1330 	movw	r3, #35120	; 0x8930
 800124a:	fa23 f000 	lsr.w	r0, r3, r0
 800124e:	43c0      	mvns	r0, r0
 8001250:	f64f 71ff 	movw	r1, #65535	; 0xffff
 8001254:	f000 0001 	and.w	r0, r0, #1
 8001258:	2800      	cmp	r0, #0
 800125a:	bf16      	itet	ne
 800125c:	460c      	movne	r4, r1
 800125e:	2400      	moveq	r4, #0
 8001260:	2100      	movne	r1, #0
 8001262:	4806      	ldr	r0, [pc, #24]	; (800127c <setFillColor+0x3c>)
 8001264:	f004 f9be 	bl	80055e4 <mr_set_fill_color>
 8001268:	4621      	mov	r1, r4
 800126a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800126e:	4803      	ldr	r0, [pc, #12]	; (800127c <setFillColor+0x3c>)
 8001270:	f004 b9c0 	b.w	80055f4 <mr_set_text_color>
 8001274:	f64f 74ff 	movw	r4, #65535	; 0xffff
 8001278:	2100      	movs	r1, #0
 800127a:	e7f2      	b.n	8001262 <setFillColor+0x22>
 800127c:	20000cd0 	ldrdcs	r0, [r0], -r0	; <UNPREDICTABLE>

08001280 <drawText>:
 8001280:	4613      	mov	r3, r2
 8001282:	460a      	mov	r2, r1
 8001284:	4601      	mov	r1, r0
 8001286:	4801      	ldr	r0, [pc, #4]	; (800128c <drawText+0xc>)
 8001288:	f004 b9b8 	b.w	80055fc <mr_draw_text>
 800128c:	20000cd0 	ldrdcs	r0, [r0], -r0	; <UNPREDICTABLE>

08001290 <setFont>:
 8001290:	4601      	mov	r1, r0
 8001292:	4801      	ldr	r0, [pc, #4]	; (8001298 <setFont+0x8>)
 8001294:	f004 b9ab 	b.w	80055ee <mr_set_font>
 8001298:	20000cd0 	ldrdcs	r0, [r0], -r0	; <UNPREDICTABLE>

0800129c <drawTitle>:
 800129c:	b51f      	push	{r0, r1, r2, r3, r4, lr}
 800129e:	4604      	mov	r4, r0
 80012a0:	4a09      	ldr	r2, [pc, #36]	; (80012c8 <drawTitle+0x2c>)
 80012a2:	ab02      	add	r3, sp, #8
 80012a4:	6810      	ldr	r0, [r2, #0]
 80012a6:	6851      	ldr	r1, [r2, #4]
 80012a8:	c303      	stmia	r3!, {r0, r1}
 80012aa:	6890      	ldr	r0, [r2, #8]
 80012ac:	9001      	str	r0, [sp, #4]
 80012ae:	4807      	ldr	r0, [pc, #28]	; (80012cc <drawTitle+0x30>)
 80012b0:	f7ff ffee 	bl	8001290 <setFont>
 80012b4:	2002      	movs	r0, #2
 80012b6:	f7ff ffc3 	bl	8001240 <setFillColor>
 80012ba:	aa01      	add	r2, sp, #4
 80012bc:	a902      	add	r1, sp, #8
 80012be:	4620      	mov	r0, r4
 80012c0:	f7ff ffde 	bl	8001280 <drawText>
 80012c4:	b004      	add	sp, #16
 80012c6:	bd10      	pop	{r4, pc}
 80012c8:	08006084 	stmdaeq	r0, {r2, r7, sp, lr}
 80012cc:	08006778 	stmdaeq	r0, {r3, r4, r5, r6, r8, r9, sl, sp, lr}

080012d0 <drawRectangle>:
 80012d0:	4601      	mov	r1, r0
 80012d2:	4801      	ldr	r0, [pc, #4]	; (80012d8 <drawRectangle+0x8>)
 80012d4:	f004 b989 	b.w	80055ea <mr_draw_rectangle>
 80012d8:	20000cd0 	ldrdcs	r0, [r0], -r0	; <UNPREDICTABLE>

080012dc <drawValueAndUnit.isra.1>:
 80012dc:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
 80012e0:	4681      	mov	r9, r0
 80012e2:	b085      	sub	sp, #20
 80012e4:	ac02      	add	r4, sp, #8
 80012e6:	460f      	mov	r7, r1
 80012e8:	e9d2 0100 	ldrd	r0, r1, [r2]
 80012ec:	e884 0003 	stmia.w	r4, {r0, r1}
 80012f0:	4649      	mov	r1, r9
 80012f2:	4814      	ldr	r0, [pc, #80]	; (8001344 <drawValueAndUnit.isra.1+0x68>)
 80012f4:	461d      	mov	r5, r3
 80012f6:	f8b3 8000 	ldrh.w	r8, [r3]
 80012fa:	4616      	mov	r6, r2
 80012fc:	f004 f988 	bl	8005610 <mr_get_text_width>
 8001300:	4440      	add	r0, r8
 8001302:	f8ad 000c 	strh.w	r0, [sp, #12]
 8001306:	462a      	mov	r2, r5
 8001308:	4621      	mov	r1, r4
 800130a:	4648      	mov	r0, r9
 800130c:	f7ff ffb8 	bl	8001280 <drawText>
 8001310:	f8bd 100c 	ldrh.w	r1, [sp, #12]
 8001314:	88b3      	ldrh	r3, [r6, #4]
 8001316:	f8bd 2008 	ldrh.w	r2, [sp, #8]
 800131a:	1a5b      	subs	r3, r3, r1
 800131c:	f8ad 300c 	strh.w	r3, [sp, #12]
 8001320:	2300      	movs	r3, #0
 8001322:	440a      	add	r2, r1
 8001324:	f8ad 3004 	strh.w	r3, [sp, #4]
 8001328:	886b      	ldrh	r3, [r5, #2]
 800132a:	f8ad 2008 	strh.w	r2, [sp, #8]
 800132e:	4621      	mov	r1, r4
 8001330:	aa01      	add	r2, sp, #4
 8001332:	4638      	mov	r0, r7
 8001334:	f8ad 3006 	strh.w	r3, [sp, #6]
 8001338:	f7ff ffa2 	bl	8001280 <drawText>
 800133c:	b005      	add	sp, #20
 800133e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 8001342:	bf00      	nop
 8001344:	20000cd0 	ldrdcs	r0, [r0], -r0	; <UNPREDICTABLE>

08001348 <drawRightAlignedText>:
 8001348:	b537      	push	{r0, r1, r2, r4, r5, lr}
 800134a:	6813      	ldr	r3, [r2, #0]
 800134c:	4604      	mov	r4, r0
 800134e:	460d      	mov	r5, r1
 8001350:	4601      	mov	r1, r0
 8001352:	4808      	ldr	r0, [pc, #32]	; (8001374 <drawRightAlignedText+0x2c>)
 8001354:	9301      	str	r3, [sp, #4]
 8001356:	f004 f95b 	bl	8005610 <mr_get_text_width>
 800135a:	f8bd 3004 	ldrh.w	r3, [sp, #4]
 800135e:	aa02      	add	r2, sp, #8
 8001360:	1a18      	subs	r0, r3, r0
 8001362:	f822 0d04 	strh.w	r0, [r2, #-4]!
 8001366:	4629      	mov	r1, r5
 8001368:	4620      	mov	r0, r4
 800136a:	f7ff ff89 	bl	8001280 <drawText>
 800136e:	b003      	add	sp, #12
 8001370:	bd30      	pop	{r4, r5, pc}
 8001372:	bf00      	nop
 8001374:	20000cd0 	ldrdcs	r0, [r0], -r0	; <UNPREDICTABLE>

08001378 <drawCenteredText>:
 8001378:	b537      	push	{r0, r1, r2, r4, r5, lr}
 800137a:	6813      	ldr	r3, [r2, #0]
 800137c:	4604      	mov	r4, r0
 800137e:	460d      	mov	r5, r1
 8001380:	4601      	mov	r1, r0
 8001382:	4809      	ldr	r0, [pc, #36]	; (80013a8 <drawCenteredText+0x30>)
 8001384:	9301      	str	r3, [sp, #4]
 8001386:	f004 f943 	bl	8005610 <mr_get_text_width>
 800138a:	2302      	movs	r3, #2
 800138c:	fb90 f0f3 	sdiv	r0, r0, r3
 8001390:	f8bd 3004 	ldrh.w	r3, [sp, #4]
 8001394:	aa02      	add	r2, sp, #8
 8001396:	1a18      	subs	r0, r3, r0
 8001398:	f822 0d04 	strh.w	r0, [r2, #-4]!
 800139c:	4629      	mov	r1, r5
 800139e:	4620      	mov	r0, r4
 80013a0:	f7ff ff6e 	bl	8001280 <drawText>
 80013a4:	b003      	add	sp, #12
 80013a6:	bd30      	pop	{r4, r5, pc}
 80013a8:	20000cd0 	ldrdcs	r0, [r0], -r0	; <UNPREDICTABLE>

080013ac <initDisplay>:
 80013ac:	b510      	push	{r4, lr}
 80013ae:	4c0e      	ldr	r4, [pc, #56]	; (80013e8 <initDisplay+0x3c>)
 80013b0:	f002 fef8 	bl	80041a4 <initDisplayController>
 80013b4:	f003 f964 	bl	8004680 <initDisplayBacklight>
 80013b8:	79a1      	ldrb	r1, [r4, #6]
 80013ba:	2208      	movs	r2, #8
 80013bc:	f3c1 0142 	ubfx	r1, r1, #1, #3
 80013c0:	480a      	ldr	r0, [pc, #40]	; (80013ec <initDisplay+0x40>)
 80013c2:	f002 f953 	bl	800366c <selectMenuItem>
 80013c6:	88e1      	ldrh	r1, [r4, #6]
 80013c8:	2207      	movs	r2, #7
 80013ca:	f3c1 1182 	ubfx	r1, r1, #6, #3
 80013ce:	4808      	ldr	r0, [pc, #32]	; (80013f0 <initDisplay+0x44>)
 80013d0:	f002 f94c 	bl	800366c <selectMenuItem>
 80013d4:	79a1      	ldrb	r1, [r4, #6]
 80013d6:	2204      	movs	r2, #4
 80013d8:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 80013dc:	f3c1 1101 	ubfx	r1, r1, #4, #2
 80013e0:	4804      	ldr	r0, [pc, #16]	; (80013f4 <initDisplay+0x48>)
 80013e2:	f002 b943 	b.w	800366c <selectMenuItem>
 80013e6:	bf00      	nop
 80013e8:	20000da0 	andcs	r0, r0, r0, lsr #27
 80013ec:	08006428 	stmdaeq	r0, {r3, r5, sl, sp, lr}
 80013f0:	08006480 	stmdaeq	r0, {r7, sl, sp, lr}
 80013f4:	080063f8 	stmdaeq	r0, {r3, r4, r5, r6, r7, r8, r9, sp, lr}

080013f8 <setDisplayOn>:
 80013f8:	4b02      	ldr	r3, [pc, #8]	; (8001404 <setDisplayOn+0xc>)
 80013fa:	4601      	mov	r1, r0
 80013fc:	7018      	strb	r0, [r3, #0]
 80013fe:	4802      	ldr	r0, [pc, #8]	; (8001408 <setDisplayOn+0x10>)
 8001400:	f004 b8ea 	b.w	80055d8 <mr_set_display>
 8001404:	20000ccc 	andcs	r0, r0, ip, asr #25
 8001408:	20000cd0 	ldrdcs	r0, [r0], -r0	; <UNPREDICTABLE>

0800140c <refreshDisplay>:
 800140c:	4801      	ldr	r0, [pc, #4]	; (8001414 <refreshDisplay+0x8>)
 800140e:	f004 b8e5 	b.w	80055dc <mr_refresh_display>
 8001412:	bf00      	nop
 8001414:	20000cd0 	ldrdcs	r0, [r0], -r0	; <UNPREDICTABLE>

08001418 <drawStatusBar>:
 8001418:	b510      	push	{r4, lr}
 800141a:	200a      	movs	r0, #10
 800141c:	b08c      	sub	sp, #48	; 0x30
 800141e:	f7ff ff0f 	bl	8001240 <setFillColor>
 8001422:	a806      	add	r0, sp, #24
 8001424:	f002 fc89 	bl	8003d3a <getDeviceDateTime>
 8001428:	2300      	movs	r3, #0
 800142a:	9304      	str	r3, [sp, #16]
 800142c:	f8ad 3014 	strh.w	r3, [sp, #20]
 8001430:	f240 73e3 	movw	r3, #2019	; 0x7e3
 8001434:	f8bd 2018 	ldrh.w	r2, [sp, #24]
 8001438:	429a      	cmp	r2, r3
 800143a:	d90f      	bls.n	800145c <drawStatusBar+0x44>
 800143c:	2202      	movs	r2, #2
 800143e:	f89d 101c 	ldrb.w	r1, [sp, #28]
 8001442:	a804      	add	r0, sp, #16
 8001444:	f7ff fb32 	bl	8000aac <strcatUInt32>
 8001448:	4929      	ldr	r1, [pc, #164]	; (80014f0 <drawStatusBar+0xd8>)
 800144a:	a804      	add	r0, sp, #16
 800144c:	f004 fdf7 	bl	800603e <strcat>
 8001450:	2202      	movs	r2, #2
 8001452:	f89d 101d 	ldrb.w	r1, [sp, #29]
 8001456:	a804      	add	r0, sp, #16
 8001458:	f7ff fb28 	bl	8000aac <strcatUInt32>
 800145c:	4a25      	ldr	r2, [pc, #148]	; (80014f4 <drawStatusBar+0xdc>)
 800145e:	ab08      	add	r3, sp, #32
 8001460:	6810      	ldr	r0, [r2, #0]
 8001462:	6851      	ldr	r1, [r2, #4]
 8001464:	ac0c      	add	r4, sp, #48	; 0x30
 8001466:	c303      	stmia	r3!, {r0, r1}
 8001468:	2302      	movs	r3, #2
 800146a:	4823      	ldr	r0, [pc, #140]	; (80014f8 <drawStatusBar+0xe0>)
 800146c:	f844 3d2c 	str.w	r3, [r4, #-44]!
 8001470:	f7ff ff0e 	bl	8001290 <setFont>
 8001474:	4622      	mov	r2, r4
 8001476:	a908      	add	r1, sp, #32
 8001478:	a804      	add	r0, sp, #16
 800147a:	f7ff ff01 	bl	8001280 <drawText>
 800147e:	f002 f995 	bl	80037ac <getDeviceBatteryLevel>
 8001482:	2300      	movs	r3, #0
 8001484:	4a1d      	ldr	r2, [pc, #116]	; (80014fc <drawStatusBar+0xe4>)
 8001486:	9302      	str	r3, [sp, #8]
 8001488:	6813      	ldr	r3, [r2, #0]
 800148a:	8812      	ldrh	r2, [r2, #0]
 800148c:	f413 33e0 	ands.w	r3, r3, #114688	; 0x1c000
 8001490:	bf18      	it	ne
 8001492:	2338      	movne	r3, #56	; 0x38
 8001494:	f402 527f 	and.w	r2, r2, #16320	; 0x3fc0
 8001498:	bf1c      	itt	ne
 800149a:	f88d 3008 	strbne.w	r3, [sp, #8]
 800149e:	2301      	movne	r3, #1
 80014a0:	b12a      	cbz	r2, 80014ae <drawStatusBar+0x96>
 80014a2:	2137      	movs	r1, #55	; 0x37
 80014a4:	aa0c      	add	r2, sp, #48	; 0x30
 80014a6:	441a      	add	r2, r3
 80014a8:	f802 1c28 	strb.w	r1, [r2, #-40]
 80014ac:	3301      	adds	r3, #1
 80014ae:	1c42      	adds	r2, r0, #1
 80014b0:	aa0c      	add	r2, sp, #48	; 0x30
 80014b2:	4413      	add	r3, r2
 80014b4:	f04f 0200 	mov.w	r2, #0
 80014b8:	bf0c      	ite	eq
 80014ba:	2036      	moveq	r0, #54	; 0x36
 80014bc:	3030      	addne	r0, #48	; 0x30
 80014be:	f803 2c27 	strb.w	r2, [r3, #-39]
 80014c2:	4a0f      	ldr	r2, [pc, #60]	; (8001500 <drawStatusBar+0xe8>)
 80014c4:	bf18      	it	ne
 80014c6:	b2c0      	uxtbne	r0, r0
 80014c8:	6851      	ldr	r1, [r2, #4]
 80014ca:	f803 0c28 	strb.w	r0, [r3, #-40]
 80014ce:	6810      	ldr	r0, [r2, #0]
 80014d0:	ab0a      	add	r3, sp, #40	; 0x28
 80014d2:	c303      	stmia	r3!, {r0, r1}
 80014d4:	233e      	movs	r3, #62	; 0x3e
 80014d6:	ac0c      	add	r4, sp, #48	; 0x30
 80014d8:	f844 3d24 	str.w	r3, [r4, #-36]!
 80014dc:	4809      	ldr	r0, [pc, #36]	; (8001504 <drawStatusBar+0xec>)
 80014de:	f7ff fed7 	bl	8001290 <setFont>
 80014e2:	4622      	mov	r2, r4
 80014e4:	a90a      	add	r1, sp, #40	; 0x28
 80014e6:	a802      	add	r0, sp, #8
 80014e8:	f7ff ff2e 	bl	8001348 <drawRightAlignedText>
 80014ec:	b00c      	add	sp, #48	; 0x30
 80014ee:	bd10      	pop	{r4, pc}
 80014f0:	0800634f 	stmdaeq	r0, {r0, r1, r2, r3, r6, r8, r9, sp, lr}
 80014f4:	08006090 	stmdaeq	r0, {r4, r7, sp, lr}
 80014f8:	08006bd8 	stmdaeq	r0, {r3, r4, r6, r7, r8, r9, fp, sp, lr}
 80014fc:	20000da0 	andcs	r0, r0, r0, lsr #27
 8001500:	08006098 	stmdaeq	r0, {r3, r4, r7, sp, lr}
 8001504:	08006b4c 	stmdaeq	r0, {r2, r3, r6, r8, r9, fp, sp, lr}

08001508 <drawNotification>:
 8001508:	b570      	push	{r4, r5, r6, lr}
 800150a:	4614      	mov	r4, r2
 800150c:	b086      	sub	sp, #24
 800150e:	4606      	mov	r6, r0
 8001510:	4816      	ldr	r0, [pc, #88]	; (800156c <drawNotification+0x64>)
 8001512:	460d      	mov	r5, r1
 8001514:	f7ff febc 	bl	8001290 <setFont>
 8001518:	2003      	movs	r0, #3
 800151a:	f7ff fe91 	bl	8001240 <setFillColor>
 800151e:	b13c      	cbz	r4, 8001530 <drawNotification+0x28>
 8001520:	4a13      	ldr	r2, [pc, #76]	; (8001570 <drawNotification+0x68>)
 8001522:	ab04      	add	r3, sp, #16
 8001524:	6810      	ldr	r0, [r2, #0]
 8001526:	6851      	ldr	r1, [r2, #4]
 8001528:	c303      	stmia	r3!, {r0, r1}
 800152a:	a804      	add	r0, sp, #16
 800152c:	f7ff fed0 	bl	80012d0 <drawRectangle>
 8001530:	4c10      	ldr	r4, [pc, #64]	; (8001574 <drawNotification+0x6c>)
 8001532:	ab02      	add	r3, sp, #8
 8001534:	6820      	ldr	r0, [r4, #0]
 8001536:	6861      	ldr	r1, [r4, #4]
 8001538:	466a      	mov	r2, sp
 800153a:	c303      	stmia	r3!, {r0, r1}
 800153c:	68a0      	ldr	r0, [r4, #8]
 800153e:	a902      	add	r1, sp, #8
 8001540:	9000      	str	r0, [sp, #0]
 8001542:	4630      	mov	r0, r6
 8001544:	f7ff ff18 	bl	8001378 <drawCenteredText>
 8001548:	4622      	mov	r2, r4
 800154a:	f852 0f0c 	ldr.w	r0, [r2, #12]!
 800154e:	ab04      	add	r3, sp, #16
 8001550:	6851      	ldr	r1, [r2, #4]
 8001552:	c303      	stmia	r3!, {r0, r1}
 8001554:	6960      	ldr	r0, [r4, #20]
 8001556:	9001      	str	r0, [sp, #4]
 8001558:	4807      	ldr	r0, [pc, #28]	; (8001578 <drawNotification+0x70>)
 800155a:	f7ff fe99 	bl	8001290 <setFont>
 800155e:	aa01      	add	r2, sp, #4
 8001560:	a904      	add	r1, sp, #16
 8001562:	4628      	mov	r0, r5
 8001564:	f7ff ff08 	bl	8001378 <drawCenteredText>
 8001568:	b006      	add	sp, #24
 800156a:	bd70      	pop	{r4, r5, r6, pc}
 800156c:	08006778 	stmdaeq	r0, {r3, r4, r5, r6, r8, r9, sl, sp, lr}
 8001570:	080060a0 	stmdaeq	r0, {r5, r7, sp, lr}
 8001574:	080060a8 	stmdaeq	r0, {r3, r5, r7, sp, lr}
 8001578:	08006bd8 	stmdaeq	r0, {r3, r4, r6, r7, r8, r9, fp, sp, lr}

0800157c <drawMenu>:
 800157c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8001580:	4c43      	ldr	r4, [pc, #268]	; (8001690 <drawMenu+0x114>)
 8001582:	b089      	sub	sp, #36	; 0x24
 8001584:	6861      	ldr	r1, [r4, #4]
 8001586:	4605      	mov	r5, r0
 8001588:	6820      	ldr	r0, [r4, #0]
 800158a:	ab04      	add	r3, sp, #16
 800158c:	c303      	stmia	r3!, {r0, r1}
 800158e:	2340      	movs	r3, #64	; 0x40
 8001590:	ae08      	add	r6, sp, #32
 8001592:	f846 3d20 	str.w	r3, [r6, #-32]!
 8001596:	483f      	ldr	r0, [pc, #252]	; (8001694 <drawMenu+0x118>)
 8001598:	f7ff fe7a 	bl	8001290 <setFont>
 800159c:	2002      	movs	r0, #2
 800159e:	f7ff fe4f 	bl	8001240 <setFillColor>
 80015a2:	4632      	mov	r2, r6
 80015a4:	a904      	add	r1, sp, #16
 80015a6:	6828      	ldr	r0, [r5, #0]
 80015a8:	f7ff fee6 	bl	8001378 <drawCenteredText>
 80015ac:	483a      	ldr	r0, [pc, #232]	; (8001698 <drawMenu+0x11c>)
 80015ae:	f7ff fe6f 	bl	8001290 <setFont>
 80015b2:	686b      	ldr	r3, [r5, #4]
 80015b4:	68a0      	ldr	r0, [r4, #8]
 80015b6:	881e      	ldrh	r6, [r3, #0]
 80015b8:	885f      	ldrh	r7, [r3, #2]
 80015ba:	230c      	movs	r3, #12
 80015bc:	f8ad 301e 	strh.w	r3, [sp, #30]
 80015c0:	2310      	movs	r3, #16
 80015c2:	9001      	str	r0, [sp, #4]
 80015c4:	68e0      	ldr	r0, [r4, #12]
 80015c6:	2400      	movs	r4, #0
 80015c8:	f8df 80d0 	ldr.w	r8, [pc, #208]	; 800169c <drawMenu+0x120>
 80015cc:	f8df 90d0 	ldr.w	r9, [pc, #208]	; 80016a0 <drawMenu+0x124>
 80015d0:	f8ad 301a 	strh.w	r3, [sp, #26]
 80015d4:	9002      	str	r0, [sp, #8]
 80015d6:	eb06 0b04 	add.w	fp, r6, r4
 80015da:	68ab      	ldr	r3, [r5, #8]
 80015dc:	aa03      	add	r2, sp, #12
 80015de:	4659      	mov	r1, fp
 80015e0:	4628      	mov	r0, r5
 80015e2:	4798      	blx	r3
 80015e4:	4682      	mov	sl, r0
 80015e6:	2800      	cmp	r0, #0
 80015e8:	d03c      	beq.n	8001664 <drawMenu+0xe8>
 80015ea:	2380      	movs	r3, #128	; 0x80
 80015ec:	455f      	cmp	r7, fp
 80015ee:	bf14      	ite	ne
 80015f0:	2003      	movne	r0, #3
 80015f2:	200b      	moveq	r0, #11
 80015f4:	f8ad 3018 	strh.w	r3, [sp, #24]
 80015f8:	f7ff fe22 	bl	8001240 <setFillColor>
 80015fc:	9b03      	ldr	r3, [sp, #12]
 80015fe:	079a      	lsls	r2, r3, #30
 8001600:	d50c      	bpl.n	800161c <drawMenu+0xa0>
 8001602:	2308      	movs	r3, #8
 8001604:	f8ad 301c 	strh.w	r3, [sp, #28]
 8001608:	f8bd 3018 	ldrh.w	r3, [sp, #24]
 800160c:	aa02      	add	r2, sp, #8
 800160e:	3b08      	subs	r3, #8
 8001610:	a906      	add	r1, sp, #24
 8001612:	4640      	mov	r0, r8
 8001614:	f8ad 3018 	strh.w	r3, [sp, #24]
 8001618:	f7ff fe32 	bl	8001280 <drawText>
 800161c:	9b03      	ldr	r3, [sp, #12]
 800161e:	07db      	lsls	r3, r3, #31
 8001620:	d50c      	bpl.n	800163c <drawMenu+0xc0>
 8001622:	220b      	movs	r2, #11
 8001624:	f9bd 3018 	ldrsh.w	r3, [sp, #24]
 8001628:	f8ad 201c 	strh.w	r2, [sp, #28]
 800162c:	3b0b      	subs	r3, #11
 800162e:	aa02      	add	r2, sp, #8
 8001630:	a906      	add	r1, sp, #24
 8001632:	4648      	mov	r0, r9
 8001634:	f8ad 3018 	strh.w	r3, [sp, #24]
 8001638:	f7ff fe22 	bl	8001280 <drawText>
 800163c:	f8bd 3018 	ldrh.w	r3, [sp, #24]
 8001640:	aa01      	add	r2, sp, #4
 8001642:	f8ad 301c 	strh.w	r3, [sp, #28]
 8001646:	2300      	movs	r3, #0
 8001648:	a906      	add	r1, sp, #24
 800164a:	4650      	mov	r0, sl
 800164c:	f8ad 3018 	strh.w	r3, [sp, #24]
 8001650:	f7ff fe16 	bl	8001280 <drawText>
 8001654:	f8bd 301a 	ldrh.w	r3, [sp, #26]
 8001658:	3401      	adds	r4, #1
 800165a:	330c      	adds	r3, #12
 800165c:	2c04      	cmp	r4, #4
 800165e:	f8ad 301a 	strh.w	r3, [sp, #26]
 8001662:	d1b8      	bne.n	80015d6 <drawMenu+0x5a>
 8001664:	2300      	movs	r3, #0
 8001666:	f8ad 3018 	strh.w	r3, [sp, #24]
 800166a:	2380      	movs	r3, #128	; 0x80
 800166c:	f8ad 301c 	strh.w	r3, [sp, #28]
 8001670:	f8bd 301a 	ldrh.w	r3, [sp, #26]
 8001674:	2003      	movs	r0, #3
 8001676:	f1c3 0340 	rsb	r3, r3, #64	; 0x40
 800167a:	f8ad 301e 	strh.w	r3, [sp, #30]
 800167e:	f7ff fddf 	bl	8001240 <setFillColor>
 8001682:	a806      	add	r0, sp, #24
 8001684:	f7ff fe24 	bl	80012d0 <drawRectangle>
 8001688:	b009      	add	sp, #36	; 0x24
 800168a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800168e:	bf00      	nop
 8001690:	080060cc 	stmdaeq	r0, {r2, r3, r6, r7, sp, lr}
 8001694:	08006bd8 	stmdaeq	r0, {r3, r4, r6, r7, r8, r9, fp, sp, lr}
 8001698:	08006778 	stmdaeq	r0, {r3, r4, r5, r6, r8, r9, sl, sp, lr}
 800169c:	080064c7 	stmdaeq	r0, {r0, r1, r2, r6, r7, sl, sp, lr}
 80016a0:	080064c9 	stmdaeq	r0, {r0, r3, r6, r7, sl, sp, lr}

080016a4 <drawMeasurement>:
 80016a4:	b5f0      	push	{r4, r5, r6, r7, lr}
 80016a6:	460d      	mov	r5, r1
 80016a8:	461c      	mov	r4, r3
 80016aa:	b09b      	sub	sp, #108	; 0x6c
 80016ac:	4617      	mov	r7, r2
 80016ae:	f7ff fdf5 	bl	800129c <drawTitle>
 80016b2:	4a49      	ldr	r2, [pc, #292]	; (80017d8 <drawMeasurement+0x134>)
 80016b4:	ab08      	add	r3, sp, #32
 80016b6:	6810      	ldr	r0, [r2, #0]
 80016b8:	6851      	ldr	r1, [r2, #4]
 80016ba:	c303      	stmia	r3!, {r0, r1}
 80016bc:	6890      	ldr	r0, [r2, #8]
 80016be:	9001      	str	r0, [sp, #4]
 80016c0:	4846      	ldr	r0, [pc, #280]	; (80017dc <drawMeasurement+0x138>)
 80016c2:	f7ff fde5 	bl	8001290 <setFont>
 80016c6:	a908      	add	r1, sp, #32
 80016c8:	4628      	mov	r0, r5
 80016ca:	aa01      	add	r2, sp, #4
 80016cc:	f7ff fdd8 	bl	8001280 <drawText>
 80016d0:	2100      	movs	r1, #0
 80016d2:	4620      	mov	r0, r4
 80016d4:	f002 feed 	bl	80044b2 <__aeabi_fcmpeq>
 80016d8:	2800      	cmp	r0, #0
 80016da:	d177      	bne.n	80017cc <drawMeasurement+0x128>
 80016dc:	4940      	ldr	r1, [pc, #256]	; (80017e0 <drawMeasurement+0x13c>)
 80016de:	a80a      	add	r0, sp, #40	; 0x28
 80016e0:	f004 fcbc 	bl	800605c <strcpy>
 80016e4:	493f      	ldr	r1, [pc, #252]	; (80017e4 <drawMeasurement+0x140>)
 80016e6:	4620      	mov	r0, r4
 80016e8:	f002 feea 	bl	80044c0 <__aeabi_fcmplt>
 80016ec:	493e      	ldr	r1, [pc, #248]	; (80017e8 <drawMeasurement+0x144>)
 80016ee:	1c05      	adds	r5, r0, #0
 80016f0:	4620      	mov	r0, r4
 80016f2:	bf18      	it	ne
 80016f4:	2501      	movne	r5, #1
 80016f6:	f002 fecd 	bl	8004494 <__aeabi_fmul>
 80016fa:	462a      	mov	r2, r5
 80016fc:	4601      	mov	r1, r0
 80016fe:	a80a      	add	r0, sp, #40	; 0x28
 8001700:	f7ff fa38 	bl	8000b74 <strcatFloat>
 8001704:	4939      	ldr	r1, [pc, #228]	; (80017ec <drawMeasurement+0x148>)
 8001706:	4c3a      	ldr	r4, [pc, #232]	; (80017f0 <drawMeasurement+0x14c>)
 8001708:	a80c      	add	r0, sp, #48	; 0x30
 800170a:	f004 fca7 	bl	800605c <strcpy>
 800170e:	6820      	ldr	r0, [r4, #0]
 8001710:	6861      	ldr	r1, [r4, #4]
 8001712:	ab0e      	add	r3, sp, #56	; 0x38
 8001714:	c303      	stmia	r3!, {r0, r1}
 8001716:	f44f 3380 	mov.w	r3, #65536	; 0x10000
 800171a:	ad1a      	add	r5, sp, #104	; 0x68
 800171c:	f845 3d60 	str.w	r3, [r5, #-96]!
 8001720:	4834      	ldr	r0, [pc, #208]	; (80017f4 <drawMeasurement+0x150>)
 8001722:	f7ff fdb5 	bl	8001290 <setFont>
 8001726:	462b      	mov	r3, r5
 8001728:	aa0e      	add	r2, sp, #56	; 0x38
 800172a:	a90c      	add	r1, sp, #48	; 0x30
 800172c:	a80a      	add	r0, sp, #40	; 0x28
 800172e:	f7ff fdd5 	bl	80012dc <drawValueAndUnit.isra.1>
 8001732:	4622      	mov	r2, r4
 8001734:	2500      	movs	r5, #0
 8001736:	f852 0f08 	ldr.w	r0, [r2, #8]!
 800173a:	ae1a      	add	r6, sp, #104	; 0x68
 800173c:	6851      	ldr	r1, [r2, #4]
 800173e:	ab10      	add	r3, sp, #64	; 0x40
 8001740:	c303      	stmia	r3!, {r0, r1}
 8001742:	f846 5d5c 	str.w	r5, [r6, #-92]!
 8001746:	482c      	ldr	r0, [pc, #176]	; (80017f8 <drawMeasurement+0x154>)
 8001748:	f7ff fda2 	bl	8001290 <setFont>
 800174c:	4632      	mov	r2, r6
 800174e:	a910      	add	r1, sp, #64	; 0x40
 8001750:	4638      	mov	r0, r7
 8001752:	f7ff fd95 	bl	8001280 <drawText>
 8001756:	4622      	mov	r2, r4
 8001758:	270c      	movs	r7, #12
 800175a:	f852 0f10 	ldr.w	r0, [r2, #16]!
 800175e:	ae1a      	add	r6, sp, #104	; 0x68
 8001760:	6851      	ldr	r1, [r2, #4]
 8001762:	ab12      	add	r3, sp, #72	; 0x48
 8001764:	c303      	stmia	r3!, {r0, r1}
 8001766:	f846 7d58 	str.w	r7, [r6, #-88]!
 800176a:	4822      	ldr	r0, [pc, #136]	; (80017f4 <drawMeasurement+0x150>)
 800176c:	f7ff fd90 	bl	8001290 <setFont>
 8001770:	4632      	mov	r2, r6
 8001772:	a912      	add	r1, sp, #72	; 0x48
 8001774:	4821      	ldr	r0, [pc, #132]	; (80017fc <drawMeasurement+0x158>)
 8001776:	f7ff fd83 	bl	8001280 <drawText>
 800177a:	4622      	mov	r2, r4
 800177c:	f852 0f18 	ldr.w	r0, [r2, #24]!
 8001780:	ab14      	add	r3, sp, #80	; 0x50
 8001782:	6851      	ldr	r1, [r2, #4]
 8001784:	aa1a      	add	r2, sp, #104	; 0x68
 8001786:	c303      	stmia	r3!, {r0, r1}
 8001788:	f842 7d54 	str.w	r7, [r2, #-84]!
 800178c:	a914      	add	r1, sp, #80	; 0x50
 800178e:	9820      	ldr	r0, [sp, #128]	; 0x80
 8001790:	f7ff fd76 	bl	8001280 <drawText>
 8001794:	4622      	mov	r2, r4
 8001796:	f852 0f20 	ldr.w	r0, [r2, #32]!
 800179a:	ab16      	add	r3, sp, #88	; 0x58
 800179c:	6851      	ldr	r1, [r2, #4]
 800179e:	aa1a      	add	r2, sp, #104	; 0x68
 80017a0:	c303      	stmia	r3!, {r0, r1}
 80017a2:	f842 5d50 	str.w	r5, [r2, #-80]!
 80017a6:	a916      	add	r1, sp, #88	; 0x58
 80017a8:	9821      	ldr	r0, [sp, #132]	; 0x84
 80017aa:	f7ff fd69 	bl	8001280 <drawText>
 80017ae:	f854 0f28 	ldr.w	r0, [r4, #40]!
 80017b2:	ab18      	add	r3, sp, #96	; 0x60
 80017b4:	6861      	ldr	r1, [r4, #4]
 80017b6:	aa18      	add	r2, sp, #96	; 0x60
 80017b8:	c303      	stmia	r3!, {r0, r1}
 80017ba:	e9dd 0122 	ldrd	r0, r1, [sp, #136]	; 0x88
 80017be:	ab1a      	add	r3, sp, #104	; 0x68
 80017c0:	f843 5d4c 	str.w	r5, [r3, #-76]!
 80017c4:	f7ff fd8a 	bl	80012dc <drawValueAndUnit.isra.1>
 80017c8:	b01b      	add	sp, #108	; 0x6c
 80017ca:	bdf0      	pop	{r4, r5, r6, r7, pc}
 80017cc:	490c      	ldr	r1, [pc, #48]	; (8001800 <drawMeasurement+0x15c>)
 80017ce:	a80a      	add	r0, sp, #40	; 0x28
 80017d0:	f004 fc44 	bl	800605c <strcpy>
 80017d4:	490a      	ldr	r1, [pc, #40]	; (8001800 <drawMeasurement+0x15c>)
 80017d6:	e796      	b.n	8001706 <drawMeasurement+0x62>
 80017d8:	080060e4 	stmdaeq	r0, {r2, r5, r6, r7, sp, lr}
 80017dc:	08006664 	stmdaeq	r0, {r2, r5, r6, r9, sl, sp, lr}
 80017e0:	080064c5 	stmdaeq	r0, {r0, r2, r6, r7, sl, sp, lr}
 80017e4:	3dcccccd 	stclcc	12, cr12, [ip, #820]	; 0x334
 80017e8:	42c80000 	sbcmi	r0, r8, #0
 80017ec:	080075d9 	stmdaeq	r0, {r0, r3, r4, r6, r7, r8, sl, ip, sp, lr}
 80017f0:	080060f0 	stmdaeq	r0, {r4, r5, r6, r7, sp, lr}
 80017f4:	08006bd8 	stmdaeq	r0, {r3, r4, r6, r7, r8, r9, fp, sp, lr}
 80017f8:	08006778 	stmdaeq	r0, {r3, r4, r5, r6, r8, r9, sl, sp, lr}
 80017fc:	08006255 	stmdaeq	r0, {r0, r2, r4, r6, r9, sp, lr}
 8001800:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}

08001804 <drawHistory>:
 8001804:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8001808:	b099      	sub	sp, #100	; 0x64
 800180a:	4688      	mov	r8, r1
 800180c:	4616      	mov	r6, r2
 800180e:	9301      	str	r3, [sp, #4]
 8001810:	9c23      	ldr	r4, [sp, #140]	; 0x8c
 8001812:	f7ff fd43 	bl	800129c <drawTitle>
 8001816:	4a64      	ldr	r2, [pc, #400]	; (80019a8 <drawHistory+0x1a4>)
 8001818:	4b64      	ldr	r3, [pc, #400]	; (80019ac <drawHistory+0x1a8>)
 800181a:	ad18      	add	r5, sp, #96	; 0x60
 800181c:	e9cd 2306 	strd	r2, r3, [sp, #24]
 8001820:	2329      	movs	r3, #41	; 0x29
 8001822:	4863      	ldr	r0, [pc, #396]	; (80019b0 <drawHistory+0x1ac>)
 8001824:	f845 3d50 	str.w	r3, [r5, #-80]!
 8001828:	f7ff fd32 	bl	8001290 <setFont>
 800182c:	462a      	mov	r2, r5
 800182e:	a906      	add	r1, sp, #24
 8001830:	4640      	mov	r0, r8
 8001832:	f7ff fd89 	bl	8001348 <drawRightAlignedText>
 8001836:	495f      	ldr	r1, [pc, #380]	; (80019b4 <drawHistory+0x1b0>)
 8001838:	4b5f      	ldr	r3, [pc, #380]	; (80019b8 <drawHistory+0x1b4>)
 800183a:	aa18      	add	r2, sp, #96	; 0x60
 800183c:	e9cd 1308 	strd	r1, r3, [sp, #32]
 8001840:	2377      	movs	r3, #119	; 0x77
 8001842:	a908      	add	r1, sp, #32
 8001844:	f842 3d4c 	str.w	r3, [r2, #-76]!
 8001848:	4630      	mov	r0, r6
 800184a:	f7ff fd7d 	bl	8001348 <drawRightAlignedText>
 800184e:	f44f 2360 	mov.w	r3, #917504	; 0xe0000
 8001852:	4d5a      	ldr	r5, [pc, #360]	; (80019bc <drawHistory+0x1b8>)
 8001854:	a80a      	add	r0, sp, #40	; 0x28
 8001856:	e9cd 350a 	strd	r3, r5, [sp, #40]	; 0x28
 800185a:	f7ff fd39 	bl	80012d0 <drawRectangle>
 800185e:	4b58      	ldr	r3, [pc, #352]	; (80019c0 <drawHistory+0x1bc>)
 8001860:	a80c      	add	r0, sp, #48	; 0x30
 8001862:	e9cd 350c 	strd	r3, r5, [sp, #48]	; 0x30
 8001866:	f7ff fd33 	bl	80012d0 <drawRectangle>
 800186a:	2301      	movs	r3, #1
 800186c:	2023      	movs	r0, #35	; 0x23
 800186e:	a918      	add	r1, sp, #96	; 0x60
 8001870:	f801 3d20 	strb.w	r3, [r1, #-32]!
 8001874:	42a3      	cmp	r3, r4
 8001876:	d976      	bls.n	8001966 <drawHistory+0x162>
 8001878:	2700      	movs	r7, #0
 800187a:	2008      	movs	r0, #8
 800187c:	f7ff fce0 	bl	8001240 <setFillColor>
 8001880:	2423      	movs	r4, #35	; 0x23
 8001882:	46b8      	mov	r8, r7
 8001884:	46b9      	mov	r9, r7
 8001886:	b239      	sxth	r1, r7
 8001888:	4588      	cmp	r8, r1
 800188a:	d17a      	bne.n	8001982 <drawHistory+0x17e>
 800188c:	2377      	movs	r3, #119	; 0x77
 800188e:	f109 0901 	add.w	r9, r9, #1
 8001892:	fa0f f989 	sxth.w	r9, r9
 8001896:	fb03 f309 	mul.w	r3, r3, r9
 800189a:	9a22      	ldr	r2, [sp, #136]	; 0x88
 800189c:	fbb3 f3f2 	udiv	r3, r3, r2
 80018a0:	2201      	movs	r2, #1
 80018a2:	fa0f f883 	sxth.w	r8, r3
 80018a6:	2f77      	cmp	r7, #119	; 0x77
 80018a8:	f04f 0523 	mov.w	r5, #35	; 0x23
 80018ac:	d06b      	beq.n	8001986 <drawHistory+0x182>
 80018ae:	9b01      	ldr	r3, [sp, #4]
 80018b0:	443b      	add	r3, r7
 80018b2:	785b      	ldrb	r3, [r3, #1]
 80018b4:	f1c3 03ff 	rsb	r3, r3, #255	; 0xff
 80018b8:	435d      	muls	r5, r3
 80018ba:	122d      	asrs	r5, r5, #8
 80018bc:	3501      	adds	r5, #1
 80018be:	b22d      	sxth	r5, r5
 80018c0:	42a5      	cmp	r5, r4
 80018c2:	bfa6      	itte	ge
 80018c4:	46a3      	movge	fp, r4
 80018c6:	462c      	movge	r4, r5
 80018c8:	46ab      	movlt	fp, r5
 80018ca:	2c23      	cmp	r4, #35	; 0x23
 80018cc:	d001      	beq.n	80018d2 <drawHistory+0xce>
 80018ce:	3401      	adds	r4, #1
 80018d0:	b224      	sxth	r4, r4
 80018d2:	2600      	movs	r6, #0
 80018d4:	1d3b      	adds	r3, r7, #4
 80018d6:	f8ad 3038 	strh.w	r3, [sp, #56]	; 0x38
 80018da:	f04f 0301 	mov.w	r3, #1
 80018de:	46b2      	mov	sl, r6
 80018e0:	f8ad 303c 	strh.w	r3, [sp, #60]	; 0x3c
 80018e4:	2a00      	cmp	r2, #0
 80018e6:	d151      	bne.n	800198c <drawHistory+0x188>
 80018e8:	ab18      	add	r3, sp, #96	; 0x60
 80018ea:	4453      	add	r3, sl
 80018ec:	f813 cc20 	ldrb.w	ip, [r3, #-32]
 80018f0:	f01a 0f01 	tst.w	sl, #1
 80018f4:	bf14      	ite	ne
 80018f6:	2007      	movne	r0, #7
 80018f8:	2008      	moveq	r0, #8
 80018fa:	45b4      	cmp	ip, r6
 80018fc:	d101      	bne.n	8001902 <drawHistory+0xfe>
 80018fe:	f10a 0a01 	add.w	sl, sl, #1
 8001902:	2f00      	cmp	r7, #0
 8001904:	d046      	beq.n	8001994 <drawHistory+0x190>
 8001906:	2977      	cmp	r1, #119	; 0x77
 8001908:	d044      	beq.n	8001994 <drawHistory+0x190>
 800190a:	2e23      	cmp	r6, #35	; 0x23
 800190c:	d042      	beq.n	8001994 <drawHistory+0x190>
 800190e:	45b3      	cmp	fp, r6
 8001910:	dc42      	bgt.n	8001998 <drawHistory+0x194>
 8001912:	42b4      	cmp	r4, r6
 8001914:	dc42      	bgt.n	800199c <drawHistory+0x198>
 8001916:	2807      	cmp	r0, #7
 8001918:	f04f 0323 	mov.w	r3, #35	; 0x23
 800191c:	d041      	beq.n	80019a2 <drawHistory+0x19e>
 800191e:	2006      	movs	r0, #6
 8001920:	b2b6      	uxth	r6, r6
 8001922:	459c      	cmp	ip, r3
 8001924:	bfd4      	ite	le
 8001926:	ebc6 030c 	rsble	r3, r6, ip
 800192a:	ebc6 0303 	rsbgt	r3, r6, r3
 800192e:	f106 0e15 	add.w	lr, r6, #21
 8001932:	e9cd 2102 	strd	r2, r1, [sp, #8]
 8001936:	f8ad 303e 	strh.w	r3, [sp, #62]	; 0x3e
 800193a:	f8ad e03a 	strh.w	lr, [sp, #58]	; 0x3a
 800193e:	f7ff fc7f 	bl	8001240 <setFillColor>
 8001942:	a80e      	add	r0, sp, #56	; 0x38
 8001944:	f7ff fcc4 	bl	80012d0 <drawRectangle>
 8001948:	e9dd 2102 	ldrd	r2, r1, [sp, #8]
 800194c:	f8bd 303e 	ldrh.w	r3, [sp, #62]	; 0x3e
 8001950:	441e      	add	r6, r3
 8001952:	b236      	sxth	r6, r6
 8001954:	2e23      	cmp	r6, #35	; 0x23
 8001956:	ddc5      	ble.n	80018e4 <drawHistory+0xe0>
 8001958:	3701      	adds	r7, #1
 800195a:	2f78      	cmp	r7, #120	; 0x78
 800195c:	462c      	mov	r4, r5
 800195e:	d192      	bne.n	8001886 <drawHistory+0x82>
 8001960:	b019      	add	sp, #100	; 0x64
 8001962:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 8001966:	fb00 f203 	mul.w	r2, r0, r3
 800196a:	fbb2 f2f4 	udiv	r2, r2, r4
 800196e:	eb01 0543 	add.w	r5, r1, r3, lsl #1
 8001972:	b2d2      	uxtb	r2, r2
 8001974:	f805 2c01 	strb.w	r2, [r5, #-1]
 8001978:	3201      	adds	r2, #1
 800197a:	f801 2013 	strb.w	r2, [r1, r3, lsl #1]
 800197e:	3301      	adds	r3, #1
 8001980:	e778      	b.n	8001874 <drawHistory+0x70>
 8001982:	2200      	movs	r2, #0
 8001984:	e78f      	b.n	80018a6 <drawHistory+0xa2>
 8001986:	46a3      	mov	fp, r4
 8001988:	462c      	mov	r4, r5
 800198a:	e7a2      	b.n	80018d2 <drawHistory+0xce>
 800198c:	f04f 0c24 	mov.w	ip, #36	; 0x24
 8001990:	2008      	movs	r0, #8
 8001992:	e7b6      	b.n	8001902 <drawHistory+0xfe>
 8001994:	2324      	movs	r3, #36	; 0x24
 8001996:	e7c3      	b.n	8001920 <drawHistory+0x11c>
 8001998:	465b      	mov	r3, fp
 800199a:	e7c1      	b.n	8001920 <drawHistory+0x11c>
 800199c:	4623      	mov	r3, r4
 800199e:	2004      	movs	r0, #4
 80019a0:	e7be      	b.n	8001920 <drawHistory+0x11c>
 80019a2:	2005      	movs	r0, #5
 80019a4:	e7bc      	b.n	8001920 <drawHistory+0x11c>
 80019a6:	bf00      	nop
 80019a8:	000e0052 	andeq	r0, lr, r2, asr r0
 80019ac:	0007002a 	andeq	r0, r7, sl, lsr #32
 80019b0:	08006bd8 	stmdaeq	r0, {r3, r4, r6, r7, r8, r9, fp, sp, lr}
 80019b4:	00390004 	eorseq	r0, r9, r4
 80019b8:	00070078 	andeq	r0, r7, r8, ror r0
 80019bc:	00320004 	eorseq	r0, r2, r4
 80019c0:	000e007c 	andeq	r0, lr, ip, ror r0

080019c4 <drawRNG>:
 80019c4:	b570      	push	{r4, r5, r6, lr}
 80019c6:	4c15      	ldr	r4, [pc, #84]	; (8001a1c <drawRNG+0x58>)
 80019c8:	b086      	sub	sp, #24
 80019ca:	460e      	mov	r6, r1
 80019cc:	4615      	mov	r5, r2
 80019ce:	f7ff fc65 	bl	800129c <drawTitle>
 80019d2:	4622      	mov	r2, r4
 80019d4:	f852 0f9c 	ldr.w	r0, [r2, #156]!
 80019d8:	ab02      	add	r3, sp, #8
 80019da:	6851      	ldr	r1, [r2, #4]
 80019dc:	c303      	stmia	r3!, {r0, r1}
 80019de:	f8d4 00a4 	ldr.w	r0, [r4, #164]	; 0xa4
 80019e2:	9000      	str	r0, [sp, #0]
 80019e4:	480e      	ldr	r0, [pc, #56]	; (8001a20 <drawRNG+0x5c>)
 80019e6:	f7ff fc53 	bl	8001290 <setFont>
 80019ea:	466a      	mov	r2, sp
 80019ec:	a902      	add	r1, sp, #8
 80019ee:	4630      	mov	r0, r6
 80019f0:	f7ff fc46 	bl	8001280 <drawText>
 80019f4:	f854 0fa8 	ldr.w	r0, [r4, #168]!
 80019f8:	ab04      	add	r3, sp, #16
 80019fa:	6861      	ldr	r1, [r4, #4]
 80019fc:	ac06      	add	r4, sp, #24
 80019fe:	c303      	stmia	r3!, {r0, r1}
 8001a00:	2340      	movs	r3, #64	; 0x40
 8001a02:	4808      	ldr	r0, [pc, #32]	; (8001a24 <drawRNG+0x60>)
 8001a04:	f844 3d14 	str.w	r3, [r4, #-20]!
 8001a08:	f7ff fc42 	bl	8001290 <setFont>
 8001a0c:	4622      	mov	r2, r4
 8001a0e:	a904      	add	r1, sp, #16
 8001a10:	4628      	mov	r0, r5
 8001a12:	f7ff fcb1 	bl	8001378 <drawCenteredText>
 8001a16:	b006      	add	sp, #24
 8001a18:	bd70      	pop	{r4, r5, r6, pc}
 8001a1a:	bf00      	nop
 8001a1c:	08006084 	stmdaeq	r0, {r2, r7, sp, lr}
 8001a20:	08006bd8 	stmdaeq	r0, {r3, r4, r6, r7, r8, r9, fp, sp, lr}
 8001a24:	08006778 	stmdaeq	r0, {r3, r4, r5, r6, r8, r9, sl, sp, lr}

08001a28 <drawStatistics>:
 8001a28:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 8001a2c:	484f      	ldr	r0, [pc, #316]	; (8001b6c <drawStatistics+0x144>)
 8001a2e:	b098      	sub	sp, #96	; 0x60
 8001a30:	f7ff fc34 	bl	800129c <drawTitle>
 8001a34:	484e      	ldr	r0, [pc, #312]	; (8001b70 <drawStatistics+0x148>)
 8001a36:	f7ff fc2b 	bl	8001290 <setFont>
 8001a3a:	2500      	movs	r5, #0
 8001a3c:	f04f 587e 	mov.w	r8, #1065353216	; 0x3f800000
 8001a40:	4f4c      	ldr	r7, [pc, #304]	; (8001b74 <drawStatistics+0x14c>)
 8001a42:	f8df 9164 	ldr.w	r9, [pc, #356]	; 8001ba8 <drawStatistics+0x180>
 8001a46:	4639      	mov	r1, r7
 8001a48:	a808      	add	r0, sp, #32
 8001a4a:	f004 fb07 	bl	800605c <strcpy>
 8001a4e:	4639      	mov	r1, r7
 8001a50:	a810      	add	r0, sp, #64	; 0x40
 8001a52:	f004 fb03 	bl	800605c <strcpy>
 8001a56:	4639      	mov	r1, r7
 8001a58:	a804      	add	r0, sp, #16
 8001a5a:	f004 faff 	bl	800605c <strcpy>
 8001a5e:	1e6b      	subs	r3, r5, #1
 8001a60:	2b03      	cmp	r3, #3
 8001a62:	d803      	bhi.n	8001a6c <drawStatistics+0x44>
 8001a64:	e8df f003 	tbb	[pc, r3]
 8001a68:	7469493d 	strbtvc	r4, [r9], #-2365	; 0xfffff6c3
 8001a6c:	4942      	ldr	r1, [pc, #264]	; (8001b78 <drawStatistics+0x150>)
 8001a6e:	a808      	add	r0, sp, #32
 8001a70:	f004 faf4 	bl	800605c <strcpy>
 8001a74:	f001 fde8 	bl	8003648 <getTubeTime>
 8001a78:	4601      	mov	r1, r0
 8001a7a:	a810      	add	r0, sp, #64	; 0x40
 8001a7c:	f7ff f850 	bl	8000b20 <strcatTime>
 8001a80:	4b3e      	ldr	r3, [pc, #248]	; (8001b7c <drawStatistics+0x154>)
 8001a82:	f04f 0a00 	mov.w	sl, #0
 8001a86:	9303      	str	r3, [sp, #12]
 8001a88:	230c      	movs	r3, #12
 8001a8a:	ae18      	add	r6, sp, #96	; 0x60
 8001a8c:	00ec      	lsls	r4, r5, #3
 8001a8e:	f846 3d5c 	str.w	r3, [r6, #-92]!
 8001a92:	3415      	adds	r4, #21
 8001a94:	b224      	sxth	r4, r4
 8001a96:	4632      	mov	r2, r6
 8001a98:	a902      	add	r1, sp, #8
 8001a9a:	a808      	add	r0, sp, #32
 8001a9c:	f8ad a008 	strh.w	sl, [sp, #8]
 8001aa0:	f8ad 400a 	strh.w	r4, [sp, #10]
 8001aa4:	f7ff fbec 	bl	8001280 <drawText>
 8001aa8:	2348      	movs	r3, #72	; 0x48
 8001aaa:	f8ad 3008 	strh.w	r3, [sp, #8]
 8001aae:	4b34      	ldr	r3, [pc, #208]	; (8001b80 <drawStatistics+0x158>)
 8001ab0:	aa02      	add	r2, sp, #8
 8001ab2:	9303      	str	r3, [sp, #12]
 8001ab4:	a904      	add	r1, sp, #16
 8001ab6:	4633      	mov	r3, r6
 8001ab8:	a810      	add	r0, sp, #64	; 0x40
 8001aba:	3501      	adds	r5, #1
 8001abc:	f8ad 400a 	strh.w	r4, [sp, #10]
 8001ac0:	f8cd a004 	str.w	sl, [sp, #4]
 8001ac4:	f7ff fc0a 	bl	80012dc <drawValueAndUnit.isra.1>
 8001ac8:	2d05      	cmp	r5, #5
 8001aca:	d1bc      	bne.n	8001a46 <drawStatistics+0x1e>
 8001acc:	f44f 1274 	mov.w	r2, #3997696	; 0x3d0000
 8001ad0:	4b2c      	ldr	r3, [pc, #176]	; (8001b84 <drawStatistics+0x15c>)
 8001ad2:	a802      	add	r0, sp, #8
 8001ad4:	e9cd 2302 	strd	r2, r3, [sp, #8]
 8001ad8:	f7ff fbfa 	bl	80012d0 <drawRectangle>
 8001adc:	b018      	add	sp, #96	; 0x60
 8001ade:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 8001ae2:	4929      	ldr	r1, [pc, #164]	; (8001b88 <drawStatistics+0x160>)
 8001ae4:	a808      	add	r0, sp, #32
 8001ae6:	f004 fab9 	bl	800605c <strcpy>
 8001aea:	f001 fdb9 	bl	8003660 <getTubePulseCount>
 8001aee:	2200      	movs	r2, #0
 8001af0:	4601      	mov	r1, r0
 8001af2:	a810      	add	r0, sp, #64	; 0x40
 8001af4:	f7fe ffda 	bl	8000aac <strcatUInt32>
 8001af8:	e7c2      	b.n	8001a80 <drawStatistics+0x58>
 8001afa:	4924      	ldr	r1, [pc, #144]	; (8001b8c <drawStatistics+0x164>)
 8001afc:	a808      	add	r0, sp, #32
 8001afe:	f004 faad 	bl	800605c <strcpy>
 8001b02:	f000 f9e5 	bl	8001ed0 <getTubeDeadTime>
 8001b06:	4641      	mov	r1, r8
 8001b08:	4604      	mov	r4, r0
 8001b0a:	f002 fcde 	bl	80044ca <__aeabi_fcmpge>
 8001b0e:	b120      	cbz	r0, 8001b1a <drawStatistics+0xf2>
 8001b10:	491f      	ldr	r1, [pc, #124]	; (8001b90 <drawStatistics+0x168>)
 8001b12:	a810      	add	r0, sp, #64	; 0x40
 8001b14:	f004 faa2 	bl	800605c <strcpy>
 8001b18:	e7b2      	b.n	8001a80 <drawStatistics+0x58>
 8001b1a:	491e      	ldr	r1, [pc, #120]	; (8001b94 <drawStatistics+0x16c>)
 8001b1c:	a810      	add	r0, sp, #64	; 0x40
 8001b1e:	f004 fa8e 	bl	800603e <strcat>
 8001b22:	4649      	mov	r1, r9
 8001b24:	4620      	mov	r0, r4
 8001b26:	f002 fcb5 	bl	8004494 <__aeabi_fmul>
 8001b2a:	2201      	movs	r2, #1
 8001b2c:	4601      	mov	r1, r0
 8001b2e:	a810      	add	r0, sp, #64	; 0x40
 8001b30:	f7ff f820 	bl	8000b74 <strcatFloat>
 8001b34:	4918      	ldr	r1, [pc, #96]	; (8001b98 <drawStatistics+0x170>)
 8001b36:	a804      	add	r0, sp, #16
 8001b38:	e7ec      	b.n	8001b14 <drawStatistics+0xec>
 8001b3a:	4918      	ldr	r1, [pc, #96]	; (8001b9c <drawStatistics+0x174>)
 8001b3c:	a808      	add	r0, sp, #32
 8001b3e:	f004 fa8d 	bl	800605c <strcpy>
 8001b42:	f002 fff1 	bl	8004b28 <getDeviceId>
 8001b46:	4601      	mov	r1, r0
 8001b48:	a804      	add	r0, sp, #16
 8001b4a:	f7ff f916 	bl	8000d7a <strcatUInt32Hex>
 8001b4e:	e797      	b.n	8001a80 <drawStatistics+0x58>
 8001b50:	4913      	ldr	r1, [pc, #76]	; (8001ba0 <drawStatistics+0x178>)
 8001b52:	a808      	add	r0, sp, #32
 8001b54:	f004 fa82 	bl	800605c <strcpy>
 8001b58:	f002 fc56 	bl	8004408 <getDeviceTemperature>
 8001b5c:	2201      	movs	r2, #1
 8001b5e:	4601      	mov	r1, r0
 8001b60:	a810      	add	r0, sp, #64	; 0x40
 8001b62:	f7ff f807 	bl	8000b74 <strcatFloat>
 8001b66:	490f      	ldr	r1, [pc, #60]	; (8001ba4 <drawStatistics+0x17c>)
 8001b68:	e7e5      	b.n	8001b36 <drawStatistics+0x10e>
 8001b6a:	bf00      	nop
 8001b6c:	080064cb 	stmdaeq	r0, {r0, r1, r3, r6, r7, sl, sp, lr}
 8001b70:	08006bd8 	stmdaeq	r0, {r3, r4, r6, r7, r8, r9, fp, sp, lr}
 8001b74:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}
 8001b78:	080064d6 	stmdaeq	r0, {r1, r2, r4, r6, r7, sl, sp, lr}
 8001b7c:	00080048 	andeq	r0, r8, r8, asr #32
 8001b80:	00080038 	andeq	r0, r8, r8, lsr r0
 8001b84:	00030080 	andeq	r0, r3, r0, lsl #1
 8001b88:	080064e5 	stmdaeq	r0, {r0, r2, r5, r6, r7, sl, sp, lr}
 8001b8c:	080064f6 	stmdaeq	r0, {r1, r2, r4, r5, r6, r7, sl, sp, lr}
 8001b90:	0800737b 	stmdaeq	r0, {r0, r1, r3, r4, r5, r6, r8, r9, ip, sp, lr}
 8001b94:	08006505 	stmdaeq	r0, {r0, r2, r8, sl, sp, lr}
 8001b98:	08006508 	stmdaeq	r0, {r3, r8, sl, sp, lr}
 8001b9c:	0800650c 	stmdaeq	r0, {r2, r3, r8, sl, sp, lr}
 8001ba0:	08006516 	stmdaeq	r0, {r1, r2, r4, r8, sl, sp, lr}
 8001ba4:	08006523 	stmdaeq	r0, {r0, r1, r5, r8, sl, sp, lr}
 8001ba8:	49742400 	ldmdbmi	r4!, {sl, sp}^

08001bac <drawGame>:
 8001bac:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8001bb0:	2500      	movs	r5, #0
 8001bb2:	4604      	mov	r4, r0
 8001bb4:	462f      	mov	r7, r5
 8001bb6:	46a9      	mov	r9, r5
 8001bb8:	b091      	sub	sp, #68	; 0x44
 8001bba:	4852      	ldr	r0, [pc, #328]	; (8001d04 <drawGame+0x158>)
 8001bbc:	4688      	mov	r8, r1
 8001bbe:	4616      	mov	r6, r2
 8001bc0:	f7ff fb66 	bl	8001290 <setFont>
 8001bc4:	4b50      	ldr	r3, [pc, #320]	; (8001d08 <drawGame+0x15c>)
 8001bc6:	9505      	str	r5, [sp, #20]
 8001bc8:	9309      	str	r3, [sp, #36]	; 0x24
 8001bca:	1e63      	subs	r3, r4, #1
 8001bcc:	9301      	str	r3, [sp, #4]
 8001bce:	4b4f      	ldr	r3, [pc, #316]	; (8001d0c <drawGame+0x160>)
 8001bd0:	2400      	movs	r4, #0
 8001bd2:	9a01      	ldr	r2, [sp, #4]
 8001bd4:	fa0f fb85 	sxth.w	fp, r5
 8001bd8:	eb02 0a05 	add.w	sl, r2, r5
 8001bdc:	f81a 1f01 	ldrb.w	r1, [sl, #1]!
 8001be0:	f001 0007 	and.w	r0, r1, #7
 8001be4:	08ca      	lsrs	r2, r1, #3
 8001be6:	06c9      	lsls	r1, r1, #27
 8001be8:	f810 c003 	ldrb.w	ip, [r0, r3]
 8001bec:	d57e      	bpl.n	8001cec <drawGame+0x140>
 8001bee:	43d2      	mvns	r2, r2
 8001bf0:	f04c 0110 	orr.w	r1, ip, #16
 8001bf4:	f002 0001 	and.w	r0, r2, #1
 8001bf8:	2800      	cmp	r0, #0
 8001bfa:	bf0c      	ite	eq
 8001bfc:	2010      	moveq	r0, #16
 8001bfe:	200f      	movne	r0, #15
 8001c00:	e9cd 1302 	strd	r1, r3, [sp, #8]
 8001c04:	f7ff fb1c 	bl	8001240 <setFillColor>
 8001c08:	9902      	ldr	r1, [sp, #8]
 8001c0a:	f8ad 4020 	strh.w	r4, [sp, #32]
 8001c0e:	3409      	adds	r4, #9
 8001c10:	f88d 1038 	strb.w	r1, [sp, #56]	; 0x38
 8001c14:	aa05      	add	r2, sp, #20
 8001c16:	a908      	add	r1, sp, #32
 8001c18:	a80e      	add	r0, sp, #56	; 0x38
 8001c1a:	b2a4      	uxth	r4, r4
 8001c1c:	f88d 9039 	strb.w	r9, [sp, #57]	; 0x39
 8001c20:	f8ad b022 	strh.w	fp, [sp, #34]	; 0x22
 8001c24:	f7ff fb2c 	bl	8001280 <drawText>
 8001c28:	2c48      	cmp	r4, #72	; 0x48
 8001c2a:	9b03      	ldr	r3, [sp, #12]
 8001c2c:	d1d6      	bne.n	8001bdc <drawGame+0x30>
 8001c2e:	3508      	adds	r5, #8
 8001c30:	2d40      	cmp	r5, #64	; 0x40
 8001c32:	f087 0701 	eor.w	r7, r7, #1
 8001c36:	d1cb      	bne.n	8001bd0 <drawGame+0x24>
 8001c38:	4835      	ldr	r0, [pc, #212]	; (8001d10 <drawGame+0x164>)
 8001c3a:	f7ff fb29 	bl	8001290 <setFont>
 8001c3e:	2002      	movs	r0, #2
 8001c40:	f7ff fafe 	bl	8001240 <setFillColor>
 8001c44:	4a33      	ldr	r2, [pc, #204]	; (8001d14 <drawGame+0x168>)
 8001c46:	ab0a      	add	r3, sp, #40	; 0x28
 8001c48:	6810      	ldr	r0, [r2, #0]
 8001c4a:	6851      	ldr	r1, [r2, #4]
 8001c4c:	2708      	movs	r7, #8
 8001c4e:	c303      	stmia	r3!, {r0, r1}
 8001c50:	6890      	ldr	r0, [r2, #8]
 8001c52:	a90a      	add	r1, sp, #40	; 0x28
 8001c54:	9006      	str	r0, [sp, #24]
 8001c56:	aa06      	add	r2, sp, #24
 8001c58:	4640      	mov	r0, r8
 8001c5a:	f7ff fb11 	bl	8001280 <drawText>
 8001c5e:	2314      	movs	r3, #20
 8001c60:	f04f 0a20 	mov.w	sl, #32
 8001c64:	f8ad 9012 	strh.w	r9, [sp, #18]
 8001c68:	f8ad 3022 	strh.w	r3, [sp, #34]	; 0x22
 8001c6c:	f8ad 7026 	strh.w	r7, [sp, #38]	; 0x26
 8001c70:	f106 0924 	add.w	r9, r6, #36	; 0x24
 8001c74:	ad10      	add	r5, sp, #64	; 0x40
 8001c76:	f825 7d30 	strh.w	r7, [r5, #-48]!
 8001c7a:	4630      	mov	r0, r6
 8001c7c:	462a      	mov	r2, r5
 8001c7e:	a908      	add	r1, sp, #32
 8001c80:	f8ad 4020 	strh.w	r4, [sp, #32]
 8001c84:	f8ad a024 	strh.w	sl, [sp, #36]	; 0x24
 8001c88:	f7ff fafa 	bl	8001280 <drawText>
 8001c8c:	f8bd 3020 	ldrh.w	r3, [sp, #32]
 8001c90:	1db0      	adds	r0, r6, #6
 8001c92:	3320      	adds	r3, #32
 8001c94:	f8ad 3020 	strh.w	r3, [sp, #32]
 8001c98:	2318      	movs	r3, #24
 8001c9a:	f8ad 3024 	strh.w	r3, [sp, #36]	; 0x24
 8001c9e:	2300      	movs	r3, #0
 8001ca0:	462a      	mov	r2, r5
 8001ca2:	a908      	add	r1, sp, #32
 8001ca4:	f8ad 3010 	strh.w	r3, [sp, #16]
 8001ca8:	f7ff faea 	bl	8001280 <drawText>
 8001cac:	f8bd 3022 	ldrh.w	r3, [sp, #34]	; 0x22
 8001cb0:	360c      	adds	r6, #12
 8001cb2:	3308      	adds	r3, #8
 8001cb4:	45b1      	cmp	r9, r6
 8001cb6:	f8ad 3022 	strh.w	r3, [sp, #34]	; 0x22
 8001cba:	d1db      	bne.n	8001c74 <drawGame+0xc8>
 8001cbc:	4c16      	ldr	r4, [pc, #88]	; (8001d18 <drawGame+0x16c>)
 8001cbe:	ab0c      	add	r3, sp, #48	; 0x30
 8001cc0:	6820      	ldr	r0, [r4, #0]
 8001cc2:	6861      	ldr	r1, [r4, #4]
 8001cc4:	aa07      	add	r2, sp, #28
 8001cc6:	c303      	stmia	r3!, {r0, r1}
 8001cc8:	68a0      	ldr	r0, [r4, #8]
 8001cca:	a90c      	add	r1, sp, #48	; 0x30
 8001ccc:	9007      	str	r0, [sp, #28]
 8001cce:	f108 0010 	add.w	r0, r8, #16
 8001cd2:	f7ff fad5 	bl	8001280 <drawText>
 8001cd6:	f854 0f0c 	ldr.w	r0, [r4, #12]!
 8001cda:	ab0e      	add	r3, sp, #56	; 0x38
 8001cdc:	6861      	ldr	r1, [r4, #4]
 8001cde:	c303      	stmia	r3!, {r0, r1}
 8001ce0:	a80e      	add	r0, sp, #56	; 0x38
 8001ce2:	f7ff faf5 	bl	80012d0 <drawRectangle>
 8001ce6:	b011      	add	sp, #68	; 0x44
 8001ce8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 8001cec:	19e0      	adds	r0, r4, r7
 8001cee:	f000 0001 	and.w	r0, r0, #1
 8001cf2:	f002 0101 	and.w	r1, r2, #1
 8001cf6:	4041      	eors	r1, r0
 8001cf8:	f081 0101 	eor.w	r1, r1, #1
 8001cfc:	ea4c 01c1 	orr.w	r1, ip, r1, lsl #3
 8001d00:	e77a      	b.n	8001bf8 <drawGame+0x4c>
 8001d02:	bf00      	nop
 8001d04:	08006527 	stmdaeq	r0, {r0, r1, r2, r5, r8, sl, sp, lr}
 8001d08:	00080009 	andeq	r0, r8, r9
 8001d0c:	080064bc 	stmdaeq	r0, {r2, r3, r4, r5, r7, sl, sp, lr}
 8001d10:	08006bd8 	stmdaeq	r0, {r3, r4, r6, r7, r8, r9, fp, sp, lr}
 8001d14:	08006134 	stmdaeq	r0, {r2, r4, r5, r8, sp, lr}
 8001d18:	08006140 	stmdaeq	r0, {r6, r8, sp, lr}

08001d1c <updateTimer>:
 8001d1c:	6803      	ldr	r3, [r0, #0]
 8001d1e:	2b00      	cmp	r3, #0
 8001d20:	d006      	beq.n	8001d30 <updateTimer+0x14>
 8001d22:	dd08      	ble.n	8001d36 <updateTimer+0x1a>
 8001d24:	3b01      	subs	r3, #1
 8001d26:	6003      	str	r3, [r0, #0]
 8001d28:	1c18      	adds	r0, r3, #0
 8001d2a:	bf18      	it	ne
 8001d2c:	2001      	movne	r0, #1
 8001d2e:	4770      	bx	lr
 8001d30:	f04f 30ff 	mov.w	r0, #4294967295
 8001d34:	4770      	bx	lr
 8001d36:	2001      	movs	r0, #1
 8001d38:	4770      	bx	lr
	...

08001d3c <setDisplayTimer>:
 8001d3c:	2801      	cmp	r0, #1
 8001d3e:	b538      	push	{r3, r4, r5, lr}
 8001d40:	4604      	mov	r4, r0
 8001d42:	4d08      	ldr	r5, [pc, #32]	; (8001d64 <setDisplayTimer+0x28>)
 8001d44:	d103      	bne.n	8001d4e <setDisplayTimer+0x12>
 8001d46:	68eb      	ldr	r3, [r5, #12]
 8001d48:	42a3      	cmp	r3, r4
 8001d4a:	db07      	blt.n	8001d5c <setDisplayTimer+0x20>
 8001d4c:	bd38      	pop	{r3, r4, r5, pc}
 8001d4e:	2001      	movs	r0, #1
 8001d50:	f002 fcce 	bl	80046f0 <setDisplayBacklight>
 8001d54:	2c00      	cmp	r4, #0
 8001d56:	daf6      	bge.n	8001d46 <setDisplayTimer+0xa>
 8001d58:	60ec      	str	r4, [r5, #12]
 8001d5a:	e7f7      	b.n	8001d4c <setDisplayTimer+0x10>
 8001d5c:	68eb      	ldr	r3, [r5, #12]
 8001d5e:	3301      	adds	r3, #1
 8001d60:	d1fa      	bne.n	8001d58 <setDisplayTimer+0x1c>
 8001d62:	e7f3      	b.n	8001d4c <setDisplayTimer+0x10>
 8001d64:	200000ec 	andcs	r0, r0, ip, ror #1

08001d68 <initEvents>:
 8001d68:	b508      	push	{r3, lr}
 8001d6a:	f002 fcd3 	bl	8004714 <initEventsController>
 8001d6e:	f04f 32ff 	mov.w	r2, #4294967295
 8001d72:	4b04      	ldr	r3, [pc, #16]	; (8001d84 <initEvents+0x1c>)
 8001d74:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
 8001d78:	609a      	str	r2, [r3, #8]
 8001d7a:	2219      	movs	r2, #25
 8001d7c:	e9c3 2007 	strd	r2, r0, [r3, #28]
 8001d80:	bd08      	pop	{r3, pc}
 8001d82:	bf00      	nop
 8001d84:	200000ec 	andcs	r0, r0, ip, ror #1

08001d88 <onTick>:
 8001d88:	b573      	push	{r0, r1, r4, r5, r6, lr}
 8001d8a:	4c2c      	ldr	r4, [pc, #176]	; (8001e3c <onTick+0xb4>)
 8001d8c:	f894 302c 	ldrb.w	r3, [r4, #44]	; 0x2c
 8001d90:	2b00      	cmp	r3, #0
 8001d92:	d143      	bne.n	8001e1c <onTick+0x94>
 8001d94:	482a      	ldr	r0, [pc, #168]	; (8001e40 <onTick+0xb8>)
 8001d96:	f7ff ffc1 	bl	8001d1c <updateTimer>
 8001d9a:	b918      	cbnz	r0, 8001da4 <onTick+0x1c>
 8001d9c:	2319      	movs	r3, #25
 8001d9e:	61e3      	str	r3, [r4, #28]
 8001da0:	f000 fc48 	bl	8002634 <onKeyboardTick>
 8001da4:	4827      	ldr	r0, [pc, #156]	; (8001e44 <onTick+0xbc>)
 8001da6:	f7ff ffb9 	bl	8001d1c <updateTimer>
 8001daa:	4602      	mov	r2, r0
 8001dac:	2800      	cmp	r0, #0
 8001dae:	d042      	beq.n	8001e36 <onTick+0xae>
 8001db0:	2801      	cmp	r0, #1
 8001db2:	d036      	beq.n	8001e22 <onTick+0x9a>
 8001db4:	4824      	ldr	r0, [pc, #144]	; (8001e48 <onTick+0xc0>)
 8001db6:	f7ff ffb1 	bl	8001d1c <updateTimer>
 8001dba:	b908      	cbnz	r0, 8001dc0 <onTick+0x38>
 8001dbc:	f002 fc98 	bl	80046f0 <setDisplayBacklight>
 8001dc0:	4822      	ldr	r0, [pc, #136]	; (8001e4c <onTick+0xc4>)
 8001dc2:	f7ff ffab 	bl	8001d1c <updateTimer>
 8001dc6:	b908      	cbnz	r0, 8001dcc <onTick+0x44>
 8001dc8:	f002 ffcc 	bl	8004d64 <setVibrator>
 8001dcc:	b002      	add	sp, #8
 8001dce:	bd70      	pop	{r4, r5, r6, pc}
 8001dd0:	9801      	ldr	r0, [sp, #4]
 8001dd2:	f001 fddd 	bl	8003990 <onRNGPulse>
 8001dd6:	7823      	ldrb	r3, [r4, #0]
 8001dd8:	9a01      	ldr	r2, [sp, #4]
 8001dda:	b1eb      	cbz	r3, 8001e18 <onTick+0x90>
 8001ddc:	6863      	ldr	r3, [r4, #4]
 8001dde:	68a1      	ldr	r1, [r4, #8]
 8001de0:	1ad3      	subs	r3, r2, r3
 8001de2:	4299      	cmp	r1, r3
 8001de4:	d900      	bls.n	8001de8 <onTick+0x60>
 8001de6:	60a3      	str	r3, [r4, #8]
 8001de8:	6062      	str	r2, [r4, #4]
 8001dea:	3501      	adds	r5, #1
 8001dec:	a801      	add	r0, sp, #4
 8001dee:	f002 ff89 	bl	8004d04 <getTubePulse>
 8001df2:	2800      	cmp	r0, #0
 8001df4:	d1ec      	bne.n	8001dd0 <onTick+0x48>
 8001df6:	4628      	mov	r0, r5
 8001df8:	f000 ff44 	bl	8002c84 <onMeasurementTick>
 8001dfc:	4814      	ldr	r0, [pc, #80]	; (8001e50 <onTick+0xc8>)
 8001dfe:	f7ff ff8d 	bl	8001d1c <updateTimer>
 8001e02:	2800      	cmp	r0, #0
 8001e04:	d1c6      	bne.n	8001d94 <onTick+0xc>
 8001e06:	f44f 737a 	mov.w	r3, #1000	; 0x3e8
 8001e0a:	6223      	str	r3, [r4, #32]
 8001e0c:	6a63      	ldr	r3, [r4, #36]	; 0x24
 8001e0e:	3301      	adds	r3, #1
 8001e10:	6263      	str	r3, [r4, #36]	; 0x24
 8001e12:	f000 ff4d 	bl	8002cb0 <onMeasurementPeriod>
 8001e16:	e7bd      	b.n	8001d94 <onTick+0xc>
 8001e18:	7026      	strb	r6, [r4, #0]
 8001e1a:	e7e5      	b.n	8001de8 <onTick+0x60>
 8001e1c:	2500      	movs	r5, #0
 8001e1e:	2601      	movs	r6, #1
 8001e20:	e7e4      	b.n	8001dec <onTick+0x64>
 8001e22:	480c      	ldr	r0, [pc, #48]	; (8001e54 <onTick+0xcc>)
 8001e24:	f7ff ff7a 	bl	8001d1c <updateTimer>
 8001e28:	b120      	cbz	r0, 8001e34 <onTick+0xac>
 8001e2a:	2801      	cmp	r0, #1
 8001e2c:	d1c2      	bne.n	8001db4 <onTick+0x2c>
 8001e2e:	f7fe fc6b 	bl	8000708 <getRandomBit>
 8001e32:	e000      	b.n	8001e36 <onTick+0xae>
 8001e34:	4610      	mov	r0, r2
 8001e36:	f002 fb1d 	bl	8004474 <setBuzzer>
 8001e3a:	e7bb      	b.n	8001db4 <onTick+0x2c>
 8001e3c:	200000ec 	andcs	r0, r0, ip, ror #1
 8001e40:	20000108 	andcs	r0, r0, r8, lsl #2
 8001e44:	200000fc 	strdcs	r0, [r0], -ip
 8001e48:	200000f8 	strdcs	r0, [r0], -r8
 8001e4c:	20000104 	andcs	r0, r0, r4, lsl #2
 8001e50:	2000010c 	andcs	r0, r0, ip, lsl #2
 8001e54:	20000100 	andcs	r0, r0, r0, lsl #2

08001e58 <getTick>:
 8001e58:	4b01      	ldr	r3, [pc, #4]	; (8001e60 <getTick+0x8>)
 8001e5a:	6818      	ldr	r0, [r3, #0]
 8001e5c:	4770      	bx	lr
 8001e5e:	bf00      	nop
 8001e60:	20000d78 	andcs	r0, r0, r8, ror sp

08001e64 <syncTimerThread>:
 8001e64:	2001      	movs	r0, #1
 8001e66:	f002 bc87 	b.w	8004778 <sleep>
	...

08001e6c <enableMeasurements>:
 8001e6c:	2201      	movs	r2, #1
 8001e6e:	4b02      	ldr	r3, [pc, #8]	; (8001e78 <enableMeasurements+0xc>)
 8001e70:	f883 202c 	strb.w	r2, [r3, #44]	; 0x2c
 8001e74:	4770      	bx	lr
 8001e76:	bf00      	nop
 8001e78:	200000ec 	andcs	r0, r0, ip, ror #1

08001e7c <disableMeasurements>:
 8001e7c:	b508      	push	{r3, lr}
 8001e7e:	f7ff fff1 	bl	8001e64 <syncTimerThread>
 8001e82:	2201      	movs	r2, #1
 8001e84:	4b03      	ldr	r3, [pc, #12]	; (8001e94 <disableMeasurements+0x18>)
 8001e86:	60da      	str	r2, [r3, #12]
 8001e88:	611a      	str	r2, [r3, #16]
 8001e8a:	619a      	str	r2, [r3, #24]
 8001e8c:	2200      	movs	r2, #0
 8001e8e:	f883 202c 	strb.w	r2, [r3, #44]	; 0x2c
 8001e92:	bd08      	pop	{r3, pc}
 8001e94:	200000ec 	andcs	r0, r0, ip, ror #1

08001e98 <dispatchEvents>:
 8001e98:	b508      	push	{r3, lr}
 8001e9a:	2000      	movs	r0, #0
 8001e9c:	f002 fc6c 	bl	8004778 <sleep>
 8001ea0:	4b0a      	ldr	r3, [pc, #40]	; (8001ecc <dispatchEvents+0x34>)
 8001ea2:	6a5a      	ldr	r2, [r3, #36]	; 0x24
 8001ea4:	6a99      	ldr	r1, [r3, #40]	; 0x28
 8001ea6:	4291      	cmp	r1, r2
 8001ea8:	d00a      	beq.n	8001ec0 <dispatchEvents+0x28>
 8001eaa:	629a      	str	r2, [r3, #40]	; 0x28
 8001eac:	f000 ff10 	bl	8002cd0 <updateMeasurements>
 8001eb0:	f7ff f90a 	bl	80010c8 <updateDatalog>
 8001eb4:	f002 fa1e 	bl	80042f4 <updateADC>
 8001eb8:	f000 fb98 	bl	80025ec <updateGame>
 8001ebc:	f003 f9ec 	bl	8005298 <updateView>
 8001ec0:	f003 f9aa 	bl	8005218 <dispatchViewEvents>
 8001ec4:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8001ec8:	f7fe bca6 	b.w	8000818 <dispatchCommEvents>
 8001ecc:	200000ec 	andcs	r0, r0, ip, ror #1

08001ed0 <getTubeDeadTime>:
 8001ed0:	b508      	push	{r3, lr}
 8001ed2:	4b04      	ldr	r3, [pc, #16]	; (8001ee4 <getTubeDeadTime+0x14>)
 8001ed4:	6898      	ldr	r0, [r3, #8]
 8001ed6:	f002 fae1 	bl	800449c <__aeabi_ui2f>
 8001eda:	4b03      	ldr	r3, [pc, #12]	; (8001ee8 <getTubeDeadTime+0x18>)
 8001edc:	6819      	ldr	r1, [r3, #0]
 8001ede:	f002 fad9 	bl	8004494 <__aeabi_fmul>
 8001ee2:	bd08      	pop	{r3, pc}
 8001ee4:	200000ec 	andcs	r0, r0, ip, ror #1
 8001ee8:	2000006c 	andcs	r0, r0, ip, rrx

08001eec <triggerDisplay>:
 8001eec:	b508      	push	{r3, lr}
 8001eee:	f7ff ffb9 	bl	8001e64 <syncTimerThread>
 8001ef2:	2200      	movs	r2, #0
 8001ef4:	4b06      	ldr	r3, [pc, #24]	; (8001f10 <triggerDisplay+0x24>)
 8001ef6:	60da      	str	r2, [r3, #12]
 8001ef8:	4b06      	ldr	r3, [pc, #24]	; (8001f14 <triggerDisplay+0x28>)
 8001efa:	4a07      	ldr	r2, [pc, #28]	; (8001f18 <triggerDisplay+0x2c>)
 8001efc:	88db      	ldrh	r3, [r3, #6]
 8001efe:	f3c3 1382 	ubfx	r3, r3, #6, #3
 8001f02:	f852 0023 	ldr.w	r0, [r2, r3, lsl #2]
 8001f06:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8001f0a:	f7ff bf17 	b.w	8001d3c <setDisplayTimer>
 8001f0e:	bf00      	nop
 8001f10:	200000ec 	andcs	r0, r0, ip, ror #1
 8001f14:	20000da0 	andcs	r0, r0, r0, lsr #27
 8001f18:	08006f10 	stmdaeq	r0, {r4, r8, r9, sl, fp, sp, lr}

08001f1c <isDisplayTimerActive>:
 8001f1c:	4b02      	ldr	r3, [pc, #8]	; (8001f28 <isDisplayTimerActive+0xc>)
 8001f1e:	68d8      	ldr	r0, [r3, #12]
 8001f20:	3000      	adds	r0, #0
 8001f22:	bf18      	it	ne
 8001f24:	2001      	movne	r0, #1
 8001f26:	4770      	bx	lr
 8001f28:	200000ec 	andcs	r0, r0, ip, ror #1

08001f2c <triggerPulse>:
 8001f2c:	b538      	push	{r3, r4, r5, lr}
 8001f2e:	4c0e      	ldr	r4, [pc, #56]	; (8001f68 <triggerPulse+0x3c>)
 8001f30:	69a3      	ldr	r3, [r4, #24]
 8001f32:	b9bb      	cbnz	r3, 8001f64 <triggerPulse+0x38>
 8001f34:	4d0d      	ldr	r5, [pc, #52]	; (8001f6c <triggerPulse+0x40>)
 8001f36:	88ea      	ldrh	r2, [r5, #6]
 8001f38:	f402 72e0 	and.w	r2, r2, #448	; 0x1c0
 8001f3c:	f5b2 7fa0 	cmp.w	r2, #320	; 0x140
 8001f40:	d102      	bne.n	8001f48 <triggerPulse+0x1c>
 8001f42:	2032      	movs	r0, #50	; 0x32
 8001f44:	f7ff fefa 	bl	8001d3c <setDisplayTimer>
 8001f48:	686b      	ldr	r3, [r5, #4]
 8001f4a:	4a09      	ldr	r2, [pc, #36]	; (8001f70 <triggerPulse+0x44>)
 8001f4c:	f3c3 33c1 	ubfx	r3, r3, #15, #2
 8001f50:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
 8001f54:	6922      	ldr	r2, [r4, #16]
 8001f56:	6921      	ldr	r1, [r4, #16]
 8001f58:	3301      	adds	r3, #1
 8001f5a:	428b      	cmp	r3, r1
 8001f5c:	bfc2      	ittt	gt
 8001f5e:	3201      	addgt	r2, #1
 8001f60:	6123      	strgt	r3, [r4, #16]
 8001f62:	6162      	strgt	r2, [r4, #20]
 8001f64:	bd38      	pop	{r3, r4, r5, pc}
 8001f66:	bf00      	nop
 8001f68:	200000ec 	andcs	r0, r0, ip, ror #1
 8001f6c:	20000da0 	andcs	r0, r0, r0, lsr #27
 8001f70:	08006f04 	stmdaeq	r0, {r2, r8, r9, sl, fp, sp, lr}

08001f74 <triggerAlarm>:
 8001f74:	b510      	push	{r4, lr}
 8001f76:	4c09      	ldr	r4, [pc, #36]	; (8001f9c <triggerAlarm+0x28>)
 8001f78:	f7ff ff74 	bl	8001e64 <syncTimerThread>
 8001f7c:	6923      	ldr	r3, [r4, #16]
 8001f7e:	2064      	movs	r0, #100	; 0x64
 8001f80:	2b63      	cmp	r3, #99	; 0x63
 8001f82:	bfdf      	itttt	le
 8001f84:	2364      	movle	r3, #100	; 0x64
 8001f86:	6123      	strle	r3, [r4, #16]
 8001f88:	2301      	movle	r3, #1
 8001f8a:	6163      	strle	r3, [r4, #20]
 8001f8c:	f7ff fed6 	bl	8001d3c <setDisplayTimer>
 8001f90:	2001      	movs	r0, #1
 8001f92:	f002 fee7 	bl	8004d64 <setVibrator>
 8001f96:	2364      	movs	r3, #100	; 0x64
 8001f98:	61a3      	str	r3, [r4, #24]
 8001f9a:	bd10      	pop	{r4, pc}
 8001f9c:	200000ec 	andcs	r0, r0, ip, ror #1

08001fa0 <isFlashPageFull>:
 8001fa0:	b537      	push	{r0, r1, r2, r4, r5, lr}
 8001fa2:	4b09      	ldr	r3, [pc, #36]	; (8001fc8 <isFlashPageFull+0x28>)
 8001fa4:	6885      	ldr	r5, [r0, #8]
 8001fa6:	681b      	ldr	r3, [r3, #0]
 8001fa8:	2201      	movs	r2, #1
 8001faa:	6083      	str	r3, [r0, #8]
 8001fac:	f10d 0107 	add.w	r1, sp, #7
 8001fb0:	4604      	mov	r4, r0
 8001fb2:	f002 fc23 	bl	80047fc <readFlash>
 8001fb6:	f89d 0007 	ldrb.w	r0, [sp, #7]
 8001fba:	60a5      	str	r5, [r4, #8]
 8001fbc:	38ff      	subs	r0, #255	; 0xff
 8001fbe:	bf18      	it	ne
 8001fc0:	2001      	movne	r0, #1
 8001fc2:	b003      	add	sp, #12
 8001fc4:	bd30      	pop	{r4, r5, pc}
 8001fc6:	bf00      	nop
 8001fc8:	08007598 	stmdaeq	r0, {r3, r4, r7, r8, sl, ip, sp, lr}

08001fcc <setFlashPageHead>:
 8001fcc:	2300      	movs	r3, #0
 8001fce:	b510      	push	{r4, lr}
 8001fd0:	4604      	mov	r4, r0
 8001fd2:	6083      	str	r3, [r0, #8]
 8001fd4:	6803      	ldr	r3, [r0, #0]
 8001fd6:	781b      	ldrb	r3, [r3, #0]
 8001fd8:	7123      	strb	r3, [r4, #4]
 8001fda:	6823      	ldr	r3, [r4, #0]
 8001fdc:	7921      	ldrb	r1, [r4, #4]
 8001fde:	785a      	ldrb	r2, [r3, #1]
 8001fe0:	4291      	cmp	r1, r2
 8001fe2:	d302      	bcc.n	8001fea <setFlashPageHead+0x1e>
 8001fe4:	781b      	ldrb	r3, [r3, #0]
 8001fe6:	7123      	strb	r3, [r4, #4]
 8001fe8:	bd10      	pop	{r4, pc}
 8001fea:	4620      	mov	r0, r4
 8001fec:	f7ff ffd8 	bl	8001fa0 <isFlashPageFull>
 8001ff0:	2800      	cmp	r0, #0
 8001ff2:	d0f9      	beq.n	8001fe8 <setFlashPageHead+0x1c>
 8001ff4:	7923      	ldrb	r3, [r4, #4]
 8001ff6:	3301      	adds	r3, #1
 8001ff8:	e7ee      	b.n	8001fd8 <setFlashPageHead+0xc>

08001ffa <setFlashPageNext>:
 8001ffa:	2100      	movs	r1, #0
 8001ffc:	7903      	ldrb	r3, [r0, #4]
 8001ffe:	6802      	ldr	r2, [r0, #0]
 8002000:	3301      	adds	r3, #1
 8002002:	b2db      	uxtb	r3, r3
 8002004:	7103      	strb	r3, [r0, #4]
 8002006:	6081      	str	r1, [r0, #8]
 8002008:	7851      	ldrb	r1, [r2, #1]
 800200a:	4299      	cmp	r1, r3
 800200c:	bf9c      	itt	ls
 800200e:	7813      	ldrbls	r3, [r2, #0]
 8002010:	7103      	strbls	r3, [r0, #4]
 8002012:	4770      	bx	lr

08002014 <setFlashPagePrev>:
 8002014:	2100      	movs	r1, #0
 8002016:	7903      	ldrb	r3, [r0, #4]
 8002018:	6802      	ldr	r2, [r0, #0]
 800201a:	3b01      	subs	r3, #1
 800201c:	b2db      	uxtb	r3, r3
 800201e:	7103      	strb	r3, [r0, #4]
 8002020:	6081      	str	r1, [r0, #8]
 8002022:	7811      	ldrb	r1, [r2, #0]
 8002024:	4299      	cmp	r1, r3
 8002026:	bf82      	ittt	hi
 8002028:	7853      	ldrbhi	r3, [r2, #1]
 800202a:	f103 33ff 	addhi.w	r3, r3, #4294967295
 800202e:	7103      	strbhi	r3, [r0, #4]
 8002030:	4770      	bx	lr

08002032 <setFlashPageTail>:
 8002032:	b570      	push	{r4, r5, r6, lr}
 8002034:	4604      	mov	r4, r0
 8002036:	f7ff ffc9 	bl	8001fcc <setFlashPageHead>
 800203a:	7926      	ldrb	r6, [r4, #4]
 800203c:	4635      	mov	r5, r6
 800203e:	4620      	mov	r0, r4
 8002040:	f7ff ffe8 	bl	8002014 <setFlashPagePrev>
 8002044:	7923      	ldrb	r3, [r4, #4]
 8002046:	42b3      	cmp	r3, r6
 8002048:	d004      	beq.n	8002054 <setFlashPageTail+0x22>
 800204a:	f7ff ffa9 	bl	8001fa0 <isFlashPageFull>
 800204e:	b108      	cbz	r0, 8002054 <setFlashPageTail+0x22>
 8002050:	7925      	ldrb	r5, [r4, #4]
 8002052:	e7f4      	b.n	800203e <setFlashPageTail+0xc>
 8002054:	7125      	strb	r5, [r4, #4]
 8002056:	bd70      	pop	{r4, r5, r6, pc}

08002058 <writeFlashPage>:
 8002058:	4b18      	ldr	r3, [pc, #96]	; (80020bc <writeFlashPage+0x64>)
 800205a:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 800205e:	681f      	ldr	r7, [r3, #0]
 8002060:	6883      	ldr	r3, [r0, #8]
 8002062:	b086      	sub	sp, #24
 8002064:	4413      	add	r3, r2
 8002066:	42bb      	cmp	r3, r7
 8002068:	4604      	mov	r4, r0
 800206a:	4688      	mov	r8, r1
 800206c:	4616      	mov	r6, r2
 800206e:	d91c      	bls.n	80020aa <writeFlashPage+0x52>
 8002070:	c807      	ldmia	r0, {r0, r1, r2}
 8002072:	ad03      	add	r5, sp, #12
 8002074:	e885 0007 	stmia.w	r5, {r0, r1, r2}
 8002078:	4620      	mov	r0, r4
 800207a:	f7ff ffbe 	bl	8001ffa <setFlashPageNext>
 800207e:	7922      	ldrb	r2, [r4, #4]
 8002080:	f89d 3010 	ldrb.w	r3, [sp, #16]
 8002084:	429a      	cmp	r2, r3
 8002086:	d00d      	beq.n	80020a4 <writeFlashPage+0x4c>
 8002088:	2300      	movs	r3, #0
 800208a:	e9cd 3301 	strd	r3, r3, [sp, #4]
 800208e:	4b0c      	ldr	r3, [pc, #48]	; (80020c0 <writeFlashPage+0x68>)
 8002090:	4628      	mov	r0, r5
 8002092:	681a      	ldr	r2, [r3, #0]
 8002094:	a901      	add	r1, sp, #4
 8002096:	9705      	str	r7, [sp, #20]
 8002098:	f002 fbe6 	bl	8004868 <writeFlash>
 800209c:	4620      	mov	r0, r4
 800209e:	f7ff ff7f 	bl	8001fa0 <isFlashPageFull>
 80020a2:	b110      	cbz	r0, 80020aa <writeFlashPage+0x52>
 80020a4:	4620      	mov	r0, r4
 80020a6:	f002 fbb9 	bl	800481c <eraseFlash>
 80020aa:	4632      	mov	r2, r6
 80020ac:	4641      	mov	r1, r8
 80020ae:	4620      	mov	r0, r4
 80020b0:	f002 fbda 	bl	8004868 <writeFlash>
 80020b4:	b006      	add	sp, #24
 80020b6:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 80020ba:	bf00      	nop
 80020bc:	08007598 	stmdaeq	r0, {r3, r4, r7, r8, sl, ip, sp, lr}
 80020c0:	08007594 	stmdaeq	r0, {r2, r4, r7, r8, sl, ip, sp, lr}

080020c4 <recordGameMove>:
 80020c4:	4b16      	ldr	r3, [pc, #88]	; (8002120 <recordGameMove+0x5c>)
 80020c6:	b082      	sub	sp, #8
 80020c8:	685a      	ldr	r2, [r3, #4]
 80020ca:	f8ad 0004 	strh.w	r0, [sp, #4]
 80020ce:	2a05      	cmp	r2, #5
 80020d0:	d807      	bhi.n	80020e2 <recordGameMove+0x1e>
 80020d2:	eb03 0142 	add.w	r1, r3, r2, lsl #1
 80020d6:	f8a1 01c6 	strh.w	r0, [r1, #454]	; 0x1c6
 80020da:	3201      	adds	r2, #1
 80020dc:	605a      	str	r2, [r3, #4]
 80020de:	b002      	add	sp, #8
 80020e0:	4770      	bx	lr
 80020e2:	f012 0101 	ands.w	r1, r2, #1
 80020e6:	d113      	bne.n	8002110 <recordGameMove+0x4c>
 80020e8:	f8b3 01ca 	ldrh.w	r0, [r3, #458]	; 0x1ca
 80020ec:	f8a3 01c6 	strh.w	r0, [r3, #454]	; 0x1c6
 80020f0:	f8b3 01cc 	ldrh.w	r0, [r3, #460]	; 0x1cc
 80020f4:	f8a3 01c8 	strh.w	r0, [r3, #456]	; 0x1c8
 80020f8:	f8b3 01ce 	ldrh.w	r0, [r3, #462]	; 0x1ce
 80020fc:	f8a3 01ca 	strh.w	r0, [r3, #458]	; 0x1ca
 8002100:	f8b3 01d0 	ldrh.w	r0, [r3, #464]	; 0x1d0
 8002104:	f8a3 01cc 	strh.w	r0, [r3, #460]	; 0x1cc
 8002108:	f248 0080 	movw	r0, #32896	; 0x8080
 800210c:	f8a3 01d0 	strh.w	r0, [r3, #464]	; 0x1d0
 8002110:	31e4      	adds	r1, #228	; 0xe4
 8002112:	f8bd 0004 	ldrh.w	r0, [sp, #4]
 8002116:	eb03 0141 	add.w	r1, r3, r1, lsl #1
 800211a:	80c8      	strh	r0, [r1, #6]
 800211c:	e7dd      	b.n	80020da <recordGameMove+0x16>
 800211e:	bf00      	nop
 8002120:	2000011c 	andcs	r0, r0, ip, lsl r1

08002124 <onGameMenuGetOption>:
 8002124:	2302      	movs	r3, #2
 8002126:	6013      	str	r3, [r2, #0]
 8002128:	4b04      	ldr	r3, [pc, #16]	; (800213c <onGameMenuGetOption+0x18>)
 800212a:	685b      	ldr	r3, [r3, #4]
 800212c:	b91b      	cbnz	r3, 8002136 <onGameMenuGetOption+0x12>
 800212e:	4b04      	ldr	r3, [pc, #16]	; (8002140 <onGameMenuGetOption+0x1c>)
 8002130:	f853 0021 	ldr.w	r0, [r3, r1, lsl #2]
 8002134:	4770      	bx	lr
 8002136:	4b03      	ldr	r3, [pc, #12]	; (8002144 <onGameMenuGetOption+0x20>)
 8002138:	e7fa      	b.n	8002130 <onGameMenuGetOption+0xc>
 800213a:	bf00      	nop
 800213c:	2000011c 	andcs	r0, r0, ip, lsl r1
 8002140:	08006f58 	stmdaeq	r0, {r3, r4, r6, r8, r9, sl, fp, sp, lr}
 8002144:	08006f2c 	stmdaeq	r0, {r2, r3, r5, r8, r9, sl, fp, sp, lr}

08002148 <onGameStrengthMenuSelect>:
 8002148:	4b03      	ldr	r3, [pc, #12]	; (8002158 <onGameStrengthMenuSelect+0x10>)
 800214a:	6841      	ldr	r1, [r0, #4]
 800214c:	7a1a      	ldrb	r2, [r3, #8]
 800214e:	8849      	ldrh	r1, [r1, #2]
 8002150:	f361 0202 	bfi	r2, r1, #0, #3
 8002154:	721a      	strb	r2, [r3, #8]
 8002156:	4770      	bx	lr
 8002158:	20000da0 	andcs	r0, r0, r0, lsr #27

0800215c <onGameStrengthMenuBack>:
 800215c:	4801      	ldr	r0, [pc, #4]	; (8002164 <onGameStrengthMenuBack+0x8>)
 800215e:	f003 b893 	b.w	8005288 <setView>
 8002162:	bf00      	nop
 8002164:	08006f50 	stmdaeq	r0, {r4, r6, r8, r9, sl, fp, sp, lr}

08002168 <onGameStrengthMenuGetOption>:
 8002168:	b510      	push	{r4, lr}
 800216a:	460c      	mov	r4, r1
 800216c:	4b0b      	ldr	r3, [pc, #44]	; (800219c <onGameStrengthMenuGetOption+0x34>)
 800216e:	7a1b      	ldrb	r3, [r3, #8]
 8002170:	f003 0307 	and.w	r3, r3, #7
 8002174:	1a59      	subs	r1, r3, r1
 8002176:	424b      	negs	r3, r1
 8002178:	414b      	adcs	r3, r1
 800217a:	2c07      	cmp	r4, #7
 800217c:	6013      	str	r3, [r2, #0]
 800217e:	d80a      	bhi.n	8002196 <onGameStrengthMenuGetOption+0x2e>
 8002180:	4907      	ldr	r1, [pc, #28]	; (80021a0 <onGameStrengthMenuGetOption+0x38>)
 8002182:	4808      	ldr	r0, [pc, #32]	; (80021a4 <onGameStrengthMenuGetOption+0x3c>)
 8002184:	f003 ff6a 	bl	800605c <strcpy>
 8002188:	4806      	ldr	r0, [pc, #24]	; (80021a4 <onGameStrengthMenuGetOption+0x3c>)
 800218a:	2200      	movs	r2, #0
 800218c:	1c61      	adds	r1, r4, #1
 800218e:	f7fe fc8d 	bl	8000aac <strcatUInt32>
 8002192:	4804      	ldr	r0, [pc, #16]	; (80021a4 <onGameStrengthMenuGetOption+0x3c>)
 8002194:	bd10      	pop	{r4, pc}
 8002196:	2000      	movs	r0, #0
 8002198:	e7fc      	b.n	8002194 <onGameStrengthMenuGetOption+0x2c>
 800219a:	bf00      	nop
 800219c:	20000da0 	andcs	r0, r0, r0, lsr #27
 80021a0:	08006e58 	stmdaeq	r0, {r3, r4, r6, r9, sl, fp, sp, lr}
 80021a4:	20000d7c 	andcs	r0, r0, ip, ror sp

080021a8 <onGameCallback>:
 80021a8:	f7ff be76 	b.w	8001e98 <dispatchEvents>

080021ac <updateValidMoves>:
 80021ac:	b510      	push	{r4, lr}
 80021ae:	4c06      	ldr	r4, [pc, #24]	; (80021c8 <updateValidMoves+0x1c>)
 80021b0:	21b5      	movs	r1, #181	; 0xb5
 80021b2:	f104 005c 	add.w	r0, r4, #92	; 0x5c
 80021b6:	f003 feaf 	bl	8005f18 <mcumax_search_valid_moves>
 80021ba:	68a3      	ldr	r3, [r4, #8]
 80021bc:	6560      	str	r0, [r4, #84]	; 0x54
 80021be:	b103      	cbz	r3, 80021c2 <updateValidMoves+0x16>
 80021c0:	1e43      	subs	r3, r0, #1
 80021c2:	65a3      	str	r3, [r4, #88]	; 0x58
 80021c4:	bd10      	pop	{r4, pc}
 80021c6:	bf00      	nop
 80021c8:	2000011c 	andcs	r0, r0, ip, lsl r1

080021cc <updateGameBoard>:
 80021cc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 80021d0:	4c33      	ldr	r4, [pc, #204]	; (80022a0 <updateGameBoard+0xd4>)
 80021d2:	b085      	sub	sp, #20
 80021d4:	7823      	ldrb	r3, [r4, #0]
 80021d6:	2b01      	cmp	r3, #1
 80021d8:	d050      	beq.n	800227c <updateGameBoard+0xb0>
 80021da:	d304      	bcc.n	80021e6 <updateGameBoard+0x1a>
 80021dc:	2b02      	cmp	r3, #2
 80021de:	d054      	beq.n	800228a <updateGameBoard+0xbe>
 80021e0:	2580      	movs	r5, #128	; 0x80
 80021e2:	46a8      	mov	r8, r5
 80021e4:	e00d      	b.n	8002202 <updateGameBoard+0x36>
 80021e6:	2580      	movs	r5, #128	; 0x80
 80021e8:	2300      	movs	r3, #0
 80021ea:	46a8      	mov	r8, r5
 80021ec:	18e1      	adds	r1, r4, r3
 80021ee:	f891 21c6 	ldrb.w	r2, [r1, #454]	; 0x1c6
 80021f2:	3302      	adds	r3, #2
 80021f4:	2a80      	cmp	r2, #128	; 0x80
 80021f6:	bf1c      	itt	ne
 80021f8:	f891 51c7 	ldrbne.w	r5, [r1, #455]	; 0x1c7
 80021fc:	4690      	movne	r8, r2
 80021fe:	2b0c      	cmp	r3, #12
 8002200:	d1f4      	bne.n	80021ec <updateGameBoard+0x20>
 8002202:	2307      	movs	r3, #7
 8002204:	f04f 0b00 	mov.w	fp, #0
 8002208:	f04f 0a00 	mov.w	sl, #0
 800220c:	fa5f f18b 	uxtb.w	r1, fp
 8002210:	010a      	lsls	r2, r1, #4
 8002212:	b2d8      	uxtb	r0, r3
 8002214:	b2d2      	uxtb	r2, r2
 8002216:	9000      	str	r0, [sp, #0]
 8002218:	fa5f f78a 	uxtb.w	r7, sl
 800221c:	68a0      	ldr	r0, [r4, #8]
 800221e:	eb07 0902 	add.w	r9, r7, r2
 8002222:	fa5f f989 	uxtb.w	r9, r9
 8002226:	2800      	cmp	r0, #0
 8002228:	d037      	beq.n	800229a <updateGameBoard+0xce>
 800222a:	f1c7 0707 	rsb	r7, r7, #7
 800222e:	9e00      	ldr	r6, [sp, #0]
 8002230:	b2ff      	uxtb	r7, r7
 8002232:	4648      	mov	r0, r9
 8002234:	e9cd 3202 	strd	r3, r2, [sp, #8]
 8002238:	9101      	str	r1, [sp, #4]
 800223a:	f003 fe5f 	bl	8005efc <mcumax_get_piece>
 800223e:	e9dd 1301 	ldrd	r1, r3, [sp, #4]
 8002242:	45c8      	cmp	r8, r9
 8002244:	9a03      	ldr	r2, [sp, #12]
 8002246:	d001      	beq.n	800224c <updateGameBoard+0x80>
 8002248:	454d      	cmp	r5, r9
 800224a:	d102      	bne.n	8002252 <updateGameBoard+0x86>
 800224c:	f040 0010 	orr.w	r0, r0, #16
 8002250:	b2c0      	uxtb	r0, r0
 8002252:	eb04 06c6 	add.w	r6, r4, r6, lsl #3
 8002256:	f10a 0a01 	add.w	sl, sl, #1
 800225a:	4437      	add	r7, r6
 800225c:	f1ba 0f08 	cmp.w	sl, #8
 8002260:	7538      	strb	r0, [r7, #20]
 8002262:	d1d9      	bne.n	8002218 <updateGameBoard+0x4c>
 8002264:	f10b 0b01 	add.w	fp, fp, #1
 8002268:	f1bb 0f08 	cmp.w	fp, #8
 800226c:	f103 33ff 	add.w	r3, r3, #4294967295
 8002270:	d1ca      	bne.n	8002208 <updateGameBoard+0x3c>
 8002272:	b005      	add	sp, #20
 8002274:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8002278:	f003 b80e 	b.w	8005298 <updateView>
 800227c:	6da3      	ldr	r3, [r4, #88]	; 0x58
 800227e:	2580      	movs	r5, #128	; 0x80
 8002280:	eb04 0343 	add.w	r3, r4, r3, lsl #1
 8002284:	f893 805c 	ldrb.w	r8, [r3, #92]	; 0x5c
 8002288:	e7bb      	b.n	8002202 <updateGameBoard+0x36>
 800228a:	6da3      	ldr	r3, [r4, #88]	; 0x58
 800228c:	332e      	adds	r3, #46	; 0x2e
 800228e:	eb04 0243 	add.w	r2, r4, r3, lsl #1
 8002292:	f814 8013 	ldrb.w	r8, [r4, r3, lsl #1]
 8002296:	7855      	ldrb	r5, [r2, #1]
 8002298:	e7b3      	b.n	8002202 <updateGameBoard+0x36>
 800229a:	460e      	mov	r6, r1
 800229c:	e7c9      	b.n	8002232 <updateGameBoard+0x66>
 800229e:	bf00      	nop
 80022a0:	2000011c 	andcs	r0, r0, ip, lsl r1

080022a4 <resetGame>:
 80022a4:	b510      	push	{r4, lr}
 80022a6:	4604      	mov	r4, r0
 80022a8:	f003 fde2 	bl	8005e70 <mcumax_init>
 80022ac:	2200      	movs	r2, #0
 80022ae:	4b0c      	ldr	r3, [pc, #48]	; (80022e0 <resetGame+0x3c>)
 80022b0:	490c      	ldr	r1, [pc, #48]	; (80022e4 <resetGame+0x40>)
 80022b2:	e9c3 2401 	strd	r2, r4, [r3, #4]
 80022b6:	e9c3 2203 	strd	r2, r2, [r3, #12]
 80022ba:	8808      	ldrh	r0, [r1, #0]
 80022bc:	1899      	adds	r1, r3, r2
 80022be:	3202      	adds	r2, #2
 80022c0:	2a0c      	cmp	r2, #12
 80022c2:	f8a1 01c6 	strh.w	r0, [r1, #454]	; 0x1c6
 80022c6:	d1f9      	bne.n	80022bc <resetGame+0x18>
 80022c8:	b93c      	cbnz	r4, 80022da <resetGame+0x36>
 80022ca:	2201      	movs	r2, #1
 80022cc:	701a      	strb	r2, [r3, #0]
 80022ce:	f7ff ff6d 	bl	80021ac <updateValidMoves>
 80022d2:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 80022d6:	f7ff bf79 	b.w	80021cc <updateGameBoard>
 80022da:	2203      	movs	r2, #3
 80022dc:	701a      	strb	r2, [r3, #0]
 80022de:	e7f8      	b.n	80022d2 <resetGame+0x2e>
 80022e0:	2000011c 	andcs	r0, r0, ip, lsl r1
 80022e4:	08006154 	stmdaeq	r0, {r2, r4, r6, r8, sp, lr}

080022e8 <stepValidMove>:
 80022e8:	4b08      	ldr	r3, [pc, #32]	; (800230c <stepValidMove+0x24>)
 80022ea:	6d9a      	ldr	r2, [r3, #88]	; 0x58
 80022ec:	4410      	add	r0, r2
 80022ee:	2800      	cmp	r0, #0
 80022f0:	6598      	str	r0, [r3, #88]	; 0x58
 80022f2:	6d5a      	ldr	r2, [r3, #84]	; 0x54
 80022f4:	da03      	bge.n	80022fe <stepValidMove+0x16>
 80022f6:	3a01      	subs	r2, #1
 80022f8:	659a      	str	r2, [r3, #88]	; 0x58
 80022fa:	2001      	movs	r0, #1
 80022fc:	4770      	bx	lr
 80022fe:	4290      	cmp	r0, r2
 8002300:	f04f 0000 	mov.w	r0, #0
 8002304:	dbfa      	blt.n	80022fc <stepValidMove+0x14>
 8002306:	6598      	str	r0, [r3, #88]	; 0x58
 8002308:	e7f7      	b.n	80022fa <stepValidMove+0x12>
 800230a:	bf00      	nop
 800230c:	2000011c 	andcs	r0, r0, ip, lsl r1

08002310 <stepValidMoveFrom>:
 8002310:	b538      	push	{r3, r4, r5, lr}
 8002312:	4605      	mov	r5, r0
 8002314:	4908      	ldr	r1, [pc, #32]	; (8002338 <stepValidMoveFrom+0x28>)
 8002316:	6d8a      	ldr	r2, [r1, #88]	; 0x58
 8002318:	eb01 0242 	add.w	r2, r1, r2, lsl #1
 800231c:	f892 405c 	ldrb.w	r4, [r2, #92]	; 0x5c
 8002320:	4628      	mov	r0, r5
 8002322:	f7ff ffe1 	bl	80022e8 <stepValidMove>
 8002326:	b930      	cbnz	r0, 8002336 <stepValidMoveFrom+0x26>
 8002328:	6d8b      	ldr	r3, [r1, #88]	; 0x58
 800232a:	eb01 0343 	add.w	r3, r1, r3, lsl #1
 800232e:	f893 305c 	ldrb.w	r3, [r3, #92]	; 0x5c
 8002332:	42a3      	cmp	r3, r4
 8002334:	d0f4      	beq.n	8002320 <stepValidMoveFrom+0x10>
 8002336:	bd38      	pop	{r3, r4, r5, pc}
 8002338:	2000011c 	andcs	r0, r0, ip, lsl r1

0800233c <onGameViewEvent>:
 800233c:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 8002340:	b092      	sub	sp, #72	; 0x48
 8002342:	2906      	cmp	r1, #6
 8002344:	d80d      	bhi.n	8002362 <onGameViewEvent+0x26>
 8002346:	e8df f001 	tbb	[pc, r1]
 800234a:	40401d04 	submi	r1, r0, r4, lsl #26
 800234e:	006d0c0c 	rsbeq	r0, sp, ip, lsl #24
 8002352:	4c60      	ldr	r4, [pc, #384]	; (80024d4 <onGameViewEvent+0x198>)
 8002354:	7823      	ldrb	r3, [r4, #0]
 8002356:	2b03      	cmp	r3, #3
 8002358:	d106      	bne.n	8002368 <onGameViewEvent+0x2c>
 800235a:	f003 fe31 	bl	8005fc0 <mcumax_stop_search>
 800235e:	2304      	movs	r3, #4
 8002360:	7023      	strb	r3, [r4, #0]
 8002362:	b012      	add	sp, #72	; 0x48
 8002364:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 8002368:	2b02      	cmp	r3, #2
 800236a:	d105      	bne.n	8002378 <onGameViewEvent+0x3c>
 800236c:	2201      	movs	r2, #1
 800236e:	4b59      	ldr	r3, [pc, #356]	; (80024d4 <onGameViewEvent+0x198>)
 8002370:	701a      	strb	r2, [r3, #0]
 8002372:	f7ff ff2b 	bl	80021cc <updateGameBoard>
 8002376:	e7f4      	b.n	8002362 <onGameViewEvent+0x26>
 8002378:	2b04      	cmp	r3, #4
 800237a:	d0f2      	beq.n	8002362 <onGameViewEvent+0x26>
 800237c:	4856      	ldr	r0, [pc, #344]	; (80024d8 <onGameViewEvent+0x19c>)
 800237e:	f002 ff83 	bl	8005288 <setView>
 8002382:	e7ee      	b.n	8002362 <onGameViewEvent+0x26>
 8002384:	4c53      	ldr	r4, [pc, #332]	; (80024d4 <onGameViewEvent+0x198>)
 8002386:	7825      	ldrb	r5, [r4, #0]
 8002388:	2d01      	cmp	r5, #1
 800238a:	d011      	beq.n	80023b0 <onGameViewEvent+0x74>
 800238c:	2d02      	cmp	r5, #2
 800238e:	d1e8      	bne.n	8002362 <onGameViewEvent+0x26>
 8002390:	6da3      	ldr	r3, [r4, #88]	; 0x58
 8002392:	eb04 0343 	add.w	r3, r4, r3, lsl #1
 8002396:	f8b3 305c 	ldrh.w	r3, [r3, #92]	; 0x5c
 800239a:	f8ad 3024 	strh.w	r3, [sp, #36]	; 0x24
 800239e:	9809      	ldr	r0, [sp, #36]	; 0x24
 80023a0:	f003 fdf8 	bl	8005f94 <mcumax_play_move>
 80023a4:	9809      	ldr	r0, [sp, #36]	; 0x24
 80023a6:	f7ff fe8d 	bl	80020c4 <recordGameMove>
 80023aa:	2303      	movs	r3, #3
 80023ac:	7023      	strb	r3, [r4, #0]
 80023ae:	e7e0      	b.n	8002372 <onGameViewEvent+0x36>
 80023b0:	6d63      	ldr	r3, [r4, #84]	; 0x54
 80023b2:	2b00      	cmp	r3, #0
 80023b4:	d0d5      	beq.n	8002362 <onGameViewEvent+0x26>
 80023b6:	2302      	movs	r3, #2
 80023b8:	f04f 30ff 	mov.w	r0, #4294967295
 80023bc:	7023      	strb	r3, [r4, #0]
 80023be:	f7ff ffa7 	bl	8002310 <stepValidMoveFrom>
 80023c2:	4628      	mov	r0, r5
 80023c4:	f7ff ff90 	bl	80022e8 <stepValidMove>
 80023c8:	e7d3      	b.n	8002372 <onGameViewEvent+0x36>
 80023ca:	4c42      	ldr	r4, [pc, #264]	; (80024d4 <onGameViewEvent+0x198>)
 80023cc:	7823      	ldrb	r3, [r4, #0]
 80023ce:	2b00      	cmp	r3, #0
 80023d0:	d0cc      	beq.n	800236c <onGameViewEvent+0x30>
 80023d2:	2b01      	cmp	r3, #1
 80023d4:	d10a      	bne.n	80023ec <onGameViewEvent+0xb0>
 80023d6:	68a3      	ldr	r3, [r4, #8]
 80023d8:	2902      	cmp	r1, #2
 80023da:	bf0c      	ite	eq
 80023dc:	f04f 30ff 	moveq.w	r0, #4294967295
 80023e0:	2001      	movne	r0, #1
 80023e2:	b103      	cbz	r3, 80023e6 <onGameViewEvent+0xaa>
 80023e4:	4240      	negs	r0, r0
 80023e6:	f7ff ff93 	bl	8002310 <stepValidMoveFrom>
 80023ea:	e7c2      	b.n	8002372 <onGameViewEvent+0x36>
 80023ec:	2b02      	cmp	r3, #2
 80023ee:	d1b8      	bne.n	8002362 <onGameViewEvent+0x26>
 80023f0:	2902      	cmp	r1, #2
 80023f2:	bf0c      	ite	eq
 80023f4:	f04f 35ff 	moveq.w	r5, #4294967295
 80023f8:	2501      	movne	r5, #1
 80023fa:	6da3      	ldr	r3, [r4, #88]	; 0x58
 80023fc:	4628      	mov	r0, r5
 80023fe:	eb04 0343 	add.w	r3, r4, r3, lsl #1
 8002402:	f893 105c 	ldrb.w	r1, [r3, #92]	; 0x5c
 8002406:	f7ff ff6f 	bl	80022e8 <stepValidMove>
 800240a:	6da3      	ldr	r3, [r4, #88]	; 0x58
 800240c:	eb04 0443 	add.w	r4, r4, r3, lsl #1
 8002410:	f894 305c 	ldrb.w	r3, [r4, #92]	; 0x5c
 8002414:	428b      	cmp	r3, r1
 8002416:	d0ac      	beq.n	8002372 <onGameViewEvent+0x36>
 8002418:	4269      	negs	r1, r5
 800241a:	4608      	mov	r0, r1
 800241c:	f7ff ff64 	bl	80022e8 <stepValidMove>
 8002420:	4608      	mov	r0, r1
 8002422:	e7cc      	b.n	80023be <onGameViewEvent+0x82>
 8002424:	4c2b      	ldr	r4, [pc, #172]	; (80024d4 <onGameViewEvent+0x198>)
 8002426:	492d      	ldr	r1, [pc, #180]	; (80024dc <onGameViewEvent+0x1a0>)
 8002428:	a801      	add	r0, sp, #4
 800242a:	f003 fe17 	bl	800605c <strcpy>
 800242e:	68a3      	ldr	r3, [r4, #8]
 8002430:	2500      	movs	r5, #0
 8002432:	2b00      	cmp	r3, #0
 8002434:	bf0c      	ite	eq
 8002436:	230c      	moveq	r3, #12
 8002438:	2308      	movne	r3, #8
 800243a:	4423      	add	r3, r4
 800243c:	6859      	ldr	r1, [r3, #4]
 800243e:	a801      	add	r0, sp, #4
 8002440:	f7fe fb6e 	bl	8000b20 <strcatTime>
 8002444:	4925      	ldr	r1, [pc, #148]	; (80024dc <onGameViewEvent+0x1a0>)
 8002446:	a805      	add	r0, sp, #20
 8002448:	f003 fe08 	bl	800605c <strcpy>
 800244c:	68a3      	ldr	r3, [r4, #8]
 800244e:	a805      	add	r0, sp, #20
 8002450:	eb04 0383 	add.w	r3, r4, r3, lsl #2
 8002454:	68d9      	ldr	r1, [r3, #12]
 8002456:	f7fe fb63 	bl	8000b20 <strcatTime>
 800245a:	46aa      	mov	sl, r5
 800245c:	f8df 907c 	ldr.w	r9, [pc, #124]	; 80024dc <onGameViewEvent+0x1a0>
 8002460:	230c      	movs	r3, #12
 8002462:	aa09      	add	r2, sp, #36	; 0x24
 8002464:	eb04 0685 	add.w	r6, r4, r5, lsl #2
 8002468:	fb03 2305 	mla	r3, r3, r5, r2
 800246c:	2700      	movs	r7, #0
 800246e:	f04f 082d 	mov.w	r8, #45	; 0x2d
 8002472:	e000      	b.n	8002476 <onGameViewEvent+0x13a>
 8002474:	2701      	movs	r7, #1
 8002476:	f896 21c6 	ldrb.w	r2, [r6, #454]	; 0x1c6
 800247a:	f896 11c7 	ldrb.w	r1, [r6, #455]	; 0x1c7
 800247e:	2a80      	cmp	r2, #128	; 0x80
 8002480:	d111      	bne.n	80024a6 <onGameViewEvent+0x16a>
 8002482:	4618      	mov	r0, r3
 8002484:	4649      	mov	r1, r9
 8002486:	f003 fde9 	bl	800605c <strcpy>
 800248a:	4603      	mov	r3, r0
 800248c:	3602      	adds	r6, #2
 800248e:	3306      	adds	r3, #6
 8002490:	2f00      	cmp	r7, #0
 8002492:	d0ef      	beq.n	8002474 <onGameViewEvent+0x138>
 8002494:	3501      	adds	r5, #1
 8002496:	2d03      	cmp	r5, #3
 8002498:	d1e2      	bne.n	8002460 <onGameViewEvent+0x124>
 800249a:	aa09      	add	r2, sp, #36	; 0x24
 800249c:	a901      	add	r1, sp, #4
 800249e:	4810      	ldr	r0, [pc, #64]	; (80024e0 <onGameViewEvent+0x1a4>)
 80024a0:	f7ff fb84 	bl	8001bac <drawGame>
 80024a4:	e75d      	b.n	8002362 <onGameViewEvent+0x26>
 80024a6:	f002 0007 	and.w	r0, r2, #7
 80024aa:	f3c2 1202 	ubfx	r2, r2, #4, #3
 80024ae:	f1c2 0238 	rsb	r2, r2, #56	; 0x38
 80024b2:	705a      	strb	r2, [r3, #1]
 80024b4:	f001 0207 	and.w	r2, r1, #7
 80024b8:	f3c1 1102 	ubfx	r1, r1, #4, #3
 80024bc:	3061      	adds	r0, #97	; 0x61
 80024be:	3261      	adds	r2, #97	; 0x61
 80024c0:	f1c1 0138 	rsb	r1, r1, #56	; 0x38
 80024c4:	7018      	strb	r0, [r3, #0]
 80024c6:	f883 8002 	strb.w	r8, [r3, #2]
 80024ca:	70da      	strb	r2, [r3, #3]
 80024cc:	7119      	strb	r1, [r3, #4]
 80024ce:	f883 a005 	strb.w	sl, [r3, #5]
 80024d2:	e7db      	b.n	800248c <onGameViewEvent+0x150>
 80024d4:	2000011c 	andcs	r0, r0, ip, lsl r1
 80024d8:	08006f50 	stmdaeq	r0, {r4, r6, r8, r9, sl, fp, sp, lr}
 80024dc:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}
 80024e0:	20000130 	andcs	r0, r0, r0, lsr r1

080024e4 <onGameMenuSelect>:
 80024e4:	6843      	ldr	r3, [r0, #4]
 80024e6:	b510      	push	{r4, lr}
 80024e8:	885c      	ldrh	r4, [r3, #2]
 80024ea:	2c01      	cmp	r4, #1
 80024ec:	d00d      	beq.n	800250a <onGameMenuSelect+0x26>
 80024ee:	d302      	bcc.n	80024f6 <onGameMenuSelect+0x12>
 80024f0:	2c02      	cmp	r4, #2
 80024f2:	d019      	beq.n	8002528 <onGameMenuSelect+0x44>
 80024f4:	bd10      	pop	{r4, pc}
 80024f6:	4b0d      	ldr	r3, [pc, #52]	; (800252c <onGameMenuSelect+0x48>)
 80024f8:	6858      	ldr	r0, [r3, #4]
 80024fa:	b908      	cbnz	r0, 8002500 <onGameMenuSelect+0x1c>
 80024fc:	f7ff fed2 	bl	80022a4 <resetGame>
 8002500:	480b      	ldr	r0, [pc, #44]	; (8002530 <onGameMenuSelect+0x4c>)
 8002502:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8002506:	f002 bebf 	b.w	8005288 <setView>
 800250a:	2203      	movs	r2, #3
 800250c:	2100      	movs	r1, #0
 800250e:	4809      	ldr	r0, [pc, #36]	; (8002534 <onGameMenuSelect+0x50>)
 8002510:	f001 f8ac 	bl	800366c <selectMenuItem>
 8002514:	4b05      	ldr	r3, [pc, #20]	; (800252c <onGameMenuSelect+0x48>)
 8002516:	685b      	ldr	r3, [r3, #4]
 8002518:	b90b      	cbnz	r3, 800251e <onGameMenuSelect+0x3a>
 800251a:	4620      	mov	r0, r4
 800251c:	e7ee      	b.n	80024fc <onGameMenuSelect+0x18>
 800251e:	2000      	movs	r0, #0
 8002520:	f7ff fec0 	bl	80022a4 <resetGame>
 8002524:	4804      	ldr	r0, [pc, #16]	; (8002538 <onGameMenuSelect+0x54>)
 8002526:	e7ec      	b.n	8002502 <onGameMenuSelect+0x1e>
 8002528:	4804      	ldr	r0, [pc, #16]	; (800253c <onGameMenuSelect+0x58>)
 800252a:	e7ea      	b.n	8002502 <onGameMenuSelect+0x1e>
 800252c:	2000011c 	andcs	r0, r0, ip, lsl r1
 8002530:	08006f94 	stmdaeq	r0, {r2, r4, r7, r8, r9, sl, fp, sp, lr}
 8002534:	08006f3c 	stmdaeq	r0, {r2, r3, r4, r5, r8, r9, sl, fp, sp, lr}
 8002538:	08006f50 	stmdaeq	r0, {r4, r6, r8, r9, sl, fp, sp, lr}
 800253c:	08006f7c 	stmdaeq	r0, {r2, r3, r4, r5, r6, r8, r9, sl, fp, sp, lr}

08002540 <initGame>:
 8002540:	4b03      	ldr	r3, [pc, #12]	; (8002550 <initGame+0x10>)
 8002542:	2208      	movs	r2, #8
 8002544:	7a19      	ldrb	r1, [r3, #8]
 8002546:	4803      	ldr	r0, [pc, #12]	; (8002554 <initGame+0x14>)
 8002548:	f001 0107 	and.w	r1, r1, #7
 800254c:	f001 b88e 	b.w	800366c <selectMenuItem>
 8002550:	20000da0 	andcs	r0, r0, r0, lsr #27
 8002554:	08006f68 	stmdaeq	r0, {r3, r5, r6, r8, r9, sl, fp, sp, lr}

08002558 <dispatchGameEvents>:
 8002558:	b573      	push	{r0, r1, r4, r5, r6, lr}
 800255a:	4c1f      	ldr	r4, [pc, #124]	; (80025d8 <dispatchGameEvents+0x80>)
 800255c:	7825      	ldrb	r5, [r4, #0]
 800255e:	4626      	mov	r6, r4
 8002560:	2d03      	cmp	r5, #3
 8002562:	d136      	bne.n	80025d2 <dispatchGameEvents+0x7a>
 8002564:	2100      	movs	r1, #0
 8002566:	481d      	ldr	r0, [pc, #116]	; (80025dc <dispatchGameEvents+0x84>)
 8002568:	f003 fd24 	bl	8005fb4 <mcumax_set_callback>
 800256c:	4b1c      	ldr	r3, [pc, #112]	; (80025e0 <dispatchGameEvents+0x88>)
 800256e:	4a1d      	ldr	r2, [pc, #116]	; (80025e4 <dispatchGameEvents+0x8c>)
 8002570:	7a1b      	ldrb	r3, [r3, #8]
 8002572:	2110      	movs	r1, #16
 8002574:	f003 0307 	and.w	r3, r3, #7
 8002578:	f832 0013 	ldrh.w	r0, [r2, r3, lsl #1]
 800257c:	f003 fce2 	bl	8005f44 <mcumax_search_best_move>
 8002580:	2100      	movs	r1, #0
 8002582:	f8ad 0004 	strh.w	r0, [sp, #4]
 8002586:	4608      	mov	r0, r1
 8002588:	f003 fd14 	bl	8005fb4 <mcumax_set_callback>
 800258c:	7823      	ldrb	r3, [r4, #0]
 800258e:	2b04      	cmp	r3, #4
 8002590:	d106      	bne.n	80025a0 <dispatchGameEvents+0x48>
 8002592:	4815      	ldr	r0, [pc, #84]	; (80025e8 <dispatchGameEvents+0x90>)
 8002594:	7025      	strb	r5, [r4, #0]
 8002596:	b002      	add	sp, #8
 8002598:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 800259c:	f002 be74 	b.w	8005288 <setView>
 80025a0:	f89d 3004 	ldrb.w	r3, [sp, #4]
 80025a4:	2b80      	cmp	r3, #128	; 0x80
 80025a6:	d106      	bne.n	80025b6 <dispatchGameEvents+0x5e>
 80025a8:	2305      	movs	r3, #5
 80025aa:	7033      	strb	r3, [r6, #0]
 80025ac:	b002      	add	sp, #8
 80025ae:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 80025b2:	f7ff be0b 	b.w	80021cc <updateGameBoard>
 80025b6:	9801      	ldr	r0, [sp, #4]
 80025b8:	f003 fcec 	bl	8005f94 <mcumax_play_move>
 80025bc:	9801      	ldr	r0, [sp, #4]
 80025be:	f7ff fd81 	bl	80020c4 <recordGameMove>
 80025c2:	f7ff fdf3 	bl	80021ac <updateValidMoves>
 80025c6:	6d63      	ldr	r3, [r4, #84]	; 0x54
 80025c8:	2b00      	cmp	r3, #0
 80025ca:	d0ed      	beq.n	80025a8 <dispatchGameEvents+0x50>
 80025cc:	2300      	movs	r3, #0
 80025ce:	7023      	strb	r3, [r4, #0]
 80025d0:	e7ec      	b.n	80025ac <dispatchGameEvents+0x54>
 80025d2:	b002      	add	sp, #8
 80025d4:	bd70      	pop	{r4, r5, r6, pc}
 80025d6:	bf00      	nop
 80025d8:	2000011c 	andcs	r0, r0, ip, lsl r1
 80025dc:	080021a9 	stmdaeq	r0, {r0, r3, r5, r7, r8, sp}
 80025e0:	20000da0 	andcs	r0, r0, r0, lsr #27
 80025e4:	08006f84 	stmdaeq	r0, {r2, r7, r8, r9, sl, fp, sp, lr}
 80025e8:	08006f50 	stmdaeq	r0, {r4, r6, r8, r9, sl, fp, sp, lr}

080025ec <updateGame>:
 80025ec:	b508      	push	{r3, lr}
 80025ee:	f002 fe59 	bl	80052a4 <getView>
 80025f2:	4b08      	ldr	r3, [pc, #32]	; (8002614 <updateGame+0x28>)
 80025f4:	4283      	cmp	r3, r0
 80025f6:	d10c      	bne.n	8002612 <updateGame+0x26>
 80025f8:	4b07      	ldr	r3, [pc, #28]	; (8002618 <updateGame+0x2c>)
 80025fa:	781a      	ldrb	r2, [r3, #0]
 80025fc:	2a05      	cmp	r2, #5
 80025fe:	d008      	beq.n	8002612 <updateGame+0x26>
 8002600:	685a      	ldr	r2, [r3, #4]
 8002602:	f002 0201 	and.w	r2, r2, #1
 8002606:	3202      	adds	r2, #2
 8002608:	eb03 0382 	add.w	r3, r3, r2, lsl #2
 800260c:	685a      	ldr	r2, [r3, #4]
 800260e:	3201      	adds	r2, #1
 8002610:	605a      	str	r2, [r3, #4]
 8002612:	bd08      	pop	{r3, pc}
 8002614:	08006f94 	stmdaeq	r0, {r2, r4, r7, r8, r9, sl, fp, sp, lr}
 8002618:	2000011c 	andcs	r0, r0, ip, lsl r1

0800261c <initKeyboard>:
 800261c:	b508      	push	{r3, lr}
 800261e:	f001 fd6d 	bl	80040fc <initKeyboardController>
 8002622:	2205      	movs	r2, #5
 8002624:	4b02      	ldr	r3, [pc, #8]	; (8002630 <initKeyboard+0x14>)
 8002626:	719a      	strb	r2, [r3, #6]
 8002628:	2201      	movs	r2, #1
 800262a:	701a      	strb	r2, [r3, #0]
 800262c:	bd08      	pop	{r3, pc}
 800262e:	bf00      	nop
 8002630:	200002f8 	strdcs	r0, [r0], -r8

08002634 <onKeyboardTick>:
 8002634:	e92d 43f7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
 8002638:	4c35      	ldr	r4, [pc, #212]	; (8002710 <onKeyboardTick+0xdc>)
 800263a:	7823      	ldrb	r3, [r4, #0]
 800263c:	2b00      	cmp	r3, #0
 800263e:	d052      	beq.n	80026e6 <onKeyboardTick+0xb2>
 8002640:	4668      	mov	r0, sp
 8002642:	f001 fd75 	bl	8004130 <getKeyboardState>
 8002646:	2000      	movs	r0, #0
 8002648:	7b27      	ldrb	r7, [r4, #12]
 800264a:	4626      	mov	r6, r4
 800264c:	f117 0900 	adds.w	r9, r7, #0
 8002650:	bf18      	it	ne
 8002652:	f04f 0901 	movne.w	r9, #1
 8002656:	4686      	mov	lr, r0
 8002658:	4603      	mov	r3, r0
 800265a:	2507      	movs	r5, #7
 800265c:	79a2      	ldrb	r2, [r4, #6]
 800265e:	f8d4 c008 	ldr.w	ip, [r4, #8]
 8002662:	f816 8f01 	ldrb.w	r8, [r6, #1]!
 8002666:	f81d 1003 	ldrb.w	r1, [sp, r3]
 800266a:	f1b8 0f00 	cmp.w	r8, #0
 800266e:	d13f      	bne.n	80026f0 <onKeyboardTick+0xbc>
 8002670:	b149      	cbz	r1, 8002686 <onKeyboardTick+0x52>
 8002672:	b2da      	uxtb	r2, r3
 8002674:	b907      	cbnz	r7, 8002678 <onKeyboardTick+0x44>
 8002676:	b113      	cbz	r3, 800267e <onKeyboardTick+0x4a>
 8002678:	2b04      	cmp	r3, #4
 800267a:	bf18      	it	ne
 800267c:	4615      	movne	r5, r2
 800267e:	4608      	mov	r0, r1
 8002680:	468e      	mov	lr, r1
 8002682:	f04f 0c00 	mov.w	ip, #0
 8002686:	3301      	adds	r3, #1
 8002688:	2b05      	cmp	r3, #5
 800268a:	7031      	strb	r1, [r6, #0]
 800268c:	d1e9      	bne.n	8002662 <onKeyboardTick+0x2e>
 800268e:	f1be 0f00 	cmp.w	lr, #0
 8002692:	d000      	beq.n	8002696 <onKeyboardTick+0x62>
 8002694:	71a2      	strb	r2, [r4, #6]
 8002696:	b108      	cbz	r0, 800269c <onKeyboardTick+0x68>
 8002698:	f8c4 c008 	str.w	ip, [r4, #8]
 800269c:	79a3      	ldrb	r3, [r4, #6]
 800269e:	2b05      	cmp	r3, #5
 80026a0:	d01f      	beq.n	80026e2 <onKeyboardTick+0xae>
 80026a2:	2101      	movs	r1, #1
 80026a4:	481b      	ldr	r0, [pc, #108]	; (8002714 <onKeyboardTick+0xe0>)
 80026a6:	f7fd fffa 	bl	800069e <addClamped>
 80026aa:	68a3      	ldr	r3, [r4, #8]
 80026ac:	2b13      	cmp	r3, #19
 80026ae:	d918      	bls.n	80026e2 <onKeyboardTick+0xae>
 80026b0:	07da      	lsls	r2, r3, #31
 80026b2:	d404      	bmi.n	80026be <onKeyboardTick+0x8a>
 80026b4:	79a2      	ldrb	r2, [r4, #6]
 80026b6:	1e91      	subs	r1, r2, #2
 80026b8:	2901      	cmp	r1, #1
 80026ba:	bf98      	it	ls
 80026bc:	4615      	movls	r5, r2
 80026be:	2b28      	cmp	r3, #40	; 0x28
 80026c0:	d10f      	bne.n	80026e2 <onKeyboardTick+0xae>
 80026c2:	79a3      	ldrb	r3, [r4, #6]
 80026c4:	b193      	cbz	r3, 80026ec <onKeyboardTick+0xb8>
 80026c6:	2b04      	cmp	r3, #4
 80026c8:	d10b      	bne.n	80026e2 <onKeyboardTick+0xae>
 80026ca:	2505      	movs	r5, #5
 80026cc:	6923      	ldr	r3, [r4, #16]
 80026ce:	4423      	add	r3, r4
 80026d0:	761d      	strb	r5, [r3, #24]
 80026d2:	6923      	ldr	r3, [r4, #16]
 80026d4:	3301      	adds	r3, #1
 80026d6:	f003 030f 	and.w	r3, r3, #15
 80026da:	6123      	str	r3, [r4, #16]
 80026dc:	e003      	b.n	80026e6 <onKeyboardTick+0xb2>
 80026de:	461d      	mov	r5, r3
 80026e0:	e013      	b.n	800270a <onKeyboardTick+0xd6>
 80026e2:	2d07      	cmp	r5, #7
 80026e4:	d1f2      	bne.n	80026cc <onKeyboardTick+0x98>
 80026e6:	b003      	add	sp, #12
 80026e8:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 80026ec:	2504      	movs	r5, #4
 80026ee:	e7ed      	b.n	80026cc <onKeyboardTick+0x98>
 80026f0:	2900      	cmp	r1, #0
 80026f2:	d1c8      	bne.n	8002686 <onKeyboardTick+0x52>
 80026f4:	429a      	cmp	r2, r3
 80026f6:	d1c6      	bne.n	8002686 <onKeyboardTick+0x52>
 80026f8:	f1bc 0f27 	cmp.w	ip, #39	; 0x27
 80026fc:	d805      	bhi.n	800270a <onKeyboardTick+0xd6>
 80026fe:	b90f      	cbnz	r7, 8002704 <onKeyboardTick+0xd0>
 8002700:	2b00      	cmp	r3, #0
 8002702:	d0ec      	beq.n	80026de <onKeyboardTick+0xaa>
 8002704:	2b04      	cmp	r3, #4
 8002706:	bf08      	it	eq
 8002708:	464d      	moveq	r5, r9
 800270a:	46c6      	mov	lr, r8
 800270c:	2205      	movs	r2, #5
 800270e:	e7ba      	b.n	8002686 <onKeyboardTick+0x52>
 8002710:	200002f8 	strdcs	r0, [r0], -r8
 8002714:	20000300 	andcs	r0, r0, r0, lsl #6

08002718 <setKeyboardMode>:
 8002718:	2205      	movs	r2, #5
 800271a:	4b02      	ldr	r3, [pc, #8]	; (8002724 <setKeyboardMode+0xc>)
 800271c:	719a      	strb	r2, [r3, #6]
 800271e:	7318      	strb	r0, [r3, #12]
 8002720:	4770      	bx	lr
 8002722:	bf00      	nop
 8002724:	200002f8 	strdcs	r0, [r0], -r8

08002728 <getKeyboardEvent>:
 8002728:	4b08      	ldr	r3, [pc, #32]	; (800274c <getKeyboardEvent+0x24>)
 800272a:	6919      	ldr	r1, [r3, #16]
 800272c:	695a      	ldr	r2, [r3, #20]
 800272e:	4291      	cmp	r1, r2
 8002730:	d009      	beq.n	8002746 <getKeyboardEvent+0x1e>
 8002732:	695a      	ldr	r2, [r3, #20]
 8002734:	441a      	add	r2, r3
 8002736:	7e10      	ldrb	r0, [r2, #24]
 8002738:	695a      	ldr	r2, [r3, #20]
 800273a:	b2c0      	uxtb	r0, r0
 800273c:	3201      	adds	r2, #1
 800273e:	f002 020f 	and.w	r2, r2, #15
 8002742:	615a      	str	r2, [r3, #20]
 8002744:	4770      	bx	lr
 8002746:	2007      	movs	r0, #7
 8002748:	4770      	bx	lr
 800274a:	bf00      	nop
 800274c:	200002f8 	strdcs	r0, [r0], -r8

08002750 <main>:
 8002750:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8002752:	f001 fccb 	bl	80040ec <initSystem>
 8002756:	f7ff fb07 	bl	8001d68 <initEvents>
 800275a:	f002 f829 	bl	80047b0 <initFlash>
 800275e:	f001 fbeb 	bl	8003f38 <initSettings>
 8002762:	f001 f803 	bl	800376c <initPower>
 8002766:	f001 fee1 	bl	800452c <initComm>
 800276a:	f001 fe09 	bl	8004380 <initADC>
 800276e:	f002 fc9d 	bl	80050ac <initTube>
 8002772:	f7ff ff53 	bl	800261c <initKeyboard>
 8002776:	f7fd ff6f 	bl	8000658 <initBuzzer>
 800277a:	f7fe fe17 	bl	80013ac <initDisplay>
 800277e:	f002 fad7 	bl	8004d30 <initVibrator>
 8002782:	f000 fa53 	bl	8002c2c <initMeasurements>
 8002786:	f7ff fedb 	bl	8002540 <initGame>
 800278a:	f7fe fc7f 	bl	800108c <initDatalog>
 800278e:	4d38      	ldr	r5, [pc, #224]	; (8002870 <main+0x120>)
 8002790:	4e38      	ldr	r6, [pc, #224]	; (8002874 <main+0x124>)
 8002792:	4f39      	ldr	r7, [pc, #228]	; (8002878 <main+0x128>)
 8002794:	f7ff ffc8 	bl	8002728 <getKeyboardEvent>
 8002798:	2805      	cmp	r0, #5
 800279a:	f04f 0001 	mov.w	r0, #1
 800279e:	d15b      	bne.n	8002858 <main+0x108>
 80027a0:	f002 f8be 	bl	8004920 <setPower>
 80027a4:	f002 f806 	bl	80047b4 <verifyFlash>
 80027a8:	b990      	cbnz	r0, 80027d0 <main+0x80>
 80027aa:	2201      	movs	r2, #1
 80027ac:	4639      	mov	r1, r7
 80027ae:	4833      	ldr	r0, [pc, #204]	; (800287c <main+0x12c>)
 80027b0:	f7fe feaa 	bl	8001508 <drawNotification>
 80027b4:	f7fe fe2a 	bl	800140c <refreshDisplay>
 80027b8:	2001      	movs	r0, #1
 80027ba:	f7fe fe1d 	bl	80013f8 <setDisplayOn>
 80027be:	2001      	movs	r0, #1
 80027c0:	f001 ff96 	bl	80046f0 <setDisplayBacklight>
 80027c4:	f7fd ff5a 	bl	800067c <playSystemAlert>
 80027c8:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
 80027cc:	f001 ffd4 	bl	8004778 <sleep>
 80027d0:	2201      	movs	r2, #1
 80027d2:	4629      	mov	r1, r5
 80027d4:	4630      	mov	r0, r6
 80027d6:	f7fe fe97 	bl	8001508 <drawNotification>
 80027da:	f7fe fe17 	bl	800140c <refreshDisplay>
 80027de:	2001      	movs	r0, #1
 80027e0:	f7fe fe0a 	bl	80013f8 <setDisplayOn>
 80027e4:	2001      	movs	r0, #1
 80027e6:	f001 ff83 	bl	80046f0 <setDisplayBacklight>
 80027ea:	f7ff fb35 	bl	8001e58 <getTick>
 80027ee:	4604      	mov	r4, r0
 80027f0:	f001 f9d8 	bl	8003ba4 <initRTC>
 80027f4:	f7ff fb30 	bl	8001e58 <getTick>
 80027f8:	1b00      	subs	r0, r0, r4
 80027fa:	f5b0 7f7a 	cmp.w	r0, #1000	; 0x3e8
 80027fe:	bf38      	it	cc
 8002800:	f44f 707a 	movcc.w	r0, #1000	; 0x3e8
 8002804:	f001 ffb8 	bl	8004778 <sleep>
 8002808:	2001      	movs	r0, #1
 800280a:	f002 fa4d 	bl	8004ca8 <setTubeHV>
 800280e:	f7ff fb2d 	bl	8001e6c <enableMeasurements>
 8002812:	2001      	movs	r0, #1
 8002814:	f7fd fffa 	bl	800080c <setCommEnabled>
 8002818:	f7fe fc59 	bl	80010ce <writeDatalog>
 800281c:	2000      	movs	r0, #0
 800281e:	f000 fc31 	bl	8003084 <setMeasurementView>
 8002822:	f7ff fb63 	bl	8001eec <triggerDisplay>
 8002826:	f000 ffb9 	bl	800379c <isPowerOffRequested>
 800282a:	b1c0      	cbz	r0, 800285e <main+0x10e>
 800282c:	f7fe fc4f 	bl	80010ce <writeDatalog>
 8002830:	f001 fbd4 	bl	8003fdc <writeSettings>
 8002834:	2000      	movs	r0, #0
 8002836:	f7fd ffe9 	bl	800080c <setCommEnabled>
 800283a:	f7ff fb1f 	bl	8001e7c <disableMeasurements>
 800283e:	2000      	movs	r0, #0
 8002840:	f002 fa32 	bl	8004ca8 <setTubeHV>
 8002844:	2000      	movs	r0, #0
 8002846:	f001 ff53 	bl	80046f0 <setDisplayBacklight>
 800284a:	2000      	movs	r0, #0
 800284c:	f7fe fdd4 	bl	80013f8 <setDisplayOn>
 8002850:	2000      	movs	r0, #0
 8002852:	f002 f865 	bl	8004920 <setPower>
 8002856:	e79d      	b.n	8002794 <main+0x44>
 8002858:	f001 ff8e 	bl	8004778 <sleep>
 800285c:	e79a      	b.n	8002794 <main+0x44>
 800285e:	2001      	movs	r0, #1
 8002860:	f001 ff8a 	bl	8004778 <sleep>
 8002864:	f7ff fe78 	bl	8002558 <dispatchGameEvents>
 8002868:	f7ff fb16 	bl	8001e98 <dispatchEvents>
 800286c:	e7db      	b.n	8002826 <main+0xd6>
 800286e:	bf00      	nop
 8002870:	08007589 	stmdaeq	r0, {r0, r3, r7, r8, sl, ip, sp, lr}
 8002874:	08006ffa 	stmdaeq	r0, {r1, r3, r4, r5, r6, r7, r8, r9, sl, fp, sp, lr}
 8002878:	08006fd7 	stmdaeq	r0, {r0, r1, r2, r4, r6, r7, r8, r9, sl, fp, sp, lr}
 800287c:	08006ff2 	stmdaeq	r0, {r1, r4, r5, r6, r7, r8, r9, sl, fp, sp, lr}

08002880 <onUnitsMenuGetOption>:
 8002880:	4b05      	ldr	r3, [pc, #20]	; (8002898 <onUnitsMenuGetOption+0x18>)
 8002882:	781b      	ldrb	r3, [r3, #0]
 8002884:	f3c3 0341 	ubfx	r3, r3, #1, #2
 8002888:	1a58      	subs	r0, r3, r1
 800288a:	4243      	negs	r3, r0
 800288c:	4143      	adcs	r3, r0
 800288e:	6013      	str	r3, [r2, #0]
 8002890:	4b02      	ldr	r3, [pc, #8]	; (800289c <onUnitsMenuGetOption+0x1c>)
 8002892:	f853 0021 	ldr.w	r0, [r3, r1, lsl #2]
 8002896:	4770      	bx	lr
 8002898:	20000da0 	andcs	r0, r0, r0, lsr #27
 800289c:	080071e4 	stmdaeq	r0, {r2, r5, r6, r7, r8, ip, sp, lr}

080028a0 <onUnitsMenuSelect>:
 80028a0:	4b03      	ldr	r3, [pc, #12]	; (80028b0 <onUnitsMenuSelect+0x10>)
 80028a2:	6841      	ldr	r1, [r0, #4]
 80028a4:	781a      	ldrb	r2, [r3, #0]
 80028a6:	8849      	ldrh	r1, [r1, #2]
 80028a8:	f361 0242 	bfi	r2, r1, #1, #2
 80028ac:	701a      	strb	r2, [r3, #0]
 80028ae:	4770      	bx	lr
 80028b0:	20000da0 	andcs	r0, r0, r0, lsr #27

080028b4 <onAverageTimerMenuGetOption>:
 80028b4:	4b05      	ldr	r3, [pc, #20]	; (80028cc <onAverageTimerMenuGetOption+0x18>)
 80028b6:	781b      	ldrb	r3, [r3, #0]
 80028b8:	f3c3 03c2 	ubfx	r3, r3, #3, #3
 80028bc:	1a58      	subs	r0, r3, r1
 80028be:	4243      	negs	r3, r0
 80028c0:	4143      	adcs	r3, r0
 80028c2:	6013      	str	r3, [r2, #0]
 80028c4:	4b02      	ldr	r3, [pc, #8]	; (80028d0 <onAverageTimerMenuGetOption+0x1c>)
 80028c6:	f853 0021 	ldr.w	r0, [r3, r1, lsl #2]
 80028ca:	4770      	bx	lr
 80028cc:	20000da0 	andcs	r0, r0, r0, lsr #27
 80028d0:	08007020 	stmdaeq	r0, {r5, ip, sp, lr}

080028d4 <onAverageTimerMenuSelect>:
 80028d4:	4b03      	ldr	r3, [pc, #12]	; (80028e4 <onAverageTimerMenuSelect+0x10>)
 80028d6:	6841      	ldr	r1, [r0, #4]
 80028d8:	781a      	ldrb	r2, [r3, #0]
 80028da:	8849      	ldrh	r1, [r1, #2]
 80028dc:	f361 02c5 	bfi	r2, r1, #3, #3
 80028e0:	701a      	strb	r2, [r3, #0]
 80028e2:	4770      	bx	lr
 80028e4:	20000da0 	andcs	r0, r0, r0, lsr #27

080028e8 <onRateAlarmMenuSelect>:
 80028e8:	4b03      	ldr	r3, [pc, #12]	; (80028f8 <onRateAlarmMenuSelect+0x10>)
 80028ea:	6841      	ldr	r1, [r0, #4]
 80028ec:	881a      	ldrh	r2, [r3, #0]
 80028ee:	8849      	ldrh	r1, [r1, #2]
 80028f0:	f361 1289 	bfi	r2, r1, #6, #4
 80028f4:	801a      	strh	r2, [r3, #0]
 80028f6:	4770      	bx	lr
 80028f8:	20000da0 	andcs	r0, r0, r0, lsr #27

080028fc <onDoseAlarmMenuSelect>:
 80028fc:	4b03      	ldr	r3, [pc, #12]	; (800290c <onDoseAlarmMenuSelect+0x10>)
 80028fe:	6841      	ldr	r1, [r0, #4]
 8002900:	785a      	ldrb	r2, [r3, #1]
 8002902:	8849      	ldrh	r1, [r1, #2]
 8002904:	f361 0285 	bfi	r2, r1, #2, #4
 8002908:	705a      	strb	r2, [r3, #1]
 800290a:	4770      	bx	lr
 800290c:	20000da0 	andcs	r0, r0, r0, lsr #27

08002910 <buildValueString>:
 8002910:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8002914:	461d      	mov	r5, r3
 8002916:	4616      	mov	r6, r2
 8002918:	460f      	mov	r7, r1
 800291a:	4911      	ldr	r1, [pc, #68]	; (8002960 <buildValueString+0x50>)
 800291c:	4604      	mov	r4, r0
 800291e:	f003 fb9d 	bl	800605c <strcpy>
 8002922:	6869      	ldr	r1, [r5, #4]
 8002924:	4630      	mov	r0, r6
 8002926:	f001 fdb5 	bl	8004494 <__aeabi_fmul>
 800292a:	f995 8008 	ldrsb.w	r8, [r5, #8]
 800292e:	4601      	mov	r1, r0
 8002930:	4642      	mov	r2, r8
 8002932:	4620      	mov	r0, r4
 8002934:	f7fe f968 	bl	8000c08 <strcatFloatWithMetricPrefix>
 8002938:	6829      	ldr	r1, [r5, #0]
 800293a:	4620      	mov	r0, r4
 800293c:	f003 fb7f 	bl	800603e <strcat>
 8002940:	2100      	movs	r1, #0
 8002942:	4630      	mov	r0, r6
 8002944:	f001 fdb5 	bl	80044b2 <__aeabi_fcmpeq>
 8002948:	b118      	cbz	r0, 8002952 <buildValueString+0x42>
 800294a:	4906      	ldr	r1, [pc, #24]	; (8002964 <buildValueString+0x54>)
 800294c:	4620      	mov	r0, r4
 800294e:	f003 fb85 	bl	800605c <strcpy>
 8002952:	2300      	movs	r3, #0
 8002954:	3406      	adds	r4, #6
 8002956:	f804 3c01 	strb.w	r3, [r4, #-1]
 800295a:	603c      	str	r4, [r7, #0]
 800295c:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8002960:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}
 8002964:	08007054 	stmdaeq	r0, {r2, r4, r6, ip, sp, lr}

08002968 <isInstantaneousRateAlarm>:
 8002968:	4b0c      	ldr	r3, [pc, #48]	; (800299c <isInstantaneousRateAlarm+0x34>)
 800296a:	b510      	push	{r4, lr}
 800296c:	881c      	ldrh	r4, [r3, #0]
 800296e:	f414 7070 	ands.w	r0, r4, #960	; 0x3c0
 8002972:	d012      	beq.n	800299a <isInstantaneousRateAlarm+0x32>
 8002974:	4b0a      	ldr	r3, [pc, #40]	; (80029a0 <isInstantaneousRateAlarm+0x38>)
 8002976:	f8d3 11ac 	ldr.w	r1, [r3, #428]	; 0x1ac
 800297a:	4b0a      	ldr	r3, [pc, #40]	; (80029a4 <isInstantaneousRateAlarm+0x3c>)
 800297c:	6858      	ldr	r0, [r3, #4]
 800297e:	f001 fd89 	bl	8004494 <__aeabi_fmul>
 8002982:	4a09      	ldr	r2, [pc, #36]	; (80029a8 <isInstantaneousRateAlarm+0x40>)
 8002984:	f3c4 1383 	ubfx	r3, r4, #6, #4
 8002988:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 800298c:	f04f 0401 	mov.w	r4, #1
 8002990:	f001 fd9b 	bl	80044ca <__aeabi_fcmpge>
 8002994:	b900      	cbnz	r0, 8002998 <isInstantaneousRateAlarm+0x30>
 8002996:	4604      	mov	r4, r0
 8002998:	b2e0      	uxtb	r0, r4
 800299a:	bd10      	pop	{r4, pc}
 800299c:	20000da0 	andcs	r0, r0, r0, lsr #27
 80029a0:	20000328 	andcs	r0, r0, r8, lsr #6
 80029a4:	2000000c 	andcs	r0, r0, ip
 80029a8:	08007144 	stmdaeq	r0, {r2, r6, r8, ip, sp, lr}

080029ac <isDoseAlarm>:
 80029ac:	4b0d      	ldr	r3, [pc, #52]	; (80029e4 <isDoseAlarm+0x38>)
 80029ae:	b510      	push	{r4, lr}
 80029b0:	785c      	ldrb	r4, [r3, #1]
 80029b2:	f014 003c 	ands.w	r0, r4, #60	; 0x3c
 80029b6:	d014      	beq.n	80029e2 <isDoseAlarm+0x36>
 80029b8:	4b0b      	ldr	r3, [pc, #44]	; (80029e8 <isDoseAlarm+0x3c>)
 80029ba:	f8d3 0200 	ldr.w	r0, [r3, #512]	; 0x200
 80029be:	f001 fd6d 	bl	800449c <__aeabi_ui2f>
 80029c2:	4b0a      	ldr	r3, [pc, #40]	; (80029ec <isDoseAlarm+0x40>)
 80029c4:	6919      	ldr	r1, [r3, #16]
 80029c6:	f001 fd65 	bl	8004494 <__aeabi_fmul>
 80029ca:	4a09      	ldr	r2, [pc, #36]	; (80029f0 <isDoseAlarm+0x44>)
 80029cc:	f3c4 0383 	ubfx	r3, r4, #2, #4
 80029d0:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 80029d4:	f04f 0401 	mov.w	r4, #1
 80029d8:	f001 fd77 	bl	80044ca <__aeabi_fcmpge>
 80029dc:	b900      	cbnz	r0, 80029e0 <isDoseAlarm+0x34>
 80029de:	4604      	mov	r4, r0
 80029e0:	b2e0      	uxtb	r0, r4
 80029e2:	bd10      	pop	{r4, pc}
 80029e4:	20000da0 	andcs	r0, r0, r0, lsr #27
 80029e8:	20000328 	andcs	r0, r0, r8, lsr #6
 80029ec:	2000000c 	andcs	r0, r0, ip
 80029f0:	08007078 	stmdaeq	r0, {r3, r4, r5, r6, ip, sp, lr}

080029f4 <onDoseAlarmMenuGetOption>:
 80029f4:	4608      	mov	r0, r1
 80029f6:	4919      	ldr	r1, [pc, #100]	; (8002a5c <onDoseAlarmMenuGetOption+0x68>)
 80029f8:	b570      	push	{r4, r5, r6, lr}
 80029fa:	784b      	ldrb	r3, [r1, #1]
 80029fc:	f3c3 0383 	ubfx	r3, r3, #2, #4
 8002a00:	1a1c      	subs	r4, r3, r0
 8002a02:	4263      	negs	r3, r4
 8002a04:	4163      	adcs	r3, r4
 8002a06:	6013      	str	r3, [r2, #0]
 8002a08:	b320      	cbz	r0, 8002a54 <onDoseAlarmMenuGetOption+0x60>
 8002a0a:	2808      	cmp	r0, #8
 8002a0c:	d824      	bhi.n	8002a58 <onDoseAlarmMenuGetOption+0x64>
 8002a0e:	4b14      	ldr	r3, [pc, #80]	; (8002a60 <onDoseAlarmMenuGetOption+0x6c>)
 8002a10:	4d14      	ldr	r5, [pc, #80]	; (8002a64 <onDoseAlarmMenuGetOption+0x70>)
 8002a12:	780c      	ldrb	r4, [r1, #0]
 8002a14:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
 8002a18:	6929      	ldr	r1, [r5, #16]
 8002a1a:	f001 fd3d 	bl	8004498 <__aeabi_fdiv>
 8002a1e:	4912      	ldr	r1, [pc, #72]	; (8002a68 <onDoseAlarmMenuGetOption+0x74>)
 8002a20:	4606      	mov	r6, r0
 8002a22:	4812      	ldr	r0, [pc, #72]	; (8002a6c <onDoseAlarmMenuGetOption+0x78>)
 8002a24:	f003 fb1a 	bl	800605c <strcpy>
 8002a28:	2318      	movs	r3, #24
 8002a2a:	f3c4 0441 	ubfx	r4, r4, #1, #2
 8002a2e:	fb03 5404 	mla	r4, r3, r4, r5
 8002a32:	4630      	mov	r0, r6
 8002a34:	6921      	ldr	r1, [r4, #16]
 8002a36:	f001 fd2d 	bl	8004494 <__aeabi_fmul>
 8002a3a:	f994 5014 	ldrsb.w	r5, [r4, #20]
 8002a3e:	4601      	mov	r1, r0
 8002a40:	462a      	mov	r2, r5
 8002a42:	480a      	ldr	r0, [pc, #40]	; (8002a6c <onDoseAlarmMenuGetOption+0x78>)
 8002a44:	f7fe f8e0 	bl	8000c08 <strcatFloatWithMetricPrefix>
 8002a48:	68e1      	ldr	r1, [r4, #12]
 8002a4a:	4808      	ldr	r0, [pc, #32]	; (8002a6c <onDoseAlarmMenuGetOption+0x78>)
 8002a4c:	f003 faf7 	bl	800603e <strcat>
 8002a50:	4806      	ldr	r0, [pc, #24]	; (8002a6c <onDoseAlarmMenuGetOption+0x78>)
 8002a52:	bd70      	pop	{r4, r5, r6, pc}
 8002a54:	4806      	ldr	r0, [pc, #24]	; (8002a70 <onDoseAlarmMenuGetOption+0x7c>)
 8002a56:	e7fc      	b.n	8002a52 <onDoseAlarmMenuGetOption+0x5e>
 8002a58:	2000      	movs	r0, #0
 8002a5a:	e7fa      	b.n	8002a52 <onDoseAlarmMenuGetOption+0x5e>
 8002a5c:	20000da0 	andcs	r0, r0, r0, lsr #27
 8002a60:	08007078 	stmdaeq	r0, {r3, r4, r5, r6, ip, sp, lr}
 8002a64:	2000000c 	andcs	r0, r0, ip
 8002a68:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}
 8002a6c:	20000d7c 	andcs	r0, r0, ip, ror sp
 8002a70:	08006184 	stmdaeq	r0, {r2, r7, r8, sp, lr}

08002a74 <onRateAlarmMenuGetOption>:
 8002a74:	4608      	mov	r0, r1
 8002a76:	4919      	ldr	r1, [pc, #100]	; (8002adc <onRateAlarmMenuGetOption+0x68>)
 8002a78:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8002a7a:	880b      	ldrh	r3, [r1, #0]
 8002a7c:	f3c3 1383 	ubfx	r3, r3, #6, #4
 8002a80:	1a1c      	subs	r4, r3, r0
 8002a82:	4263      	negs	r3, r4
 8002a84:	4163      	adcs	r3, r4
 8002a86:	6013      	str	r3, [r2, #0]
 8002a88:	b320      	cbz	r0, 8002ad4 <onRateAlarmMenuGetOption+0x60>
 8002a8a:	2808      	cmp	r0, #8
 8002a8c:	d824      	bhi.n	8002ad8 <onRateAlarmMenuGetOption+0x64>
 8002a8e:	4b14      	ldr	r3, [pc, #80]	; (8002ae0 <onRateAlarmMenuGetOption+0x6c>)
 8002a90:	4d14      	ldr	r5, [pc, #80]	; (8002ae4 <onRateAlarmMenuGetOption+0x70>)
 8002a92:	780c      	ldrb	r4, [r1, #0]
 8002a94:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
 8002a98:	6869      	ldr	r1, [r5, #4]
 8002a9a:	f001 fcfd 	bl	8004498 <__aeabi_fdiv>
 8002a9e:	f3c4 0641 	ubfx	r6, r4, #1, #2
 8002aa2:	2418      	movs	r4, #24
 8002aa4:	4607      	mov	r7, r0
 8002aa6:	4374      	muls	r4, r6
 8002aa8:	490f      	ldr	r1, [pc, #60]	; (8002ae8 <onRateAlarmMenuGetOption+0x74>)
 8002aaa:	4810      	ldr	r0, [pc, #64]	; (8002aec <onRateAlarmMenuGetOption+0x78>)
 8002aac:	f003 fad6 	bl	800605c <strcpy>
 8002ab0:	192b      	adds	r3, r5, r4
 8002ab2:	6859      	ldr	r1, [r3, #4]
 8002ab4:	4638      	mov	r0, r7
 8002ab6:	f993 6008 	ldrsb.w	r6, [r3, #8]
 8002aba:	f001 fceb 	bl	8004494 <__aeabi_fmul>
 8002abe:	4632      	mov	r2, r6
 8002ac0:	4601      	mov	r1, r0
 8002ac2:	480a      	ldr	r0, [pc, #40]	; (8002aec <onRateAlarmMenuGetOption+0x78>)
 8002ac4:	f7fe f8a0 	bl	8000c08 <strcatFloatWithMetricPrefix>
 8002ac8:	5929      	ldr	r1, [r5, r4]
 8002aca:	4808      	ldr	r0, [pc, #32]	; (8002aec <onRateAlarmMenuGetOption+0x78>)
 8002acc:	f003 fab7 	bl	800603e <strcat>
 8002ad0:	4806      	ldr	r0, [pc, #24]	; (8002aec <onRateAlarmMenuGetOption+0x78>)
 8002ad2:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 8002ad4:	4806      	ldr	r0, [pc, #24]	; (8002af0 <onRateAlarmMenuGetOption+0x7c>)
 8002ad6:	e7fc      	b.n	8002ad2 <onRateAlarmMenuGetOption+0x5e>
 8002ad8:	2000      	movs	r0, #0
 8002ada:	e7fa      	b.n	8002ad2 <onRateAlarmMenuGetOption+0x5e>
 8002adc:	20000da0 	andcs	r0, r0, r0, lsr #27
 8002ae0:	08007144 	stmdaeq	r0, {r2, r6, r8, ip, sp, lr}
 8002ae4:	2000000c 	andcs	r0, r0, ip
 8002ae8:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}
 8002aec:	20000d7c 	andcs	r0, r0, ip, ror sp
 8002af0:	08006184 	stmdaeq	r0, {r2, r7, r8, sp, lr}

08002af4 <getDeadTimeCompensationFactor>:
 8002af4:	4b10      	ldr	r3, [pc, #64]	; (8002b38 <getDeadTimeCompensationFactor+0x44>)
 8002af6:	b510      	push	{r4, lr}
 8002af8:	78db      	ldrb	r3, [r3, #3]
 8002afa:	4604      	mov	r4, r0
 8002afc:	069b      	lsls	r3, r3, #26
 8002afe:	d015      	beq.n	8002b2c <getDeadTimeCompensationFactor+0x38>
 8002b00:	f002 fb28 	bl	8005154 <getTubeDeadTimeCompensation>
 8002b04:	4601      	mov	r1, r0
 8002b06:	4620      	mov	r0, r4
 8002b08:	f001 fcc4 	bl	8004494 <__aeabi_fmul>
 8002b0c:	4601      	mov	r1, r0
 8002b0e:	f04f 507e 	mov.w	r0, #1065353216	; 0x3f800000
 8002b12:	f001 fcbd 	bl	8004490 <__aeabi_fsub>
 8002b16:	4909      	ldr	r1, [pc, #36]	; (8002b3c <getDeadTimeCompensationFactor+0x48>)
 8002b18:	4604      	mov	r4, r0
 8002b1a:	f001 fcdc 	bl	80044d6 <__aeabi_fcmpgt>
 8002b1e:	b140      	cbz	r0, 8002b32 <getDeadTimeCompensationFactor+0x3e>
 8002b20:	4621      	mov	r1, r4
 8002b22:	f04f 507e 	mov.w	r0, #1065353216	; 0x3f800000
 8002b26:	f001 fcb7 	bl	8004498 <__aeabi_fdiv>
 8002b2a:	bd10      	pop	{r4, pc}
 8002b2c:	f04f 507e 	mov.w	r0, #1065353216	; 0x3f800000
 8002b30:	e7fb      	b.n	8002b2a <getDeadTimeCompensationFactor+0x36>
 8002b32:	4803      	ldr	r0, [pc, #12]	; (8002b40 <getDeadTimeCompensationFactor+0x4c>)
 8002b34:	e7f9      	b.n	8002b2a <getDeadTimeCompensationFactor+0x36>
 8002b36:	bf00      	nop
 8002b38:	20000da0 	andcs	r0, r0, r0, lsr #27
 8002b3c:	3dcccccd 	stclcc	12, cr12, [ip, #820]	; 0x334
 8002b40:	41200000 			; <UNDEFINED> instruction: 0x41200000

08002b44 <calculateRate>:
 8002b44:	e92d 41f3 	stmdb	sp!, {r0, r1, r4, r5, r6, r7, r8, lr}
 8002b48:	4614      	mov	r4, r2
 8002b4a:	e9cd 1000 	strd	r1, r0, [sp]
 8002b4e:	b929      	cbnz	r1, 8002b5c <calculateRate+0x18>
 8002b50:	2300      	movs	r3, #0
 8002b52:	6053      	str	r3, [r2, #4]
 8002b54:	6093      	str	r3, [r2, #8]
 8002b56:	b002      	add	sp, #8
 8002b58:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8002b5c:	4281      	cmp	r1, r0
 8002b5e:	d207      	bcs.n	8002b70 <calculateRate+0x2c>
 8002b60:	2101      	movs	r1, #1
 8002b62:	a801      	add	r0, sp, #4
 8002b64:	f7fd fd9b 	bl	800069e <addClamped>
 8002b68:	2101      	movs	r1, #1
 8002b6a:	4668      	mov	r0, sp
 8002b6c:	f7fd fd97 	bl	800069e <addClamped>
 8002b70:	9d01      	ldr	r5, [sp, #4]
 8002b72:	3d01      	subs	r5, #1
 8002b74:	4628      	mov	r0, r5
 8002b76:	f001 fc91 	bl	800449c <__aeabi_ui2f>
 8002b7a:	4919      	ldr	r1, [pc, #100]	; (8002be0 <calculateRate+0x9c>)
 8002b7c:	f001 fc8a 	bl	8004494 <__aeabi_fmul>
 8002b80:	4606      	mov	r6, r0
 8002b82:	9800      	ldr	r0, [sp, #0]
 8002b84:	f001 fc8a 	bl	800449c <__aeabi_ui2f>
 8002b88:	4601      	mov	r1, r0
 8002b8a:	4630      	mov	r0, r6
 8002b8c:	f001 fc84 	bl	8004498 <__aeabi_fdiv>
 8002b90:	4606      	mov	r6, r0
 8002b92:	4628      	mov	r0, r5
 8002b94:	f7fd fd8c 	bl	80006b0 <getConfidenceInterval>
 8002b98:	4607      	mov	r7, r0
 8002b9a:	4630      	mov	r0, r6
 8002b9c:	f7ff ffaa 	bl	8002af4 <getDeadTimeCompensationFactor>
 8002ba0:	f04f 517e 	mov.w	r1, #1065353216	; 0x3f800000
 8002ba4:	4605      	mov	r5, r0
 8002ba6:	4638      	mov	r0, r7
 8002ba8:	f001 fc70 	bl	800448c <__aeabi_fadd>
 8002bac:	4631      	mov	r1, r6
 8002bae:	4607      	mov	r7, r0
 8002bb0:	f001 fc70 	bl	8004494 <__aeabi_fmul>
 8002bb4:	f7ff ff9e 	bl	8002af4 <getDeadTimeCompensationFactor>
 8002bb8:	4629      	mov	r1, r5
 8002bba:	4680      	mov	r8, r0
 8002bbc:	4630      	mov	r0, r6
 8002bbe:	f001 fc69 	bl	8004494 <__aeabi_fmul>
 8002bc2:	4629      	mov	r1, r5
 8002bc4:	6060      	str	r0, [r4, #4]
 8002bc6:	4640      	mov	r0, r8
 8002bc8:	f001 fc66 	bl	8004498 <__aeabi_fdiv>
 8002bcc:	4639      	mov	r1, r7
 8002bce:	f001 fc61 	bl	8004494 <__aeabi_fmul>
 8002bd2:	f04f 517e 	mov.w	r1, #1065353216	; 0x3f800000
 8002bd6:	f001 fc5b 	bl	8004490 <__aeabi_fsub>
 8002bda:	60a0      	str	r0, [r4, #8]
 8002bdc:	e7bb      	b.n	8002b56 <calculateRate+0x12>
 8002bde:	bf00      	nop
 8002be0:	447a0000 	ldrbtmi	r0, [sl], #-0

08002be4 <updateMeasurementUnits>:
 8002be4:	b538      	push	{r3, r4, r5, lr}
 8002be6:	f002 fa97 	bl	8005118 <getTubeConversionFactor>
 8002bea:	4605      	mov	r5, r0
 8002bec:	4601      	mov	r1, r0
 8002bee:	480a      	ldr	r0, [pc, #40]	; (8002c18 <updateMeasurementUnits+0x34>)
 8002bf0:	f001 fc52 	bl	8004498 <__aeabi_fdiv>
 8002bf4:	4c09      	ldr	r4, [pc, #36]	; (8002c1c <updateMeasurementUnits+0x38>)
 8002bf6:	4629      	mov	r1, r5
 8002bf8:	6060      	str	r0, [r4, #4]
 8002bfa:	4809      	ldr	r0, [pc, #36]	; (8002c20 <updateMeasurementUnits+0x3c>)
 8002bfc:	f001 fc4c 	bl	8004498 <__aeabi_fdiv>
 8002c00:	4629      	mov	r1, r5
 8002c02:	6120      	str	r0, [r4, #16]
 8002c04:	4807      	ldr	r0, [pc, #28]	; (8002c24 <updateMeasurementUnits+0x40>)
 8002c06:	f001 fc47 	bl	8004498 <__aeabi_fdiv>
 8002c0a:	4629      	mov	r1, r5
 8002c0c:	61e0      	str	r0, [r4, #28]
 8002c0e:	4806      	ldr	r0, [pc, #24]	; (8002c28 <updateMeasurementUnits+0x44>)
 8002c10:	f001 fc42 	bl	8004498 <__aeabi_fdiv>
 8002c14:	62a0      	str	r0, [r4, #40]	; 0x28
 8002c16:	bd38      	pop	{r3, r4, r5, pc}
 8002c18:	387ba882 	ldmdacc	fp!, {r1, r7, fp, sp, pc}^
 8002c1c:	2000000c 	andcs	r0, r0, ip
 8002c20:	328f2a63 	addcc	r2, pc, #405504	; 0x63000
 8002c24:	3bc49ba6 	blcc	7129ac4 <_Min_Stack_Size+0x71296c4>
 8002c28:	35dfb23b 	ldrbcc	fp, [pc, #571]	; 8002e6b <updateMeasurements+0x19b>

08002c2c <initMeasurements>:
 8002c2c:	b510      	push	{r4, lr}
 8002c2e:	4c10      	ldr	r4, [pc, #64]	; (8002c70 <initMeasurements+0x44>)
 8002c30:	2204      	movs	r2, #4
 8002c32:	7821      	ldrb	r1, [r4, #0]
 8002c34:	480f      	ldr	r0, [pc, #60]	; (8002c74 <initMeasurements+0x48>)
 8002c36:	f3c1 0141 	ubfx	r1, r1, #1, #2
 8002c3a:	f000 fd17 	bl	800366c <selectMenuItem>
 8002c3e:	7821      	ldrb	r1, [r4, #0]
 8002c40:	2205      	movs	r2, #5
 8002c42:	f3c1 01c2 	ubfx	r1, r1, #3, #3
 8002c46:	480c      	ldr	r0, [pc, #48]	; (8002c78 <initMeasurements+0x4c>)
 8002c48:	f000 fd10 	bl	800366c <selectMenuItem>
 8002c4c:	8821      	ldrh	r1, [r4, #0]
 8002c4e:	2209      	movs	r2, #9
 8002c50:	f3c1 1183 	ubfx	r1, r1, #6, #4
 8002c54:	4809      	ldr	r0, [pc, #36]	; (8002c7c <initMeasurements+0x50>)
 8002c56:	f000 fd09 	bl	800366c <selectMenuItem>
 8002c5a:	7861      	ldrb	r1, [r4, #1]
 8002c5c:	2209      	movs	r2, #9
 8002c5e:	f3c1 0183 	ubfx	r1, r1, #2, #4
 8002c62:	4807      	ldr	r0, [pc, #28]	; (8002c80 <initMeasurements+0x54>)
 8002c64:	f000 fd02 	bl	800366c <selectMenuItem>
 8002c68:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8002c6c:	f7ff bfba 	b.w	8002be4 <updateMeasurementUnits>
 8002c70:	20000da0 	andcs	r0, r0, r0, lsr #27
 8002c74:	080071d0 	stmdaeq	r0, {r4, r6, r7, r8, ip, sp, lr}
 8002c78:	0800700c 	stmdaeq	r0, {r2, r3, ip, sp, lr}
 8002c7c:	08007128 	stmdaeq	r0, {r3, r5, r8, ip, sp, lr}
 8002c80:	0800705c 	stmdaeq	r0, {r2, r3, r4, r6, ip, sp, lr}

08002c84 <onMeasurementTick>:
 8002c84:	b538      	push	{r3, r4, r5, lr}
 8002c86:	4605      	mov	r5, r0
 8002c88:	b178      	cbz	r0, 8002caa <onMeasurementTick+0x26>
 8002c8a:	4c08      	ldr	r4, [pc, #32]	; (8002cac <onMeasurementTick+0x28>)
 8002c8c:	68a3      	ldr	r3, [r4, #8]
 8002c8e:	b913      	cbnz	r3, 8002c96 <onMeasurementTick+0x12>
 8002c90:	f7ff f8e2 	bl	8001e58 <getTick>
 8002c94:	6020      	str	r0, [r4, #0]
 8002c96:	f7ff f8df 	bl	8001e58 <getTick>
 8002c9a:	6060      	str	r0, [r4, #4]
 8002c9c:	68a0      	ldr	r0, [r4, #8]
 8002c9e:	4428      	add	r0, r5
 8002ca0:	60a0      	str	r0, [r4, #8]
 8002ca2:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8002ca6:	f7ff b941 	b.w	8001f2c <triggerPulse>
 8002caa:	bd38      	pop	{r3, r4, r5, pc}
 8002cac:	20000328 	andcs	r0, r0, r8, lsr #6

08002cb0 <onMeasurementPeriod>:
 8002cb0:	b510      	push	{r4, lr}
 8002cb2:	f7ff f8d1 	bl	8001e58 <getTick>
 8002cb6:	4b05      	ldr	r3, [pc, #20]	; (8002ccc <onMeasurementPeriod+0x1c>)
 8002cb8:	f103 0410 	add.w	r4, r3, #16
 8002cbc:	60d8      	str	r0, [r3, #12]
 8002cbe:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
 8002cc2:	e884 0007 	stmia.w	r4, {r0, r1, r2}
 8002cc6:	2200      	movs	r2, #0
 8002cc8:	609a      	str	r2, [r3, #8]
 8002cca:	bd10      	pop	{r4, pc}
 8002ccc:	20000328 	andcs	r0, r0, r8, lsr #6

08002cd0 <updateMeasurements>:
 8002cd0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8002cd4:	4cbb      	ldr	r4, [pc, #748]	; (8002fc4 <updateMeasurements+0x2f4>)
 8002cd6:	b089      	sub	sp, #36	; 0x24
 8002cd8:	6923      	ldr	r3, [r4, #16]
 8002cda:	e9d4 6005 	ldrd	r6, r0, [r4, #20]
 8002cde:	9303      	str	r3, [sp, #12]
 8002ce0:	f001 fbdc 	bl	800449c <__aeabi_ui2f>
 8002ce4:	4605      	mov	r5, r0
 8002ce6:	f8d4 01ac 	ldr.w	r0, [r4, #428]	; 0x1ac
 8002cea:	f7ff ff03 	bl	8002af4 <getDeadTimeCompensationFactor>
 8002cee:	4601      	mov	r1, r0
 8002cf0:	4628      	mov	r0, r5
 8002cf2:	f001 fbcf 	bl	8004494 <__aeabi_fmul>
 8002cf6:	69e1      	ldr	r1, [r4, #28]
 8002cf8:	f001 fbc8 	bl	800448c <__aeabi_fadd>
 8002cfc:	4607      	mov	r7, r0
 8002cfe:	f001 fbd6 	bl	80044ae <__aeabi_f2uiz>
 8002d02:	4605      	mov	r5, r0
 8002d04:	f001 fbca 	bl	800449c <__aeabi_ui2f>
 8002d08:	4601      	mov	r1, r0
 8002d0a:	4638      	mov	r0, r7
 8002d0c:	f001 fbc0 	bl	8004490 <__aeabi_fsub>
 8002d10:	61e0      	str	r0, [r4, #28]
 8002d12:	2d00      	cmp	r5, #0
 8002d14:	f000 81a8 	beq.w	8003068 <updateMeasurements+0x398>
 8002d18:	230c      	movs	r3, #12
 8002d1a:	6a62      	ldr	r2, [r4, #36]	; 0x24
 8002d1c:	9903      	ldr	r1, [sp, #12]
 8002d1e:	4353      	muls	r3, r2
 8002d20:	3328      	adds	r3, #40	; 0x28
 8002d22:	18e2      	adds	r2, r4, r3
 8002d24:	50e1      	str	r1, [r4, r3]
 8002d26:	e9c2 6501 	strd	r6, r5, [r2, #4]
 8002d2a:	6a63      	ldr	r3, [r4, #36]	; 0x24
 8002d2c:	6a22      	ldr	r2, [r4, #32]
 8002d2e:	3301      	adds	r3, #1
 8002d30:	f003 031f 	and.w	r3, r3, #31
 8002d34:	4293      	cmp	r3, r2
 8002d36:	6263      	str	r3, [r4, #36]	; 0x24
 8002d38:	bf02      	ittt	eq
 8002d3a:	3301      	addeq	r3, #1
 8002d3c:	f003 031f 	andeq.w	r3, r3, #31
 8002d40:	6223      	streq	r3, [r4, #32]
 8002d42:	4ba1      	ldr	r3, [pc, #644]	; (8002fc8 <updateMeasurements+0x2f8>)
 8002d44:	49a1      	ldr	r1, [pc, #644]	; (8002fcc <updateMeasurements+0x2fc>)
 8002d46:	6858      	ldr	r0, [r3, #4]
 8002d48:	f001 fba4 	bl	8004494 <__aeabi_fmul>
 8002d4c:	f8d4 71ac 	ldr.w	r7, [r4, #428]	; 0x1ac
 8002d50:	4601      	mov	r1, r0
 8002d52:	4681      	mov	r9, r0
 8002d54:	4638      	mov	r0, r7
 8002d56:	f001 fb9d 	bl	8004494 <__aeabi_fmul>
 8002d5a:	499d      	ldr	r1, [pc, #628]	; (8002fd0 <updateMeasurements+0x300>)
 8002d5c:	4680      	mov	r8, r0
 8002d5e:	f001 fbba 	bl	80044d6 <__aeabi_fcmpgt>
 8002d62:	2800      	cmp	r0, #0
 8002d64:	f000 815b 	beq.w	800301e <updateMeasurements+0x34e>
 8002d68:	f04f 517e 	mov.w	r1, #1065353216	; 0x3f800000
 8002d6c:	4640      	mov	r0, r8
 8002d6e:	f001 fba7 	bl	80044c0 <__aeabi_fcmplt>
 8002d72:	2800      	cmp	r0, #0
 8002d74:	f000 8153 	beq.w	800301e <updateMeasurements+0x34e>
 8002d78:	4996      	ldr	r1, [pc, #600]	; (8002fd4 <updateMeasurements+0x304>)
 8002d7a:	4648      	mov	r0, r9
 8002d7c:	f001 fb8a 	bl	8004494 <__aeabi_fmul>
 8002d80:	f001 fbbc 	bl	80044fc <log10f>
 8002d84:	4994      	ldr	r1, [pc, #592]	; (8002fd8 <updateMeasurements+0x308>)
 8002d86:	f001 fb87 	bl	8004498 <__aeabi_fdiv>
 8002d8a:	4601      	mov	r1, r0
 8002d8c:	4640      	mov	r0, r8
 8002d8e:	f001 fbbf 	bl	8004510 <powf>
 8002d92:	4992      	ldr	r1, [pc, #584]	; (8002fdc <updateMeasurements+0x30c>)
 8002d94:	f001 fb7e 	bl	8004494 <__aeabi_fmul>
 8002d98:	f04f 517e 	mov.w	r1, #1065353216	; 0x3f800000
 8002d9c:	f001 fb76 	bl	800448c <__aeabi_fadd>
 8002da0:	4607      	mov	r7, r0
 8002da2:	f04f 517e 	mov.w	r1, #1065353216	; 0x3f800000
 8002da6:	4640      	mov	r0, r8
 8002da8:	f001 fb95 	bl	80044d6 <__aeabi_fcmpgt>
 8002dac:	b128      	cbz	r0, 8002dba <updateMeasurements+0xea>
 8002dae:	498b      	ldr	r1, [pc, #556]	; (8002fdc <updateMeasurements+0x30c>)
 8002db0:	4638      	mov	r0, r7
 8002db2:	f001 fb90 	bl	80044d6 <__aeabi_fcmpgt>
 8002db6:	b100      	cbz	r0, 8002dba <updateMeasurements+0xea>
 8002db8:	4f88      	ldr	r7, [pc, #544]	; (8002fdc <updateMeasurements+0x30c>)
 8002dba:	4638      	mov	r0, r7
 8002dbc:	f001 fb77 	bl	80044ae <__aeabi_f2uiz>
 8002dc0:	f04f 0900 	mov.w	r9, #0
 8002dc4:	9007      	str	r0, [sp, #28]
 8002dc6:	487f      	ldr	r0, [pc, #508]	; (8002fc4 <updateMeasurements+0x2f4>)
 8002dc8:	46b2      	mov	sl, r6
 8002dca:	f04f 0c01 	mov.w	ip, #1
 8002dce:	46c8      	mov	r8, r9
 8002dd0:	4607      	mov	r7, r0
 8002dd2:	6a63      	ldr	r3, [r4, #36]	; 0x24
 8002dd4:	f8d4 e1a8 	ldr.w	lr, [r4, #424]	; 0x1a8
 8002dd8:	9304      	str	r3, [sp, #16]
 8002dda:	68e3      	ldr	r3, [r4, #12]
 8002ddc:	9305      	str	r3, [sp, #20]
 8002dde:	6a23      	ldr	r3, [r4, #32]
 8002de0:	9306      	str	r3, [sp, #24]
 8002de2:	9b04      	ldr	r3, [sp, #16]
 8002de4:	eba3 030c 	sub.w	r3, r3, ip
 8002de8:	f003 031f 	and.w	r3, r3, #31
 8002dec:	9301      	str	r3, [sp, #4]
 8002dee:	230c      	movs	r3, #12
 8002df0:	9a01      	ldr	r2, [sp, #4]
 8002df2:	fb03 0b02 	mla	fp, r3, r2, r0
 8002df6:	f44f 737a 	mov.w	r3, #1000	; 0x3e8
 8002dfa:	f8db 1028 	ldr.w	r1, [fp, #40]	; 0x28
 8002dfe:	9a05      	ldr	r2, [sp, #20]
 8002e00:	1a52      	subs	r2, r2, r1
 8002e02:	fbb2 f2f3 	udiv	r2, r2, r3
 8002e06:	1c53      	adds	r3, r2, #1
 8002e08:	9302      	str	r3, [sp, #8]
 8002e0a:	9a02      	ldr	r2, [sp, #8]
 8002e0c:	9b07      	ldr	r3, [sp, #28]
 8002e0e:	429a      	cmp	r2, r3
 8002e10:	f240 8111 	bls.w	8003036 <updateMeasurements+0x366>
 8002e14:	f1b9 0f00 	cmp.w	r9, #0
 8002e18:	d001      	beq.n	8002e1e <updateMeasurements+0x14e>
 8002e1a:	f8c0 e1a8 	str.w	lr, [r0, #424]	; 0x1a8
 8002e1e:	4651      	mov	r1, sl
 8002e20:	9b01      	ldr	r3, [sp, #4]
 8002e22:	3301      	adds	r3, #1
 8002e24:	f003 031f 	and.w	r3, r3, #31
 8002e28:	623b      	str	r3, [r7, #32]
 8002e2a:	4a6d      	ldr	r2, [pc, #436]	; (8002fe0 <updateMeasurements+0x310>)
 8002e2c:	1a71      	subs	r1, r6, r1
 8002e2e:	4640      	mov	r0, r8
 8002e30:	f7ff fe88 	bl	8002b44 <calculateRate>
 8002e34:	f1b8 0f0a 	cmp.w	r8, #10
 8002e38:	d909      	bls.n	8002e4e <updateMeasurements+0x17e>
 8002e3a:	f8d7 81ac 	ldr.w	r8, [r7, #428]	; 0x1ac
 8002e3e:	f8d7 11b4 	ldr.w	r1, [r7, #436]	; 0x1b4
 8002e42:	4640      	mov	r0, r8
 8002e44:	f001 fb47 	bl	80044d6 <__aeabi_fcmpgt>
 8002e48:	b108      	cbz	r0, 8002e4e <updateMeasurements+0x17e>
 8002e4a:	f8c7 81b4 	str.w	r8, [r7, #436]	; 0x1b4
 8002e4e:	4f5d      	ldr	r7, [pc, #372]	; (8002fc4 <updateMeasurements+0x2f4>)
 8002e50:	f8d7 31d4 	ldr.w	r3, [r7, #468]	; 0x1d4
 8002e54:	3301      	adds	r3, #1
 8002e56:	d036      	beq.n	8002ec6 <updateMeasurements+0x1f6>
 8002e58:	f8d7 31d0 	ldr.w	r3, [r7, #464]	; 0x1d0
 8002e5c:	1c59      	adds	r1, r3, #1
 8002e5e:	d032      	beq.n	8002ec6 <updateMeasurements+0x1f6>
 8002e60:	b19d      	cbz	r5, 8002e8a <updateMeasurements+0x1ba>
 8002e62:	b913      	cbnz	r3, 8002e6a <updateMeasurements+0x19a>
 8002e64:	9b03      	ldr	r3, [sp, #12]
 8002e66:	f8c7 31c8 	str.w	r3, [r7, #456]	; 0x1c8
 8002e6a:	4629      	mov	r1, r5
 8002e6c:	485d      	ldr	r0, [pc, #372]	; (8002fe4 <updateMeasurements+0x314>)
 8002e6e:	f8c7 61cc 	str.w	r6, [r7, #460]	; 0x1cc
 8002e72:	f7fd fc14 	bl	800069e <addClamped>
 8002e76:	f8d7 11cc 	ldr.w	r1, [r7, #460]	; 0x1cc
 8002e7a:	f8d7 31c8 	ldr.w	r3, [r7, #456]	; 0x1c8
 8002e7e:	4a5a      	ldr	r2, [pc, #360]	; (8002fe8 <updateMeasurements+0x318>)
 8002e80:	1ac9      	subs	r1, r1, r3
 8002e82:	f8d7 01d0 	ldr.w	r0, [r7, #464]	; 0x1d0
 8002e86:	f7ff fe5d 	bl	8002b44 <calculateRate>
 8002e8a:	f8d7 31d4 	ldr.w	r3, [r7, #468]	; 0x1d4
 8002e8e:	1c5a      	adds	r2, r3, #1
 8002e90:	d006      	beq.n	8002ea0 <updateMeasurements+0x1d0>
 8002e92:	f8d7 21d0 	ldr.w	r2, [r7, #464]	; 0x1d0
 8002e96:	3201      	adds	r2, #1
 8002e98:	bf1c      	itt	ne
 8002e9a:	3301      	addne	r3, #1
 8002e9c:	f8c7 31d4 	strne.w	r3, [r7, #468]	; 0x1d4
 8002ea0:	4b52      	ldr	r3, [pc, #328]	; (8002fec <updateMeasurements+0x31c>)
 8002ea2:	4953      	ldr	r1, [pc, #332]	; (8002ff0 <updateMeasurements+0x320>)
 8002ea4:	781b      	ldrb	r3, [r3, #0]
 8002ea6:	f8d7 21d4 	ldr.w	r2, [r7, #468]	; 0x1d4
 8002eaa:	f3c3 03c2 	ubfx	r3, r3, #3, #3
 8002eae:	f851 3023 	ldr.w	r3, [r1, r3, lsl #2]
 8002eb2:	429a      	cmp	r2, r3
 8002eb4:	d807      	bhi.n	8002ec6 <updateMeasurements+0x1f6>
 8002eb6:	4b4f      	ldr	r3, [pc, #316]	; (8002ff4 <updateMeasurements+0x324>)
 8002eb8:	e913 0007 	ldmdb	r3, {r0, r1, r2}
 8002ebc:	e883 0007 	stmia.w	r3, {r0, r1, r2}
 8002ec0:	d101      	bne.n	8002ec6 <updateMeasurements+0x1f6>
 8002ec2:	f7ff f857 	bl	8001f74 <triggerAlarm>
 8002ec6:	f8d7 31fc 	ldr.w	r3, [r7, #508]	; 0x1fc
 8002eca:	3301      	adds	r3, #1
 8002ecc:	d00b      	beq.n	8002ee6 <updateMeasurements+0x216>
 8002ece:	f8d7 3200 	ldr.w	r3, [r7, #512]	; 0x200
 8002ed2:	3301      	adds	r3, #1
 8002ed4:	d007      	beq.n	8002ee6 <updateMeasurements+0x216>
 8002ed6:	2101      	movs	r1, #1
 8002ed8:	4847      	ldr	r0, [pc, #284]	; (8002ff8 <updateMeasurements+0x328>)
 8002eda:	f7fd fbe0 	bl	800069e <addClamped>
 8002ede:	4629      	mov	r1, r5
 8002ee0:	4846      	ldr	r0, [pc, #280]	; (8002ffc <updateMeasurements+0x32c>)
 8002ee2:	f7fd fbdc 	bl	800069e <addClamped>
 8002ee6:	f8d7 3210 	ldr.w	r3, [r7, #528]	; 0x210
 8002eea:	3301      	adds	r3, #1
 8002eec:	d00b      	beq.n	8002f06 <updateMeasurements+0x236>
 8002eee:	f8d7 3214 	ldr.w	r3, [r7, #532]	; 0x214
 8002ef2:	3301      	adds	r3, #1
 8002ef4:	d007      	beq.n	8002f06 <updateMeasurements+0x236>
 8002ef6:	2101      	movs	r1, #1
 8002ef8:	4841      	ldr	r0, [pc, #260]	; (8003000 <updateMeasurements+0x330>)
 8002efa:	f7fd fbd0 	bl	800069e <addClamped>
 8002efe:	4629      	mov	r1, r5
 8002f00:	4840      	ldr	r0, [pc, #256]	; (8003004 <updateMeasurements+0x334>)
 8002f02:	f7fd fbcc 	bl	800069e <addClamped>
 8002f06:	2600      	movs	r6, #0
 8002f08:	f8d7 33a8 	ldr.w	r3, [r7, #936]	; 0x3a8
 8002f0c:	f04f 0900 	mov.w	r9, #0
 8002f10:	3301      	adds	r3, #1
 8002f12:	f5b3 7f34 	cmp.w	r3, #720	; 0x2d0
 8002f16:	bf88      	it	hi
 8002f18:	4633      	movhi	r3, r6
 8002f1a:	4d2a      	ldr	r5, [pc, #168]	; (8002fc4 <updateMeasurements+0x2f4>)
 8002f1c:	f8c7 33a8 	str.w	r3, [r7, #936]	; 0x3a8
 8002f20:	f8df 80ec 	ldr.w	r8, [pc, #236]	; 8003010 <updateMeasurements+0x340>
 8002f24:	4f38      	ldr	r7, [pc, #224]	; (8003008 <updateMeasurements+0x338>)
 8002f26:	f8d4 11ac 	ldr.w	r1, [r4, #428]	; 0x1ac
 8002f2a:	f8d5 0218 	ldr.w	r0, [r5, #536]	; 0x218
 8002f2e:	f001 faad 	bl	800448c <__aeabi_fadd>
 8002f32:	f8c5 0218 	str.w	r0, [r5, #536]	; 0x218
 8002f36:	4683      	mov	fp, r0
 8002f38:	f8d5 021c 	ldr.w	r0, [r5, #540]	; 0x21c
 8002f3c:	4b33      	ldr	r3, [pc, #204]	; (800300c <updateMeasurements+0x33c>)
 8002f3e:	3001      	adds	r0, #1
 8002f40:	f8c5 021c 	str.w	r0, [r5, #540]	; 0x21c
 8002f44:	eb03 03c6 	add.w	r3, r3, r6, lsl #3
 8002f48:	889b      	ldrh	r3, [r3, #4]
 8002f4a:	f8d4 23a8 	ldr.w	r2, [r4, #936]	; 0x3a8
 8002f4e:	fbb2 faf3 	udiv	sl, r2, r3
 8002f52:	fb03 2a1a 	mls	sl, r3, sl, r2
 8002f56:	f1ba 0f00 	cmp.w	sl, #0
 8002f5a:	d125      	bne.n	8002fa8 <updateMeasurements+0x2d8>
 8002f5c:	f001 fa9e 	bl	800449c <__aeabi_ui2f>
 8002f60:	4601      	mov	r1, r0
 8002f62:	4658      	mov	r0, fp
 8002f64:	f001 fa98 	bl	8004498 <__aeabi_fdiv>
 8002f68:	4639      	mov	r1, r7
 8002f6a:	f001 fa95 	bl	8004498 <__aeabi_fdiv>
 8002f6e:	f001 fac5 	bl	80044fc <log10f>
 8002f72:	4641      	mov	r1, r8
 8002f74:	f001 fa8e 	bl	8004494 <__aeabi_fmul>
 8002f78:	f001 fa92 	bl	80044a0 <__aeabi_f2iz>
 8002f7c:	2284      	movs	r2, #132	; 0x84
 8002f7e:	fb02 4206 	mla	r2, r2, r6, r4
 8002f82:	f8b5 3220 	ldrh.w	r3, [r5, #544]	; 0x220
 8002f86:	f380 0008 	usat	r0, #8, r0
 8002f8a:	441a      	add	r2, r3
 8002f8c:	3301      	adds	r3, #1
 8002f8e:	b29b      	uxth	r3, r3
 8002f90:	2b77      	cmp	r3, #119	; 0x77
 8002f92:	f882 0222 	strb.w	r0, [r2, #546]	; 0x222
 8002f96:	f8c5 9218 	str.w	r9, [r5, #536]	; 0x218
 8002f9a:	f8c5 a21c 	str.w	sl, [r5, #540]	; 0x21c
 8002f9e:	bf94      	ite	ls
 8002fa0:	f8a5 3220 	strhls.w	r3, [r5, #544]	; 0x220
 8002fa4:	f8a5 a220 	strhhi.w	sl, [r5, #544]	; 0x220
 8002fa8:	3601      	adds	r6, #1
 8002faa:	2e03      	cmp	r6, #3
 8002fac:	f105 0584 	add.w	r5, r5, #132	; 0x84
 8002fb0:	d1b9      	bne.n	8002f26 <updateMeasurements+0x256>
 8002fb2:	f7ff fcd9 	bl	8002968 <isInstantaneousRateAlarm>
 8002fb6:	bb68      	cbnz	r0, 8003014 <updateMeasurements+0x344>
 8002fb8:	f7ff fcf8 	bl	80029ac <isDoseAlarm>
 8002fbc:	2800      	cmp	r0, #0
 8002fbe:	d059      	beq.n	8003074 <updateMeasurements+0x3a4>
 8002fc0:	e028      	b.n	8003014 <updateMeasurements+0x344>
 8002fc2:	bf00      	nop
 8002fc4:	20000328 	andcs	r0, r0, r8, lsr #6
 8002fc8:	2000000c 	andcs	r0, r0, ip
 8002fcc:	49742400 	ldmdbmi	r4!, {sl, sp}^
 8002fd0:	3e99999a 			; <UNDEFINED> instruction: 0x3e99999a
 8002fd4:	413fffff 	teqmi	pc, pc	; <illegal shifter operand>	; <UNPREDICTABLE>
 8002fd8:	bf05db61 	svclt	0x0005db61
 8002fdc:	40a00000 	adcmi	r0, r0, r0
 8002fe0:	200004d0 	ldrdcs	r0, [r0], -r0	; <UNPREDICTABLE>
 8002fe4:	200004f8 	strdcs	r0, [r0], -r8
 8002fe8:	200004fc 	strdcs	r0, [r0], -ip
 8002fec:	20000da0 	andcs	r0, r0, r0, lsr #27
 8002ff0:	08007040 	stmdaeq	r0, {r6, ip, sp, lr}
 8002ff4:	20000508 	andcs	r0, r0, r8, lsl #10
 8002ff8:	20000524 	andcs	r0, r0, r4, lsr #10
 8002ffc:	20000528 	andcs	r0, r0, r8, lsr #10
 8003000:	20000538 	andcs	r0, r0, r8, lsr r5
 8003004:	2000053c 	andcs	r0, r0, ip, lsr r5
 8003008:	3ca3d70a 	stccc	7, cr13, [r3], #40	; 0x28
 800300c:	080070a8 	stmdaeq	r0, {r3, r5, r7, ip, sp, lr}
 8003010:	42200000 	eormi	r0, r0, #0
 8003014:	b009      	add	sp, #36	; 0x24
 8003016:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800301a:	f7fe bfab 	b.w	8001f74 <triggerAlarm>
 800301e:	2100      	movs	r1, #0
 8003020:	4638      	mov	r0, r7
 8003022:	f001 fa46 	bl	80044b2 <__aeabi_fcmpeq>
 8003026:	b920      	cbnz	r0, 8003032 <updateMeasurements+0x362>
 8003028:	4639      	mov	r1, r7
 800302a:	4814      	ldr	r0, [pc, #80]	; (800307c <updateMeasurements+0x3ac>)
 800302c:	f001 fa34 	bl	8004498 <__aeabi_fdiv>
 8003030:	e6b2      	b.n	8002d98 <updateMeasurements+0xc8>
 8003032:	4f13      	ldr	r7, [pc, #76]	; (8003080 <updateMeasurements+0x3b0>)
 8003034:	e6b5      	b.n	8002da2 <updateMeasurements+0xd2>
 8003036:	9b06      	ldr	r3, [sp, #24]
 8003038:	9a01      	ldr	r2, [sp, #4]
 800303a:	f8db e030 	ldr.w	lr, [fp, #48]	; 0x30
 800303e:	4293      	cmp	r3, r2
 8003040:	44f0      	add	r8, lr
 8003042:	d103      	bne.n	800304c <updateMeasurements+0x37c>
 8003044:	9b02      	ldr	r3, [sp, #8]
 8003046:	f8c0 31a8 	str.w	r3, [r0, #424]	; 0x1a8
 800304a:	e6ee      	b.n	8002e2a <updateMeasurements+0x15a>
 800304c:	f10c 0c01 	add.w	ip, ip, #1
 8003050:	f1bc 0f20 	cmp.w	ip, #32
 8003054:	468a      	mov	sl, r1
 8003056:	f8dd e008 	ldr.w	lr, [sp, #8]
 800305a:	f04f 0901 	mov.w	r9, #1
 800305e:	f47f aec0 	bne.w	8002de2 <updateMeasurements+0x112>
 8003062:	f8c0 e1a8 	str.w	lr, [r0, #424]	; 0x1a8
 8003066:	e6e0      	b.n	8002e2a <updateMeasurements+0x15a>
 8003068:	f8d4 31a8 	ldr.w	r3, [r4, #424]	; 0x1a8
 800306c:	3301      	adds	r3, #1
 800306e:	f8c4 31a8 	str.w	r3, [r4, #424]	; 0x1a8
 8003072:	e6ec      	b.n	8002e4e <updateMeasurements+0x17e>
 8003074:	b009      	add	sp, #36	; 0x24
 8003076:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800307a:	bf00      	nop
 800307c:	41900000 	orrsmi	r0, r0, r0
 8003080:	43960000 	orrsmi	r0, r6, #0

08003084 <setMeasurementView>:
 8003084:	b510      	push	{r4, lr}
 8003086:	4c08      	ldr	r4, [pc, #32]	; (80030a8 <setMeasurementView+0x24>)
 8003088:	2800      	cmp	r0, #0
 800308a:	bfa8      	it	ge
 800308c:	f8c4 03ac 	strge.w	r0, [r4, #940]	; 0x3ac
 8003090:	2000      	movs	r0, #0
 8003092:	f7ff fb41 	bl	8002718 <setKeyboardMode>
 8003096:	f8d4 23ac 	ldr.w	r2, [r4, #940]	; 0x3ac
 800309a:	4b04      	ldr	r3, [pc, #16]	; (80030ac <setMeasurementView+0x28>)
 800309c:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 80030a0:	f853 0022 	ldr.w	r0, [r3, r2, lsl #2]
 80030a4:	f002 b8f0 	b.w	8005288 <setView>
 80030a8:	20000328 	andcs	r0, r0, r8, lsr #6
 80030ac:	080070d0 	stmdaeq	r0, {r4, r6, r7, ip, sp, lr}

080030b0 <onMeasurementEvent.isra.5>:
 80030b0:	2802      	cmp	r0, #2
 80030b2:	b508      	push	{r3, lr}
 80030b4:	d00a      	beq.n	80030cc <onMeasurementEvent.isra.5+0x1c>
 80030b6:	2803      	cmp	r0, #3
 80030b8:	d016      	beq.n	80030e8 <onMeasurementEvent.isra.5+0x38>
 80030ba:	2801      	cmp	r0, #1
 80030bc:	d11c      	bne.n	80030f8 <onMeasurementEvent.isra.5+0x48>
 80030be:	f7ff fb2b 	bl	8002718 <setKeyboardMode>
 80030c2:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 80030c6:	480d      	ldr	r0, [pc, #52]	; (80030fc <onMeasurementEvent.isra.5+0x4c>)
 80030c8:	f002 b8de 	b.w	8005288 <setView>
 80030cc:	4b0c      	ldr	r3, [pc, #48]	; (8003100 <onMeasurementEvent.isra.5+0x50>)
 80030ce:	f8d3 23ac 	ldr.w	r2, [r3, #940]	; 0x3ac
 80030d2:	3a01      	subs	r2, #1
 80030d4:	bf48      	it	mi
 80030d6:	2203      	movmi	r2, #3
 80030d8:	f8c3 23ac 	str.w	r2, [r3, #940]	; 0x3ac
 80030dc:	f8d3 03ac 	ldr.w	r0, [r3, #940]	; 0x3ac
 80030e0:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 80030e4:	f7ff bfce 	b.w	8003084 <setMeasurementView>
 80030e8:	4b05      	ldr	r3, [pc, #20]	; (8003100 <onMeasurementEvent.isra.5+0x50>)
 80030ea:	f8d3 23ac 	ldr.w	r2, [r3, #940]	; 0x3ac
 80030ee:	3201      	adds	r2, #1
 80030f0:	2a03      	cmp	r2, #3
 80030f2:	bfc8      	it	gt
 80030f4:	2200      	movgt	r2, #0
 80030f6:	e7ef      	b.n	80030d8 <onMeasurementEvent.isra.5+0x28>
 80030f8:	bd08      	pop	{r3, pc}
 80030fa:	bf00      	nop
 80030fc:	0800753c 	stmdaeq	r0, {r2, r3, r4, r5, r8, sl, ip, sp, lr}
 8003100:	20000328 	andcs	r0, r0, r8, lsr #6

08003104 <onInstantaneousRateViewEvent>:
 8003104:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8003108:	460c      	mov	r4, r1
 800310a:	b0a1      	sub	sp, #132	; 0x84
 800310c:	4608      	mov	r0, r1
 800310e:	f7ff ffcf 	bl	80030b0 <onMeasurementEvent.isra.5>
 8003112:	2c04      	cmp	r4, #4
 8003114:	d017      	beq.n	8003146 <onInstantaneousRateViewEvent+0x42>
 8003116:	2c06      	cmp	r4, #6
 8003118:	d01c      	beq.n	8003154 <onInstantaneousRateViewEvent+0x50>
 800311a:	b98c      	cbnz	r4, 8003140 <onInstantaneousRateViewEvent+0x3c>
 800311c:	4b43      	ldr	r3, [pc, #268]	; (800322c <onInstantaneousRateViewEvent+0x128>)
 800311e:	f893 21b8 	ldrb.w	r2, [r3, #440]	; 0x1b8
 8003122:	f082 0201 	eor.w	r2, r2, #1
 8003126:	f883 21b8 	strb.w	r2, [r3, #440]	; 0x1b8
 800312a:	b13a      	cbz	r2, 800313c <onInstantaneousRateViewEvent+0x38>
 800312c:	f503 74de 	add.w	r4, r3, #444	; 0x1bc
 8003130:	f503 73d4 	add.w	r3, r3, #424	; 0x1a8
 8003134:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
 8003138:	e884 0007 	stmia.w	r4, {r0, r1, r2}
 800313c:	f002 f8ac 	bl	8005298 <updateView>
 8003140:	b021      	add	sp, #132	; 0x84
 8003142:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 8003146:	f44f 72d4 	mov.w	r2, #424	; 0x1a8
 800314a:	2100      	movs	r1, #0
 800314c:	4838      	ldr	r0, [pc, #224]	; (8003230 <onInstantaneousRateViewEvent+0x12c>)
 800314e:	f002 ff6e 	bl	800602e <memset>
 8003152:	e7f3      	b.n	800313c <onInstantaneousRateViewEvent+0x38>
 8003154:	4c35      	ldr	r4, [pc, #212]	; (800322c <onInstantaneousRateViewEvent+0x128>)
 8003156:	f894 31b8 	ldrb.w	r3, [r4, #440]	; 0x1b8
 800315a:	2b00      	cmp	r3, #0
 800315c:	d056      	beq.n	800320c <onInstantaneousRateViewEvent+0x108>
 800315e:	f8d4 51bc 	ldr.w	r5, [r4, #444]	; 0x1bc
 8003162:	f8d4 21c0 	ldr.w	r2, [r4, #448]	; 0x1c0
 8003166:	f8d4 81c4 	ldr.w	r8, [r4, #452]	; 0x1c4
 800316a:	f04f 0b18 	mov.w	fp, #24
 800316e:	4b31      	ldr	r3, [pc, #196]	; (8003234 <onInstantaneousRateViewEvent+0x130>)
 8003170:	f8df a0dc 	ldr.w	sl, [pc, #220]	; 8003250 <onInstantaneousRateViewEvent+0x14c>
 8003174:	781b      	ldrb	r3, [r3, #0]
 8003176:	eb0d 010b 	add.w	r1, sp, fp
 800317a:	f3c3 0341 	ubfx	r3, r3, #1, #2
 800317e:	fb0b a303 	mla	r3, fp, r3, sl
 8003182:	a810      	add	r0, sp, #64	; 0x40
 8003184:	ae08      	add	r6, sp, #32
 8003186:	f7ff fbc3 	bl	8002910 <buildValueString>
 800318a:	492b      	ldr	r1, [pc, #172]	; (8003238 <onInstantaneousRateViewEvent+0x134>)
 800318c:	4630      	mov	r0, r6
 800318e:	f002 ff65 	bl	800605c <strcpy>
 8003192:	af18      	add	r7, sp, #96	; 0x60
 8003194:	4629      	mov	r1, r5
 8003196:	4630      	mov	r0, r6
 8003198:	f7fd fcc2 	bl	8000b20 <strcatTime>
 800319c:	ad0c      	add	r5, sp, #48	; 0x30
 800319e:	4926      	ldr	r1, [pc, #152]	; (8003238 <onInstantaneousRateViewEvent+0x134>)
 80031a0:	4638      	mov	r0, r7
 80031a2:	f002 ff5b 	bl	800605c <strcpy>
 80031a6:	4924      	ldr	r1, [pc, #144]	; (8003238 <onInstantaneousRateViewEvent+0x134>)
 80031a8:	4628      	mov	r0, r5
 80031aa:	f002 ff57 	bl	800605c <strcpy>
 80031ae:	f894 31b8 	ldrb.w	r3, [r4, #440]	; 0x1b8
 80031b2:	bb93      	cbnz	r3, 800321a <onInstantaneousRateViewEvent+0x116>
 80031b4:	f7ff fbd8 	bl	8002968 <isInstantaneousRateAlarm>
 80031b8:	4681      	mov	r9, r0
 80031ba:	bb88      	cbnz	r0, 8003220 <onInstantaneousRateViewEvent+0x11c>
 80031bc:	f8d4 41b4 	ldr.w	r4, [r4, #436]	; 0x1b4
 80031c0:	2100      	movs	r1, #0
 80031c2:	4620      	mov	r0, r4
 80031c4:	f001 f987 	bl	80044d6 <__aeabi_fcmpgt>
 80031c8:	b368      	cbz	r0, 8003226 <onInstantaneousRateViewEvent+0x122>
 80031ca:	4b1a      	ldr	r3, [pc, #104]	; (8003234 <onInstantaneousRateViewEvent+0x130>)
 80031cc:	4622      	mov	r2, r4
 80031ce:	781b      	ldrb	r3, [r3, #0]
 80031d0:	a907      	add	r1, sp, #28
 80031d2:	f3c3 0341 	ubfx	r3, r3, #1, #2
 80031d6:	fb0b a303 	mla	r3, fp, r3, sl
 80031da:	4638      	mov	r0, r7
 80031dc:	f7ff fb98 	bl	8002910 <buildValueString>
 80031e0:	4916      	ldr	r1, [pc, #88]	; (800323c <onInstantaneousRateViewEvent+0x138>)
 80031e2:	4628      	mov	r0, r5
 80031e4:	f002 ff3a 	bl	800605c <strcpy>
 80031e8:	9907      	ldr	r1, [sp, #28]
 80031ea:	4628      	mov	r0, r5
 80031ec:	f002 ff27 	bl	800603e <strcat>
 80031f0:	4648      	mov	r0, r9
 80031f2:	4b13      	ldr	r3, [pc, #76]	; (8003240 <onInstantaneousRateViewEvent+0x13c>)
 80031f4:	e9cd 5003 	strd	r5, r0, [sp, #12]
 80031f8:	e9cd 3701 	strd	r3, r7, [sp, #4]
 80031fc:	9600      	str	r6, [sp, #0]
 80031fe:	4643      	mov	r3, r8
 8003200:	9a06      	ldr	r2, [sp, #24]
 8003202:	a910      	add	r1, sp, #64	; 0x40
 8003204:	480f      	ldr	r0, [pc, #60]	; (8003244 <onInstantaneousRateViewEvent+0x140>)
 8003206:	f7fe fa4d 	bl	80016a4 <drawMeasurement>
 800320a:	e799      	b.n	8003140 <onInstantaneousRateViewEvent+0x3c>
 800320c:	f8d4 51a8 	ldr.w	r5, [r4, #424]	; 0x1a8
 8003210:	f8d4 21ac 	ldr.w	r2, [r4, #428]	; 0x1ac
 8003214:	f8d4 81b0 	ldr.w	r8, [r4, #432]	; 0x1b0
 8003218:	e7a7      	b.n	800316a <onInstantaneousRateViewEvent+0x66>
 800321a:	2002      	movs	r0, #2
 800321c:	4b0a      	ldr	r3, [pc, #40]	; (8003248 <onInstantaneousRateViewEvent+0x144>)
 800321e:	e7e9      	b.n	80031f4 <onInstantaneousRateViewEvent+0xf0>
 8003220:	2001      	movs	r0, #1
 8003222:	4b0a      	ldr	r3, [pc, #40]	; (800324c <onInstantaneousRateViewEvent+0x148>)
 8003224:	e7e6      	b.n	80031f4 <onInstantaneousRateViewEvent+0xf0>
 8003226:	4b04      	ldr	r3, [pc, #16]	; (8003238 <onInstantaneousRateViewEvent+0x134>)
 8003228:	e7e4      	b.n	80031f4 <onInstantaneousRateViewEvent+0xf0>
 800322a:	bf00      	nop
 800322c:	20000328 	andcs	r0, r0, r8, lsr #6
 8003230:	20000348 	andcs	r0, r0, r8, asr #6
 8003234:	20000da0 	andcs	r0, r0, r0, lsr #27
 8003238:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}
 800323c:	08006506 	stmdaeq	r0, {r1, r2, r8, sl, sp, lr}
 8003240:	08007116 	stmdaeq	r0, {r1, r2, r4, r8, ip, sp, lr}
 8003244:	0800711a 	stmdaeq	r0, {r1, r3, r4, r8, ip, sp, lr}
 8003248:	08007106 	stmdaeq	r0, {r1, r2, r8, ip, sp, lr}
 800324c:	0800710b 	stmdaeq	r0, {r0, r1, r3, r8, ip, sp, lr}
 8003250:	2000000c 	andcs	r0, r0, ip

08003254 <onAverageRateViewEvent>:
 8003254:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8003258:	460c      	mov	r4, r1
 800325a:	b094      	sub	sp, #80	; 0x50
 800325c:	4608      	mov	r0, r1
 800325e:	f7ff ff27 	bl	80030b0 <onMeasurementEvent.isra.5>
 8003262:	2c04      	cmp	r4, #4
 8003264:	d021      	beq.n	80032aa <onAverageRateViewEvent+0x56>
 8003266:	2c06      	cmp	r4, #6
 8003268:	d025      	beq.n	80032b6 <onAverageRateViewEvent+0x62>
 800326a:	b9dc      	cbnz	r4, 80032a4 <onAverageRateViewEvent+0x50>
 800326c:	4a3a      	ldr	r2, [pc, #232]	; (8003358 <onAverageRateViewEvent+0x104>)
 800326e:	f892 31ec 	ldrb.w	r3, [r2, #492]	; 0x1ec
 8003272:	b953      	cbnz	r3, 800328a <onAverageRateViewEvent+0x36>
 8003274:	4939      	ldr	r1, [pc, #228]	; (800335c <onAverageRateViewEvent+0x108>)
 8003276:	4c3a      	ldr	r4, [pc, #232]	; (8003360 <onAverageRateViewEvent+0x10c>)
 8003278:	7809      	ldrb	r1, [r1, #0]
 800327a:	f8d2 01d4 	ldr.w	r0, [r2, #468]	; 0x1d4
 800327e:	f3c1 01c2 	ubfx	r1, r1, #3, #3
 8003282:	f854 1021 	ldr.w	r1, [r4, r1, lsl #2]
 8003286:	4288      	cmp	r0, r1
 8003288:	d20c      	bcs.n	80032a4 <onAverageRateViewEvent+0x50>
 800328a:	f083 0301 	eor.w	r3, r3, #1
 800328e:	f882 31ec 	strb.w	r3, [r2, #492]	; 0x1ec
 8003292:	b12b      	cbz	r3, 80032a0 <onAverageRateViewEvent+0x4c>
 8003294:	4b33      	ldr	r3, [pc, #204]	; (8003364 <onAverageRateViewEvent+0x110>)
 8003296:	f1a3 021c 	sub.w	r2, r3, #28
 800329a:	ca07      	ldmia	r2, {r0, r1, r2}
 800329c:	e883 0007 	stmia.w	r3, {r0, r1, r2}
 80032a0:	f001 fffa 	bl	8005298 <updateView>
 80032a4:	b014      	add	sp, #80	; 0x50
 80032a6:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 80032aa:	2234      	movs	r2, #52	; 0x34
 80032ac:	2100      	movs	r1, #0
 80032ae:	482e      	ldr	r0, [pc, #184]	; (8003368 <onAverageRateViewEvent+0x114>)
 80032b0:	f002 febd 	bl	800602e <memset>
 80032b4:	e7f4      	b.n	80032a0 <onAverageRateViewEvent+0x4c>
 80032b6:	4c28      	ldr	r4, [pc, #160]	; (8003358 <onAverageRateViewEvent+0x104>)
 80032b8:	f894 31ec 	ldrb.w	r3, [r4, #492]	; 0x1ec
 80032bc:	2b00      	cmp	r3, #0
 80032be:	d03e      	beq.n	800333e <onAverageRateViewEvent+0xea>
 80032c0:	f8d4 71f0 	ldr.w	r7, [r4, #496]	; 0x1f0
 80032c4:	f8d4 81f4 	ldr.w	r8, [r4, #500]	; 0x1f4
 80032c8:	f8d4 61f8 	ldr.w	r6, [r4, #504]	; 0x1f8
 80032cc:	ad08      	add	r5, sp, #32
 80032ce:	4927      	ldr	r1, [pc, #156]	; (800336c <onAverageRateViewEvent+0x118>)
 80032d0:	4628      	mov	r0, r5
 80032d2:	f002 fec3 	bl	800605c <strcpy>
 80032d6:	4639      	mov	r1, r7
 80032d8:	4628      	mov	r0, r5
 80032da:	f7fd fc21 	bl	8000b20 <strcatTime>
 80032de:	2218      	movs	r2, #24
 80032e0:	4f1e      	ldr	r7, [pc, #120]	; (800335c <onAverageRateViewEvent+0x108>)
 80032e2:	4b23      	ldr	r3, [pc, #140]	; (8003370 <onAverageRateViewEvent+0x11c>)
 80032e4:	7839      	ldrb	r1, [r7, #0]
 80032e6:	a80c      	add	r0, sp, #48	; 0x30
 80032e8:	f3c1 0141 	ubfx	r1, r1, #1, #2
 80032ec:	fb02 3301 	mla	r3, r2, r1, r3
 80032f0:	4642      	mov	r2, r8
 80032f2:	a907      	add	r1, sp, #28
 80032f4:	f7ff fb0c 	bl	8002910 <buildValueString>
 80032f8:	f894 31ec 	ldrb.w	r3, [r4, #492]	; 0x1ec
 80032fc:	4a1b      	ldr	r2, [pc, #108]	; (800336c <onAverageRateViewEvent+0x118>)
 80032fe:	bb2b      	cbnz	r3, 800334c <onAverageRateViewEvent+0xf8>
 8003300:	783b      	ldrb	r3, [r7, #0]
 8003302:	4817      	ldr	r0, [pc, #92]	; (8003360 <onAverageRateViewEvent+0x10c>)
 8003304:	f3c3 03c2 	ubfx	r3, r3, #3, #3
 8003308:	f8d4 11d4 	ldr.w	r1, [r4, #468]	; 0x1d4
 800330c:	f850 3023 	ldr.w	r3, [r0, r3, lsl #2]
 8003310:	4299      	cmp	r1, r3
 8003312:	d21e      	bcs.n	8003352 <onAverageRateViewEvent+0xfe>
 8003314:	f8d4 31d0 	ldr.w	r3, [r4, #464]	; 0x1d0
 8003318:	3301      	adds	r3, #1
 800331a:	bf18      	it	ne
 800331c:	2100      	movne	r1, #0
 800331e:	4b15      	ldr	r3, [pc, #84]	; (8003374 <onAverageRateViewEvent+0x120>)
 8003320:	bf0c      	ite	eq
 8003322:	2102      	moveq	r1, #2
 8003324:	4613      	movne	r3, r2
 8003326:	e9cd 2103 	strd	r2, r1, [sp, #12]
 800332a:	e9cd 3201 	strd	r3, r2, [sp, #4]
 800332e:	9500      	str	r5, [sp, #0]
 8003330:	4633      	mov	r3, r6
 8003332:	9a07      	ldr	r2, [sp, #28]
 8003334:	a90c      	add	r1, sp, #48	; 0x30
 8003336:	4810      	ldr	r0, [pc, #64]	; (8003378 <onAverageRateViewEvent+0x124>)
 8003338:	f7fe f9b4 	bl	80016a4 <drawMeasurement>
 800333c:	e7b2      	b.n	80032a4 <onAverageRateViewEvent+0x50>
 800333e:	f8d4 71e0 	ldr.w	r7, [r4, #480]	; 0x1e0
 8003342:	f8d4 81e4 	ldr.w	r8, [r4, #484]	; 0x1e4
 8003346:	f8d4 61e8 	ldr.w	r6, [r4, #488]	; 0x1e8
 800334a:	e7bf      	b.n	80032cc <onAverageRateViewEvent+0x78>
 800334c:	2102      	movs	r1, #2
 800334e:	4b0b      	ldr	r3, [pc, #44]	; (800337c <onAverageRateViewEvent+0x128>)
 8003350:	e7e9      	b.n	8003326 <onAverageRateViewEvent+0xd2>
 8003352:	2102      	movs	r1, #2
 8003354:	4b0a      	ldr	r3, [pc, #40]	; (8003380 <onAverageRateViewEvent+0x12c>)
 8003356:	e7e6      	b.n	8003326 <onAverageRateViewEvent+0xd2>
 8003358:	20000328 	andcs	r0, r0, r8, lsr #6
 800335c:	20000da0 	andcs	r0, r0, r0, lsr #27
 8003360:	08007040 	stmdaeq	r0, {r6, ip, sp, lr}
 8003364:	20000518 	andcs	r0, r0, r8, lsl r5
 8003368:	200004f0 	strdcs	r0, [r0], -r0	; <UNPREDICTABLE>
 800336c:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}
 8003370:	2000000c 	andcs	r0, r0, ip
 8003374:	080070e5 	stmdaeq	r0, {r0, r2, r5, r6, r7, ip, sp, lr}
 8003378:	080070ee 	stmdaeq	r0, {r1, r2, r3, r5, r6, r7, ip, sp, lr}
 800337c:	08007106 	stmdaeq	r0, {r1, r2, r8, ip, sp, lr}
 8003380:	080070e0 	stmdaeq	r0, {r5, r6, r7, ip, sp, lr}

08003384 <onDoseViewEvent>:
 8003384:	b5f0      	push	{r4, r5, r6, r7, lr}
 8003386:	460c      	mov	r4, r1
 8003388:	b095      	sub	sp, #84	; 0x54
 800338a:	4608      	mov	r0, r1
 800338c:	f7ff fe90 	bl	80030b0 <onMeasurementEvent.isra.5>
 8003390:	2c04      	cmp	r4, #4
 8003392:	d016      	beq.n	80033c2 <onDoseViewEvent+0x3e>
 8003394:	2c06      	cmp	r4, #6
 8003396:	d01a      	beq.n	80033ce <onDoseViewEvent+0x4a>
 8003398:	b98c      	cbnz	r4, 80033be <onDoseViewEvent+0x3a>
 800339a:	4b31      	ldr	r3, [pc, #196]	; (8003460 <onDoseViewEvent+0xdc>)
 800339c:	f893 2204 	ldrb.w	r2, [r3, #516]	; 0x204
 80033a0:	f082 0201 	eor.w	r2, r2, #1
 80033a4:	f883 2204 	strb.w	r2, [r3, #516]	; 0x204
 80033a8:	b13a      	cbz	r2, 80033ba <onDoseViewEvent+0x36>
 80033aa:	f503 7202 	add.w	r2, r3, #520	; 0x208
 80033ae:	f503 73fe 	add.w	r3, r3, #508	; 0x1fc
 80033b2:	e893 0003 	ldmia.w	r3, {r0, r1}
 80033b6:	e882 0003 	stmia.w	r2, {r0, r1}
 80033ba:	f001 ff6d 	bl	8005298 <updateView>
 80033be:	b015      	add	sp, #84	; 0x54
 80033c0:	bdf0      	pop	{r4, r5, r6, r7, pc}
 80033c2:	2214      	movs	r2, #20
 80033c4:	2100      	movs	r1, #0
 80033c6:	4827      	ldr	r0, [pc, #156]	; (8003464 <onDoseViewEvent+0xe0>)
 80033c8:	f002 fe31 	bl	800602e <memset>
 80033cc:	e7f5      	b.n	80033ba <onDoseViewEvent+0x36>
 80033ce:	4c24      	ldr	r4, [pc, #144]	; (8003460 <onDoseViewEvent+0xdc>)
 80033d0:	f894 3204 	ldrb.w	r3, [r4, #516]	; 0x204
 80033d4:	2b00      	cmp	r3, #0
 80033d6:	d138      	bne.n	800344a <onDoseViewEvent+0xc6>
 80033d8:	f8d4 51fc 	ldr.w	r5, [r4, #508]	; 0x1fc
 80033dc:	f8d4 7200 	ldr.w	r7, [r4, #512]	; 0x200
 80033e0:	ae08      	add	r6, sp, #32
 80033e2:	4921      	ldr	r1, [pc, #132]	; (8003468 <onDoseViewEvent+0xe4>)
 80033e4:	4630      	mov	r0, r6
 80033e6:	f002 fe39 	bl	800605c <strcpy>
 80033ea:	4629      	mov	r1, r5
 80033ec:	4630      	mov	r0, r6
 80033ee:	f7fd fb97 	bl	8000b20 <strcatTime>
 80033f2:	2218      	movs	r2, #24
 80033f4:	4b1d      	ldr	r3, [pc, #116]	; (800346c <onDoseViewEvent+0xe8>)
 80033f6:	4638      	mov	r0, r7
 80033f8:	781d      	ldrb	r5, [r3, #0]
 80033fa:	4b1d      	ldr	r3, [pc, #116]	; (8003470 <onDoseViewEvent+0xec>)
 80033fc:	f3c5 0541 	ubfx	r5, r5, #1, #2
 8003400:	fb02 3505 	mla	r5, r2, r5, r3
 8003404:	f001 f84a 	bl	800449c <__aeabi_ui2f>
 8003408:	350c      	adds	r5, #12
 800340a:	462b      	mov	r3, r5
 800340c:	4602      	mov	r2, r0
 800340e:	a907      	add	r1, sp, #28
 8003410:	a80c      	add	r0, sp, #48	; 0x30
 8003412:	f7ff fa7d 	bl	8002910 <buildValueString>
 8003416:	f894 3204 	ldrb.w	r3, [r4, #516]	; 0x204
 800341a:	4c13      	ldr	r4, [pc, #76]	; (8003468 <onDoseViewEvent+0xe4>)
 800341c:	b9d3      	cbnz	r3, 8003454 <onDoseViewEvent+0xd0>
 800341e:	3701      	adds	r7, #1
 8003420:	d01b      	beq.n	800345a <onDoseViewEvent+0xd6>
 8003422:	f7ff fac3 	bl	80029ac <isDoseAlarm>
 8003426:	2800      	cmp	r0, #0
 8003428:	4b12      	ldr	r3, [pc, #72]	; (8003474 <onDoseViewEvent+0xf0>)
 800342a:	bf0a      	itet	eq
 800342c:	2200      	moveq	r2, #0
 800342e:	2201      	movne	r2, #1
 8003430:	4623      	moveq	r3, r4
 8003432:	e9cd 4203 	strd	r4, r2, [sp, #12]
 8003436:	e9cd 3401 	strd	r3, r4, [sp, #4]
 800343a:	9600      	str	r6, [sp, #0]
 800343c:	2300      	movs	r3, #0
 800343e:	9a07      	ldr	r2, [sp, #28]
 8003440:	a90c      	add	r1, sp, #48	; 0x30
 8003442:	480d      	ldr	r0, [pc, #52]	; (8003478 <onDoseViewEvent+0xf4>)
 8003444:	f7fe f92e 	bl	80016a4 <drawMeasurement>
 8003448:	e7b9      	b.n	80033be <onDoseViewEvent+0x3a>
 800344a:	f8d4 5208 	ldr.w	r5, [r4, #520]	; 0x208
 800344e:	f8d4 720c 	ldr.w	r7, [r4, #524]	; 0x20c
 8003452:	e7c5      	b.n	80033e0 <onDoseViewEvent+0x5c>
 8003454:	2202      	movs	r2, #2
 8003456:	4b09      	ldr	r3, [pc, #36]	; (800347c <onDoseViewEvent+0xf8>)
 8003458:	e7eb      	b.n	8003432 <onDoseViewEvent+0xae>
 800345a:	2202      	movs	r2, #2
 800345c:	4b08      	ldr	r3, [pc, #32]	; (8003480 <onDoseViewEvent+0xfc>)
 800345e:	e7e8      	b.n	8003432 <onDoseViewEvent+0xae>
 8003460:	20000328 	andcs	r0, r0, r8, lsr #6
 8003464:	20000524 	andcs	r0, r0, r4, lsr #10
 8003468:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}
 800346c:	20000da0 	andcs	r0, r0, r0, lsr #27
 8003470:	2000000c 	andcs	r0, r0, ip
 8003474:	080070f6 	stmdaeq	r0, {r1, r2, r4, r5, r6, r7, ip, sp, lr}
 8003478:	08007101 	stmdaeq	r0, {r0, r8, ip, sp, lr}
 800347c:	08007106 	stmdaeq	r0, {r1, r2, r8, ip, sp, lr}
 8003480:	080070e5 	stmdaeq	r0, {r0, r2, r5, r6, r7, ip, sp, lr}

08003484 <onHistoryViewEvent>:
 8003484:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 8003488:	460c      	mov	r4, r1
 800348a:	b0a8      	sub	sp, #160	; 0xa0
 800348c:	4608      	mov	r0, r1
 800348e:	f7ff fe0f 	bl	80030b0 <onMeasurementEvent.isra.5>
 8003492:	2c04      	cmp	r4, #4
 8003494:	d010      	beq.n	80034b8 <onHistoryViewEvent+0x34>
 8003496:	2c06      	cmp	r4, #6
 8003498:	d01a      	beq.n	80034d0 <onHistoryViewEvent+0x4c>
 800349a:	b954      	cbnz	r4, 80034b2 <onHistoryViewEvent+0x2e>
 800349c:	4a52      	ldr	r2, [pc, #328]	; (80035e8 <onHistoryViewEvent+0x164>)
 800349e:	f8d2 33a4 	ldr.w	r3, [r2, #932]	; 0x3a4
 80034a2:	3301      	adds	r3, #1
 80034a4:	2b02      	cmp	r3, #2
 80034a6:	bf88      	it	hi
 80034a8:	4623      	movhi	r3, r4
 80034aa:	f8c2 33a4 	str.w	r3, [r2, #932]	; 0x3a4
 80034ae:	f001 fef3 	bl	8005298 <updateView>
 80034b2:	b028      	add	sp, #160	; 0xa0
 80034b4:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 80034b8:	4c4b      	ldr	r4, [pc, #300]	; (80035e8 <onHistoryViewEvent+0x164>)
 80034ba:	f44f 72c6 	mov.w	r2, #396	; 0x18c
 80034be:	2100      	movs	r1, #0
 80034c0:	f504 7006 	add.w	r0, r4, #536	; 0x218
 80034c4:	f002 fdb3 	bl	800602e <memset>
 80034c8:	2300      	movs	r3, #0
 80034ca:	f8c4 33a8 	str.w	r3, [r4, #936]	; 0x3a8
 80034ce:	e7ee      	b.n	80034ae <onHistoryViewEvent+0x2a>
 80034d0:	2400      	movs	r4, #0
 80034d2:	2284      	movs	r2, #132	; 0x84
 80034d4:	26ff      	movs	r6, #255	; 0xff
 80034d6:	4623      	mov	r3, r4
 80034d8:	4d43      	ldr	r5, [pc, #268]	; (80035e8 <onHistoryViewEvent+0x164>)
 80034da:	f8d5 73a4 	ldr.w	r7, [r5, #932]	; 0x3a4
 80034de:	437a      	muls	r2, r7
 80034e0:	18d1      	adds	r1, r2, r3
 80034e2:	4429      	add	r1, r5
 80034e4:	f891 1222 	ldrb.w	r1, [r1, #546]	; 0x222
 80034e8:	b129      	cbz	r1, 80034f6 <onHistoryViewEvent+0x72>
 80034ea:	428c      	cmp	r4, r1
 80034ec:	bf38      	it	cc
 80034ee:	460c      	movcc	r4, r1
 80034f0:	428e      	cmp	r6, r1
 80034f2:	bf28      	it	cs
 80034f4:	460e      	movcs	r6, r1
 80034f6:	3301      	adds	r3, #1
 80034f8:	2b78      	cmp	r3, #120	; 0x78
 80034fa:	d1f1      	bne.n	80034e0 <onHistoryViewEvent+0x5c>
 80034fc:	2200      	movs	r2, #0
 80034fe:	e9cd 2202 	strd	r2, r2, [sp, #8]
 8003502:	e9cd 2204 	strd	r2, r2, [sp, #16]
 8003506:	e9cd 2206 	strd	r2, r2, [sp, #24]
 800350a:	e9cd 2208 	strd	r2, r2, [sp, #32]
 800350e:	2c00      	cmp	r4, #0
 8003510:	d067      	beq.n	80035e2 <onHistoryViewEvent+0x15e>
 8003512:	4b36      	ldr	r3, [pc, #216]	; (80035ec <onHistoryViewEvent+0x168>)
 8003514:	f8df 90e4 	ldr.w	r9, [pc, #228]	; 80035fc <onHistoryViewEvent+0x178>
 8003518:	f893 8000 	ldrb.w	r8, [r3]
 800351c:	4934      	ldr	r1, [pc, #208]	; (80035f0 <onHistoryViewEvent+0x16c>)
 800351e:	f3c8 0341 	ubfx	r3, r8, #1, #2
 8003522:	f04f 0818 	mov.w	r8, #24
 8003526:	fb08 f803 	mul.w	r8, r8, r3
 800352a:	eb09 0308 	add.w	r3, r9, r8
 800352e:	6858      	ldr	r0, [r3, #4]
 8003530:	f000 ffb0 	bl	8004494 <__aeabi_fmul>
 8003534:	f000 ffe2 	bl	80044fc <log10f>
 8003538:	f04f 4183 	mov.w	r1, #1098907648	; 0x41800000
 800353c:	f000 ffa6 	bl	800448c <__aeabi_fadd>
 8003540:	492c      	ldr	r1, [pc, #176]	; (80035f4 <onHistoryViewEvent+0x170>)
 8003542:	f000 ffa7 	bl	8004494 <__aeabi_fmul>
 8003546:	f000 ffb2 	bl	80044ae <__aeabi_f2uiz>
 800354a:	1901      	adds	r1, r0, r4
 800354c:	2428      	movs	r4, #40	; 0x28
 800354e:	1983      	adds	r3, r0, r6
 8003550:	fbb3 f3f4 	udiv	r3, r3, r4
 8003554:	fbb1 f1f4 	udiv	r1, r1, r4
 8003558:	435c      	muls	r4, r3
 800355a:	f1a3 0a10 	sub.w	sl, r3, #16
 800355e:	390f      	subs	r1, #15
 8003560:	1a24      	subs	r4, r4, r0
 8003562:	a806      	add	r0, sp, #24
 8003564:	eba1 060a 	sub.w	r6, r1, sl
 8003568:	f7fd fb96 	bl	8000c98 <strcatDecimalPowerWithMetricPrefix>
 800356c:	f859 1008 	ldr.w	r1, [r9, r8]
 8003570:	a806      	add	r0, sp, #24
 8003572:	f002 fd64 	bl	800603e <strcat>
 8003576:	4651      	mov	r1, sl
 8003578:	a802      	add	r0, sp, #8
 800357a:	f7fd fb8d 	bl	8000c98 <strcatDecimalPowerWithMetricPrefix>
 800357e:	f859 1008 	ldr.w	r1, [r9, r8]
 8003582:	a802      	add	r0, sp, #8
 8003584:	f002 fd5b 	bl	800603e <strcat>
 8003588:	2384      	movs	r3, #132	; 0x84
 800358a:	f04f 0c28 	mov.w	ip, #40	; 0x28
 800358e:	2000      	movs	r0, #0
 8003590:	fb03 5307 	mla	r3, r3, r7, r5
 8003594:	fb0c fc06 	mul.w	ip, ip, r6
 8003598:	f8b3 7220 	ldrh.w	r7, [r3, #544]	; 0x220
 800359c:	4619      	mov	r1, r3
 800359e:	3778      	adds	r7, #120	; 0x78
 80035a0:	ab0a      	add	r3, sp, #40	; 0x28
 80035a2:	183a      	adds	r2, r7, r0
 80035a4:	3a78      	subs	r2, #120	; 0x78
 80035a6:	2a77      	cmp	r2, #119	; 0x77
 80035a8:	d8fc      	bhi.n	80035a4 <onHistoryViewEvent+0x120>
 80035aa:	440a      	add	r2, r1
 80035ac:	f892 2222 	ldrb.w	r2, [r2, #546]	; 0x222
 80035b0:	b122      	cbz	r2, 80035bc <onHistoryViewEvent+0x138>
 80035b2:	1b12      	subs	r2, r2, r4
 80035b4:	ebc2 2202 	rsb	r2, r2, r2, lsl #8
 80035b8:	fbb2 f2fc 	udiv	r2, r2, ip
 80035bc:	541a      	strb	r2, [r3, r0]
 80035be:	3001      	adds	r0, #1
 80035c0:	2878      	cmp	r0, #120	; 0x78
 80035c2:	d1ee      	bne.n	80035a2 <onHistoryViewEvent+0x11e>
 80035c4:	f8d5 43a4 	ldr.w	r4, [r5, #932]	; 0x3a4
 80035c8:	480b      	ldr	r0, [pc, #44]	; (80035f8 <onHistoryViewEvent+0x174>)
 80035ca:	9601      	str	r6, [sp, #4]
 80035cc:	eb00 02c4 	add.w	r2, r0, r4, lsl #3
 80035d0:	7992      	ldrb	r2, [r2, #6]
 80035d2:	a906      	add	r1, sp, #24
 80035d4:	9200      	str	r2, [sp, #0]
 80035d6:	f850 0034 	ldr.w	r0, [r0, r4, lsl #3]
 80035da:	aa02      	add	r2, sp, #8
 80035dc:	f7fe f912 	bl	8001804 <drawHistory>
 80035e0:	e767      	b.n	80034b2 <onHistoryViewEvent+0x2e>
 80035e2:	2601      	movs	r6, #1
 80035e4:	e7d0      	b.n	8003588 <onHistoryViewEvent+0x104>
 80035e6:	bf00      	nop
 80035e8:	20000328 	andcs	r0, r0, r8, lsr #6
 80035ec:	20000da0 	andcs	r0, r0, r0, lsr #27
 80035f0:	3ca3d70a 	stccc	7, cr13, [r3], #40	; 0x28
 80035f4:	42200000 	eormi	r0, r0, #0
 80035f8:	080070a8 	stmdaeq	r0, {r3, r5, r7, ip, sp, lr}
 80035fc:	2000000c 	andcs	r0, r0, ip

08003600 <getInstantaneousRate>:
 8003600:	4b01      	ldr	r3, [pc, #4]	; (8003608 <getInstantaneousRate+0x8>)
 8003602:	f8d3 01ac 	ldr.w	r0, [r3, #428]	; 0x1ac
 8003606:	4770      	bx	lr
 8003608:	20000328 	andcs	r0, r0, r8, lsr #6

0800360c <setDoseTime>:
 800360c:	4b01      	ldr	r3, [pc, #4]	; (8003614 <setDoseTime+0x8>)
 800360e:	f8c3 01fc 	str.w	r0, [r3, #508]	; 0x1fc
 8003612:	4770      	bx	lr
 8003614:	20000328 	andcs	r0, r0, r8, lsr #6

08003618 <getDoseTime>:
 8003618:	4b01      	ldr	r3, [pc, #4]	; (8003620 <getDoseTime+0x8>)
 800361a:	f8d3 01fc 	ldr.w	r0, [r3, #508]	; 0x1fc
 800361e:	4770      	bx	lr
 8003620:	20000328 	andcs	r0, r0, r8, lsr #6

08003624 <setDosePulseCount>:
 8003624:	4b01      	ldr	r3, [pc, #4]	; (800362c <setDosePulseCount+0x8>)
 8003626:	f8c3 0200 	str.w	r0, [r3, #512]	; 0x200
 800362a:	4770      	bx	lr
 800362c:	20000328 	andcs	r0, r0, r8, lsr #6

08003630 <getDosePulseCount>:
 8003630:	4b01      	ldr	r3, [pc, #4]	; (8003638 <getDosePulseCount+0x8>)
 8003632:	f8d3 0200 	ldr.w	r0, [r3, #512]	; 0x200
 8003636:	4770      	bx	lr
 8003638:	20000328 	andcs	r0, r0, r8, lsr #6

0800363c <setTubeTime>:
 800363c:	4b01      	ldr	r3, [pc, #4]	; (8003644 <setTubeTime+0x8>)
 800363e:	f8c3 0210 	str.w	r0, [r3, #528]	; 0x210
 8003642:	4770      	bx	lr
 8003644:	20000328 	andcs	r0, r0, r8, lsr #6

08003648 <getTubeTime>:
 8003648:	4b01      	ldr	r3, [pc, #4]	; (8003650 <getTubeTime+0x8>)
 800364a:	f8d3 0210 	ldr.w	r0, [r3, #528]	; 0x210
 800364e:	4770      	bx	lr
 8003650:	20000328 	andcs	r0, r0, r8, lsr #6

08003654 <setTubePulseCount>:
 8003654:	4b01      	ldr	r3, [pc, #4]	; (800365c <setTubePulseCount+0x8>)
 8003656:	f8c3 0214 	str.w	r0, [r3, #532]	; 0x214
 800365a:	4770      	bx	lr
 800365c:	20000328 	andcs	r0, r0, r8, lsr #6

08003660 <getTubePulseCount>:
 8003660:	4b01      	ldr	r3, [pc, #4]	; (8003668 <getTubePulseCount+0x8>)
 8003662:	f8d3 0214 	ldr.w	r0, [r3, #532]	; 0x214
 8003666:	4770      	bx	lr
 8003668:	20000328 	andcs	r0, r0, r8, lsr #6

0800366c <selectMenuItem>:
 800366c:	b510      	push	{r4, lr}
 800366e:	4c07      	ldr	r4, [pc, #28]	; (800368c <selectMenuItem+0x20>)
 8003670:	6843      	ldr	r3, [r0, #4]
 8003672:	6824      	ldr	r4, [r4, #0]
 8003674:	b288      	uxth	r0, r1
 8003676:	428c      	cmp	r4, r1
 8003678:	8058      	strh	r0, [r3, #2]
 800367a:	d902      	bls.n	8003682 <selectMenuItem+0x16>
 800367c:	2200      	movs	r2, #0
 800367e:	801a      	strh	r2, [r3, #0]
 8003680:	e003      	b.n	800368a <selectMenuItem+0x1e>
 8003682:	1b12      	subs	r2, r2, r4
 8003684:	428a      	cmp	r2, r1
 8003686:	d9fa      	bls.n	800367e <selectMenuItem+0x12>
 8003688:	8018      	strh	r0, [r3, #0]
 800368a:	bd10      	pop	{r4, pc}
 800368c:	08006e54 	stmdaeq	r0, {r2, r4, r6, r9, sl, fp, sp, lr}

08003690 <onMenuEvent>:
 8003690:	b573      	push	{r0, r1, r4, r5, r6, lr}
 8003692:	6846      	ldr	r6, [r0, #4]
 8003694:	2906      	cmp	r1, #6
 8003696:	d809      	bhi.n	80036ac <onMenuEvent+0x1c>
 8003698:	e8df f001 	tbb	[pc, r1]
 800369c:	11110a04 	tstne	r1, r4, lsl #20
 80036a0:	00470808 	subeq	r0, r7, r8, lsl #16
 80036a4:	6933      	ldr	r3, [r6, #16]
 80036a6:	b10b      	cbz	r3, 80036ac <onMenuEvent+0x1c>
 80036a8:	4630      	mov	r0, r6
 80036aa:	4798      	blx	r3
 80036ac:	b002      	add	sp, #8
 80036ae:	bd70      	pop	{r4, r5, r6, pc}
 80036b0:	68f3      	ldr	r3, [r6, #12]
 80036b2:	b10b      	cbz	r3, 80036b8 <onMenuEvent+0x28>
 80036b4:	4630      	mov	r0, r6
 80036b6:	4798      	blx	r3
 80036b8:	f001 fdee 	bl	8005298 <updateView>
 80036bc:	e7f6      	b.n	80036ac <onMenuEvent+0x1c>
 80036be:	6875      	ldr	r5, [r6, #4]
 80036c0:	2902      	cmp	r1, #2
 80036c2:	886c      	ldrh	r4, [r5, #2]
 80036c4:	d11a      	bne.n	80036fc <onMenuEvent+0x6c>
 80036c6:	b14c      	cbz	r4, 80036dc <onMenuEvent+0x4c>
 80036c8:	3c01      	subs	r4, #1
 80036ca:	882b      	ldrh	r3, [r5, #0]
 80036cc:	b2a4      	uxth	r4, r4
 80036ce:	429c      	cmp	r4, r3
 80036d0:	806c      	strh	r4, [r5, #2]
 80036d2:	d2f1      	bcs.n	80036b8 <onMenuEvent+0x28>
 80036d4:	3b01      	subs	r3, #1
 80036d6:	802b      	strh	r3, [r5, #0]
 80036d8:	e7ee      	b.n	80036b8 <onMenuEvent+0x28>
 80036da:	3401      	adds	r4, #1
 80036dc:	68b3      	ldr	r3, [r6, #8]
 80036de:	aa01      	add	r2, sp, #4
 80036e0:	4621      	mov	r1, r4
 80036e2:	4630      	mov	r0, r6
 80036e4:	4798      	blx	r3
 80036e6:	2800      	cmp	r0, #0
 80036e8:	d1f7      	bne.n	80036da <onMenuEvent+0x4a>
 80036ea:	b2a3      	uxth	r3, r4
 80036ec:	1e5a      	subs	r2, r3, #1
 80036ee:	806a      	strh	r2, [r5, #2]
 80036f0:	4a10      	ldr	r2, [pc, #64]	; (8003734 <onMenuEvent+0xa4>)
 80036f2:	6812      	ldr	r2, [r2, #0]
 80036f4:	42a2      	cmp	r2, r4
 80036f6:	d216      	bcs.n	8003726 <onMenuEvent+0x96>
 80036f8:	1a9b      	subs	r3, r3, r2
 80036fa:	e7ec      	b.n	80036d6 <onMenuEvent+0x46>
 80036fc:	68b3      	ldr	r3, [r6, #8]
 80036fe:	aa01      	add	r2, sp, #4
 8003700:	1c61      	adds	r1, r4, #1
 8003702:	4630      	mov	r0, r6
 8003704:	4798      	blx	r3
 8003706:	b168      	cbz	r0, 8003724 <onMenuEvent+0x94>
 8003708:	4a0a      	ldr	r2, [pc, #40]	; (8003734 <onMenuEvent+0xa4>)
 800370a:	886b      	ldrh	r3, [r5, #2]
 800370c:	6812      	ldr	r2, [r2, #0]
 800370e:	8829      	ldrh	r1, [r5, #0]
 8003710:	3301      	adds	r3, #1
 8003712:	3a01      	subs	r2, #1
 8003714:	b29b      	uxth	r3, r3
 8003716:	440a      	add	r2, r1
 8003718:	4293      	cmp	r3, r2
 800371a:	806b      	strh	r3, [r5, #2]
 800371c:	d9cc      	bls.n	80036b8 <onMenuEvent+0x28>
 800371e:	3101      	adds	r1, #1
 8003720:	8029      	strh	r1, [r5, #0]
 8003722:	e7c9      	b.n	80036b8 <onMenuEvent+0x28>
 8003724:	8068      	strh	r0, [r5, #2]
 8003726:	8028      	strh	r0, [r5, #0]
 8003728:	e7c6      	b.n	80036b8 <onMenuEvent+0x28>
 800372a:	4630      	mov	r0, r6
 800372c:	f7fd ff26 	bl	800157c <drawMenu>
 8003730:	e7bc      	b.n	80036ac <onMenuEvent+0x1c>
 8003732:	bf00      	nop
 8003734:	08006e54 	stmdaeq	r0, {r2, r4, r6, r9, sl, fp, sp, lr}

08003738 <onPulseClicksMenuGetOption>:
 8003738:	4b05      	ldr	r3, [pc, #20]	; (8003750 <onPulseClicksMenuGetOption+0x18>)
 800373a:	79db      	ldrb	r3, [r3, #7]
 800373c:	f3c3 1380 	ubfx	r3, r3, #6, #1
 8003740:	1a58      	subs	r0, r3, r1
 8003742:	4243      	negs	r3, r0
 8003744:	4143      	adcs	r3, r0
 8003746:	6013      	str	r3, [r2, #0]
 8003748:	4b02      	ldr	r3, [pc, #8]	; (8003754 <onPulseClicksMenuGetOption+0x1c>)
 800374a:	f853 0021 	ldr.w	r0, [r3, r1, lsl #2]
 800374e:	4770      	bx	lr
 8003750:	20000da0 	andcs	r0, r0, r0, lsr #27
 8003754:	0800723c 	stmdaeq	r0, {r2, r3, r4, r5, r9, ip, sp, lr}

08003758 <onBatteryTypeMenuSelect>:
 8003758:	4b03      	ldr	r3, [pc, #12]	; (8003768 <onBatteryTypeMenuSelect+0x10>)
 800375a:	6841      	ldr	r1, [r0, #4]
 800375c:	79da      	ldrb	r2, [r3, #7]
 800375e:	8849      	ldrh	r1, [r1, #2]
 8003760:	f361 1286 	bfi	r2, r1, #6, #1
 8003764:	71da      	strb	r2, [r3, #7]
 8003766:	4770      	bx	lr
 8003768:	20000da0 	andcs	r0, r0, r0, lsr #27

0800376c <initPower>:
 800376c:	b508      	push	{r3, lr}
 800376e:	f001 f8ad 	bl	80048cc <initPowerController>
 8003772:	4b05      	ldr	r3, [pc, #20]	; (8003788 <initPower+0x1c>)
 8003774:	2202      	movs	r2, #2
 8003776:	79d9      	ldrb	r1, [r3, #7]
 8003778:	4804      	ldr	r0, [pc, #16]	; (800378c <initPower+0x20>)
 800377a:	f3c1 1180 	ubfx	r1, r1, #6, #1
 800377e:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8003782:	f7ff bf73 	b.w	800366c <selectMenuItem>
 8003786:	bf00      	nop
 8003788:	20000da0 	andcs	r0, r0, r0, lsr #27
 800378c:	08007228 	stmdaeq	r0, {r3, r5, r9, ip, sp, lr}

08003790 <requestPowerOff>:
 8003790:	2201      	movs	r2, #1
 8003792:	4b01      	ldr	r3, [pc, #4]	; (8003798 <requestPowerOff+0x8>)
 8003794:	701a      	strb	r2, [r3, #0]
 8003796:	4770      	bx	lr
 8003798:	20000d9c 	mulcs	r0, ip, sp

0800379c <isPowerOffRequested>:
 800379c:	4b02      	ldr	r3, [pc, #8]	; (80037a8 <isPowerOffRequested+0xc>)
 800379e:	7818      	ldrb	r0, [r3, #0]
 80037a0:	b108      	cbz	r0, 80037a6 <isPowerOffRequested+0xa>
 80037a2:	2200      	movs	r2, #0
 80037a4:	701a      	strb	r2, [r3, #0]
 80037a6:	4770      	bx	lr
 80037a8:	20000d9c 	mulcs	r0, ip, sp

080037ac <getDeviceBatteryLevel>:
 80037ac:	b570      	push	{r4, r5, r6, lr}
 80037ae:	f001 f8c3 	bl	8004938 <isBatteryCharging>
 80037b2:	4604      	mov	r4, r0
 80037b4:	b9b8      	cbnz	r0, 80037e6 <getDeviceBatteryLevel+0x3a>
 80037b6:	2214      	movs	r2, #20
 80037b8:	4b0c      	ldr	r3, [pc, #48]	; (80037ec <getDeviceBatteryLevel+0x40>)
 80037ba:	79dd      	ldrb	r5, [r3, #7]
 80037bc:	4b0c      	ldr	r3, [pc, #48]	; (80037f0 <getDeviceBatteryLevel+0x44>)
 80037be:	f3c5 1580 	ubfx	r5, r5, #6, #1
 80037c2:	fb02 3505 	mla	r5, r2, r5, r3
 80037c6:	f000 fe13 	bl	80043f0 <getDeviceBatteryVoltage>
 80037ca:	4606      	mov	r6, r0
 80037cc:	4631      	mov	r1, r6
 80037ce:	f855 0024 	ldr.w	r0, [r5, r4, lsl #2]
 80037d2:	f000 fe80 	bl	80044d6 <__aeabi_fcmpgt>
 80037d6:	b108      	cbz	r0, 80037dc <getDeviceBatteryLevel+0x30>
 80037d8:	b260      	sxtb	r0, r4
 80037da:	bd70      	pop	{r4, r5, r6, pc}
 80037dc:	3401      	adds	r4, #1
 80037de:	2c05      	cmp	r4, #5
 80037e0:	d1f4      	bne.n	80037cc <getDeviceBatteryLevel+0x20>
 80037e2:	4620      	mov	r0, r4
 80037e4:	e7f9      	b.n	80037da <getDeviceBatteryLevel+0x2e>
 80037e6:	f04f 30ff 	mov.w	r0, #4294967295
 80037ea:	e7f6      	b.n	80037da <getDeviceBatteryLevel+0x2e>
 80037ec:	20000da0 	andcs	r0, r0, r0, lsr #27
 80037f0:	08007200 	stmdaeq	r0, {r9, ip, sp, lr}

080037f4 <dequeueBit>:
 80037f4:	4b0e      	ldr	r3, [pc, #56]	; (8003830 <dequeueBit+0x3c>)
 80037f6:	6819      	ldr	r1, [r3, #0]
 80037f8:	685a      	ldr	r2, [r3, #4]
 80037fa:	4291      	cmp	r1, r2
 80037fc:	d014      	beq.n	8003828 <dequeueBit+0x34>
 80037fe:	6859      	ldr	r1, [r3, #4]
 8003800:	685a      	ldr	r2, [r3, #4]
 8003802:	eb03 01d1 	add.w	r1, r3, r1, lsr #3
 8003806:	7a08      	ldrb	r0, [r1, #8]
 8003808:	f002 0107 	and.w	r1, r2, #7
 800380c:	2201      	movs	r2, #1
 800380e:	408a      	lsls	r2, r1
 8003810:	4210      	tst	r0, r2
 8003812:	685a      	ldr	r2, [r3, #4]
 8003814:	bf18      	it	ne
 8003816:	2001      	movne	r0, #1
 8003818:	f102 0201 	add.w	r2, r2, #1
 800381c:	f002 027f 	and.w	r2, r2, #127	; 0x7f
 8003820:	605a      	str	r2, [r3, #4]
 8003822:	bf08      	it	eq
 8003824:	2000      	moveq	r0, #0
 8003826:	4770      	bx	lr
 8003828:	f04f 30ff 	mov.w	r0, #4294967295
 800382c:	4770      	bx	lr
 800382e:	bf00      	nop
 8003830:	200006e4 	andcs	r0, r0, r4, ror #13

08003834 <onRNGMenuGetOption>:
 8003834:	2302      	movs	r3, #2
 8003836:	6013      	str	r3, [r2, #0]
 8003838:	4b01      	ldr	r3, [pc, #4]	; (8003840 <onRNGMenuGetOption+0xc>)
 800383a:	f853 0021 	ldr.w	r0, [r3, r1, lsl #2]
 800383e:	4770      	bx	lr
 8003840:	08007294 	stmdaeq	r0, {r2, r4, r7, r9, ip, sp, lr}

08003844 <onRNGMenuSelect>:
 8003844:	b538      	push	{r3, r4, r5, lr}
 8003846:	6843      	ldr	r3, [r0, #4]
 8003848:	4c0b      	ldr	r4, [pc, #44]	; (8003878 <onRNGMenuSelect+0x34>)
 800384a:	789b      	ldrb	r3, [r3, #2]
 800384c:	4a0b      	ldr	r2, [pc, #44]	; (800387c <onRNGMenuSelect+0x38>)
 800384e:	f884 3025 	strb.w	r3, [r4, #37]	; 0x25
 8003852:	5cd3      	ldrb	r3, [r2, r3]
 8003854:	2501      	movs	r5, #1
 8003856:	62a3      	str	r3, [r4, #40]	; 0x28
 8003858:	2300      	movs	r3, #0
 800385a:	f104 0034 	add.w	r0, r4, #52	; 0x34
 800385e:	4908      	ldr	r1, [pc, #32]	; (8003880 <onRNGMenuSelect+0x3c>)
 8003860:	e9c4 530b 	strd	r5, r3, [r4, #44]	; 0x2c
 8003864:	f002 fbfa 	bl	800605c <strcpy>
 8003868:	f884 5045 	strb.w	r5, [r4, #69]	; 0x45
 800386c:	4805      	ldr	r0, [pc, #20]	; (8003884 <onRNGMenuSelect+0x40>)
 800386e:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8003872:	f001 bd09 	b.w	8005288 <setView>
 8003876:	bf00      	nop
 8003878:	200006e4 	andcs	r0, r0, r4, ror #13
 800387c:	080072c0 	stmdaeq	r0, {r6, r7, r9, ip, sp, lr}
 8003880:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}
 8003884:	080072cc 	stmdaeq	r0, {r2, r3, r6, r7, r9, ip, sp, lr}

08003888 <onRNGEvent>:
 8003888:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 800388c:	b119      	cbz	r1, 8003896 <onRNGEvent+0xe>
 800388e:	2906      	cmp	r1, #6
 8003890:	d006      	beq.n	80038a0 <onRNGEvent+0x18>
 8003892:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8003896:	4838      	ldr	r0, [pc, #224]	; (8003978 <onRNGEvent+0xf0>)
 8003898:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
 800389c:	f001 bcf4 	b.w	8005288 <setView>
 80038a0:	4c36      	ldr	r4, [pc, #216]	; (800397c <onRNGEvent+0xf4>)
 80038a2:	f06f 0833 	mvn.w	r8, #51	; 0x33
 80038a6:	f104 0034 	add.w	r0, r4, #52	; 0x34
 80038aa:	f7fc fe89 	bl	80005c0 <strlen>
 80038ae:	2701      	movs	r7, #1
 80038b0:	2600      	movs	r6, #0
 80038b2:	3034      	adds	r0, #52	; 0x34
 80038b4:	1825      	adds	r5, r4, r0
 80038b6:	eba8 0804 	sub.w	r8, r8, r4
 80038ba:	eb08 0305 	add.w	r3, r8, r5
 80038be:	2b0f      	cmp	r3, #15
 80038c0:	d91f      	bls.n	8003902 <onRNGEvent+0x7a>
 80038c2:	2300      	movs	r3, #0
 80038c4:	f884 3045 	strb.w	r3, [r4, #69]	; 0x45
 80038c8:	f894 3045 	ldrb.w	r3, [r4, #69]	; 0x45
 80038cc:	2b00      	cmp	r3, #0
 80038ce:	d050      	beq.n	8003972 <onRNGEvent+0xea>
 80038d0:	1e59      	subs	r1, r3, #1
 80038d2:	3301      	adds	r3, #1
 80038d4:	b2db      	uxtb	r3, r3
 80038d6:	2b03      	cmp	r3, #3
 80038d8:	bf88      	it	hi
 80038da:	2301      	movhi	r3, #1
 80038dc:	4a28      	ldr	r2, [pc, #160]	; (8003980 <onRNGEvent+0xf8>)
 80038de:	f884 3045 	strb.w	r3, [r4, #69]	; 0x45
 80038e2:	f852 2021 	ldr.w	r2, [r2, r1, lsl #2]
 80038e6:	f894 0025 	ldrb.w	r0, [r4, #37]	; 0x25
 80038ea:	4b26      	ldr	r3, [pc, #152]	; (8003984 <onRNGEvent+0xfc>)
 80038ec:	4926      	ldr	r1, [pc, #152]	; (8003988 <onRNGEvent+0x100>)
 80038ee:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
 80038f2:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
 80038f6:	f7fe b865 	b.w	80019c4 <drawRNG>
 80038fa:	1ad2      	subs	r2, r2, r3
 80038fc:	1ac0      	subs	r0, r0, r3
 80038fe:	62e2      	str	r2, [r4, #44]	; 0x2c
 8003900:	6320      	str	r0, [r4, #48]	; 0x30
 8003902:	f7ff ff77 	bl	80037f4 <dequeueBit>
 8003906:	1c41      	adds	r1, r0, #1
 8003908:	d0de      	beq.n	80038c8 <onRNGEvent+0x40>
 800390a:	6b23      	ldr	r3, [r4, #48]	; 0x30
 800390c:	6ae2      	ldr	r2, [r4, #44]	; 0x2c
 800390e:	eb00 0043 	add.w	r0, r0, r3, lsl #1
 8003912:	6aa3      	ldr	r3, [r4, #40]	; 0x28
 8003914:	0052      	lsls	r2, r2, #1
 8003916:	429a      	cmp	r2, r3
 8003918:	62e2      	str	r2, [r4, #44]	; 0x2c
 800391a:	6320      	str	r0, [r4, #48]	; 0x30
 800391c:	d3f1      	bcc.n	8003902 <onRNGEvent+0x7a>
 800391e:	4298      	cmp	r0, r3
 8003920:	d2eb      	bcs.n	80038fa <onRNGEvent+0x72>
 8003922:	1c42      	adds	r2, r0, #1
 8003924:	e9c4 760b 	strd	r7, r6, [r4, #44]	; 0x2c
 8003928:	f894 3025 	ldrb.w	r3, [r4, #37]	; 0x25
 800392c:	d0cc      	beq.n	80038c8 <onRNGEvent+0x40>
 800392e:	2b09      	cmp	r3, #9
 8003930:	d80a      	bhi.n	8003948 <onRNGEvent+0xc0>
 8003932:	f240 220d 	movw	r2, #525	; 0x20d
 8003936:	fa07 f303 	lsl.w	r3, r7, r3
 800393a:	4213      	tst	r3, r2
 800393c:	d112      	bne.n	8003964 <onRNGEvent+0xdc>
 800393e:	f413 7ff8 	tst.w	r3, #496	; 0x1f0
 8003942:	d10e      	bne.n	8003962 <onRNGEvent+0xda>
 8003944:	079b      	lsls	r3, r3, #30
 8003946:	d409      	bmi.n	800395c <onRNGEvent+0xd4>
 8003948:	233f      	movs	r3, #63	; 0x3f
 800394a:	e003      	b.n	8003954 <onRNGEvent+0xcc>
 800394c:	2823      	cmp	r0, #35	; 0x23
 800394e:	dc0e      	bgt.n	800396e <onRNGEvent+0xe6>
 8003950:	3357      	adds	r3, #87	; 0x57
 8003952:	b2db      	uxtb	r3, r3
 8003954:	702b      	strb	r3, [r5, #0]
 8003956:	f805 6f01 	strb.w	r6, [r5, #1]!
 800395a:	e7ae      	b.n	80038ba <onRNGEvent+0x32>
 800395c:	3021      	adds	r0, #33	; 0x21
 800395e:	b2c3      	uxtb	r3, r0
 8003960:	e7f8      	b.n	8003954 <onRNGEvent+0xcc>
 8003962:	3001      	adds	r0, #1
 8003964:	2809      	cmp	r0, #9
 8003966:	b2c3      	uxtb	r3, r0
 8003968:	dcf0      	bgt.n	800394c <onRNGEvent+0xc4>
 800396a:	3330      	adds	r3, #48	; 0x30
 800396c:	e7f1      	b.n	8003952 <onRNGEvent+0xca>
 800396e:	331d      	adds	r3, #29
 8003970:	e7ef      	b.n	8003952 <onRNGEvent+0xca>
 8003972:	4a06      	ldr	r2, [pc, #24]	; (800398c <onRNGEvent+0x104>)
 8003974:	e7b7      	b.n	80038e6 <onRNGEvent+0x5e>
 8003976:	bf00      	nop
 8003978:	0800728c 	stmdaeq	r0, {r2, r3, r7, r9, ip, sp, lr}
 800397c:	200006e4 	andcs	r0, r0, r4, ror #13
 8003980:	0800726c 	stmdaeq	r0, {r2, r3, r5, r6, r9, ip, sp, lr}
 8003984:	08007294 	stmdaeq	r0, {r2, r4, r7, r9, ip, sp, lr}
 8003988:	20000718 	andcs	r0, r0, r8, lsl r7
 800398c:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}

08003990 <onRNGPulse>:
 8003990:	4a22      	ldr	r2, [pc, #136]	; (8003a1c <onRNGPulse+0x8c>)
 8003992:	b570      	push	{r4, r5, r6, lr}
 8003994:	6991      	ldr	r1, [r2, #24]
 8003996:	4613      	mov	r3, r2
 8003998:	2901      	cmp	r1, #1
 800399a:	d007      	beq.n	80039ac <onRNGPulse+0x1c>
 800399c:	d302      	bcc.n	80039a4 <onRNGPulse+0x14>
 800399e:	2902      	cmp	r1, #2
 80039a0:	d007      	beq.n	80039b2 <onRNGPulse+0x22>
 80039a2:	bd70      	pop	{r4, r5, r6, pc}
 80039a4:	2301      	movs	r3, #1
 80039a6:	61d0      	str	r0, [r2, #28]
 80039a8:	6193      	str	r3, [r2, #24]
 80039aa:	e7fa      	b.n	80039a2 <onRNGPulse+0x12>
 80039ac:	6210      	str	r0, [r2, #32]
 80039ae:	2302      	movs	r3, #2
 80039b0:	e7fa      	b.n	80039a8 <onRNGPulse+0x18>
 80039b2:	e9d2 1207 	ldrd	r1, r2, [r2, #28]
 80039b6:	1a51      	subs	r1, r2, r1
 80039b8:	1a82      	subs	r2, r0, r2
 80039ba:	4291      	cmp	r1, r2
 80039bc:	d025      	beq.n	8003a0a <onRNGPulse+0x7a>
 80039be:	f893 4024 	ldrb.w	r4, [r3, #36]	; 0x24
 80039c2:	b334      	cbz	r4, 8003a12 <onRNGPulse+0x82>
 80039c4:	4291      	cmp	r1, r2
 80039c6:	bf94      	ite	ls
 80039c8:	2200      	movls	r2, #0
 80039ca:	2201      	movhi	r2, #1
 80039cc:	f084 0401 	eor.w	r4, r4, #1
 80039d0:	6819      	ldr	r1, [r3, #0]
 80039d2:	f883 4024 	strb.w	r4, [r3, #36]	; 0x24
 80039d6:	685c      	ldr	r4, [r3, #4]
 80039d8:	1b09      	subs	r1, r1, r4
 80039da:	f001 017f 	and.w	r1, r1, #127	; 0x7f
 80039de:	297f      	cmp	r1, #127	; 0x7f
 80039e0:	d013      	beq.n	8003a0a <onRNGPulse+0x7a>
 80039e2:	2101      	movs	r1, #1
 80039e4:	681c      	ldr	r4, [r3, #0]
 80039e6:	681d      	ldr	r5, [r3, #0]
 80039e8:	eb03 04d4 	add.w	r4, r3, r4, lsr #3
 80039ec:	f005 0507 	and.w	r5, r5, #7
 80039f0:	fa01 f605 	lsl.w	r6, r1, r5
 80039f4:	7a21      	ldrb	r1, [r4, #8]
 80039f6:	40aa      	lsls	r2, r5
 80039f8:	ea21 0106 	bic.w	r1, r1, r6
 80039fc:	430a      	orrs	r2, r1
 80039fe:	7222      	strb	r2, [r4, #8]
 8003a00:	681a      	ldr	r2, [r3, #0]
 8003a02:	3201      	adds	r2, #1
 8003a04:	f002 027f 	and.w	r2, r2, #127	; 0x7f
 8003a08:	601a      	str	r2, [r3, #0]
 8003a0a:	2201      	movs	r2, #1
 8003a0c:	e9c3 2006 	strd	r2, r0, [r3, #24]
 8003a10:	e7c7      	b.n	80039a2 <onRNGPulse+0x12>
 8003a12:	4291      	cmp	r1, r2
 8003a14:	bf2c      	ite	cs
 8003a16:	2200      	movcs	r2, #0
 8003a18:	2201      	movcc	r2, #1
 8003a1a:	e7d7      	b.n	80039cc <onRNGPulse+0x3c>
 8003a1c:	200006e4 	andcs	r0, r0, r4, ror #13

08003a20 <getRandomData>:
 8003a20:	4a0a      	ldr	r2, [pc, #40]	; (8003a4c <getRandomData+0x2c>)
 8003a22:	b538      	push	{r3, r4, r5, lr}
 8003a24:	6813      	ldr	r3, [r2, #0]
 8003a26:	6852      	ldr	r2, [r2, #4]
 8003a28:	1a9b      	subs	r3, r3, r2
 8003a2a:	f003 037f 	and.w	r3, r3, #127	; 0x7f
 8003a2e:	2b07      	cmp	r3, #7
 8003a30:	d909      	bls.n	8003a46 <getRandomData+0x26>
 8003a32:	2508      	movs	r5, #8
 8003a34:	2000      	movs	r0, #0
 8003a36:	0044      	lsls	r4, r0, #1
 8003a38:	f7ff fedc 	bl	80037f4 <dequeueBit>
 8003a3c:	3d01      	subs	r5, #1
 8003a3e:	ea40 0004 	orr.w	r0, r0, r4
 8003a42:	d1f8      	bne.n	8003a36 <getRandomData+0x16>
 8003a44:	bd38      	pop	{r3, r4, r5, pc}
 8003a46:	f04f 30ff 	mov.w	r0, #4294967295
 8003a4a:	e7fb      	b.n	8003a44 <getRandomData+0x24>
 8003a4c:	200006e4 	andcs	r0, r0, r4, ror #13

08003a50 <onRTCTimeZoneMenuSelect>:
 8003a50:	4b03      	ldr	r3, [pc, #12]	; (8003a60 <onRTCTimeZoneMenuSelect+0x10>)
 8003a52:	6841      	ldr	r1, [r0, #4]
 8003a54:	79da      	ldrb	r2, [r3, #7]
 8003a56:	8849      	ldrh	r1, [r1, #2]
 8003a58:	f361 0245 	bfi	r2, r1, #1, #5
 8003a5c:	71da      	strb	r2, [r3, #7]
 8003a5e:	4770      	bx	lr
 8003a60:	20000da0 	andcs	r0, r0, r0, lsr #27

08003a64 <onRTCMenuGetOption>:
 8003a64:	2302      	movs	r3, #2
 8003a66:	6013      	str	r3, [r2, #0]
 8003a68:	4b01      	ldr	r3, [pc, #4]	; (8003a70 <onRTCMenuGetOption+0xc>)
 8003a6a:	f853 0021 	ldr.w	r0, [r3, r1, lsl #2]
 8003a6e:	4770      	bx	lr
 8003a70:	080073f8 	stmdaeq	r0, {r3, r4, r5, r6, r7, r8, r9, ip, sp, lr}

08003a74 <onRTCSubMenuBack>:
 8003a74:	4801      	ldr	r0, [pc, #4]	; (8003a7c <onRTCSubMenuBack+0x8>)
 8003a76:	f001 bc07 	b.w	8005288 <setView>
 8003a7a:	bf00      	nop
 8003a7c:	08007364 	stmdaeq	r0, {r2, r5, r6, r8, r9, ip, sp, lr}

08003a80 <onRTCTimeZoneMenuGetOption>:
 8003a80:	291a      	cmp	r1, #26
 8003a82:	b510      	push	{r4, lr}
 8003a84:	d81c      	bhi.n	8003ac0 <onRTCTimeZoneMenuGetOption+0x40>
 8003a86:	243c      	movs	r4, #60	; 0x3c
 8003a88:	4b0e      	ldr	r3, [pc, #56]	; (8003ac4 <onRTCTimeZoneMenuGetOption+0x44>)
 8003a8a:	434c      	muls	r4, r1
 8003a8c:	79db      	ldrb	r3, [r3, #7]
 8003a8e:	f3c3 0344 	ubfx	r3, r3, #1, #5
 8003a92:	1a58      	subs	r0, r3, r1
 8003a94:	4243      	negs	r3, r0
 8003a96:	4143      	adcs	r3, r0
 8003a98:	f5b4 7434 	subs.w	r4, r4, #720	; 0x2d0
 8003a9c:	6013      	str	r3, [r2, #0]
 8003a9e:	d50a      	bpl.n	8003ab6 <onRTCTimeZoneMenuGetOption+0x36>
 8003aa0:	4909      	ldr	r1, [pc, #36]	; (8003ac8 <onRTCTimeZoneMenuGetOption+0x48>)
 8003aa2:	480a      	ldr	r0, [pc, #40]	; (8003acc <onRTCTimeZoneMenuGetOption+0x4c>)
 8003aa4:	f002 fada 	bl	800605c <strcpy>
 8003aa8:	4264      	negs	r4, r4
 8003aaa:	4808      	ldr	r0, [pc, #32]	; (8003acc <onRTCTimeZoneMenuGetOption+0x4c>)
 8003aac:	4621      	mov	r1, r4
 8003aae:	f7fd f837 	bl	8000b20 <strcatTime>
 8003ab2:	4806      	ldr	r0, [pc, #24]	; (8003acc <onRTCTimeZoneMenuGetOption+0x4c>)
 8003ab4:	bd10      	pop	{r4, pc}
 8003ab6:	4906      	ldr	r1, [pc, #24]	; (8003ad0 <onRTCTimeZoneMenuGetOption+0x50>)
 8003ab8:	4804      	ldr	r0, [pc, #16]	; (8003acc <onRTCTimeZoneMenuGetOption+0x4c>)
 8003aba:	f002 facf 	bl	800605c <strcpy>
 8003abe:	e7f4      	b.n	8003aaa <onRTCTimeZoneMenuGetOption+0x2a>
 8003ac0:	2000      	movs	r0, #0
 8003ac2:	e7f7      	b.n	8003ab4 <onRTCTimeZoneMenuGetOption+0x34>
 8003ac4:	20000da0 	andcs	r0, r0, r0, lsr #27
 8003ac8:	08007378 	stmdaeq	r0, {r3, r4, r5, r6, r8, r9, ip, sp, lr}
 8003acc:	20000d7c 	andcs	r0, r0, ip, ror sp
 8003ad0:	0800737d 	stmdaeq	r0, {r0, r2, r3, r4, r5, r6, r8, r9, ip, sp, lr}

08003ad4 <getDaysInMonth>:
 8003ad4:	2902      	cmp	r1, #2
 8003ad6:	d112      	bne.n	8003afe <getDaysInMonth+0x2a>
 8003ad8:	0783      	lsls	r3, r0, #30
 8003ada:	d115      	bne.n	8003b08 <getDaysInMonth+0x34>
 8003adc:	2264      	movs	r2, #100	; 0x64
 8003ade:	fbb0 f3f2 	udiv	r3, r0, r2
 8003ae2:	fb02 0313 	mls	r3, r2, r3, r0
 8003ae6:	b98b      	cbnz	r3, 8003b0c <getDaysInMonth+0x38>
 8003ae8:	f44f 72c8 	mov.w	r2, #400	; 0x190
 8003aec:	fbb0 f3f2 	udiv	r3, r0, r2
 8003af0:	fb02 0013 	mls	r0, r2, r3, r0
 8003af4:	2800      	cmp	r0, #0
 8003af6:	bf14      	ite	ne
 8003af8:	201c      	movne	r0, #28
 8003afa:	201d      	moveq	r0, #29
 8003afc:	4770      	bx	lr
 8003afe:	4b04      	ldr	r3, [pc, #16]	; (8003b10 <getDaysInMonth+0x3c>)
 8003b00:	4419      	add	r1, r3
 8003b02:	f811 0c01 	ldrb.w	r0, [r1, #-1]
 8003b06:	4770      	bx	lr
 8003b08:	201c      	movs	r0, #28
 8003b0a:	4770      	bx	lr
 8003b0c:	201d      	movs	r0, #29
 8003b0e:	4770      	bx	lr
 8003b10:	0800736c 	stmdaeq	r0, {r2, r3, r5, r6, r8, r9, ip, sp, lr}

08003b14 <onRTCSubMenuGetOption>:
 8003b14:	4b1e      	ldr	r3, [pc, #120]	; (8003b90 <onRTCSubMenuGetOption+0x7c>)
 8003b16:	b570      	push	{r4, r5, r6, lr}
 8003b18:	460c      	mov	r4, r1
 8003b1a:	8859      	ldrh	r1, [r3, #2]
 8003b1c:	1e4d      	subs	r5, r1, #1
 8003b1e:	2d04      	cmp	r5, #4
 8003b20:	d80c      	bhi.n	8003b3c <onRTCSubMenuGetOption+0x28>
 8003b22:	e8df f005 	tbb	[pc, r5]
 8003b26:	2c292503 	cfstr32cs	mvfx2, [r9], #-12
 8003b2a:	4b19002f 	blmi	8643bee <_sidata+0x63c39e>
 8003b2e:	881b      	ldrh	r3, [r3, #0]
 8003b30:	f2a3 73e4 	subw	r3, r3, #2020	; 0x7e4
 8003b34:	1b18      	subs	r0, r3, r4
 8003b36:	4243      	negs	r3, r0
 8003b38:	4143      	adcs	r3, r0
 8003b3a:	6013      	str	r3, [r2, #0]
 8003b3c:	4e16      	ldr	r6, [pc, #88]	; (8003b98 <onRTCSubMenuGetOption+0x84>)
 8003b3e:	2903      	cmp	r1, #3
 8003b40:	eb06 02c5 	add.w	r2, r6, r5, lsl #3
 8003b44:	6850      	ldr	r0, [r2, #4]
 8003b46:	d104      	bne.n	8003b52 <onRTCSubMenuGetOption+0x3e>
 8003b48:	4b12      	ldr	r3, [pc, #72]	; (8003b94 <onRTCSubMenuGetOption+0x80>)
 8003b4a:	7899      	ldrb	r1, [r3, #2]
 8003b4c:	8818      	ldrh	r0, [r3, #0]
 8003b4e:	f7ff ffc1 	bl	8003ad4 <getDaysInMonth>
 8003b52:	42a0      	cmp	r0, r4
 8003b54:	d919      	bls.n	8003b8a <onRTCSubMenuGetOption+0x76>
 8003b56:	4911      	ldr	r1, [pc, #68]	; (8003b9c <onRTCSubMenuGetOption+0x88>)
 8003b58:	4811      	ldr	r0, [pc, #68]	; (8003ba0 <onRTCSubMenuGetOption+0x8c>)
 8003b5a:	f002 fa7f 	bl	800605c <strcpy>
 8003b5e:	f856 1035 	ldr.w	r1, [r6, r5, lsl #3]
 8003b62:	480f      	ldr	r0, [pc, #60]	; (8003ba0 <onRTCSubMenuGetOption+0x8c>)
 8003b64:	2200      	movs	r2, #0
 8003b66:	4421      	add	r1, r4
 8003b68:	f7fc ffa0 	bl	8000aac <strcatUInt32>
 8003b6c:	480c      	ldr	r0, [pc, #48]	; (8003ba0 <onRTCSubMenuGetOption+0x8c>)
 8003b6e:	bd70      	pop	{r4, r5, r6, pc}
 8003b70:	4b08      	ldr	r3, [pc, #32]	; (8003b94 <onRTCSubMenuGetOption+0x80>)
 8003b72:	789b      	ldrb	r3, [r3, #2]
 8003b74:	3b01      	subs	r3, #1
 8003b76:	e7dd      	b.n	8003b34 <onRTCSubMenuGetOption+0x20>
 8003b78:	4b06      	ldr	r3, [pc, #24]	; (8003b94 <onRTCSubMenuGetOption+0x80>)
 8003b7a:	78db      	ldrb	r3, [r3, #3]
 8003b7c:	e7fa      	b.n	8003b74 <onRTCSubMenuGetOption+0x60>
 8003b7e:	4b05      	ldr	r3, [pc, #20]	; (8003b94 <onRTCSubMenuGetOption+0x80>)
 8003b80:	791b      	ldrb	r3, [r3, #4]
 8003b82:	e7d7      	b.n	8003b34 <onRTCSubMenuGetOption+0x20>
 8003b84:	4b03      	ldr	r3, [pc, #12]	; (8003b94 <onRTCSubMenuGetOption+0x80>)
 8003b86:	795b      	ldrb	r3, [r3, #5]
 8003b88:	e7d4      	b.n	8003b34 <onRTCSubMenuGetOption+0x20>
 8003b8a:	2000      	movs	r0, #0
 8003b8c:	e7ef      	b.n	8003b6e <onRTCSubMenuGetOption+0x5a>
 8003b8e:	bf00      	nop
 8003b90:	2000073c 	andcs	r0, r0, ip, lsr r7
 8003b94:	20000730 	andcs	r0, r0, r0, lsr r7
 8003b98:	080073d0 	stmdaeq	r0, {r4, r6, r7, r8, r9, ip, sp, lr}
 8003b9c:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}
 8003ba0:	20000d7c 	andcs	r0, r0, ip, ror sp

08003ba4 <initRTC>:
 8003ba4:	b508      	push	{r3, lr}
 8003ba6:	f000 ff41 	bl	8004a2c <initRTCController>
 8003baa:	4b05      	ldr	r3, [pc, #20]	; (8003bc0 <initRTC+0x1c>)
 8003bac:	221b      	movs	r2, #27
 8003bae:	79d9      	ldrb	r1, [r3, #7]
 8003bb0:	4804      	ldr	r0, [pc, #16]	; (8003bc4 <initRTC+0x20>)
 8003bb2:	f3c1 0144 	ubfx	r1, r1, #1, #5
 8003bb6:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8003bba:	f7ff bd57 	b.w	800366c <selectMenuItem>
 8003bbe:	bf00      	nop
 8003bc0:	20000da0 	andcs	r0, r0, r0, lsr #27
 8003bc4:	0800744c 	stmdaeq	r0, {r2, r3, r6, sl, ip, sp, lr}

08003bc8 <getTimeZoneOffset>:
 8003bc8:	f44f 6061 	mov.w	r0, #3600	; 0xe10
 8003bcc:	4b03      	ldr	r3, [pc, #12]	; (8003bdc <getTimeZoneOffset+0x14>)
 8003bce:	79db      	ldrb	r3, [r3, #7]
 8003bd0:	f3c3 0344 	ubfx	r3, r3, #1, #5
 8003bd4:	3b0c      	subs	r3, #12
 8003bd6:	4358      	muls	r0, r3
 8003bd8:	4770      	bx	lr
 8003bda:	bf00      	nop
 8003bdc:	20000da0 	andcs	r0, r0, r0, lsr #27

08003be0 <getTimeFromDateTime>:
 8003be0:	b530      	push	{r4, r5, lr}
 8003be2:	7884      	ldrb	r4, [r0, #2]
 8003be4:	f44f 71c8 	mov.w	r1, #400	; 0x190
 8003be8:	2c02      	cmp	r4, #2
 8003bea:	bf94      	ite	ls
 8003bec:	2301      	movls	r3, #1
 8003bee:	2300      	movhi	r3, #0
 8003bf0:	8802      	ldrh	r2, [r0, #0]
 8003bf2:	bf88      	it	hi
 8003bf4:	f46f 7ce4 	mvnhi.w	ip, #456	; 0x1c8
 8003bf8:	eba2 0203 	sub.w	r2, r2, r3
 8003bfc:	fbb2 f3f1 	udiv	r3, r2, r1
 8003c00:	fb01 2213 	mls	r2, r1, r3, r2
 8003c04:	f04f 0199 	mov.w	r1, #153	; 0x99
 8003c08:	fb01 f104 	mul.w	r1, r1, r4
 8003c0c:	f04f 0405 	mov.w	r4, #5
 8003c10:	bf8c      	ite	hi
 8003c12:	4461      	addhi	r1, ip
 8003c14:	f201 5163 	addwls	r1, r1, #1379	; 0x563
 8003c18:	fbb1 f1f4 	udiv	r1, r1, r4
 8003c1c:	f240 146d 	movw	r4, #365	; 0x16d
 8003c20:	78c5      	ldrb	r5, [r0, #3]
 8003c22:	f5a5 252f 	sub.w	r5, r5, #716800	; 0xaf000
 8003c26:	f6a5 256d 	subw	r5, r5, #2669	; 0xa6d
 8003c2a:	fb04 5402 	mla	r4, r4, r2, r5
 8003c2e:	4d0c      	ldr	r5, [pc, #48]	; (8003c60 <getTimeFromDateTime+0x80>)
 8003c30:	eb04 0492 	add.w	r4, r4, r2, lsr #2
 8003c34:	fb05 4303 	mla	r3, r5, r3, r4
 8003c38:	2464      	movs	r4, #100	; 0x64
 8003c3a:	fbb2 f2f4 	udiv	r2, r2, r4
 8003c3e:	1a9b      	subs	r3, r3, r2
 8003c40:	440b      	add	r3, r1
 8003c42:	213c      	movs	r1, #60	; 0x3c
 8003c44:	7944      	ldrb	r4, [r0, #5]
 8003c46:	7902      	ldrb	r2, [r0, #4]
 8003c48:	4361      	muls	r1, r4
 8003c4a:	f44f 6461 	mov.w	r4, #3600	; 0xe10
 8003c4e:	fb04 1202 	mla	r2, r4, r2, r1
 8003c52:	7980      	ldrb	r0, [r0, #6]
 8003c54:	4402      	add	r2, r0
 8003c56:	4803      	ldr	r0, [pc, #12]	; (8003c64 <getTimeFromDateTime+0x84>)
 8003c58:	fb00 2003 	mla	r0, r0, r3, r2
 8003c5c:	bd30      	pop	{r4, r5, pc}
 8003c5e:	bf00      	nop
 8003c60:	00023ab1 			; <UNDEFINED> instruction: 0x00023ab1
 8003c64:	00015180 	andeq	r5, r1, r0, lsl #3

08003c68 <getDateTimeFromTime>:
 8003c68:	b5f0      	push	{r4, r5, r6, r7, lr}
 8003c6a:	4c2b      	ldr	r4, [pc, #172]	; (8003d18 <getDateTimeFromTime+0xb0>)
 8003c6c:	f240 57b4 	movw	r7, #1460	; 0x5b4
 8003c70:	fbb0 f2f4 	udiv	r2, r0, r4
 8003c74:	fb04 0012 	mls	r0, r4, r2, r0
 8003c78:	f502 232f 	add.w	r3, r2, #716800	; 0xaf000
 8003c7c:	4c27      	ldr	r4, [pc, #156]	; (8003d1c <getDateTimeFromTime+0xb4>)
 8003c7e:	f603 236c 	addw	r3, r3, #2668	; 0xa6c
 8003c82:	fbb3 f5f4 	udiv	r5, r3, r4
 8003c86:	f648 62ac 	movw	r2, #36524	; 0x8eac
 8003c8a:	fb04 3415 	mls	r4, r4, r5, r3
 8003c8e:	fbb4 f2f2 	udiv	r2, r4, r2
 8003c92:	fbb4 f3f7 	udiv	r3, r4, r7
 8003c96:	4422      	add	r2, r4
 8003c98:	1ad2      	subs	r2, r2, r3
 8003c9a:	4b21      	ldr	r3, [pc, #132]	; (8003d20 <getDateTimeFromTime+0xb8>)
 8003c9c:	f240 1c6d 	movw	ip, #365	; 0x16d
 8003ca0:	fbb4 f3f3 	udiv	r3, r4, r3
 8003ca4:	1ad3      	subs	r3, r2, r3
 8003ca6:	fbb3 f2fc 	udiv	r2, r3, ip
 8003caa:	f44f 76c8 	mov.w	r6, #400	; 0x190
 8003cae:	fb06 2505 	mla	r5, r6, r5, r2
 8003cb2:	f648 6694 	movw	r6, #36500	; 0x8e94
 8003cb6:	fbb3 f6f6 	udiv	r6, r3, r6
 8003cba:	fbb3 f3f7 	udiv	r3, r3, r7
 8003cbe:	4434      	add	r4, r6
 8003cc0:	1ae3      	subs	r3, r4, r3
 8003cc2:	2499      	movs	r4, #153	; 0x99
 8003cc4:	fb0c 3312 	mls	r3, ip, r2, r3
 8003cc8:	eb03 0283 	add.w	r2, r3, r3, lsl #2
 8003ccc:	3202      	adds	r2, #2
 8003cce:	fbb2 f2f4 	udiv	r2, r2, r4
 8003cd2:	2605      	movs	r6, #5
 8003cd4:	4354      	muls	r4, r2
 8003cd6:	3402      	adds	r4, #2
 8003cd8:	fbb4 f4f6 	udiv	r4, r4, r6
 8003cdc:	3301      	adds	r3, #1
 8003cde:	1b1b      	subs	r3, r3, r4
 8003ce0:	70cb      	strb	r3, [r1, #3]
 8003ce2:	f44f 6361 	mov.w	r3, #3600	; 0xe10
 8003ce6:	2a09      	cmp	r2, #9
 8003ce8:	bf94      	ite	ls
 8003cea:	3203      	addls	r2, #3
 8003cec:	3a09      	subhi	r2, #9
 8003cee:	fbb0 f3f3 	udiv	r3, r0, r3
 8003cf2:	2a02      	cmp	r2, #2
 8003cf4:	bf98      	it	ls
 8003cf6:	3501      	addls	r5, #1
 8003cf8:	708a      	strb	r2, [r1, #2]
 8003cfa:	223c      	movs	r2, #60	; 0x3c
 8003cfc:	710b      	strb	r3, [r1, #4]
 8003cfe:	fbb0 f3f2 	udiv	r3, r0, r2
 8003d02:	fbb3 f4f2 	udiv	r4, r3, r2
 8003d06:	fb02 0013 	mls	r0, r2, r3, r0
 8003d0a:	fb02 3414 	mls	r4, r2, r4, r3
 8003d0e:	800d      	strh	r5, [r1, #0]
 8003d10:	714c      	strb	r4, [r1, #5]
 8003d12:	7188      	strb	r0, [r1, #6]
 8003d14:	bdf0      	pop	{r4, r5, r6, r7, pc}
 8003d16:	bf00      	nop
 8003d18:	00015180 	andeq	r5, r1, r0, lsl #3
 8003d1c:	00023ab1 			; <UNDEFINED> instruction: 0x00023ab1
 8003d20:	00023ab0 			; <UNDEFINED> instruction: 0x00023ab0

08003d24 <setDeviceDateTime>:
 8003d24:	b508      	push	{r3, lr}
 8003d26:	f7ff ff5b 	bl	8003be0 <getTimeFromDateTime>
 8003d2a:	4602      	mov	r2, r0
 8003d2c:	f7ff ff4c 	bl	8003bc8 <getTimeZoneOffset>
 8003d30:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8003d34:	1a10      	subs	r0, r2, r0
 8003d36:	f000 be13 	b.w	8004960 <setDeviceTime>

08003d3a <getDeviceDateTime>:
 8003d3a:	b510      	push	{r4, lr}
 8003d3c:	4604      	mov	r4, r0
 8003d3e:	f000 fea9 	bl	8004a94 <getDeviceTime>
 8003d42:	4602      	mov	r2, r0
 8003d44:	f7ff ff40 	bl	8003bc8 <getTimeZoneOffset>
 8003d48:	4621      	mov	r1, r4
 8003d4a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8003d4e:	4410      	add	r0, r2
 8003d50:	f7ff bf8a 	b.w	8003c68 <getDateTimeFromTime>

08003d54 <onRTCSubMenuSelect>:
 8003d54:	b513      	push	{r0, r1, r4, lr}
 8003d56:	4604      	mov	r4, r0
 8003d58:	4668      	mov	r0, sp
 8003d5a:	f7ff ffee 	bl	8003d3a <getDeviceDateTime>
 8003d5e:	f240 73e3 	movw	r3, #2019	; 0x7e3
 8003d62:	f8bd 2000 	ldrh.w	r2, [sp]
 8003d66:	429a      	cmp	r2, r3
 8003d68:	bf9c      	itt	ls
 8003d6a:	f240 73e4 	movwls	r3, #2020	; 0x7e4
 8003d6e:	f8ad 3000 	strhls.w	r3, [sp]
 8003d72:	f640 0333 	movw	r3, #2099	; 0x833
 8003d76:	f8bd 2000 	ldrh.w	r2, [sp]
 8003d7a:	429a      	cmp	r2, r3
 8003d7c:	bf88      	it	hi
 8003d7e:	f8ad 3000 	strhhi.w	r3, [sp]
 8003d82:	4b19      	ldr	r3, [pc, #100]	; (8003de8 <onRTCSubMenuSelect+0x94>)
 8003d84:	885b      	ldrh	r3, [r3, #2]
 8003d86:	3b01      	subs	r3, #1
 8003d88:	2b04      	cmp	r3, #4
 8003d8a:	d80a      	bhi.n	8003da2 <onRTCSubMenuSelect+0x4e>
 8003d8c:	e8df f003 	tbb	[pc, r3]
 8003d90:	1f191303 	svcne	0x00191303
 8003d94:	68630024 	stmdavs	r3!, {r2, r5}^
 8003d98:	885b      	ldrh	r3, [r3, #2]
 8003d9a:	f203 73e4 	addw	r3, r3, #2020	; 0x7e4
 8003d9e:	f8ad 3000 	strh.w	r3, [sp]
 8003da2:	4668      	mov	r0, sp
 8003da4:	f7ff ffbe 	bl	8003d24 <setDeviceDateTime>
 8003da8:	466b      	mov	r3, sp
 8003daa:	4a10      	ldr	r2, [pc, #64]	; (8003dec <onRTCSubMenuSelect+0x98>)
 8003dac:	cb03      	ldmia	r3!, {r0, r1}
 8003dae:	6010      	str	r0, [r2, #0]
 8003db0:	6051      	str	r1, [r2, #4]
 8003db2:	b002      	add	sp, #8
 8003db4:	bd10      	pop	{r4, pc}
 8003db6:	6863      	ldr	r3, [r4, #4]
 8003db8:	789b      	ldrb	r3, [r3, #2]
 8003dba:	3301      	adds	r3, #1
 8003dbc:	f88d 3002 	strb.w	r3, [sp, #2]
 8003dc0:	e7ef      	b.n	8003da2 <onRTCSubMenuSelect+0x4e>
 8003dc2:	6863      	ldr	r3, [r4, #4]
 8003dc4:	789b      	ldrb	r3, [r3, #2]
 8003dc6:	3301      	adds	r3, #1
 8003dc8:	f88d 3003 	strb.w	r3, [sp, #3]
 8003dcc:	e7e9      	b.n	8003da2 <onRTCSubMenuSelect+0x4e>
 8003dce:	6863      	ldr	r3, [r4, #4]
 8003dd0:	885b      	ldrh	r3, [r3, #2]
 8003dd2:	f88d 3004 	strb.w	r3, [sp, #4]
 8003dd6:	e7e4      	b.n	8003da2 <onRTCSubMenuSelect+0x4e>
 8003dd8:	6863      	ldr	r3, [r4, #4]
 8003dda:	885b      	ldrh	r3, [r3, #2]
 8003ddc:	f88d 3005 	strb.w	r3, [sp, #5]
 8003de0:	2300      	movs	r3, #0
 8003de2:	f88d 3006 	strb.w	r3, [sp, #6]
 8003de6:	e7dc      	b.n	8003da2 <onRTCSubMenuSelect+0x4e>
 8003de8:	2000073c 	andcs	r0, r0, ip, lsr r7
 8003dec:	20000730 	andcs	r0, r0, r0, lsr r7

08003df0 <onRTCMenuSelect>:
 8003df0:	b538      	push	{r3, r4, r5, lr}
 8003df2:	4c25      	ldr	r4, [pc, #148]	; (8003e88 <onRTCMenuSelect+0x98>)
 8003df4:	4620      	mov	r0, r4
 8003df6:	f7ff ffa0 	bl	8003d3a <getDeviceDateTime>
 8003dfa:	f240 73e3 	movw	r3, #2019	; 0x7e3
 8003dfe:	8822      	ldrh	r2, [r4, #0]
 8003e00:	4620      	mov	r0, r4
 8003e02:	429a      	cmp	r2, r3
 8003e04:	bf9c      	itt	ls
 8003e06:	f240 73e4 	movwls	r3, #2020	; 0x7e4
 8003e0a:	8023      	strhls	r3, [r4, #0]
 8003e0c:	f640 0333 	movw	r3, #2099	; 0x833
 8003e10:	8822      	ldrh	r2, [r4, #0]
 8003e12:	429a      	cmp	r2, r3
 8003e14:	bf88      	it	hi
 8003e16:	8023      	strhhi	r3, [r4, #0]
 8003e18:	4b1c      	ldr	r3, [pc, #112]	; (8003e8c <onRTCMenuSelect+0x9c>)
 8003e1a:	885b      	ldrh	r3, [r3, #2]
 8003e1c:	2b05      	cmp	r3, #5
 8003e1e:	d82f      	bhi.n	8003e80 <onRTCMenuSelect+0x90>
 8003e20:	e8df f003 	tbb	[pc, r3]
 8003e24:	1d181203 	lfmne	f1, 4, [r8, #-12]
 8003e28:	221b2a26 	andscs	r2, fp, #155648	; 0x26000
 8003e2c:	4b18      	ldr	r3, [pc, #96]	; (8003e90 <onRTCMenuSelect+0xa0>)
 8003e2e:	4d19      	ldr	r5, [pc, #100]	; (8003e94 <onRTCMenuSelect+0xa4>)
 8003e30:	79dc      	ldrb	r4, [r3, #7]
 8003e32:	f3c4 0444 	ubfx	r4, r4, #1, #5
 8003e36:	6868      	ldr	r0, [r5, #4]
 8003e38:	4621      	mov	r1, r4
 8003e3a:	f7ff fc17 	bl	800366c <selectMenuItem>
 8003e3e:	4628      	mov	r0, r5
 8003e40:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8003e44:	f001 ba20 	b.w	8005288 <setView>
 8003e48:	8824      	ldrh	r4, [r4, #0]
 8003e4a:	2250      	movs	r2, #80	; 0x50
 8003e4c:	f2a4 74e4 	subw	r4, r4, #2020	; 0x7e4
 8003e50:	4d11      	ldr	r5, [pc, #68]	; (8003e98 <onRTCMenuSelect+0xa8>)
 8003e52:	e7f0      	b.n	8003e36 <onRTCMenuSelect+0x46>
 8003e54:	78a4      	ldrb	r4, [r4, #2]
 8003e56:	220c      	movs	r2, #12
 8003e58:	3c01      	subs	r4, #1
 8003e5a:	4d10      	ldr	r5, [pc, #64]	; (8003e9c <onRTCMenuSelect+0xac>)
 8003e5c:	e7eb      	b.n	8003e36 <onRTCMenuSelect+0x46>
 8003e5e:	7881      	ldrb	r1, [r0, #2]
 8003e60:	8800      	ldrh	r0, [r0, #0]
 8003e62:	f7ff fe37 	bl	8003ad4 <getDaysInMonth>
 8003e66:	78e4      	ldrb	r4, [r4, #3]
 8003e68:	4602      	mov	r2, r0
 8003e6a:	3c01      	subs	r4, #1
 8003e6c:	4d0c      	ldr	r5, [pc, #48]	; (8003ea0 <onRTCMenuSelect+0xb0>)
 8003e6e:	e7e2      	b.n	8003e36 <onRTCMenuSelect+0x46>
 8003e70:	7924      	ldrb	r4, [r4, #4]
 8003e72:	2218      	movs	r2, #24
 8003e74:	4d0b      	ldr	r5, [pc, #44]	; (8003ea4 <onRTCMenuSelect+0xb4>)
 8003e76:	e7de      	b.n	8003e36 <onRTCMenuSelect+0x46>
 8003e78:	7964      	ldrb	r4, [r4, #5]
 8003e7a:	223c      	movs	r2, #60	; 0x3c
 8003e7c:	4d0a      	ldr	r5, [pc, #40]	; (8003ea8 <onRTCMenuSelect+0xb8>)
 8003e7e:	e7da      	b.n	8003e36 <onRTCMenuSelect+0x46>
 8003e80:	2300      	movs	r3, #0
 8003e82:	685b      	ldr	r3, [r3, #4]
 8003e84:	deff      	udf	#255	; 0xff
 8003e86:	bf00      	nop
 8003e88:	20000730 	andcs	r0, r0, r0, lsr r7
 8003e8c:	2000073c 	andcs	r0, r0, ip, lsr r7
 8003e90:	20000da0 	andcs	r0, r0, r0, lsr #27
 8003e94:	08007460 	stmdaeq	r0, {r5, r6, sl, ip, sp, lr}
 8003e98:	0800747c 	stmdaeq	r0, {r2, r3, r4, r5, r6, sl, ip, sp, lr}
 8003e9c:	08007444 	stmdaeq	r0, {r2, r6, sl, ip, sp, lr}
 8003ea0:	08007398 	stmdaeq	r0, {r3, r4, r7, r8, r9, ip, sp, lr}
 8003ea4:	080073b4 	stmdaeq	r0, {r2, r4, r5, r7, r8, r9, ip, sp, lr}
 8003ea8:	08007428 	stmdaeq	r0, {r3, r5, sl, ip, sp, lr}

08003eac <onSettingsMenuGetOption>:
 8003eac:	2302      	movs	r3, #2
 8003eae:	6013      	str	r3, [r2, #0]
 8003eb0:	4b01      	ldr	r3, [pc, #4]	; (8003eb8 <onSettingsMenuGetOption+0xc>)
 8003eb2:	f853 0031 	ldr.w	r0, [r3, r1, lsl #3]
 8003eb6:	4770      	bx	lr
 8003eb8:	080074cc 	stmdaeq	r0, {r2, r3, r6, r7, sl, ip, sp, lr}

08003ebc <onSettingsMenuSelect>:
 8003ebc:	6843      	ldr	r3, [r0, #4]
 8003ebe:	885a      	ldrh	r2, [r3, #2]
 8003ec0:	4b02      	ldr	r3, [pc, #8]	; (8003ecc <onSettingsMenuSelect+0x10>)
 8003ec2:	eb03 03c2 	add.w	r3, r3, r2, lsl #3
 8003ec6:	6858      	ldr	r0, [r3, #4]
 8003ec8:	f001 b9de 	b.w	8005288 <setView>
 8003ecc:	080074cc 	stmdaeq	r0, {r2, r3, r6, r7, sl, ip, sp, lr}

08003ed0 <onSettingsMenuBack>:
 8003ed0:	f04f 30ff 	mov.w	r0, #4294967295
 8003ed4:	f7ff b8d6 	b.w	8003084 <setMeasurementView>

08003ed8 <getFlashSettings.constprop.0>:
 8003ed8:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
 8003edc:	2400      	movs	r4, #0
 8003ede:	b089      	sub	sp, #36	; 0x24
 8003ee0:	4606      	mov	r6, r0
 8003ee2:	4689      	mov	r9, r1
 8003ee4:	f7fe f872 	bl	8001fcc <setFlashPageHead>
 8003ee8:	4b12      	ldr	r3, [pc, #72]	; (8003f34 <getFlashSettings.constprop.0+0x5c>)
 8003eea:	af01      	add	r7, sp, #4
 8003eec:	681b      	ldr	r3, [r3, #0]
 8003eee:	f1a3 081c 	sub.w	r8, r3, #28
 8003ef2:	68b3      	ldr	r3, [r6, #8]
 8003ef4:	4543      	cmp	r3, r8
 8003ef6:	d903      	bls.n	8003f00 <getFlashSettings.constprop.0+0x28>
 8003ef8:	4620      	mov	r0, r4
 8003efa:	b009      	add	sp, #36	; 0x24
 8003efc:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 8003f00:	221c      	movs	r2, #28
 8003f02:	4639      	mov	r1, r7
 8003f04:	4630      	mov	r0, r6
 8003f06:	f000 fc79 	bl	80047fc <readFlash>
 8003f0a:	2300      	movs	r3, #0
 8003f0c:	5cfa      	ldrb	r2, [r7, r3]
 8003f0e:	2aff      	cmp	r2, #255	; 0xff
 8003f10:	d106      	bne.n	8003f20 <getFlashSettings.constprop.0+0x48>
 8003f12:	3301      	adds	r3, #1
 8003f14:	2b1c      	cmp	r3, #28
 8003f16:	d1f9      	bne.n	8003f0c <getFlashSettings.constprop.0+0x34>
 8003f18:	68b3      	ldr	r3, [r6, #8]
 8003f1a:	3b1c      	subs	r3, #28
 8003f1c:	60b3      	str	r3, [r6, #8]
 8003f1e:	e7eb      	b.n	8003ef8 <getFlashSettings.constprop.0+0x20>
 8003f20:	464c      	mov	r4, r9
 8003f22:	463d      	mov	r5, r7
 8003f24:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
 8003f26:	c40f      	stmia	r4!, {r0, r1, r2, r3}
 8003f28:	e895 0007 	ldmia.w	r5, {r0, r1, r2}
 8003f2c:	e884 0007 	stmia.w	r4, {r0, r1, r2}
 8003f30:	2401      	movs	r4, #1
 8003f32:	e7de      	b.n	8003ef2 <getFlashSettings.constprop.0+0x1a>
 8003f34:	08007598 	stmdaeq	r0, {r3, r4, r7, r8, sl, ip, sp, lr}

08003f38 <initSettings>:
 8003f38:	2201      	movs	r2, #1
 8003f3a:	2104      	movs	r1, #4
 8003f3c:	b530      	push	{r4, r5, lr}
 8003f3e:	4c25      	ldr	r4, [pc, #148]	; (8003fd4 <initSettings+0x9c>)
 8003f40:	b08b      	sub	sp, #44	; 0x2c
 8003f42:	78a3      	ldrb	r3, [r4, #2]
 8003f44:	4668      	mov	r0, sp
 8003f46:	f36f 0347 	bfc	r3, #1, #7
 8003f4a:	70a3      	strb	r3, [r4, #2]
 8003f4c:	6863      	ldr	r3, [r4, #4]
 8003f4e:	f362 33d0 	bfi	r3, r2, #15, #2
 8003f52:	6063      	str	r3, [r4, #4]
 8003f54:	f3c3 4307 	ubfx	r3, r3, #16, #8
 8003f58:	f361 0343 	bfi	r3, r1, #1, #3
 8003f5c:	2102      	movs	r1, #2
 8003f5e:	f361 1305 	bfi	r3, r1, #4, #2
 8003f62:	71a3      	strb	r3, [r4, #6]
 8003f64:	88e3      	ldrh	r3, [r4, #6]
 8003f66:	f362 1388 	bfi	r3, r2, #6, #3
 8003f6a:	220c      	movs	r2, #12
 8003f6c:	80e3      	strh	r3, [r4, #6]
 8003f6e:	f3c3 2307 	ubfx	r3, r3, #8, #8
 8003f72:	f362 0345 	bfi	r3, r2, #1, #5
 8003f76:	71e3      	strb	r3, [r4, #7]
 8003f78:	eb0d 0502 	add.w	r5, sp, r2
 8003f7c:	4b16      	ldr	r3, [pc, #88]	; (8003fd8 <initSettings+0xa0>)
 8003f7e:	4629      	mov	r1, r5
 8003f80:	9300      	str	r3, [sp, #0]
 8003f82:	f7ff ffa9 	bl	8003ed8 <getFlashSettings.constprop.0>
 8003f86:	b318      	cbz	r0, 8003fd0 <initSettings+0x98>
 8003f88:	e895 0007 	ldmia.w	r5, {r0, r1, r2}
 8003f8c:	e884 0007 	stmia.w	r4, {r0, r1, r2}
 8003f90:	7923      	ldrb	r3, [r4, #4]
 8003f92:	9806      	ldr	r0, [sp, #24]
 8003f94:	f3c3 02c2 	ubfx	r2, r3, #3, #3
 8003f98:	2a05      	cmp	r2, #5
 8003f9a:	bf82      	ittt	hi
 8003f9c:	2205      	movhi	r2, #5
 8003f9e:	f362 03c5 	bfihi	r3, r2, #3, #3
 8003fa2:	7123      	strbhi	r3, [r4, #4]
 8003fa4:	88a3      	ldrh	r3, [r4, #4]
 8003fa6:	f3c3 1288 	ubfx	r2, r3, #6, #9
 8003faa:	f5b2 7fb4 	cmp.w	r2, #360	; 0x168
 8003fae:	bf22      	ittt	cs
 8003fb0:	f240 1267 	movwcs	r2, #359	; 0x167
 8003fb4:	f362 138e 	bfics	r3, r2, #6, #9
 8003fb8:	80a3      	strhcs	r3, [r4, #4]
 8003fba:	f7ff fb27 	bl	800360c <setDoseTime>
 8003fbe:	9807      	ldr	r0, [sp, #28]
 8003fc0:	f7ff fb30 	bl	8003624 <setDosePulseCount>
 8003fc4:	9808      	ldr	r0, [sp, #32]
 8003fc6:	f7ff fb39 	bl	800363c <setTubeTime>
 8003fca:	9809      	ldr	r0, [sp, #36]	; 0x24
 8003fcc:	f7ff fb42 	bl	8003654 <setTubePulseCount>
 8003fd0:	b00b      	add	sp, #44	; 0x2c
 8003fd2:	bd30      	pop	{r4, r5, pc}
 8003fd4:	20000da0 	andcs	r0, r0, r0, lsr #27
 8003fd8:	0800756b 	stmdaeq	r0, {r0, r1, r3, r5, r6, r8, sl, ip, sp, lr}

08003fdc <writeSettings>:
 8003fdc:	b510      	push	{r4, lr}
 8003fde:	b08a      	sub	sp, #40	; 0x28
 8003fe0:	ac03      	add	r4, sp, #12
 8003fe2:	4b0f      	ldr	r3, [pc, #60]	; (8004020 <writeSettings+0x44>)
 8003fe4:	4621      	mov	r1, r4
 8003fe6:	4668      	mov	r0, sp
 8003fe8:	9300      	str	r3, [sp, #0]
 8003fea:	f7ff ff75 	bl	8003ed8 <getFlashSettings.constprop.0>
 8003fee:	4b0d      	ldr	r3, [pc, #52]	; (8004024 <writeSettings+0x48>)
 8003ff0:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
 8003ff4:	e884 0007 	stmia.w	r4, {r0, r1, r2}
 8003ff8:	f7ff fb0e 	bl	8003618 <getDoseTime>
 8003ffc:	9006      	str	r0, [sp, #24]
 8003ffe:	f7ff fb17 	bl	8003630 <getDosePulseCount>
 8004002:	9007      	str	r0, [sp, #28]
 8004004:	f7ff fb20 	bl	8003648 <getTubeTime>
 8004008:	9008      	str	r0, [sp, #32]
 800400a:	f7ff fb29 	bl	8003660 <getTubePulseCount>
 800400e:	221c      	movs	r2, #28
 8004010:	9009      	str	r0, [sp, #36]	; 0x24
 8004012:	4621      	mov	r1, r4
 8004014:	4668      	mov	r0, sp
 8004016:	f7fe f81f 	bl	8002058 <writeFlashPage>
 800401a:	b00a      	add	sp, #40	; 0x28
 800401c:	bd10      	pop	{r4, pc}
 800401e:	bf00      	nop
 8004020:	0800756b 	stmdaeq	r0, {r0, r1, r3, r5, r6, r8, sl, ip, sp, lr}
 8004024:	20000da0 	andcs	r0, r0, r0, lsr #27

08004028 <onSettingsSubMenuBack>:
 8004028:	4801      	ldr	r0, [pc, #4]	; (8004030 <onSettingsSubMenuBack+0x8>)
 800402a:	f001 b92d 	b.w	8005288 <setView>
 800402e:	bf00      	nop
 8004030:	0800753c 	stmdaeq	r0, {r2, r3, r4, r5, r8, sl, ip, sp, lr}

08004034 <onDisplaySend>:
 8004034:	b510      	push	{r4, lr}
 8004036:	f44f 4100 	mov.w	r1, #32768	; 0x8000
 800403a:	f04f 4490 	mov.w	r4, #1207959552	; 0x48000000
 800403e:	0203      	lsls	r3, r0, #8
 8004040:	4a07      	ldr	r2, [pc, #28]	; (8004060 <onDisplaySend+0x2c>)
 8004042:	f403 431f 	and.w	r3, r3, #40704	; 0x9f00
 8004046:	f043 431f 	orr.w	r3, r3, #2667577344	; 0x9f000000
 800404a:	0040      	lsls	r0, r0, #1
 800404c:	6191      	str	r1, [r2, #24]
 800404e:	f000 00c0 	and.w	r0, r0, #192	; 0xc0
 8004052:	61a3      	str	r3, [r4, #24]
 8004054:	4b03      	ldr	r3, [pc, #12]	; (8004064 <onDisplaySend+0x30>)
 8004056:	f440 0040 	orr.w	r0, r0, #12582912	; 0xc00000
 800405a:	6198      	str	r0, [r3, #24]
 800405c:	6291      	str	r1, [r2, #40]	; 0x28
 800405e:	bd10      	pop	{r4, pc}
 8004060:	48000400 	stmdami	r0, {sl}
 8004064:	48001400 	stmdami	r0, {sl, ip}

08004068 <onDisplaySleep>:
 8004068:	f000 bb86 	b.w	8004778 <sleep>

0800406c <onDisplaySetReset>:
 800406c:	4b04      	ldr	r3, [pc, #16]	; (8004080 <onDisplaySetReset+0x14>)
 800406e:	695a      	ldr	r2, [r3, #20]
 8004070:	b918      	cbnz	r0, 800407a <onDisplaySetReset+0xe>
 8004072:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
 8004076:	615a      	str	r2, [r3, #20]
 8004078:	4770      	bx	lr
 800407a:	f422 5280 	bic.w	r2, r2, #4096	; 0x1000
 800407e:	e7fa      	b.n	8004076 <onDisplaySetReset+0xa>
 8004080:	48000400 	stmdami	r0, {sl}

08004084 <gpio_setup_output.constprop.2>:
 8004084:	2303      	movs	r3, #3
 8004086:	b570      	push	{r4, r5, r6, lr}
 8004088:	004e      	lsls	r6, r1, #1
 800408a:	40b3      	lsls	r3, r6
 800408c:	43dc      	mvns	r4, r3
 800408e:	2501      	movs	r5, #1
 8004090:	6802      	ldr	r2, [r0, #0]
 8004092:	fa05 f606 	lsl.w	r6, r5, r6
 8004096:	4022      	ands	r2, r4
 8004098:	4332      	orrs	r2, r6
 800409a:	6002      	str	r2, [r0, #0]
 800409c:	6842      	ldr	r2, [r0, #4]
 800409e:	408d      	lsls	r5, r1
 80040a0:	ea22 0505 	bic.w	r5, r2, r5
 80040a4:	6045      	str	r5, [r0, #4]
 80040a6:	6882      	ldr	r2, [r0, #8]
 80040a8:	4313      	orrs	r3, r2
 80040aa:	6083      	str	r3, [r0, #8]
 80040ac:	68c3      	ldr	r3, [r0, #12]
 80040ae:	401c      	ands	r4, r3
 80040b0:	60c4      	str	r4, [r0, #12]
 80040b2:	bd70      	pop	{r4, r5, r6, pc}

080040b4 <onDisplaySetCommand>:
 80040b4:	4b04      	ldr	r3, [pc, #16]	; (80040c8 <onDisplaySetCommand+0x14>)
 80040b6:	695a      	ldr	r2, [r3, #20]
 80040b8:	b918      	cbnz	r0, 80040c2 <onDisplaySetCommand+0xe>
 80040ba:	f442 5200 	orr.w	r2, r2, #8192	; 0x2000
 80040be:	615a      	str	r2, [r3, #20]
 80040c0:	4770      	bx	lr
 80040c2:	f422 5200 	bic.w	r2, r2, #8192	; 0x2000
 80040c6:	e7fa      	b.n	80040be <onDisplaySetCommand+0xa>
 80040c8:	48000400 	stmdami	r0, {sl}

080040cc <gpio_setup_input.constprop.3>:
 80040cc:	2303      	movs	r3, #3
 80040ce:	0049      	lsls	r1, r1, #1
 80040d0:	408b      	lsls	r3, r1
 80040d2:	43db      	mvns	r3, r3
 80040d4:	6802      	ldr	r2, [r0, #0]
 80040d6:	401a      	ands	r2, r3
 80040d8:	6002      	str	r2, [r0, #0]
 80040da:	68c2      	ldr	r2, [r0, #12]
 80040dc:	4013      	ands	r3, r2
 80040de:	2201      	movs	r2, #1
 80040e0:	fa02 f101 	lsl.w	r1, r2, r1
 80040e4:	430b      	orrs	r3, r1
 80040e6:	60c3      	str	r3, [r0, #12]
 80040e8:	4770      	bx	lr
	...

080040ec <initSystem>:
 80040ec:	4a02      	ldr	r2, [pc, #8]	; (80040f8 <initSystem+0xc>)
 80040ee:	6953      	ldr	r3, [r2, #20]
 80040f0:	f443 038c 	orr.w	r3, r3, #4587520	; 0x460000
 80040f4:	6153      	str	r3, [r2, #20]
 80040f6:	4770      	bx	lr
 80040f8:	40021000 	andmi	r1, r2, r0

080040fc <initKeyboardController>:
 80040fc:	b510      	push	{r4, lr}
 80040fe:	4c0b      	ldr	r4, [pc, #44]	; (800412c <initKeyboardController+0x30>)
 8004100:	2102      	movs	r1, #2
 8004102:	4620      	mov	r0, r4
 8004104:	f7ff ffe2 	bl	80040cc <gpio_setup_input.constprop.3>
 8004108:	2101      	movs	r1, #1
 800410a:	f7ff ffdf 	bl	80040cc <gpio_setup_input.constprop.3>
 800410e:	2107      	movs	r1, #7
 8004110:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
 8004114:	f7ff ffda 	bl	80040cc <gpio_setup_input.constprop.3>
 8004118:	2100      	movs	r1, #0
 800411a:	4620      	mov	r0, r4
 800411c:	f7ff ffd6 	bl	80040cc <gpio_setup_input.constprop.3>
 8004120:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8004124:	210b      	movs	r1, #11
 8004126:	f7ff bfd1 	b.w	80040cc <gpio_setup_input.constprop.3>
 800412a:	bf00      	nop
 800412c:	48000400 	stmdami	r0, {sl}

08004130 <getKeyboardState>:
 8004130:	4a10      	ldr	r2, [pc, #64]	; (8004174 <getKeyboardState+0x44>)
 8004132:	6913      	ldr	r3, [r2, #16]
 8004134:	f083 0304 	eor.w	r3, r3, #4
 8004138:	f3c3 0380 	ubfx	r3, r3, #2, #1
 800413c:	7003      	strb	r3, [r0, #0]
 800413e:	6913      	ldr	r3, [r2, #16]
 8004140:	f083 0302 	eor.w	r3, r3, #2
 8004144:	f3c3 0340 	ubfx	r3, r3, #1, #1
 8004148:	7043      	strb	r3, [r0, #1]
 800414a:	f04f 4390 	mov.w	r3, #1207959552	; 0x48000000
 800414e:	691b      	ldr	r3, [r3, #16]
 8004150:	f083 0380 	eor.w	r3, r3, #128	; 0x80
 8004154:	f3c3 13c0 	ubfx	r3, r3, #7, #1
 8004158:	7083      	strb	r3, [r0, #2]
 800415a:	6913      	ldr	r3, [r2, #16]
 800415c:	43db      	mvns	r3, r3
 800415e:	f003 0301 	and.w	r3, r3, #1
 8004162:	70c3      	strb	r3, [r0, #3]
 8004164:	6913      	ldr	r3, [r2, #16]
 8004166:	f483 6300 	eor.w	r3, r3, #2048	; 0x800
 800416a:	f3c3 23c0 	ubfx	r3, r3, #11, #1
 800416e:	7103      	strb	r3, [r0, #4]
 8004170:	4770      	bx	lr
 8004172:	bf00      	nop
 8004174:	48000400 	stmdami	r0, {sl}

08004178 <updateDisplayContrast>:
 8004178:	b510      	push	{r4, lr}
 800417a:	4c08      	ldr	r4, [pc, #32]	; (800419c <updateDisplayContrast+0x24>)
 800417c:	2181      	movs	r1, #129	; 0x81
 800417e:	4620      	mov	r0, r4
 8004180:	f001 f9b3 	bl	80054ea <mr_send_command>
 8004184:	4b06      	ldr	r3, [pc, #24]	; (80041a0 <updateDisplayContrast+0x28>)
 8004186:	4620      	mov	r0, r4
 8004188:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800418c:	7999      	ldrb	r1, [r3, #6]
 800418e:	f3c1 0142 	ubfx	r1, r1, #1, #3
 8004192:	0089      	lsls	r1, r1, #2
 8004194:	3114      	adds	r1, #20
 8004196:	f001 b9a8 	b.w	80054ea <mr_send_command>
 800419a:	bf00      	nop
 800419c:	20000cd0 	ldrdcs	r0, [r0], -r0	; <UNPREDICTABLE>
 80041a0:	20000da0 	andcs	r0, r0, r0, lsr #27

080041a4 <initDisplayController>:
 80041a4:	b510      	push	{r4, lr}
 80041a6:	4c2e      	ldr	r4, [pc, #184]	; (8004260 <initDisplayController+0xbc>)
 80041a8:	b086      	sub	sp, #24
 80041aa:	6963      	ldr	r3, [r4, #20]
 80041ac:	4620      	mov	r0, r4
 80041ae:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
 80041b2:	6163      	str	r3, [r4, #20]
 80041b4:	6963      	ldr	r3, [r4, #20]
 80041b6:	210c      	movs	r1, #12
 80041b8:	f423 4380 	bic.w	r3, r3, #16384	; 0x4000
 80041bc:	6163      	str	r3, [r4, #20]
 80041be:	6963      	ldr	r3, [r4, #20]
 80041c0:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
 80041c4:	6163      	str	r3, [r4, #20]
 80041c6:	f7ff ff5d 	bl	8004084 <gpio_setup_output.constprop.2>
 80041ca:	210d      	movs	r1, #13
 80041cc:	f7ff ff5a 	bl	8004084 <gpio_setup_output.constprop.2>
 80041d0:	210e      	movs	r1, #14
 80041d2:	f7ff ff57 	bl	8004084 <gpio_setup_output.constprop.2>
 80041d6:	210f      	movs	r1, #15
 80041d8:	f7ff ff54 	bl	8004084 <gpio_setup_output.constprop.2>
 80041dc:	2108      	movs	r1, #8
 80041de:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
 80041e2:	f7ff ff4f 	bl	8004084 <gpio_setup_output.constprop.2>
 80041e6:	2109      	movs	r1, #9
 80041e8:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
 80041ec:	f7ff ff4a 	bl	8004084 <gpio_setup_output.constprop.2>
 80041f0:	210a      	movs	r1, #10
 80041f2:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
 80041f6:	f7ff ff45 	bl	8004084 <gpio_setup_output.constprop.2>
 80041fa:	210b      	movs	r1, #11
 80041fc:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
 8004200:	f7ff ff40 	bl	8004084 <gpio_setup_output.constprop.2>
 8004204:	f504 5480 	add.w	r4, r4, #4096	; 0x1000
 8004208:	210c      	movs	r1, #12
 800420a:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
 800420e:	f7ff ff39 	bl	8004084 <gpio_setup_output.constprop.2>
 8004212:	4620      	mov	r0, r4
 8004214:	2106      	movs	r1, #6
 8004216:	f7ff ff35 	bl	8004084 <gpio_setup_output.constprop.2>
 800421a:	2107      	movs	r1, #7
 800421c:	f7ff ff32 	bl	8004084 <gpio_setup_output.constprop.2>
 8004220:	210f      	movs	r1, #15
 8004222:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
 8004226:	f7ff ff2d 	bl	8004084 <gpio_setup_output.constprop.2>
 800422a:	4b0e      	ldr	r3, [pc, #56]	; (8004264 <initDisplayController+0xc0>)
 800422c:	4c0e      	ldr	r4, [pc, #56]	; (8004268 <initDisplayController+0xc4>)
 800422e:	9304      	str	r3, [sp, #16]
 8004230:	4b0e      	ldr	r3, [pc, #56]	; (800426c <initDisplayController+0xc8>)
 8004232:	2240      	movs	r2, #64	; 0x40
 8004234:	9303      	str	r3, [sp, #12]
 8004236:	4b0e      	ldr	r3, [pc, #56]	; (8004270 <initDisplayController+0xcc>)
 8004238:	4620      	mov	r0, r4
 800423a:	9302      	str	r3, [sp, #8]
 800423c:	4b0d      	ldr	r3, [pc, #52]	; (8004274 <initDisplayController+0xd0>)
 800423e:	2180      	movs	r1, #128	; 0x80
 8004240:	9301      	str	r3, [sp, #4]
 8004242:	4b0d      	ldr	r3, [pc, #52]	; (8004278 <initDisplayController+0xd4>)
 8004244:	9300      	str	r3, [sp, #0]
 8004246:	2300      	movs	r3, #0
 8004248:	f001 f872 	bl	8005330 <mr_st7565_init>
 800424c:	490b      	ldr	r1, [pc, #44]	; (800427c <initDisplayController+0xd8>)
 800424e:	4620      	mov	r0, r4
 8004250:	f001 f961 	bl	8005516 <mr_send_sequence>
 8004254:	b006      	add	sp, #24
 8004256:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800425a:	f7ff bf8d 	b.w	8004178 <updateDisplayContrast>
 800425e:	bf00      	nop
 8004260:	48000400 	stmdami	r0, {sl}
 8004264:	08004035 	stmdaeq	r0, {r0, r2, r4, r5, lr}
 8004268:	20000cd0 	ldrdcs	r0, [r0], -r0	; <UNPREDICTABLE>
 800426c:	080040b5 	stmdaeq	r0, {r0, r2, r4, r5, r7, lr}
 8004270:	0800406d 	stmdaeq	r0, {r0, r2, r3, r5, r6, lr}
 8004274:	08004069 	stmdaeq	r0, {r0, r3, r5, r6, lr}
 8004278:	20000744 	andcs	r0, r0, r4, asr #14
 800427c:	08007560 	stmdaeq	r0, {r5, r6, r8, sl, ip, sp, lr}

08004280 <readADC>:
 8004280:	b538      	push	{r3, r4, r5, lr}
 8004282:	4d1b      	ldr	r5, [pc, #108]	; (80042f0 <readADC+0x70>)
 8004284:	4604      	mov	r4, r0
 8004286:	f7fd fded 	bl	8001e64 <syncTimerThread>
 800428a:	68ab      	ldr	r3, [r5, #8]
 800428c:	2001      	movs	r0, #1
 800428e:	07db      	lsls	r3, r3, #31
 8004290:	bf5e      	ittt	pl
 8004292:	68ab      	ldrpl	r3, [r5, #8]
 8004294:	f043 0301 	orrpl.w	r3, r3, #1
 8004298:	60ab      	strpl	r3, [r5, #8]
 800429a:	f000 fa6d 	bl	8004778 <sleep>
 800429e:	2300      	movs	r3, #0
 80042a0:	62eb      	str	r3, [r5, #44]	; 0x2c
 80042a2:	636c      	str	r4, [r5, #52]	; 0x34
 80042a4:	68ab      	ldr	r3, [r5, #8]
 80042a6:	2c09      	cmp	r4, #9
 80042a8:	f443 13f0 	orr.w	r3, r3, #1966080	; 0x1e0000
 80042ac:	60ab      	str	r3, [r5, #8]
 80042ae:	f04f 0307 	mov.w	r3, #7
 80042b2:	bf85      	ittet	hi
 80042b4:	3c0a      	subhi	r4, #10
 80042b6:	eb04 0444 	addhi.w	r4, r4, r4, lsl #1
 80042ba:	eb04 0444 	addls.w	r4, r4, r4, lsl #1
 80042be:	fa03 f404 	lslhi.w	r4, r3, r4
 80042c2:	bf96      	itet	ls
 80042c4:	fa03 f404 	lslls.w	r4, r3, r4
 80042c8:	60ec      	strhi	r4, [r5, #12]
 80042ca:	612c      	strls	r4, [r5, #16]
 80042cc:	4c08      	ldr	r4, [pc, #32]	; (80042f0 <readADC+0x70>)
 80042ce:	2001      	movs	r0, #1
 80042d0:	68a3      	ldr	r3, [r4, #8]
 80042d2:	f443 0380 	orr.w	r3, r3, #4194304	; 0x400000
 80042d6:	60a3      	str	r3, [r4, #8]
 80042d8:	f000 fa4e 	bl	8004778 <sleep>
 80042dc:	6c25      	ldr	r5, [r4, #64]	; 0x40
 80042de:	68a3      	ldr	r3, [r4, #8]
 80042e0:	2001      	movs	r0, #1
 80042e2:	f023 0301 	bic.w	r3, r3, #1
 80042e6:	60a3      	str	r3, [r4, #8]
 80042e8:	f000 fa46 	bl	8004778 <sleep>
 80042ec:	4628      	mov	r0, r5
 80042ee:	bd38      	pop	{r3, r4, r5, pc}
 80042f0:	40012400 	andmi	r2, r1, r0, lsl #8

080042f4 <updateADC>:
 80042f4:	b570      	push	{r4, r5, r6, lr}
 80042f6:	2004      	movs	r0, #4
 80042f8:	f7ff ffc2 	bl	8004280 <readADC>
 80042fc:	f000 f8ce 	bl	800449c <__aeabi_ui2f>
 8004300:	4c1b      	ldr	r4, [pc, #108]	; (8004370 <updateADC+0x7c>)
 8004302:	4605      	mov	r5, r0
 8004304:	6826      	ldr	r6, [r4, #0]
 8004306:	2100      	movs	r1, #0
 8004308:	4630      	mov	r0, r6
 800430a:	f000 f8d2 	bl	80044b2 <__aeabi_fcmpeq>
 800430e:	b958      	cbnz	r0, 8004328 <updateADC+0x34>
 8004310:	4629      	mov	r1, r5
 8004312:	4630      	mov	r0, r6
 8004314:	f000 f8bc 	bl	8004490 <__aeabi_fsub>
 8004318:	4916      	ldr	r1, [pc, #88]	; (8004374 <updateADC+0x80>)
 800431a:	f000 f8bb 	bl	8004494 <__aeabi_fmul>
 800431e:	4601      	mov	r1, r0
 8004320:	4628      	mov	r0, r5
 8004322:	f000 f8b3 	bl	800448c <__aeabi_fadd>
 8004326:	4605      	mov	r5, r0
 8004328:	4e13      	ldr	r6, [pc, #76]	; (8004378 <updateADC+0x84>)
 800432a:	6025      	str	r5, [r4, #0]
 800432c:	68b3      	ldr	r3, [r6, #8]
 800432e:	2010      	movs	r0, #16
 8004330:	f443 0300 	orr.w	r3, r3, #8388608	; 0x800000
 8004334:	60b3      	str	r3, [r6, #8]
 8004336:	f7ff ffa3 	bl	8004280 <readADC>
 800433a:	f000 f8af 	bl	800449c <__aeabi_ui2f>
 800433e:	68b3      	ldr	r3, [r6, #8]
 8004340:	4605      	mov	r5, r0
 8004342:	f423 0300 	bic.w	r3, r3, #8388608	; 0x800000
 8004346:	60b3      	str	r3, [r6, #8]
 8004348:	6866      	ldr	r6, [r4, #4]
 800434a:	2100      	movs	r1, #0
 800434c:	4630      	mov	r0, r6
 800434e:	f000 f8b0 	bl	80044b2 <__aeabi_fcmpeq>
 8004352:	b958      	cbnz	r0, 800436c <updateADC+0x78>
 8004354:	4629      	mov	r1, r5
 8004356:	4630      	mov	r0, r6
 8004358:	f000 f89a 	bl	8004490 <__aeabi_fsub>
 800435c:	4907      	ldr	r1, [pc, #28]	; (800437c <updateADC+0x88>)
 800435e:	f000 f899 	bl	8004494 <__aeabi_fmul>
 8004362:	4601      	mov	r1, r0
 8004364:	4628      	mov	r0, r5
 8004366:	f000 f891 	bl	800448c <__aeabi_fadd>
 800436a:	4605      	mov	r5, r0
 800436c:	6065      	str	r5, [r4, #4]
 800436e:	bd70      	pop	{r4, r5, r6, pc}
 8004370:	20000b44 	andcs	r0, r0, r4, asr #22
 8004374:	3f7d70a4 	svccc	0x007d70a4
 8004378:	40012400 	andmi	r2, r1, r0, lsl #8
 800437c:	3f779b78 	svccc	0x00779b78

08004380 <initADC>:
 8004380:	b510      	push	{r4, lr}
 8004382:	f7fd fd6f 	bl	8001e64 <syncTimerThread>
 8004386:	4b18      	ldr	r3, [pc, #96]	; (80043e8 <initADC+0x68>)
 8004388:	6b5a      	ldr	r2, [r3, #52]	; 0x34
 800438a:	f042 0201 	orr.w	r2, r2, #1
 800438e:	635a      	str	r2, [r3, #52]	; 0x34
 8004390:	6b5a      	ldr	r2, [r3, #52]	; 0x34
 8004392:	0792      	lsls	r2, r2, #30
 8004394:	d5fc      	bpl.n	8004390 <initADC+0x10>
 8004396:	699a      	ldr	r2, [r3, #24]
 8004398:	4c14      	ldr	r4, [pc, #80]	; (80043ec <initADC+0x6c>)
 800439a:	f442 7200 	orr.w	r2, r2, #512	; 0x200
 800439e:	619a      	str	r2, [r3, #24]
 80043a0:	68a3      	ldr	r3, [r4, #8]
 80043a2:	2001      	movs	r0, #1
 80043a4:	07db      	lsls	r3, r3, #31
 80043a6:	bf5e      	ittt	pl
 80043a8:	68a3      	ldrpl	r3, [r4, #8]
 80043aa:	f043 0301 	orrpl.w	r3, r3, #1
 80043ae:	60a3      	strpl	r3, [r4, #8]
 80043b0:	f000 f9e2 	bl	8004778 <sleep>
 80043b4:	68a3      	ldr	r3, [r4, #8]
 80043b6:	2001      	movs	r0, #1
 80043b8:	f043 0308 	orr.w	r3, r3, #8
 80043bc:	60a3      	str	r3, [r4, #8]
 80043be:	f000 f9db 	bl	8004778 <sleep>
 80043c2:	68a3      	ldr	r3, [r4, #8]
 80043c4:	2001      	movs	r0, #1
 80043c6:	f043 0304 	orr.w	r3, r3, #4
 80043ca:	60a3      	str	r3, [r4, #8]
 80043cc:	f000 f9d4 	bl	8004778 <sleep>
 80043d0:	68a3      	ldr	r3, [r4, #8]
 80043d2:	2001      	movs	r0, #1
 80043d4:	f023 0301 	bic.w	r3, r3, #1
 80043d8:	60a3      	str	r3, [r4, #8]
 80043da:	f000 f9cd 	bl	8004778 <sleep>
 80043de:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 80043e2:	f7ff bf87 	b.w	80042f4 <updateADC>
 80043e6:	bf00      	nop
 80043e8:	40021000 	andmi	r1, r2, r0
 80043ec:	40012400 	andmi	r2, r1, r0, lsl #8

080043f0 <getDeviceBatteryVoltage>:
 80043f0:	b508      	push	{r3, lr}
 80043f2:	4b03      	ldr	r3, [pc, #12]	; (8004400 <getDeviceBatteryVoltage+0x10>)
 80043f4:	4903      	ldr	r1, [pc, #12]	; (8004404 <getDeviceBatteryVoltage+0x14>)
 80043f6:	6818      	ldr	r0, [r3, #0]
 80043f8:	f000 f84c 	bl	8004494 <__aeabi_fmul>
 80043fc:	bd08      	pop	{r3, pc}
 80043fe:	bf00      	nop
 8004400:	20000b44 	andcs	r0, r0, r4, asr #22
 8004404:	39d34067 	ldmibcc	r3, {r0, r1, r2, r5, r6, lr}^

08004408 <getDeviceTemperature>:
 8004408:	b508      	push	{r3, lr}
 800440a:	4b08      	ldr	r3, [pc, #32]	; (800442c <getDeviceTemperature+0x24>)
 800440c:	4908      	ldr	r1, [pc, #32]	; (8004430 <getDeviceTemperature+0x28>)
 800440e:	6858      	ldr	r0, [r3, #4]
 8004410:	f000 f840 	bl	8004494 <__aeabi_fmul>
 8004414:	4601      	mov	r1, r0
 8004416:	4807      	ldr	r0, [pc, #28]	; (8004434 <getDeviceTemperature+0x2c>)
 8004418:	f000 f83a 	bl	8004490 <__aeabi_fsub>
 800441c:	4906      	ldr	r1, [pc, #24]	; (8004438 <getDeviceTemperature+0x30>)
 800441e:	f000 f83b 	bl	8004498 <__aeabi_fdiv>
 8004422:	4906      	ldr	r1, [pc, #24]	; (800443c <getDeviceTemperature+0x34>)
 8004424:	f000 f832 	bl	800448c <__aeabi_fadd>
 8004428:	bd08      	pop	{r3, pc}
 800442a:	bf00      	nop
 800442c:	20000b44 	andcs	r0, r0, r4, asr #22
 8004430:	3a534067 	bcc	94d45d4 <_sidata+0x14ccd84>
 8004434:	3fb9999a 	svccc	0x00b9999a
 8004438:	3b86594b 	blcc	619a96c <_Min_Stack_Size+0x619a56c>
 800443c:	41c80000 	bicmi	r0, r8, r0

08004440 <initBuzzerController>:
 8004440:	4b0b      	ldr	r3, [pc, #44]	; (8004470 <initBuzzerController+0x30>)
 8004442:	695a      	ldr	r2, [r3, #20]
 8004444:	f022 0220 	bic.w	r2, r2, #32
 8004448:	615a      	str	r2, [r3, #20]
 800444a:	681a      	ldr	r2, [r3, #0]
 800444c:	f422 6240 	bic.w	r2, r2, #3072	; 0xc00
 8004450:	f442 6280 	orr.w	r2, r2, #1024	; 0x400
 8004454:	601a      	str	r2, [r3, #0]
 8004456:	685a      	ldr	r2, [r3, #4]
 8004458:	f022 0220 	bic.w	r2, r2, #32
 800445c:	605a      	str	r2, [r3, #4]
 800445e:	689a      	ldr	r2, [r3, #8]
 8004460:	f422 6240 	bic.w	r2, r2, #3072	; 0xc00
 8004464:	609a      	str	r2, [r3, #8]
 8004466:	68da      	ldr	r2, [r3, #12]
 8004468:	f422 6240 	bic.w	r2, r2, #3072	; 0xc00
 800446c:	60da      	str	r2, [r3, #12]
 800446e:	4770      	bx	lr
 8004470:	48000400 	stmdami	r0, {sl}

08004474 <setBuzzer>:
 8004474:	4b04      	ldr	r3, [pc, #16]	; (8004488 <setBuzzer+0x14>)
 8004476:	695a      	ldr	r2, [r3, #20]
 8004478:	b118      	cbz	r0, 8004482 <setBuzzer+0xe>
 800447a:	f042 0220 	orr.w	r2, r2, #32
 800447e:	615a      	str	r2, [r3, #20]
 8004480:	4770      	bx	lr
 8004482:	f022 0220 	bic.w	r2, r2, #32
 8004486:	e7fa      	b.n	800447e <setBuzzer+0xa>
 8004488:	48000400 	stmdami	r0, {sl}

0800448c <__aeabi_fadd>:
 800448c:	f7fb be9f 	b.w	80001ce <qfp_fadd>

08004490 <__aeabi_fsub>:
 8004490:	f7fb be9a 	b.w	80001c8 <qfp_fsub>

08004494 <__aeabi_fmul>:
 8004494:	f7fb beca 	b.w	800022c <qfp_fmul>

08004498 <__aeabi_fdiv>:
 8004498:	f7fb bed0 	b.w	800023c <qfp_fdiv>

0800449c <__aeabi_ui2f>:
 800449c:	f7fb bf7b 	b.w	8000396 <qfp_uint2float>

080044a0 <__aeabi_f2iz>:
 80044a0:	b508      	push	{r3, lr}
 80044a2:	f7fb ff4f 	bl	8000344 <qfp_float2int>
 80044a6:	2800      	cmp	r0, #0
 80044a8:	bfb8      	it	lt
 80044aa:	3001      	addlt	r0, #1
 80044ac:	bd08      	pop	{r3, pc}

080044ae <__aeabi_f2uiz>:
 80044ae:	f7fb bf5e 	b.w	800036e <qfp_float2uint>

080044b2 <__aeabi_fcmpeq>:
 80044b2:	b508      	push	{r3, lr}
 80044b4:	f7fb feac 	bl	8000210 <qfp_fcmp>
 80044b8:	fab0 f080 	clz	r0, r0
 80044bc:	0940      	lsrs	r0, r0, #5
 80044be:	bd08      	pop	{r3, pc}

080044c0 <__aeabi_fcmplt>:
 80044c0:	b508      	push	{r3, lr}
 80044c2:	f7fb fea5 	bl	8000210 <qfp_fcmp>
 80044c6:	0fc0      	lsrs	r0, r0, #31
 80044c8:	bd08      	pop	{r3, pc}

080044ca <__aeabi_fcmpge>:
 80044ca:	b508      	push	{r3, lr}
 80044cc:	f7fb fea0 	bl	8000210 <qfp_fcmp>
 80044d0:	43c0      	mvns	r0, r0
 80044d2:	0fc0      	lsrs	r0, r0, #31
 80044d4:	bd08      	pop	{r3, pc}

080044d6 <__aeabi_fcmpgt>:
 80044d6:	b508      	push	{r3, lr}
 80044d8:	f7fb fe9a 	bl	8000210 <qfp_fcmp>
 80044dc:	2800      	cmp	r0, #0
 80044de:	bfd4      	ite	le
 80044e0:	2000      	movle	r0, #0
 80044e2:	2001      	movgt	r0, #1
 80044e4:	bd08      	pop	{r3, pc}
	...

080044e8 <exp2f>:
 80044e8:	b508      	push	{r3, lr}
 80044ea:	4903      	ldr	r1, [pc, #12]	; (80044f8 <exp2f+0x10>)
 80044ec:	f7ff ffd2 	bl	8004494 <__aeabi_fmul>
 80044f0:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 80044f4:	f7fb bfc0 	b.w	8000478 <qfp_fexp>
 80044f8:	3f317218 	svccc	0x00317218

080044fc <log10f>:
 80044fc:	b508      	push	{r3, lr}
 80044fe:	f7fb fff0 	bl	80004e2 <qfp_fln>
 8004502:	4902      	ldr	r1, [pc, #8]	; (800450c <log10f+0x10>)
 8004504:	f7ff ffc6 	bl	8004494 <__aeabi_fmul>
 8004508:	bd08      	pop	{r3, pc}
 800450a:	bf00      	nop
 800450c:	3ede5bd9 	vmovcc.u8	r5, d30[2]

08004510 <powf>:
 8004510:	b510      	push	{r4, lr}
 8004512:	460c      	mov	r4, r1
 8004514:	f7fb ffe5 	bl	80004e2 <qfp_fln>
 8004518:	4621      	mov	r1, r4
 800451a:	f7ff ffbb 	bl	8004494 <__aeabi_fmul>
 800451e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8004522:	f7fb bfa9 	b.w	8000478 <qfp_fexp>

08004526 <sqrtf>:
 8004526:	f7fb bfbf 	b.w	80004a8 <qfp_fsqrt>
	...

0800452c <initComm>:
 800452c:	f04f 4390 	mov.w	r3, #1207959552	; 0x48000000
 8004530:	681a      	ldr	r2, [r3, #0]
 8004532:	f022 02c0 	bic.w	r2, r2, #192	; 0xc0
 8004536:	f042 0280 	orr.w	r2, r2, #128	; 0x80
 800453a:	601a      	str	r2, [r3, #0]
 800453c:	685a      	ldr	r2, [r3, #4]
 800453e:	f022 0208 	bic.w	r2, r2, #8
 8004542:	605a      	str	r2, [r3, #4]
 8004544:	689a      	ldr	r2, [r3, #8]
 8004546:	f042 02c0 	orr.w	r2, r2, #192	; 0xc0
 800454a:	609a      	str	r2, [r3, #8]
 800454c:	68da      	ldr	r2, [r3, #12]
 800454e:	f022 02c0 	bic.w	r2, r2, #192	; 0xc0
 8004552:	60da      	str	r2, [r3, #12]
 8004554:	6a1a      	ldr	r2, [r3, #32]
 8004556:	f422 4270 	bic.w	r2, r2, #61440	; 0xf000
 800455a:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
 800455e:	621a      	str	r2, [r3, #32]
 8004560:	681a      	ldr	r2, [r3, #0]
 8004562:	f022 0230 	bic.w	r2, r2, #48	; 0x30
 8004566:	f042 0220 	orr.w	r2, r2, #32
 800456a:	601a      	str	r2, [r3, #0]
 800456c:	685a      	ldr	r2, [r3, #4]
 800456e:	f022 0204 	bic.w	r2, r2, #4
 8004572:	605a      	str	r2, [r3, #4]
 8004574:	689a      	ldr	r2, [r3, #8]
 8004576:	f042 0230 	orr.w	r2, r2, #48	; 0x30
 800457a:	609a      	str	r2, [r3, #8]
 800457c:	68da      	ldr	r2, [r3, #12]
 800457e:	f022 0230 	bic.w	r2, r2, #48	; 0x30
 8004582:	60da      	str	r2, [r3, #12]
 8004584:	6a1a      	ldr	r2, [r3, #32]
 8004586:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
 800458a:	f442 7280 	orr.w	r2, r2, #256	; 0x100
 800458e:	621a      	str	r2, [r3, #32]
 8004590:	4a0d      	ldr	r2, [pc, #52]	; (80045c8 <initComm+0x9c>)
 8004592:	69d3      	ldr	r3, [r2, #28]
 8004594:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
 8004598:	61d3      	str	r3, [r2, #28]
 800459a:	2245      	movs	r2, #69	; 0x45
 800459c:	4b0b      	ldr	r3, [pc, #44]	; (80045cc <initComm+0xa0>)
 800459e:	60da      	str	r2, [r3, #12]
 80045a0:	220d      	movs	r2, #13
 80045a2:	601a      	str	r2, [r3, #0]
 80045a4:	2200      	movs	r2, #0
 80045a6:	605a      	str	r2, [r3, #4]
 80045a8:	609a      	str	r2, [r3, #8]
 80045aa:	681a      	ldr	r2, [r3, #0]
 80045ac:	f042 0220 	orr.w	r2, r2, #32
 80045b0:	601a      	str	r2, [r3, #0]
 80045b2:	4b07      	ldr	r3, [pc, #28]	; (80045d0 <initComm+0xa4>)
 80045b4:	f8d3 231c 	ldr.w	r2, [r3, #796]	; 0x31c
 80045b8:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
 80045bc:	f8c3 231c 	str.w	r2, [r3, #796]	; 0x31c
 80045c0:	f04f 5280 	mov.w	r2, #268435456	; 0x10000000
 80045c4:	601a      	str	r2, [r3, #0]
 80045c6:	4770      	bx	lr
 80045c8:	40021000 	andmi	r1, r2, r0
 80045cc:	40004400 	andmi	r4, r0, r0, lsl #8
 80045d0:	e000e100 	and	lr, r0, r0, lsl #2

080045d4 <transmitComm>:
 80045d4:	2200      	movs	r2, #0
 80045d6:	4b06      	ldr	r3, [pc, #24]	; (80045f0 <transmitComm+0x1c>)
 80045d8:	605a      	str	r2, [r3, #4]
 80045da:	2202      	movs	r2, #2
 80045dc:	705a      	strb	r2, [r3, #1]
 80045de:	781b      	ldrb	r3, [r3, #0]
 80045e0:	b923      	cbnz	r3, 80045ec <transmitComm+0x18>
 80045e2:	4a04      	ldr	r2, [pc, #16]	; (80045f4 <transmitComm+0x20>)
 80045e4:	6813      	ldr	r3, [r2, #0]
 80045e6:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 80045ea:	6013      	str	r3, [r2, #0]
 80045ec:	4770      	bx	lr
 80045ee:	bf00      	nop
 80045f0:	20000c7c 	andcs	r0, r0, ip, ror ip
 80045f4:	40004400 	andmi	r4, r0, r0, lsl #8

080045f8 <USART2_IRQHandler>:
 80045f8:	4b1e      	ldr	r3, [pc, #120]	; (8004674 <USART2_IRQHandler+0x7c>)
 80045fa:	69da      	ldr	r2, [r3, #28]
 80045fc:	0692      	lsls	r2, r2, #26
 80045fe:	d517      	bpl.n	8004630 <USART2_IRQHandler+0x38>
 8004600:	8c9a      	ldrh	r2, [r3, #36]	; 0x24
 8004602:	4b1d      	ldr	r3, [pc, #116]	; (8004678 <USART2_IRQHandler+0x80>)
 8004604:	b292      	uxth	r2, r2
 8004606:	f893 1048 	ldrb.w	r1, [r3, #72]	; 0x48
 800460a:	2900      	cmp	r1, #0
 800460c:	d031      	beq.n	8004672 <USART2_IRQHandler+0x7a>
 800460e:	2100      	movs	r1, #0
 8004610:	7019      	strb	r1, [r3, #0]
 8004612:	7859      	ldrb	r1, [r3, #1]
 8004614:	f001 00ff 	and.w	r0, r1, #255	; 0xff
 8004618:	b951      	cbnz	r1, 8004630 <USART2_IRQHandler+0x38>
 800461a:	b2d2      	uxtb	r2, r2
 800461c:	2a1f      	cmp	r2, #31
 800461e:	d91a      	bls.n	8004656 <USART2_IRQHandler+0x5e>
 8004620:	6859      	ldr	r1, [r3, #4]
 8004622:	293e      	cmp	r1, #62	; 0x3e
 8004624:	d804      	bhi.n	8004630 <USART2_IRQHandler+0x38>
 8004626:	6859      	ldr	r1, [r3, #4]
 8004628:	1c48      	adds	r0, r1, #1
 800462a:	6058      	str	r0, [r3, #4]
 800462c:	440b      	add	r3, r1
 800462e:	721a      	strb	r2, [r3, #8]
 8004630:	4a10      	ldr	r2, [pc, #64]	; (8004674 <USART2_IRQHandler+0x7c>)
 8004632:	69d3      	ldr	r3, [r2, #28]
 8004634:	061b      	lsls	r3, r3, #24
 8004636:	d51c      	bpl.n	8004672 <USART2_IRQHandler+0x7a>
 8004638:	4b0f      	ldr	r3, [pc, #60]	; (8004678 <USART2_IRQHandler+0x80>)
 800463a:	7859      	ldrb	r1, [r3, #1]
 800463c:	2902      	cmp	r1, #2
 800463e:	d118      	bne.n	8004672 <USART2_IRQHandler+0x7a>
 8004640:	6859      	ldr	r1, [r3, #4]
 8004642:	4419      	add	r1, r3
 8004644:	7a09      	ldrb	r1, [r1, #8]
 8004646:	b171      	cbz	r1, 8004666 <USART2_IRQHandler+0x6e>
 8004648:	6859      	ldr	r1, [r3, #4]
 800464a:	1c48      	adds	r0, r1, #1
 800464c:	6058      	str	r0, [r3, #4]
 800464e:	440b      	add	r3, r1
 8004650:	7a1b      	ldrb	r3, [r3, #8]
 8004652:	8513      	strh	r3, [r2, #40]	; 0x28
 8004654:	4770      	bx	lr
 8004656:	2a0a      	cmp	r2, #10
 8004658:	d1ea      	bne.n	8004630 <USART2_IRQHandler+0x38>
 800465a:	685a      	ldr	r2, [r3, #4]
 800465c:	441a      	add	r2, r3
 800465e:	7210      	strb	r0, [r2, #8]
 8004660:	2201      	movs	r2, #1
 8004662:	705a      	strb	r2, [r3, #1]
 8004664:	e7e4      	b.n	8004630 <USART2_IRQHandler+0x38>
 8004666:	6811      	ldr	r1, [r2, #0]
 8004668:	f021 0180 	bic.w	r1, r1, #128	; 0x80
 800466c:	6011      	str	r1, [r2, #0]
 800466e:	2203      	movs	r2, #3
 8004670:	705a      	strb	r2, [r3, #1]
 8004672:	4770      	bx	lr
 8004674:	40004400 	andmi	r4, r0, r0, lsl #8
 8004678:	20000c7c 	andcs	r0, r0, ip, ror ip

0800467c <updateCommController>:
 800467c:	4770      	bx	lr
	...

08004680 <initDisplayBacklight>:
 8004680:	4b19      	ldr	r3, [pc, #100]	; (80046e8 <initDisplayBacklight+0x68>)
 8004682:	681a      	ldr	r2, [r3, #0]
 8004684:	f022 02c0 	bic.w	r2, r2, #192	; 0xc0
 8004688:	f042 0280 	orr.w	r2, r2, #128	; 0x80
 800468c:	601a      	str	r2, [r3, #0]
 800468e:	685a      	ldr	r2, [r3, #4]
 8004690:	f022 0208 	bic.w	r2, r2, #8
 8004694:	605a      	str	r2, [r3, #4]
 8004696:	689a      	ldr	r2, [r3, #8]
 8004698:	f022 02c0 	bic.w	r2, r2, #192	; 0xc0
 800469c:	609a      	str	r2, [r3, #8]
 800469e:	68da      	ldr	r2, [r3, #12]
 80046a0:	f022 02c0 	bic.w	r2, r2, #192	; 0xc0
 80046a4:	60da      	str	r2, [r3, #12]
 80046a6:	6a1a      	ldr	r2, [r3, #32]
 80046a8:	f422 4270 	bic.w	r2, r2, #61440	; 0xf000
 80046ac:	f442 5200 	orr.w	r2, r2, #8192	; 0x2000
 80046b0:	621a      	str	r2, [r3, #32]
 80046b2:	4a0e      	ldr	r2, [pc, #56]	; (80046ec <initDisplayBacklight+0x6c>)
 80046b4:	69d3      	ldr	r3, [r2, #28]
 80046b6:	f043 0301 	orr.w	r3, r3, #1
 80046ba:	61d3      	str	r3, [r2, #28]
 80046bc:	f04f 4380 	mov.w	r3, #1073741824	; 0x40000000
 80046c0:	699a      	ldr	r2, [r3, #24]
 80046c2:	f422 42e0 	bic.w	r2, r2, #28672	; 0x7000
 80046c6:	f442 42c0 	orr.w	r2, r2, #24576	; 0x6000
 80046ca:	619a      	str	r2, [r3, #24]
 80046cc:	6a1a      	ldr	r2, [r3, #32]
 80046ce:	f042 0210 	orr.w	r2, r2, #16
 80046d2:	621a      	str	r2, [r3, #32]
 80046d4:	2200      	movs	r2, #0
 80046d6:	639a      	str	r2, [r3, #56]	; 0x38
 80046d8:	f641 723f 	movw	r2, #7999	; 0x1f3f
 80046dc:	62da      	str	r2, [r3, #44]	; 0x2c
 80046de:	681a      	ldr	r2, [r3, #0]
 80046e0:	f042 0201 	orr.w	r2, r2, #1
 80046e4:	601a      	str	r2, [r3, #0]
 80046e6:	4770      	bx	lr
 80046e8:	48000400 	stmdami	r0, {sl}
 80046ec:	40021000 	andmi	r1, r2, r0

080046f0 <setDisplayBacklight>:
 80046f0:	b138      	cbz	r0, 8004702 <setDisplayBacklight+0x12>
 80046f2:	4b06      	ldr	r3, [pc, #24]	; (800470c <setDisplayBacklight+0x1c>)
 80046f4:	4a06      	ldr	r2, [pc, #24]	; (8004710 <setDisplayBacklight+0x20>)
 80046f6:	799b      	ldrb	r3, [r3, #6]
 80046f8:	f3c3 1301 	ubfx	r3, r3, #4, #2
 80046fc:	f832 0013 	ldrh.w	r0, [r2, r3, lsl #1]
 8004700:	00c0      	lsls	r0, r0, #3
 8004702:	f04f 4380 	mov.w	r3, #1073741824	; 0x40000000
 8004706:	6398      	str	r0, [r3, #56]	; 0x38
 8004708:	4770      	bx	lr
 800470a:	bf00      	nop
 800470c:	20000da0 	andcs	r0, r0, r0, lsr #27
 8004710:	20000004 	andcs	r0, r0, r4

08004714 <initEventsController>:
 8004714:	f641 723f 	movw	r2, #7999	; 0x1f3f
 8004718:	4b0e      	ldr	r3, [pc, #56]	; (8004754 <initEventsController+0x40>)
 800471a:	490f      	ldr	r1, [pc, #60]	; (8004758 <initEventsController+0x44>)
 800471c:	605a      	str	r2, [r3, #4]
 800471e:	2200      	movs	r2, #0
 8004720:	609a      	str	r2, [r3, #8]
 8004722:	6a0a      	ldr	r2, [r1, #32]
 8004724:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
 8004728:	620a      	str	r2, [r1, #32]
 800472a:	2207      	movs	r2, #7
 800472c:	601a      	str	r2, [r3, #0]
 800472e:	4b0b      	ldr	r3, [pc, #44]	; (800475c <initEventsController+0x48>)
 8004730:	461a      	mov	r2, r3
 8004732:	68d9      	ldr	r1, [r3, #12]
 8004734:	07c9      	lsls	r1, r1, #31
 8004736:	d4fc      	bmi.n	8004732 <initEventsController+0x1e>
 8004738:	f245 5155 	movw	r1, #21845	; 0x5555
 800473c:	6019      	str	r1, [r3, #0]
 800473e:	2101      	movs	r1, #1
 8004740:	6059      	str	r1, [r3, #4]
 8004742:	68d3      	ldr	r3, [r2, #12]
 8004744:	079b      	lsls	r3, r3, #30
 8004746:	d4fc      	bmi.n	8004742 <initEventsController+0x2e>
 8004748:	f245 5355 	movw	r3, #21845	; 0x5555
 800474c:	6013      	str	r3, [r2, #0]
 800474e:	4b04      	ldr	r3, [pc, #16]	; (8004760 <initEventsController+0x4c>)
 8004750:	6093      	str	r3, [r2, #8]
 8004752:	4770      	bx	lr
 8004754:	e000e010 	and	lr, r0, r0, lsl r0
 8004758:	e000ed00 	and	lr, r0, r0, lsl #26
 800475c:	40003000 	andmi	r3, r0, r0
 8004760:	026259ff 	rsbeq	r5, r2, #4177920	; 0x3fc000

08004764 <SysTick_Handler>:
 8004764:	b508      	push	{r3, lr}
 8004766:	f7fd fb0f 	bl	8001d88 <onTick>
 800476a:	4a02      	ldr	r2, [pc, #8]	; (8004774 <SysTick_Handler+0x10>)
 800476c:	6813      	ldr	r3, [r2, #0]
 800476e:	3301      	adds	r3, #1
 8004770:	6013      	str	r3, [r2, #0]
 8004772:	bd08      	pop	{r3, pc}
 8004774:	20000d78 	andcs	r0, r0, r8, ror sp

08004778 <sleep>:
 8004778:	f64a 21aa 	movw	r1, #43690	; 0xaaaa
 800477c:	4a06      	ldr	r2, [pc, #24]	; (8004798 <sleep+0x20>)
 800477e:	4b07      	ldr	r3, [pc, #28]	; (800479c <sleep+0x24>)
 8004780:	b530      	push	{r4, r5, lr}
 8004782:	6011      	str	r1, [r2, #0]
 8004784:	681d      	ldr	r5, [r3, #0]
 8004786:	681c      	ldr	r4, [r3, #0]
 8004788:	1b64      	subs	r4, r4, r5
 800478a:	4284      	cmp	r4, r0
 800478c:	d300      	bcc.n	8004790 <sleep+0x18>
 800478e:	bd30      	pop	{r4, r5, pc}
 8004790:	bf30      	wfi
 8004792:	6011      	str	r1, [r2, #0]
 8004794:	e7f7      	b.n	8004786 <sleep+0xe>
 8004796:	bf00      	nop
 8004798:	40003000 	andmi	r3, r0, r0
 800479c:	20000d78 	andcs	r0, r0, r8, ror sp

080047a0 <flash_wait_while_busy>:
 80047a0:	4a02      	ldr	r2, [pc, #8]	; (80047ac <flash_wait_while_busy+0xc>)
 80047a2:	68d3      	ldr	r3, [r2, #12]
 80047a4:	07db      	lsls	r3, r3, #31
 80047a6:	d4fc      	bmi.n	80047a2 <flash_wait_while_busy+0x2>
 80047a8:	4770      	bx	lr
 80047aa:	bf00      	nop
 80047ac:	40022000 	andmi	r2, r2, r0

080047b0 <initFlash>:
 80047b0:	4770      	bx	lr
	...

080047b4 <verifyFlash>:
 80047b4:	4b0e      	ldr	r3, [pc, #56]	; (80047f0 <verifyFlash+0x3c>)
 80047b6:	b510      	push	{r4, lr}
 80047b8:	695a      	ldr	r2, [r3, #20]
 80047ba:	480e      	ldr	r0, [pc, #56]	; (80047f4 <verifyFlash+0x40>)
 80047bc:	f042 0240 	orr.w	r2, r2, #64	; 0x40
 80047c0:	615a      	str	r2, [r3, #20]
 80047c2:	4a0d      	ldr	r2, [pc, #52]	; (80047f8 <verifyFlash+0x44>)
 80047c4:	6891      	ldr	r1, [r2, #8]
 80047c6:	f041 0101 	orr.w	r1, r1, #1
 80047ca:	6091      	str	r1, [r2, #8]
 80047cc:	f04f 6100 	mov.w	r1, #134217728	; 0x8000000
 80047d0:	f851 4b04 	ldr.w	r4, [r1], #4
 80047d4:	4281      	cmp	r1, r0
 80047d6:	6014      	str	r4, [r2, #0]
 80047d8:	d1fa      	bne.n	80047d0 <verifyFlash+0x1c>
 80047da:	6810      	ldr	r0, [r2, #0]
 80047dc:	695a      	ldr	r2, [r3, #20]
 80047de:	f022 0240 	bic.w	r2, r2, #64	; 0x40
 80047e2:	615a      	str	r2, [r3, #20]
 80047e4:	680b      	ldr	r3, [r1, #0]
 80047e6:	1a1b      	subs	r3, r3, r0
 80047e8:	4258      	negs	r0, r3
 80047ea:	4158      	adcs	r0, r3
 80047ec:	bd10      	pop	{r4, pc}
 80047ee:	bf00      	nop
 80047f0:	40021000 	andmi	r1, r2, r0
 80047f4:	08007ffc 	stmdaeq	r0, {r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, sp, lr}
 80047f8:	40023000 	andmi	r3, r2, r0

080047fc <readFlash>:
 80047fc:	b538      	push	{r3, r4, r5, lr}
 80047fe:	4604      	mov	r4, r0
 8004800:	4615      	mov	r5, r2
 8004802:	68a3      	ldr	r3, [r4, #8]
 8004804:	4608      	mov	r0, r1
 8004806:	7921      	ldrb	r1, [r4, #4]
 8004808:	f103 6300 	add.w	r3, r3, #134217728	; 0x8000000
 800480c:	eb03 2181 	add.w	r1, r3, r1, lsl #10
 8004810:	f001 fc02 	bl	8006018 <memcpy>
 8004814:	68a2      	ldr	r2, [r4, #8]
 8004816:	442a      	add	r2, r5
 8004818:	60a2      	str	r2, [r4, #8]
 800481a:	bd38      	pop	{r3, r4, r5, pc}

0800481c <eraseFlash>:
 800481c:	b508      	push	{r3, lr}
 800481e:	4910      	ldr	r1, [pc, #64]	; (8004860 <eraseFlash+0x44>)
 8004820:	4b10      	ldr	r3, [pc, #64]	; (8004864 <eraseFlash+0x48>)
 8004822:	604b      	str	r3, [r1, #4]
 8004824:	f103 3388 	add.w	r3, r3, #2290649224	; 0x88888888
 8004828:	604b      	str	r3, [r1, #4]
 800482a:	7900      	ldrb	r0, [r0, #4]
 800482c:	f7ff ffb8 	bl	80047a0 <flash_wait_while_busy>
 8004830:	690b      	ldr	r3, [r1, #16]
 8004832:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
 8004836:	f043 0302 	orr.w	r3, r3, #2
 800483a:	0280      	lsls	r0, r0, #10
 800483c:	610b      	str	r3, [r1, #16]
 800483e:	6148      	str	r0, [r1, #20]
 8004840:	690b      	ldr	r3, [r1, #16]
 8004842:	f043 0340 	orr.w	r3, r3, #64	; 0x40
 8004846:	610b      	str	r3, [r1, #16]
 8004848:	f7ff ffaa 	bl	80047a0 <flash_wait_while_busy>
 800484c:	690b      	ldr	r3, [r1, #16]
 800484e:	f023 0302 	bic.w	r3, r3, #2
 8004852:	610b      	str	r3, [r1, #16]
 8004854:	690b      	ldr	r3, [r1, #16]
 8004856:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 800485a:	610b      	str	r3, [r1, #16]
 800485c:	bd08      	pop	{r3, pc}
 800485e:	bf00      	nop
 8004860:	40022000 	andmi	r2, r2, r0
 8004864:	45670123 	strbmi	r0, [r7, #-291]!	; 0xfffffedd

08004868 <writeFlash>:
 8004868:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
 800486c:	4617      	mov	r7, r2
 800486e:	2500      	movs	r5, #0
 8004870:	6883      	ldr	r3, [r0, #8]
 8004872:	f890 8004 	ldrb.w	r8, [r0, #4]
 8004876:	f103 6300 	add.w	r3, r3, #134217728	; 0x8000000
 800487a:	4c12      	ldr	r4, [pc, #72]	; (80048c4 <writeFlash+0x5c>)
 800487c:	eb03 2888 	add.w	r8, r3, r8, lsl #10
 8004880:	4b11      	ldr	r3, [pc, #68]	; (80048c8 <writeFlash+0x60>)
 8004882:	6063      	str	r3, [r4, #4]
 8004884:	f103 3388 	add.w	r3, r3, #2290649224	; 0x88888888
 8004888:	6063      	str	r3, [r4, #4]
 800488a:	42bd      	cmp	r5, r7
 800488c:	d308      	bcc.n	80048a0 <writeFlash+0x38>
 800488e:	6921      	ldr	r1, [r4, #16]
 8004890:	f041 0180 	orr.w	r1, r1, #128	; 0x80
 8004894:	6121      	str	r1, [r4, #16]
 8004896:	6883      	ldr	r3, [r0, #8]
 8004898:	443b      	add	r3, r7
 800489a:	6083      	str	r3, [r0, #8]
 800489c:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 80048a0:	f831 9005 	ldrh.w	r9, [r1, r5]
 80048a4:	f7ff ff7c 	bl	80047a0 <flash_wait_while_busy>
 80048a8:	6926      	ldr	r6, [r4, #16]
 80048aa:	f046 0601 	orr.w	r6, r6, #1
 80048ae:	6126      	str	r6, [r4, #16]
 80048b0:	f825 9008 	strh.w	r9, [r5, r8]
 80048b4:	f7ff ff74 	bl	80047a0 <flash_wait_while_busy>
 80048b8:	6926      	ldr	r6, [r4, #16]
 80048ba:	3502      	adds	r5, #2
 80048bc:	f026 0601 	bic.w	r6, r6, #1
 80048c0:	6126      	str	r6, [r4, #16]
 80048c2:	e7e2      	b.n	800488a <writeFlash+0x22>
 80048c4:	40022000 	andmi	r2, r2, r0
 80048c8:	45670123 	strbmi	r0, [r7, #-291]!	; 0xfffffedd

080048cc <initPowerController>:
 80048cc:	4b13      	ldr	r3, [pc, #76]	; (800491c <initPowerController+0x50>)
 80048ce:	681a      	ldr	r2, [r3, #0]
 80048d0:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
 80048d4:	f442 1280 	orr.w	r2, r2, #1048576	; 0x100000
 80048d8:	601a      	str	r2, [r3, #0]
 80048da:	685a      	ldr	r2, [r3, #4]
 80048dc:	f422 6280 	bic.w	r2, r2, #1024	; 0x400
 80048e0:	605a      	str	r2, [r3, #4]
 80048e2:	689a      	ldr	r2, [r3, #8]
 80048e4:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
 80048e8:	609a      	str	r2, [r3, #8]
 80048ea:	68da      	ldr	r2, [r3, #12]
 80048ec:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
 80048f0:	60da      	str	r2, [r3, #12]
 80048f2:	f04f 4390 	mov.w	r3, #1207959552	; 0x48000000
 80048f6:	681a      	ldr	r2, [r3, #0]
 80048f8:	f442 7240 	orr.w	r2, r2, #768	; 0x300
 80048fc:	601a      	str	r2, [r3, #0]
 80048fe:	68da      	ldr	r2, [r3, #12]
 8004900:	f422 7240 	bic.w	r2, r2, #768	; 0x300
 8004904:	60da      	str	r2, [r3, #12]
 8004906:	681a      	ldr	r2, [r3, #0]
 8004908:	f422 6240 	bic.w	r2, r2, #3072	; 0xc00
 800490c:	601a      	str	r2, [r3, #0]
 800490e:	68da      	ldr	r2, [r3, #12]
 8004910:	f422 6240 	bic.w	r2, r2, #3072	; 0xc00
 8004914:	f442 6280 	orr.w	r2, r2, #1024	; 0x400
 8004918:	60da      	str	r2, [r3, #12]
 800491a:	4770      	bx	lr
 800491c:	48000400 	stmdami	r0, {sl}

08004920 <setPower>:
 8004920:	4b04      	ldr	r3, [pc, #16]	; (8004934 <setPower+0x14>)
 8004922:	695a      	ldr	r2, [r3, #20]
 8004924:	b118      	cbz	r0, 800492e <setPower+0xe>
 8004926:	f442 6280 	orr.w	r2, r2, #1024	; 0x400
 800492a:	615a      	str	r2, [r3, #20]
 800492c:	4770      	bx	lr
 800492e:	f422 6280 	bic.w	r2, r2, #1024	; 0x400
 8004932:	e7fa      	b.n	800492a <setPower+0xa>
 8004934:	48000400 	stmdami	r0, {sl}

08004938 <isBatteryCharging>:
 8004938:	f04f 4390 	mov.w	r3, #1207959552	; 0x48000000
 800493c:	691b      	ldr	r3, [r3, #16]
 800493e:	b510      	push	{r4, lr}
 8004940:	069b      	lsls	r3, r3, #26
 8004942:	d50b      	bpl.n	800495c <isBatteryCharging+0x24>
 8004944:	f7ff fd54 	bl	80043f0 <getDeviceBatteryVoltage>
 8004948:	f04f 517c 	mov.w	r1, #1056964608	; 0x3f000000
 800494c:	f04f 0401 	mov.w	r4, #1
 8004950:	f7ff fdb6 	bl	80044c0 <__aeabi_fcmplt>
 8004954:	b900      	cbnz	r0, 8004958 <isBatteryCharging+0x20>
 8004956:	4604      	mov	r4, r0
 8004958:	b2e0      	uxtb	r0, r4
 800495a:	bd10      	pop	{r4, pc}
 800495c:	2001      	movs	r0, #1
 800495e:	e7fc      	b.n	800495a <isBatteryCharging+0x22>

08004960 <setDeviceTime>:
 8004960:	b537      	push	{r0, r1, r2, r4, r5, lr}
 8004962:	4669      	mov	r1, sp
 8004964:	f7ff f980 	bl	8003c68 <getDateTimeFromTime>
 8004968:	2364      	movs	r3, #100	; 0x64
 800496a:	f8bd 1000 	ldrh.w	r1, [sp]
 800496e:	f89d 0002 	ldrb.w	r0, [sp, #2]
 8004972:	fbb1 f2f3 	udiv	r2, r1, r3
 8004976:	fb03 1212 	mls	r2, r3, r2, r1
 800497a:	230a      	movs	r3, #10
 800497c:	fbb0 f4f3 	udiv	r4, r0, r3
 8004980:	fb03 0014 	mls	r0, r3, r4, r0
 8004984:	f89d 1003 	ldrb.w	r1, [sp, #3]
 8004988:	ea40 1004 	orr.w	r0, r0, r4, lsl #4
 800498c:	fbb1 f4f3 	udiv	r4, r1, r3
 8004990:	fb03 1114 	mls	r1, r3, r4, r1
 8004994:	b292      	uxth	r2, r2
 8004996:	ea41 1104 	orr.w	r1, r1, r4, lsl #4
 800499a:	ea41 2100 	orr.w	r1, r1, r0, lsl #8
 800499e:	fbb2 f0f3 	udiv	r0, r2, r3
 80049a2:	fb03 2210 	mls	r2, r3, r0, r2
 80049a6:	ea42 1200 	orr.w	r2, r2, r0, lsl #4
 80049aa:	f89d 0004 	ldrb.w	r0, [sp, #4]
 80049ae:	ea41 4102 	orr.w	r1, r1, r2, lsl #16
 80049b2:	fbb0 f5f3 	udiv	r5, r0, r3
 80049b6:	fb03 0015 	mls	r0, r3, r5, r0
 80049ba:	f89d 2006 	ldrb.w	r2, [sp, #6]
 80049be:	ea40 1005 	orr.w	r0, r0, r5, lsl #4
 80049c2:	fbb2 f5f3 	udiv	r5, r2, r3
 80049c6:	fb03 2215 	mls	r2, r3, r5, r2
 80049ca:	f89d 4005 	ldrb.w	r4, [sp, #5]
 80049ce:	ea42 1205 	orr.w	r2, r2, r5, lsl #4
 80049d2:	ea42 4200 	orr.w	r2, r2, r0, lsl #16
 80049d6:	fbb4 f0f3 	udiv	r0, r4, r3
 80049da:	fb03 4310 	mls	r3, r3, r0, r4
 80049de:	ea43 1300 	orr.w	r3, r3, r0, lsl #4
 80049e2:	20ca      	movs	r0, #202	; 0xca
 80049e4:	ea42 2303 	orr.w	r3, r2, r3, lsl #8
 80049e8:	4a0f      	ldr	r2, [pc, #60]	; (8004a28 <setDeviceTime+0xc8>)
 80049ea:	6250      	str	r0, [r2, #36]	; 0x24
 80049ec:	2053      	movs	r0, #83	; 0x53
 80049ee:	6250      	str	r0, [r2, #36]	; 0x24
 80049f0:	68d0      	ldr	r0, [r2, #12]
 80049f2:	f040 0080 	orr.w	r0, r0, #128	; 0x80
 80049f6:	60d0      	str	r0, [r2, #12]
 80049f8:	68d0      	ldr	r0, [r2, #12]
 80049fa:	0640      	lsls	r0, r0, #25
 80049fc:	d5fc      	bpl.n	80049f8 <setDeviceTime+0x98>
 80049fe:	6013      	str	r3, [r2, #0]
 8004a00:	6051      	str	r1, [r2, #4]
 8004a02:	68d3      	ldr	r3, [r2, #12]
 8004a04:	f023 0380 	bic.w	r3, r3, #128	; 0x80
 8004a08:	60d3      	str	r3, [r2, #12]
 8004a0a:	68d3      	ldr	r3, [r2, #12]
 8004a0c:	f023 0320 	bic.w	r3, r3, #32
 8004a10:	60d3      	str	r3, [r2, #12]
 8004a12:	4b05      	ldr	r3, [pc, #20]	; (8004a28 <setDeviceTime+0xc8>)
 8004a14:	68da      	ldr	r2, [r3, #12]
 8004a16:	0692      	lsls	r2, r2, #26
 8004a18:	d5fc      	bpl.n	8004a14 <setDeviceTime+0xb4>
 8004a1a:	22fe      	movs	r2, #254	; 0xfe
 8004a1c:	625a      	str	r2, [r3, #36]	; 0x24
 8004a1e:	2264      	movs	r2, #100	; 0x64
 8004a20:	625a      	str	r2, [r3, #36]	; 0x24
 8004a22:	b003      	add	sp, #12
 8004a24:	bd30      	pop	{r4, r5, pc}
 8004a26:	bf00      	nop
 8004a28:	40002800 	andmi	r2, r0, r0, lsl #16

08004a2c <initRTCController>:
 8004a2c:	b510      	push	{r4, lr}
 8004a2e:	4c16      	ldr	r4, [pc, #88]	; (8004a88 <initRTCController+0x5c>)
 8004a30:	f7fd fa18 	bl	8001e64 <syncTimerThread>
 8004a34:	69e3      	ldr	r3, [r4, #28]
 8004a36:	4a15      	ldr	r2, [pc, #84]	; (8004a8c <initRTCController+0x60>)
 8004a38:	f043 5380 	orr.w	r3, r3, #268435456	; 0x10000000
 8004a3c:	61e3      	str	r3, [r4, #28]
 8004a3e:	6813      	ldr	r3, [r2, #0]
 8004a40:	f443 7380 	orr.w	r3, r3, #256	; 0x100
 8004a44:	6013      	str	r3, [r2, #0]
 8004a46:	6a23      	ldr	r3, [r4, #32]
 8004a48:	041a      	lsls	r2, r3, #16
 8004a4a:	d41c      	bmi.n	8004a86 <initRTCController+0x5a>
 8004a4c:	6a23      	ldr	r3, [r4, #32]
 8004a4e:	f043 0301 	orr.w	r3, r3, #1
 8004a52:	6223      	str	r3, [r4, #32]
 8004a54:	6a23      	ldr	r3, [r4, #32]
 8004a56:	079b      	lsls	r3, r3, #30
 8004a58:	d511      	bpl.n	8004a7e <initRTCController+0x52>
 8004a5a:	6a23      	ldr	r3, [r4, #32]
 8004a5c:	2001      	movs	r0, #1
 8004a5e:	f423 7340 	bic.w	r3, r3, #768	; 0x300
 8004a62:	f443 7380 	orr.w	r3, r3, #256	; 0x100
 8004a66:	6223      	str	r3, [r4, #32]
 8004a68:	6a23      	ldr	r3, [r4, #32]
 8004a6a:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8004a6e:	6223      	str	r3, [r4, #32]
 8004a70:	f7ff fe82 	bl	8004778 <sleep>
 8004a74:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8004a78:	4805      	ldr	r0, [pc, #20]	; (8004a90 <initRTCController+0x64>)
 8004a7a:	f7ff bf71 	b.w	8004960 <setDeviceTime>
 8004a7e:	2001      	movs	r0, #1
 8004a80:	f7ff fe7a 	bl	8004778 <sleep>
 8004a84:	e7e6      	b.n	8004a54 <initRTCController+0x28>
 8004a86:	bd10      	pop	{r4, pc}
 8004a88:	40021000 	andmi	r1, r2, r0
 8004a8c:	40007000 	andmi	r7, r0, r0
 8004a90:	386d4380 	stmdacc	sp!, {r7, r8, r9, lr}^

08004a94 <getDeviceTime>:
 8004a94:	b513      	push	{r0, r1, r4, lr}
 8004a96:	4c23      	ldr	r4, [pc, #140]	; (8004b24 <getDeviceTime+0x90>)
 8004a98:	6aa3      	ldr	r3, [r4, #40]	; 0x28
 8004a9a:	2bfb      	cmp	r3, #251	; 0xfb
 8004a9c:	d83e      	bhi.n	8004b1c <getDeviceTime+0x88>
 8004a9e:	210a      	movs	r1, #10
 8004aa0:	6823      	ldr	r3, [r4, #0]
 8004aa2:	6862      	ldr	r2, [r4, #4]
 8004aa4:	f3c2 4407 	ubfx	r4, r2, #16, #8
 8004aa8:	0920      	lsrs	r0, r4, #4
 8004aaa:	f004 040f 	and.w	r4, r4, #15
 8004aae:	fb01 4000 	mla	r0, r1, r0, r4
 8004ab2:	f500 60fa 	add.w	r0, r0, #2000	; 0x7d0
 8004ab6:	f8ad 0000 	strh.w	r0, [sp]
 8004aba:	f3c2 2007 	ubfx	r0, r2, #8, #8
 8004abe:	f3c0 1400 	ubfx	r4, r0, #4, #1
 8004ac2:	f000 000f 	and.w	r0, r0, #15
 8004ac6:	fb01 0004 	mla	r0, r1, r4, r0
 8004aca:	b2d2      	uxtb	r2, r2
 8004acc:	f88d 0002 	strb.w	r0, [sp, #2]
 8004ad0:	0910      	lsrs	r0, r2, #4
 8004ad2:	f002 020f 	and.w	r2, r2, #15
 8004ad6:	fb01 2200 	mla	r2, r1, r0, r2
 8004ada:	f88d 2003 	strb.w	r2, [sp, #3]
 8004ade:	f3c3 4207 	ubfx	r2, r3, #16, #8
 8004ae2:	0910      	lsrs	r0, r2, #4
 8004ae4:	f002 020f 	and.w	r2, r2, #15
 8004ae8:	fb01 2200 	mla	r2, r1, r0, r2
 8004aec:	f88d 2004 	strb.w	r2, [sp, #4]
 8004af0:	f3c3 2207 	ubfx	r2, r3, #8, #8
 8004af4:	0910      	lsrs	r0, r2, #4
 8004af6:	f002 020f 	and.w	r2, r2, #15
 8004afa:	fb01 2200 	mla	r2, r1, r0, r2
 8004afe:	b2db      	uxtb	r3, r3
 8004b00:	f88d 2005 	strb.w	r2, [sp, #5]
 8004b04:	091a      	lsrs	r2, r3, #4
 8004b06:	f003 030f 	and.w	r3, r3, #15
 8004b0a:	fb01 3302 	mla	r3, r1, r2, r3
 8004b0e:	4668      	mov	r0, sp
 8004b10:	f88d 3006 	strb.w	r3, [sp, #6]
 8004b14:	f7ff f864 	bl	8003be0 <getTimeFromDateTime>
 8004b18:	b002      	add	sp, #8
 8004b1a:	bd10      	pop	{r4, pc}
 8004b1c:	2001      	movs	r0, #1
 8004b1e:	f7ff fe2b 	bl	8004778 <sleep>
 8004b22:	e7b9      	b.n	8004a98 <getDeviceTime+0x4>
 8004b24:	40002800 	andmi	r2, r0, r0, lsl #16

08004b28 <getDeviceId>:
 8004b28:	4b04      	ldr	r3, [pc, #16]	; (8004b3c <getDeviceId+0x14>)
 8004b2a:	4a05      	ldr	r2, [pc, #20]	; (8004b40 <getDeviceId+0x18>)
 8004b2c:	4905      	ldr	r1, [pc, #20]	; (8004b44 <getDeviceId+0x1c>)
 8004b2e:	681b      	ldr	r3, [r3, #0]
 8004b30:	6812      	ldr	r2, [r2, #0]
 8004b32:	6808      	ldr	r0, [r1, #0]
 8004b34:	4053      	eors	r3, r2
 8004b36:	4058      	eors	r0, r3
 8004b38:	4770      	bx	lr
 8004b3a:	bf00      	nop
 8004b3c:	1ffff7ac 	svcne	0x00fff7ac
 8004b40:	1ffff7b0 	svcne	0x00fff7b0
 8004b44:	1ffff7b4 	svcne	0x00fff7b4

08004b48 <updateTubeHV>:
 8004b48:	b538      	push	{r3, r4, r5, lr}
 8004b4a:	f000 fb1d 	bl	8005188 <getTubeHVFrequency>
 8004b4e:	4601      	mov	r1, r0
 8004b50:	480c      	ldr	r0, [pc, #48]	; (8004b84 <updateTubeHV+0x3c>)
 8004b52:	f7ff fca1 	bl	8004498 <__aeabi_fdiv>
 8004b56:	f7ff fcaa 	bl	80044ae <__aeabi_f2uiz>
 8004b5a:	4b0b      	ldr	r3, [pc, #44]	; (8004b88 <updateTubeHV+0x40>)
 8004b5c:	1e42      	subs	r2, r0, #1
 8004b5e:	62da      	str	r2, [r3, #44]	; 0x2c
 8004b60:	4b0a      	ldr	r3, [pc, #40]	; (8004b8c <updateTubeHV+0x44>)
 8004b62:	4604      	mov	r4, r0
 8004b64:	7818      	ldrb	r0, [r3, #0]
 8004b66:	b150      	cbz	r0, 8004b7e <updateTubeHV+0x36>
 8004b68:	f000 fb30 	bl	80051cc <getTubeHVDutyCycle>
 8004b6c:	4605      	mov	r5, r0
 8004b6e:	4620      	mov	r0, r4
 8004b70:	f7ff fc94 	bl	800449c <__aeabi_ui2f>
 8004b74:	4629      	mov	r1, r5
 8004b76:	f7ff fc8d 	bl	8004494 <__aeabi_fmul>
 8004b7a:	f7ff fc98 	bl	80044ae <__aeabi_f2uiz>
 8004b7e:	4b02      	ldr	r3, [pc, #8]	; (8004b88 <updateTubeHV+0x40>)
 8004b80:	6358      	str	r0, [r3, #52]	; 0x34
 8004b82:	bd38      	pop	{r3, r4, r5, pc}
 8004b84:	4af42400 	bmi	7d0db8c <_Min_Stack_Size+0x7d0d78c>
 8004b88:	40000400 	andmi	r0, r0, r0, lsl #8
 8004b8c:	20000b4c 	andcs	r0, r0, ip, asr #22

08004b90 <initTubeController>:
 8004b90:	b538      	push	{r3, r4, r5, lr}
 8004b92:	4b3d      	ldr	r3, [pc, #244]	; (8004c88 <initTubeController+0xf8>)
 8004b94:	4c3d      	ldr	r4, [pc, #244]	; (8004c8c <initTubeController+0xfc>)
 8004b96:	681a      	ldr	r2, [r3, #0]
 8004b98:	4d3d      	ldr	r5, [pc, #244]	; (8004c90 <initTubeController+0x100>)
 8004b9a:	f422 7240 	bic.w	r2, r2, #768	; 0x300
 8004b9e:	f442 7200 	orr.w	r2, r2, #512	; 0x200
 8004ba2:	601a      	str	r2, [r3, #0]
 8004ba4:	685a      	ldr	r2, [r3, #4]
 8004ba6:	f022 0210 	bic.w	r2, r2, #16
 8004baa:	605a      	str	r2, [r3, #4]
 8004bac:	689a      	ldr	r2, [r3, #8]
 8004bae:	f442 7240 	orr.w	r2, r2, #768	; 0x300
 8004bb2:	609a      	str	r2, [r3, #8]
 8004bb4:	68da      	ldr	r2, [r3, #12]
 8004bb6:	f422 7240 	bic.w	r2, r2, #768	; 0x300
 8004bba:	60da      	str	r2, [r3, #12]
 8004bbc:	6a1a      	ldr	r2, [r3, #32]
 8004bbe:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
 8004bc2:	f442 3280 	orr.w	r2, r2, #65536	; 0x10000
 8004bc6:	621a      	str	r2, [r3, #32]
 8004bc8:	f04f 4290 	mov.w	r2, #1207959552	; 0x48000000
 8004bcc:	6813      	ldr	r3, [r2, #0]
 8004bce:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
 8004bd2:	6013      	str	r3, [r2, #0]
 8004bd4:	68d3      	ldr	r3, [r2, #12]
 8004bd6:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
 8004bda:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
 8004bde:	60d3      	str	r3, [r2, #12]
 8004be0:	69e3      	ldr	r3, [r4, #28]
 8004be2:	f043 0302 	orr.w	r3, r3, #2
 8004be6:	61e3      	str	r3, [r4, #28]
 8004be8:	69ab      	ldr	r3, [r5, #24]
 8004bea:	f023 0370 	bic.w	r3, r3, #112	; 0x70
 8004bee:	f043 0360 	orr.w	r3, r3, #96	; 0x60
 8004bf2:	61ab      	str	r3, [r5, #24]
 8004bf4:	6a2b      	ldr	r3, [r5, #32]
 8004bf6:	f043 0301 	orr.w	r3, r3, #1
 8004bfa:	622b      	str	r3, [r5, #32]
 8004bfc:	f7ff ffa4 	bl	8004b48 <updateTubeHV>
 8004c00:	f64f 70ff 	movw	r0, #65535	; 0xffff
 8004c04:	682b      	ldr	r3, [r5, #0]
 8004c06:	f043 0301 	orr.w	r3, r3, #1
 8004c0a:	602b      	str	r3, [r5, #0]
 8004c0c:	69a3      	ldr	r3, [r4, #24]
 8004c0e:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
 8004c12:	61a3      	str	r3, [r4, #24]
 8004c14:	4b1f      	ldr	r3, [pc, #124]	; (8004c94 <initTubeController+0x104>)
 8004c16:	685a      	ldr	r2, [r3, #4]
 8004c18:	f042 0220 	orr.w	r2, r2, #32
 8004c1c:	605a      	str	r2, [r3, #4]
 8004c1e:	62d8      	str	r0, [r3, #44]	; 0x2c
 8004c20:	69a2      	ldr	r2, [r4, #24]
 8004c22:	f442 6200 	orr.w	r2, r2, #2048	; 0x800
 8004c26:	61a2      	str	r2, [r4, #24]
 8004c28:	4a1b      	ldr	r2, [pc, #108]	; (8004c98 <initTubeController+0x108>)
 8004c2a:	6891      	ldr	r1, [r2, #8]
 8004c2c:	f041 0107 	orr.w	r1, r1, #7
 8004c30:	6091      	str	r1, [r2, #8]
 8004c32:	62d0      	str	r0, [r2, #44]	; 0x2c
 8004c34:	6959      	ldr	r1, [r3, #20]
 8004c36:	f041 0101 	orr.w	r1, r1, #1
 8004c3a:	6159      	str	r1, [r3, #20]
 8004c3c:	2100      	movs	r1, #0
 8004c3e:	6299      	str	r1, [r3, #40]	; 0x28
 8004c40:	6811      	ldr	r1, [r2, #0]
 8004c42:	f041 0101 	orr.w	r1, r1, #1
 8004c46:	6011      	str	r1, [r2, #0]
 8004c48:	681a      	ldr	r2, [r3, #0]
 8004c4a:	f042 0201 	orr.w	r2, r2, #1
 8004c4e:	601a      	str	r2, [r3, #0]
 8004c50:	4a12      	ldr	r2, [pc, #72]	; (8004c9c <initTubeController+0x10c>)
 8004c52:	68d3      	ldr	r3, [r2, #12]
 8004c54:	f423 6370 	bic.w	r3, r3, #3840	; 0xf00
 8004c58:	60d3      	str	r3, [r2, #12]
 8004c5a:	4b11      	ldr	r3, [pc, #68]	; (8004ca0 <initTubeController+0x110>)
 8004c5c:	689a      	ldr	r2, [r3, #8]
 8004c5e:	f022 0240 	bic.w	r2, r2, #64	; 0x40
 8004c62:	609a      	str	r2, [r3, #8]
 8004c64:	68da      	ldr	r2, [r3, #12]
 8004c66:	f042 0240 	orr.w	r2, r2, #64	; 0x40
 8004c6a:	60da      	str	r2, [r3, #12]
 8004c6c:	4a0d      	ldr	r2, [pc, #52]	; (8004ca4 <initTubeController+0x114>)
 8004c6e:	f8d2 1304 	ldr.w	r1, [r2, #772]	; 0x304
 8004c72:	f021 417f 	bic.w	r1, r1, #4278190080	; 0xff000000
 8004c76:	f8c2 1304 	str.w	r1, [r2, #772]	; 0x304
 8004c7a:	2180      	movs	r1, #128	; 0x80
 8004c7c:	6011      	str	r1, [r2, #0]
 8004c7e:	681a      	ldr	r2, [r3, #0]
 8004c80:	f042 0240 	orr.w	r2, r2, #64	; 0x40
 8004c84:	601a      	str	r2, [r3, #0]
 8004c86:	bd38      	pop	{r3, r4, r5, pc}
 8004c88:	48000400 	stmdami	r0, {sl}
 8004c8c:	40021000 	andmi	r1, r2, r0
 8004c90:	40000400 	andmi	r0, r0, r0, lsl #8
 8004c94:	40014000 	andmi	r4, r1, r0
 8004c98:	40012c00 	andmi	r2, r1, r0, lsl #24
 8004c9c:	40010000 	andmi	r0, r1, r0
 8004ca0:	40010400 	andmi	r0, r1, r0, lsl #8
 8004ca4:	e000e100 	and	lr, r0, r0, lsl #2

08004ca8 <setTubeHV>:
 8004ca8:	4b01      	ldr	r3, [pc, #4]	; (8004cb0 <setTubeHV+0x8>)
 8004caa:	7018      	strb	r0, [r3, #0]
 8004cac:	f7ff bf4c 	b.w	8004b48 <updateTubeHV>
 8004cb0:	20000b4c 	andcs	r0, r0, ip, asr #22

08004cb4 <EXTI4_15_IRQHandler>:
 8004cb4:	4a0f      	ldr	r2, [pc, #60]	; (8004cf4 <EXTI4_15_IRQHandler+0x40>)
 8004cb6:	4910      	ldr	r1, [pc, #64]	; (8004cf8 <EXTI4_15_IRQHandler+0x44>)
 8004cb8:	6953      	ldr	r3, [r2, #20]
 8004cba:	f043 0340 	orr.w	r3, r3, #64	; 0x40
 8004cbe:	6153      	str	r3, [r2, #20]
 8004cc0:	4b0e      	ldr	r3, [pc, #56]	; (8004cfc <EXTI4_15_IRQHandler+0x48>)
 8004cc2:	6a4a      	ldr	r2, [r1, #36]	; 0x24
 8004cc4:	6a5b      	ldr	r3, [r3, #36]	; 0x24
 8004cc6:	6a49      	ldr	r1, [r1, #36]	; 0x24
 8004cc8:	428a      	cmp	r2, r1
 8004cca:	d10d      	bne.n	8004ce8 <EXTI4_15_IRQHandler+0x34>
 8004ccc:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
 8004cd0:	490b      	ldr	r1, [pc, #44]	; (8004d00 <EXTI4_15_IRQHandler+0x4c>)
 8004cd2:	684a      	ldr	r2, [r1, #4]
 8004cd4:	3202      	adds	r2, #2
 8004cd6:	eb01 0282 	add.w	r2, r1, r2, lsl #2
 8004cda:	6053      	str	r3, [r2, #4]
 8004cdc:	684b      	ldr	r3, [r1, #4]
 8004cde:	3301      	adds	r3, #1
 8004ce0:	f003 033f 	and.w	r3, r3, #63	; 0x3f
 8004ce4:	604b      	str	r3, [r1, #4]
 8004ce6:	4770      	bx	lr
 8004ce8:	0418      	lsls	r0, r3, #16
 8004cea:	d4ef      	bmi.n	8004ccc <EXTI4_15_IRQHandler+0x18>
 8004cec:	ea43 4301 	orr.w	r3, r3, r1, lsl #16
 8004cf0:	e7ee      	b.n	8004cd0 <EXTI4_15_IRQHandler+0x1c>
 8004cf2:	bf00      	nop
 8004cf4:	40010400 	andmi	r0, r1, r0, lsl #8
 8004cf8:	40012c00 	andmi	r2, r1, r0, lsl #24
 8004cfc:	40014000 	andmi	r4, r1, r0
 8004d00:	20000b4c 	andcs	r0, r0, ip, asr #22

08004d04 <getTubePulse>:
 8004d04:	4b09      	ldr	r3, [pc, #36]	; (8004d2c <getTubePulse+0x28>)
 8004d06:	6859      	ldr	r1, [r3, #4]
 8004d08:	689a      	ldr	r2, [r3, #8]
 8004d0a:	4291      	cmp	r1, r2
 8004d0c:	d00c      	beq.n	8004d28 <getTubePulse+0x24>
 8004d0e:	689a      	ldr	r2, [r3, #8]
 8004d10:	3202      	adds	r2, #2
 8004d12:	eb03 0282 	add.w	r2, r3, r2, lsl #2
 8004d16:	6852      	ldr	r2, [r2, #4]
 8004d18:	6002      	str	r2, [r0, #0]
 8004d1a:	689a      	ldr	r2, [r3, #8]
 8004d1c:	2001      	movs	r0, #1
 8004d1e:	3201      	adds	r2, #1
 8004d20:	f002 023f 	and.w	r2, r2, #63	; 0x3f
 8004d24:	609a      	str	r2, [r3, #8]
 8004d26:	4770      	bx	lr
 8004d28:	2000      	movs	r0, #0
 8004d2a:	4770      	bx	lr
 8004d2c:	20000b4c 	andcs	r0, r0, ip, asr #22

08004d30 <initVibrator>:
 8004d30:	4b0b      	ldr	r3, [pc, #44]	; (8004d60 <initVibrator+0x30>)
 8004d32:	695a      	ldr	r2, [r3, #20]
 8004d34:	f022 0240 	bic.w	r2, r2, #64	; 0x40
 8004d38:	615a      	str	r2, [r3, #20]
 8004d3a:	681a      	ldr	r2, [r3, #0]
 8004d3c:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
 8004d40:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
 8004d44:	601a      	str	r2, [r3, #0]
 8004d46:	685a      	ldr	r2, [r3, #4]
 8004d48:	f022 0240 	bic.w	r2, r2, #64	; 0x40
 8004d4c:	605a      	str	r2, [r3, #4]
 8004d4e:	689a      	ldr	r2, [r3, #8]
 8004d50:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
 8004d54:	609a      	str	r2, [r3, #8]
 8004d56:	68da      	ldr	r2, [r3, #12]
 8004d58:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
 8004d5c:	60da      	str	r2, [r3, #12]
 8004d5e:	4770      	bx	lr
 8004d60:	48000400 	stmdami	r0, {sl}

08004d64 <setVibrator>:
 8004d64:	4b04      	ldr	r3, [pc, #16]	; (8004d78 <setVibrator+0x14>)
 8004d66:	695a      	ldr	r2, [r3, #20]
 8004d68:	b118      	cbz	r0, 8004d72 <setVibrator+0xe>
 8004d6a:	f042 0240 	orr.w	r2, r2, #64	; 0x40
 8004d6e:	615a      	str	r2, [r3, #20]
 8004d70:	4770      	bx	lr
 8004d72:	f022 0240 	bic.w	r2, r2, #64	; 0x40
 8004d76:	e7fa      	b.n	8004d6e <setVibrator+0xa>
 8004d78:	48000400 	stmdami	r0, {sl}

08004d7c <onStatisticsViewEvent>:
 8004d7c:	b111      	cbz	r1, 8004d84 <onStatisticsViewEvent+0x8>
 8004d7e:	2906      	cmp	r1, #6
 8004d80:	d003      	beq.n	8004d8a <onStatisticsViewEvent+0xe>
 8004d82:	4770      	bx	lr
 8004d84:	4802      	ldr	r0, [pc, #8]	; (8004d90 <onStatisticsViewEvent+0x14>)
 8004d86:	f000 ba7f 	b.w	8005288 <setView>
 8004d8a:	f7fc be4d 	b.w	8001a28 <drawStatistics>
 8004d8e:	bf00      	nop
 8004d90:	0800753c 	stmdaeq	r0, {r2, r3, r4, r5, r8, sl, ip, sp, lr}

08004d94 <onTubeMenuGetOption>:
 8004d94:	2302      	movs	r3, #2
 8004d96:	6013      	str	r3, [r2, #0]
 8004d98:	4b01      	ldr	r3, [pc, #4]	; (8004da0 <onTubeMenuGetOption+0xc>)
 8004d9a:	f853 0021 	ldr.w	r0, [r3, r1, lsl #2]
 8004d9e:	4770      	bx	lr
 8004da0:	080077f0 	stmdaeq	r0, {r4, r5, r6, r7, r8, r9, sl, ip, sp, lr}

08004da4 <onTubeHVProfileMenuGetOption>:
 8004da4:	4b07      	ldr	r3, [pc, #28]	; (8004dc4 <onTubeHVProfileMenuGetOption+0x20>)
 8004da6:	791b      	ldrb	r3, [r3, #4]
 8004da8:	f003 0307 	and.w	r3, r3, #7
 8004dac:	1a58      	subs	r0, r3, r1
 8004dae:	4243      	negs	r3, r0
 8004db0:	4143      	adcs	r3, r0
 8004db2:	2903      	cmp	r1, #3
 8004db4:	bf08      	it	eq
 8004db6:	f043 0302 	orreq.w	r3, r3, #2
 8004dba:	6013      	str	r3, [r2, #0]
 8004dbc:	4b02      	ldr	r3, [pc, #8]	; (8004dc8 <onTubeHVProfileMenuGetOption+0x24>)
 8004dbe:	f853 0021 	ldr.w	r0, [r3, r1, lsl #2]
 8004dc2:	4770      	bx	lr
 8004dc4:	20000da0 	andcs	r0, r0, r0, lsr #27
 8004dc8:	080077ac 	stmdaeq	r0, {r2, r3, r5, r7, r8, r9, sl, ip, sp, lr}

08004dcc <onTubeHVCustomProfileMenuGetOption>:
 8004dcc:	2302      	movs	r3, #2
 8004dce:	6013      	str	r3, [r2, #0]
 8004dd0:	4b01      	ldr	r3, [pc, #4]	; (8004dd8 <onTubeHVCustomProfileMenuGetOption+0xc>)
 8004dd2:	f853 0021 	ldr.w	r0, [r3, r1, lsl #2]
 8004dd6:	4770      	bx	lr
 8004dd8:	08007730 	stmdaeq	r0, {r4, r5, r8, r9, sl, ip, sp, lr}

08004ddc <onTubeHVCustomProfileSubMenuBack>:
 8004ddc:	4801      	ldr	r0, [pc, #4]	; (8004de4 <onTubeHVCustomProfileSubMenuBack+0x8>)
 8004dde:	f000 ba53 	b.w	8005288 <setView>
 8004de2:	bf00      	nop
 8004de4:	0800773c 	stmdaeq	r0, {r2, r3, r4, r5, r8, r9, sl, ip, sp, lr}

08004de8 <onTubeHVGeneratorSubMenuBack>:
 8004de8:	4801      	ldr	r0, [pc, #4]	; (8004df0 <onTubeHVGeneratorSubMenuBack+0x8>)
 8004dea:	f000 ba4d 	b.w	8005288 <setView>
 8004dee:	bf00      	nop
 8004df0:	080077c0 	stmdaeq	r0, {r6, r7, r8, r9, sl, ip, sp, lr}

08004df4 <onTubeHVCustomProfileMenuSelect>:
 8004df4:	6843      	ldr	r3, [r0, #4]
 8004df6:	885b      	ldrh	r3, [r3, #2]
 8004df8:	b113      	cbz	r3, 8004e00 <onTubeHVCustomProfileMenuSelect+0xc>
 8004dfa:	2b01      	cmp	r3, #1
 8004dfc:	d003      	beq.n	8004e06 <onTubeHVCustomProfileMenuSelect+0x12>
 8004dfe:	4770      	bx	lr
 8004e00:	4802      	ldr	r0, [pc, #8]	; (8004e0c <onTubeHVCustomProfileMenuSelect+0x18>)
 8004e02:	f000 ba41 	b.w	8005288 <setView>
 8004e06:	4802      	ldr	r0, [pc, #8]	; (8004e10 <onTubeHVCustomProfileMenuSelect+0x1c>)
 8004e08:	e7fb      	b.n	8004e02 <onTubeHVCustomProfileMenuSelect+0xe>
 8004e0a:	bf00      	nop
 8004e0c:	080077c8 	stmdaeq	r0, {r3, r6, r7, r8, r9, sl, ip, sp, lr}
 8004e10:	08007760 	stmdaeq	r0, {r5, r6, r8, r9, sl, ip, sp, lr}

08004e14 <onTubeSubMenuBack>:
 8004e14:	4801      	ldr	r0, [pc, #4]	; (8004e1c <onTubeSubMenuBack+0x8>)
 8004e16:	f000 ba37 	b.w	8005288 <setView>
 8004e1a:	bf00      	nop
 8004e1c:	08007800 	stmdaeq	r0, {fp, ip, sp, lr}

08004e20 <onTubeMenuSelect>:
 8004e20:	6843      	ldr	r3, [r0, #4]
 8004e22:	885a      	ldrh	r2, [r3, #2]
 8004e24:	4b02      	ldr	r3, [pc, #8]	; (8004e30 <onTubeMenuSelect+0x10>)
 8004e26:	f853 0022 	ldr.w	r0, [r3, r2, lsl #2]
 8004e2a:	f000 ba2d 	b.w	8005288 <setView>
 8004e2e:	bf00      	nop
 8004e30:	080077e4 	stmdaeq	r0, {r2, r5, r6, r7, r8, r9, sl, ip, sp, lr}

08004e34 <onTubeHVDutyCycleMenuSelect>:
 8004e34:	4b04      	ldr	r3, [pc, #16]	; (8004e48 <onTubeHVDutyCycleMenuSelect+0x14>)
 8004e36:	6841      	ldr	r1, [r0, #4]
 8004e38:	889a      	ldrh	r2, [r3, #4]
 8004e3a:	8849      	ldrh	r1, [r1, #2]
 8004e3c:	f361 128e 	bfi	r2, r1, #6, #9
 8004e40:	809a      	strh	r2, [r3, #4]
 8004e42:	f7ff be81 	b.w	8004b48 <updateTubeHV>
 8004e46:	bf00      	nop
 8004e48:	20000da0 	andcs	r0, r0, r0, lsr #27

08004e4c <onTubeHVFrequencyMenuSelect>:
 8004e4c:	4b04      	ldr	r3, [pc, #16]	; (8004e60 <onTubeHVFrequencyMenuSelect+0x14>)
 8004e4e:	6841      	ldr	r1, [r0, #4]
 8004e50:	791a      	ldrb	r2, [r3, #4]
 8004e52:	8849      	ldrh	r1, [r1, #2]
 8004e54:	f361 02c5 	bfi	r2, r1, #3, #3
 8004e58:	711a      	strb	r2, [r3, #4]
 8004e5a:	f7ff be75 	b.w	8004b48 <updateTubeHV>
 8004e5e:	bf00      	nop
 8004e60:	20000da0 	andcs	r0, r0, r0, lsr #27

08004e64 <onTubeHVProfileMenuSelect>:
 8004e64:	6843      	ldr	r3, [r0, #4]
 8004e66:	8859      	ldrh	r1, [r3, #2]
 8004e68:	2902      	cmp	r1, #2
 8004e6a:	d806      	bhi.n	8004e7a <onTubeHVProfileMenuSelect+0x16>
 8004e6c:	4b04      	ldr	r3, [pc, #16]	; (8004e80 <onTubeHVProfileMenuSelect+0x1c>)
 8004e6e:	791a      	ldrb	r2, [r3, #4]
 8004e70:	f361 0202 	bfi	r2, r1, #0, #3
 8004e74:	711a      	strb	r2, [r3, #4]
 8004e76:	f7ff be67 	b.w	8004b48 <updateTubeHV>
 8004e7a:	4802      	ldr	r0, [pc, #8]	; (8004e84 <onTubeHVProfileMenuSelect+0x20>)
 8004e7c:	f000 ba04 	b.w	8005288 <setView>
 8004e80:	20000da0 	andcs	r0, r0, r0, lsr #27
 8004e84:	08007744 	stmdaeq	r0, {r2, r6, r8, r9, sl, ip, sp, lr}

08004e88 <onTubeHVFrequencyMenuGetOption>:
 8004e88:	b508      	push	{r3, lr}
 8004e8a:	4b0a      	ldr	r3, [pc, #40]	; (8004eb4 <onTubeHVFrequencyMenuGetOption+0x2c>)
 8004e8c:	791b      	ldrb	r3, [r3, #4]
 8004e8e:	f3c3 03c2 	ubfx	r3, r3, #3, #3
 8004e92:	1a58      	subs	r0, r3, r1
 8004e94:	4243      	negs	r3, r0
 8004e96:	4143      	adcs	r3, r0
 8004e98:	2905      	cmp	r1, #5
 8004e9a:	6013      	str	r3, [r2, #0]
 8004e9c:	d807      	bhi.n	8004eae <onTubeHVFrequencyMenuGetOption+0x26>
 8004e9e:	4b06      	ldr	r3, [pc, #24]	; (8004eb8 <onTubeHVFrequencyMenuGetOption+0x30>)
 8004ea0:	4806      	ldr	r0, [pc, #24]	; (8004ebc <onTubeHVFrequencyMenuGetOption+0x34>)
 8004ea2:	f853 1021 	ldr.w	r1, [r3, r1, lsl #2]
 8004ea6:	f001 f8d9 	bl	800605c <strcpy>
 8004eaa:	4804      	ldr	r0, [pc, #16]	; (8004ebc <onTubeHVFrequencyMenuGetOption+0x34>)
 8004eac:	bd08      	pop	{r3, pc}
 8004eae:	2000      	movs	r0, #0
 8004eb0:	e7fc      	b.n	8004eac <onTubeHVFrequencyMenuGetOption+0x24>
 8004eb2:	bf00      	nop
 8004eb4:	20000da0 	andcs	r0, r0, r0, lsr #27
 8004eb8:	0800777c 	stmdaeq	r0, {r2, r3, r4, r5, r6, r8, r9, sl, ip, sp, lr}
 8004ebc:	20000d7c 	andcs	r0, r0, ip, ror sp

08004ec0 <onTubeHVDutyCycleMenuGetOption>:
 8004ec0:	b510      	push	{r4, lr}
 8004ec2:	460c      	mov	r4, r1
 8004ec4:	4b12      	ldr	r3, [pc, #72]	; (8004f10 <onTubeHVDutyCycleMenuGetOption+0x50>)
 8004ec6:	889b      	ldrh	r3, [r3, #4]
 8004ec8:	f3c3 1388 	ubfx	r3, r3, #6, #9
 8004ecc:	1a59      	subs	r1, r3, r1
 8004ece:	424b      	negs	r3, r1
 8004ed0:	414b      	adcs	r3, r1
 8004ed2:	f5b4 7fb4 	cmp.w	r4, #360	; 0x168
 8004ed6:	6013      	str	r3, [r2, #0]
 8004ed8:	d217      	bcs.n	8004f0a <onTubeHVDutyCycleMenuGetOption+0x4a>
 8004eda:	490e      	ldr	r1, [pc, #56]	; (8004f14 <onTubeHVDutyCycleMenuGetOption+0x54>)
 8004edc:	480e      	ldr	r0, [pc, #56]	; (8004f18 <onTubeHVDutyCycleMenuGetOption+0x58>)
 8004ede:	f001 f8bd 	bl	800605c <strcpy>
 8004ee2:	1c60      	adds	r0, r4, #1
 8004ee4:	f7ff fada 	bl	800449c <__aeabi_ui2f>
 8004ee8:	490c      	ldr	r1, [pc, #48]	; (8004f1c <onTubeHVDutyCycleMenuGetOption+0x5c>)
 8004eea:	f7ff fad3 	bl	8004494 <__aeabi_fmul>
 8004eee:	490c      	ldr	r1, [pc, #48]	; (8004f20 <onTubeHVDutyCycleMenuGetOption+0x60>)
 8004ef0:	f7ff fad0 	bl	8004494 <__aeabi_fmul>
 8004ef4:	2202      	movs	r2, #2
 8004ef6:	4601      	mov	r1, r0
 8004ef8:	4807      	ldr	r0, [pc, #28]	; (8004f18 <onTubeHVDutyCycleMenuGetOption+0x58>)
 8004efa:	f7fb fe3b 	bl	8000b74 <strcatFloat>
 8004efe:	4909      	ldr	r1, [pc, #36]	; (8004f24 <onTubeHVDutyCycleMenuGetOption+0x64>)
 8004f00:	4805      	ldr	r0, [pc, #20]	; (8004f18 <onTubeHVDutyCycleMenuGetOption+0x58>)
 8004f02:	f001 f89c 	bl	800603e <strcat>
 8004f06:	4804      	ldr	r0, [pc, #16]	; (8004f18 <onTubeHVDutyCycleMenuGetOption+0x58>)
 8004f08:	bd10      	pop	{r4, pc}
 8004f0a:	2000      	movs	r0, #0
 8004f0c:	e7fc      	b.n	8004f08 <onTubeHVDutyCycleMenuGetOption+0x48>
 8004f0e:	bf00      	nop
 8004f10:	20000da0 	andcs	r0, r0, r0, lsr #27
 8004f14:	08006506 	stmdaeq	r0, {r1, r2, r8, sl, sp, lr}
 8004f18:	20000d7c 	andcs	r0, r0, ip, ror sp
 8004f1c:	3b23d70a 	blcc	88fab4c <_sidata+0x8f32fc>
 8004f20:	42c80000 	sbcmi	r0, r8, #0
 8004f24:	080075d8 	stmdaeq	r0, {r3, r4, r6, r7, r8, sl, ip, sp, lr}

08004f28 <onTubeDeadTimeCompensationMenuSelect>:
 8004f28:	4b04      	ldr	r3, [pc, #16]	; (8004f3c <onTubeDeadTimeCompensationMenuSelect+0x14>)
 8004f2a:	6841      	ldr	r1, [r0, #4]
 8004f2c:	78da      	ldrb	r2, [r3, #3]
 8004f2e:	8849      	ldrh	r1, [r1, #2]
 8004f30:	f361 0205 	bfi	r2, r1, #0, #6
 8004f34:	70da      	strb	r2, [r3, #3]
 8004f36:	f7fd be55 	b.w	8002be4 <updateMeasurementUnits>
 8004f3a:	bf00      	nop
 8004f3c:	20000da0 	andcs	r0, r0, r0, lsr #27

08004f40 <onTubeConversionFactorMenuSelect>:
 8004f40:	4b04      	ldr	r3, [pc, #16]	; (8004f54 <onTubeConversionFactorMenuSelect+0x14>)
 8004f42:	6841      	ldr	r1, [r0, #4]
 8004f44:	789a      	ldrb	r2, [r3, #2]
 8004f46:	8849      	ldrh	r1, [r1, #2]
 8004f48:	f361 0247 	bfi	r2, r1, #1, #7
 8004f4c:	709a      	strb	r2, [r3, #2]
 8004f4e:	f7fd be49 	b.w	8002be4 <updateMeasurementUnits>
 8004f52:	bf00      	nop
 8004f54:	20000da0 	andcs	r0, r0, r0, lsr #27

08004f58 <onHVCustomProfileWarningEvent>:
 8004f58:	2901      	cmp	r1, #1
 8004f5a:	b508      	push	{r3, lr}
 8004f5c:	d008      	beq.n	8004f70 <onHVCustomProfileWarningEvent+0x18>
 8004f5e:	d302      	bcc.n	8004f66 <onHVCustomProfileWarningEvent+0xe>
 8004f60:	2906      	cmp	r1, #6
 8004f62:	d00f      	beq.n	8004f84 <onHVCustomProfileWarningEvent+0x2c>
 8004f64:	bd08      	pop	{r3, pc}
 8004f66:	480b      	ldr	r0, [pc, #44]	; (8004f94 <onHVCustomProfileWarningEvent+0x3c>)
 8004f68:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8004f6c:	f000 b98c 	b.w	8005288 <setView>
 8004f70:	2103      	movs	r1, #3
 8004f72:	4b09      	ldr	r3, [pc, #36]	; (8004f98 <onHVCustomProfileWarningEvent+0x40>)
 8004f74:	791a      	ldrb	r2, [r3, #4]
 8004f76:	f361 0202 	bfi	r2, r1, #0, #3
 8004f7a:	711a      	strb	r2, [r3, #4]
 8004f7c:	f7ff fde4 	bl	8004b48 <updateTubeHV>
 8004f80:	4806      	ldr	r0, [pc, #24]	; (8004f9c <onHVCustomProfileWarningEvent+0x44>)
 8004f82:	e7f1      	b.n	8004f68 <onHVCustomProfileWarningEvent+0x10>
 8004f84:	2200      	movs	r2, #0
 8004f86:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8004f8a:	4905      	ldr	r1, [pc, #20]	; (8004fa0 <onHVCustomProfileWarningEvent+0x48>)
 8004f8c:	4805      	ldr	r0, [pc, #20]	; (8004fa4 <onHVCustomProfileWarningEvent+0x4c>)
 8004f8e:	f7fc babb 	b.w	8001508 <drawNotification>
 8004f92:	bf00      	nop
 8004f94:	080077c0 	stmdaeq	r0, {r6, r7, r8, r9, sl, ip, sp, lr}
 8004f98:	20000da0 	andcs	r0, r0, r0, lsr #27
 8004f9c:	0800773c 	stmdaeq	r0, {r2, r3, r4, r5, r8, r9, sl, ip, sp, lr}
 8004fa0:	080075a4 	stmdaeq	r0, {r2, r5, r7, r8, sl, ip, sp, lr}
 8004fa4:	08006ff2 	stmdaeq	r0, {r1, r4, r5, r6, r7, r8, r9, sl, fp, sp, lr}

08004fa8 <onTubeDeadTimeCompensationMenuGetOption>:
 8004fa8:	b510      	push	{r4, lr}
 8004faa:	460c      	mov	r4, r1
 8004fac:	4b15      	ldr	r3, [pc, #84]	; (8005004 <onTubeDeadTimeCompensationMenuGetOption+0x5c>)
 8004fae:	78db      	ldrb	r3, [r3, #3]
 8004fb0:	f003 033f 	and.w	r3, r3, #63	; 0x3f
 8004fb4:	1a59      	subs	r1, r3, r1
 8004fb6:	424b      	negs	r3, r1
 8004fb8:	414b      	adcs	r3, r1
 8004fba:	6013      	str	r3, [r2, #0]
 8004fbc:	b1f4      	cbz	r4, 8004ffc <onTubeDeadTimeCompensationMenuGetOption+0x54>
 8004fbe:	2c3f      	cmp	r4, #63	; 0x3f
 8004fc0:	d81e      	bhi.n	8005000 <onTubeDeadTimeCompensationMenuGetOption+0x58>
 8004fc2:	4911      	ldr	r1, [pc, #68]	; (8005008 <onTubeDeadTimeCompensationMenuGetOption+0x60>)
 8004fc4:	4811      	ldr	r0, [pc, #68]	; (800500c <onTubeDeadTimeCompensationMenuGetOption+0x64>)
 8004fc6:	f001 f849 	bl	800605c <strcpy>
 8004fca:	1e60      	subs	r0, r4, #1
 8004fcc:	f7ff fa66 	bl	800449c <__aeabi_ui2f>
 8004fd0:	490f      	ldr	r1, [pc, #60]	; (8005010 <onTubeDeadTimeCompensationMenuGetOption+0x68>)
 8004fd2:	f7ff fa5f 	bl	8004494 <__aeabi_fmul>
 8004fd6:	f7ff fa87 	bl	80044e8 <exp2f>
 8004fda:	490e      	ldr	r1, [pc, #56]	; (8005014 <onTubeDeadTimeCompensationMenuGetOption+0x6c>)
 8004fdc:	f7ff fa5a 	bl	8004494 <__aeabi_fmul>
 8004fe0:	490d      	ldr	r1, [pc, #52]	; (8005018 <onTubeDeadTimeCompensationMenuGetOption+0x70>)
 8004fe2:	f7ff fa57 	bl	8004494 <__aeabi_fmul>
 8004fe6:	2201      	movs	r2, #1
 8004fe8:	4601      	mov	r1, r0
 8004fea:	4808      	ldr	r0, [pc, #32]	; (800500c <onTubeDeadTimeCompensationMenuGetOption+0x64>)
 8004fec:	f7fb fdc2 	bl	8000b74 <strcatFloat>
 8004ff0:	490a      	ldr	r1, [pc, #40]	; (800501c <onTubeDeadTimeCompensationMenuGetOption+0x74>)
 8004ff2:	4806      	ldr	r0, [pc, #24]	; (800500c <onTubeDeadTimeCompensationMenuGetOption+0x64>)
 8004ff4:	f001 f823 	bl	800603e <strcat>
 8004ff8:	4804      	ldr	r0, [pc, #16]	; (800500c <onTubeDeadTimeCompensationMenuGetOption+0x64>)
 8004ffa:	bd10      	pop	{r4, pc}
 8004ffc:	4808      	ldr	r0, [pc, #32]	; (8005020 <onTubeDeadTimeCompensationMenuGetOption+0x78>)
 8004ffe:	e7fc      	b.n	8004ffa <onTubeDeadTimeCompensationMenuGetOption+0x52>
 8005000:	2000      	movs	r0, #0
 8005002:	e7fa      	b.n	8004ffa <onTubeDeadTimeCompensationMenuGetOption+0x52>
 8005004:	20000da0 	andcs	r0, r0, r0, lsr #27
 8005008:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}
 800500c:	20000d7c 	andcs	r0, r0, ip, ror sp
 8005010:	3d842108 	stfccs	f2, [r4, #32]
 8005014:	3827c5ac 	stmdacc	r7!, {r2, r3, r5, r7, r8, sl, lr, pc}
 8005018:	49742400 	ldmdbmi	r4!, {sl, sp}^
 800501c:	08006508 	stmdaeq	r0, {r3, r8, sl, sp, lr}
 8005020:	08006184 	stmdaeq	r0, {r2, r7, r8, sp, lr}

08005024 <onTubeConversionFactorMenuGetOption>:
 8005024:	4b19      	ldr	r3, [pc, #100]	; (800508c <onTubeConversionFactorMenuGetOption+0x68>)
 8005026:	b510      	push	{r4, lr}
 8005028:	789b      	ldrb	r3, [r3, #2]
 800502a:	460c      	mov	r4, r1
 800502c:	ebb1 0f53 	cmp.w	r1, r3, lsr #1
 8005030:	bf0c      	ite	eq
 8005032:	2301      	moveq	r3, #1
 8005034:	2300      	movne	r3, #0
 8005036:	2905      	cmp	r1, #5
 8005038:	6013      	str	r3, [r2, #0]
 800503a:	d80c      	bhi.n	8005056 <onTubeConversionFactorMenuGetOption+0x32>
 800503c:	4b14      	ldr	r3, [pc, #80]	; (8005090 <onTubeConversionFactorMenuGetOption+0x6c>)
 800503e:	4815      	ldr	r0, [pc, #84]	; (8005094 <onTubeConversionFactorMenuGetOption+0x70>)
 8005040:	f853 1021 	ldr.w	r1, [r3, r1, lsl #2]
 8005044:	f001 f80a 	bl	800605c <strcpy>
 8005048:	b91c      	cbnz	r4, 8005052 <onTubeConversionFactorMenuGetOption+0x2e>
 800504a:	4913      	ldr	r1, [pc, #76]	; (8005098 <onTubeConversionFactorMenuGetOption+0x74>)
 800504c:	4811      	ldr	r0, [pc, #68]	; (8005094 <onTubeConversionFactorMenuGetOption+0x70>)
 800504e:	f000 fff6 	bl	800603e <strcat>
 8005052:	4810      	ldr	r0, [pc, #64]	; (8005094 <onTubeConversionFactorMenuGetOption+0x70>)
 8005054:	bd10      	pop	{r4, pc}
 8005056:	2945      	cmp	r1, #69	; 0x45
 8005058:	d815      	bhi.n	8005086 <onTubeConversionFactorMenuGetOption+0x62>
 800505a:	4910      	ldr	r1, [pc, #64]	; (800509c <onTubeConversionFactorMenuGetOption+0x78>)
 800505c:	480d      	ldr	r0, [pc, #52]	; (8005094 <onTubeConversionFactorMenuGetOption+0x70>)
 800505e:	f000 fffd 	bl	800605c <strcpy>
 8005062:	1fa0      	subs	r0, r4, #6
 8005064:	f7ff fa1a 	bl	800449c <__aeabi_ui2f>
 8005068:	490d      	ldr	r1, [pc, #52]	; (80050a0 <onTubeConversionFactorMenuGetOption+0x7c>)
 800506a:	f7ff fa13 	bl	8004494 <__aeabi_fmul>
 800506e:	f7ff fa3b 	bl	80044e8 <exp2f>
 8005072:	490c      	ldr	r1, [pc, #48]	; (80050a4 <onTubeConversionFactorMenuGetOption+0x80>)
 8005074:	f7ff fa0e 	bl	8004494 <__aeabi_fmul>
 8005078:	2201      	movs	r2, #1
 800507a:	4601      	mov	r1, r0
 800507c:	4805      	ldr	r0, [pc, #20]	; (8005094 <onTubeConversionFactorMenuGetOption+0x70>)
 800507e:	f7fb fd79 	bl	8000b74 <strcatFloat>
 8005082:	4909      	ldr	r1, [pc, #36]	; (80050a8 <onTubeConversionFactorMenuGetOption+0x84>)
 8005084:	e7e2      	b.n	800504c <onTubeConversionFactorMenuGetOption+0x28>
 8005086:	2000      	movs	r0, #0
 8005088:	e7e4      	b.n	8005054 <onTubeConversionFactorMenuGetOption+0x30>
 800508a:	bf00      	nop
 800508c:	20000da0 	andcs	r0, r0, r0, lsr #27
 8005090:	080076c8 	stmdaeq	r0, {r3, r6, r7, r9, sl, ip, sp, lr}
 8005094:	20000d7c 	andcs	r0, r0, ip, ror sp
 8005098:	080075c2 	stmdaeq	r0, {r1, r6, r7, r8, sl, ip, sp, lr}
 800509c:	080062da 	stmdaeq	r0, {r1, r3, r4, r6, r7, r9, sp, lr}
 80050a0:	3d820821 	stccc	8, cr0, [r2, #132]	; 0x84
 80050a4:	41c80000 	bicmi	r0, r8, r0
 80050a8:	080075cd 	stmdaeq	r0, {r0, r2, r3, r6, r7, r8, sl, ip, sp, lr}

080050ac <initTube>:
 80050ac:	b510      	push	{r4, lr}
 80050ae:	4c14      	ldr	r4, [pc, #80]	; (8005100 <initTube+0x54>)
 80050b0:	f7ff fd6e 	bl	8004b90 <initTubeController>
 80050b4:	78a1      	ldrb	r1, [r4, #2]
 80050b6:	2240      	movs	r2, #64	; 0x40
 80050b8:	0849      	lsrs	r1, r1, #1
 80050ba:	4812      	ldr	r0, [pc, #72]	; (8005104 <initTube+0x58>)
 80050bc:	f7fe fad6 	bl	800366c <selectMenuItem>
 80050c0:	78e1      	ldrb	r1, [r4, #3]
 80050c2:	2240      	movs	r2, #64	; 0x40
 80050c4:	f001 013f 	and.w	r1, r1, #63	; 0x3f
 80050c8:	480f      	ldr	r0, [pc, #60]	; (8005108 <initTube+0x5c>)
 80050ca:	f7fe facf 	bl	800366c <selectMenuItem>
 80050ce:	7921      	ldrb	r1, [r4, #4]
 80050d0:	2204      	movs	r2, #4
 80050d2:	f001 0107 	and.w	r1, r1, #7
 80050d6:	480d      	ldr	r0, [pc, #52]	; (800510c <initTube+0x60>)
 80050d8:	f7fe fac8 	bl	800366c <selectMenuItem>
 80050dc:	7921      	ldrb	r1, [r4, #4]
 80050de:	2206      	movs	r2, #6
 80050e0:	f3c1 01c2 	ubfx	r1, r1, #3, #3
 80050e4:	480a      	ldr	r0, [pc, #40]	; (8005110 <initTube+0x64>)
 80050e6:	f7fe fac1 	bl	800366c <selectMenuItem>
 80050ea:	88a1      	ldrh	r1, [r4, #4]
 80050ec:	f44f 72b4 	mov.w	r2, #360	; 0x168
 80050f0:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 80050f4:	f3c1 1188 	ubfx	r1, r1, #6, #9
 80050f8:	4806      	ldr	r0, [pc, #24]	; (8005114 <initTube+0x68>)
 80050fa:	f7fe bab7 	b.w	800366c <selectMenuItem>
 80050fe:	bf00      	nop
 8005100:	20000da0 	andcs	r0, r0, r0, lsr #27
 8005104:	080076b4 	stmdaeq	r0, {r2, r4, r5, r7, r9, sl, ip, sp, lr}
 8005108:	08007700 	stmdaeq	r0, {r8, r9, sl, ip, sp, lr}
 800510c:	08007798 	stmdaeq	r0, {r3, r4, r7, r8, r9, sl, ip, sp, lr}
 8005110:	08007768 	stmdaeq	r0, {r3, r5, r6, r8, r9, sl, ip, sp, lr}
 8005114:	0800774c 	stmdaeq	r0, {r2, r3, r6, r8, r9, sl, ip, sp, lr}

08005118 <getTubeConversionFactor>:
 8005118:	b508      	push	{r3, lr}
 800511a:	4b0a      	ldr	r3, [pc, #40]	; (8005144 <getTubeConversionFactor+0x2c>)
 800511c:	7898      	ldrb	r0, [r3, #2]
 800511e:	0840      	lsrs	r0, r0, #1
 8005120:	2805      	cmp	r0, #5
 8005122:	d803      	bhi.n	800512c <getTubeConversionFactor+0x14>
 8005124:	4b08      	ldr	r3, [pc, #32]	; (8005148 <getTubeConversionFactor+0x30>)
 8005126:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
 800512a:	bd08      	pop	{r3, pc}
 800512c:	3806      	subs	r0, #6
 800512e:	f7ff f9b5 	bl	800449c <__aeabi_ui2f>
 8005132:	4906      	ldr	r1, [pc, #24]	; (800514c <getTubeConversionFactor+0x34>)
 8005134:	f7ff f9ae 	bl	8004494 <__aeabi_fmul>
 8005138:	f7ff f9d6 	bl	80044e8 <exp2f>
 800513c:	4904      	ldr	r1, [pc, #16]	; (8005150 <getTubeConversionFactor+0x38>)
 800513e:	f7ff f9a9 	bl	8004494 <__aeabi_fmul>
 8005142:	e7f2      	b.n	800512a <getTubeConversionFactor+0x12>
 8005144:	20000da0 	andcs	r0, r0, r0, lsr #27
 8005148:	080076e0 	stmdaeq	r0, {r5, r6, r7, r9, sl, ip, sp, lr}
 800514c:	3d820821 	stccc	8, cr0, [r2, #132]	; 0x84
 8005150:	41c80000 	bicmi	r0, r8, r0

08005154 <getTubeDeadTimeCompensation>:
 8005154:	b508      	push	{r3, lr}
 8005156:	4b09      	ldr	r3, [pc, #36]	; (800517c <getTubeDeadTimeCompensation+0x28>)
 8005158:	78d8      	ldrb	r0, [r3, #3]
 800515a:	f010 003f 	ands.w	r0, r0, #63	; 0x3f
 800515e:	d00b      	beq.n	8005178 <getTubeDeadTimeCompensation+0x24>
 8005160:	3801      	subs	r0, #1
 8005162:	f7ff f99b 	bl	800449c <__aeabi_ui2f>
 8005166:	4906      	ldr	r1, [pc, #24]	; (8005180 <getTubeDeadTimeCompensation+0x2c>)
 8005168:	f7ff f994 	bl	8004494 <__aeabi_fmul>
 800516c:	f7ff f9bc 	bl	80044e8 <exp2f>
 8005170:	4904      	ldr	r1, [pc, #16]	; (8005184 <getTubeDeadTimeCompensation+0x30>)
 8005172:	f7ff f98f 	bl	8004494 <__aeabi_fmul>
 8005176:	bd08      	pop	{r3, pc}
 8005178:	2000      	movs	r0, #0
 800517a:	e7fc      	b.n	8005176 <getTubeDeadTimeCompensation+0x22>
 800517c:	20000da0 	andcs	r0, r0, r0, lsr #27
 8005180:	3d842108 	stfccs	f2, [r4, #32]
 8005184:	3827c5ac 	stmdacc	r7!, {r2, r3, r5, r7, r8, sl, lr, pc}

08005188 <getTubeHVFrequency>:
 8005188:	b508      	push	{r3, lr}
 800518a:	4b0c      	ldr	r3, [pc, #48]	; (80051bc <getTubeHVFrequency+0x34>)
 800518c:	791a      	ldrb	r2, [r3, #4]
 800518e:	f002 0207 	and.w	r2, r2, #7
 8005192:	2a03      	cmp	r2, #3
 8005194:	d80e      	bhi.n	80051b4 <getTubeHVFrequency+0x2c>
 8005196:	e8df f002 	tbb	[pc, r2]
 800519a:	0402020f 	streq	r0, [r2], #-527	; 0xfffffdf1
 800519e:	4808      	ldr	r0, [pc, #32]	; (80051c0 <getTubeHVFrequency+0x38>)
 80051a0:	bd08      	pop	{r3, pc}
 80051a2:	f240 40e2 	movw	r0, #1250	; 0x4e2
 80051a6:	791b      	ldrb	r3, [r3, #4]
 80051a8:	f3c3 03c2 	ubfx	r3, r3, #3, #3
 80051ac:	4098      	lsls	r0, r3
 80051ae:	f7ff f975 	bl	800449c <__aeabi_ui2f>
 80051b2:	e7f5      	b.n	80051a0 <getTubeHVFrequency+0x18>
 80051b4:	4803      	ldr	r0, [pc, #12]	; (80051c4 <getTubeHVFrequency+0x3c>)
 80051b6:	e7f3      	b.n	80051a0 <getTubeHVFrequency+0x18>
 80051b8:	4803      	ldr	r0, [pc, #12]	; (80051c8 <getTubeHVFrequency+0x40>)
 80051ba:	e7f1      	b.n	80051a0 <getTubeHVFrequency+0x18>
 80051bc:	20000da0 	andcs	r0, r0, r0, lsr #27
 80051c0:	451c4000 	ldrmi	r4, [ip, #-0]
 80051c4:	449c4000 	ldrmi	r4, [ip], #0
 80051c8:	471c4000 	ldrmi	r4, [ip, -r0]

080051cc <getTubeHVDutyCycle>:
 80051cc:	b508      	push	{r3, lr}
 80051ce:	4b0e      	ldr	r3, [pc, #56]	; (8005208 <getTubeHVDutyCycle+0x3c>)
 80051d0:	791a      	ldrb	r2, [r3, #4]
 80051d2:	f002 0207 	and.w	r2, r2, #7
 80051d6:	2a03      	cmp	r2, #3
 80051d8:	d811      	bhi.n	80051fe <getTubeHVDutyCycle+0x32>
 80051da:	e8df f002 	tbb	[pc, r2]
 80051de:	06040212 			; <UNDEFINED> instruction: 0x06040212
 80051e2:	480a      	ldr	r0, [pc, #40]	; (800520c <getTubeHVDutyCycle+0x40>)
 80051e4:	bd08      	pop	{r3, pc}
 80051e6:	480a      	ldr	r0, [pc, #40]	; (8005210 <getTubeHVDutyCycle+0x44>)
 80051e8:	e7fc      	b.n	80051e4 <getTubeHVDutyCycle+0x18>
 80051ea:	8898      	ldrh	r0, [r3, #4]
 80051ec:	f3c0 1088 	ubfx	r0, r0, #6, #9
 80051f0:	3001      	adds	r0, #1
 80051f2:	f7ff f953 	bl	800449c <__aeabi_ui2f>
 80051f6:	4907      	ldr	r1, [pc, #28]	; (8005214 <getTubeHVDutyCycle+0x48>)
 80051f8:	f7ff f94c 	bl	8004494 <__aeabi_fmul>
 80051fc:	e7f2      	b.n	80051e4 <getTubeHVDutyCycle+0x18>
 80051fe:	2000      	movs	r0, #0
 8005200:	e7f0      	b.n	80051e4 <getTubeHVDutyCycle+0x18>
 8005202:	f04f 507c 	mov.w	r0, #1056964608	; 0x3f000000
 8005206:	e7ed      	b.n	80051e4 <getTubeHVDutyCycle+0x18>
 8005208:	20000da0 	andcs	r0, r0, r0, lsr #27
 800520c:	3d851eb8 	stccc	14, cr1, [r5, #736]	; 0x2e0
 8005210:	3cf5c28f 	lfmcc	f4, 3, [r5], #572	; 0x23c
 8005214:	3b23d70a 	blcc	88fae44 <_sidata+0x8f35f4>

08005218 <dispatchViewEvents>:
 8005218:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 800521a:	f7fc fe23 	bl	8001e64 <syncTimerThread>
 800521e:	f7fc fe7d 	bl	8001f1c <isDisplayTimerActive>
 8005222:	4605      	mov	r5, r0
 8005224:	4e16      	ldr	r6, [pc, #88]	; (8005280 <dispatchViewEvents+0x68>)
 8005226:	4f17      	ldr	r7, [pc, #92]	; (8005284 <dispatchViewEvents+0x6c>)
 8005228:	f7fd fa7e 	bl	8002728 <getKeyboardEvent>
 800522c:	2807      	cmp	r0, #7
 800522e:	4604      	mov	r4, r0
 8005230:	d016      	beq.n	8005260 <dispatchViewEvents+0x48>
 8005232:	88f3      	ldrh	r3, [r6, #6]
 8005234:	f413 73e0 	ands.w	r3, r3, #448	; 0x1c0
 8005238:	d003      	beq.n	8005242 <dispatchViewEvents+0x2a>
 800523a:	f5b3 7fa0 	cmp.w	r3, #320	; 0x140
 800523e:	d000      	beq.n	8005242 <dispatchViewEvents+0x2a>
 8005240:	b15d      	cbz	r5, 800525a <dispatchViewEvents+0x42>
 8005242:	2c05      	cmp	r4, #5
 8005244:	d102      	bne.n	800524c <dispatchViewEvents+0x34>
 8005246:	f7fe faa3 	bl	8003790 <requestPowerOff>
 800524a:	e7ed      	b.n	8005228 <dispatchViewEvents+0x10>
 800524c:	f7fc fe4e 	bl	8001eec <triggerDisplay>
 8005250:	6838      	ldr	r0, [r7, #0]
 8005252:	4621      	mov	r1, r4
 8005254:	6803      	ldr	r3, [r0, #0]
 8005256:	4798      	blx	r3
 8005258:	e7e6      	b.n	8005228 <dispatchViewEvents+0x10>
 800525a:	f7fc fe47 	bl	8001eec <triggerDisplay>
 800525e:	e7e3      	b.n	8005228 <dispatchViewEvents+0x10>
 8005260:	4c08      	ldr	r4, [pc, #32]	; (8005284 <dispatchViewEvents+0x6c>)
 8005262:	7923      	ldrb	r3, [r4, #4]
 8005264:	b15b      	cbz	r3, 800527e <dispatchViewEvents+0x66>
 8005266:	2300      	movs	r3, #0
 8005268:	7123      	strb	r3, [r4, #4]
 800526a:	f7fc f8d5 	bl	8001418 <drawStatusBar>
 800526e:	6820      	ldr	r0, [r4, #0]
 8005270:	2106      	movs	r1, #6
 8005272:	6803      	ldr	r3, [r0, #0]
 8005274:	4798      	blx	r3
 8005276:	e8bd 40f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, lr}
 800527a:	f7fc b8c7 	b.w	800140c <refreshDisplay>
 800527e:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 8005280:	20000da0 	andcs	r0, r0, r0, lsr #27
 8005284:	20000c74 	andcs	r0, r0, r4, ror ip

08005288 <setView>:
 8005288:	2201      	movs	r2, #1
 800528a:	4b02      	ldr	r3, [pc, #8]	; (8005294 <setView+0xc>)
 800528c:	6018      	str	r0, [r3, #0]
 800528e:	711a      	strb	r2, [r3, #4]
 8005290:	4770      	bx	lr
 8005292:	bf00      	nop
 8005294:	20000c74 	andcs	r0, r0, r4, ror ip

08005298 <updateView>:
 8005298:	2201      	movs	r2, #1
 800529a:	4b01      	ldr	r3, [pc, #4]	; (80052a0 <updateView+0x8>)
 800529c:	711a      	strb	r2, [r3, #4]
 800529e:	4770      	bx	lr
 80052a0:	20000c74 	andcs	r0, r0, r4, ror ip

080052a4 <getView>:
 80052a4:	4b01      	ldr	r3, [pc, #4]	; (80052ac <getView+0x8>)
 80052a6:	6818      	ldr	r0, [r3, #0]
 80052a8:	4770      	bx	lr
 80052aa:	bf00      	nop
 80052ac:	20000c74 	andcs	r0, r0, r4, ror ip

080052b0 <mr_st7565_set_display>:
 80052b0:	4a03      	ldr	r2, [pc, #12]	; (80052c0 <mr_st7565_set_display+0x10>)
 80052b2:	4b04      	ldr	r3, [pc, #16]	; (80052c4 <mr_st7565_set_display+0x14>)
 80052b4:	2900      	cmp	r1, #0
 80052b6:	bf14      	ite	ne
 80052b8:	4611      	movne	r1, r2
 80052ba:	4619      	moveq	r1, r3
 80052bc:	f000 b92b 	b.w	8005516 <mr_send_sequence>
 80052c0:	0800780d 	stmdaeq	r0, {r0, r2, r3, fp, ip, sp, lr}
 80052c4:	08007808 	stmdaeq	r0, {r3, fp, ip, sp, lr}

080052c8 <mr_st7565_refresh_display>:
 80052c8:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
 80052cc:	4604      	mov	r4, r0
 80052ce:	2500      	movs	r5, #0
 80052d0:	f04f 0808 	mov.w	r8, #8
 80052d4:	f9b4 302e 	ldrsh.w	r3, [r4, #46]	; 0x2e
 80052d8:	fa0f f985 	sxth.w	r9, r5
 80052dc:	fb93 f3f8 	sdiv	r3, r3, r8
 80052e0:	454b      	cmp	r3, r9
 80052e2:	dc01      	bgt.n	80052e8 <mr_st7565_refresh_display+0x20>
 80052e4:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 80052e8:	2600      	movs	r6, #0
 80052ea:	f065 014f 	orn	r1, r5, #79	; 0x4f
 80052ee:	b2c9      	uxtb	r1, r1
 80052f0:	4620      	mov	r0, r4
 80052f2:	f000 f8fa 	bl	80054ea <mr_send_command>
 80052f6:	2100      	movs	r1, #0
 80052f8:	4620      	mov	r0, r4
 80052fa:	f000 f8f6 	bl	80054ea <mr_send_command>
 80052fe:	2110      	movs	r1, #16
 8005300:	4620      	mov	r0, r4
 8005302:	f000 f8f2 	bl	80054ea <mr_send_command>
 8005306:	6a23      	ldr	r3, [r4, #32]
 8005308:	2000      	movs	r0, #0
 800530a:	4798      	blx	r3
 800530c:	6ba3      	ldr	r3, [r4, #56]	; 0x38
 800530e:	f9b4 702c 	ldrsh.w	r7, [r4, #44]	; 0x2c
 8005312:	fb09 3707 	mla	r7, r9, r7, r3
 8005316:	f9b4 202c 	ldrsh.w	r2, [r4, #44]	; 0x2c
 800531a:	b233      	sxth	r3, r6
 800531c:	429a      	cmp	r2, r3
 800531e:	f106 0601 	add.w	r6, r6, #1
 8005322:	dc01      	bgt.n	8005328 <mr_st7565_refresh_display+0x60>
 8005324:	3501      	adds	r5, #1
 8005326:	e7d5      	b.n	80052d4 <mr_st7565_refresh_display+0xc>
 8005328:	6a62      	ldr	r2, [r4, #36]	; 0x24
 800532a:	5cf8      	ldrb	r0, [r7, r3]
 800532c:	4790      	blx	r2
 800532e:	e7f2      	b.n	8005316 <mr_st7565_refresh_display+0x4e>

08005330 <mr_st7565_init>:
 8005330:	e92d 4ff7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8005334:	4604      	mov	r4, r0
 8005336:	e9dd 980c 	ldrd	r9, r8, [sp, #48]	; 0x30
 800533a:	e9dd 760e 	ldrd	r7, r6, [sp, #56]	; 0x38
 800533e:	469a      	mov	sl, r3
 8005340:	9101      	str	r1, [sp, #4]
 8005342:	4693      	mov	fp, r2
 8005344:	9d10      	ldr	r5, [sp, #64]	; 0x40
 8005346:	f000 f943 	bl	80055d0 <mr_init>
 800534a:	4620      	mov	r0, r4
 800534c:	4b0c      	ldr	r3, [pc, #48]	; (8005380 <mr_st7565_init+0x50>)
 800534e:	9901      	ldr	r1, [sp, #4]
 8005350:	6023      	str	r3, [r4, #0]
 8005352:	4b0c      	ldr	r3, [pc, #48]	; (8005384 <mr_st7565_init+0x54>)
 8005354:	85a1      	strh	r1, [r4, #44]	; 0x2c
 8005356:	6063      	str	r3, [r4, #4]
 8005358:	4b0b      	ldr	r3, [pc, #44]	; (8005388 <mr_st7565_init+0x58>)
 800535a:	490c      	ldr	r1, [pc, #48]	; (800538c <mr_st7565_init+0x5c>)
 800535c:	60a3      	str	r3, [r4, #8]
 800535e:	4b0c      	ldr	r3, [pc, #48]	; (8005390 <mr_st7565_init+0x60>)
 8005360:	f8a4 b02e 	strh.w	fp, [r4, #46]	; 0x2e
 8005364:	f884 a030 	strb.w	sl, [r4, #48]	; 0x30
 8005368:	f8c4 9038 	str.w	r9, [r4, #56]	; 0x38
 800536c:	e9c4 3805 	strd	r3, r8, [r4, #20]
 8005370:	e9c4 7607 	strd	r7, r6, [r4, #28]
 8005374:	6265      	str	r5, [r4, #36]	; 0x24
 8005376:	b003      	add	sp, #12
 8005378:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800537c:	f000 b8cb 	b.w	8005516 <mr_send_sequence>
 8005380:	080052b1 	stmdaeq	r0, {r0, r4, r5, r7, r9, ip, lr}
 8005384:	0800555b 	stmdaeq	r0, {r0, r1, r3, r4, r6, r8, sl, ip, lr}
 8005388:	0800562d 	stmdaeq	r0, {r0, r2, r3, r5, r9, sl, ip, lr}
 800538c:	08007812 	stmdaeq	r0, {r1, r4, fp, ip, sp, lr}
 8005390:	080052c9 	stmdaeq	r0, {r0, r3, r6, r7, r9, ip, lr}

08005394 <mr_decode_c_string>:
 8005394:	4602      	mov	r2, r0
 8005396:	6803      	ldr	r3, [r0, #0]
 8005398:	f813 0b01 	ldrb.w	r0, [r3], #1
 800539c:	6013      	str	r3, [r2, #0]
 800539e:	4770      	bx	lr

080053a0 <mr_get_unsigned_bits>:
 80053a0:	b570      	push	{r4, r5, r6, lr}
 80053a2:	7904      	ldrb	r4, [r0, #4]
 80053a4:	6806      	ldr	r6, [r0, #0]
 80053a6:	1862      	adds	r2, r4, r1
 80053a8:	b2d2      	uxtb	r2, r2
 80053aa:	7833      	ldrb	r3, [r6, #0]
 80053ac:	2a07      	cmp	r2, #7
 80053ae:	fa43 f504 	asr.w	r5, r3, r4
 80053b2:	d806      	bhi.n	80053c2 <mr_get_unsigned_bits+0x22>
 80053b4:	b2eb      	uxtb	r3, r5
 80053b6:	7102      	strb	r2, [r0, #4]
 80053b8:	2001      	movs	r0, #1
 80053ba:	4088      	lsls	r0, r1
 80053bc:	3801      	subs	r0, #1
 80053be:	4018      	ands	r0, r3
 80053c0:	bd70      	pop	{r4, r5, r6, pc}
 80053c2:	1c73      	adds	r3, r6, #1
 80053c4:	6003      	str	r3, [r0, #0]
 80053c6:	7873      	ldrb	r3, [r6, #1]
 80053c8:	f1c4 0408 	rsb	r4, r4, #8
 80053cc:	40a3      	lsls	r3, r4
 80053ce:	432b      	orrs	r3, r5
 80053d0:	3a08      	subs	r2, #8
 80053d2:	b2db      	uxtb	r3, r3
 80053d4:	b2d2      	uxtb	r2, r2
 80053d6:	e7ee      	b.n	80053b6 <mr_get_unsigned_bits+0x16>

080053d8 <mr_get_variable_length_word>:
 80053d8:	2300      	movs	r3, #0
 80053da:	b510      	push	{r4, lr}
 80053dc:	6802      	ldr	r2, [r0, #0]
 80053de:	f812 1b01 	ldrb.w	r1, [r2], #1
 80053e2:	f001 047f 	and.w	r4, r1, #127	; 0x7f
 80053e6:	6002      	str	r2, [r0, #0]
 80053e8:	060a      	lsls	r2, r1, #24
 80053ea:	ea44 13c3 	orr.w	r3, r4, r3, lsl #7
 80053ee:	d4f5      	bmi.n	80053dc <mr_get_variable_length_word+0x4>
 80053f0:	4618      	mov	r0, r3
 80053f2:	bd10      	pop	{r4, pc}

080053f4 <mr_get_glyph>:
 80053f4:	e92d 43f7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
 80053f8:	4604      	mov	r4, r0
 80053fa:	4688      	mov	r8, r1
 80053fc:	f04f 37ff 	mov.w	r7, #4294967295
 8005400:	f8d0 608c 	ldr.w	r6, [r0, #140]	; 0x8c
 8005404:	f106 0516 	add.w	r5, r6, #22
 8005408:	a801      	add	r0, sp, #4
 800540a:	9501      	str	r5, [sp, #4]
 800540c:	f7ff ffe4 	bl	80053d8 <mr_get_variable_length_word>
 8005410:	4681      	mov	r9, r0
 8005412:	b148      	cbz	r0, 8005428 <mr_get_glyph+0x34>
 8005414:	a801      	add	r0, sp, #4
 8005416:	f7ff ffdf 	bl	80053d8 <mr_get_variable_length_word>
 800541a:	4540      	cmp	r0, r8
 800541c:	dc04      	bgt.n	8005428 <mr_get_glyph+0x34>
 800541e:	9d01      	ldr	r5, [sp, #4]
 8005420:	4607      	mov	r7, r0
 8005422:	9500      	str	r5, [sp, #0]
 8005424:	444d      	add	r5, r9
 8005426:	e7ef      	b.n	8005408 <mr_get_glyph+0x14>
 8005428:	1c7b      	adds	r3, r7, #1
 800542a:	d106      	bne.n	800543a <mr_get_glyph+0x46>
 800542c:	2000      	movs	r0, #0
 800542e:	b003      	add	sp, #12
 8005430:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 8005434:	4403      	add	r3, r0
 8005436:	3701      	adds	r7, #1
 8005438:	9300      	str	r3, [sp, #0]
 800543a:	9b00      	ldr	r3, [sp, #0]
 800543c:	42ab      	cmp	r3, r5
 800543e:	d2f5      	bcs.n	800542c <mr_get_glyph+0x38>
 8005440:	4668      	mov	r0, sp
 8005442:	f7ff ffc9 	bl	80053d8 <mr_get_variable_length_word>
 8005446:	4547      	cmp	r7, r8
 8005448:	9b00      	ldr	r3, [sp, #0]
 800544a:	d1f3      	bne.n	8005434 <mr_get_glyph+0x40>
 800544c:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
 8005450:	2300      	movs	r3, #0
 8005452:	f884 3094 	strb.w	r3, [r4, #148]	; 0x94
 8005456:	7bb5      	ldrb	r5, [r6, #14]
 8005458:	f104 0790 	add.w	r7, r4, #144	; 0x90
 800545c:	4629      	mov	r1, r5
 800545e:	4638      	mov	r0, r7
 8005460:	f7ff ff9e 	bl	80053a0 <mr_get_unsigned_bits>
 8005464:	f1c5 0508 	rsb	r5, r5, #8
 8005468:	b2ed      	uxtb	r5, r5
 800546a:	b240      	sxtb	r0, r0
 800546c:	40a8      	lsls	r0, r5
 800546e:	b240      	sxtb	r0, r0
 8005470:	4128      	asrs	r0, r5
 8005472:	f884 0098 	strb.w	r0, [r4, #152]	; 0x98
 8005476:	7bf5      	ldrb	r5, [r6, #15]
 8005478:	4638      	mov	r0, r7
 800547a:	4629      	mov	r1, r5
 800547c:	f7ff ff90 	bl	80053a0 <mr_get_unsigned_bits>
 8005480:	f1c5 0508 	rsb	r5, r5, #8
 8005484:	b2ed      	uxtb	r5, r5
 8005486:	b240      	sxtb	r0, r0
 8005488:	40a8      	lsls	r0, r5
 800548a:	b240      	sxtb	r0, r0
 800548c:	4128      	asrs	r0, r5
 800548e:	f884 0099 	strb.w	r0, [r4, #153]	; 0x99
 8005492:	7c31      	ldrb	r1, [r6, #16]
 8005494:	4638      	mov	r0, r7
 8005496:	f7ff ff83 	bl	80053a0 <mr_get_unsigned_bits>
 800549a:	f884 009a 	strb.w	r0, [r4, #154]	; 0x9a
 800549e:	7c71      	ldrb	r1, [r6, #17]
 80054a0:	4638      	mov	r0, r7
 80054a2:	f7ff ff7d 	bl	80053a0 <mr_get_unsigned_bits>
 80054a6:	f884 009b 	strb.w	r0, [r4, #155]	; 0x9b
 80054aa:	7cb1      	ldrb	r1, [r6, #18]
 80054ac:	4638      	mov	r0, r7
 80054ae:	f7ff ff77 	bl	80053a0 <mr_get_unsigned_bits>
 80054b2:	f884 009c 	strb.w	r0, [r4, #156]	; 0x9c
 80054b6:	2001      	movs	r0, #1
 80054b8:	e7b9      	b.n	800542e <mr_get_glyph+0x3a>

080054ba <mr_get_string_width>:
 80054ba:	b5f7      	push	{r0, r1, r2, r4, r5, r6, r7, lr}
 80054bc:	4606      	mov	r6, r0
 80054be:	4617      	mov	r7, r2
 80054c0:	2400      	movs	r4, #0
 80054c2:	ad02      	add	r5, sp, #8
 80054c4:	f845 1d04 	str.w	r1, [r5, #-4]!
 80054c8:	4628      	mov	r0, r5
 80054ca:	47b8      	blx	r7
 80054cc:	4601      	mov	r1, r0
 80054ce:	b148      	cbz	r0, 80054e4 <mr_get_string_width+0x2a>
 80054d0:	4630      	mov	r0, r6
 80054d2:	f7ff ff8f 	bl	80053f4 <mr_get_glyph>
 80054d6:	2800      	cmp	r0, #0
 80054d8:	d0f6      	beq.n	80054c8 <mr_get_string_width+0xe>
 80054da:	f896 309c 	ldrb.w	r3, [r6, #156]	; 0x9c
 80054de:	441c      	add	r4, r3
 80054e0:	b224      	sxth	r4, r4
 80054e2:	e7f1      	b.n	80054c8 <mr_get_string_width+0xe>
 80054e4:	4620      	mov	r0, r4
 80054e6:	b003      	add	sp, #12
 80054e8:	bdf0      	pop	{r4, r5, r6, r7, pc}

080054ea <mr_send_command>:
 80054ea:	b570      	push	{r4, r5, r6, lr}
 80054ec:	4604      	mov	r4, r0
 80054ee:	460d      	mov	r5, r1
 80054f0:	6a03      	ldr	r3, [r0, #32]
 80054f2:	2001      	movs	r0, #1
 80054f4:	4798      	blx	r3
 80054f6:	6a63      	ldr	r3, [r4, #36]	; 0x24
 80054f8:	4628      	mov	r0, r5
 80054fa:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 80054fe:	4718      	bx	r3

08005500 <mr_send_data>:
 8005500:	b570      	push	{r4, r5, r6, lr}
 8005502:	4604      	mov	r4, r0
 8005504:	460d      	mov	r5, r1
 8005506:	6a03      	ldr	r3, [r0, #32]
 8005508:	2000      	movs	r0, #0
 800550a:	4798      	blx	r3
 800550c:	6a63      	ldr	r3, [r4, #36]	; 0x24
 800550e:	4628      	mov	r0, r5
 8005510:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 8005514:	4718      	bx	r3

08005516 <mr_send_sequence>:
 8005516:	b538      	push	{r3, r4, r5, lr}
 8005518:	4604      	mov	r4, r0
 800551a:	1c8d      	adds	r5, r1, #2
 800551c:	f815 3c02 	ldrb.w	r3, [r5, #-2]
 8005520:	f815 1c01 	ldrb.w	r1, [r5, #-1]
 8005524:	2b04      	cmp	r3, #4
 8005526:	d807      	bhi.n	8005538 <mr_send_sequence+0x22>
 8005528:	e8df f003 	tbb	[pc, r3]
 800552c:	120e0803 	andne	r0, lr, #196608	; 0x30000
 8005530:	69a30016 	stmibvs	r3!, {r1, r2, r4}
 8005534:	4608      	mov	r0, r1
 8005536:	4798      	blx	r3
 8005538:	3502      	adds	r5, #2
 800553a:	e7ef      	b.n	800551c <mr_send_sequence+0x6>
 800553c:	1c08      	adds	r0, r1, #0
 800553e:	69e3      	ldr	r3, [r4, #28]
 8005540:	bf18      	it	ne
 8005542:	2001      	movne	r0, #1
 8005544:	4798      	blx	r3
 8005546:	e7f7      	b.n	8005538 <mr_send_sequence+0x22>
 8005548:	4620      	mov	r0, r4
 800554a:	f7ff ffce 	bl	80054ea <mr_send_command>
 800554e:	e7f3      	b.n	8005538 <mr_send_sequence+0x22>
 8005550:	4620      	mov	r0, r4
 8005552:	f7ff ffd5 	bl	8005500 <mr_send_data>
 8005556:	e7ef      	b.n	8005538 <mr_send_sequence+0x22>
 8005558:	bd38      	pop	{r3, r4, r5, pc}

0800555a <mr_draw_rectangle_framebuffer_monochrome_vertical>:
 800555a:	f04f 0c01 	mov.w	ip, #1
 800555e:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
 8005562:	884c      	ldrh	r4, [r1, #2]
 8005564:	f9b1 3002 	ldrsh.w	r3, [r1, #2]
 8005568:	f9b1 2006 	ldrsh.w	r2, [r1, #6]
 800556c:	b227      	sxth	r7, r4
 800556e:	4413      	add	r3, r2
 8005570:	429f      	cmp	r7, r3
 8005572:	db01      	blt.n	8005578 <mr_draw_rectangle_framebuffer_monochrome_vertical+0x1e>
 8005574:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 8005578:	f004 0307 	and.w	r3, r4, #7
 800557c:	fa0c f303 	lsl.w	r3, ip, r3
 8005580:	fa5f fe83 	uxtb.w	lr, r3
 8005584:	43db      	mvns	r3, r3
 8005586:	880d      	ldrh	r5, [r1, #0]
 8005588:	10ff      	asrs	r7, r7, #3
 800558a:	b25a      	sxtb	r2, r3
 800558c:	f9b1 3000 	ldrsh.w	r3, [r1]
 8005590:	f9b1 6004 	ldrsh.w	r6, [r1, #4]
 8005594:	fa0f f885 	sxth.w	r8, r5
 8005598:	4433      	add	r3, r6
 800559a:	4598      	cmp	r8, r3
 800559c:	db02      	blt.n	80055a4 <mr_draw_rectangle_framebuffer_monochrome_vertical+0x4a>
 800559e:	3401      	adds	r4, #1
 80055a0:	b2a4      	uxth	r4, r4
 80055a2:	e7df      	b.n	8005564 <mr_draw_rectangle_framebuffer_monochrome_vertical+0xa>
 80055a4:	f9b0 602c 	ldrsh.w	r6, [r0, #44]	; 0x2c
 80055a8:	f9b0 9044 	ldrsh.w	r9, [r0, #68]	; 0x44
 80055ac:	fb07 8606 	mla	r6, r7, r6, r8
 80055b0:	f8d0 8038 	ldr.w	r8, [r0, #56]	; 0x38
 80055b4:	f1b9 0f00 	cmp.w	r9, #0
 80055b8:	f818 3006 	ldrb.w	r3, [r8, r6]
 80055bc:	f105 0501 	add.w	r5, r5, #1
 80055c0:	bfb4      	ite	lt
 80055c2:	ea4e 0303 	orrlt.w	r3, lr, r3
 80055c6:	4013      	andge	r3, r2
 80055c8:	f808 3006 	strb.w	r3, [r8, r6]
 80055cc:	b2ad      	uxth	r5, r5
 80055ce:	e7dd      	b.n	800558c <mr_draw_rectangle_framebuffer_monochrome_vertical+0x32>

080055d0 <mr_init>:
 80055d0:	22a0      	movs	r2, #160	; 0xa0
 80055d2:	2100      	movs	r1, #0
 80055d4:	f000 bd2b 	b.w	800602e <memset>

080055d8 <mr_set_display>:
 80055d8:	6803      	ldr	r3, [r0, #0]
 80055da:	4718      	bx	r3

080055dc <mr_refresh_display>:
 80055dc:	6943      	ldr	r3, [r0, #20]
 80055de:	b103      	cbz	r3, 80055e2 <mr_refresh_display+0x6>
 80055e0:	4718      	bx	r3
 80055e2:	4770      	bx	lr

080055e4 <mr_set_fill_color>:
 80055e4:	f8a0 1044 	strh.w	r1, [r0, #68]	; 0x44
 80055e8:	4770      	bx	lr

080055ea <mr_draw_rectangle>:
 80055ea:	6843      	ldr	r3, [r0, #4]
 80055ec:	4718      	bx	r3

080055ee <mr_set_font>:
 80055ee:	f8c0 108c 	str.w	r1, [r0, #140]	; 0x8c
 80055f2:	4770      	bx	lr

080055f4 <mr_set_text_color>:
 80055f4:	f8a0 1046 	strh.w	r1, [r0, #70]	; 0x46
 80055f8:	4770      	bx	lr
	...

080055fc <mr_draw_text>:
 80055fc:	b513      	push	{r0, r1, r4, lr}
 80055fe:	4c03      	ldr	r4, [pc, #12]	; (800560c <mr_draw_text+0x10>)
 8005600:	9400      	str	r4, [sp, #0]
 8005602:	6884      	ldr	r4, [r0, #8]
 8005604:	47a0      	blx	r4
 8005606:	b002      	add	sp, #8
 8005608:	bd10      	pop	{r4, pc}
 800560a:	bf00      	nop
 800560c:	08005395 	stmdaeq	r0, {r0, r2, r4, r7, r8, r9, ip, lr}

08005610 <mr_get_text_width>:
 8005610:	4a01      	ldr	r2, [pc, #4]	; (8005618 <mr_get_text_width+0x8>)
 8005612:	f7ff bf52 	b.w	80054ba <mr_get_string_width>
 8005616:	bf00      	nop
 8005618:	08005395 	stmdaeq	r0, {r0, r2, r4, r7, r8, r9, ip, lr}

0800561c <mr_get_ascent>:
 800561c:	f8d0 308c 	ldr.w	r3, [r0, #140]	; 0x8c
 8005620:	789a      	ldrb	r2, [r3, #2]
 8005622:	78d8      	ldrb	r0, [r3, #3]
 8005624:	ea40 2002 	orr.w	r0, r0, r2, lsl #8
 8005628:	b200      	sxth	r0, r0
 800562a:	4770      	bx	lr

0800562c <mr_draw_string_framebuffer_monochrome_vertical>:
 800562c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8005630:	4690      	mov	r8, r2
 8005632:	461e      	mov	r6, r3
 8005634:	4604      	mov	r4, r0
 8005636:	b095      	sub	sp, #84	; 0x54
 8005638:	ad14      	add	r5, sp, #80	; 0x50
 800563a:	f845 1d04 	str.w	r1, [r5, #-4]!
 800563e:	4611      	mov	r1, r2
 8005640:	9507      	str	r5, [sp, #28]
 8005642:	f7ff ffd2 	bl	80055ea <mr_draw_rectangle>
 8005646:	8870      	ldrh	r0, [r6, #2]
 8005648:	f8b8 1002 	ldrh.w	r1, [r8, #2]
 800564c:	8833      	ldrh	r3, [r6, #0]
 800564e:	f8b8 5000 	ldrh.w	r5, [r8]
 8005652:	4401      	add	r1, r0
 8005654:	4620      	mov	r0, r4
 8005656:	441d      	add	r5, r3
 8005658:	b289      	uxth	r1, r1
 800565a:	f7ff ffdf 	bl	800561c <mr_get_ascent>
 800565e:	4408      	add	r0, r1
 8005660:	b283      	uxth	r3, r0
 8005662:	b22d      	sxth	r5, r5
 8005664:	930d      	str	r3, [sp, #52]	; 0x34
 8005666:	9807      	ldr	r0, [sp, #28]
 8005668:	9b1e      	ldr	r3, [sp, #120]	; 0x78
 800566a:	4798      	blx	r3
 800566c:	4601      	mov	r1, r0
 800566e:	2800      	cmp	r0, #0
 8005670:	f000 80eb 	beq.w	800584a <mr_draw_string_framebuffer_monochrome_vertical+0x21e>
 8005674:	4620      	mov	r0, r4
 8005676:	f7ff febd 	bl	80053f4 <mr_get_glyph>
 800567a:	2800      	cmp	r0, #0
 800567c:	d0f3      	beq.n	8005666 <mr_draw_string_framebuffer_monochrome_vertical+0x3a>
 800567e:	f9b4 302c 	ldrsh.w	r3, [r4, #44]	; 0x2c
 8005682:	f04f 0b00 	mov.w	fp, #0
 8005686:	930e      	str	r3, [sp, #56]	; 0x38
 8005688:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
 800568c:	f8cd b030 	str.w	fp, [sp, #48]	; 0x30
 8005690:	9305      	str	r3, [sp, #20]
 8005692:	7cdb      	ldrb	r3, [r3, #19]
 8005694:	f8cd b018 	str.w	fp, [sp, #24]
 8005698:	9308      	str	r3, [sp, #32]
 800569a:	2301      	movs	r3, #1
 800569c:	9a08      	ldr	r2, [sp, #32]
 800569e:	f8cd b00c 	str.w	fp, [sp, #12]
 80056a2:	4093      	lsls	r3, r2
 80056a4:	3b01      	subs	r3, #1
 80056a6:	b21b      	sxth	r3, r3
 80056a8:	930f      	str	r3, [sp, #60]	; 0x3c
 80056aa:	f104 0390 	add.w	r3, r4, #144	; 0x90
 80056ae:	9302      	str	r3, [sp, #8]
 80056b0:	f894 309b 	ldrb.w	r3, [r4, #155]	; 0x9b
 80056b4:	f8cd b004 	str.w	fp, [sp, #4]
 80056b8:	9309      	str	r3, [sp, #36]	; 0x24
 80056ba:	f894 309a 	ldrb.w	r3, [r4, #154]	; 0x9a
 80056be:	9310      	str	r3, [sp, #64]	; 0x40
 80056c0:	b2ab      	uxth	r3, r5
 80056c2:	930a      	str	r3, [sp, #40]	; 0x28
 80056c4:	9a0a      	ldr	r2, [sp, #40]	; 0x28
 80056c6:	f994 3098 	ldrsb.w	r3, [r4, #152]	; 0x98
 80056ca:	4413      	add	r3, r2
 80056cc:	b21b      	sxth	r3, r3
 80056ce:	930b      	str	r3, [sp, #44]	; 0x2c
 80056d0:	9a09      	ldr	r2, [sp, #36]	; 0x24
 80056d2:	f994 3099 	ldrsb.w	r3, [r4, #153]	; 0x99
 80056d6:	9e0b      	ldr	r6, [sp, #44]	; 0x2c
 80056d8:	4413      	add	r3, r2
 80056da:	9a0d      	ldr	r2, [sp, #52]	; 0x34
 80056dc:	1ad3      	subs	r3, r2, r3
 80056de:	fa0f f983 	sxth.w	r9, r3
 80056e2:	9b09      	ldr	r3, [sp, #36]	; 0x24
 80056e4:	9a06      	ldr	r2, [sp, #24]
 80056e6:	4293      	cmp	r3, r2
 80056e8:	dc05      	bgt.n	80056f6 <mr_draw_string_framebuffer_monochrome_vertical+0xca>
 80056ea:	f894 509c 	ldrb.w	r5, [r4, #156]	; 0x9c
 80056ee:	9b0a      	ldr	r3, [sp, #40]	; 0x28
 80056f0:	442b      	add	r3, r5
 80056f2:	b21d      	sxth	r5, r3
 80056f4:	e7b7      	b.n	8005666 <mr_draw_string_framebuffer_monochrome_vertical+0x3a>
 80056f6:	9908      	ldr	r1, [sp, #32]
 80056f8:	9802      	ldr	r0, [sp, #8]
 80056fa:	f7ff fe51 	bl	80053a0 <mr_get_unsigned_bits>
 80056fe:	4603      	mov	r3, r0
 8005700:	b1d8      	cbz	r0, 800573a <mr_draw_string_framebuffer_monochrome_vertical+0x10e>
 8005702:	9a0f      	ldr	r2, [sp, #60]	; 0x3c
 8005704:	4290      	cmp	r0, r2
 8005706:	d03a      	beq.n	800577e <mr_draw_string_framebuffer_monochrome_vertical+0x152>
 8005708:	2501      	movs	r5, #1
 800570a:	f04f 0a01 	mov.w	sl, #1
 800570e:	f64f 72ff 	movw	r2, #65535	; 0xffff
 8005712:	3d01      	subs	r5, #1
 8005714:	b2ad      	uxth	r5, r5
 8005716:	4295      	cmp	r5, r2
 8005718:	d153      	bne.n	80057c2 <mr_draw_string_framebuffer_monochrome_vertical+0x196>
 800571a:	9b01      	ldr	r3, [sp, #4]
 800571c:	2b00      	cmp	r3, #0
 800571e:	d0e0      	beq.n	80056e2 <mr_draw_string_framebuffer_monochrome_vertical+0xb6>
 8005720:	3b01      	subs	r3, #1
 8005722:	9301      	str	r3, [sp, #4]
 8005724:	d1dd      	bne.n	80056e2 <mr_draw_string_framebuffer_monochrome_vertical+0xb6>
 8005726:	2f00      	cmp	r7, #0
 8005728:	f000 808d 	beq.w	8005846 <mr_draw_string_framebuffer_monochrome_vertical+0x21a>
 800572c:	9b0c      	ldr	r3, [sp, #48]	; 0x30
 800572e:	3f01      	subs	r7, #1
 8005730:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
 8005734:	f884 b094 	strb.w	fp, [r4, #148]	; 0x94
 8005738:	e03b      	b.n	80057b2 <mr_draw_string_framebuffer_monochrome_vertical+0x186>
 800573a:	9a05      	ldr	r2, [sp, #20]
 800573c:	f992 5014 	ldrsb.w	r5, [r2, #20]
 8005740:	b2e9      	uxtb	r1, r5
 8005742:	9802      	ldr	r0, [sp, #8]
 8005744:	9312      	str	r3, [sp, #72]	; 0x48
 8005746:	f7ff fe2b 	bl	80053a0 <mr_get_unsigned_bits>
 800574a:	f04f 0a00 	mov.w	sl, #0
 800574e:	b283      	uxth	r3, r0
 8005750:	9311      	str	r3, [sp, #68]	; 0x44
 8005752:	9b12      	ldr	r3, [sp, #72]	; 0x48
 8005754:	2101      	movs	r1, #1
 8005756:	9802      	ldr	r0, [sp, #8]
 8005758:	9312      	str	r3, [sp, #72]	; 0x48
 800575a:	f7ff fe21 	bl	80053a0 <mr_get_unsigned_bits>
 800575e:	f10a 0101 	add.w	r1, sl, #1
 8005762:	9b12      	ldr	r3, [sp, #72]	; 0x48
 8005764:	b978      	cbnz	r0, 8005786 <mr_draw_string_framebuffer_monochrome_vertical+0x15a>
 8005766:	fa1f fa8a 	uxth.w	sl, sl
 800576a:	9911      	ldr	r1, [sp, #68]	; 0x44
 800576c:	fa0a f205 	lsl.w	r2, sl, r5
 8005770:	4411      	add	r1, r2
 8005772:	b28d      	uxth	r5, r1
 8005774:	bb13      	cbnz	r3, 80057bc <mr_draw_string_framebuffer_monochrome_vertical+0x190>
 8005776:	2d00      	cmp	r5, #0
 8005778:	d1c7      	bne.n	800570a <mr_draw_string_framebuffer_monochrome_vertical+0xde>
 800577a:	2302      	movs	r3, #2
 800577c:	e007      	b.n	800578e <mr_draw_string_framebuffer_monochrome_vertical+0x162>
 800577e:	9a05      	ldr	r2, [sp, #20]
 8005780:	f992 5015 	ldrsb.w	r5, [r2, #21]
 8005784:	e7dc      	b.n	8005740 <mr_draw_string_framebuffer_monochrome_vertical+0x114>
 8005786:	468a      	mov	sl, r1
 8005788:	e7e4      	b.n	8005754 <mr_draw_string_framebuffer_monochrome_vertical+0x128>
 800578a:	9b04      	ldr	r3, [sp, #16]
 800578c:	3301      	adds	r3, #1
 800578e:	2101      	movs	r1, #1
 8005790:	9802      	ldr	r0, [sp, #8]
 8005792:	9304      	str	r3, [sp, #16]
 8005794:	f7ff fe04 	bl	80053a0 <mr_get_unsigned_bits>
 8005798:	2800      	cmp	r0, #0
 800579a:	d1f6      	bne.n	800578a <mr_draw_string_framebuffer_monochrome_vertical+0x15e>
 800579c:	2701      	movs	r7, #1
 800579e:	2101      	movs	r1, #1
 80057a0:	9802      	ldr	r0, [sp, #8]
 80057a2:	f7ff fdfd 	bl	80053a0 <mr_get_unsigned_bits>
 80057a6:	b938      	cbnz	r0, 80057b8 <mr_draw_string_framebuffer_monochrome_vertical+0x18c>
 80057a8:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
 80057ac:	f894 b094 	ldrb.w	fp, [r4, #148]	; 0x94
 80057b0:	930c      	str	r3, [sp, #48]	; 0x30
 80057b2:	9b04      	ldr	r3, [sp, #16]
 80057b4:	9301      	str	r3, [sp, #4]
 80057b6:	e794      	b.n	80056e2 <mr_draw_string_framebuffer_monochrome_vertical+0xb6>
 80057b8:	3701      	adds	r7, #1
 80057ba:	e7f0      	b.n	800579e <mr_draw_string_framebuffer_monochrome_vertical+0x172>
 80057bc:	3501      	adds	r5, #1
 80057be:	b2ad      	uxth	r5, r5
 80057c0:	e7a3      	b.n	800570a <mr_draw_string_framebuffer_monochrome_vertical+0xde>
 80057c2:	b333      	cbz	r3, 8005812 <mr_draw_string_framebuffer_monochrome_vertical+0x1e6>
 80057c4:	f9b8 1000 	ldrsh.w	r1, [r8]
 80057c8:	428e      	cmp	r6, r1
 80057ca:	db22      	blt.n	8005812 <mr_draw_string_framebuffer_monochrome_vertical+0x1e6>
 80057cc:	f9b8 2004 	ldrsh.w	r2, [r8, #4]
 80057d0:	440a      	add	r2, r1
 80057d2:	4296      	cmp	r6, r2
 80057d4:	da1d      	bge.n	8005812 <mr_draw_string_framebuffer_monochrome_vertical+0x1e6>
 80057d6:	f9b8 1002 	ldrsh.w	r1, [r8, #2]
 80057da:	4549      	cmp	r1, r9
 80057dc:	dc19      	bgt.n	8005812 <mr_draw_string_framebuffer_monochrome_vertical+0x1e6>
 80057de:	f9b8 2006 	ldrsh.w	r2, [r8, #6]
 80057e2:	440a      	add	r2, r1
 80057e4:	4591      	cmp	r9, r2
 80057e6:	da14      	bge.n	8005812 <mr_draw_string_framebuffer_monochrome_vertical+0x1e6>
 80057e8:	9a0e      	ldr	r2, [sp, #56]	; 0x38
 80057ea:	ea4f 01e9 	mov.w	r1, r9, asr #3
 80057ee:	fb02 6101 	mla	r1, r2, r1, r6
 80057f2:	f8d4 c038 	ldr.w	ip, [r4, #56]	; 0x38
 80057f6:	f8b4 e046 	ldrh.w	lr, [r4, #70]	; 0x46
 80057fa:	f009 0007 	and.w	r0, r9, #7
 80057fe:	fa0a f000 	lsl.w	r0, sl, r0
 8005802:	f81c 2001 	ldrb.w	r2, [ip, r1]
 8005806:	f1be 0f00 	cmp.w	lr, #0
 800580a:	d00d      	beq.n	8005828 <mr_draw_string_framebuffer_monochrome_vertical+0x1fc>
 800580c:	4302      	orrs	r2, r0
 800580e:	f80c 2001 	strb.w	r2, [ip, r1]
 8005812:	9a03      	ldr	r2, [sp, #12]
 8005814:	3201      	adds	r2, #1
 8005816:	b212      	sxth	r2, r2
 8005818:	9203      	str	r2, [sp, #12]
 800581a:	9903      	ldr	r1, [sp, #12]
 800581c:	9a10      	ldr	r2, [sp, #64]	; 0x40
 800581e:	428a      	cmp	r2, r1
 8005820:	dd05      	ble.n	800582e <mr_draw_string_framebuffer_monochrome_vertical+0x202>
 8005822:	3601      	adds	r6, #1
 8005824:	b236      	sxth	r6, r6
 8005826:	e772      	b.n	800570e <mr_draw_string_framebuffer_monochrome_vertical+0xe2>
 8005828:	ea22 0200 	bic.w	r2, r2, r0
 800582c:	e7ef      	b.n	800580e <mr_draw_string_framebuffer_monochrome_vertical+0x1e2>
 800582e:	f109 0201 	add.w	r2, r9, #1
 8005832:	fa0f f982 	sxth.w	r9, r2
 8005836:	9a06      	ldr	r2, [sp, #24]
 8005838:	9e0b      	ldr	r6, [sp, #44]	; 0x2c
 800583a:	3201      	adds	r2, #1
 800583c:	b212      	sxth	r2, r2
 800583e:	9206      	str	r2, [sp, #24]
 8005840:	2200      	movs	r2, #0
 8005842:	9203      	str	r2, [sp, #12]
 8005844:	e763      	b.n	800570e <mr_draw_string_framebuffer_monochrome_vertical+0xe2>
 8005846:	9701      	str	r7, [sp, #4]
 8005848:	e74b      	b.n	80056e2 <mr_draw_string_framebuffer_monochrome_vertical+0xb6>
 800584a:	b015      	add	sp, #84	; 0x54
 800584c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}

08005850 <mcumax_search>:
 8005850:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8005854:	b09d      	sub	sp, #116	; 0x74
 8005856:	9311      	str	r3, [sp, #68]	; 0x44
 8005858:	f89d 3098 	ldrb.w	r3, [sp, #152]	; 0x98
 800585c:	9206      	str	r2, [sp, #24]
 800585e:	9319      	str	r3, [sp, #100]	; 0x64
 8005860:	4a84      	ldr	r2, [pc, #528]	; (8005a74 <mcumax_search+0x224>)
 8005862:	f89d 309c 	ldrb.w	r3, [sp, #156]	; 0x9c
 8005866:	4605      	mov	r5, r0
 8005868:	930e      	str	r3, [sp, #56]	; 0x38
 800586a:	f8d2 30a4 	ldr.w	r3, [r2, #164]	; 0xa4
 800586e:	460c      	mov	r4, r1
 8005870:	b113      	cbz	r3, 8005878 <mcumax_search+0x28>
 8005872:	f8d2 00a8 	ldr.w	r0, [r2, #168]	; 0xa8
 8005876:	4798      	blx	r3
 8005878:	462b      	mov	r3, r5
 800587a:	9a06      	ldr	r2, [sp, #24]
 800587c:	4295      	cmp	r5, r2
 800587e:	bfb8      	it	lt
 8005880:	3b01      	sublt	r3, #1
 8005882:	9315      	str	r3, [sp, #84]	; 0x54
 8005884:	4623      	mov	r3, r4
 8005886:	42a2      	cmp	r2, r4
 8005888:	bfa8      	it	ge
 800588a:	3b01      	subge	r3, #1
 800588c:	9305      	str	r3, [sp, #20]
 800588e:	2300      	movs	r3, #0
 8005890:	461f      	mov	r7, r3
 8005892:	4c78      	ldr	r4, [pc, #480]	; (8005a74 <mcumax_search+0x224>)
 8005894:	9309      	str	r3, [sp, #36]	; 0x24
 8005896:	9313      	str	r3, [sp, #76]	; 0x4c
 8005898:	9a19      	ldr	r2, [sp, #100]	; 0x64
 800589a:	1c5e      	adds	r6, r3, #1
 800589c:	4293      	cmp	r3, r2
 800589e:	b2f6      	uxtb	r6, r6
 80058a0:	d31c      	bcc.n	80058dc <mcumax_search+0x8c>
 80058a2:	2e02      	cmp	r6, #2
 80058a4:	d91a      	bls.n	80058dc <mcumax_search+0x8c>
 80058a6:	9b0e      	ldr	r3, [sp, #56]	; 0x38
 80058a8:	2b00      	cmp	r3, #0
 80058aa:	f000 82a5 	beq.w	8005df8 <mcumax_search+0x5a8>
 80058ae:	f894 3090 	ldrb.w	r3, [r4, #144]	; 0x90
 80058b2:	2b80      	cmp	r3, #128	; 0x80
 80058b4:	f040 82a0 	bne.w	8005df8 <mcumax_search+0x5a8>
 80058b8:	e9d4 2325 	ldrd	r2, r3, [r4, #148]	; 0x94
 80058bc:	429a      	cmp	r2, r3
 80058be:	d203      	bcs.n	80058c8 <mcumax_search+0x78>
 80058c0:	f8d4 309c 	ldr.w	r3, [r4, #156]	; 0x9c
 80058c4:	429e      	cmp	r6, r3
 80058c6:	d909      	bls.n	80058dc <mcumax_search+0x8c>
 80058c8:	2603      	movs	r6, #3
 80058ca:	f89d 304c 	ldrb.w	r3, [sp, #76]	; 0x4c
 80058ce:	f884 3090 	strb.w	r3, [r4, #144]	; 0x90
 80058d2:	9b09      	ldr	r3, [sp, #36]	; 0x24
 80058d4:	f003 0377 	and.w	r3, r3, #119	; 0x77
 80058d8:	f884 3091 	strb.w	r3, [r4, #145]	; 0x91
 80058dc:	f894 20a0 	ldrb.w	r2, [r4, #160]	; 0xa0
 80058e0:	2a00      	cmp	r2, #0
 80058e2:	f040 8289 	bne.w	8005df8 <mcumax_search+0x5a8>
 80058e6:	9b0e      	ldr	r3, [sp, #56]	; 0x38
 80058e8:	2b01      	cmp	r3, #1
 80058ea:	9b13      	ldr	r3, [sp, #76]	; 0x4c
 80058ec:	bf08      	it	eq
 80058ee:	2300      	moveq	r3, #0
 80058f0:	9314      	str	r3, [sp, #80]	; 0x50
 80058f2:	9b09      	ldr	r3, [sp, #36]	; 0x24
 80058f4:	2e02      	cmp	r6, #2
 80058f6:	f003 0380 	and.w	r3, r3, #128	; 0x80
 80058fa:	930a      	str	r3, [sp, #40]	; 0x28
 80058fc:	f894 3081 	ldrb.w	r3, [r4, #129]	; 0x81
 8005900:	f083 0318 	eor.w	r3, r3, #24
 8005904:	f884 3081 	strb.w	r3, [r4, #129]	; 0x81
 8005908:	f240 81be 	bls.w	8005c88 <mcumax_search+0x438>
 800590c:	9b05      	ldr	r3, [sp, #20]
 800590e:	f513 5ffa 	cmn.w	r3, #8000	; 0x1f40
 8005912:	f000 81b9 	beq.w	8005c88 <mcumax_search+0x438>
 8005916:	9201      	str	r2, [sp, #4]
 8005918:	9905      	ldr	r1, [sp, #20]
 800591a:	9a06      	ldr	r2, [sp, #24]
 800591c:	9805      	ldr	r0, [sp, #20]
 800591e:	1ef3      	subs	r3, r6, #3
 8005920:	b2db      	uxtb	r3, r3
 8005922:	9300      	str	r3, [sp, #0]
 8005924:	4252      	negs	r2, r2
 8005926:	2380      	movs	r3, #128	; 0x80
 8005928:	f1c1 0101 	rsb	r1, r1, #1
 800592c:	4240      	negs	r0, r0
 800592e:	f7ff ff8f 	bl	8005850 <mcumax_search>
 8005932:	9010      	str	r0, [sp, #64]	; 0x40
 8005934:	f894 3081 	ldrb.w	r3, [r4, #129]	; 0x81
 8005938:	f083 0318 	eor.w	r3, r3, #24
 800593c:	f884 3081 	strb.w	r3, [r4, #129]	; 0x81
 8005940:	9b10      	ldr	r3, [sp, #64]	; 0x40
 8005942:	425f      	negs	r7, r3
 8005944:	9b05      	ldr	r3, [sp, #20]
 8005946:	429f      	cmp	r7, r3
 8005948:	db03      	blt.n	8005952 <mcumax_search+0x102>
 800594a:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
 800594e:	2b23      	cmp	r3, #35	; 0x23
 8005950:	dd04      	ble.n	800595c <mcumax_search+0x10c>
 8005952:	4f49      	ldr	r7, [pc, #292]	; (8005a78 <mcumax_search+0x228>)
 8005954:	9b06      	ldr	r3, [sp, #24]
 8005956:	2e02      	cmp	r6, #2
 8005958:	bf08      	it	eq
 800595a:	461f      	moveq	r7, r3
 800595c:	f8d4 3094 	ldr.w	r3, [r4, #148]	; 0x94
 8005960:	f8dd b050 	ldr.w	fp, [sp, #80]	; 0x50
 8005964:	3301      	adds	r3, #1
 8005966:	f8c4 3094 	str.w	r3, [r4, #148]	; 0x94
 800596a:	f814 300b 	ldrb.w	r3, [r4, fp]
 800596e:	9308      	str	r3, [sp, #32]
 8005970:	9a08      	ldr	r2, [sp, #32]
 8005972:	f894 3081 	ldrb.w	r3, [r4, #129]	; 0x81
 8005976:	421a      	tst	r2, r3
 8005978:	f000 8228 	beq.w	8005dcc <mcumax_search+0x57c>
 800597c:	f002 0a07 	and.w	sl, r2, #7
 8005980:	fa4f f38a 	sxtb.w	r3, sl
 8005984:	9304      	str	r3, [sp, #16]
 8005986:	4b3d      	ldr	r3, [pc, #244]	; (8005a7c <mcumax_search+0x22c>)
 8005988:	f913 300a 	ldrsb.w	r3, [r3, sl]
 800598c:	9312      	str	r3, [sp, #72]	; 0x48
 800598e:	f1ba 0f02 	cmp.w	sl, #2
 8005992:	f240 820e 	bls.w	8005db2 <mcumax_search+0x562>
 8005996:	9b04      	ldr	r3, [sp, #16]
 8005998:	2b00      	cmp	r3, #0
 800599a:	f280 820a 	bge.w	8005db2 <mcumax_search+0x562>
 800599e:	425b      	negs	r3, r3
 80059a0:	b25b      	sxtb	r3, r3
 80059a2:	9304      	str	r3, [sp, #16]
 80059a4:	9b04      	ldr	r3, [sp, #16]
 80059a6:	f10b 0203 	add.w	r2, fp, #3
 80059aa:	f3c3 0342 	ubfx	r3, r3, #1, #3
 80059ae:	4053      	eors	r3, r2
 80059b0:	b2db      	uxtb	r3, r3
 80059b2:	930d      	str	r3, [sp, #52]	; 0x34
 80059b4:	f083 0301 	eor.w	r3, r3, #1
 80059b8:	9317      	str	r3, [sp, #92]	; 0x5c
 80059ba:	9b0d      	ldr	r3, [sp, #52]	; 0x34
 80059bc:	f083 0302 	eor.w	r3, r3, #2
 80059c0:	9318      	str	r3, [sp, #96]	; 0x60
 80059c2:	2380      	movs	r3, #128	; 0x80
 80059c4:	9307      	str	r3, [sp, #28]
 80059c6:	930b      	str	r3, [sp, #44]	; 0x2c
 80059c8:	9b0a      	ldr	r3, [sp, #40]	; 0x28
 80059ca:	465d      	mov	r5, fp
 80059cc:	2b00      	cmp	r3, #0
 80059ce:	f000 815f 	beq.w	8005c90 <mcumax_search+0x440>
 80059d2:	e9dd 3209 	ldrd	r3, r2, [sp, #36]	; 0x24
 80059d6:	ea83 0502 	eor.w	r5, r3, r2
 80059da:	f015 0f88 	tst.w	r5, #136	; 0x88
 80059de:	d1d6      	bne.n	800598e <mcumax_search+0x13e>
 80059e0:	9b11      	ldr	r3, [sp, #68]	; 0x44
 80059e2:	2b80      	cmp	r3, #128	; 0x80
 80059e4:	d00b      	beq.n	80059fe <mcumax_search+0x1ae>
 80059e6:	5ce3      	ldrb	r3, [r4, r3]
 80059e8:	b14b      	cbz	r3, 80059fe <mcumax_search+0x1ae>
 80059ea:	9b11      	ldr	r3, [sp, #68]	; 0x44
 80059ec:	1aeb      	subs	r3, r5, r3
 80059ee:	2b01      	cmp	r3, #1
 80059f0:	dc05      	bgt.n	80059fe <mcumax_search+0x1ae>
 80059f2:	9b11      	ldr	r3, [sp, #68]	; 0x44
 80059f4:	1b5b      	subs	r3, r3, r5
 80059f6:	2b01      	cmp	r3, #1
 80059f8:	bfd8      	it	le
 80059fa:	f44f 57fa 	movle.w	r7, #8000	; 0x1f40
 80059fe:	f1ba 0f02 	cmp.w	sl, #2
 8005a02:	f200 8149 	bhi.w	8005c98 <mcumax_search+0x448>
 8005a06:	9b11      	ldr	r3, [sp, #68]	; 0x44
 8005a08:	429d      	cmp	r5, r3
 8005a0a:	f040 8145 	bne.w	8005c98 <mcumax_search+0x448>
 8005a0e:	f085 0310 	eor.w	r3, r5, #16
 8005a12:	b2db      	uxtb	r3, r3
 8005a14:	930f      	str	r3, [sp, #60]	; 0x3c
 8005a16:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
 8005a18:	f894 c081 	ldrb.w	ip, [r4, #129]	; 0x81
 8005a1c:	5ce3      	ldrb	r3, [r4, r3]
 8005a1e:	ea1c 0f03 	tst.w	ip, r3
 8005a22:	9303      	str	r3, [sp, #12]
 8005a24:	d1b3      	bne.n	800598e <mcumax_search+0x13e>
 8005a26:	f1ba 0f02 	cmp.w	sl, #2
 8005a2a:	d80d      	bhi.n	8005a48 <mcumax_search+0x1f8>
 8005a2c:	eba5 030b 	sub.w	r3, r5, fp
 8005a30:	f013 0f07 	tst.w	r3, #7
 8005a34:	9b03      	ldr	r3, [sp, #12]
 8005a36:	bf08      	it	eq
 8005a38:	2201      	moveq	r2, #1
 8005a3a:	fab3 f383 	clz	r3, r3
 8005a3e:	bf18      	it	ne
 8005a40:	2200      	movne	r2, #0
 8005a42:	095b      	lsrs	r3, r3, #5
 8005a44:	429a      	cmp	r2, r3
 8005a46:	d1a2      	bne.n	800598e <mcumax_search+0x13e>
 8005a48:	2125      	movs	r1, #37	; 0x25
 8005a4a:	9b03      	ldr	r3, [sp, #12]
 8005a4c:	4a0c      	ldr	r2, [pc, #48]	; (8005a80 <mcumax_search+0x230>)
 8005a4e:	f003 0307 	and.w	r3, r3, #7
 8005a52:	56d2      	ldrsb	r2, [r2, r3]
 8005a54:	9b03      	ldr	r3, [sp, #12]
 8005a56:	f003 03c0 	and.w	r3, r3, #192	; 0xc0
 8005a5a:	fb01 3302 	mla	r3, r1, r2, r3
 8005a5e:	2b00      	cmp	r3, #0
 8005a60:	930c      	str	r3, [sp, #48]	; 0x30
 8005a62:	9b05      	ldr	r3, [sp, #20]
 8005a64:	f2c0 81cf 	blt.w	8005e06 <mcumax_search+0x5b6>
 8005a68:	429f      	cmp	r7, r3
 8005a6a:	db0b      	blt.n	8005a84 <mcumax_search+0x234>
 8005a6c:	2e01      	cmp	r6, #1
 8005a6e:	f200 81b5 	bhi.w	8005ddc <mcumax_search+0x58c>
 8005a72:	e007      	b.n	8005a84 <mcumax_search+0x234>
 8005a74:	20000dac 	andcs	r0, r0, ip, lsr #27
 8005a78:	ffffe0c0 			; <UNDEFINED> instruction: 0xffffe0c0
 8005a7c:	08007840 	stmdaeq	r0, {r6, fp, ip, sp, lr}
 8005a80:	08007827 	stmdaeq	r0, {r0, r1, r2, r5, fp, ip, sp, lr}
 8005a84:	2e01      	cmp	r6, #1
 8005a86:	bf0a      	itet	eq
 8005a88:	9b0c      	ldreq	r3, [sp, #48]	; 0x30
 8005a8a:	9806      	ldrne	r0, [sp, #24]
 8005a8c:	eba3 000a 	subeq.w	r0, r3, sl
 8005a90:	9b03      	ldr	r3, [sp, #12]
 8005a92:	2b00      	cmp	r3, #0
 8005a94:	bf14      	ite	ne
 8005a96:	4633      	movne	r3, r6
 8005a98:	1e73      	subeq	r3, r6, #1
 8005a9a:	2b01      	cmp	r3, #1
 8005a9c:	f340 8129 	ble.w	8005cf2 <mcumax_search+0x4a2>
 8005aa0:	f1ba 0f05 	cmp.w	sl, #5
 8005aa4:	bf9f      	itttt	ls
 8005aa6:	eb04 030b 	addls.w	r3, r4, fp
 8005aaa:	f893 9008 	ldrbls.w	r9, [r3, #8]
 8005aae:	1963      	addls	r3, r4, r5
 8005ab0:	7a1b      	ldrbls	r3, [r3, #8]
 8005ab2:	9a0f      	ldr	r2, [sp, #60]	; 0x3c
 8005ab4:	bf98      	it	ls
 8005ab6:	eba9 0903 	subls.w	r9, r9, r3
 8005aba:	f04f 0300 	mov.w	r3, #0
 8005abe:	f804 300b 	strb.w	r3, [r4, fp]
 8005ac2:	54a3      	strb	r3, [r4, r2]
 8005ac4:	9a07      	ldr	r2, [sp, #28]
 8005ac6:	bf88      	it	hi
 8005ac8:	f04f 0900 	movhi.w	r9, #0
 8005acc:	54a3      	strb	r3, [r4, r2]
 8005ace:	9b08      	ldr	r3, [sp, #32]
 8005ad0:	f012 0f88 	tst.w	r2, #136	; 0x88
 8005ad4:	f043 0320 	orr.w	r3, r3, #32
 8005ad8:	5563      	strb	r3, [r4, r5]
 8005ada:	4613      	mov	r3, r2
 8005adc:	bf01      	itttt	eq
 8005ade:	9a0b      	ldreq	r2, [sp, #44]	; 0x2c
 8005ae0:	f10c 0306 	addeq.w	r3, ip, #6
 8005ae4:	54a3      	strbeq	r3, [r4, r2]
 8005ae6:	f109 0932 	addeq.w	r9, r9, #50	; 0x32
 8005aea:	f1ba 0f04 	cmp.w	sl, #4
 8005aee:	f8d4 108c 	ldr.w	r1, [r4, #140]	; 0x8c
 8005af2:	f040 80d3 	bne.w	8005c9c <mcumax_search+0x44c>
 8005af6:	291e      	cmp	r1, #30
 8005af8:	bfcc      	ite	gt
 8005afa:	2200      	movgt	r2, #0
 8005afc:	2214      	movle	r2, #20
 8005afe:	f1ba 0f02 	cmp.w	sl, #2
 8005b02:	eba9 0202 	sub.w	r2, r9, r2
 8005b06:	d840      	bhi.n	8005b8a <mcumax_search+0x33a>
 8005b08:	f1ab 0302 	sub.w	r3, fp, #2
 8005b0c:	f013 0f88 	tst.w	r3, #136	; 0x88
 8005b10:	f040 80c6 	bne.w	8005ca0 <mcumax_search+0x450>
 8005b14:	5ce0      	ldrb	r0, [r4, r3]
 8005b16:	9b08      	ldr	r3, [sp, #32]
 8005b18:	1ac0      	subs	r0, r0, r3
 8005b1a:	bf18      	it	ne
 8005b1c:	2001      	movne	r0, #1
 8005b1e:	f10b 0302 	add.w	r3, fp, #2
 8005b22:	f013 0f88 	tst.w	r3, #136	; 0x88
 8005b26:	f040 80bd 	bne.w	8005ca4 <mcumax_search+0x454>
 8005b2a:	f814 e003 	ldrb.w	lr, [r4, r3]
 8005b2e:	9b08      	ldr	r3, [sp, #32]
 8005b30:	ebbe 0e03 	subs.w	lr, lr, r3
 8005b34:	bf18      	it	ne
 8005b36:	f04f 0e01 	movne.w	lr, #1
 8005b3a:	f08b 0310 	eor.w	r3, fp, #16
 8005b3e:	b2db      	uxtb	r3, r3
 8005b40:	f814 8003 	ldrb.w	r8, [r4, r3]
 8005b44:	f10c 0c24 	add.w	ip, ip, #36	; 0x24
 8005b48:	eb00 030e 	add.w	r3, r0, lr
 8005b4c:	45e0      	cmp	r8, ip
 8005b4e:	bf18      	it	ne
 8005b50:	3b01      	subne	r3, #1
 8005b52:	eb03 03c3 	add.w	r3, r3, r3, lsl #3
 8005b56:	eba3 03a1 	sub.w	r3, r3, r1, asr #2
 8005b5a:	1ad2      	subs	r2, r2, r3
 8005b5c:	9b04      	ldr	r3, [sp, #16]
 8005b5e:	442b      	add	r3, r5
 8005b60:	3301      	adds	r3, #1
 8005b62:	0618      	lsls	r0, r3, #24
 8005b64:	bf5f      	itttt	pl
 8005b66:	9b08      	ldrpl	r3, [sp, #32]
 8005b68:	f105 0010 	addpl.w	r0, r5, #16
 8005b6c:	f003 0320 	andpl.w	r3, r3, #32
 8005b70:	4003      	andpl	r3, r0
 8005b72:	bf48      	it	mi
 8005b74:	f5ca 7321 	rsbmi	r3, sl, #644	; 0x284
 8005b78:	980c      	ldr	r0, [sp, #48]	; 0x30
 8005b7a:	bf4c      	ite	mi
 8005b7c:	3303      	addmi	r3, #3
 8005b7e:	005b      	lslpl	r3, r3, #1
 8005b80:	4418      	add	r0, r3
 8005b82:	900c      	str	r0, [sp, #48]	; 0x30
 8005b84:	5d60      	ldrb	r0, [r4, r5]
 8005b86:	4403      	add	r3, r0
 8005b88:	5563      	strb	r3, [r4, r5]
 8005b8a:	9b0c      	ldr	r3, [sp, #48]	; 0x30
 8005b8c:	9806      	ldr	r0, [sp, #24]
 8005b8e:	eb03 0900 	add.w	r9, r3, r0
 8005b92:	9b15      	ldr	r3, [sp, #84]	; 0x54
 8005b94:	4491      	add	r9, r2
 8005b96:	42bb      	cmp	r3, r7
 8005b98:	bfb8      	it	lt
 8005b9a:	463b      	movlt	r3, r7
 8005b9c:	2e05      	cmp	r6, #5
 8005b9e:	9316      	str	r3, [sp, #88]	; 0x58
 8005ba0:	f240 8083 	bls.w	8005caa <mcumax_search+0x45a>
 8005ba4:	f1ba 0f02 	cmp.w	sl, #2
 8005ba8:	d97f      	bls.n	8005caa <mcumax_search+0x45a>
 8005baa:	9b03      	ldr	r3, [sp, #12]
 8005bac:	2b00      	cmp	r3, #0
 8005bae:	d17c      	bne.n	8005caa <mcumax_search+0x45a>
 8005bb0:	9b0a      	ldr	r3, [sp, #40]	; 0x28
 8005bb2:	fab3 f383 	clz	r3, r3
 8005bb6:	095b      	lsrs	r3, r3, #5
 8005bb8:	f106 38ff 	add.w	r8, r6, #4294967295
 8005bbc:	eba8 0803 	sub.w	r8, r8, r3
 8005bc0:	291e      	cmp	r1, #30
 8005bc2:	fa5f f888 	uxtb.w	r8, r8
 8005bc6:	dc0c      	bgt.n	8005be2 <mcumax_search+0x392>
 8005bc8:	9b10      	ldr	r3, [sp, #64]	; 0x40
 8005bca:	f5b3 5ffa 	cmp.w	r3, #8000	; 0x1f40
 8005bce:	d108      	bne.n	8005be2 <mcumax_search+0x392>
 8005bd0:	2e02      	cmp	r6, #2
 8005bd2:	d906      	bls.n	8005be2 <mcumax_search+0x392>
 8005bd4:	9b03      	ldr	r3, [sp, #12]
 8005bd6:	2b00      	cmp	r3, #0
 8005bd8:	d069      	beq.n	8005cae <mcumax_search+0x45e>
 8005bda:	f1ba 0f04 	cmp.w	sl, #4
 8005bde:	bf08      	it	eq
 8005be0:	46b0      	moveq	r8, r6
 8005be2:	f1c9 0300 	rsb	r3, r9, #0
 8005be6:	931a      	str	r3, [sp, #104]	; 0x68
 8005be8:	9b16      	ldr	r3, [sp, #88]	; 0x58
 8005bea:	425b      	negs	r3, r3
 8005bec:	931b      	str	r3, [sp, #108]	; 0x6c
 8005bee:	f894 3081 	ldrb.w	r3, [r4, #129]	; 0x81
 8005bf2:	f083 0318 	eor.w	r3, r3, #24
 8005bf6:	f884 3081 	strb.w	r3, [r4, #129]	; 0x81
 8005bfa:	9b0e      	ldr	r3, [sp, #56]	; 0x38
 8005bfc:	2b01      	cmp	r3, #1
 8005bfe:	d005      	beq.n	8005c0c <mcumax_search+0x3bc>
 8005c00:	f1b8 0f02 	cmp.w	r8, #2
 8005c04:	d802      	bhi.n	8005c0c <mcumax_search+0x3bc>
 8005c06:	9b16      	ldr	r3, [sp, #88]	; 0x58
 8005c08:	4599      	cmp	r9, r3
 8005c0a:	dd52      	ble.n	8005cb2 <mcumax_search+0x462>
 8005c0c:	e9dd 211a 	ldrd	r2, r1, [sp, #104]	; 0x68
 8005c10:	2300      	movs	r3, #0
 8005c12:	9805      	ldr	r0, [sp, #20]
 8005c14:	e9cd 8300 	strd	r8, r3, [sp]
 8005c18:	4240      	negs	r0, r0
 8005c1a:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
 8005c1c:	f7ff fe18 	bl	8005850 <mcumax_search>
 8005c20:	4240      	negs	r0, r0
 8005c22:	f894 2081 	ldrb.w	r2, [r4, #129]	; 0x81
 8005c26:	9915      	ldr	r1, [sp, #84]	; 0x54
 8005c28:	f082 0318 	eor.w	r3, r2, #24
 8005c2c:	4288      	cmp	r0, r1
 8005c2e:	f884 3081 	strb.w	r3, [r4, #129]	; 0x81
 8005c32:	dd05      	ble.n	8005c40 <mcumax_search+0x3f0>
 8005c34:	f108 0801 	add.w	r8, r8, #1
 8005c38:	fa5f f888 	uxtb.w	r8, r8
 8005c3c:	45b0      	cmp	r8, r6
 8005c3e:	d3d6      	bcc.n	8005bee <mcumax_search+0x39e>
 8005c40:	990e      	ldr	r1, [sp, #56]	; 0x38
 8005c42:	2903      	cmp	r1, #3
 8005c44:	d137      	bne.n	8005cb6 <mcumax_search+0x466>
 8005c46:	f510 5ffa 	cmn.w	r0, #8000	; 0x1f40
 8005c4a:	d034      	beq.n	8005cb6 <mcumax_search+0x466>
 8005c4c:	f894 1090 	ldrb.w	r1, [r4, #144]	; 0x90
 8005c50:	4559      	cmp	r1, fp
 8005c52:	d130      	bne.n	8005cb6 <mcumax_search+0x466>
 8005c54:	f894 1091 	ldrb.w	r1, [r4, #145]	; 0x91
 8005c58:	42a9      	cmp	r1, r5
 8005c5a:	d12c      	bne.n	8005cb6 <mcumax_search+0x466>
 8005c5c:	9b06      	ldr	r3, [sp, #24]
 8005c5e:	990c      	ldr	r1, [sp, #48]	; 0x30
 8005c60:	425b      	negs	r3, r3
 8005c62:	1a5b      	subs	r3, r3, r1
 8005c64:	f8c4 3084 	str.w	r3, [r4, #132]	; 0x84
 8005c68:	f89d 302c 	ldrb.w	r3, [sp, #44]	; 0x2c
 8005c6c:	f884 2081 	strb.w	r2, [r4, #129]	; 0x81
 8005c70:	f884 3088 	strb.w	r3, [r4, #136]	; 0x88
 8005c74:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
 8005c78:	eb03 13e1 	add.w	r3, r3, r1, asr #7
 8005c7c:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
 8005c80:	9805      	ldr	r0, [sp, #20]
 8005c82:	b01d      	add	sp, #116	; 0x74
 8005c84:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 8005c88:	f44f 53fa 	mov.w	r3, #8000	; 0x1f40
 8005c8c:	9310      	str	r3, [sp, #64]	; 0x40
 8005c8e:	e651      	b.n	8005934 <mcumax_search+0xe4>
 8005c90:	9b04      	ldr	r3, [sp, #16]
 8005c92:	441d      	add	r5, r3
 8005c94:	b2ed      	uxtb	r5, r5
 8005c96:	e6a0      	b.n	80059da <mcumax_search+0x18a>
 8005c98:	950f      	str	r5, [sp, #60]	; 0x3c
 8005c9a:	e6bc      	b.n	8005a16 <mcumax_search+0x1c6>
 8005c9c:	2200      	movs	r2, #0
 8005c9e:	e72e      	b.n	8005afe <mcumax_search+0x2ae>
 8005ca0:	2001      	movs	r0, #1
 8005ca2:	e73c      	b.n	8005b1e <mcumax_search+0x2ce>
 8005ca4:	f04f 0e01 	mov.w	lr, #1
 8005ca8:	e747      	b.n	8005b3a <mcumax_search+0x2ea>
 8005caa:	2300      	movs	r3, #0
 8005cac:	e784      	b.n	8005bb8 <mcumax_search+0x368>
 8005cae:	46b0      	mov	r8, r6
 8005cb0:	e797      	b.n	8005be2 <mcumax_search+0x392>
 8005cb2:	4648      	mov	r0, r9
 8005cb4:	e7b5      	b.n	8005c22 <mcumax_search+0x3d2>
 8005cb6:	9a07      	ldr	r2, [sp, #28]
 8005cb8:	3306      	adds	r3, #6
 8005cba:	54a3      	strb	r3, [r4, r2]
 8005cbc:	2300      	movs	r3, #0
 8005cbe:	9a0b      	ldr	r2, [sp, #44]	; 0x2c
 8005cc0:	5563      	strb	r3, [r4, r5]
 8005cc2:	54a3      	strb	r3, [r4, r2]
 8005cc4:	f89d 3020 	ldrb.w	r3, [sp, #32]
 8005cc8:	f804 300b 	strb.w	r3, [r4, fp]
 8005ccc:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
 8005cce:	461a      	mov	r2, r3
 8005cd0:	f89d 300c 	ldrb.w	r3, [sp, #12]
 8005cd4:	54a3      	strb	r3, [r4, r2]
 8005cd6:	9b0e      	ldr	r3, [sp, #56]	; 0x38
 8005cd8:	2b02      	cmp	r3, #2
 8005cda:	d11a      	bne.n	8005d12 <mcumax_search+0x4c2>
 8005cdc:	f510 5ffa 	cmn.w	r0, #8000	; 0x1f40
 8005ce0:	d007      	beq.n	8005cf2 <mcumax_search+0x4a2>
 8005ce2:	f894 3090 	ldrb.w	r3, [r4, #144]	; 0x90
 8005ce6:	455b      	cmp	r3, fp
 8005ce8:	d103      	bne.n	8005cf2 <mcumax_search+0x4a2>
 8005cea:	f894 3091 	ldrb.w	r3, [r4, #145]	; 0x91
 8005cee:	42ab      	cmp	r3, r5
 8005cf0:	d0c6      	beq.n	8005c80 <mcumax_search+0x430>
 8005cf2:	42b8      	cmp	r0, r7
 8005cf4:	dd08      	ble.n	8005d08 <mcumax_search+0x4b8>
 8005cf6:	4607      	mov	r7, r0
 8005cf8:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
 8005cfa:	f8cd b04c 	str.w	fp, [sp, #76]	; 0x4c
 8005cfe:	f023 037f 	bic.w	r3, r3, #127	; 0x7f
 8005d02:	432b      	orrs	r3, r5
 8005d04:	b2db      	uxtb	r3, r3
 8005d06:	9309      	str	r3, [sp, #36]	; 0x24
 8005d08:	9b0a      	ldr	r3, [sp, #40]	; 0x28
 8005d0a:	b30b      	cbz	r3, 8005d50 <mcumax_search+0x500>
 8005d0c:	2300      	movs	r3, #0
 8005d0e:	930a      	str	r3, [sp, #40]	; 0x28
 8005d10:	e657      	b.n	80059c2 <mcumax_search+0x172>
 8005d12:	9b0e      	ldr	r3, [sp, #56]	; 0x38
 8005d14:	2b01      	cmp	r3, #1
 8005d16:	d1ec      	bne.n	8005cf2 <mcumax_search+0x4a2>
 8005d18:	f510 5ffa 	cmn.w	r0, #8000	; 0x1f40
 8005d1c:	d0e9      	beq.n	8005cf2 <mcumax_search+0x4a2>
 8005d1e:	f894 3090 	ldrb.w	r3, [r4, #144]	; 0x90
 8005d22:	2b80      	cmp	r3, #128	; 0x80
 8005d24:	d1e5      	bne.n	8005cf2 <mcumax_search+0x4a2>
 8005d26:	2e03      	cmp	r6, #3
 8005d28:	d1e3      	bne.n	8005cf2 <mcumax_search+0x4a2>
 8005d2a:	9b0a      	ldr	r3, [sp, #40]	; 0x28
 8005d2c:	2b00      	cmp	r3, #0
 8005d2e:	d172      	bne.n	8005e16 <mcumax_search+0x5c6>
 8005d30:	e9d4 232c 	ldrd	r2, r3, [r4, #176]	; 0xb0
 8005d34:	4293      	cmp	r3, r2
 8005d36:	bf3f      	itttt	cc
 8005d38:	f8d4 20ac 	ldrcc.w	r2, [r4, #172]	; 0xac
 8005d3c:	eb02 0143 	addcc.w	r1, r2, r3, lsl #1
 8005d40:	f802 b013 	strbcc.w	fp, [r2, r3, lsl #1]
 8005d44:	704d      	strbcc	r5, [r1, #1]
 8005d46:	3301      	adds	r3, #1
 8005d48:	42b8      	cmp	r0, r7
 8005d4a:	f8c4 30b4 	str.w	r3, [r4, #180]	; 0xb4
 8005d4e:	dcd2      	bgt.n	8005cf6 <mcumax_search+0x4a6>
 8005d50:	9b04      	ldr	r3, [sp, #16]
 8005d52:	445b      	add	r3, fp
 8005d54:	429d      	cmp	r5, r3
 8005d56:	d11a      	bne.n	8005d8e <mcumax_search+0x53e>
 8005d58:	9b08      	ldr	r3, [sp, #32]
 8005d5a:	069b      	lsls	r3, r3, #26
 8005d5c:	d417      	bmi.n	8005d8e <mcumax_search+0x53e>
 8005d5e:	f1ba 0f02 	cmp.w	sl, #2
 8005d62:	d924      	bls.n	8005dae <mcumax_search+0x55e>
 8005d64:	f1ba 0f04 	cmp.w	sl, #4
 8005d68:	d111      	bne.n	8005d8e <mcumax_search+0x53e>
 8005d6a:	9b12      	ldr	r3, [sp, #72]	; 0x48
 8005d6c:	2b07      	cmp	r3, #7
 8005d6e:	d10e      	bne.n	8005d8e <mcumax_search+0x53e>
 8005d70:	9b0d      	ldr	r3, [sp, #52]	; 0x34
 8005d72:	f894 2081 	ldrb.w	r2, [r4, #129]	; 0x81
 8005d76:	5ce3      	ldrb	r3, [r4, r3]
 8005d78:	1a9b      	subs	r3, r3, r2
 8005d7a:	2b06      	cmp	r3, #6
 8005d7c:	d105      	bne.n	8005d8a <mcumax_search+0x53a>
 8005d7e:	9b17      	ldr	r3, [sp, #92]	; 0x5c
 8005d80:	5ce3      	ldrb	r3, [r4, r3]
 8005d82:	b913      	cbnz	r3, 8005d8a <mcumax_search+0x53a>
 8005d84:	9b18      	ldr	r3, [sp, #96]	; 0x60
 8005d86:	5ce3      	ldrb	r3, [r4, r3]
 8005d88:	b17b      	cbz	r3, 8005daa <mcumax_search+0x55a>
 8005d8a:	9b0d      	ldr	r3, [sp, #52]	; 0x34
 8005d8c:	9307      	str	r3, [sp, #28]
 8005d8e:	9b03      	ldr	r3, [sp, #12]
 8005d90:	f1ba 0f04 	cmp.w	sl, #4
 8005d94:	bf98      	it	ls
 8005d96:	3301      	addls	r3, #1
 8005d98:	b2db      	uxtb	r3, r3
 8005d9a:	9303      	str	r3, [sp, #12]
 8005d9c:	9b03      	ldr	r3, [sp, #12]
 8005d9e:	2b00      	cmp	r3, #0
 8005da0:	f43f af76 	beq.w	8005c90 <mcumax_search+0x440>
 8005da4:	2300      	movs	r3, #0
 8005da6:	930a      	str	r3, [sp, #40]	; 0x28
 8005da8:	e5f1      	b.n	800598e <mcumax_search+0x13e>
 8005daa:	9b0d      	ldr	r3, [sp, #52]	; 0x34
 8005dac:	9307      	str	r3, [sp, #28]
 8005dae:	950b      	str	r5, [sp, #44]	; 0x2c
 8005db0:	e7f4      	b.n	8005d9c <mcumax_search+0x54c>
 8005db2:	9b12      	ldr	r3, [sp, #72]	; 0x48
 8005db4:	3301      	adds	r3, #1
 8005db6:	b25b      	sxtb	r3, r3
 8005db8:	9312      	str	r3, [sp, #72]	; 0x48
 8005dba:	9a12      	ldr	r2, [sp, #72]	; 0x48
 8005dbc:	4b18      	ldr	r3, [pc, #96]	; (8005e20 <mcumax_search+0x5d0>)
 8005dbe:	5c9b      	ldrb	r3, [r3, r2]
 8005dc0:	425b      	negs	r3, r3
 8005dc2:	b25b      	sxtb	r3, r3
 8005dc4:	9304      	str	r3, [sp, #16]
 8005dc6:	2b00      	cmp	r3, #0
 8005dc8:	f47f adec 	bne.w	80059a4 <mcumax_search+0x154>
 8005dcc:	f10b 0309 	add.w	r3, fp, #9
 8005dd0:	f003 0b77 	and.w	fp, r3, #119	; 0x77
 8005dd4:	9b14      	ldr	r3, [sp, #80]	; 0x50
 8005dd6:	455b      	cmp	r3, fp
 8005dd8:	f47f adc7 	bne.w	800596a <mcumax_search+0x11a>
 8005ddc:	f517 5ffa 	cmn.w	r7, #8000	; 0x1f40
 8005de0:	d104      	bne.n	8005dec <mcumax_search+0x59c>
 8005de2:	9b10      	ldr	r3, [sp, #64]	; 0x40
 8005de4:	f5b3 5ffa 	cmp.w	r3, #8000	; 0x1f40
 8005de8:	bf18      	it	ne
 8005dea:	2700      	movne	r7, #0
 8005dec:	4633      	mov	r3, r6
 8005dee:	e553      	b.n	8005898 <mcumax_search+0x48>
 8005df0:	2662      	movs	r6, #98	; 0x62
 8005df2:	f44f 57fa 	mov.w	r7, #8000	; 0x1f40
 8005df6:	e7f9      	b.n	8005dec <mcumax_search+0x59c>
 8005df8:	463b      	mov	r3, r7
 8005dfa:	9a06      	ldr	r2, [sp, #24]
 8005dfc:	4297      	cmp	r7, r2
 8005dfe:	bfb8      	it	lt
 8005e00:	3301      	addlt	r3, #1
 8005e02:	9305      	str	r3, [sp, #20]
 8005e04:	e73c      	b.n	8005c80 <mcumax_search+0x430>
 8005e06:	f5b3 5ffa 	cmp.w	r3, #8000	; 0x1f40
 8005e0a:	ddf1      	ble.n	8005df0 <mcumax_search+0x5a0>
 8005e0c:	9806      	ldr	r0, [sp, #24]
 8005e0e:	f44f 57fa 	mov.w	r7, #8000	; 0x1f40
 8005e12:	2662      	movs	r6, #98	; 0x62
 8005e14:	e63c      	b.n	8005a90 <mcumax_search+0x240>
 8005e16:	42b8      	cmp	r0, r7
 8005e18:	f77f af78 	ble.w	8005d0c <mcumax_search+0x4bc>
 8005e1c:	e76b      	b.n	8005cf6 <mcumax_search+0x4a6>
 8005e1e:	bf00      	nop
 8005e20:	0800782f 	stmdaeq	r0, {r0, r1, r2, r3, r5, fp, ip, sp, lr}

08005e24 <mcumax_start_search>:
 8005e24:	b51f      	push	{r0, r1, r2, r3, r4, lr}
 8005e26:	4c10      	ldr	r4, [pc, #64]	; (8005e68 <mcumax_start_search+0x44>)
 8005e28:	f8ad 100c 	strh.w	r1, [sp, #12]
 8005e2c:	f8c4 3098 	str.w	r3, [r4, #152]	; 0x98
 8005e30:	f8c4 209c 	str.w	r2, [r4, #156]	; 0x9c
 8005e34:	2300      	movs	r3, #0
 8005e36:	2203      	movs	r2, #3
 8005e38:	f884 1090 	strb.w	r1, [r4, #144]	; 0x90
 8005e3c:	f3c1 2107 	ubfx	r1, r1, #8, #8
 8005e40:	f884 1091 	strb.w	r1, [r4, #145]	; 0x91
 8005e44:	f8c4 3094 	str.w	r3, [r4, #148]	; 0x94
 8005e48:	f884 30a0 	strb.w	r3, [r4, #160]	; 0xa0
 8005e4c:	9001      	str	r0, [sp, #4]
 8005e4e:	9200      	str	r2, [sp, #0]
 8005e50:	f894 3088 	ldrb.w	r3, [r4, #136]	; 0x88
 8005e54:	f8d4 2084 	ldr.w	r2, [r4, #132]	; 0x84
 8005e58:	f44f 51fa 	mov.w	r1, #8000	; 0x1f40
 8005e5c:	4803      	ldr	r0, [pc, #12]	; (8005e6c <mcumax_start_search+0x48>)
 8005e5e:	f7ff fcf7 	bl	8005850 <mcumax_search>
 8005e62:	b004      	add	sp, #16
 8005e64:	bd10      	pop	{r4, pc}
 8005e66:	bf00      	nop
 8005e68:	20000dac 	andcs	r0, r0, ip, lsr #27
 8005e6c:	ffffe0c0 			; <UNDEFINED> instruction: 0xffffe0c0

08005e70 <mcumax_init>:
 8005e70:	2200      	movs	r2, #0
 8005e72:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8005e76:	f04f 0c09 	mov.w	ip, #9
 8005e7a:	2712      	movs	r7, #18
 8005e7c:	4615      	mov	r5, r2
 8005e7e:	4b1c      	ldr	r3, [pc, #112]	; (8005ef0 <mcumax_init+0x80>)
 8005e80:	4e1c      	ldr	r6, [pc, #112]	; (8005ef4 <mcumax_init+0x84>)
 8005e82:	2400      	movs	r4, #0
 8005e84:	5791      	ldrsb	r1, [r2, r6]
 8005e86:	f041 0010 	orr.w	r0, r1, #16
 8005e8a:	f803 0f01 	strb.w	r0, [r3, #1]!
 8005e8e:	f041 0108 	orr.w	r1, r1, #8
 8005e92:	f883 1070 	strb.w	r1, [r3, #112]	; 0x70
 8005e96:	1f11      	subs	r1, r2, #4
 8005e98:	4349      	muls	r1, r1
 8005e9a:	741f      	strb	r7, [r3, #16]
 8005e9c:	f883 5020 	strb.w	r5, [r3, #32]
 8005ea0:	f883 5030 	strb.w	r5, [r3, #48]	; 0x30
 8005ea4:	f883 5040 	strb.w	r5, [r3, #64]	; 0x40
 8005ea8:	f883 5050 	strb.w	r5, [r3, #80]	; 0x50
 8005eac:	f883 c060 	strb.w	ip, [r3, #96]	; 0x60
 8005eb0:	f001 00ff 	and.w	r0, r1, #255	; 0xff
 8005eb4:	b2e1      	uxtb	r1, r4
 8005eb6:	f1a1 0803 	sub.w	r8, r1, #3
 8005eba:	3904      	subs	r1, #4
 8005ebc:	fb08 0101 	mla	r1, r8, r1, r0
 8005ec0:	eb03 1e04 	add.w	lr, r3, r4, lsl #4
 8005ec4:	3401      	adds	r4, #1
 8005ec6:	2c08      	cmp	r4, #8
 8005ec8:	f88e 1008 	strb.w	r1, [lr, #8]
 8005ecc:	d1f2      	bne.n	8005eb4 <mcumax_init+0x44>
 8005ece:	3201      	adds	r2, #1
 8005ed0:	2a08      	cmp	r2, #8
 8005ed2:	d1d6      	bne.n	8005e82 <mcumax_init+0x12>
 8005ed4:	4b08      	ldr	r3, [pc, #32]	; (8005ef8 <mcumax_init+0x88>)
 8005ed6:	2180      	movs	r1, #128	; 0x80
 8005ed8:	f883 2081 	strb.w	r2, [r3, #129]	; 0x81
 8005edc:	2200      	movs	r2, #0
 8005ede:	f883 1088 	strb.w	r1, [r3, #136]	; 0x88
 8005ee2:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
 8005ee6:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
 8005eea:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8005eee:	bf00      	nop
 8005ef0:	20000dab 	andcs	r0, r0, fp, lsr #27
 8005ef4:	0800781f 	stmdaeq	r0, {r0, r1, r2, r3, r4, fp, ip, sp, lr}
 8005ef8:	20000dac 	andcs	r0, r0, ip, lsr #27

08005efc <mcumax_get_piece>:
 8005efc:	f010 0f88 	tst.w	r0, #136	; 0x88
 8005f00:	bf01      	itttt	eq
 8005f02:	4b04      	ldreq	r3, [pc, #16]	; (8005f14 <mcumax_get_piece+0x18>)
 8005f04:	5c18      	ldrbeq	r0, [r3, r0]
 8005f06:	f000 000f 	andeq.w	r0, r0, #15
 8005f0a:	f080 0008 	eoreq.w	r0, r0, #8
 8005f0e:	bf18      	it	ne
 8005f10:	2000      	movne	r0, #0
 8005f12:	4770      	bx	lr
 8005f14:	20000dac 	andcs	r0, r0, ip, lsr #27

08005f18 <mcumax_search_valid_moves>:
 8005f18:	f248 0280 	movw	r2, #32896	; 0x8080
 8005f1c:	2300      	movs	r3, #0
 8005f1e:	b513      	push	{r0, r1, r4, lr}
 8005f20:	4c07      	ldr	r4, [pc, #28]	; (8005f40 <mcumax_search_valid_moves+0x28>)
 8005f22:	f8ad 2004 	strh.w	r2, [sp, #4]
 8005f26:	e9c4 012b 	strd	r0, r1, [r4, #172]	; 0xac
 8005f2a:	461a      	mov	r2, r3
 8005f2c:	9901      	ldr	r1, [sp, #4]
 8005f2e:	2001      	movs	r0, #1
 8005f30:	f8c4 30b4 	str.w	r3, [r4, #180]	; 0xb4
 8005f34:	f7ff ff76 	bl	8005e24 <mcumax_start_search>
 8005f38:	f8d4 00b4 	ldr.w	r0, [r4, #180]	; 0xb4
 8005f3c:	b002      	add	sp, #8
 8005f3e:	bd10      	pop	{r4, pc}
 8005f40:	20000dac 	andcs	r0, r0, ip, lsr #27

08005f44 <mcumax_search_best_move>:
 8005f44:	b513      	push	{r0, r1, r4, lr}
 8005f46:	f248 0480 	movw	r4, #32896	; 0x8080
 8005f4a:	f8ad 4000 	strh.w	r4, [sp]
 8005f4e:	4603      	mov	r3, r0
 8005f50:	1cca      	adds	r2, r1, #3
 8005f52:	2002      	movs	r0, #2
 8005f54:	9900      	ldr	r1, [sp, #0]
 8005f56:	f7ff ff65 	bl	8005e24 <mcumax_start_search>
 8005f5a:	f5b0 5ffa 	cmp.w	r0, #8000	; 0x1f40
 8005f5e:	f04f 0000 	mov.w	r0, #0
 8005f62:	bf09      	itett	eq
 8005f64:	4b0a      	ldreq	r3, [pc, #40]	; (8005f90 <mcumax_search_best_move+0x4c>)
 8005f66:	f8ad 4004 	strhne.w	r4, [sp, #4]
 8005f6a:	f893 2091 	ldrbeq.w	r2, [r3, #145]	; 0x91
 8005f6e:	f893 3090 	ldrbeq.w	r3, [r3, #144]	; 0x90
 8005f72:	bf04      	itt	eq
 8005f74:	f88d 2005 	strbeq.w	r2, [sp, #5]
 8005f78:	f88d 3004 	strbeq.w	r3, [sp, #4]
 8005f7c:	f89d 3004 	ldrb.w	r3, [sp, #4]
 8005f80:	f363 0007 	bfi	r0, r3, #0, #8
 8005f84:	f89d 3005 	ldrb.w	r3, [sp, #5]
 8005f88:	f363 200f 	bfi	r0, r3, #8, #8
 8005f8c:	b002      	add	sp, #8
 8005f8e:	bd10      	pop	{r4, pc}
 8005f90:	20000dac 	andcs	r0, r0, ip, lsr #27

08005f94 <mcumax_play_move>:
 8005f94:	2300      	movs	r3, #0
 8005f96:	b507      	push	{r0, r1, r2, lr}
 8005f98:	f8ad 0004 	strh.w	r0, [sp, #4]
 8005f9c:	461a      	mov	r2, r3
 8005f9e:	9901      	ldr	r1, [sp, #4]
 8005fa0:	2003      	movs	r0, #3
 8005fa2:	f7ff ff3f 	bl	8005e24 <mcumax_start_search>
 8005fa6:	f5a0 53fa 	sub.w	r3, r0, #8000	; 0x1f40
 8005faa:	4258      	negs	r0, r3
 8005fac:	4158      	adcs	r0, r3
 8005fae:	b003      	add	sp, #12
 8005fb0:	f85d fb04 	ldr.w	pc, [sp], #4

08005fb4 <mcumax_set_callback>:
 8005fb4:	4b01      	ldr	r3, [pc, #4]	; (8005fbc <mcumax_set_callback+0x8>)
 8005fb6:	e9c3 0129 	strd	r0, r1, [r3, #164]	; 0xa4
 8005fba:	4770      	bx	lr
 8005fbc:	20000dac 	andcs	r0, r0, ip, lsr #27

08005fc0 <mcumax_stop_search>:
 8005fc0:	2201      	movs	r2, #1
 8005fc2:	4b02      	ldr	r3, [pc, #8]	; (8005fcc <mcumax_stop_search+0xc>)
 8005fc4:	f883 20a0 	strb.w	r2, [r3, #160]	; 0xa0
 8005fc8:	4770      	bx	lr
 8005fca:	bf00      	nop
 8005fcc:	20000dac 	andcs	r0, r0, ip, lsr #27

08005fd0 <__libc_init_array>:
 8005fd0:	b570      	push	{r4, r5, r6, lr}
 8005fd2:	2500      	movs	r5, #0
 8005fd4:	4e0c      	ldr	r6, [pc, #48]	; (8006008 <__libc_init_array+0x38>)
 8005fd6:	4c0d      	ldr	r4, [pc, #52]	; (800600c <__libc_init_array+0x3c>)
 8005fd8:	1ba4      	subs	r4, r4, r6
 8005fda:	10a4      	asrs	r4, r4, #2
 8005fdc:	42a5      	cmp	r5, r4
 8005fde:	d109      	bne.n	8005ff4 <__libc_init_array+0x24>
 8005fe0:	f000 f844 	bl	800606c <_init>
 8005fe4:	2500      	movs	r5, #0
 8005fe6:	4e0a      	ldr	r6, [pc, #40]	; (8006010 <__libc_init_array+0x40>)
 8005fe8:	4c0a      	ldr	r4, [pc, #40]	; (8006014 <__libc_init_array+0x44>)
 8005fea:	1ba4      	subs	r4, r4, r6
 8005fec:	10a4      	asrs	r4, r4, #2
 8005fee:	42a5      	cmp	r5, r4
 8005ff0:	d105      	bne.n	8005ffe <__libc_init_array+0x2e>
 8005ff2:	bd70      	pop	{r4, r5, r6, pc}
 8005ff4:	f856 3025 	ldr.w	r3, [r6, r5, lsl #2]
 8005ff8:	4798      	blx	r3
 8005ffa:	3501      	adds	r5, #1
 8005ffc:	e7ee      	b.n	8005fdc <__libc_init_array+0xc>
 8005ffe:	f856 3025 	ldr.w	r3, [r6, r5, lsl #2]
 8006002:	4798      	blx	r3
 8006004:	3501      	adds	r5, #1
 8006006:	e7f2      	b.n	8005fee <__libc_init_array+0x1e>
 8006008:	08007848 	stmdaeq	r0, {r3, r6, fp, ip, sp, lr}
 800600c:	08007848 	stmdaeq	r0, {r3, r6, fp, ip, sp, lr}
 8006010:	08007848 	stmdaeq	r0, {r3, r6, fp, ip, sp, lr}
 8006014:	0800784c 	stmdaeq	r0, {r2, r3, r6, fp, ip, sp, lr}

08006018 <memcpy>:
 8006018:	b510      	push	{r4, lr}
 800601a:	1e43      	subs	r3, r0, #1
 800601c:	440a      	add	r2, r1
 800601e:	4291      	cmp	r1, r2
 8006020:	d100      	bne.n	8006024 <memcpy+0xc>
 8006022:	bd10      	pop	{r4, pc}
 8006024:	f811 4b01 	ldrb.w	r4, [r1], #1
 8006028:	f803 4f01 	strb.w	r4, [r3, #1]!
 800602c:	e7f7      	b.n	800601e <memcpy+0x6>

0800602e <memset>:
 800602e:	4603      	mov	r3, r0
 8006030:	4402      	add	r2, r0
 8006032:	4293      	cmp	r3, r2
 8006034:	d100      	bne.n	8006038 <memset+0xa>
 8006036:	4770      	bx	lr
 8006038:	f803 1b01 	strb.w	r1, [r3], #1
 800603c:	e7f9      	b.n	8006032 <memset+0x4>

0800603e <strcat>:
 800603e:	4603      	mov	r3, r0
 8006040:	b510      	push	{r4, lr}
 8006042:	781a      	ldrb	r2, [r3, #0]
 8006044:	1c5c      	adds	r4, r3, #1
 8006046:	b93a      	cbnz	r2, 8006058 <strcat+0x1a>
 8006048:	3b01      	subs	r3, #1
 800604a:	f811 2b01 	ldrb.w	r2, [r1], #1
 800604e:	f803 2f01 	strb.w	r2, [r3, #1]!
 8006052:	2a00      	cmp	r2, #0
 8006054:	d1f9      	bne.n	800604a <strcat+0xc>
 8006056:	bd10      	pop	{r4, pc}
 8006058:	4623      	mov	r3, r4
 800605a:	e7f2      	b.n	8006042 <strcat+0x4>

0800605c <strcpy>:
 800605c:	4603      	mov	r3, r0
 800605e:	f811 2b01 	ldrb.w	r2, [r1], #1
 8006062:	f803 2b01 	strb.w	r2, [r3], #1
 8006066:	2a00      	cmp	r2, #0
 8006068:	d1f9      	bne.n	800605e <strcpy+0x2>
 800606a:	4770      	bx	lr

0800606c <_init>:
 800606c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 800606e:	bf00      	nop
 8006070:	bcf8      	pop	{r3, r4, r5, r6, r7}
 8006072:	bc08      	pop	{r3}
 8006074:	469e      	mov	lr, r3
 8006076:	4770      	bx	lr

08006078 <_fini>:
 8006078:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 800607a:	bf00      	nop
 800607c:	bcf8      	pop	{r3, r4, r5, r6, r7}
 800607e:	bc08      	pop	{r3}
 8006080:	469e      	mov	lr, r3
 8006082:	4770      	bx	lr

Disassembly of section .rodata:

08006084 <pulseClicksMenu-0xd4>:
 8006084:	00080000 	andeq	r0, r8, r0
 8006088:	000d0080 	andeq	r0, sp, r0, lsl #1
 800608c:	ffff000c 			; <UNDEFINED> instruction: 0xffff000c
 8006090:	00000000 	andeq	r0, r0, r0
 8006094:	00080040 	andeq	r0, r8, r0, asr #32
 8006098:	00000040 	andeq	r0, r0, r0, asr #32
 800609c:	00080040 	andeq	r0, r8, r0, asr #32
 80060a0:	00000000 	andeq	r0, r0, r0
 80060a4:	00080080 	andeq	r0, r8, r0, lsl #1
 80060a8:	00080000 	andeq	r0, r8, r0
 80060ac:	00180080 	andseq	r0, r8, r0, lsl #1
 80060b0:	000a0040 	andeq	r0, sl, r0, asr #32
 80060b4:	00200000 	eoreq	r0, r0, r0
 80060b8:	00200080 	eoreq	r0, r0, r0, lsl #1
 80060bc:	00040040 	andeq	r0, r4, r0, asr #32
 80060c0:	00000000 	andeq	r0, r0, r0
 80060c4:	00400080 	subeq	r0, r0, r0, lsl #1
 80060c8:	001c0040 	andseq	r0, ip, r0, asr #32
 80060cc:	00080000 	andeq	r0, r8, r0
 80060d0:	00080080 	andeq	r0, r8, r0, lsl #1
 80060d4:	ffff0003 			; <UNDEFINED> instruction: 0xffff0003
 80060d8:	ffff0001 			; <UNDEFINED> instruction: 0xffff0001
 80060dc:	00200000 	eoreq	r0, r0, r0
 80060e0:	00200080 	eoreq	r0, r0, r0, lsl #1
 80060e4:	00150000 	andseq	r0, r5, r0
 80060e8:	0017004c 	andseq	r0, r7, ip, asr #32
 80060ec:	ffff000b 			; <UNDEFINED> instruction: 0xffff000b
 80060f0:	0015004c 	andseq	r0, r5, ip, asr #32
 80060f4:	00070034 	andeq	r0, r7, r4, lsr r0
 80060f8:	001c004c 	andseq	r0, ip, ip, asr #32
 80060fc:	00100034 	andseq	r0, r0, r4, lsr r0
 8006100:	002c0000 	eoreq	r0, ip, r0
 8006104:	0008004c 	andeq	r0, r8, ip, asr #32
 8006108:	00340000 	eorseq	r0, r4, r0
 800610c:	000c004c 	andeq	r0, ip, ip, asr #32
 8006110:	002c004c 	eoreq	r0, ip, ip, asr #32
 8006114:	00080034 	andeq	r0, r8, r4, lsr r0
 8006118:	0034004c 	eorseq	r0, r4, ip, asr #32
 800611c:	000c0034 	andeq	r0, ip, r4, lsr r0
 8006120:	00150000 	andseq	r0, r5, r0
 8006124:	00160080 	andseq	r0, r6, r0, lsl #1
 8006128:	0007000c 	andeq	r0, r7, ip
 800612c:	002b0000 	eoreq	r0, fp, r0
 8006130:	00150080 	andseq	r0, r5, r0, lsl #1
 8006134:	00000048 	andeq	r0, r0, r8, asr #32
 8006138:	00140017 	andseq	r0, r4, r7, lsl r0
 800613c:	00040002 	andeq	r0, r4, r2
 8006140:	002c0048 	eoreq	r0, ip, r8, asr #32
 8006144:	00140038 	andseq	r0, r4, r8, lsr r0
 8006148:	00080002 	andeq	r0, r8, r2
 800614c:	0008005f 	andeq	r0, r8, pc, asr r0
 8006150:	000c0021 	andeq	r0, ip, r1, lsr #32
 8006154:	00008080 	andeq	r8, r0, r0, lsl #1

08006158 <pulseClicksMenu>:
 8006158:	08006193 	stmdaeq	r0, {r0, r1, r4, r7, r8, sp, lr}
 800615c:	2000008c 	andcs	r0, r0, ip, lsl #1
 8006160:	08000625 	stmdaeq	r0, {r0, r2, r5, r9, sl}
 8006164:	08000645 	stmdaeq	r0, {r0, r2, r6, r9, sl}
 8006168:	08004029 	stmdaeq	r0, {r0, r3, r5, lr}

0800616c <pulseClicksMenuOptions>:
 800616c:	08006184 	stmdaeq	r0, {r2, r7, r8, sp, lr}
 8006170:	08006188 	stmdaeq	r0, {r3, r7, r8, sp, lr}
 8006174:	0800618e 	stmdaeq	r0, {r1, r2, r3, r7, r8, sp, lr}
 8006178:	00000000 	andeq	r0, r0, r0

0800617c <pulseClicksMenuView>:
 800617c:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8006180:	08006158 	stmdaeq	r0, {r3, r4, r6, r8, sp, lr}
 8006184:	0066664f 	rsbeq	r6, r6, pc, asr #12
 8006188:	65697551 	strbvs	r7, [r9, #-1361]!	; 0xfffffaaf
 800618c:	6f4c0074 	svcvs	0x004c0074
 8006190:	50006475 	andpl	r6, r0, r5, ror r4
 8006194:	65736c75 	ldrbvs	r6, [r3, #-3189]!	; 0xfffff38b
 8006198:	696c6320 	stmdbvs	ip!, {r5, r8, r9, sp, lr}^
 800619c:	00736b63 	rsbseq	r6, r3, r3, ror #22
 80061a0:	20544547 	subscs	r4, r4, r7, asr #10
 80061a4:	69766564 	ldmdbvs	r6!, {r2, r5, r6, r8, sl, sp, lr}^
 80061a8:	64496563 	strbvs	r6, [r9], #-1379	; 0xfffffa9d
 80061ac:	47003b00 	strmi	r3, [r0, -r0, lsl #22]
 80061b0:	64205445 	strtvs	r5, [r0], #-1093	; 0xfffffbbb
 80061b4:	63697665 	cmnvs	r9, #105906176	; 0x6500000
 80061b8:	74614265 	strbtvc	r4, [r1], #-613	; 0xfffffd9b
 80061bc:	79726574 	ldmdbvc	r2!, {r2, r4, r5, r6, r8, sl, sp, lr}^
 80061c0:	746c6f56 	strbtvc	r6, [ip], #-3926	; 0xfffff0aa
 80061c4:	00656761 	rsbeq	r6, r5, r1, ror #14
 80061c8:	20544547 	subscs	r4, r4, r7, asr #10
 80061cc:	69766564 	ldmdbvs	r6!, {r2, r5, r6, r8, sl, sp, lr}^
 80061d0:	65546563 	ldrbvs	r6, [r4, #-1379]	; 0xfffffa9d
 80061d4:	7265706d 	rsbvc	r7, r5, #109	; 0x6d
 80061d8:	72757461 	rsbsvc	r7, r5, #1627389952	; 0x61000000
 80061dc:	45470065 	strbmi	r0, [r7, #-101]	; 0xffffff9b
 80061e0:	65642054 	strbvs	r2, [r4, #-84]!	; 0xffffffac
 80061e4:	65636976 	strbvs	r6, [r3, #-2422]!	; 0xfffff68a
 80061e8:	656d6954 	strbvs	r6, [sp, #-2388]!	; 0xfffff6ac
 80061ec:	54455300 	strbpl	r5, [r5], #-768	; 0xfffffd00
 80061f0:	76656420 	strbtvc	r6, [r5], -r0, lsr #8
 80061f4:	54656369 	strbtpl	r6, [r5], #-873	; 0xfffffc97
 80061f8:	00656d69 	rsbeq	r6, r5, r9, ror #26
 80061fc:	20544547 	subscs	r4, r4, r7, asr #10
 8006200:	65627574 	strbvs	r7, [r2, #-1396]!	; 0xfffffa8c
 8006204:	656d6954 	strbvs	r6, [sp, #-2388]!	; 0xfffff6ac
 8006208:	54455300 	strbpl	r5, [r5], #-768	; 0xfffffd00
 800620c:	62757420 	rsbsvs	r7, r5, #32, 8	; 0x20000000
 8006210:	6d695465 	cfstrdvs	mvd5, [r9, #-404]!	; 0xfffffe6c
 8006214:	45470065 	strbmi	r0, [r7, #-101]	; 0xffffff9b
 8006218:	75742054 	ldrbvc	r2, [r4, #-84]!	; 0xffffffac
 800621c:	75506562 	ldrbvc	r6, [r0, #-1378]	; 0xfffffa9e
 8006220:	4365736c 	cmnmi	r5, #108, 6	; 0xb0000001
 8006224:	746e756f 	strbtvc	r7, [lr], #-1391	; 0xfffffa91
 8006228:	54455300 	strbpl	r5, [r5], #-768	; 0xfffffd00
 800622c:	62757420 	rsbsvs	r7, r5, #32, 8	; 0x20000000
 8006230:	6c755065 	ldclvs	0, cr5, [r5], #-404	; 0xfffffe6c
 8006234:	6f436573 	svcvs	0x00436573
 8006238:	00746e75 	rsbseq	r6, r4, r5, ror lr
 800623c:	20544547 	subscs	r4, r4, r7, asr #10
 8006240:	65627574 	strbvs	r7, [r2, #-1396]!	; 0xfffffa8c
 8006244:	65746152 	ldrbvs	r6, [r4, #-338]!	; 0xfffffeae
 8006248:	54454700 	strbpl	r4, [r5], #-1792	; 0xfffff900
 800624c:	62757420 	rsbsvs	r7, r5, #32, 8	; 0x20000000
 8006250:	61654465 	cmnvs	r5, r5, ror #8
 8006254:	6d695464 	cfstrdvs	mvd5, [r9, #-400]!	; 0xfffffe70
 8006258:	45470065 	strbmi	r0, [r7, #-101]	; 0xffffff9b
 800625c:	75742054 	ldrbvc	r2, [r4, #-84]!	; 0xffffffac
 8006260:	6f436562 	svcvs	0x00436562
 8006264:	7265766e 	rsbvc	r7, r5, #115343360	; 0x6e00000
 8006268:	6e6f6973 			; <UNDEFINED> instruction: 0x6e6f6973
 800626c:	74636146 	strbtvc	r6, [r3], #-326	; 0xfffffeba
 8006270:	4700726f 	strmi	r7, [r0, -pc, ror #4]
 8006274:	74205445 	strtvc	r5, [r0], #-1093	; 0xfffffbbb
 8006278:	44656275 	strbtmi	r6, [r5], #-629	; 0xfffffd8b
 800627c:	54646165 	strbtpl	r6, [r4], #-357	; 0xfffffe9b
 8006280:	43656d69 	cmnmi	r5, #6720	; 0x1a40
 8006284:	65706d6f 	ldrbvs	r6, [r0, #-3439]!	; 0xfffff291
 8006288:	7461736e 	strbtvc	r7, [r1], #-878	; 0xfffffc92
 800628c:	006e6f69 	rsbeq	r6, lr, r9, ror #30
 8006290:	20544547 	subscs	r4, r4, r7, asr #10
 8006294:	65627574 	strbvs	r7, [r2, #-1396]!	; 0xfffffa8c
 8006298:	72465648 	subvc	r5, r6, #72, 12	; 0x4800000
 800629c:	65757165 	ldrbvs	r7, [r5, #-357]!	; 0xfffffe9b
 80062a0:	0079636e 	rsbseq	r6, r9, lr, ror #6
 80062a4:	20544547 	subscs	r4, r4, r7, asr #10
 80062a8:	65627574 	strbvs	r7, [r2, #-1396]!	; 0xfffffa8c
 80062ac:	75445648 	strbvc	r5, [r4, #-1608]	; 0xfffff9b8
 80062b0:	79437974 	stmdbvc	r3, {r2, r4, r5, r6, r8, fp, ip, sp, lr}^
 80062b4:	00656c63 	rsbeq	r6, r5, r3, ror #24
 80062b8:	20544547 	subscs	r4, r4, r7, asr #10
 80062bc:	61746164 	cmnvs	r4, r4, ror #2
 80062c0:	00676f6c 	rsbeq	r6, r7, ip, ror #30
 80062c4:	20544547 	subscs	r4, r4, r7, asr #10
 80062c8:	646e6172 	strbtvs	r6, [lr], #-370	; 0xfffffe8e
 80062cc:	61446d6f 	cmpvs	r4, pc, ror #26
 80062d0:	45006174 	strmi	r6, [r0, #-372]	; 0xfffffe8c
 80062d4:	524f5252 	subpl	r5, pc, #536870917	; 0x20000005
 80062d8:	2c000a00 			; <UNDEFINED> instruction: 0x2c000a00
 80062dc:	004b4f00 	subeq	r4, fp, r0, lsl #30
 80062e0:	6d697420 	cfstrdvs	mvd7, [r9, #-128]!	; 0xffffff80
 80062e4:	75742c65 	ldrbvc	r2, [r4, #-3173]!	; 0xfffff39b
 80062e8:	75506562 	ldrbvc	r6, [r0, #-1378]	; 0xfffffa9e
 80062ec:	4365736c 	cmnmi	r5, #108, 6	; 0xb0000001
 80062f0:	746e756f 	strbtvc	r7, [lr], #-1391	; 0xfffffa91
 80062f4:	00000000 	andeq	r0, r0, r0

080062f8 <metricPowers>:
 80062f8:	358637bd 	strcc	r3, [r6, #1981]	; 0x7bd
 80062fc:	3a8310c1 	bcc	60ca608 <_Min_Stack_Size+0x60ca208>
 8006300:	000000b5 	strheq	r0, [r0], -r5
 8006304:	3a83126f 	bcc	60cacc8 <_Min_Stack_Size+0x60ca8c8>
 8006308:	3f7ffcb9 	svccc	0x007ffcb9
 800630c:	0000006d 	andeq	r0, r0, sp, rrx
 8006310:	3f800000 	svccc	0x00800000
 8006314:	4479fccd 	ldrbtmi	pc, [r9], #-3277	; 0xfffff333	; <UNPREDICTABLE>
 8006318:	00000000 	andeq	r0, r0, r0
 800631c:	447a0000 	ldrbtmi	r0, [sl], #-0
 8006320:	497420e0 	ldmdbmi	r4!, {r5, r6, r7, sp}^
 8006324:	0000006b 	andeq	r0, r0, fp, rrx
 8006328:	49742400 	ldmdbmi	r4!, {sl, sp}^
 800632c:	4e6e681b 	mcrmi	8, 3, r6, cr14, cr11, {0}
 8006330:	0000004d 	andeq	r0, r0, sp, asr #32
 8006334:	4e6e6b28 	vmulmi.f64	d22, d14, d24
 8006338:	5368d1aa 	cmnpl	r8, #-2147483606	; 0x8000002a
 800633c:	00000047 	andeq	r0, r0, r7, asr #32
 8006340:	7f7fffff 	svcvc	0x007fffff
 8006344:	7f7fffff 	svcvc	0x007fffff
 8006348:	00000000 	andeq	r0, r0, r0
 800634c:	3a002e30 	bcc	8011c14 <_sidata+0xa3c4>
 8006350:	00000000 	andeq	r0, r0, r0

08006354 <datalogMenu>:
 8006354:	08006398 	stmdaeq	r0, {r3, r4, r7, r8, r9, sp, lr}
 8006358:	200000d8 	ldrdcs	r0, [r0], -r8
 800635c:	08000dc9 	stmdaeq	r0, {r0, r3, r6, r7, r8, sl, fp}
 8006360:	08000f59 	stmdaeq	r0, {r0, r3, r4, r6, r8, r9, sl, fp}
 8006364:	08004029 	stmdaeq	r0, {r0, r3, r5, lr}

08006368 <datalogMenuOptions>:
 8006368:	08006184 	stmdaeq	r0, {r2, r7, r8, sp, lr}
 800636c:	080063a5 	stmdaeq	r0, {r0, r2, r5, r7, r8, r9, sp, lr}
 8006370:	080063b6 	stmdaeq	r0, {r1, r2, r4, r5, r7, r8, r9, sp, lr}
 8006374:	080063c7 	stmdaeq	r0, {r0, r1, r2, r6, r7, r8, r9, sp, lr}
 8006378:	080063d8 	stmdaeq	r0, {r3, r4, r6, r7, r8, r9, sp, lr}
 800637c:	080063e8 	stmdaeq	r0, {r3, r5, r6, r7, r8, r9, sp, lr}
 8006380:	00000000 	andeq	r0, r0, r0

08006384 <datalogMenuView>:
 8006384:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8006388:	08006354 	stmdaeq	r0, {r2, r4, r6, r8, r9, sp, lr}

0800638c <datalogTimeIntervals>:
 800638c:	0e100000 	cdpeq	0, 1, cr0, cr0, cr0, {0}
 8006390:	02580708 	subseq	r0, r8, #8, 14	; 0x200000
 8006394:	003c012c 	eorseq	r0, ip, ip, lsr #2
 8006398:	61746144 	cmnvs	r4, r4, asr #2
 800639c:	676f6c20 	strbvs	r6, [pc, -r0, lsr #24]!
 80063a0:	676e6967 	strbvs	r6, [lr, -r7, ror #18]!
 80063a4:	65764500 	ldrbvs	r4, [r6, #-1280]!	; 0xfffffb00
 80063a8:	36207972 			; <UNDEFINED> instruction: 0x36207972
 80063ac:	696d2030 	stmdbvs	sp!, {r4, r5, sp}^
 80063b0:	6574756e 	ldrbvs	r7, [r4, #-1390]!	; 0xfffffa92
 80063b4:	76450073 			; <UNDEFINED> instruction: 0x76450073
 80063b8:	20797265 	rsbscs	r7, r9, r5, ror #4
 80063bc:	6d203033 	stcvs	0, cr3, [r0, #-204]!	; 0xffffff34
 80063c0:	74756e69 	ldrbtvc	r6, [r5], #-3689	; 0xfffff197
 80063c4:	45007365 	strmi	r7, [r0, #-869]	; 0xfffffc9b
 80063c8:	79726576 	ldmdbvc	r2!, {r1, r2, r4, r5, r6, r8, sl, sp, lr}^
 80063cc:	20303120 	eorscs	r3, r0, r0, lsr #2
 80063d0:	756e696d 	strbvc	r6, [lr, #-2413]!	; 0xfffff693
 80063d4:	00736574 	rsbseq	r6, r3, r4, ror r5
 80063d8:	72657645 	rsbvc	r7, r5, #72351744	; 0x4500000
 80063dc:	20352079 	eorscs	r2, r5, r9, ror r0
 80063e0:	756e696d 	strbvc	r6, [lr, #-2413]!	; 0xfffff693
 80063e4:	00736574 	rsbseq	r6, r3, r4, ror r5
 80063e8:	72657645 	rsbvc	r7, r5, #72351744	; 0x4500000
 80063ec:	696d2079 	stmdbvs	sp!, {r0, r3, r4, r5, r6, sp}^
 80063f0:	6574756e 	ldrbvs	r7, [r4, #-1390]!	; 0xfffffa92
 80063f4:	00000000 	andeq	r0, r0, r0

080063f8 <displayBrightnessMenu>:
 80063f8:	08006ee6 	stmdaeq	r0, {r1, r2, r5, r6, r7, r9, sl, fp, sp, lr}
 80063fc:	200000dc 	ldrdcs	r0, [r0], -ip
 8006400:	08001159 	stmdaeq	r0, {r0, r3, r4, r6, r8, ip}
 8006404:	080011b9 	stmdaeq	r0, {r0, r3, r4, r5, r7, r8, ip}
 8006408:	08001199 	stmdaeq	r0, {r0, r3, r4, r7, r8, ip}

0800640c <displayBrightnessMenuOptions>:
 800640c:	08006ebb 	stmdaeq	r0, {r0, r1, r3, r4, r5, r7, r9, sl, fp, sp, lr}
 8006410:	08006ebf 	stmdaeq	r0, {r0, r1, r2, r3, r4, r5, r7, r9, sl, fp, sp, lr}
 8006414:	08006ec6 	stmdaeq	r0, {r1, r2, r6, r7, r9, sl, fp, sp, lr}
 8006418:	08006ecb 	stmdaeq	r0, {r0, r1, r3, r6, r7, r9, sl, fp, sp, lr}
 800641c:	00000000 	andeq	r0, r0, r0

08006420 <displayBrightnessMenuView>:
 8006420:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8006424:	080063f8 	stmdaeq	r0, {r3, r4, r5, r6, r7, r8, r9, sp, lr}

08006428 <displayContrastMenu>:
 8006428:	08006edd 	stmdaeq	r0, {r0, r2, r3, r4, r6, r7, r9, sl, fp, sp, lr}
 800642c:	200000e0 	andcs	r0, r0, r0, ror #1
 8006430:	08001201 	stmdaeq	r0, {r0, r9, ip}
 8006434:	080011e9 	stmdaeq	r0, {r0, r3, r5, r6, r7, r8, ip}
 8006438:	08001199 	stmdaeq	r0, {r0, r3, r4, r7, r8, ip}

0800643c <displayContrastMenuView>:
 800643c:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8006440:	08006428 	stmdaeq	r0, {r3, r5, sl, sp, lr}

08006444 <displayMenu>:
 8006444:	08006ed5 	stmdaeq	r0, {r0, r2, r4, r6, r7, r9, sl, fp, sp, lr}
 8006448:	200000e4 	andcs	r0, r0, r4, ror #1
 800644c:	08001149 	stmdaeq	r0, {r0, r3, r6, r8, ip}
 8006450:	080011a5 	stmdaeq	r0, {r0, r2, r5, r7, r8, ip}
 8006454:	08004029 	stmdaeq	r0, {r0, r3, r5, lr}

08006458 <displayMenuOptions>:
 8006458:	08006edd 	stmdaeq	r0, {r0, r2, r3, r4, r6, r7, r9, sl, fp, sp, lr}
 800645c:	0800643c 	stmdaeq	r0, {r2, r3, r4, r5, sl, sp, lr}
 8006460:	08006ee6 	stmdaeq	r0, {r1, r2, r5, r6, r7, r9, sl, fp, sp, lr}
 8006464:	08006420 	stmdaeq	r0, {r5, sl, sp, lr}
 8006468:	08006ef7 	stmdaeq	r0, {r0, r1, r2, r4, r5, r6, r7, r9, sl, fp, sp, lr}
 800646c:	080064b4 	stmdaeq	r0, {r2, r4, r5, r7, sl, sp, lr}
	...

08006478 <displayMenuView>:
 8006478:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 800647c:	08006444 	stmdaeq	r0, {r2, r6, sl, sp, lr}

08006480 <displaySleepMenu>:
 8006480:	08006ef7 	stmdaeq	r0, {r0, r1, r2, r4, r5, r6, r7, r9, sl, fp, sp, lr}
 8006484:	200000e8 	andcs	r0, r0, r8, ror #1
 8006488:	08001179 	stmdaeq	r0, {r0, r3, r4, r5, r6, r8, ip}
 800648c:	080011d1 	stmdaeq	r0, {r0, r4, r6, r7, r8, ip}
 8006490:	08001199 	stmdaeq	r0, {r0, r3, r4, r7, r8, ip}

08006494 <displaySleepMenuOptions>:
 8006494:	08006184 	stmdaeq	r0, {r2, r7, r8, sp, lr}
 8006498:	08006e5f 	stmdaeq	r0, {r0, r1, r2, r3, r4, r6, r9, sl, fp, sp, lr}
 800649c:	08006e71 	stmdaeq	r0, {r0, r4, r5, r6, r9, sl, fp, sp, lr}
 80064a0:	08006e81 	stmdaeq	r0, {r0, r7, r9, sl, fp, sp, lr}
 80064a4:	08006e92 	stmdaeq	r0, {r1, r4, r7, r9, sl, fp, sp, lr}
 80064a8:	08006ea3 	stmdaeq	r0, {r0, r1, r5, r7, r9, sl, fp, sp, lr}
 80064ac:	08006eb1 	stmdaeq	r0, {r0, r4, r5, r7, r9, sl, fp, sp, lr}
 80064b0:	00000000 	andeq	r0, r0, r0

080064b4 <displaySleepMenuView>:
 80064b4:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 80064b8:	08006480 	stmdaeq	r0, {r7, sl, sp, lr}
 80064bc:	43414140 	movtmi	r4, #4416	; 0x1140
 80064c0:	45444246 	strbmi	r4, [r4, #-582]	; 0xfffffdba
 80064c4:	8000b100 	andhi	fp, r0, r0, lsl #2
 80064c8:	53008100 	movwpl	r8, #256	; 0x100
 80064cc:	69746174 	ldmdbvs	r4!, {r2, r4, r5, r6, r8, sp, lr}^
 80064d0:	63697473 	cmnvs	r9, #1929379840	; 0x73000000
 80064d4:	75540073 	ldrbvc	r0, [r4, #-115]	; 0xffffff8d
 80064d8:	6c206562 	cfstr32vs	mvfx6, [r0], #-392	; 0xfffffe78
 80064dc:	20656669 	rsbcs	r6, r5, r9, ror #12
 80064e0:	656d6974 	strbvs	r6, [sp, #-2420]!	; 0xfffff68c
 80064e4:	62755400 	rsbsvs	r5, r5, #0, 8
 80064e8:	696c2065 	stmdbvs	ip!, {r0, r2, r5, r6, sp}^
 80064ec:	70206566 	eorvc	r6, r0, r6, ror #10
 80064f0:	65736c75 	ldrbvs	r6, [r3, #-3189]!	; 0xfffff38b
 80064f4:	75540073 	ldrbvc	r0, [r4, #-115]	; 0xffffff8d
 80064f8:	64206562 	strtvs	r6, [r0], #-1378	; 0xfffffa9e
 80064fc:	2d646165 	stfcse	f6, [r4, #-404]!	; 0xfffffe6c
 8006500:	656d6974 	strbvs	r6, [sp, #-2420]!	; 0xfffff68c
 8006504:	00203c00 	eoreq	r3, r0, r0, lsl #24
 8006508:	0073b520 	rsbseq	fp, r3, r0, lsr #10
 800650c:	69766544 	ldmdbvs	r6!, {r2, r6, r8, sl, sp, lr}^
 8006510:	49206563 	stmdbmi	r0!, {r0, r1, r5, r6, r8, sl, sp, lr}
 8006514:	65440044 	strbvs	r0, [r4, #-68]	; 0xffffffbc
 8006518:	65636976 	strbvs	r6, [r3, #-2422]!	; 0xfffff68a
 800651c:	6d657420 	cfstrdvs	mvd7, [r5, #-128]!	; 0xffffff80
 8006520:	20002e70 	andcs	r2, r0, r0, ror lr
 8006524:	 			; <UNDEFINED> instruction: 0x000043b0

08006527 <font_chess8>:
 8006527:	08000800 	stmdaeq	r0, {fp}
 800652b:	00000000 	andeq	r0, r0, r0
 800652f:	09000000 	stmdbeq	r0, {}	; <UNPREDICTABLE>
 8006533:	02030800 	andeq	r0, r3, #0, 16
 8006537:	01040404 	tsteq	r4, r4, lsl #8
 800653b:	40040001 	andmi	r0, r4, r1
 800653f:	00200003 	eoreq	r0, r0, r3
 8006543:	07411d82 	strbeq	r1, [r1, -r2, lsl #27]
 8006547:	74752aaa 	ldrbtvc	r2, [r5], #-2730	; 0xfffff556
 800654b:	093e9d96 	ldmdbeq	lr!, {r1, r2, r4, r7, r8, sl, fp, ip, pc}
 800654f:	4b292caa 	blmi	8a517ff <_sidata+0xa49faf>
 8006553:	a68b3a01 	strge	r3, [fp], r1, lsl #20
 8006557:	2ec90b01 	vdivcs.f64	d16, d9, d1
 800655b:	d347452d 	movtle	r4, #29997	; 0x752d
 800655f:	f470253f 			; <UNDEFINED> instruction: 0xf470253f
 8006563:	2caa0801 	stccs	8, cr0, [sl], #4
 8006567:	1c09fa53 			; <UNDEFINED> instruction: 0x1c09fa53
 800656b:	e90a3e9d 	stmdb	sl, {r0, r2, r3, r4, r7, r9, sl, fp, ip, sp}
 800656f:	5f062d2e 	svcpl	0x00062d2e
 8006573:	b3b3e8fa 			; <UNDEFINED> instruction: 0xb3b3e8fa
 8006577:	2eaa094f 	vfmscs.f16	s0, s20, s30	; <UNPREDICTABLE>
 800657b:	fe8b3b29 	vmaxnm.f64	d3, d11, d25
 800657f:	0307d3a7 	movweq	sp, #29607	; 0x73a7
 8006583:	03002000 	movweq	r2, #0
 8006587:	0b002000 	bleq	800e58f <_sidata+0x6d3f>
 800658b:	80e92ee1 	rschi	r2, r9, r1, ror #29
 800658f:	2cb27845 	ldccs	8, cr7, [r2], #276	; 0x114
 8006593:	0e27d2e5 	cdpeq	2, 2, cr13, cr7, cr5, {7}
 8006597:	972d30e1 	strls	r3, [sp, -r1, ror #1]!
 800659b:	513094b2 	teqpl	r0, r2	; <illegal shifter operand>
 800659f:	44098116 	strmi	r8, [r9], #-278	; 0xfffffeea
 80065a3:	000f4d01 	andeq	r4, pc, r1, lsl #26
 80065a7:	28a52d31 	stmiacs	r5!, {r0, r4, r5, r8, sl, fp, sp}
 80065ab:	2971a283 	ldmdbcs	r1!, {r0, r1, r7, r9, sp, pc}^
 80065af:	1728b261 	strne	fp, [r8, -r1, ror #4]!
 80065b3:	e10d027d 	hlt	0xd02d
 80065b7:	cd29e130 	stfgtd	f6, [r9, #-192]!	; 0xffffff40
 80065bb:	65164c25 	ldrvs	r4, [r6, #-3109]	; 0xfffff3db
 80065bf:	013e9729 	teqeq	lr, r9, lsr #14
 80065c3:	6931200e 	ldmdbvs	r1!, {r1, r2, r3, sp}
 80065c7:	ca5fc19a 	bgt	97f6c37 <_sidata+0x17ef3e7>
 80065cb:	cb283945 	blgt	8a14ae7 <_sidata+0xa0d297>
 80065cf:	0e011f65 	cdpeq	15, 0, cr1, cr1, cr5, {3}
 80065d3:	812930e1 			; <UNDEFINED> instruction: 0x812930e1
 80065d7:	6e53022c 	cdpvs	2, 5, cr0, cr3, cr12, {1}
 80065db:	9729652e 	strls	r6, [r9, -lr, lsr #10]!
 80065df:	0003013e 	andeq	r0, r3, lr, lsr r1
 80065e3:	20080020 	andcs	r0, r8, r0, lsr #32
 80065e7:	ffffe131 			; <UNDEFINED> instruction: 0xffffe131
 80065eb:	11097fff 	strdne	r7, [r9, -pc]
 80065ef:	52e13120 	rscpl	r3, r1, #32, 2
 80065f3:	2947452e 	stmdbcs	r7, {r1, r2, r3, r5, r8, sl, lr}^
 80065f7:	a74a7c1d 	smlaldge	r7, sl, sp, ip
 80065fb:	5ca53e94 	stcpl	14, cr3, [r5], #592	; 0x250
 80065ff:	31201502 			; <UNDEFINED> instruction: 0x31201502
 8006603:	88129653 	ldmdahi	r2, {r0, r1, r4, r6, r9, sl, ip, pc}
 8006607:	e94a2930 	stmdb	sl, {r4, r5, r8, fp, sp}^
 800660b:	04a3a294 	strteq	sl, [r3], #660	; 0x294
 800660f:	94d014a2 	ldrbls	r1, [r0], #1186	; 0x4a2
 8006613:	12009440 	andne	r9, r0, #64, 8	; 0x40000000
 8006617:	a5e13120 	strbge	r3, [r1, #288]!	; 0x120
 800661b:	4a680a53 	bmi	9a08f6f <_sidata+0x1a0171f>
 800661f:	d293c23f 	addsle	ip, r3, #-268435453	; 0xf0000003
 8006623:	294fa529 	stmdbcs	pc, {r0, r3, r5, r8, sl, sp, pc}^	; <UNPREDICTABLE>
 8006627:	20130097 	mulscs	r3, r7, r0
 800662b:	94a2f331 	strtls	pc, [r2], #817	; 0x331
 800662f:	a529f4a2 	strge	pc, [r9, #-1186]!	; 0xfffffb5e
 8006633:	528e8a53 	addpl	r8, lr, #339968	; 0x53000
 8006637:	29f4a53a 	ldmibcs	r4!, {r1, r3, r4, r5, r8, sl, sp, pc}^
 800663b:	201212e5 	andscs	r1, r2, r5, ror #5
 800663f:	a5164131 	ldrge	r4, [r6, #-305]	; 0xfffffecf
 8006643:	fb294194 	blx	8a56c9d <_sidata+0xa4f44d>
 8006647:	e94a7d28 	stmdb	sl, {r3, r5, r8, sl, fp, ip, sp, lr}^
 800664b:	94a7d294 	strtls	sp, [r7], #660	; 0x294
 800664f:	3120124b 			; <UNDEFINED> instruction: 0x3120124b
 8006653:	0253a5e1 	subseq	sl, r3, #943718400	; 0x38400000
 8006657:	fa51f459 	blx	94837c3 <_sidata+0x147bf73>
 800665b:	a529d294 	strge	sp, [r9, #-660]!	; 0xfffffd6c
 800665f:	0097294f 	addseq	r2, r7, pc, asr #18
	...

08006664 <font_robotoM18_digits>:
 8006664:	13001200 	movwne	r1, #512	; 0x200
 8006668:	01000600 	tsteq	r0, r0, lsl #12
 800666c:	0c000000 	stceq	0, cr0, [r0], {-0}
 8006670:	04041200 	streq	r1, [r4], #-512	; 0xfffffe00
 8006674:	01040504 	tsteq	r4, r4, lsl #10
 8006678:	2e050202 	cdpcs	2, 0, cr0, cr5, cr2, {0}
 800667c:	2e330204 	cdpcs	2, 3, cr0, cr3, cr4, {0}
 8006680:	306c8103 	rsbcc	r8, ip, r3, lsl #2
 8006684:	dd2c0112 	stfles	f0, [ip, #-72]!	; 0xffffffb8
 8006688:	50a8da16 	adcpl	sp, r8, r6, lsl sl
 800668c:	d8ff8149 	ldmle	pc!, {r0, r3, r6, r8, pc}^	; <UNPREDICTABLE>
 8006690:	8a852562 	bhi	614fc20 <_Min_Stack_Size+0x614f820>
 8006694:	0b0cb436 	bleq	8333774 <_sidata+0x32bf24>
 8006698:	8d5d2702 	ldclhi	7, cr2, [sp, #-8]
 800669c:	fc08f49d 	stc2	4, cr15, [r8], {157}	; 0x9d
 80066a0:	1402a15f 	strne	sl, [r2], #-351	; 0xfffffea1
 80066a4:	16dd2c01 	ldrbne	r2, [sp], r1, lsl #24
 80066a8:	4950a8da 	ldmdbmi	r0, {r1, r3, r4, r6, r7, fp, sp, pc}^
 80066ac:	4a60c5b1 	bmi	9837d78 <_sidata+0x1830528>
 80066b0:	f030ac2b 			; <UNDEFINED> instruction: 0xf030ac2b
 80066b4:	0ff61dce 	svceq	0x00f61dce
 80066b8:	dd2c011c 	stfles	f0, [ip, #-112]!	; 0xffffff90
 80066bc:	50a8da16 	adcpl	sp, r8, r6, lsl sl
 80066c0:	80c5b149 	sbchi	fp, r5, r9, asr #2
 80066c4:	962b0ad2 			; <UNDEFINED> instruction: 0x962b0ad2
 80066c8:	2b4ed2dc 	blcs	93bb240 <_sidata+0x13b39f0>
 80066cc:	a4ac580d 	strtge	r5, [ip], #2061	; 0x80d
 80066d0:	9686d150 	pkhtbls	sp, r6, r0, asr #2
 80066d4:	2c011c01 	stccs	12, cr1, [r1], {1}
 80066d8:	80761d9d 			; <UNDEFINED> instruction: 0x80761d9d
 80066dc:	91b16c4b 			; <UNDEFINED> instruction: 0x91b16c4b
 80066e0:	86a52552 	ssathi	r2, #6, r2, asr #10
 80066e4:	1a15150a 	bne	854bb14 <_sidata+0x5442c4>
 80066e8:	2a0d9590 	bcs	836bd30 <_sidata+0x3644e0>
 80066ec:	a540e7fa 	strbge	lr, [r0, #-2042]	; 0xfffff806
 80066f0:	01190115 	tsteq	r9, r5, lsl r1
 80066f4:	8da01d2c 	stchi	13, cr1, [r0, #176]!	; 0xb0
 80066f8:	280c2b4a 	stmdacs	ip, {r1, r3, r6, r8, r9, fp, sp}
 80066fc:	0a8da1ed 	beq	636eeb8 <_Min_Stack_Size+0x636eab8>
 8006700:	0d2a0715 	stceq	7, cr0, [sl, #-84]!	; 0xffffffac
 8006704:	50a4ac58 	adcpl	sl, r4, r8, asr ip
 8006708:	019686d1 			; <UNDEFINED> instruction: 0x019686d1
 800670c:	9d2c011a 	stflss	f0, [ip, #-104]!	; 0xffffff98
 8006710:	5cec4b95 	fstmiaxpl	ip!, {d20-d93}	;@ Deprecated
 8006714:	5615a561 	ldrpl	sl, [r5], -r1, ror #10
 8006718:	6493ac92 	ldrvs	sl, [r3], #3218	; 0xc92
 800671c:	2dc0a725 	stclcs	7, cr10, [r0, #148]	; 0x94
 8006720:	68a85256 	stmiavs	r8!, {r1, r2, r4, r6, r9, ip, lr}
 8006724:	0e00cb43 	vmlseq.f64	d12, d0, d3
 8006728:	1ffd2c01 	svcne	0x00fd2c01
 800672c:	ac2b4bfc 			; <UNDEFINED> instruction: 0xac2b4bfc
 8006730:	2b0ad2b4 	blcs	82bb208 <_sidata+0x2b39b8>
 8006734:	011e18ad 	tsteq	lr, sp, lsr #17
 8006738:	da16dd2c 	ble	85bdbf0 <_sidata+0x5b63a0>
 800673c:	014950a8 	smlaltbeq	r5, r9, r8, r0
 8006740:	6192b16c 	orrsvs	fp, r2, ip, ror #2
 8006744:	1eca8525 	cdpne	5, 12, cr8, cr10, cr5, {1}
 8006748:	a4a8567a 	strtge	r5, [r8], #1658	; 0x67a
 800674c:	92b16c40 	adcsls	r6, r1, #64, 24	; 0x4000
 8006750:	5a1b4542 	bpl	86d7c60 <_sidata+0x6d0410>
 8006754:	2c011a06 			; <UNDEFINED> instruction: 0x2c011a06
 8006758:	a8da16dd 	ldmge	sl, {r0, r2, r3, r4, r6, r7, r9, sl, ip}^
 800675c:	63814950 	orrvs	r4, r1, #80, 18	; 0x140000
 8006760:	25949d4b 	ldrcs	r9, [r4, #3403]	; 0xd4b
 8006764:	61492ce9 	smlalttvs	r2, r9, r9, ip
 8006768:	ee5615a5 	cdp	5, 5, cr1, cr6, cr5, {5}
 800676c:	00c57258 	sbceq	r7, r5, r8, asr r2
 8006770:	64047f05 	strvs	r7, [r4], #-3845	; 0xfffff0fb
 8006774:	000f7c36 	andeq	r7, pc, r6, lsr ip	; <UNPREDICTABLE>

08006778 <font_robotoR8>:
 8006778:	0b000800 	bleq	8008780 <_sidata+0xf30>
 800677c:	ffff0300 			; <UNDEFINED> instruction: 0xffff0300
 8006780:	0a00feff 	beq	8046384 <_sidata+0x3eb34>
 8006784:	04020b00 	streq	r0, [r2], #-2816	; 0xfffff500
 8006788:	01040404 	tsteq	r4, r4, lsl #8
 800678c:	33820001 	orrcc	r0, r2, #1
 8006790:	80000320 	andhi	r0, r0, r0, lsr #6
 8006794:	a0400400 	subge	r0, r0, r0, lsl #8
 8006798:	d40514fc 	strle	r1, [r5], #-1276	; 0xfffffb04
 800679c:	0534050c 	ldreq	r0, [r4, #-1292]!	; 0xfffffaf4
 80067a0:	59e1c00f 	stmibpl	r1!, {r0, r1, r2, r3, lr, pc}^
 80067a4:	47e8a58a 	strbmi	sl, [r8, sl, lsl #11]!
 80067a8:	1fa29629 	svcne	0x00a29629
 80067ac:	00ca58a5 	sbceq	r5, sl, r5, lsr #17
 80067b0:	51a97c0c 			; <UNDEFINED> instruction: 0x51a97c0c
 80067b4:	cee6ca76 			; <UNDEFINED> instruction: 0xcee6ca76
 80067b8:	2ce966c5 	stclcs	6, cr6, [r9], #788	; 0x314
 80067bc:	62001002 	andvs	r1, r0, #2
 80067c0:	29440e6a 	stmdbcs	r4, {r1, r3, r5, r6, r9, sl, fp}^
 80067c4:	9714d94a 	ldrls	sp, [r4, -sl, asr #18]
 80067c8:	45149866 	ldrmi	r9, [r4, #-2150]	; 0xfffff79a
 80067cc:	800d09b9 			; <UNDEFINED> instruction: 0x800d09b9
 80067d0:	450669e1 	strmi	r6, [r6, #-2529]	; 0xfffff61f
 80067d4:	b34a2cd9 	movtlt	r2, #44249	; 0xacd9
 80067d8:	00a74594 	umlaleq	r4, r7, r4, r5
 80067dc:	0c885803 	stceq	8, cr5, [r8], {3}
 80067e0:	512cf809 	msrpl	CPSR_fs, r9, lsl #16
 80067e4:	ca2c128a 	bgt	8b0b214 <_sidata+0xb039c4>
 80067e8:	f8092ca2 			; <UNDEFINED> instruction: 0xf8092ca2
 80067ec:	0651652c 	ldrbeq	r6, [r1], -ip, lsr #10
 80067f0:	2294528b 	addscs	r5, r4, #-1342177272	; 0xb0000008
 80067f4:	51954c08 	orrspl	r4, r5, r8, lsl #24
 80067f8:	4b28fa2c 	blmi	8a450b0 <_sidata+0xa3d860>
 80067fc:	95440809 	strbls	r0, [r4, #-2057]	; 0xfffff7f7
 8006800:	28fa2c51 	ldmcs	sl!, {r0, r4, r6, sl, fp, sp}^
 8006804:	bb040116 	bllt	8106c64 <_sidata+0xff414>
 8006808:	034d288c 	movteq	r2, #55436	; 0xd88c
 800680c:	031d04cc 	tsteq	sp, #204, 8	; 0xcc000000
 8006810:	0a048440 	beq	8127918 <_sidata+0x1200c8>
 8006814:	8b01653c 	blhi	805fd0c <_sidata+0x584bc>
 8006818:	11651640 	cmnne	r5, r0, asr #12
 800681c:	400800c8 	andmi	r0, r8, r8, asr #1
 8006820:	9c14e9a1 			; <UNDEFINED> instruction: 0x9c14e9a1
 8006824:	0609d2cd 	streq	sp, [r9], -sp, asr #5
 8006828:	83d1a0c1 	bicshi	sl, r1, #193	; 0xc1
 800682c:	40090147 	andmi	r0, r9, r7, asr #2
 8006830:	c594e9a1 	ldrgt	lr, [r4, #2465]	; 0x9a1
 8006834:	03ec5960 	mvneq	r5, #96, 18	; 0x180000
 8006838:	e9a1400b 	stmib	r1!, {r0, r1, r3, lr}
 800683c:	b9a2c594 	stmiblt	r2!, {r2, r4, r7, r8, sl, lr, pc}
 8006840:	009d2cd8 			; <UNDEFINED> instruction: 0x009d2cd8
 8006844:	59a1400b 	stmibpl	r1!, {r0, r1, r3, lr}
 8006848:	14a50c36 	strtne	r0, [r5], #3126	; 0xc36
 800684c:	0962cfa5 	stmdbeq	r2!, {r0, r2, r5, r7, r8, r9, sl, fp, lr, pc}^
 8006850:	fda1400a 	stc2	0, cr4, [r1, #40]!	; 0x28
 8006854:	d8b9ec58 	ldmle	r9!, {r3, r4, r6, sl, fp, sp, lr, pc}
 8006858:	0a009d2c 	beq	802dd10 <_sidata+0x264c0>
 800685c:	28d1a140 	ldmcs	r1, {r6, r8, sp, pc}^
 8006860:	6c053d8b 	stcvs	13, cr3, [r5], {139}	; 0x8b
 8006864:	40074e96 	mulmi	r7, r6, lr
 8006868:	2c587da1 	mrrccs	13, 10, r7, r8, cr1
 800686c:	400b658b 	andmi	r6, fp, fp, lsl #11
 8006870:	cd94e9a1 	vldrgt.16	s28, [r4, #322]	; 0x142	; <UNPREDICTABLE>
 8006874:	b36029d2 	cmnlt	r0, #3440640	; 0x348000
 8006878:	400a0274 	andmi	r0, sl, r4, ror r2
 800687c:	b014e9a1 	andslt	lr, r4, r1, lsr #19
 8006880:	8b2c7a59 	blhi	8b251ec <_sidata+0xb1d99c>
 8006884:	98400446 	stmdals	r0, {r1, r2, r6, sl}^
 8006888:	b80602c4 	stmdalt	r6, {r2, r6, r7, r9}
 800688c:	9a5f28e0 	bls	97d0c14 <_sidata+0x17c93c4>
 8006890:	95440700 	strbls	r0, [r4, #-1792]	; 0xfffff900
 8006894:	b80a34d9 	stmdalt	sl, {r0, r3, r4, r6, r7, sl, ip, sp}
 8006898:	4d080501 	cfstr32mi	mvfx0, [r8, #-4]
 800689c:	0700f63d 	smuadxeq	r0, sp, r6
 80068a0:	b9819544 	stmiblt	r1, {r2, r6, r8, sl, ip, pc}
 80068a4:	0a019a68 	beq	806d24c <_sidata+0x659fc>
 80068a8:	94e9a140 	strbtls	sl, [r9], #320	; 0x140
 80068ac:	cb1640c5 	blgt	8596bc8 <_sidata+0x58f378>
 80068b0:	38110117 	ldmdacc	r1, {r0, r1, r2, r4, r8}
 80068b4:	58b3d26a 	ldmpl	r3!, {r1, r3, r5, r6, r9, ip, lr, pc}
 80068b8:	52f0d00a 	rscspl	sp, r0, #10
 80068bc:	4680a694 	pkhbtmi	sl, r0, r4, lsl #13
 80068c0:	13ef1469 	mvnne	r1, #1761607680	; 0x69000000
 80068c4:	0d414a82 	vstreq	s9, [r1, #-520]	; 0xfffffdf8
 80068c8:	5c5a21c0 	ldfple	f2, [sl], {192}	; 0xc0
 80068cc:	c812c52e 	ldmdagt	r2, {r1, r2, r3, r5, r8, sl, lr, pc}
 80068d0:	b9b94fa2 	ldmiblt	r9!, {r1, r5, r7, r8, r9, sl, fp, lr}
 80068d4:	a1400a00 	cmpge	r0, r0, lsl #20
 80068d8:	4fb3653d 	svcmi	0x00b3653d
 80068dc:	027d9b01 	rsbseq	r9, sp, #1024	; 0x400
 80068e0:	e9e1800b 	stmib	r1!, {r0, r1, r3, pc}^
 80068e4:	5c06c029 	stcpl	0, cr12, [r6], {41}	; 0x29
 80068e8:	013d2c6c 	teqeq	sp, ip, ror #24
 80068ec:	3de1800a 	stclcc	0, cr8, [r1, #40]!	; 0x28
 80068f0:	363052ca 	ldrtcc	r5, [r0], -sl, asr #5
 80068f4:	08008f4b 	stmdaeq	r0, {r0, r1, r3, r6, r8, r9, sl, fp, pc}
 80068f8:	58fda140 	ldmpl	sp!, {r6, r8, sp, pc}^
 80068fc:	7d8a09ec 	vstrvc.16	s0, [sl, #472]	; 0x1d8	; <UNPREDICTABLE>
 8006900:	fda14008 	stc2	0, cr4, [r1, #32]!
 8006904:	1609ec58 			; <UNDEFINED> instruction: 0x1609ec58
 8006908:	e1800b03 	orr	r0, r0, r3, lsl #22
 800690c:	e6c029e9 	strb	r2, [r0], r9, ror #19
 8006910:	d2c6c7a2 	sbcle	ip, r6, #42467328	; 0x2880000
 8006914:	e1800913 	orr	r0, r0, r3, lsl r9
 8006918:	3fd8d805 	svccc	0x00d8d805
 800691c:	04058d88 	streq	r8, [r5], #-3464	; 0xfffff278
 8006920:	03fca040 	mvnseq	sl, #64	; 0x40
 8006924:	c1a14008 			; <UNDEFINED> instruction: 0xc1a14008
 8006928:	9d2cd8b1 	stcls	8, cr13, [ip, #-708]!	; 0xfffffd3c
 800692c:	e1800d00 	orr	r0, r0, r0, lsl #26
 8006930:	440966c5 	strmi	r6, [r9], #-1733	; 0xfffff93b
 8006934:	4c229b29 			; <UNDEFINED> instruction: 0x4c229b29
 8006938:	06016296 			; <UNDEFINED> instruction: 0x06016296
 800693c:	17c1a140 	strbne	sl, [r1, r0, asr #2]
 8006940:	c00d00fb 	strdgt	r0, [sp], -fp
 8006944:	cee6e621 	cdpgt	6, 14, cr14, cr6, cr1, {1}
 8006948:	a51e1d9e 	ldrge	r1, [lr, #-3486]	; 0xfffff262
 800694c:	28068a34 	stmdacs	r6, {r2, r4, r5, r9, fp, pc}
 8006950:	c5e1800b 	strbgt	r8, [r1, #11]!
 8006954:	8d14d9c0 	vldrhi.16	s26, [r4, #-384]	; 0xfffffe80	; <UNPREDICTABLE>
 8006958:	0b1d80d2 	bleq	8766ca8 <_sidata+0x75f458>
 800695c:	e9e18009 	stmib	r1!, {r0, r3, pc}^
 8006960:	96363829 	ldrtls	r3, [r6], -r9, lsr #16
 8006964:	8009009e 	mulhi	r9, lr, r0
 8006968:	b00a7de1 	andlt	r7, sl, r1, ror #27
 800696c:	1ca09fb1 	stcne	15, cr9, [r0], #708	; 0x2c4
 8006970:	e9e5bc09 	stmib	r5!, {r0, r3, sl, fp, ip, sp, pc}^
 8006974:	96363829 	ldrtls	r3, [r6], -r9, lsr #16
 8006978:	400a1b9e 	mulmi	sl, lr, fp
 800697c:	6c053da1 	stcvs	13, cr3, [r5], {161}	; 0xa1
 8006980:	d94a29f6 	stmdble	sl, {r1, r2, r4, r5, r6, r7, r8, fp, sp}^
 8006984:	a1400a2c 	cmpge	r0, ip, lsr #20
 8006988:	9dcd94e9 	cfstrdls	mvd9, [sp, #932]	; 0x3a4
 800698c:	09d2cd8b 	ldmibeq	r2, {r0, r1, r3, r7, r8, sl, fp, lr, pc}^
 8006990:	fe21c007 	cdp2	0, 2, cr12, cr1, cr7, {0}
 8006994:	0cb8bc19 	ldceq	12, cr11, [r8], #100	; 0x64
 8006998:	05e18007 	strbeq	r8, [r1, #7]!
 800699c:	27a58d8f 	strcs	r8, [r5, pc, lsl #27]!
 80069a0:	e621c00d 	strt	ip, [r1], -sp
 80069a4:	811652e6 	tsthi	r6, r6, ror #5
 80069a8:	2e52c52c 	cdpcs	5, 5, cr12, cr2, cr12, {1}
 80069ac:	4010032e 	andsmi	r0, r0, lr, lsr #6
 80069b0:	c06c82a2 	rsbgt	r8, ip, r2, lsr #5
 80069b4:	29411412 	stmdbcs	r1, {r1, r4, sl, ip}^
 80069b8:	30c81282 	sbccc	r1, r8, r2, lsl #5
 80069bc:	0c022cb1 	stceq	12, cr2, [r2], {177}	; 0xb1
 80069c0:	12c5e180 	sbcne	lr, r5, #128, 2
 80069c4:	33636514 	cmncc	r3, #20, 10	; 0x5000000
 80069c8:	00b14a28 	adcseq	r4, r1, r8, lsr #20
 80069cc:	e621c00b 	strt	ip, [r1], -fp
 80069d0:	2c811652 	stccs	6, cr1, [r1], {82}	; 0x52
 80069d4:	0cb8a0e5 	ldceq	0, cr10, [r8], #916	; 0x394
 80069d8:	fde18008 	stc2l	0, cr8, [r1, #32]!
 80069dc:	f72c5c1c 			; <UNDEFINED> instruction: 0xf72c5c1c
 80069e0:	ecb80903 			; <UNDEFINED> instruction: 0xecb80903
 80069e4:	294a529c 	stmdbcs	sl, {r2, r3, r4, r7, r9, ip, lr}^
 80069e8:	090694a5 	stmdbeq	r6, {r0, r2, r5, r7, sl, ip, pc}
 80069ec:	3165653c 	cmncc	r5, ip, lsr r5
 80069f0:	58b2c590 	ldmpl	r2!, {r4, r7, r8, sl, lr, pc}
 80069f4:	ecb80916 			; <UNDEFINED> instruction: 0xecb80916
 80069f8:	94a5294c 	strtls	r2, [r5], #2380	; 0x94c
 80069fc:	08074a52 	stmdaeq	r7, {r1, r4, r6, r9, fp, lr}
 8006a00:	96519150 			; <UNDEFINED> instruction: 0x96519150
 8006a04:	00b294a2 	adcseq	r9, r2, r2, lsr #9
 8006a08:	7d857c03 	stcvc	12, cr7, [r5, #12]
 8006a0c:	44c89804 	strbmi	r9, [r8], #2052	; 0x804
 8006a10:	612b8201 			; <UNDEFINED> instruction: 0x612b8201
 8006a14:	e9994009 	ldmib	r9, {r0, r3, lr}
 8006a18:	66cfa594 			; <UNDEFINED> instruction: 0x66cfa594
 8006a1c:	400901e9 	andmi	r0, r9, r9, ror #3
 8006a20:	53d881a1 	bicspl	r8, r8, #1073741864	; 0x40000028
 8006a24:	013ecd90 	teqeq	lr, r0	; <illegal shifter operand>
 8006a28:	e9994009 	ldmib	r9, {r0, r3, lr}
 8006a2c:	2cb00d94 	ldccs	13, cr0, [r0], #592	; 0x250
 8006a30:	4009009d 	mulmi	r9, sp, r0
 8006a34:	3e9601a1 	fmlccdp	f0, f6, f1
 8006a38:	01e966c8 	mvneq	r6, r8, asr #13
 8006a3c:	e9994009 	ldmib	r9, {r0, r3, lr}
 8006a40:	2cb1fd94 	ldccs	13, cr15, [r1], #592	; 0x250
 8006a44:	0009009d 	muleq	r9, sp, r0
 8006a48:	4594d125 	ldrmi	sp, [r4, #293]	; 0x125
 8006a4c:	022cb047 	eoreq	fp, ip, #71	; 0x47
 8006a50:	e9a17809 	stmib	r1!, {r0, r3, fp, ip, sp, lr}
 8006a54:	1e966c83 	cdpne	12, 9, cr6, cr6, cr3, {4}
 8006a58:	4008274b 	andmi	r2, r8, fp, asr #14
 8006a5c:	53d881a1 	bicspl	r8, r8, #1073741864	; 0x40000028
 8006a60:	04059b30 	streq	r9, [r5], #-2864	; 0xfffff4d0
 8006a64:	1fa4a040 	svcne	0x00a4a040
 8006a68:	28a8bb08 	stmiacs	r8!, {r3, r8, r9, fp, ip, sp, pc}
 8006a6c:	94a5294b 	strtls	r2, [r5], #2379	; 0x94b
 8006a70:	a1400c26 	cmpge	r0, r6, lsr #24
 8006a74:	4a365881 	bmi	8d9cc80 <_sidata+0xd95430>
 8006a78:	94529a29 	ldrbls	r9, [r2], #-2601	; 0xfffff5d7
 8006a7c:	400400b2 	strhmi	r0, [r4], -r2
 8006a80:	0a03fca0 	beq	8105d08 <_sidata+0xfe4b8>
 8006a84:	e9a69a40 	stmib	r6!, {r6, r9, fp, ip, pc}
 8006a88:	cd96c234 	lfmgt	f4, 1, [r6, #208]	; 0xd0
 8006a8c:	400700b2 	strhmi	r0, [r7], -r2
 8006a90:	82340599 	eorshi	r0, r4, #641728512	; 0x26400000
 8006a94:	400802cd 	andmi	r0, r8, sp, asr #5
 8006a98:	6414e999 	ldrvs	lr, [r4], #-2457	; 0xfffff667
 8006a9c:	090274b3 	stmdbeq	r2, {r0, r1, r4, r5, r7, sl, ip, sp, lr}
 8006aa0:	053da178 	ldreq	sl, [sp, #-376]!	; 0xfffffe88
 8006aa4:	8813ecd9 	ldmdahi	r3, {r0, r3, r4, r6, r7, sl, fp, sp, lr, pc}
 8006aa8:	a1780901 	cmnge	r8, r1, lsl #18
 8006aac:	966c83e9 	strbtls	r8, [ip], -r9, ror #7
 8006ab0:	0601601e 			; <UNDEFINED> instruction: 0x0601601e
 8006ab4:	13a518c0 			; <UNDEFINED> instruction: 0x13a518c0
 8006ab8:	4009022c 	andmi	r0, r9, ip, lsr #4
 8006abc:	a594e999 	ldrge	lr, [r4, #2457]	; 0x999
 8006ac0:	0274b373 	rsbseq	fp, r4, #-872415231	; 0xcc000001
 8006ac4:	291cc007 	ldmdbcs	ip, {r0, r1, r2, lr, pc}
 8006ac8:	0b28a09d 	bleq	8a2ed44 <_sidata+0xa274f4>
 8006acc:	05994007 	ldreq	r4, [r9, #7]
 8006ad0:	029811b3 	addseq	r1, r8, #-1073741780	; 0xc000002c
 8006ad4:	65994009 	ldrvs	r4, [r9, #9]
 8006ad8:	94a294b3 	strtls	r9, [r2], #1203	; 0x4b3
 8006adc:	400d22c5 	andmi	r2, sp, r5, asr #5
 8006ae0:	c06c829a 	mlsgt	ip, sl, r2, r8
 8006ae4:	12822982 	addne	r2, r2, #2129920	; 0x208000
 8006ae8:	045962c8 	ldrbeq	r6, [r9], #-712	; 0xfffffd38
 8006aec:	65994009 	ldrvs	r4, [r9, #9]
 8006af0:	2cb16529 	cfldr32cs	mvfx6, [r1], #164	; 0xa4
 8006af4:	780b2ca5 	stmdavc	fp, {r0, r2, r5, r7, sl, fp, sp}
 8006af8:	94b365a1 	ldrtls	r6, [r3], #1441	; 0x5a1
 8006afc:	588194a2 	stmpl	r1, {r1, r5, r7, sl, ip, pc}
 8006b00:	40060334 	andmi	r0, r6, r4, lsr r3
 8006b04:	8b307d99 	blhi	8c26170 <_sidata+0xc1e920>
 8006b08:	2cf8097d 			; <UNDEFINED> instruction: 0x2cf8097d
 8006b0c:	94a28251 	strtls	r8, [r2], #593	; 0x251
 8006b10:	04165141 	ldreq	r5, [r6], #-321	; 0xfffffebf
 8006b14:	07fca47c 			; <UNDEFINED> instruction: 0x07fca47c
 8006b18:	652cf809 	strvs	pc, [ip, #-2057]!	; 0xfffff7f7
 8006b1c:	28259450 	stmdacs	r5!, {r4, r6, sl, ip, pc}
 8006b20:	8805114a 	stmdahi	r5, {r1, r3, r6, r8, ip}
 8006b24:	268c09c9 	strcs	r0, [ip], r9, asr #19
 8006b28:	00400003 	subeq	r0, r0, r3
 8006b2c:	655d0008 	ldrbvs	r0, [sp, #-8]
 8006b30:	469fd3a3 	ldrmi	sp, [pc], r3, lsr #7
 8006b34:	15c40919 	strbne	r0, [r4, #2329]	; 0x919
 8006b38:	b204b972 	andlt	fp, r4, #1867776	; 0x1c8000
 8006b3c:	09018b94 	stmdbeq	r1, {r2, r4, r7, r8, r9, fp, pc}
 8006b40:	78083581 	stmdavc	r8, {r0, r7, r8, sl, ip, sp}
 8006b44:	d9b305a1 	ldmible	r3!, {r0, r5, r7, r8, sl}
 8006b48:	00031027 	andeq	r1, r3, r7, lsr #32

08006b4c <font_symbols5>:
 8006b4c:	07000600 	streq	r0, [r0, -r0, lsl #12]
 8006b50:	02000100 	andeq	r0, r0, #0, 2
 8006b54:	0c00ffff 	stceq	15, cr15, [r0], {255}	; 0xff
 8006b58:	01030800 	tsteq	r3, r0, lsl #16
 8006b5c:	01040404 	tsteq	r4, r4, lsl #8
 8006b60:	30730101 	rsbscc	r0, r3, r1, lsl #2
 8006b64:	7de6c20b 	sfmvc	f4, 3, [r6, #44]!	; 0x2c
 8006b68:	f00a2f8a 			; <UNDEFINED> instruction: 0xf00a2f8a
 8006b6c:	13ea2f95 	mvnne	r2, #596	; 0x254
 8006b70:	7de6c20b 	sfmvc	f4, 3, [r6, #44]!	; 0x2c
 8006b74:	e02a2f2a 	eor	r2, sl, sl, lsr #30
 8006b78:	13ea2f35 	mvnne	r2, #53, 30	; 0xd4
 8006b7c:	7de6c20b 	sfmvc	f4, 3, [r6, #44]!	; 0x2c
 8006b80:	c06a2e6a 	rsbgt	r2, sl, sl, ror #28
 8006b84:	13ea2e75 	mvnne	r2, #1872	; 0x750
 8006b88:	7de6c20b 	sfmvc	f4, 3, [r6, #44]!	; 0x2c
 8006b8c:	80ea2cea 	rschi	r2, sl, sl, ror #25
 8006b90:	13ea2cf5 	mvnne	r2, #62720	; 0xf500
 8006b94:	7de6c20b 	sfmvc	f4, 3, [r6, #44]!	; 0x2c
 8006b98:	01ea29ea 	mvneq	r2, sl, ror #19
 8006b9c:	13ea29f5 	mvnne	r2, #4014080	; 0x3d4000
 8006ba0:	a0e6c209 	rscge	ip, r6, r9, lsl #4
 8006ba4:	a7fffd4f 	ldrbge	pc, [pc, pc, asr #26]!	; <UNPREDICTABLE>
 8006ba8:	c210013e 	andsgt	r0, r0, #-2147483633	; 0x8000000f
 8006bac:	9501a7e6 	strls	sl, [r1, #-2022]	; 0xfffff81a
 8006bb0:	e9a9a546 	stmib	r9!, {r1, r2, r6, r8, sl, sp, pc}
 8006bb4:	3a29f4d1 	bcc	8a83f00 <_sidata+0xa7c6b0>
 8006bb8:	1026a295 	mlane	r6, r5, r2, sl
 8006bbc:	8b8ab89a 	blhi	62b4e2c <_Min_Stack_Size+0x62b4a2c>
 8006bc0:	2c534542 	cfldr64cs	mvdx4, [r3], {66}	; 0x42
 8006bc4:	6162610a 	cmnvs	r2, sl, lsl #2
 8006bc8:	30b9d620 	adcscc	sp, r9, r0, lsr #12
 8006bcc:	5186620a 	orrpl	r6, r6, sl, lsl #4
 8006bd0:	37147669 	ldrcc	r7, [r4, -r9, ror #12]
 8006bd4:	00009d16 	andeq	r9, r0, r6, lsl sp

08006bd8 <font_tiny5>:
 8006bd8:	06000500 	streq	r0, [r0], -r0, lsl #10
 8006bdc:	00000200 	andeq	r0, r0, r0, lsl #4
 8006be0:	0500ffff 	streq	pc, [r0, #-4095]	; 0xfffff001
 8006be4:	03010700 	movweq	r0, #5888	; 0x1700
 8006be8:	01030303 	tsteq	r3, r3, lsl #6
 8006bec:	3a810000 	bcc	6046bf4 <_Min_Stack_Size+0x60467f4>
 8006bf0:	08000220 	stmdaeq	r0, {r5, r9}
 8006bf4:	14ea9003 	strbtne	r9, [sl], #3
 8006bf8:	4d313604 	ldcmi	6, cr3, [r1, #-16]!
 8006bfc:	1ad00801 	bne	7408c08 <_Min_Stack_Size+0x7408808>
 8006c00:	fa503e94 	blx	9416658 <_sidata+0x140ee08>
 8006c04:	3e060940 	vmlscc.f16	s0, s12, s0	; <UNPREDICTABLE>
 8006c08:	6773d953 			; <UNDEFINED> instruction: 0x6773d953
 8006c0c:	16420409 	strbne	r0, [r2], -r9, lsl #8
 8006c10:	c0070597 	mulgt	r7, r7, r5
 8006c14:	2c825956 			; <UNDEFINED> instruction: 0x2c825956
 8006c18:	16022965 	strne	r2, [r2], -r5, ror #18
 8006c1c:	4ea00569 	cdpmi	5, 10, cr0, cr0, cr9, {3}
 8006c20:	0502ca53 	streq	ip, [r2, #-2643]	; 0xfffff5ad
 8006c24:	694b2ea0 	stmdbvs	fp, {r5, r7, r9, sl, fp, sp}^
 8006c28:	52320502 	eorspl	r0, r2, #8388608	; 0x800000
 8006c2c:	050290e9 	streq	r9, [r2, #-233]	; 0xffffff17
 8006c30:	94e951b2 	strbtls	r5, [r9], #434	; 0x1b2
 8006c34:	4d2e0300 	stcmi	3, cr0, [lr, #-0]
 8006c38:	f0b40313 			; <UNDEFINED> instruction: 0xf0b40313
 8006c3c:	28900200 	ldmcs	r0, {r9}
 8006c40:	694ea005 	stmdbvs	lr, {r0, r2, sp, pc}^
 8006c44:	b006094a 	andlt	r0, r6, sl, asr #18
 8006c48:	a0681412 	rsbge	r1, r8, r2, lsl r4
 8006c4c:	52a00504 	adcpl	r0, r0, #4, 10	; 0x1000000
 8006c50:	050294a7 	streq	r9, [r2, #-1191]	; 0xfffffb59
 8006c54:	520e72b0 	andpl	r7, lr, #176, 4
 8006c58:	72b00576 	adcsvc	r0, r0, #494927872	; 0x1d800000
 8006c5c:	054ee52e 	strbeq	lr, [lr, #-1326]	; 0xfffffad2
 8006c60:	e9d2d2b0 	ldmib	r2, {r4, r5, r7, r9, ip, lr, pc}^
 8006c64:	f2b00559 	vshl.s32	q0, <illegal reg q4.5>, #16
 8006c68:	0509dcd9 	streq	sp, [r9, #-3289]	; 0xfffff327
 8006c6c:	436752b0 	cmnmi	r7, #176, 4
 8006c70:	f2b0064a 	vext.8	q0, q0, q5, #6
 8006c74:	0096412c 	addseq	r4, r6, ip, lsr #2
 8006c78:	9f12b004 	svcls	0x0012b004
 8006c7c:	12b00512 	adcsne	r0, r0, #75497472	; 0x4800000
 8006c80:	034ecd29 	movteq	ip, #60713	; 0xed29
 8006c84:	040b2a10 	streq	r2, [fp], #-2576	; 0xfffff5f0
 8006c88:	26f94eae 	ldrbtcs	r4, [r9], lr, lsr #29
 8006c8c:	20d2b005 	sbcscs	fp, r2, r5
 8006c90:	b2042e41 	andlt	r2, r4, #1040	; 0x410
 8006c94:	0501dcf1 	streq	sp, [r1, #-3313]	; 0xfffff30f
 8006c98:	907212b0 	ldrhtls	r1, [r2], #-32	; 0xffffffe0
 8006c9c:	72b00532 	adcsvc	r0, r0, #209715200	; 0xc800000
 8006ca0:	074be52e 	strbeq	lr, [fp, -lr, lsr #10]
 8006ca4:	365356c0 	ldrbcc	r5, [r3], -r0, asr #13
 8006ca8:	8101379d 			; <UNDEFINED> instruction: 0x8101379d
 8006cac:	c0064156 	andgt	r4, r6, r6, asr r1
 8006cb0:	b3f65356 	mvnslt	r5, #1476395009	; 0x58000001
 8006cb4:	f6c00659 			; <UNDEFINED> instruction: 0xf6c00659
 8006cb8:	4f653d94 	svcmi	0x00653d94
 8006cbc:	5356c007 	cmppl	r6, #7
 8006cc0:	09a59736 	stmibeq	r5!, {r1, r2, r4, r5, r8, r9, sl, ip, pc}
 8006cc4:	14f6c006 	ldrbtne	ip, [r6], #6
 8006cc8:	0609ecd8 			; <UNDEFINED> instruction: 0x0609ecd8
 8006ccc:	a773f6c0 	ldrbge	pc, [r3, -r0, asr #13]!	; <UNPREDICTABLE>
 8006cd0:	c00603dc 	ldrdgt	r0, [r6], -ip
 8006cd4:	c82773f6 	stmdagt	r7!, {r1, r2, r4, r5, r6, r7, r8, r9, ip, sp, lr}
 8006cd8:	56c00601 	strbpl	r0, [r0], r1, lsl #12
 8006cdc:	74b3a5cf 	ldrtvc	sl, [r3], #1487	; 0x5cf
 8006ce0:	9b36c006 	blls	8db6d00 <_sidata+0xdaf4b0>
 8006ce4:	03166cfd 	tsteq	r6, #64768	; 0xfd00
 8006ce8:	0603ea90 			; <UNDEFINED> instruction: 0x0603ea90
 8006cec:	1b2c12b0 	blne	8b0b7b4 <_sidata+0xb03f64>
 8006cf0:	c0070128 	andgt	r0, r7, r8, lsr #2
 8006cf4:	0cd29b36 	vldmiaeq	r2, {d25-<overflow reg d51>}
 8006cf8:	b0041652 	andlt	r1, r4, r2, asr r6
 8006cfc:	070ecb12 	smladeq	lr, r2, fp, ip
 8006d00:	a7073ad0 			; <UNDEFINED> instruction: 0xa7073ad0
 8006d04:	07017304 	streq	r7, [r1, -r4, lsl #6]
 8006d08:	4d3b36c0 	ldcmi	6, cr3, [fp, #-768]!	; 0xfffffd00
 8006d0c:	0700b367 	streq	fp, [r0, -r7, ror #6]
 8006d10:	605356c0 	subsvs	r5, r3, r0, asr #13
 8006d14:	060134b3 			; <UNDEFINED> instruction: 0x060134b3
 8006d18:	3d94f6c0 	ldccc	6, cr15, [r4, #768]	; 0x300
 8006d1c:	c0070e41 	andgt	r0, r7, r1, asr #28
 8006d20:	53605356 	cmnpl	r0, #1476395009	; 0x58000001
 8006d24:	c0070296 	mulgt	r7, r6, r2
 8006d28:	653d94f6 	ldrvs	r9, [sp, #-1270]!	; 0xfffffb0a
 8006d2c:	c00600b3 	strhgt	r0, [r6], -r3
 8006d30:	f79bcf56 			; <UNDEFINED> instruction: 0xf79bcf56
 8006d34:	f2b00504 	vext.8	d0, d0, d4, #5
 8006d38:	0604b284 	streq	fp, [r4], -r4, lsl #5
 8006d3c:	b36436c0 	cmnlt	r4, #192, 12	; 0xc000000
 8006d40:	b0060134 	andlt	r0, r6, r4, lsr r1
 8006d44:	5940d032 	stmdbpl	r0, {r1, r4, r5, ip, lr, pc}^
 8006d48:	1ad00802 	bne	7408d58 <_Min_Stack_Size+0x7408958>
 8006d4c:	48253312 	stmdami	r5!, {r1, r4, r8, r9, ip, sp}
 8006d50:	b0062419 	andlt	r2, r6, r9, lsl r4
 8006d54:	4d290d32 	stcmi	13, cr0, [r9, #-200]!	; 0xffffff38
 8006d58:	32b00601 	adcscc	r0, r0, #1048576	; 0x100000
 8006d5c:	0258140d 	subseq	r1, r8, #218103808	; 0xd000000
 8006d60:	0cf2b005 	ldcleq	0, cr11, [r2], #20
 8006d64:	a0057652 	andge	r7, r5, r2, asr r6
 8006d68:	00d294ee 	sbcseq	r9, r2, lr, ror #9
 8006d6c:	652ea005 	strvs	sl, [lr, #-5]!
 8006d70:	a0050529 	andge	r0, r5, r9, lsr #10
 8006d74:	00e94a6e 	rsceq	r4, r9, lr, ror #20
 8006d78:	29113603 	ldmdbcs	r1, {r0, r1, r9, sl, ip, sp}
 8006d7c:	00f0b003 	rscseq	fp, r0, r3
 8006d80:	0b2d2603 	bleq	8b50594 <_sidata+0xb48d44>
 8006d84:	05613581 	strbeq	r3, [r1, #-1409]!	; 0xfffffa7f
 8006d88:	29a75230 	stmibcs	r7!, {r4, r5, r9, ip, lr}
 8006d8c:	32b0060d 	adcscc	r0, r0, #13631488	; 0xd00000
 8006d90:	0274d29b 	rsbseq	sp, r4, #-1342177271	; 0xb0000009
 8006d94:	67523005 	ldrbvs	r3, [r2, -r5]
 8006d98:	b00601b9 			; <UNDEFINED> instruction: 0xb00601b9
 8006d9c:	4a69d2d2 	bmi	9a7b8ec <_sidata+0x1a7409c>
 8006da0:	52300503 	eorspl	r0, r0, #12582912	; 0xc00000
 8006da4:	0501b9e9 	streq	fp, [r1, #-2537]	; 0xfffff617
 8006da8:	29d34ea0 	ldmibcs	r3, {r5, r7, r9, sl, fp, lr}^
 8006dac:	52be0601 	adcspl	r0, lr, #1048576	; 0x100000
 8006db0:	013d29a7 	teqeq	sp, r7, lsr #19
 8006db4:	9b32b006 	blls	8cb2dd4 <_sidata+0xcab584>
 8006db8:	03014d02 	movweq	r4, #7426	; 0x1d02
 8006dbc:	033d2b10 	teqeq	sp, #16, 22	; 0x4000
 8006dc0:	067d2b9e 			; <UNDEFINED> instruction: 0x067d2b9e
 8006dc4:	a74b32b0 			; <UNDEFINED> instruction: 0xa74b32b0
 8006dc8:	90030534 	andls	r0, r3, r4, lsr r5
 8006dcc:	500703ea 	andpl	r0, r7, sl, ror #7
 8006dd0:	346281fa 	strbtcc	r8, [r2], #-506	; 0xfffffe06
 8006dd4:	300500a5 	andcc	r0, r5, r5, lsr #1
 8006dd8:	05340a72 	ldreq	r0, [r4, #-2674]!	; 0xfffff58e
 8006ddc:	94123005 	ldrls	r3, [r2], #-5
 8006de0:	be064a06 	vmlalt.f32	s8, s12, s12
 8006de4:	29d34a72 	ldmibcs	r3, {r1, r4, r5, r6, r9, fp, lr}^
 8006de8:	52be0603 	adcspl	r0, lr, #3145728	; 0x300000
 8006dec:	02cd29a7 	sbceq	r2, sp, #2736128	; 0x29c000
 8006df0:	94ee2004 	strbtls	r2, [lr], #4
 8006df4:	52300512 	eorspl	r0, r0, #75497472	; 0x4800000
 8006df8:	05009dcf 	streq	r9, [r0, #-3535]	; 0xfffff231
 8006dfc:	b29d2ea0 	addslt	r2, sp, #160, 28	; 0xa00
 8006e00:	32300500 	eorscc	r0, r0, #0, 10
 8006e04:	050694d0 	streq	r9, [r6, #-1232]	; 0xfffffb30
 8006e08:	940d3230 	strls	r3, [sp], #-560	; 0xfffffdd0
 8006e0c:	1a500725 	bne	9408aa8 <_sidata+0x1401258>
 8006e10:	906530d2 	ldrdls	r3, [r5], #-2	; <UNPREDICTABLE>
 8006e14:	12300500 	eorsne	r0, r0, #0, 10
 8006e18:	05290652 	streq	r0, [r9, #-1618]!	; 0xfffff9ae
 8006e1c:	90d032be 	ldrhls	r3, [r0], #46	; 0x2e
 8006e20:	f2300532 	vqrshl.s64	d0, d18, d0
 8006e24:	0601d940 	streq	sp, [r1], -r0, asr #18
 8006e28:	e50352b0 	str	r5, [r3, #-688]	; 0xfffffd50
 8006e2c:	900301b2 			; <UNDEFINED> instruction: 0x900301b2
 8006e30:	b00603ea 	andlt	r0, r6, sl, ror #7
 8006e34:	34839672 	strcc	r9, [r3], #1650	; 0x672
 8006e38:	6ca40201 	sfmvs	f0, 4, [r4], #4
 8006e3c:	0430810b 	ldrteq	r8, [r0], #-267	; 0xfffffef5
 8006e40:	025311b4 	subseq	r1, r3, #180, 2	; 0x2d
 8006e44:	e952b005 	ldmdb	r2, {r0, r2, ip, sp, pc}^
 8006e48:	81067794 			; <UNDEFINED> instruction: 0x81067794
 8006e4c:	32be0535 	adcscc	r0, lr, #222298112	; 0xd400000
 8006e50:	0019f4d0 			; <UNDEFINED> instruction: 0x0019f4d0

08006e54 <menuLineNum>:
 8006e54:	00000004 	andeq	r0, r0, r4
 8006e58:	6576654c 	ldrbvs	r6, [r6, #-1356]!	; 0xfffffab4
 8006e5c:	4f00206c 	svcmi	0x0000206c
 8006e60:	6f66206e 	svcvs	0x0066206e
 8006e64:	30332072 	eorscc	r2, r3, r2, ror r0
 8006e68:	63657320 	cmnvs	r5, #32, 6	; 0x80000000
 8006e6c:	73646e6f 	cmnvc	r4, #1776	; 0x6f0
 8006e70:	206e4f00 	rsbcs	r4, lr, r0, lsl #30
 8006e74:	20726f66 	rsbscs	r6, r2, r6, ror #30
 8006e78:	696d2031 	stmdbvs	sp!, {r0, r4, r5, sp}^
 8006e7c:	6574756e 	ldrbvs	r7, [r4, #-1390]!	; 0xfffffa92
 8006e80:	206e4f00 	rsbcs	r4, lr, r0, lsl #30
 8006e84:	20726f66 	rsbscs	r6, r2, r6, ror #30
 8006e88:	696d2032 	stmdbvs	sp!, {r1, r4, r5, sp}^
 8006e8c:	6574756e 	ldrbvs	r7, [r4, #-1390]!	; 0xfffffa92
 8006e90:	6e4f0073 	mcrvs	0, 2, r0, cr15, cr3, {3}
 8006e94:	726f6620 	rsbvc	r6, pc, #32, 12	; 0x2000000
 8006e98:	6d203520 	cfstr32vs	mvfx3, [r0, #-128]!	; 0xffffff80
 8006e9c:	74756e69 	ldrbtvc	r6, [r5], #-3689	; 0xfffff197
 8006ea0:	50007365 	andpl	r7, r0, r5, ror #6
 8006ea4:	65736c75 	ldrbvs	r6, [r3, #-3189]!	; 0xfffff38b
 8006ea8:	616c6620 	cmnvs	ip, r0, lsr #12
 8006eac:	73656873 	cmnvc	r5, #7536640	; 0x730000
 8006eb0:	776c4100 	strbvc	r4, [ip, -r0, lsl #2]!
 8006eb4:	20737961 	rsbscs	r7, r3, r1, ror #18
 8006eb8:	4c006e6f 	stcmi	14, cr6, [r0], {111}	; 0x6f
 8006ebc:	4d00776f 	stcmi	7, cr7, [r0, #-444]	; 0xfffffe44
 8006ec0:	75696465 	strbvc	r6, [r9, #-1125]!	; 0xfffffb9b
 8006ec4:	6948006d 	stmdbvs	r8, {r0, r2, r3, r5, r6}^
 8006ec8:	56006867 	strpl	r6, [r0], -r7, ror #16
 8006ecc:	20797265 	rsbscs	r7, r9, r5, ror #4
 8006ed0:	68676968 	stmdavs	r7!, {r3, r5, r6, r8, fp, sp, lr}^
 8006ed4:	73694400 	cmnvc	r9, #0, 8
 8006ed8:	79616c70 	stmdbvc	r1!, {r4, r5, r6, sl, fp, sp, lr}^
 8006edc:	6e6f4300 	cdpvs	3, 6, cr4, cr15, cr0, {0}
 8006ee0:	73617274 	cmnvc	r1, #116, 4	; 0x40000007
 8006ee4:	72420074 	subvc	r0, r2, #116	; 0x74
 8006ee8:	74686769 	strbtvc	r6, [r8], #-1897	; 0xfffff897
 8006eec:	7373656e 	cmnvc	r3, #461373440	; 0x1b800000
 8006ef0:	76656c20 	strbtvc	r6, [r5], -r0, lsr #24
 8006ef4:	42006c65 	andmi	r6, r0, #25856	; 0x6500
 8006ef8:	6c6b6361 	stclvs	3, cr6, [fp], #-388	; 0xfffffe7c
 8006efc:	74686769 	strbtvc	r6, [r8], #-1897	; 0xfffff897
 8006f00:	00000000 	andeq	r0, r0, r0

08006f04 <buzzerTimerValues>:
 8006f04:	00000000 	andeq	r0, r0, r0
 8006f08:	00000001 	andeq	r0, r0, r1
 8006f0c:	0000000f 	andeq	r0, r0, pc

08006f10 <displayTimerValues>:
 8006f10:	00000001 	andeq	r0, r0, r1
 8006f14:	00007530 	andeq	r7, r0, r0, lsr r5
 8006f18:	0000ea60 	andeq	lr, r0, r0, ror #20
 8006f1c:	0001d4c0 	andeq	sp, r1, r0, asr #9
 8006f20:	000493e0 	andeq	r9, r4, r0, ror #7
 8006f24:	00000001 	andeq	r0, r0, r1
 8006f28:	ffffffff 			; <UNDEFINED> instruction: 0xffffffff

08006f2c <gameContinueMenuOptions>:
 8006f2c:	08006f9c 	stmdaeq	r0, {r2, r3, r4, r7, r8, r9, sl, fp, sp, lr}
 8006f30:	08006faa 	stmdaeq	r0, {r1, r3, r5, r7, r8, r9, sl, fp, sp, lr}
 8006f34:	08006fb3 	stmdaeq	r0, {r0, r1, r4, r5, r7, r8, r9, sl, fp, sp, lr}
 8006f38:	00000000 	andeq	r0, r0, r0

08006f3c <gameMenu>:
 8006f3c:	08006fd2 	stmdaeq	r0, {r1, r4, r6, r7, r8, r9, sl, fp, sp, lr}
 8006f40:	200002f0 	strdcs	r0, [r0], -r0	; <UNPREDICTABLE>
 8006f44:	08002125 	stmdaeq	r0, {r0, r2, r5, r8, sp}
 8006f48:	080024e5 	stmdaeq	r0, {r0, r2, r5, r6, r7, sl, sp}
 8006f4c:	08004029 	stmdaeq	r0, {r0, r3, r5, lr}

08006f50 <gameMenuView>:
 8006f50:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8006f54:	08006f3c 	stmdaeq	r0, {r2, r3, r4, r5, r8, r9, sl, fp, sp, lr}

08006f58 <gameStartMenuOptions>:
 8006f58:	08006fbc 	stmdaeq	r0, {r2, r3, r4, r5, r7, r8, r9, sl, fp, sp, lr}
 8006f5c:	08006fc7 	stmdaeq	r0, {r0, r1, r2, r6, r7, r8, r9, sl, fp, sp, lr}
 8006f60:	08006fb3 	stmdaeq	r0, {r0, r1, r4, r5, r7, r8, r9, sl, fp, sp, lr}
 8006f64:	00000000 	andeq	r0, r0, r0

08006f68 <gameStrengthMenu>:
 8006f68:	08006fb3 	stmdaeq	r0, {r0, r1, r4, r5, r7, r8, r9, sl, fp, sp, lr}
 8006f6c:	200002f4 	strdcs	r0, [r0], -r4
 8006f70:	08002169 	stmdaeq	r0, {r0, r3, r5, r6, r8, sp}
 8006f74:	08002149 	stmdaeq	r0, {r0, r3, r6, r8, sp}
 8006f78:	0800215d 	stmdaeq	r0, {r0, r2, r3, r4, r6, r8, sp}

08006f7c <gameStrengthMenuView>:
 8006f7c:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8006f80:	08006f68 	stmdaeq	r0, {r3, r5, r6, r8, r9, sl, fp, sp, lr}

08006f84 <gameStrengthToNodesCount>:
 8006f84:	01000001 	tsteq	r0, r1
 8006f88:	04000200 	streq	r0, [r0], #-512	; 0xfffffe00
 8006f8c:	10000800 	andne	r0, r0, r0, lsl #16
 8006f90:	40002000 	andmi	r2, r0, r0

08006f94 <gameView>:
 8006f94:	0800233d 	stmdaeq	r0, {r0, r2, r3, r4, r5, r8, r9, sp}
 8006f98:	00000000 	andeq	r0, r0, r0
 8006f9c:	746e6f43 	strbtvc	r6, [lr], #-3907	; 0xfffff0bd
 8006fa0:	65756e69 	ldrbvs	r6, [r5, #-3689]!	; 0xfffff197
 8006fa4:	6d616720 	stclvs	7, cr6, [r1, #-128]!	; 0xffffff80
 8006fa8:	654e0065 	strbvs	r0, [lr, #-101]	; 0xffffff9b
 8006fac:	61672077 	smcvs	29191	; 0x7207
 8006fb0:	5300656d 	movwpl	r6, #1389	; 0x56d
 8006fb4:	6e657274 	mcrvs	2, 3, r7, cr5, cr4, {3}
 8006fb8:	00687467 	rsbeq	r7, r8, r7, ror #8
 8006fbc:	79616c50 	stmdbvc	r1!, {r4, r6, sl, fp, sp, lr}^
 8006fc0:	69687720 	stmdbvs	r8!, {r5, r8, r9, sl, ip, sp, lr}^
 8006fc4:	50006574 	andpl	r6, r0, r4, ror r5
 8006fc8:	2079616c 	rsbscs	r6, r9, ip, ror #2
 8006fcc:	63616c62 	cmnvs	r1, #25088	; 0x6200
 8006fd0:	6147006b 	cmpvs	r7, fp, rrx
 8006fd4:	4600656d 	strmi	r6, [r0], -sp, ror #10
 8006fd8:	776d7269 	strbvc	r7, [sp, -r9, ror #4]!
 8006fdc:	20657261 	rsbcs	r7, r5, r1, ror #4
 8006fe0:	63656863 	cmnvs	r5, #6488064	; 0x630000
 8006fe4:	6d75736b 	ldclvs	3, cr7, [r5, #-428]!	; 0xfffffe54
 8006fe8:	69616620 	stmdbvs	r1!, {r5, r9, sl, sp, lr}^
 8006fec:	6572756c 	ldrbvs	r7, [r2, #-1388]!	; 0xfffffa94
 8006ff0:	4157002e 	cmpmi	r7, lr, lsr #32
 8006ff4:	4e494e52 	mcrmi	14, 2, r4, cr9, cr2, {2}
 8006ff8:	61520047 	cmpvs	r2, r7, asr #32
 8006ffc:	72502064 	subsvc	r2, r0, #100	; 0x64
 8007000:	0000006f 	andeq	r0, r0, pc, rrx

08007004 <averageRateView>:
 8007004:	08003255 	stmdaeq	r0, {r0, r2, r4, r6, r9, ip, sp}
 8007008:	00000000 	andeq	r0, r0, r0

0800700c <averageTimerMenu>:
 800700c:	08007190 	stmdaeq	r0, {r4, r7, r8, ip, sp, lr}
 8007010:	20000320 	andcs	r0, r0, r0, lsr #6
 8007014:	080028b5 	stmdaeq	r0, {r0, r2, r4, r5, r7, fp, sp}
 8007018:	080028d5 	stmdaeq	r0, {r0, r2, r4, r6, r7, fp, sp}
 800701c:	08004029 	stmdaeq	r0, {r0, r3, r5, lr}

08007020 <averageTimerMenuOptions>:
 8007020:	08006184 	stmdaeq	r0, {r2, r7, r8, sp, lr}
 8007024:	08006e99 	stmdaeq	r0, {r0, r3, r4, r7, r9, sl, fp, sp, lr}
 8007028:	080063cd 	stmdaeq	r0, {r0, r2, r3, r6, r7, r8, r9, sp, lr}
 800702c:	080063bc 	stmdaeq	r0, {r2, r3, r4, r5, r7, r8, r9, sp, lr}
 8007030:	080063ab 	stmdaeq	r0, {r0, r1, r3, r5, r7, r8, r9, sp, lr}
 8007034:	00000000 	andeq	r0, r0, r0

08007038 <averageTimerMenuView>:
 8007038:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 800703c:	0800700c 	stmdaeq	r0, {r2, r3, ip, sp, lr}

08007040 <averageTimerTimes>:
 8007040:	00278d00 	eoreq	r8, r7, r0, lsl #26
 8007044:	0000012c 	andeq	r0, r0, ip, lsr #2
 8007048:	00000258 	andeq	r0, r0, r8, asr r2
 800704c:	00000708 	andeq	r0, r0, r8, lsl #14
 8007050:	00000e10 	andeq	r0, r0, r0, lsl lr
 8007054:	7f7f2e7f 	svcvc	0x007f2e7f
 8007058:	0000007f 	andeq	r0, r0, pc, ror r0

0800705c <doseAlarmMenu>:
 800705c:	080070f6 	stmdaeq	r0, {r1, r2, r4, r5, r6, r7, ip, sp, lr}
 8007060:	20000324 	andcs	r0, r0, r4, lsr #6
 8007064:	080029f5 	stmdaeq	r0, {r0, r2, r4, r5, r6, r7, r8, fp, sp}
 8007068:	080028fd 	stmdaeq	r0, {r0, r2, r3, r4, r5, r6, r7, fp, sp}
 800706c:	08004029 	stmdaeq	r0, {r0, r3, r5, lr}

08007070 <doseAlarmMenuView>:
 8007070:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8007074:	0800705c 	stmdaeq	r0, {r2, r3, r4, r6, ip, sp, lr}

08007078 <doseAlarmsSv>:
 8007078:	00000000 	andeq	r0, r0, r0
 800707c:	360637bd 			; <UNDEFINED> instruction: 0x360637bd
 8007080:	36a7c5ac 	strtcc	ip, [r7], ip, lsr #11
 8007084:	3727c5ac 	strcc	ip, [r7, -ip, lsr #11]!
 8007088:	37a7c5ac 	strcc	ip, [r7, ip, lsr #11]!
 800708c:	3851b717 	ldmdacc	r1, {r0, r1, r2, r4, r8, r9, sl, ip, sp, pc}^
 8007090:	38d1b717 	ldmcc	r1, {r0, r1, r2, r4, r8, r9, sl, ip, sp, pc}^
 8007094:	3951b717 	ldmdbcc	r1, {r0, r1, r2, r4, r8, r9, sl, ip, sp, pc}^
 8007098:	3a03126f 	bcc	80cba5c <_sidata+0xc420c>
 800709c:	3a83126f 	bcc	60cba60 <_Min_Stack_Size+0x60cb660>

080070a0 <doseView>:
 80070a0:	08003385 	stmdaeq	r0, {r0, r2, r7, r8, r9, ip, sp}
 80070a4:	00000000 	andeq	r0, r0, r0

080070a8 <histories>:
 80070a8:	080071a4 	stmdaeq	r0, {r2, r5, r7, r8, ip, sp, lr}
 80070ac:	000a0005 	andeq	r0, sl, r5
 80070b0:	080071b3 	stmdaeq	r0, {r0, r1, r4, r5, r7, r8, ip, sp, lr}
 80070b4:	0006001e 	andeq	r0, r6, lr, lsl r0
 80070b8:	080071c1 	stmdaeq	r0, {r0, r6, r7, r8, ip, sp, lr}
 80070bc:	000602d0 	ldrdeq	r0, [r6], -r0	; <UNPREDICTABLE>

080070c0 <historyView>:
 80070c0:	08003485 	stmdaeq	r0, {r0, r2, r7, sl, ip, sp}
 80070c4:	00000000 	andeq	r0, r0, r0

080070c8 <instantaneousRateView>:
 80070c8:	08003105 	stmdaeq	r0, {r0, r2, r8, ip, sp}
 80070cc:	00000000 	andeq	r0, r0, r0

080070d0 <measurementViews>:
 80070d0:	080070c8 	stmdaeq	r0, {r3, r6, r7, ip, sp, lr}
 80070d4:	08007004 	stmdaeq	r0, {r2, ip, sp, lr}
 80070d8:	080070a0 	stmdaeq	r0, {r5, r7, ip, sp, lr}
 80070dc:	080070c0 	stmdaeq	r0, {r6, r7, ip, sp, lr}
 80070e0:	656e6f44 	strbvs	r6, [lr, #-3908]!	; 0xfffff0bc
 80070e4:	65764f00 	ldrbvs	r4, [r6, #-3840]!	; 0xfffff100
 80070e8:	6f6c6672 	svcvs	0x006c6672
 80070ec:	76410077 			; <UNDEFINED> instruction: 0x76410077
 80070f0:	67617265 	strbvs	r7, [r1, -r5, ror #4]!
 80070f4:	6f440065 	svcvs	0x00440065
 80070f8:	61206573 			; <UNDEFINED> instruction: 0x61206573
 80070fc:	6d72616c 	ldfvse	f6, [r2, #-432]!	; 0xfffffe50
 8007100:	736f4400 	cmnvc	pc, #0, 8
 8007104:	6f480065 	svcvs	0x00480065
 8007108:	5200646c 	andpl	r6, r0, #108, 8	; 0x6c000000
 800710c:	20657461 	rsbcs	r7, r5, r1, ror #8
 8007110:	72616c61 	rsbvc	r6, r1, #24832	; 0x6100
 8007114:	614d006d 	cmpvs	sp, sp, rrx
 8007118:	6e490078 	mcrvs	0, 2, r0, cr9, cr8, {3}
 800711c:	6e617473 	mcrvs	4, 3, r7, cr1, cr3, {3}
 8007120:	656e6174 	strbvs	r6, [lr, #-372]!	; 0xfffffe8c
 8007124:	0073756f 	rsbseq	r7, r3, pc, ror #10

08007128 <rateAlarmMenu>:
 8007128:	0800710b 	stmdaeq	r0, {r0, r1, r3, r8, ip, sp, lr}
 800712c:	200006d8 	ldrdcs	r0, [r0], -r8
 8007130:	08002a75 	stmdaeq	r0, {r0, r2, r4, r5, r6, r9, fp, sp}
 8007134:	080028e9 	stmdaeq	r0, {r0, r3, r5, r6, r7, fp, sp}
 8007138:	08004029 	stmdaeq	r0, {r0, r3, r5, lr}

0800713c <rateAlarmMenuView>:
 800713c:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8007140:	08007128 	stmdaeq	r0, {r3, r5, r8, ip, sp, lr}

08007144 <rateAlarmsSvH>:
 8007144:	00000000 	andeq	r0, r0, r0
 8007148:	3456bf95 	ldrbcc	fp, [r6], #-3989	; 0xfffff06b
 800714c:	350637bd 	strcc	r3, [r6, #-1981]	; 0xfffff843
 8007150:	358637bd 	strcc	r3, [r6, #1981]	; 0x7bd
 8007154:	360637bd 			; <UNDEFINED> instruction: 0x360637bd
 8007158:	36a7c5ac 	strtcc	ip, [r7], ip, lsr #11
 800715c:	3727c5ac 	strcc	ip, [r7, -ip, lsr #11]!
 8007160:	37a7c5ac 	strcc	ip, [r7, ip, lsr #11]!
 8007164:	3851b717 	ldmdacc	r1, {r0, r1, r2, r4, r8, r9, sl, ip, sp, pc}^
 8007168:	38d1b717 	ldmcc	r1, {r0, r1, r2, r4, r8, r9, sl, ip, sp, pc}^
 800716c:	76656953 			; <UNDEFINED> instruction: 0x76656953
 8007170:	00747265 	rsbseq	r7, r4, r5, ror #4
 8007174:	006d6572 	rsbeq	r6, sp, r2, ror r5
 8007178:	006d7063 	rsbeq	r7, sp, r3, rrx
 800717c:	00737063 	rsbseq	r7, r3, r3, rrx
 8007180:	72007653 	andvc	r7, r0, #87031808	; 0x5300000
 8007184:	682f6d65 	stmdavs	pc!, {r0, r2, r5, r6, r8, sl, fp, sp, lr}	; <UNPREDICTABLE>
 8007188:	756f6300 	strbvc	r6, [pc, #-768]!	; 8006e90 <menuLineNum+0x3c>
 800718c:	0073746e 	rsbseq	r7, r3, lr, ror #8
 8007190:	72657641 	rsbvc	r7, r5, #68157440	; 0x4100000
 8007194:	20656761 	rsbcs	r6, r5, r1, ror #14
 8007198:	656d6974 	strbvs	r6, [sp, #-2420]!	; 0xfffff68c
 800719c:	6e550072 	mrcvs	0, 2, r0, cr5, cr2, {3}
 80071a0:	00737469 	rsbseq	r7, r3, r9, ror #8
 80071a4:	74736948 	ldrbtvc	r6, [r3], #-2376	; 0xfffff6b8
 80071a8:	2079726f 	rsbscs	r7, r9, pc, ror #4
 80071ac:	20303128 	eorscs	r3, r0, r8, lsr #2
 80071b0:	4800296d 	stmdami	r0, {r0, r2, r3, r5, r6, r8, fp, sp}
 80071b4:	6f747369 	svcvs	0x00747369
 80071b8:	28207972 	stmdacs	r0!, {r1, r4, r5, r6, r8, fp, ip, sp, lr}
 80071bc:	29682031 	stmdbcs	r8!, {r0, r4, r5, sp}^
 80071c0:	73694800 	cmnvc	r9, #0, 16
 80071c4:	79726f74 	ldmdbvc	r2!, {r2, r4, r5, r6, r8, r9, sl, fp, sp, lr}^
 80071c8:	34322820 	ldrtcc	r2, [r2], #-2080	; 0xfffff7e0
 80071cc:	00296820 	eoreq	r6, r9, r0, lsr #16

080071d0 <unitsMenu>:
 80071d0:	0800719e 	stmdaeq	r0, {r1, r2, r3, r4, r7, r8, ip, sp, lr}
 80071d4:	200006dc 	ldrdcs	r0, [r0], -ip
 80071d8:	08002881 	stmdaeq	r0, {r0, r7, fp, sp}
 80071dc:	080028a1 	stmdaeq	r0, {r0, r5, r7, fp, sp}
 80071e0:	08004029 	stmdaeq	r0, {r0, r3, r5, lr}

080071e4 <unitsMenuOptions>:
 80071e4:	0800716c 	stmdaeq	r0, {r2, r3, r5, r6, r8, ip, sp, lr}
 80071e8:	08007174 	stmdaeq	r0, {r2, r4, r5, r6, r8, ip, sp, lr}
 80071ec:	08007178 	stmdaeq	r0, {r3, r4, r5, r6, r8, ip, sp, lr}
 80071f0:	0800717c 	stmdaeq	r0, {r2, r3, r4, r5, r6, r8, ip, sp, lr}
 80071f4:	00000000 	andeq	r0, r0, r0

080071f8 <unitsMenuView>:
 80071f8:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 80071fc:	080071d0 	stmdaeq	r0, {r4, r6, r7, r8, ip, sp, lr}

08007200 <batteryLevelThresholds>:
 8007200:	3f9a1cac 	svccc	0x009a1cac
 8007204:	3f9eb852 	svccc	0x009eb852
 8007208:	3fa16873 	svccc	0x00a16873
 800720c:	3fa353f8 	svccc	0x00a353f8
 8007210:	3fa7ae14 	svccc	0x00a7ae14
 8007214:	3f97ced9 	svccc	0x0097ced9
 8007218:	3fa147ae 	svccc	0x00a147ae
 800721c:	3fa5e354 	svccc	0x00a5e354
 8007220:	3fab4396 	svccc	0x00ab4396
 8007224:	3fb28f5c 	svccc	0x00b28f5c

08007228 <batteryTypeMenu>:
 8007228:	0800725f 	stmdaeq	r0, {r0, r1, r2, r3, r4, r6, r9, ip, sp, lr}
 800722c:	200006e0 	andcs	r0, r0, r0, ror #13
 8007230:	08003739 	stmdaeq	r0, {r0, r3, r4, r5, r8, r9, sl, ip, sp}
 8007234:	08003759 	stmdaeq	r0, {r0, r3, r4, r6, r8, r9, sl, ip, sp}
 8007238:	08004029 	stmdaeq	r0, {r0, r3, r5, lr}

0800723c <batteryTypeMenuOptions>:
 800723c:	08007250 	stmdaeq	r0, {r4, r6, r9, ip, sp, lr}
 8007240:	08007256 	stmdaeq	r0, {r1, r2, r4, r6, r9, ip, sp, lr}
 8007244:	00000000 	andeq	r0, r0, r0

08007248 <batteryTypeMenuView>:
 8007248:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 800724c:	08007228 	stmdaeq	r0, {r3, r5, r9, ip, sp, lr}
 8007250:	4d2d694e 			; <UNDEFINED> instruction: 0x4d2d694e
 8007254:	6c410048 	mcrrvs	0, 4, r0, r1, cr8
 8007258:	696c616b 	stmdbvs	ip!, {r0, r1, r3, r5, r6, r8, sp, lr}^
 800725c:	4200656e 	andmi	r6, r0, #461373440	; 0x1b800000
 8007260:	65747461 	ldrbvs	r7, [r4, #-1121]!	; 0xfffffb9f
 8007264:	74207972 	strtvc	r7, [r0], #-2418	; 0xfffff68e
 8007268:	00657079 	rsbeq	r7, r5, r9, ror r0

0800726c <rngActivitySubtitles>:
 800726c:	080072e7 	stmdaeq	r0, {r0, r1, r2, r5, r6, r7, r9, ip, sp, lr}
 8007270:	080072e6 	stmdaeq	r0, {r1, r2, r5, r6, r7, r9, ip, sp, lr}
 8007274:	080072e5 	stmdaeq	r0, {r0, r2, r5, r6, r7, r9, ip, sp, lr}

08007278 <rngMenu>:
 8007278:	080072d4 	stmdaeq	r0, {r2, r4, r6, r7, r9, ip, sp, lr}
 800727c:	2000072c 	andcs	r0, r0, ip, lsr #14
 8007280:	08003835 	stmdaeq	r0, {r0, r2, r4, r5, fp, ip, sp}
 8007284:	08003845 	stmdaeq	r0, {r0, r2, r6, fp, ip, sp}
 8007288:	08004029 	stmdaeq	r0, {r0, r3, r5, lr}

0800728c <rngMenuView>:
 800728c:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8007290:	08007278 	stmdaeq	r0, {r3, r4, r5, r6, r9, ip, sp, lr}

08007294 <rngModeMenuOptions>:
 8007294:	080072e9 	stmdaeq	r0, {r0, r3, r5, r6, r7, r9, ip, sp, lr}
 8007298:	080072f6 	stmdaeq	r0, {r1, r2, r4, r5, r6, r7, r9, ip, sp, lr}
 800729c:	08007301 	stmdaeq	r0, {r0, r8, r9, ip, sp, lr}
 80072a0:	0800730d 	stmdaeq	r0, {r0, r2, r3, r8, r9, ip, sp, lr}
 80072a4:	08007315 	stmdaeq	r0, {r0, r2, r4, r8, r9, ip, sp, lr}
 80072a8:	08007323 	stmdaeq	r0, {r0, r1, r5, r8, r9, ip, sp, lr}
 80072ac:	08007331 	stmdaeq	r0, {r0, r4, r5, r8, r9, ip, sp, lr}
 80072b0:	0800733e 	stmdaeq	r0, {r1, r2, r3, r4, r5, r8, r9, ip, sp, lr}
 80072b4:	0800734b 	stmdaeq	r0, {r0, r1, r3, r6, r8, r9, ip, sp, lr}
 80072b8:	08007358 	stmdaeq	r0, {r3, r4, r6, r8, r9, ip, sp, lr}
 80072bc:	00000000 	andeq	r0, r0, r0

080072c0 <rngModeRanges>:
 80072c0:	0a105e3e 	beq	841ebc0 <_sidata+0x417370>
 80072c4:	06080c14 			; <UNDEFINED> instruction: 0x06080c14
 80072c8:	00000204 	andeq	r0, r0, r4, lsl #4

080072cc <rngView>:
 80072cc:	08003889 	stmdaeq	r0, {r0, r3, r7, fp, ip, sp}
 80072d0:	00000000 	andeq	r0, r0, r0
 80072d4:	646e6152 	strbtvs	r6, [lr], #-338	; 0xfffffeae
 80072d8:	67206d6f 	strvs	r6, [r0, -pc, ror #26]!
 80072dc:	72656e65 	rsbvc	r6, r5, #1616	; 0x650
 80072e0:	726f7461 	rsbvc	r7, pc, #1627389952	; 0x61000000
 80072e4:	2e2e2e00 	cdpcs	14, 2, cr2, cr14, cr0, {0}
 80072e8:	706c4100 	rsbvc	r4, ip, r0, lsl #2
 80072ec:	756e6168 	strbvc	r6, [lr, #-360]!	; 0xfffffe98
 80072f0:	6972656d 	ldmdbvs	r2!, {r0, r2, r3, r5, r6, r8, sl, sp, lr}^
 80072f4:	75460063 	strbvc	r0, [r6, #-99]	; 0xffffff9d
 80072f8:	41206c6c 			; <UNDEFINED> instruction: 0x41206c6c
 80072fc:	49494353 	stmdbmi	r9, {r0, r1, r4, r6, r8, r9, lr}^
 8007300:	78654800 	stmdavc	r5!, {fp, lr}^
 8007304:	63656461 	cmnvs	r5, #1627389952	; 0x61000000
 8007308:	6c616d69 	stclvs	13, cr6, [r1], #-420	; 0xfffffe5c
 800730c:	63654400 	cmnvs	r5, #0, 8
 8007310:	6c616d69 	stclvs	13, cr6, [r1], #-420	; 0xfffffe5c
 8007314:	2d303200 	lfmcs	f3, 4, [r0, #-0]
 8007318:	65646973 	strbvs	r6, [r4, #-2419]!	; 0xfffff68d
 800731c:	69642064 	stmdbvs	r4!, {r2, r5, r6, sp}^
 8007320:	31006563 	tstcc	r0, r3, ror #10
 8007324:	69732d32 	ldmdbvs	r3!, {r1, r4, r5, r8, sl, fp, sp}^
 8007328:	20646564 	rsbcs	r6, r4, r4, ror #10
 800732c:	65636964 	strbvs	r6, [r3, #-2404]!	; 0xfffff69c
 8007330:	732d3800 			; <UNDEFINED> instruction: 0x732d3800
 8007334:	64656469 	strbtvs	r6, [r5], #-1129	; 0xfffffb97
 8007338:	63696420 	cmnvs	r9, #32, 8	; 0x20000000
 800733c:	2d360065 	ldccs	0, cr0, [r6, #-404]!	; 0xfffffe6c
 8007340:	65646973 	strbvs	r6, [r4, #-2419]!	; 0xfffff68d
 8007344:	69642064 	stmdbvs	r4!, {r2, r5, r6, sp}^
 8007348:	34006563 	strcc	r6, [r0], #-1379	; 0xfffffa9d
 800734c:	6469732d 	strbtvs	r7, [r9], #-813	; 0xfffffcd3
 8007350:	64206465 	strtvs	r6, [r0], #-1125	; 0xfffffb9b
 8007354:	00656369 	rsbeq	r6, r5, r9, ror #6
 8007358:	6e696f43 	cdpvs	15, 6, cr6, cr9, cr3, {2}
 800735c:	696c6620 	stmdbvs	ip!, {r5, r9, sl, sp, lr}^
 8007360:	00000070 	andeq	r0, r0, r0, ror r0

08007364 <dateAndTimeMenuView>:
 8007364:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8007368:	080073bc 	stmdaeq	r0, {r2, r3, r4, r5, r7, r8, r9, ip, sp, lr}

0800736c <daysInMonth>:
 800736c:	1e1f1c1f 	mrcne	12, 0, r1, cr15, cr15, {0}
 8007370:	1f1f1e1f 	svcne	0x001f1e1f
 8007374:	1f1e1f1e 	svcne	0x001e1f1e
 8007378:	2d435455 	cfstrdcs	mvd5, [r3, #-340]	; 0xfffffeac
 800737c:	43545500 	cmpmi	r4, #0, 10
 8007380:	0000002b 	andeq	r0, r0, fp, lsr #32

08007384 <rtcDayMenu>:
 8007384:	080074a7 	stmdaeq	r0, {r0, r1, r2, r5, r7, sl, ip, sp, lr}
 8007388:	20000738 	andcs	r0, r0, r8, lsr r7
 800738c:	08003b15 	stmdaeq	r0, {r0, r2, r4, r8, r9, fp, ip, sp}
 8007390:	08003d55 	stmdaeq	r0, {r0, r2, r4, r6, r8, sl, fp, ip, sp}
 8007394:	08003a75 	stmdaeq	r0, {r0, r2, r4, r5, r6, r9, fp, ip, sp}

08007398 <rtcDayMenuView>:
 8007398:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 800739c:	08007384 	stmdaeq	r0, {r2, r7, r8, r9, ip, sp, lr}

080073a0 <rtcHourMenu>:
 80073a0:	080074ab 	stmdaeq	r0, {r0, r1, r3, r5, r7, sl, ip, sp, lr}
 80073a4:	20000738 	andcs	r0, r0, r8, lsr r7
 80073a8:	08003b15 	stmdaeq	r0, {r0, r2, r4, r8, r9, fp, ip, sp}
 80073ac:	08003d55 	stmdaeq	r0, {r0, r2, r4, r6, r8, sl, fp, ip, sp}
 80073b0:	08003a75 	stmdaeq	r0, {r0, r2, r4, r5, r6, r9, fp, ip, sp}

080073b4 <rtcHourMenuView>:
 80073b4:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 80073b8:	080073a0 	stmdaeq	r0, {r5, r7, r8, r9, ip, sp, lr}

080073bc <rtcMenu>:
 80073bc:	08007484 	stmdaeq	r0, {r2, r7, sl, ip, sp, lr}
 80073c0:	2000073c 	andcs	r0, r0, ip, lsr r7
 80073c4:	08003a65 	stmdaeq	r0, {r0, r2, r5, r6, r9, fp, ip, sp}
 80073c8:	08003df1 	stmdaeq	r0, {r0, r4, r5, r6, r7, r8, sl, fp, ip, sp}
 80073cc:	08004029 	stmdaeq	r0, {r0, r3, r5, lr}

080073d0 <rtcMenuOptionSettings>:
 80073d0:	000007e4 	andeq	r0, r0, r4, ror #15
 80073d4:	00000050 	andeq	r0, r0, r0, asr r0
 80073d8:	00000001 	andeq	r0, r0, r1
 80073dc:	0000000c 	andeq	r0, r0, ip
 80073e0:	00000001 	andeq	r0, r0, r1
 80073e4:	0000001f 	andeq	r0, r0, pc, lsl r0
 80073e8:	00000000 	andeq	r0, r0, r0
 80073ec:	00000018 	andeq	r0, r0, r8, lsl r0
 80073f0:	00000000 	andeq	r0, r0, r0
 80073f4:	0000003c 	andeq	r0, r0, ip, lsr r0

080073f8 <rtcMenuOptions>:
 80073f8:	08007492 	stmdaeq	r0, {r1, r4, r7, sl, ip, sp, lr}
 80073fc:	0800749c 	stmdaeq	r0, {r2, r3, r4, r7, sl, ip, sp, lr}
 8007400:	080074a1 	stmdaeq	r0, {r0, r5, r7, sl, ip, sp, lr}
 8007404:	080074a7 	stmdaeq	r0, {r0, r1, r2, r5, r7, sl, ip, sp, lr}
 8007408:	080074ab 	stmdaeq	r0, {r0, r1, r3, r5, r7, sl, ip, sp, lr}
 800740c:	080074b0 	stmdaeq	r0, {r4, r5, r7, sl, ip, sp, lr}
 8007410:	00000000 	andeq	r0, r0, r0

08007414 <rtcMinuteMenu>:
 8007414:	080074b0 	stmdaeq	r0, {r4, r5, r7, sl, ip, sp, lr}
 8007418:	20000738 	andcs	r0, r0, r8, lsr r7
 800741c:	08003b15 	stmdaeq	r0, {r0, r2, r4, r8, r9, fp, ip, sp}
 8007420:	08003d55 	stmdaeq	r0, {r0, r2, r4, r6, r8, sl, fp, ip, sp}
 8007424:	08003a75 	stmdaeq	r0, {r0, r2, r4, r5, r6, r9, fp, ip, sp}

08007428 <rtcMinuteMenuView>:
 8007428:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 800742c:	08007414 	stmdaeq	r0, {r2, r4, sl, ip, sp, lr}

08007430 <rtcMonthMenu>:
 8007430:	080074a1 	stmdaeq	r0, {r0, r5, r7, sl, ip, sp, lr}
 8007434:	20000738 	andcs	r0, r0, r8, lsr r7
 8007438:	08003b15 	stmdaeq	r0, {r0, r2, r4, r8, r9, fp, ip, sp}
 800743c:	08003d55 	stmdaeq	r0, {r0, r2, r4, r6, r8, sl, fp, ip, sp}
 8007440:	08003a75 	stmdaeq	r0, {r0, r2, r4, r5, r6, r9, fp, ip, sp}

08007444 <rtcMonthMenuView>:
 8007444:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8007448:	08007430 	stmdaeq	r0, {r4, r5, sl, ip, sp, lr}

0800744c <rtcTimeZoneMenu>:
 800744c:	08007492 	stmdaeq	r0, {r1, r4, r7, sl, ip, sp, lr}
 8007450:	20000738 	andcs	r0, r0, r8, lsr r7
 8007454:	08003a81 	stmdaeq	r0, {r0, r7, r9, fp, ip, sp}
 8007458:	08003a51 	stmdaeq	r0, {r0, r4, r6, r9, fp, ip, sp}
 800745c:	08003a75 	stmdaeq	r0, {r0, r2, r4, r5, r6, r9, fp, ip, sp}

08007460 <rtcTimeZoneMenuView>:
 8007460:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8007464:	0800744c 	stmdaeq	r0, {r2, r3, r6, sl, ip, sp, lr}

08007468 <rtcYearMenu>:
 8007468:	0800749c 	stmdaeq	r0, {r2, r3, r4, r7, sl, ip, sp, lr}
 800746c:	20000738 	andcs	r0, r0, r8, lsr r7
 8007470:	08003b15 	stmdaeq	r0, {r0, r2, r4, r8, r9, fp, ip, sp}
 8007474:	08003d55 	stmdaeq	r0, {r0, r2, r4, r6, r8, sl, fp, ip, sp}
 8007478:	08003a75 	stmdaeq	r0, {r0, r2, r4, r5, r6, r9, fp, ip, sp}

0800747c <rtcYearMenuView>:
 800747c:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8007480:	08007468 	stmdaeq	r0, {r3, r5, r6, sl, ip, sp, lr}
 8007484:	65746144 	ldrbvs	r6, [r4, #-324]!	; 0xfffffebc
 8007488:	646e6120 	strbtvs	r6, [lr], #-288	; 0xfffffee0
 800748c:	6d697420 	cfstrdvs	mvd7, [r9, #-128]!	; 0xffffff80
 8007490:	69540065 	ldmdbvs	r4, {r0, r2, r5, r6}^
 8007494:	7a20656d 	bvc	8820a50 <_sidata+0x819200>
 8007498:	00656e6f 	rsbeq	r6, r5, pc, ror #28
 800749c:	72616559 	rsbvc	r6, r1, #373293056	; 0x16400000
 80074a0:	6e6f4d00 	cdpvs	13, 6, cr4, cr15, cr0, {0}
 80074a4:	44006874 	strmi	r6, [r0], #-2164	; 0xfffff78c
 80074a8:	48007961 	stmdami	r0, {r0, r5, r6, r8, fp, ip, sp, lr}
 80074ac:	0072756f 	rsbseq	r7, r2, pc, ror #10
 80074b0:	756e694d 	strbvc	r6, [lr, #-2381]!	; 0xfffff6b3
 80074b4:	00006574 	andeq	r6, r0, r4, ror r5

080074b8 <settingsMenu>:
 80074b8:	08007544 	stmdaeq	r0, {r2, r6, r8, sl, ip, sp, lr}
 80074bc:	20000740 	andcs	r0, r0, r0, asr #14
 80074c0:	08003ead 	stmdaeq	r0, {r0, r2, r3, r5, r7, r9, sl, fp, ip, sp}
 80074c4:	08003ebd 	stmdaeq	r0, {r0, r2, r3, r4, r5, r7, r9, sl, fp, ip, sp}
 80074c8:	08003ed1 	stmdaeq	r0, {r0, r4, r6, r7, r9, sl, fp, ip, sp}

080074cc <settingsMenuOptions>:
 80074cc:	0800719e 	stmdaeq	r0, {r1, r2, r3, r4, r7, r8, ip, sp, lr}
 80074d0:	080071f8 	stmdaeq	r0, {r3, r4, r5, r6, r7, r8, ip, sp, lr}
 80074d4:	08007190 	stmdaeq	r0, {r4, r7, r8, ip, sp, lr}
 80074d8:	08007038 	stmdaeq	r0, {r3, r4, r5, ip, sp, lr}
 80074dc:	0800710b 	stmdaeq	r0, {r0, r1, r3, r8, ip, sp, lr}
 80074e0:	0800713c 	stmdaeq	r0, {r2, r3, r4, r5, r8, ip, sp, lr}
 80074e4:	080070f6 	stmdaeq	r0, {r1, r2, r4, r5, r6, r7, ip, sp, lr}
 80074e8:	08007070 	stmdaeq	r0, {r4, r5, r6, ip, sp, lr}
 80074ec:	0800754d 	stmdaeq	r0, {r0, r2, r3, r6, r8, sl, ip, sp, lr}
 80074f0:	08007800 	stmdaeq	r0, {fp, ip, sp, lr}
 80074f4:	08006398 	stmdaeq	r0, {r3, r4, r7, r8, r9, sp, lr}
 80074f8:	08006384 	stmdaeq	r0, {r2, r7, r8, r9, sp, lr}
 80074fc:	08006193 	stmdaeq	r0, {r0, r1, r4, r7, r8, sp, lr}
 8007500:	0800617c 	stmdaeq	r0, {r2, r3, r4, r5, r6, r8, sp, lr}
 8007504:	08006ed5 	stmdaeq	r0, {r0, r2, r4, r6, r7, r9, sl, fp, sp, lr}
 8007508:	08006478 	stmdaeq	r0, {r3, r4, r5, r6, sl, sp, lr}
 800750c:	08007484 	stmdaeq	r0, {r2, r7, sl, ip, sp, lr}
 8007510:	08007364 	stmdaeq	r0, {r2, r5, r6, r8, r9, ip, sp, lr}
 8007514:	0800725f 	stmdaeq	r0, {r0, r1, r2, r3, r4, r6, r9, ip, sp, lr}
 8007518:	08007248 	stmdaeq	r0, {r3, r6, r9, ip, sp, lr}
 800751c:	080072d4 	stmdaeq	r0, {r2, r4, r6, r7, r9, ip, sp, lr}
 8007520:	0800728c 	stmdaeq	r0, {r2, r3, r7, r9, ip, sp, lr}
 8007524:	08006fd2 	stmdaeq	r0, {r1, r4, r6, r7, r8, r9, sl, fp, sp, lr}
 8007528:	08006f50 	stmdaeq	r0, {r4, r6, r8, r9, sl, fp, sp, lr}
 800752c:	080064cb 	stmdaeq	r0, {r0, r1, r3, r6, r7, sl, sp, lr}
 8007530:	0800759c 	stmdaeq	r0, {r2, r3, r4, r7, r8, sl, ip, sp, lr}
	...

0800753c <settingsMenuView>:
 800753c:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8007540:	080074b8 	stmdaeq	r0, {r3, r4, r5, r7, sl, ip, sp, lr}
 8007544:	74746553 	ldrbtvc	r6, [r4], #-1363	; 0xfffffaad
 8007548:	73676e69 	cmnvc	r7, #1680	; 0x690
 800754c:	69654700 	stmdbvs	r5!, {r8, r9, sl, lr}^
 8007550:	20726567 	rsbscs	r6, r2, r7, ror #10
 8007554:	65627574 	strbvs	r7, [r2, #-1396]!	; 0xfffffa8c
 8007558:	00000000 	andeq	r0, r0, r0

0800755c <commId>:
 800755c:	0800756d 	stmdaeq	r0, {r0, r2, r3, r5, r6, r8, sl, ip, sp, lr}

08007560 <displayInitSequence>:
 8007560:	a002a202 	andge	sl, r2, r2, lsl #4
 8007564:	2302c802 	movwcs	ip, #10242	; 0x2802
 8007568:	 	subcs	r2, r0, r4, lsl #2

08007569 <flashDatalogRegion>:
 8007569:	 			; <UNDEFINED> instruction: 0x21204021

0800756b <flashSettingsRegion>:
 800756b:	53462120 	movtpl	r2, #24864	; 0x6120
 800756f:	31313032 	teqcc	r1, r2, lsr r0
 8007573:	44472820 	strbmi	r2, [r7], #-2080	; 0xfffff7e0
 8007577:	31463233 	cmpcc	r6, r3, lsr r2
 800757b:	38433035 	stmdacc	r3, {r0, r2, r4, r5, ip, sp}^
 800757f:	61523b29 	cmpvs	r2, r9, lsr #22
 8007583:	72502064 	subsvc	r2, r0, #100	; 0x64
 8007587:	2e32206f 	cdpcs	0, 3, cr2, cr2, cr15, {3}
 800758b:	74656230 	strbtvc	r6, [r5], #-560	; 0xfffffdd0
 800758f:	00383161 	eorseq	r3, r8, r1, ror #2
	...

08007594 <flashBlockSize>:
 8007594:	00000002 	andeq	r0, r0, r2

08007598 <flashPageDataSize>:
 8007598:	000003fe 	strdeq	r0, [r0], -lr

0800759c <statisticsView>:
 800759c:	08004d7d 	stmdaeq	r0, {r0, r2, r3, r4, r5, r6, r8, sl, fp, lr}
 80075a0:	00000000 	andeq	r0, r0, r0
 80075a4:	6e6f7257 	mcrvs	2, 3, r7, cr15, cr7, {2}
 80075a8:	61762067 	cmnvs	r6, r7, rrx
 80075ac:	7365756c 	cmnvc	r5, #108, 10	; 0x1b000000
 80075b0:	79616d20 	stmdbvc	r1!, {r5, r8, sl, fp, sp, lr}^
 80075b4:	72616820 	rsbvc	r6, r1, #32, 16	; 0x200000
 80075b8:	6564206d 	strbvs	r2, [r4, #-109]!	; 0xffffff93
 80075bc:	65636976 	strbvs	r6, [r3, #-2422]!	; 0xfffff68a
 80075c0:	2820002e 	stmdacs	r0!, {r1, r2, r3, r5}
 80075c4:	61666564 	cmnvs	r6, r4, ror #10
 80075c8:	29746c75 	ldmdbcs	r4!, {r0, r2, r4, r5, r6, sl, fp, sp, lr}^
 80075cc:	70632000 	rsbvc	r2, r3, r0
 80075d0:	53b52f6d 			; <UNDEFINED> instruction: 0x53b52f6d
 80075d4:	00682f76 	rsbeq	r2, r8, r6, ror pc
 80075d8:	31002520 	tstcc	r0, r0, lsr #10
 80075dc:	2035322e 	eorscs	r3, r5, lr, lsr #4
 80075e0:	007a486b 	rsbseq	r4, sl, fp, ror #16
 80075e4:	20352e32 	eorscs	r2, r5, r2, lsr lr
 80075e8:	007a486b 	rsbseq	r4, sl, fp, ror #16
 80075ec:	6b203031 	blvs	88136b8 <_sidata+0x80be68>
 80075f0:	32007a48 	andcc	r7, r0, #72, 20	; 0x48000
 80075f4:	486b2030 	stmdami	fp!, {r4, r5, sp}^
 80075f8:	3034007a 	eorscc	r0, r4, sl, ror r0
 80075fc:	7a486b20 	bvc	9222284 <_sidata+0x121aa34>
 8007600:	4d575000 	ldclmi	0, cr5, [r7, #-0]
 8007604:	65726620 	ldrbvs	r6, [r2, #-1568]!	; 0xfffff9e0
 8007608:	6e657571 	mcrvs	5, 3, r7, cr5, cr1, {3}
 800760c:	50007963 	andpl	r7, r0, r3, ror #18
 8007610:	64204d57 	strtvs	r4, [r0], #-3415	; 0xfffff2a9
 8007614:	20797475 	rsbscs	r7, r9, r5, ror r4
 8007618:	6c637963 			; <UNDEFINED> instruction: 0x6c637963
 800761c:	61460065 	cmpvs	r6, r5, rrx
 8007620:	726f7463 	rsbvc	r7, pc, #1660944384	; 0x63000000
 8007624:	65642079 	strbvs	r2, [r4, #-121]!	; 0xffffff87
 8007628:	6c756166 	ldfvse	f6, [r5], #-408	; 0xfffffe68
 800762c:	63410074 	movtvs	r0, #4212	; 0x1074
 8007630:	61727563 	cmnvs	r2, r3, ror #10
 8007634:	45007963 	strmi	r7, [r0, #-2403]	; 0xfffff69d
 8007638:	6772656e 	ldrbvs	r6, [r2, -lr, ror #10]!
 800763c:	61732d79 	cmnvs	r3, r9, ror sp
 8007640:	676e6976 			; <UNDEFINED> instruction: 0x676e6976
 8007644:	73754300 	cmnvc	r5, #0, 6
 8007648:	006d6f74 	rsbeq	r6, sp, r4, ror pc
 800764c:	31364848 	teqcc	r6, r8, asr #16
 8007650:	334a0034 	movtcc	r0, #41012	; 0xa034
 8007654:	4a003530 	bmi	8014b1c <_sidata+0xd2cc>
 8007658:	00313233 	eorseq	r3, r1, r3, lsr r2
 800765c:	3331364a 	teqcc	r1, #77594624	; 0x4a00000
 8007660:	30344d00 	eorscc	r4, r4, r0, lsl #26
 8007664:	53003131 	movwpl	r3, #305	; 0x131
 8007668:	322d4d42 	eorcc	r4, sp, #4224	; 0x1080
 800766c:	6f430030 	svcvs	0x00430030
 8007670:	7265766e 	rsbvc	r7, r5, #115343360	; 0x6e00000
 8007674:	6e6f6973 			; <UNDEFINED> instruction: 0x6e6f6973
 8007678:	63616620 	cmnvs	r1, #32, 12	; 0x2000000
 800767c:	00726f74 	rsbseq	r6, r2, r4, ror pc
 8007680:	64616544 	strbtvs	r6, [r1], #-1348	; 0xfffffabc
 8007684:	6d69742d 	cfstrdvs	mvd7, [r9, #-180]!	; 0xffffff4c
 8007688:	6f632065 	svcvs	0x00632065
 800768c:	002e706d 	eoreq	r7, lr, sp, rrx
 8007690:	70205648 	eorvc	r5, r0, r8, asr #12
 8007694:	69666f72 	stmdbvs	r6!, {r1, r4, r5, r6, r8, r9, sl, fp, sp, lr}^
 8007698:	4400656c 	strmi	r6, [r0], #-1388	; 0xfffffa94
 800769c:	2d646165 	stfcse	f6, [r4, #-404]!	; 0xfffffe6c
 80076a0:	656d6974 	strbvs	r6, [sp, #-2420]!	; 0xfffff68c
 80076a4:	6d6f6320 	stclvs	3, cr6, [pc, #-128]!	; 800762c <statisticsView+0x90>
 80076a8:	736e6570 	cmnvc	lr, #112, 10	; 0x1c000000
 80076ac:	6f697461 	svcvs	0x00697461
 80076b0:	0000006e 	andeq	r0, r0, lr, rrx

080076b4 <tubeConversionFactorMenu>:
 80076b4:	0800766e 	stmdaeq	r0, {r1, r2, r3, r5, r6, r9, sl, ip, sp, lr}
 80076b8:	20000c58 	andcs	r0, r0, r8, asr ip
 80076bc:	08005025 	stmdaeq	r0, {r0, r2, r5, ip, lr}
 80076c0:	08004f41 	stmdaeq	r0, {r0, r6, r8, r9, sl, fp, lr}
 80076c4:	08004e15 	stmdaeq	r0, {r0, r2, r4, r9, sl, fp, lr}

080076c8 <tubeConversionFactorMenuOptions>:
 80076c8:	0800764c 	stmdaeq	r0, {r2, r3, r6, r9, sl, ip, sp, lr}
 80076cc:	08007652 	stmdaeq	r0, {r1, r4, r6, r9, sl, ip, sp, lr}
 80076d0:	08007657 	stmdaeq	r0, {r0, r1, r2, r4, r6, r9, sl, ip, sp, lr}
 80076d4:	0800765c 	stmdaeq	r0, {r2, r3, r4, r6, r9, sl, ip, sp, lr}
 80076d8:	08007661 	stmdaeq	r0, {r0, r5, r6, r9, sl, ip, sp, lr}
 80076dc:	08007667 	stmdaeq	r0, {r0, r1, r2, r5, r6, r9, sl, ip, sp, lr}

080076e0 <tubeConversionFactorMenuValues>:
 80076e0:	4288cccd 	addmi	ip, r8, #52480	; 0xcd00
 80076e4:	432f0000 			; <UNDEFINED> instruction: 0x432f0000
 80076e8:	43190000 	tstmi	r9, #0
 80076ec:	4288cccd 	addmi	ip, r8, #52480	; 0xcd00
 80076f0:	43190000 	tstmi	r9, #0
 80076f4:	432f0000 			; <UNDEFINED> instruction: 0x432f0000

080076f8 <tubeConversionFactorMenuView>:
 80076f8:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 80076fc:	080076b4 	stmdaeq	r0, {r2, r4, r5, r7, r9, sl, ip, sp, lr}

08007700 <tubeDeadTimeCompensationMenu>:
 8007700:	0800769b 	stmdaeq	r0, {r0, r1, r3, r4, r7, r9, sl, ip, sp, lr}
 8007704:	20000c5c 	andcs	r0, r0, ip, asr ip
 8007708:	08004fa9 	stmdaeq	r0, {r0, r3, r5, r7, r8, r9, sl, fp, lr}
 800770c:	08004f29 	stmdaeq	r0, {r0, r3, r5, r8, r9, sl, fp, lr}
 8007710:	08004e15 	stmdaeq	r0, {r0, r2, r4, r9, sl, fp, lr}

08007714 <tubeDeadTimeCompensationMenuView>:
 8007714:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8007718:	08007700 	stmdaeq	r0, {r8, r9, sl, ip, sp, lr}

0800771c <tubeHVCustomProfileMenu>:
 800771c:	08007645 	stmdaeq	r0, {r0, r2, r6, r9, sl, ip, sp, lr}
 8007720:	20000c60 	andcs	r0, r0, r0, ror #24
 8007724:	08004dcd 	stmdaeq	r0, {r0, r2, r3, r6, r7, r8, sl, fp, lr}
 8007728:	08004df5 	stmdaeq	r0, {r0, r2, r4, r5, r6, r7, r8, sl, fp, lr}
 800772c:	08004de9 	stmdaeq	r0, {r0, r3, r5, r6, r7, r8, sl, fp, lr}

08007730 <tubeHVCustomProfileMenuOptions>:
 8007730:	08007601 	stmdaeq	r0, {r0, r9, sl, ip, sp, lr}
 8007734:	0800760f 	stmdaeq	r0, {r0, r1, r2, r3, r9, sl, ip, sp, lr}
 8007738:	00000000 	andeq	r0, r0, r0

0800773c <tubeHVCustomProfileMenuView>:
 800773c:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8007740:	0800771c 	stmdaeq	r0, {r2, r3, r4, r8, r9, sl, ip, sp, lr}

08007744 <tubeHVCustomProfileWarningView>:
 8007744:	08004f59 	stmdaeq	r0, {r0, r3, r4, r6, r8, r9, sl, fp, lr}
 8007748:	00000000 	andeq	r0, r0, r0

0800774c <tubeHVDutyCycleMenu>:
 800774c:	0800760f 	stmdaeq	r0, {r0, r1, r2, r3, r9, sl, ip, sp, lr}
 8007750:	20000c64 	andcs	r0, r0, r4, ror #24
 8007754:	08004ec1 	stmdaeq	r0, {r0, r6, r7, r9, sl, fp, lr}
 8007758:	08004e35 	stmdaeq	r0, {r0, r2, r4, r5, r9, sl, fp, lr}
 800775c:	08004ddd 	stmdaeq	r0, {r0, r2, r3, r4, r6, r7, r8, sl, fp, lr}

08007760 <tubeHVDutyCycleMenuView>:
 8007760:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8007764:	0800774c 	stmdaeq	r0, {r2, r3, r6, r8, r9, sl, ip, sp, lr}

08007768 <tubeHVFrequencyMenu>:
 8007768:	08007601 	stmdaeq	r0, {r0, r9, sl, ip, sp, lr}
 800776c:	20000c68 	andcs	r0, r0, r8, ror #24
 8007770:	08004e89 	stmdaeq	r0, {r0, r3, r7, r9, sl, fp, lr}
 8007774:	08004e4d 	stmdaeq	r0, {r0, r2, r3, r6, r9, sl, fp, lr}
 8007778:	08004ddd 	stmdaeq	r0, {r0, r2, r3, r4, r6, r7, r8, sl, fp, lr}

0800777c <tubeHVFrequencyMenuOptions>:
 800777c:	080075db 	stmdaeq	r0, {r0, r1, r3, r4, r6, r7, r8, sl, ip, sp, lr}
 8007780:	080075e4 	stmdaeq	r0, {r2, r5, r6, r7, r8, sl, ip, sp, lr}
 8007784:	080075e6 	stmdaeq	r0, {r1, r2, r5, r6, r7, r8, sl, ip, sp, lr}
 8007788:	080075ec 	stmdaeq	r0, {r2, r3, r5, r6, r7, r8, sl, ip, sp, lr}
 800778c:	080075f3 	stmdaeq	r0, {r0, r1, r4, r5, r6, r7, r8, sl, ip, sp, lr}
 8007790:	080075fa 	stmdaeq	r0, {r1, r3, r4, r5, r6, r7, r8, sl, ip, sp, lr}
 8007794:	00000000 	andeq	r0, r0, r0

08007798 <tubeHVProfileMenu>:
 8007798:	08007690 	stmdaeq	r0, {r4, r7, r9, sl, ip, sp, lr}
 800779c:	20000c6c 	andcs	r0, r0, ip, ror #24
 80077a0:	08004da5 	stmdaeq	r0, {r0, r2, r5, r7, r8, sl, fp, lr}
 80077a4:	08004e65 	stmdaeq	r0, {r0, r2, r5, r6, r9, sl, fp, lr}
 80077a8:	08004e15 	stmdaeq	r0, {r0, r2, r4, r9, sl, fp, lr}

080077ac <tubeHVProfileMenuOptions>:
 80077ac:	0800761e 	stmdaeq	r0, {r1, r2, r3, r4, r9, sl, ip, sp, lr}
 80077b0:	0800762e 	stmdaeq	r0, {r1, r2, r3, r5, r9, sl, ip, sp, lr}
 80077b4:	08007637 	stmdaeq	r0, {r0, r1, r2, r4, r5, r9, sl, ip, sp, lr}
 80077b8:	08007645 	stmdaeq	r0, {r0, r2, r6, r9, sl, ip, sp, lr}
 80077bc:	00000000 	andeq	r0, r0, r0

080077c0 <tubeHVProfileMenuView>:
 80077c0:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 80077c4:	08007798 	stmdaeq	r0, {r3, r4, r7, r8, r9, sl, ip, sp, lr}

080077c8 <tubeHVrequencyMenuView>:
 80077c8:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 80077cc:	08007768 	stmdaeq	r0, {r3, r5, r6, r8, r9, sl, ip, sp, lr}

080077d0 <tubeMenu>:
 80077d0:	0800754d 	stmdaeq	r0, {r0, r2, r3, r6, r8, sl, ip, sp, lr}
 80077d4:	20000c70 	andcs	r0, r0, r0, ror ip
 80077d8:	08004d95 	stmdaeq	r0, {r0, r2, r4, r7, r8, sl, fp, lr}
 80077dc:	08004e21 	stmdaeq	r0, {r0, r5, r9, sl, fp, lr}
 80077e0:	08004029 	stmdaeq	r0, {r0, r3, r5, lr}

080077e4 <tubeMenuOptionViews>:
 80077e4:	080076f8 	stmdaeq	r0, {r3, r4, r5, r6, r7, r9, sl, ip, sp, lr}
 80077e8:	08007714 	stmdaeq	r0, {r2, r4, r8, r9, sl, ip, sp, lr}
 80077ec:	080077c0 	stmdaeq	r0, {r6, r7, r8, r9, sl, ip, sp, lr}

080077f0 <tubeMenuOptions>:
 80077f0:	0800766e 	stmdaeq	r0, {r1, r2, r3, r5, r6, r9, sl, ip, sp, lr}
 80077f4:	08007680 	stmdaeq	r0, {r7, r9, sl, ip, sp, lr}
 80077f8:	08007690 	stmdaeq	r0, {r4, r7, r9, sl, ip, sp, lr}
 80077fc:	00000000 	andeq	r0, r0, r0

08007800 <tubeMenuView>:
 8007800:	08003691 	stmdaeq	r0, {r0, r4, r7, r9, sl, ip, sp}
 8007804:	080077d0 	stmdaeq	r0, {r4, r6, r7, r8, r9, sl, ip, sp, lr}

08007808 <mr_st7565_display_off_sequence>:
 8007808:	a502ae02 	strge	sl, [r2, #-3586]	; 0xfffff1fe
 800780c:	 	adceq	r0, r4, #4, 4	; 0x40000000

0800780d <mr_st7565_display_on_sequence>:
 800780d:	af02a402 	svcge	0x0002a402
 8007811:	 	tsteq	r1, r4

08007812 <mr_st7565_init_sequence>:
 8007812:	01010100 	mrseq	r0, (UNDEF: 17)
 8007816:	00010100 	andeq	r0, r1, r0, lsl #2
 800781a:	2f020100 	svccs	0x00020100
 800781e:	 	streq	r0, [r3, #-1540]	; 0xfffff9fc

0800781f <mcumax_board_setup>:
 800781f:	07050306 	streq	r0, [r5, -r6, lsl #6]
 8007823:	06030504 	streq	r0, [r3], -r4, lsl #10

08007827 <mcumax_capture_values>:
 8007827:	07020200 	streq	r0, [r2, -r0, lsl #4]
 800782b:	170c08ff 			; <UNDEFINED> instruction: 0x170c08ff

0800782f <mcumax_step_vectors>:
 800782f:	00eff1f0 	strdeq	pc, [pc], #16	; <UNPREDICTABLE>
 8007833:	01001001 	tsteq	r0, r1
 8007837:	00110f10 	andseq	r0, r1, r0, lsl pc
 800783b:	211f120e 	tstcs	pc, lr, lsl #4
	...

08007840 <mcumax_step_vectors_indices>:
 8007840:	0bff0700 	bleq	7fc9448 <_Min_Stack_Size+0x7fc9048>
 8007844:	06030806 	streq	r0, [r3], -r6, lsl #16

Disassembly of section .init_array:

08007848 <__frame_dummy_init_array_entry>:
 8007848:	080000e5 	stmdaeq	r0, {r0, r2, r5, r6, r7}

Disassembly of section .fini_array:

0800784c <__do_global_dtors_aux_fini_array_entry>:
 800784c:	080000c1 	stmdaeq	r0, {r0, r6, r7}

Disassembly of section .data:

20000000 <randomLFSR>:
20000000:	00000001 	andeq	r0, r0, r1

20000004 <displayBrightnessValue>:
20000004:	00da002f 	sbcseq	r0, sl, pc, lsr #32
20000008:	03e80213 	mvneq	r0, #805306369	; 0x30000001

2000000c <units>:
2000000c:	080075d3 	stmdaeq	r0, {r0, r1, r4, r6, r7, r8, sl, ip, sp, lr}
20000010:	387ba882 	ldmdacc	fp!, {r1, r7, fp, sp, pc}^
20000014:	00000000 	andeq	r0, r0, r0
20000018:	08007180 	stmdaeq	r0, {r7, r8, ip, sp, lr}
2000001c:	328f2a63 	addcc	r2, pc, #405504	; 0x63000
20000020:	00000000 	andeq	r0, r0, r0
20000024:	08007183 	stmdaeq	r0, {r0, r1, r7, r8, ip, sp, lr}
20000028:	3bc49ba6 	blcc	1f126ec8 <_sidata+0x1711f678>
2000002c:	00000000 	andeq	r0, r0, r0
20000030:	08007174 	stmdaeq	r0, {r2, r4, r5, r6, r8, ip, sp, lr}
20000034:	35dfb23b 	ldrbcc	fp, [pc, #571]	; 20000277 <game+0x15b>
20000038:	00000000 	andeq	r0, r0, r0
2000003c:	08007178 	stmdaeq	r0, {r3, r4, r5, r6, r8, ip, sp, lr}
20000040:	42700000 	rsbsmi	r0, r0, #0
20000044:	00000002 	andeq	r0, r0, r2
20000048:	08007189 	stmdaeq	r0, {r0, r3, r7, r8, ip, sp, lr}
2000004c:	3f800000 	svccc	0x00800000
20000050:	00000002 	andeq	r0, r0, r2
20000054:	0800717c 	stmdaeq	r0, {r2, r3, r4, r5, r6, r8, ip, sp, lr}
20000058:	3f800000 	svccc	0x00800000
2000005c:	00000002 	andeq	r0, r0, r2
20000060:	08007189 	stmdaeq	r0, {r0, r3, r7, r8, ip, sp, lr}
20000064:	3f800000 	svccc	0x00800000
20000068:	00000002 	andeq	r0, r0, r2

2000006c <timerCountToSeconds>:
2000006c:	340637bd 	strcc	r3, [r6], #-1981	; 0xfffff843

Disassembly of section .bss:

20000070 <__bss_start__>:
20000070:	00000000 	andeq	r0, r0, r0

20000074 <object.8659>:
	...

2000008c <pulseClicksMenuState>:
2000008c:	00000000 	andeq	r0, r0, r0

20000090 <datalog>:
	...

200000d8 <datalogMenuState>:
200000d8:	00000000 	andeq	r0, r0, r0

200000dc <displayBrightnessMenuState>:
200000dc:	00000000 	andeq	r0, r0, r0

200000e0 <displayContrastMenuState>:
200000e0:	00000000 	andeq	r0, r0, r0

200000e4 <displayMenuState>:
200000e4:	00000000 	andeq	r0, r0, r0

200000e8 <displaySleepMenuState>:
200000e8:	00000000 	andeq	r0, r0, r0

200000ec <events>:
	...

2000011c <game>:
	...

200002f0 <gameMenuState>:
200002f0:	00000000 	andeq	r0, r0, r0

200002f4 <gameStrengthMenuState>:
200002f4:	00000000 	andeq	r0, r0, r0

200002f8 <keyboard>:
	...

20000320 <averageTimerMenuState>:
20000320:	00000000 	andeq	r0, r0, r0

20000324 <doseAlarmMenuState>:
20000324:	00000000 	andeq	r0, r0, r0

20000328 <measurements>:
	...

200006d8 <rateAlarmMenuState>:
200006d8:	00000000 	andeq	r0, r0, r0

200006dc <unitsMenuState>:
200006dc:	00000000 	andeq	r0, r0, r0

200006e0 <batteryTypeMenuState>:
200006e0:	00000000 	andeq	r0, r0, r0

200006e4 <rng>:
	...

2000072c <rngMenuState>:
2000072c:	00000000 	andeq	r0, r0, r0

20000730 <rtcCurrentDateTime>:
	...

20000738 <rtcItemMenuState>:
20000738:	00000000 	andeq	r0, r0, r0

2000073c <rtcMenuState>:
2000073c:	00000000 	andeq	r0, r0, r0

20000740 <settingsMenuState>:
20000740:	00000000 	andeq	r0, r0, r0

20000744 <displayFramebuffer>:
	...

20000b44 <adc>:
	...

20000b4c <tube>:
	...

20000c58 <tubeConversionFactorMenuState>:
20000c58:	00000000 	andeq	r0, r0, r0

20000c5c <tubeDeadTimeCompensationMenuState>:
20000c5c:	00000000 	andeq	r0, r0, r0

20000c60 <tubeHVCustomProfileMenuState>:
20000c60:	00000000 	andeq	r0, r0, r0

20000c64 <tubeHVDutyCycleMenuState>:
20000c64:	00000000 	andeq	r0, r0, r0

20000c68 <tubeHVFrequencyMenuState>:
20000c68:	00000000 	andeq	r0, r0, r0

20000c6c <tubeHVProfileMenuState>:
20000c6c:	00000000 	andeq	r0, r0, r0

20000c70 <tubeMenuState>:
20000c70:	00000000 	andeq	r0, r0, r0

20000c74 <view>:
	...

20000c7c <comm>:
	...

20000ccc <displayOn>:
20000ccc:	00000000 	andeq	r0, r0, r0

20000cd0 <mr>:
	...

20000d70 <displayThemeMenuView>:
	...

20000d78 <eventsCurrentTick>:
20000d78:	00000000 	andeq	r0, r0, r0

20000d7c <menuOption>:
	...

20000d9c <powerOffRequested>:
20000d9c:	00000000 	andeq	r0, r0, r0

20000da0 <settings>:
	...

20000dac <mcumax>:
	...

Disassembly of section ._user_heap_stack:

20000e64 <._user_heap_stack>:
	...

Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:	00002841 	andeq	r2, r0, r1, asr #16
   4:	61656100 	cmnvs	r5, r0, lsl #2
   8:	01006962 	tsteq	r0, r2, ror #18
   c:	0000001e 	andeq	r0, r0, lr, lsl r0
  10:	4d2d3705 	stcmi	7, cr3, [sp, #-20]!	; 0xffffffec
  14:	070a0600 	streq	r0, [sl, -r0, lsl #12]
  18:	1202094d 	andne	r0, r2, #1261568	; 0x134000
  1c:	15011404 	strne	r1, [r1, #-1028]	; 0xfffffbfc
  20:	18031701 	stmdane	r3, {r0, r8, r9, sl, ip}
  24:	22011a01 	andcs	r1, r1, #4096	; 0x1000
  28:	Address 0x0000000000000028 is out of bounds.


Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347 	bcc	10d0d24 <_Min_Stack_Size+0x10d0924>
   4:	4e472820 	cdpmi	8, 4, cr2, cr7, cr0, {1}
   8:	6f542055 	svcvs	0x00542055
   c:	20736c6f 	rsbscs	r6, r3, pc, ror #24
  10:	20726f66 	rsbscs	r6, r2, r6, ror #30
  14:	206d7241 	rsbcs	r7, sp, r1, asr #4
  18:	65626d45 	strbvs	r6, [r2, #-3397]!	; 0xfffff2bb
  1c:	64656464 	strbtvs	r6, [r5], #-1124	; 0xfffffb9c
  20:	6f725020 	svcvs	0x00725020
  24:	73736563 	cmnvc	r3, #415236096	; 0x18c00000
  28:	2073726f 	rsbscs	r7, r3, pc, ror #4
  2c:	30322d37 	eorscc	r2, r2, r7, lsr sp
  30:	712d3731 			; <UNDEFINED> instruction: 0x712d3731
  34:	616d2d34 	cmnvs	sp, r4, lsr sp
  38:	29726f6a 	ldmdbcs	r2!, {r1, r3, r5, r6, r8, r9, sl, fp, sp, lr}^
  3c:	322e3720 	eorcc	r3, lr, #32, 14	; 0x800000
  40:	3220312e 	eorcc	r3, r0, #-2147483637	; 0x8000000b
  44:	30373130 	eorscc	r3, r7, r0, lsr r1
  48:	20343039 	eorscs	r3, r4, r9, lsr r0
  4c:	6c657228 	sfmvs	f7, 2, [r5], #-160	; 0xffffff60
  50:	65736165 	ldrbvs	r6, [r3, #-357]!	; 0xfffffe9b
  54:	415b2029 	cmpmi	fp, r9, lsr #32
  58:	652f4d52 	strvs	r4, [pc, #-3410]!	; fffff30e <_estack+0xdfffd30e>
  5c:	6465626d 	strbtvs	r6, [r5], #-621	; 0xfffffd93
  60:	2d646564 	cfstr64cs	mvdx6, [r4, #-400]!	; 0xfffffe70
  64:	72622d37 	rsbvc	r2, r2, #3520	; 0xdc0
  68:	68636e61 	stmdavs	r3!, {r0, r5, r6, r9, sl, fp, sp, lr}^
  6c:	76657220 	strbtvc	r7, [r5], -r0, lsr #4
  70:	6f697369 	svcvs	0x00697369
  74:	3532206e 	ldrcc	r2, [r2, #-110]!	; 0xffffff92
  78:	34303235 	ldrtcc	r3, [r0], #-565	; 0xfffffdcb
  7c:	Address 0x000000000000007c is out of bounds.


Disassembly of section .debug_frame:

00000000 <.debug_frame>:
   0:	0000000c 	andeq	r0, r0, ip
   4:	ffffffff 			; <UNDEFINED> instruction: 0xffffffff
   8:	7c020001 	stcvc	0, cr0, [r2], {1}
   c:	000d0c0e 	andeq	r0, sp, lr, lsl #24
  10:	00000018 	andeq	r0, r0, r8, lsl r0
  14:	00000000 	andeq	r0, r0, r0
  18:	08005fd0 	stmdaeq	r0, {r4, r6, r7, r8, r9, sl, fp, ip, lr}
  1c:	00000048 	andeq	r0, r0, r8, asr #32
  20:	84100e41 	ldrhi	r0, [r0], #-3649	; 0xfffff1bf
  24:	86038504 	strhi	r8, [r3], -r4, lsl #10
  28:	00018e02 	andeq	r8, r1, r2, lsl #28
  2c:	0000000c 	andeq	r0, r0, ip
  30:	ffffffff 			; <UNDEFINED> instruction: 0xffffffff
  34:	7c020001 	stcvc	0, cr0, [r2], {1}
  38:	000d0c0e 	andeq	r0, sp, lr, lsl #24
  3c:	00000014 	andeq	r0, r0, r4, lsl r0
  40:	0000002c 	andeq	r0, r0, ip, lsr #32
  44:	08006018 	stmdaeq	r0, {r3, r4, sp, lr}
  48:	00000016 	andeq	r0, r0, r6, lsl r0
  4c:	84080e41 	strhi	r0, [r8], #-3649	; 0xfffff1bf
  50:	00018e02 	andeq	r8, r1, r2, lsl #28
  54:	0000000c 	andeq	r0, r0, ip
  58:	ffffffff 			; <UNDEFINED> instruction: 0xffffffff
  5c:	7c020001 	stcvc	0, cr0, [r2], {1}
  60:	000d0c0e 	andeq	r0, sp, lr, lsl #24
  64:	0000000c 	andeq	r0, r0, ip
  68:	00000054 	andeq	r0, r0, r4, asr r0
  6c:	0800602e 	stmdaeq	r0, {r1, r2, r3, r5, sp, lr}
  70:	00000010 	andeq	r0, r0, r0, lsl r0
  74:	0000000c 	andeq	r0, r0, ip
  78:	ffffffff 			; <UNDEFINED> instruction: 0xffffffff
  7c:	7c020001 	stcvc	0, cr0, [r2], {1}
  80:	000d0c0e 	andeq	r0, sp, lr, lsl #24
  84:	00000014 	andeq	r0, r0, r4, lsl r0
  88:	00000074 	andeq	r0, r0, r4, ror r0
  8c:	0800603e 	stmdaeq	r0, {r1, r2, r3, r4, r5, sp, lr}
  90:	0000001e 	andeq	r0, r0, lr, lsl r0
  94:	84080e42 	strhi	r0, [r8], #-3650	; 0xfffff1be
  98:	00018e02 	andeq	r8, r1, r2, lsl #28
  9c:	0000000c 	andeq	r0, r0, ip
  a0:	ffffffff 			; <UNDEFINED> instruction: 0xffffffff
  a4:	7c020001 	stcvc	0, cr0, [r2], {1}
  a8:	000d0c0e 	andeq	r0, sp, lr, lsl #24
  ac:	0000000c 	andeq	r0, r0, ip
  b0:	0000009c 	muleq	r0, ip, r0
  b4:	0800605c 	stmdaeq	r0, {r2, r3, r4, r6, sp, lr}
  b8:	00000010 	andeq	r0, r0, r0, lsl r0
