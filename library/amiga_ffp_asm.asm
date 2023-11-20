* Routines FFPDBF, FFPAFP, FFPFPA and FFPARND are taken from https://github.com/bayerf42/MotoFFP
* Routine ffpfpbcd is taken from https://github.com/GunterMueller/Le_Lisp/blob/master/Le_Lisp11/ffpfpbcd.s

************************************
* (C) COPYRIGHT 1980 MOTORLA INC.  *
************************************


*****************************************
*         POWER OF TEN TABLE            *
*                                       *
*  EACH ENTRY CORRESPONDS TO A FLOATING *
*  POINT POWER OF TEN WITH A 16 BIT     *
*  EXPONENT AND 32 BIT MANTISSA.        *
*  THIS TABLE IS USED TO INSURE         *
*  PRECISE CONVERSIONS TO AND FROM      *
*  FLOATING POINT AND EXTERNAL FORMATS. *
*  THS IS USED IN ROUTINES "FFPDBF" AND *
*  "FFPFASC".                           *
*                                       *
*          CODE SIZE: 288 BYTES         *
*                                       *
*****************************************

         XDEF      FFP10TBL    ENTRY POINT

         DC.W      64        10**19
         DC.L      $8AC72305
         DC.W      60        10**18
         DC.L      $DE0B6B3A
         DC.W      57        10**17
         DC.L      $16345785<<3+7
         DC.W      54        10**16
         DC.L      $2386F26F<<2+3
         DC.W      50        10**15
         DC.L      $38D7EA4C<<2+2
         DC.W      47        10**14
         DC.L      $5AF3107A<<1+1
         DC.W      44        10**13
         DC.L      $9184E72A
         DC.W      40        10**12
         DC.L      $E8D4A510
         DC.W      37        10**11
         DC.L      $174876E8<<3
         DC.W      34        10**10
         DC.L      $2540BE40<<2
         DC.W      30        10**9
         DC.L      1000000000<<2
         DC.W      27        10**8
         DC.L      100000000<<5
         DC.W      24        10**7
         DC.L      10000000<<8
         DC.W      20        10**6
         DC.L      1000000<<12
         DC.W      17        10**5
         DC.L      100000<<15
         DC.W      14        10**4
         DC.L      10000<<18
         DC.W      10        10**3
         DC.L      1000<<22
         DC.W      7         10**2
         DC.L      100<<25
         DC.W      4         10**1
         DC.L      10<<28
FFP10TBL DC.W      1         10**0
         DC.L      1<<31
         DC.W      -3        10**-1
         DC.L      $CCCCCCCD
         DC.W      -6        10**-2
         DC.L      $A3D70A3D
         DC.W      -9        10**-3
         DC.L      $83126E98
         DC.W      -13       10**-4
         DC.L      $D1B71759
         DC.W      -16       10**-5
         DC.L      $A7C5AC47
         DC.W      -19       10**-6
         DC.L      $8637BD06
         DC.W      -23       10**-7
         DC.L      $D6BF94D6
         DC.W      -26       10**-8
         DC.L      $ABCC7712
         DC.W      -29       10**-9
         DC.L      $89705F41
         DC.W      -33       10**-10
         DC.L      $DBE6FECF
         DC.W      -36       10**-11
         DC.L      $AFEBFF0C
         DC.W      -39       10**-12
         DC.L      $8CBCCC09
         DC.W      -43       10**-13
         DC.L      $E12E1342
         DC.W      -46       10**-14
         DC.L      $B424DC35
         DC.W      -49       10**-15
         DC.L      $901D7CF7
         DC.W      -53       10**-16
         DC.L      $E69594BF
         DC.W      -56       10**-17
         DC.L      $B877AA32
         DC.W      -59       10**-18
         DC.L      $9392EE8F
         DC.W      -63       10**-19
         DC.L      $EC1E4A7E
         DC.W      -66       10**-20
         DC.L      $BCE50865
         DC.W      -69       10**-21
         DC.L      $971DA050
         DC.W      -73       10**-22
         DC.L      $F1C90081
         DC.W      -76       10**-23
         DC.L      $C16D9A01
         DC.W      -79       10**-24
         DC.L      $9ABE14CD
         DC.W      -83       10**-25
         DC.L      $F79687AE
         DC.W      -86       10**-26
         DC.L      $C6120625
         DC.W      -89       10**-27
         DC.L      $9E74D1B8
         DC.W      -93       10**-28
         DC.L      $FD87B5F3

************************************
* (C) COPYRIGHT 1980 MOTORLA INC.  *
************************************

***********************************************************
*                                                         *
*          FAST FLOATING POINT DUAL-BINARY TO FLOAT       *
*                                                         *
*      INPUT:  D6 BIT #16 - REPRESENTS SIGN (0=POSITIVE)  *
*                                           (1=NEGATIVE)  *
*              D6.W - REPRESENTS BASE TEN EXPONENT        *
*                     CONSIDERING D7 A BINARY INTEGER     *
*              D7 -   BINARY INTEGER MANTISSA             *
*                                                         *
*      OUTPUT: D7 - FAST FLOATING POINT EQUIVALENT        *
*                                                         *
*      CONDITION CODES:                                   *
*                N - SET IF RESULT IS NEGATIVE            *
*                Z - SET IF RESULT IS ZERO                *
*                V - SET IF RESULT OVERFLOWED             *
*                C - CLEARED                              *
*                X - UNDEFINED                            *
*                                                         *
*      REGISTERS D3 THRU D6 DESTROYED                     *
*                                                         *
*      CODE SIZE: 164 BYTES     STACK WORK AREA: 4 BYTES  *
*                                                         *
*                                                         *
*      FLOATING POINT RANGE:                              *
*                                                         *
*          FAST FLOATING POINT SUPPORTS THE VALUE ZERO    *
*          AND NON-ZERO VALUES WITHIN THE FOLLOWING       *
*          BOUNDS -                                       *
*                                                         *
* BASE 10                                                 *
*                  18                             -20     *
*   9.22337177 X 10   > +NUMBER >  5.42101070 X 10        *
*                                                         *
*                  18                             -20     *
*  -9.22337177 X 10   > -NUMBER > -2.71050535 X 10        *
*                                                         *
* BASE 2                                                  *
*                   63                            -63     *
*      .FFFFFF  X  2   > +NUMBER >  .FFFFFF  X  2         *
*                                                         *
*                   63                            -64     *
*     -.FFFFFF  X  2   > -NUMBER > -.FFFFFF  X  2         *
*                                                         *
*      PRECISION:                                         *
*                                                         *
*          THIS CONVERSION RESULTS IN A 24 BIT PRECISION  *
*          WITH GUARANTEED ERROR LESS THAN OR EQUAL TO    *
*          ONE-HALF LEAST SIGNIFICANT BIT.                *
*                                                         *
*                                                         *
*      NOTES:                                             *
*          1) THE INPUT FORMATS HAVE BEEN DESIGNED FOR    *
*             EASE OF PARSING TEXT FOR CONVERSION TO      *
*             FLOATING POINT.  SEE FFPASF FOR COMMENTS    *
*             DESCRIBING THE METHOD FOR SETUP TO THIS     *
*             ROUTINE.                                    *
*          2) UNDERFLOWS RETURN A ZERO WITHOUT ANY        *
*             INDICATORS SET.                             *
*          3) OVERFLOWS WILL RETURN THE MAXIMUM VALUE     *
*             POSSIBLE WITH PROPER SIGN AND THE 'V' BIT   *
*             SET IN THE CCR REGISTER.                    *
*                                                         *
***********************************************************

         XDEF      FFPDBF      ENTRY POINT

