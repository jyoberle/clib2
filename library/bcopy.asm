* copied from string_memcpy.c and modified by JOB

	xdef    __bcopy
	
___bcopy

	movem.l	4(sp),a0-a1
	move.l	12(sp),d0

	move.w	a0,d1			* Make sure that A0 is long word aligned.
	andi.w	#3,d1
	beq		already_aligned

	not.w	d1				* This may de-align A1, but that's life.
	andi.w	#3,d1

align_loop:

	move.b	(a0)+,(a1)+		* Move from one to four bytes to long
	subq.l	#1,d0			* word align A0.
	dbeq	d1,align_loop
	beq		done

already_aligned:

	move.w	d0,d1
	andi.w	#511,d1
	move.w	d1,-(sp)
	moveq	#9,d1
	lsr.l	d1,d0
	beq		no_big_chunks_to_move

	movem.l	d2-d7/a2-a6,-(sp)
	swap	d0
	move.w	d0,-(sp)
	swap	d0
	bra		move_big_chunk_end

move_big_chunk:

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

move_big_chunk_end:

	dbf		d0,move_big_chunk
	subq.w	#1,(sp)
	bge		move_big_chunk

	addq.w	#2,sp

	movem.l	(sp)+,d2-d7/a2-a6

no_big_chunks_to_move:

	move.w	(sp)+,d0

	move.w	d0,d1
	lsr.w	#5,d1			* D1 now holds numbytes / 32 and
	andi.w	#31,d0			* D0 holds numbytes % 32.

	bra	move_eight_longs_end

move_eight_longs:

	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+

move_eight_longs_end:

	dbf		d1,move_eight_longs

	move.w	d0,d1
	lsr.w	#2,d1			* D1 now holds numbytes / 4 and

	bra		move_long_end

move_long:

	move.l	(a0)+,(a1)+

move_long_end:

	dbf		d1,move_long

	andi.w	#3,d0			* D0 holds numbytes % 4.
	bra		move_byte_end

move_byte:

	move.b	(a0)+,(a1)+

move_byte_end:

	dbf		d0,move_byte

done:

	rts