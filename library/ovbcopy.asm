* copied from string_memmove.c and modified by JOB

	xdef    __ovbcopy

__ovbcopy
	movem.l	4(sp),a0-a1
	move.l	12(sp),d0

	move.l	a2,-(sp)
	cmpa.l	a0,a1
	bcs		forwards

* If A0 is less than A1 then we _may_ have to do the memory move
* backwards. However, since doing the memory move backwards is less
* efficient if we have to do any byte moves, I'll first check to see
* whether there is actually any overlap.

	lea		(0,a0,d0.l),a2
	cmpa.l	a2,a1			* If A2 is greater than A1 then there is overlap,
	bcs		backwards		* and we'd better do the move backwards.

forwards:

	move.w	a0,d1			* Make sure that A0 is long word aligned.
	not.w	d1				* This may de-align A1, but that's life.
	andi.w	#3,d1

align_loop1:

	move.b	(a0)+,(a1)+		* Move from one to four bytes to long
	subq.l	#1,d0			* word align A0.
	dbeq	d1,align_loop1
	beq		done

	move.w	d0,d1
	andi.w	#511,d1
	move.w	d1,-(sp)
	moveq	#9,d1
	lsr.l	d1,d0
	beq		no_big_chunks_to_move1

	movem.l	d2-d7/a3-a6,-(sp)
	swap	d0
	move.w	d0,-(sp)
	swap	d0

	bra.s	move_big_chunk_end1

move_big_chunk1:

	movem.l	(a0)+,d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,(a1)
	movem.l	(a0)+,d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,48(a1)
	movem.l	(a0)+,d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,96(a1)
	movem.l	(a0)+,d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,144(a1)
	movem.l	(a0)+,d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,192(a1)
	movem.l	(a0)+,d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,240(a1)
	movem.l	(a0)+,d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,288(a1)
	movem.l	(a0)+,d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,336(a1)
	movem.l	(a0)+,d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,384(a1)
	movem.l	(a0)+,d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,432(a1)
	movem.l	(a0)+,d1-d7/a2
	movem.l	d1-d7/a2,480(a1)
	adda.w	#512,a1

move_big_chunk_end1:

	dbf		d0,move_big_chunk1

	subq.w	#1,(sp)
	bge		move_big_chunk1

	addq.w	#2,sp

	movem.l	(sp)+,d2-d7/a3-a6

no_big_chunks_to_move1:

	move.w	(sp)+,d0

	move.w	d0,d1
	lsr.w	#5,d1			* D1 now holds numbytes / 32 and
	andi.w	#31,d0			* D0 holds numbytes % 32.

	bra.s	move_eight_longs_end1

move_eight_longs1:

	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+

move_eight_longs_end1:

	dbf		d1,move_eight_longs1

	move.w	d0,d1
	lsr.w	#2,d1			* D1 now holds numbytes / 4

	bra.s	move_long_end1

move_long1:

	move.l	(a0)+,(a1)+

move_long_end1:

	dbf		d1,move_long1

	andi.w	#3,d0			* D0 now holds numbytes % 4.

	bra		move_byte_end1

move_byte1:

	move.b	(a0)+,(a1)+

move_byte_end1:

	dbf		d0,move_byte1

done:

	move.l	(sp)+,a2
	rts

backwards:

	adda.l	d0,a0
	adda.l	d0,a1

	move.w	a0,d1			* Make sure that A0 is long word aligned.
	subq.w	#1,d1			* This may de-align A1, but that's life.
	andi.w	#3,d1

align_loop2:

	move.b	-(a0),-(a1)
	subq.l	#1,d0
	dbeq	d1,align_loop2
	beq		done

	move.w	d0,d1
	andi.w	#511,d1
	move.w	d1,-(sp)
	moveq	#9,d1
	lsr.l	d1,d0
	beq		no_big_chunks_to_move2

	movem.l	d2-d7/a3-a6,-(sp)
	swap	d0
	move.w	d0,-(sp)
	swap	d0

	suba.w	#48,a0
	bra.s	move_big_chunk_end2

move_big_chunk2:

	movem.l	(a0),d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,-(a1)
	movem.l	-48(a0),d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,-(a1)
	movem.l	-96(a0),d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,-(a1)
	movem.l	-144(a0),d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,-(a1)
	movem.l	-192(a0),d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,-(a1)
	movem.l	-240(a0),d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,-(a1)
	movem.l	-288(a0),d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,-(a1)
	movem.l	-336(a0),d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,-(a1)
	movem.l	-384(a0),d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,-(a1)
	movem.l	-432(a0),d1-d7/a2-a6
	movem.l	d1-d7/a2-a6,-(a1)
	movem.l	-464(a0),d1-d7/a2
	movem.l	d1-d7/a2,-(a1)
	suba.w	#512,a0

move_big_chunk_end2:

	dbf		d0,move_big_chunk2

	subq.w	#1,(sp)
	bge		move_big_chunk2

	adda.w	#48,a0

	addq.w	#2,sp

	movem.l	(sp)+,d2-d7/a3-a6

no_big_chunks_to_move2:

	move.w	(sp)+,d0

	move.w	d0,d1
	lsr.w	#5,d1			* D1 now holds numbytes / 32 and
	andi.w	#31,d0			* D0 holds numbytes % 32.

	bra.s	move_eight_longs_end2

move_eight_longs2:

	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)

move_eight_longs_end2:

	dbf		d1,move_eight_longs2

	move.w	d0,d1
	lsr.w	#2,d1			* D1 now holds numbytes / 4

	bra		move_long_end2

move_long2:

	move.l	-(a0),-(a1)

move_long_end2:

	dbf		d1,move_long2

	andi.w	#3,d0			* D0 now holds numbytes % 4.
	bra		move_byte_end2

move_byte2:

	move.b	-(a0),-(a1)

move_byte_end2:

	dbf		d0,move_byte2

	move.l	(sp)+,a2
	rts