*         XREF      FFP10TBL    POWER OF TEN TABLE

* NORMALIZE THE INPUT BINARY MANTISSA
FFPDBF   MOVEQ     #32,D5    SETUP BASE 2 EXPONENT MAX
         TST.L     D7        ? TEST FOR ZERO
         BEQ       FPDzRTN   RETURN, NO CONVERSION NEEDED
         BMI.S     FPDINM    BRANCH INPUT ALREADY NORMALIZED
         MOVEQ     #31,D5    PREPARE FOR NORMALIZE LOOP
FPDNMI   ADD.L     D7,D7     SHIFT UP BY ONE
         DBMI      D5,FPDNMI DECREMENT AND LOOP IF NOT YET

* INSURE INPUT 10 POWER INDEX NOT WAY OFF BASE
FPDINM   CMP.W     #18,D6    ? WAY TOO LARGE
         BGT       FPDzOVF   BRANCH OVERFLOW
         CMP.W     #-28,D6   ? WAY TOO SMALL
         BLT.S     FPDRT0    RETURN ZERO IF UNDERFLOW
         MOVE.W    D6,D4     COPY 10 POWER INDEX
         NEG.W     D4        INVERT TO GO PROPER DIRECTION
         MULS.W    #6,D4     TIMES FOUR FOR INDEX
         MOVE.L    A0,-(SP)  SAVE WORK ADDRESS REGISTER
         LEA       (FFP10TBL,PC),A0 LOAD TABLE ADDRESS
         ADD.W     0(A0,D4.W),D5 ADD EXPONENTS FOR MULTIPLY
         MOVE.W    D5,D6     SAVE RESULT EXPONENT IN D6.W

* NOW PERFORM 32 BIT MULTIPLY OF INPUT WITH POWER OF TEN TABLE
         MOVE.L    2(A0,D4.W),D3 LOAD TABLE MANTISSA VALUE
         MOVE.L    (SP),A0   RESTORE WORK REGISTER
         MOVE.L    D3,(SP)   NOW SAVE TABLE MANTISSA ON STACK
         MOVE.W    D7,D5     COPY INPUT VALUE
         MULU.W    D3,D5     TABLELOW X INPUTLOW
         CLR.W     D5        LOW END NO LONGER TAKES AFFECT
         SWAP.W    D5        SAVE INTERMEDIATE SUM
         MOVEQ     #0,D4     CREATE A ZERO FOR DOUBLE PRECISION
         SWAP.W    D3        TO HIGH TABLE WORD
         MULU.W    D7,D3     INPUTLOW X TABLEHIGH
         ADD.L     D3,D5     ADD ANOTHER PARTIAL SUM
         ADDX.B    D4,D4     CREATE CARRY IF ANY
         SWAP.W    D7        NOW TO INPUT HIGH
         MOVE.W    D7,D3     COPY TO WORK REGISTER
         MULU.W    2(SP),D3  TABLELOW X INPUTHIGH
         ADD.L     D3,D5     ADD ANOTHER PARTIAL
         BCC.S     FPDNOC    BRANCH NO CARRY
         ADDQ.B    #1,D4     ADD ANOTHER CARRY
FPDNOC   MOVE.W    D4,D5     CONCAT HIGH WORK WITH LOW
         SWAP.W    D5        AND CORRECT POSITIONS
         MULU.W    (SP),D7   TABLEHIGH X INPUTHIGH
         LEA       4(SP),SP  CLEAN UP STACK
         ADD.L     D5,D7     FINAL PARTIAL PRODUCT
         BMI.S     FPDNON    BRANCH IF NO NEED TO NORMALIZE
         ADD.L     D7,D7     NORMALIZE
         SUBQ.W    #1,D6     ADJUST EXPONENT
FPDNON   ADD.L     #$80,D7   ROUND RESULT TO 24 BITS
         BCC.S     FPDROK    BRANCH ROUND DID NOT OVERFLOW
         ROXR.L    #1,D7     ADJUST BACK
         ADDQ.W    #1,D6     AND INCREMENT EXPONENT
FPDROK   MOVEQ     #9,D3     PREPARE TO FINALIZE EXPONENT TO 7 BITS
         MOVE.W    D6,D4     SAVE SIGN OF EXPONENT
         ASL.W     D3,D6     FORCE 7 BIT PRECISION
         BVS.S     FPDXOV    BRANCH EXPONENT OVERFLOW
         EOR.W     #$8000,D6 EXPONENT BACK FROM 2'S-COMPLEMENT
         LSR.L     D3,D6     PLACE INTO LOW BYTE WITH SIGN
         MOVE.B    D6,D7     INSERT INTO RESULT
         BEQ.S     FPDRT0    RETURN ZERO IF EXPONENT ZERO
FPDzRTN  RTS                 RETURN TO CALLER

* RETURN ZERO FOR UNDERFLOW
FPDRT0   MOVEQ     #0,D7     RETURN ZERO
         RTS                 RETURN TO CALLER

* EXPONENT OVERFLOW/UNDERFLOW
FPDXOV   TST.W     D4        TEST ORIGINAL SIGN
         BMI.S     FPDRT0    BRANCH UNDERFLOW TO RETURN ZERO
FPDzOVF  MOVEQ     #-1,D7    CREATE ALL ONES
         SWAP.W    D6        SIGN TO LOW BIT
         ROXR.B    #1,D6     SIGN TO X BIT
         ROXR.B    #1,D7     SIGN INTO HIGHEST POSSIBLE RESULT
         TST.B     D7        CLEAR CARRY BIT
         OR.B      #$02,CCR  SET OVERFLOW ON
         RTS                 RETURN TO CALLER WITH OVERFLOW


************************************
* (C) COPYRIGHT 1980 MOTORLA INC.  *
************************************

