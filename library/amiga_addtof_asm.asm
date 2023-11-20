* call routine called by AddTOF
* D0, D1, A0, A1, A5, A6 are scratch registers

  entry call_routine

call_routine
  move.l 26(a1),-(sp)   argument for the called routine (Carg)
  movea.l 22(a1),a0     address of the routine to call (ccode)
  jsr (a0)
  addq.l #4,sp
  moveq #0,d0           set Z at 1 to continue chain
  rts