***********************************************************
*                        FFPAFP                           *
*                    ASCII TO FLOAT                       *
*                                                         *
*      INPUT:  A0 - POINTER TO ASCII STRING OF A FORMAT   *
*                   DESCRIBED BELOW                       *
*                                                         *
*      OUTPUT: D7 - FAST FLOATING POINT EQUIVALENT        *
*              A0 - POINTS TO THE CHARACTER WHICH         *
*                   TERMINATED THE SCAN                   *
*                                                         *
*      CONDITION CODES:                                   *
*                N - SET IF RESULT IS NEGATIVE            *
*                Z - SET IF RESULT IS ZERO                *
*                V - SET IF RESULT OVERFLOWED             *
*                C - SET IF INVALID FORMAT DETECTED       *
*                X - UNDEFINED                            *
*                                                         *
*      REGISTERS D3 THRU D6 ARE VOLATILE                  *
*                                                         *
*      CODE SIZE: 246 BYTES     STACK WORK: 8 BYTES       *
*                                                         *
*      INPUT FORMAT:                                      *
*                                                         *
*     {SIGN}{DIGITS}{'.'}{DIGITS}{'E'}{SIGN}{DIGITS}      *
*     <*********MANTISSA********><*****EXPONENT****>      *
*                                                         *
*                   SYNTAX RULES                          *
*          BOTH SIGNS ARE OPTIONAL AND ARE '+' OR '-'.    *
*          THE MANTISSA MUST BE PRESENT.                  *
*          THE EXPONENT NEED NOT BE PRESENT.              *
*          THE MANTISSA MAY LEAD WITH A DECIMAL POINT.    *
*          THE MANTISSA NEED NOT HAVE A DECIMAL POINT.    *
*                                                         *
*      EXAMPLES:  ALL OF THESE VALUES REPRESENT THE       *
*                 NUMBER ONE-HUNDRED-TWENTY.              *
*                                                         *
*                       120            .120E3             *
*                       120.          +.120E+03           *
*                      +120.          0.000120E6          *
*                   0000120.00  1200000E-4                *
*                               1200000.00E-0004          *
*                                                         *
*      FLOATING POINT RANGE:                              *
*                                                         *
*          FAST FLOATING POINT SUPPORTS THE VALUE ZERO    *
*          AND NON-ZERO VALUES WITHIN THE FOLLOWING       *
*          BOUNDS -                                       *
*                                                         *
*                   18                             -20    *
*    9.22337177 X 10   > +NUMBER >  5.42101070 X 10       *
*                                                         *
*                   18                             -20    *
*   -9.22337177 X 10   > -NUMBER > -2.71050535 X 10       *
*                                                         *
*      PRECISION:                                         *
*                                                         *
*          THIS CONVERSION RESULTS IN A 24 BIT PRECISION  *
*          WITH GUARANTEED ERROR LESS THAN OR EQUAL TO    *
*          ONE-HALF LEAST SIGNIFICANT BIT.                *
*                                                         *
*                                                         *
*      NOTES:                                             *
*          1) THIS ROUTINE CALLS THE DUAL-BINARY TO FLOAT *
*             ROUTINE AND CAN BE USED AS AN ILLUSTRATION  *
*             OF HOW TO 'FRONT-END' THAT ROUTINE WITH     *
*             A CUSTOMIZED SCANNER.                       *
*          2) UNDERFLOWS RETURN A ZERO WITHOUT ANY        *
*             INDICATORS SET.                             *
*          3) OVERFLOWS WILL RETURN THE MAXIMUM VALUE     *
*             POSSIBLE WITH PROPER SIGN AND THE 'V' BIT   *
*             SET IN THE CCR.                             *
*          4) IF THE 'C' BIT IN THE CCR INDICATES AN      *
*             INVALID PATTERN DETECTED, THEN A0 WILL      *
*             POINT TO THE INVALID CHARACTER.             *
*                                                         *
*      LOGIC SUMMARY:                                     *
*                                                         *
*          A) PROCESS LEADING SIGN                        *
*          B) PROCESS PRE-DECIMALPOINT DIGITS AND         *
*             INCREMENT 10 POWER BIAS FOR EACH            *
*             DIGIT BYPASSED DUE TO 32 BIT OVERFLOW       *
*          C) PROCESS POST-DECIMALPOINT DIGITS            *
*             DECREMENTING THE 10 POWER BIAS FOR EACH     *
*          D) PROCESS THE EXPONENT                        *
*          E) ADD THE 10 POWER BIAS TO THE EXPONENT       *
*          F) CALL 'FFPDBF' ROUTINE TO FINISH CONVERSION  *
*                                                         *
*   TIMES: (8 MHZ NO WAIT STATES)                         *
*          374 MICROSECONDS CONVERTING THE STRING         *
*                                                         *
*                                                         *
***********************************************************

         XDEF      FFPAFP    ENTRY POINT

*         XREF      FFPDBF

FFPAFP   MOVEQ     #0,D7     CLEAR MANTISSA BUILD
         MOVEQ     #0,D6     CLEAR SIGN+BASE10 BUILD

* CHECK FOR LEADING SIGN
         BSR       FPANXT    OBTAIN NEXT CHARACTER
         BEQ.S     FPANMB    BRANCH DIGIT FOUND
         BCS.S     FPANOS    BRANCH NO SIGN ENCOUNTERED

* LEADING SIGN ENCOUNTERED
         CMP.B     #'-',D5   COMPARE FOR MINUS
         SEQ.B     D6        SET ONES IF SO
         SWAP.W    D6        SIGN TO HIGH WORD IN D6

* TEST FOR DIGIT OR PERIOD
         BSR       FPANXT    OBTAIN NEXT CHARACTER
         BEQ.S     FPANMB    BRANCH DIGIT TO BUILD MANTISSA
FPANOS   CMP.B     #'.',D5   ? LEADING DECIMALPOINT
         BNE.S     FPABAD    BRANCH INVALID PATTERN IF NOT

* INSURE AT LEAST ONE DIGIT
         BSR       FPANXT    OBTAIN NEXT CHARACTER
         BEQ.S     FPADOF    BRANCH IF FRACTION DIGIT

* INVALID PATTERN DETECTED
FPABAD   SUBQ.L    #1,A0     POINT TO INVALID CHARACTER
         OR.B      #$01,CCR  FORCE CARRY BIT ON
         RTS                 RETURN TO CALLER

* PRE-DECIMALPOINT MANTISSA BUILD
FPANXD   BSR       FPANXT    NEXT CHARACTER
         BNE.S     FPANOD    BRANCH NOT A DIGIT
FPANMB   BSR.S     FPAX10    MULTIPLY TIMES TEN
         BCC.S     FPANXD    LOOP FOR MORE DIGITS

* PRE-DECIMALPOINT MANTISSA OVERFLOW, COUNT TILL END OR DECIMAL REACHED
FPAMOV   ADDQ.W    #1,D6     INCREMENT TEN POWER BY ONE
         BSR.S     FPANXT    OBTAIN NEXT PATTERN
         BEQ.S     FPAMOV    LOOP UNTIL NON-DIGIT
         CMP.B     #'.',D5   ? DECIMAL POINT REACHED
         BNE.S     FPATSE    NO, NO CHECK FOR EXPONENT

* FLUSH REMAINING FRACTIONAL DIGITS
FPASRD   BSR.S     FPANXT    NEXT CHARACTER
         BEQ.S     FPASRD    IGNORE IT IF STILL DIGIT
FPATSE   CMP.B     #'E',D5   ? EXPONENT HERE
         BEQ.S     FPAPEXP   YES, PROCESS EXPONENT
         CMP.B     #'e',D5   ? EXPONENT HERE
         BNE.S     FPACNV    NO, FINISHED - GO CONVERT

* NOW PROCESS THE EXPONENT
FPAPEXP  BSR.S     FPANXT    OBTAIN FIRST DIGIT
         BEQ.S     FPANTE    BRANCH GOT IT
         BCS.S     FPABAD    BRANCH INVALID FORMAT, NO SIGN OR DIGITS
         ROL.L     #8,D6     HIGH BYTE OF D6 INTO LOW
         CMP.B     #'-',D5   ? MINUS SIGN
         SEQ.B     D6        SET ONES OR ZERO
         ROR.L     #8,D6     D6 HIGH BYTE IS EXPONENTS SIGN
         BSR.S     FPANXT    NOW TO FIRST DIGIT
         BNE.S     FPABAD    BRANCH INVALID - DIGIT EXPECTED

* PROCESS EXPONENT'S DIGITS
FPANTE   MOVE.W    D5,D4     COPY DIGIT JUST LOADED
FPANXE   BSR.S     FPANXT    EXAMINE NEXT CHARACTER
         BNE.S     FPAFNE    BRANCH END OF EXPONENT
         MULU.W    #10,D4    PREVIOUS VALUE TIMES TEN
         CMP.W     #2000,D4  ? TOO LARGE
         BHI.S     FPABAD    BRANCH EXPONENT WAY OFF BASE
         ADD.W     D5,D4     ADD LATEST DIGIT
         BRA.S     FPANXE    LOOP FOR NEXT CHARACTER

* ADJUST FOR SIGN AND ADD TO ORIGINAL INDEX
FPAFNE   TST.L     D6        ? WAS EXPONENT NEGATIVE
         BPL.S     FPAADP    BRANCH IF SO
         NEG.W     D4        CONVERT TO NEGATIVE VALUE
FPAADP   ADD.W     D4,D6     FINAL RESULT
FPACNV   SUBQ.L    #1,A0     POINT TO TERMINATION CHARACTER
         BRA       FFPDBF    NOW CONVERT TO FLOAT

* PRE-DECIMALPOINT NON-DIGIT ENCOUNTERED
FPANOD   CMP.B     #'.',D5   ? DECIMAL POINT HERE
         BNE.S     FPATSE    NOPE, TRY FOR THE 'E'

* POST-DECIMALPOINT PROCESSING
FPADPN   BSR.S     FPANXT    OBTAIN NEXT CHARACTER
         BNE.S     FPATSE    NOT A DIGIT, TEST FOR 'E'
FPADOF   BSR.S     FPAX10    TIMES TEN PREVIOUS VALUE
         BCS.S     FPASRD    FLUSH IF OVERFLOW NOW
         SUBQ.W    #1,D6     ADJUST 10 POWER BIAS
         BRA.S     FPADPN    AND TO NEXT CHARACTER

*   *
*   * FPAX10 SUBROUTINE - PROCESS NEXT DIGIT
*   *  OUTPUT: C=0 NO OVERFLOW, C=1 OVERFLOW (D7 UNALTERED)
*   *
FPAX10   MOVE.L    D7,D3     COPY VALUE
         LSL.L     #1,D3     TIMES TWO
         BCS.S     FPAXRT    RETURN IF OVERFLOW
         LSL.L     #1,D3     TIMES FOUR
         BCS.S     FPAXRT    RETURN IF OVERFLOW
         LSL.L     #1,D3     TIMES EIGHT
         BCS.S     FPAXRT    RETURN IF OVERFLOW
         ADD.L     D7,D3     ADD ONE TO MAKE X 9
         BCS.S     FPAXRT    RETURN IF OVERFLOW
         ADD.L     D7,D3     ADD ONE TO MAKE X 10
         BCS.S     FPAXRT    RETURN IF OVERFLOW
         ADD.L     D5,D3     ADD NEW UNITS DIGIT
         BCS.S     FPAXRT    RETURN IF OVERFLOW
         MOVE.L    D3,D7     UPDATE RESULT
FPAXRT   RTS                 RETURN TO CALLER


*
* FPANXT SUBROUTINE - RETURN NEXT INPUT PATTERN
*
*    INPUT:  A0
*
*    OUTPUT:  A0 INCREMENTED BY ONE
*             IF Z=1 THEN DIGIT ENCOUNTERED AND D5.L SET TO BINARY VALUE
*             IF Z=0 THEN D6.B SET TO CHARACTER ENCOUNTERED
*                         AND C=0 IF PLUS OR MINUS SIGN
*                             C=1 IF NOT PLUS OR MINUS SIGN
*

FPANXT   MOVEQ     #0,D5     ZERO RETURN REGISTER
         MOVE.B    (A0)+,D5  LOAD CHARACTER
         CMP.B     #'+',D5   ? PLUS SIGN
         BEQ.S     FPASGN    BRANCH IF SIGN
         CMP.B     #'-',D5   ? MINUS SIGN
         BEQ.S     FPASGN    BRANCH IF SIGN
         CMP.B     #'0',D5   ? LOWER THAN A DIGIT
         BCS.S     FPAOTR    BRANCH IF NON-SIGNDIGIT
         CMP.B     #'9',D5   ? HIGHER THAN A DIGIT
         BHI.S     FPAOTR    BRANCH IF NON-SIGNDIGIT
* IT IS A DIGIT
         AND.B     #$0F,D5   TO BINARY
         MOVE.W    #$04,CCR  SET Z=1 FOR DIGIT
         RTS                 RETURN TO CALLER
* IT IS A SIGN
FPASGN   MOVE.W    #$00,CCR  SET Z=0 AND C=0
         RTS                 RETURN TO CALLER

* IT IS NEITHER SIGN NOR DIGIT
FPAOTR   MOVE.W    #$01,CCR  SET Z=0 AND C=1
         RTS       RETURN TO CALLER

*********************************************
* (C) COPYRIGHT 1980, 1983 BY MOTOROLA INC. *
*********************************************

*******************************************************
*                     FFPFPA                          *
*                 FLOAT TO ASCII                      *
*                                                     *
*    INPUT:  D7 - FLOATING POINT NUMBER               *
*                                                     *
*    OUTPUT: D7 - THE BASE TEN EXPONENT IN BINARY     *
*                 FOR THE RETURNED FORMAT             *
*            SP - DECREMENTED BY 14 AND               *
*                 POINTING TO THE CONVERTED           *
*                 NUMBER IN ASCII FORMAT              *
*                                                     *
*            ALL OTHER REGISTERS UNAFFECTED           *
*                                                     *
*    CONDITION CODES:                                 *
*            N - SET IF THE RESULT IS NEGATIVE        *
*            Z - SET IF THE RESULT IS ZERO            *
*            V - CLEARED                              *
*            C - CLEARED                              *
*            X - UNDEFINED                            *
*                                                     *
*   CODE SIZE: 192 BYTES   STACK WORK AREA: 42 BYTES  *
*                                                     *
*                                                     *
*            {S}{'.'}{DDDDDDDD}{'E'}{S}{DD}           *
*            <     FRACTION   >< EXPONENT >           *
*                                                     *
*        WHERE  S - SIGN OF MANTISSA OR EXPONENT      *
*                   ('+' OR '-')                      *
*               D - DECIMAL DIGIT                     *
*                                                     *
*        STACK OFFSET OF RESULT  S.DDDDDDDDESDD       *
*        AFTER RETURN            00000000001111       *
*                                01234567890123       *
*                                                     *
*                                                     *
*        EXAMPLES   +.12000000E+03  120               *
*                   +.31415927E+01  PI                *
*                   +.10000000E-01  ONE-HUNDREDTH     *
*                   -.12000000E+03  MINUS 120         *
*                                                     *
*     NOTES:                                          *
*       1) THE BINARY BASE 10 EXPONENT IS RETURNED    *
*          IN D7 TO FACILITATE CONVERSIONS TO         *
*          OTHER FORMATS.                             *
*       2) EVEN THOUGH EIGHT DIGITS ARE RETURNED, THE *
*          PRECISION AVAILABLE IS ONLY 7.167 DIGITS.  *
*          ROUNDING SHOULD BE PERFORMED WHEN LESS     *
*          THAN EIGHT DIGITS ARE ACTUALLY UTILIZED    *
*          IN THE MANTISSA.                           *
*       3) THE STACK IS LOWERED BY 14 BYTES BY THIS   *
*          ROUTINE.  THE RETURN ADDRESS TO THE CALLER *
*          IS REPLACED BY A PORTION OF THE RESULTS.   *
*                                                     *
*  TIME: (8MHZ NO WAIT STATES ASSUMED)                *
*        330 MICROSECONDS CONVERTING THE SAMPLE FLOAT *
*        VALUE OF 55.55 TO ASCII.                     *
*                                                     *
*******************************************************

         XDEF      FFPFPA   ENTRY POINT
*         XREF      FFP10TBL POWER OF TEN TABLE

* STACK DEFINITION
STKOLD   EQU       48       PREVIOUS CALLERS STACK POINTER
STKEXP   EQU       46       EXPONENT
STKEXPS  EQU       45       EXPONENTS SIGN
STKLTRE  EQU       44       'E'
STKMANT  EQU       36       MANTISSA
STKPER   EQU       35       '.'
STKMANS  EQU       34       MANTISSA'S SIGN
STKNEWRT EQU       30       NEW RETURN POSITION
STKRTCC  EQU       28       RETURN CONDITION CODE
STKSAVE  EQU       0        REGISTER SAVE AREA


FFPFPA   LEA       -10(SP),SP   SET STACK TO NEW LOCATION
         MOVE.L    10(SP),-(SP) SAVE RETURN
         TST.B     D7           TEST VALUE
*         MOVE      SR,-(SP)     SAVE CCR OF RESULT ON THE STACK, not 68010 compatible
         JSR       -$0210(A6)   GETCC()
         MOVE.W    D0,-(SP)     SAVE CCR OF RESULT ON THE STACK
         MOVEM.L   D2-D6/A0/A1,-(SP)  SAVE WORK ADDRESS REGISTER
         TST.B     D7           RETEST VALUE SINCE GETCC CHANGES CCR

* ADJUST FOR ZERO VALUE
         BNE.S     FPFNOT0   BRANCH NO ZERO INPUT
         MOVEQ     #$41,D7   SETUP PSEUDO INTEGER EXPONENT

* SETUP MANTISSA'S SIGN
FPFNOT0  MOVE.W    #'+.',STKMANS(SP) INSERT PLUS AND DECIMAL
         MOVE.B    D7,D6     COPY SIGN+EXPONENT
         BPL.S     FPFPLS    BRANCH IF PLUS
         ADDQ.B    #2,STKMANS(SP) CHANGE PLUS TO MINUS

* START SEARCH FOR MAGNITUDE IN BASE 10 POWER TABLE
FPFPLS   ADD.B     D6,D6     SIGN OUT OF PICTURE
         MOVE.B    #$80,D7   SET ROUDING FACTOR FOR SEARCH
         EOR.B     D7,D6     CONVERT EXPONENT TO BINARY
         EXT.W     D6        EXPONENT TO WORD
         ASR.W     #1,D6     BACK FROM SIGN EXTRACTMENT
         MOVEQ     #1,D3     START BASE TEN COMPUTATION
         LEA       (FFP10TBL,PC),A0 START AT TEN TO THE ZERO
         CMP.W     (A0),D6   COMPARE TO TABLE
         BLT.S     FPFMIN    BRANCH MINUS EXPONENT
         BGT.S     FPFPLU    BRANCH PLUS EXPONENT
FPFEQE   CMP.L     2(A0),D7  EQUAL SO COMPARE MANTISSA'S
         BCC.S     FPFFND    BRANCH IF INPUT GREATER OR EQUAL THAN TABLE
FPFBCK   ADDQ.W    #6,A0     TO NEXT LOWER ENTRY IN TABLE
         SUBQ.W    #1,D3     DECREMENT BASE 10 EXPONENT
         BRA.S     FPFFND    BRANCH POWER OF TEN FOUND

* EXPONENT IS HIGHER THAN TABLE
FPFPLU   LEA       -6(A0),A0 TO NEXT HIGHER ENTRY
         ADDQ.W    #1,D3     INCREMENT POWER OF TEN
         CMP.W     (A0),D6   TEST NEW MAGNITUDE
         BGT.S     FPFPLU    LOOP IF STILL GREATER
         BEQ.S     FPFEQE    BRANCH EQUAL EXPONENT
         BRA.S     FPFBCK    BACK TO LOWER AND FOUND

* EXPONENT IS LOWER THAN TABLE
FPFMIN   LEA       6(A0),A0  TO NEXT LOWER ENTRY
         SUBQ.W    #1,D3     DECREMENT POWER OF TEN BY ONE
         CMP.W     (A0),D6   TEST NEW MAGNITUDE
         BLT.S     FPFMIN    LOOP IF STILL LESS THAN
         BEQ.S     FPFEQE    BRANCH EQUAL EXPONENT

* CONVERT THE EXPONENT TO ASCII
FPFFND   MOVE.L    #'E+00',STKLTRE(SP) SETUP EXPONENT PATTERN
         MOVE.W    D3,D2     ? EXPONENT POSITIVE
         BPL.S     FPFPEX    BRANCH IF SO
         NEG.W     D2        ABSOLUTIZE
         ADDQ.B    #2,STKEXPS(SP) TURN TO MINUS SIGN
FPFPEX   CMP.W     #10,D2    ? TEN OR GREATER
         BCS.S     FPFGEN    BRANCH IF NOT
         ADDQ.B    #1,STKEXP(SP) CHANGE ZERO TO A ONE
         SUB.W     #10,D2    ADJUST TO DECIMAL
FPFGEN   OR.B      D2,STKEXP+1(SP) FILL IN LOW DIGIT

* GENERATE THE MANTISSA IN ASCII A0->TABLE  D7=BINARY MANTISSA
* D5 - MANTISSA FROM TABLE       D6.W = BINARY EXPONENT
* D4 - SHIFT AND DIGIT BUILDER   D2 = DBRA MANTISSA DIGIT COUNT
* A1->MANTISSA STACK POSITION
         MOVEQ     #7,D2     COUNT FOR EIGHT DIGITS
         LEA       STKMANT(SP),A1 POINT TO MANTISSA START
         TST.L     D7        ? ZERO TO CONVERT
         BPL.S     FPFZRO    BRANCH IF SO TO NOT ROUND
         TST.B     5(A0)     ? 24 BIT PRECISE IN TABLE
         BNE.S     FPFNXI    BRANCH IF NO TRAILING ZEROES
FPFZRO   CLR.B     D7        CLEAR ADJUSTMENT FOR .5 LSB PRECISION
FPFNXI   MOVE.W    D6,D4     COPY BINARY EXPONENT
         SUB.W     (A0)+,D4  FIND NORMALIZATION FACTOR
         MOVE.L    (A0)+,D5  LOAD MANTISSA FROM TABLE
         LSR.L     D4,D5     ADJUST TO SAME EXPONENT
         MOVEQ     #9,D4     START AT NINE AND COUNT DOWN
FPFINC   SUB.L     D5,D7     SUBTRACT FOR ANOTHER COUNT
         DBCS      D4,FPFINC DECREMENT AND BRANCH IF OVER
         BCS.S     FPFNIM    BRANCH NO IMPRECISION
         CLR.B     D4        CORRECT RARE UNDERFLOW DUE TO TABLE IMPRECISION
FPFNIM   ADD.L     D5,D7     MAKE UP FOR OVER SUBTRACTION
         SUB.B     #9,D4     CORRECT VALUE
         NEG.B     D4        TO BETWEEN 0 AND 9 BINARY
         OR.B      #'0',D4   CONVERT TO ASCII
         MOVE.B    D4,(A1)+  INSERT INTO ASCII MANTISSA PATTERN
         DBRA      D2,FPFNXI BRANCH IF MORE DIGITS TO GO

* RETURN WITH BASE TEN EXPONENT BINARY IN D7
         MOVE.W    D3,D7     TO D7
         EXT.L     D7        TO FULL WORD
         MOVEM.L   (SP)+,D2-D6/A0/A1 RESTORE WORK REGISTERS
         RTR       RETURN WITH PROPER CONDITION CODE

****************************************
* (C) COPYRIGHT 1981 BY MOTOROLA INC.  *
****************************************

***********************************************
*                  FFPARND                    *
*           ASCII ROUND SUBROUTINE            *
*                                             *
*  THIS ROUTINE IS NORMALLY CALLED AFTER THE  *
*  'FFPFPA' FLOAT TO ASCII ROUTINE AND ACTS   *
*  UPON ITS RESULTS.                          *
*                                             *
*  INPUT:  D6 - ROUNDING MAGNITUDE IN BINARY  *
*               AS EXPLAINED BELOW.           *
*          D7 - BINARY REPRESENTATION OF THE  *
*               BASE 10 EXPONENT.             *
*          SP ->  RETURN ADDRESS AND OUTPUT   *
*                 FROM FFPFPA ROUTINE         *
*                                             *
*  OUTPUT: THE ASCII VALUE ON THE STACK IS    *
*          CORRECTLY ROUNDED                  *
*                                             *
*          THE CONDITION CODES ARE UNDEFINED  *
*                                             *
*          ALL REGISTERS TRANSPARENT          *
*                                             *
*     THE ROUNDING PRECISION REPRESENTS THE   *
*     POWER OF TEN TO WHICH THE ROUNDING WILL *
*     OCCUR.  (I.E. A -2 MEANS ROUND THE DIGIT*
*     IN THE HUNDREDTH POSITION FOR RESULTANT *
*     ROUNDING TO TENTHS.)  A POSITIVE VALUE  *
*     INDICATES ROUNDING TO THE LEFT OF THE   *
*     DECIMAL POINT (0 IS UNITS, 1 IS TENS    *
*     E.T.C.)                                 *
*                                             *
*     THE BASE TEN EXPONENT IN BINARY IS D7   *
*     FROM THE 'FFPFPA' ROUTINE OR COMPUTED BY*
*     THE CALLER.                             *
*                                             *
*     THE STACK CONTAINS THE RETURN ADDRESS   *
*     FOLLOWED BY THE ASCII NUMBER AS FROM    *
*     THE 'FFPFPA' ROUTINE.  SEE THE          *
*     DESCRIPTION OF THAT ROUTINE FOR THE     *
*     REQUIRED FORMAT.                        *
*                                             *
*  EXAMPLE:                                   *
*                                             *
*  INPUT PATTERN '+.98765432+01' = 9.8765432  *
*                                             *
*     ROUND +1 IS +.00000000+00 =  0.         *
*     ROUND  0 IS +.10000000+02 = 10.         *
*     ROUND -1 IS +.10000000+02 = 10.         *
*     ROUND -2 IS +.99000000+01 =  9.9        *
*     ROUND -3 IS +.98800000+01 =  9.88       *
*     ROUND -6 IS +.98765400+01 =  9.87654    *
*                                             *
*  NOTES:                                     *
*     1) IF THE ROUNDING DIGIT IS TO THE LEFT *
*        OF THE MOST SIGNIFICANT DIGIT, A ZERO*
*        RESULTS.  IF THE ROUNDING DIGIT IS TO*
*        THE RIGHT OF THE LEAST SIGNIFICANT   *
*        DIGIT, THEN NO ROUNDING OCCURS       *
*     2) ROUNDING IS HANDY FOR ELIMINATING THE*
*        DANGLING '999...' PROBLEM COMMON WITH*
*        FLOAT TO DECIMAL CONVERSIONS.        *
*     3) POSITIONS FROM THE ROUNDED DIGIT AND *
*        TO THE RIGHT ARE SET TO ZEROES.      *
*     4) THE EXPONENT MAY BE AFFECTED.        *
*     5) ROUNDING IS FORCED BY ADDING FIVE.   *
*     6) THE BINARY EXPONENT IN D7 MAY BE     *
*        PRE-BIASED BY THE CALLER TO PROVIDE  *
*        ENHANCED EDITING CONTROL.            *
*     7) THE RETURN ADDRESS IS REMOVED FROM   *
*        THE STACK UPON EXIT.                 *
***********************************************

         XDEF      FFPARND   ENTRY POINT

FFPARND  MOVEM.L   D7/A0,-(SP)         SAVE WORK ON STACK
         SUB.W     D6,D7               COMPUTE ROUNDING DIGIT OFFSET
         BLE.S     FAFZRO              BRANCH IF LARGER THAN VALUE
         CMP.W     #8,D7               INSURE NOT PAST LAST DIGIT
         BHI       FARTN               RETURN IF SO
         LEA       8+4+1(SP,D7),A0       POINT TO ROUNDING DIGIT
         CMP.B     #'5',(A0)           ? MUST ROUND UP
         BCC.S     FADORND             YEP - GO ROUND
         SUBQ.W    #1,D7               ? ROUND LEADING DIGIT ZERO (D7=1)
         BNE       FAZEROL             NOPE, JUST ZERO OUT
FAFZRO   LEA       8+4+2(SP),A0        FORCE ZEROES ALL THE WAY ACROSS
         MOVE.L    #'E+00',8+4+10(SP)  FORCE ZERO EXPONENT
         MOVE.B    #'+',8+4(SP)        ZERO IS ALWAYS POSITIVE
         BRA.S     FAZEROL            ZERO MANTISSA THEN RETURN

* ROUND UP MUST OCCUR
FADORND  MOVE.L    A0,-(SP)            SAVE ZERO START ADDRESS ON STACK
FACARRY  CMP.B     #'.',-(A0)          ? HIT BEGINNING
         BEQ.S     FASHIFT             YES, MUST SHIFT DOWN
         ADDQ.B    #1,(A0)             UP BY ONE
         CMP.B     #'9'+1,(A0)         ? PAST NINE
         BNE.S     FAZERO              NO, NOW ZERO THE END
         MOVE.B    #'0',(A0)           FORCE ZERO FOR OVERFLOW
         BRA       FACARRY             LOOP FOR CARRY

* OVERFLOW PAST TOP DIGIT - SHIFT RIGHT AND UP EXPONENT
FASHIFT  ADDQ.L    #1,(SP)             ZERO PADD STARTS ONE LOWER NOW
         ADDQ.L    #1,A0               BACK TO LEADING DIGIT
         MOVEQ     #$31,D7             DEFAULT FIRST DIGIT ASCII ONE
         SWAP.W    D7                  INITIALIZE OLD DIGIT
         MOVE.B    (A0),D7             PRE-LOAD CURRENT DIGIT
FASHFTR  SWAP.W    D7                  TO PREVIOUS DIGIT
         MOVE.B    D7,(A0)+            STORE INTO THIS POSITION
         MOVE.B    (A0),D7             LOAD UP NEXT DIGIT
         CMP.B     #'E',D7             ? THE END
         BNE.S     FASHFTR             NO, SHIFT ANOTHER TO THE RIGHT

* INCREMENT EXPONENT FOR SHIFT RIGHT
         CMP.B     #'+',1(A0)          ? POSITIVE EXPONENT
         ADDQ.L    #3,A0               POINT TO LEAST EXP DIGIT
         BNE.S     FANGEXP             BRANCH NEGATIVE EXPONENT
         ADDQ.B    #1,(A0)             ADD ONE TO EXPONENT
         CMP.B     #'9'+1,(A0)         ? OVERFLOW PAST NINE
         BNE.S     FAZERO              NO, NOW ZERO
         ADDQ.B    #1,-(A0)            CARRY TO NEXT DIGIT
         BRA.S     FAZERO              AND NOW ZERO END
FANGEXP  CMP.W     #'01',-1(A0)        ? GOING FROM -1 TO +0
         BNE.S     FANGOK              BRANCH IF NOT
         MOVE.B    #'+',-2(A0)         CHANGE MINUS TO PLUS
FANGOK   SUBQ.B    #1,(A0)             SUBTRACT ONE FROM EXPONENT
         CMP.B     #'0'-1,(A0)         ? UNDERFLOW BELOW ZERO
         BNE.S     FAZERO              NO, ZERO REMAINDER
         SUBQ.B    #1,-(A0)            BORROW FROM NEXT DIGIT

* ZERO THE DIGITS PAST PRECISION REQUIRED
FAZERO   MOVE.L    (SP)+,A0            RELOAD SAVED PRECISION
FAZEROL  CMP.B     #'E',(A0)           ? AT END
         BEQ.S     FARTN               BRANCH IF SO
         MOVE.B    #'0',(A0)+          ZERO NEXT DIGIT
         BRA.S     FAZEROL             AND TEST AGAIN

* RETURN TO THE CALLER
FARTN    MOVEM.L   (SP)+,D7/A0         RESTORE REGISTERS
         RTS                           RETURN TO CALLER

  nolist 
*  xrefoff
 ttl 'fast floating point float to bcd (ffpfpbcd)'
***************************************
* (c) copyright 1980 by motorola inc. *
***************************************
 
*******************************************************
*                   ffpfpbcd                          *
*                 float to bcd                        *
*                                                     *
*    input:  d7 - floating point number               *
*                                                     *
*    output: sp - decremented by 8 and                *
*                 pointing to the converted           *
*                 number in bcd format as described   *
*                 below -                             *
*                                                     *
*                mmmm s e s b                         *
*                                                     *
*        where  m - four bytes of bcd, each with two  *
*                   digits of the mantissa (8 digits) *
*               s - sign of mantissa ($00 +, $ff -)   *
*               e - bcd byte for two digit exponent   *
*               s - sign of exponent ($00 +, $ff -)   *
*               b - one byte binary 2's complement    *
*                   representation of the exponent    *
*                                                     *
*        stack offset of result  mmmmsesb             *
*        after return            00000000             *
*                                01234567             *
*                                                     *
*                    m m m m s e s b                  *
*        examples  $1200000000030003 +120             *
*                  $3141592700010001 3.14159          *
*                  $12000000ff030003 -120             *
*                  $10000000ff10000a -1,000,000,000   *
*                  $100000000002fffe .001 (.1e-2)     *
*                    m m m m s e s b                  *
*                                                     *
*    condition codes:                                 *
*            n - set if the result is negative        *
*            z - set if the result is zero            *
*            v - cleared                              *
*            c - cleared                              *
*            x - undefined                            *
*                                                     *
*    all registers are transparent                    *
*                                                     *
*    code size: 168 bytes     stack work: 72 bytes    *
*                                                     *
*     notes:                                          *
*       1) even though eight digits are returned, the *
*          precision available is only 7.167 digits.  *
*          rounding should be performed when less     *
*          than the full eight digits are actually    *
*          used in the mantissa.                      *
*       2) the stack is lowered by 8 bytes by this    *
*          routine.  the return address to the caller *
*          is replaced by a portion of the results.   *
*       3) the binary base 10 exponent is returned    *
*          to facilitate conversions to other formats *
*                                                     *
*   time: (8mhz no wait states assumed)               *
*        approximately 330 microseconds converting    *
*        the float value 55.55 to bcd.                *
*                                                     *
*******************************************************


*ffpfpbcd idnt      1,1  ffp float to bcd
 
 entry      ffpfpbcd            entry point
* extern      ffp10tbl,ffpcpyrt power of ten table
 
* ?          opt       pcs
 
 
* stack definition
stkold   equ       46        previous callers stack pointer
stkbexp  equ       45        exponent in binary
stkexps  equ       44        exponents sign
stkexp   equ       43        exponent in bcd
stkmans  equ       42        mantissa's sign
stkmant  equ       38        mantissa in bcd
stknewrt equ       34        new return position
stkrtcc  equ       32        return condition code
stksave  equ       0         register save area
 
 
ffpfpbcd lea       0-4(a7),a7 set stack to new location
	 move.l    4(a7),-(a7)   save return
         tst.b     d7        test value
*         move.w    sr,-(a7)  save for return code
         jsr       -$0210(A6)   getcc()
         move.w    d0,-(a7)     save ccr of result on the stack         
         movem.l   d2-d7/a0/a1,-(a7)  save work address register
         tst.b     d7           restest value since getcc changes ccr
 
* adjust for zero value
         bne.s     fpfnot0   branch no zero input
         move.l    #$41,d7   setup psuedo integer exponent
 
* setup mantissa's sign
fpfnot0  move.b    d7,d6     copy sign+exponent
         smi     stkmans(a7) set results sign
 
* start search for magnitude in base 10 power table
         add.b     d6,d6     sign out of picture
         move.b    #$80,d7   set rouding factor for search
         eor.b     d7,d6     convert exponent to binary
         ext.w     d6        exponent to word
         asr.w     #1,d6     back from sign extractment
         move.l    #1,d3     start base ten computation
         lea       FFP10TBL,a0 start at ten to the zero
         cmp.w     (a0),d6   compare to table
         blt.s     fpfmin    branch minus exponent
         bgt.s     fpfplu    branch plus exponent
fpfeqe   cmp.l     2(a0),d7  equal so compare mantissa's
         bcc.s     fpffnd    branch if input greater or equal than table
fpfbck   adda.w     #6,a0     to next lower entry in table
         subi.w     #1,d3     decrement base 10 exponent
         bra.s     fpffnd    branch power of ten found
 
* exponent is higher than table
fpfplu   lea       0-6(a0),a0 to next higher entry
         addi.w     #1,d3     increment power of ten
         cmp.w     (a0),d6   test new magnitude
         bgt.s     fpfplu    loop if still greater
         beq.s     fpfeqe    branch equal exponent
         bra.s     fpfbck    back to lower and found
 
* exponent is lower than table
fpfmin   lea       6(a0),a0  to next lower entry
         subi.w     #1,d3     decrement power of ten by one
         cmp.w     (a0),d6   test new magnitude
         blt.s     fpfmin    loop if still less than
         beq.s     fpfeqe    branch equal exponent
 
* convert the exponent to bcd
fpffnd   move.b    d3,stkbexp(a7) fill in binary exponent
         smi     stkexps(a7) and set exponents sign
         bpl.s     fpfpex    branch if positive
         neg.w     d3        absolutize
fpfpex   clr.b     d2        create a zeroter
         add.b     d2,d2     clear x bit
         abcd    d3,d2     convert to bcd
         move.b    d2,stkexp(a7) return exponent in bcd
 
* generate the mantissa in ascii a0->table  d7=binary mantissa
* d5 - mantissa from table       d6.w = binary exponent
* d4 - shift and digit builder   d2 = dbra mantissa digit count
* a1->mantissa stack position
         move.l    #7,d2     count for eight digits
         move.l    #0,d3     zero bcd longword build value
         tst.l     d7        Q zero to convert
         bpl.s     fpfzro    branch if so to not round
         tst.b     5(a0)     Q 24 bit precise in table
         bne.s     fpfnxi    branch if no trailing zeroes
fpfzro   clr.b     d7        clear adjustment for .5 lsb precision
fpfnxi   asl.l     #4,d3     shift for next digit generation
         move.w    d6,d4     copy binary exponent
         sub.w     (a0)+,d4  find normalization factor
         move.l    (a0)+,d5  load mantissa from table
         lsr.l     d4,d5     adjust to same exponent
         move.l    #9,d4     start at nine and count down
fpfinc   sub.l     d5,d7     subtract for another count
         dbcs      d4,fpfinc decrement and branch if over
         bcs.s     fpfnir    branch proper execution
         clr.b     d4        correct rare error due to table imprecision
fpfnir   add.l     d5,d7     make up for over subtraction
         subi.b     #9,d4     correct value
         neg.b     d4        to between 0 and 9 binary
         or.b      d4,d3     insert as next digit
         dbra      d2,fpfnxi branch if more digits to go
         move.l    d3,stkmant(a7) store mantissa bcd result
 
* return
         movem.l   (a7)+,d2-d7/a0/a1 restore work registers
         rtr       return with proper condition code
 
 
         end
