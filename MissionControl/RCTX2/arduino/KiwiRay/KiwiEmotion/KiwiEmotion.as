opt subtitle "HI-TECH Software Omniscient Code Generator (PRO mode) build 10920"

opt pagewidth 120

	opt pm

	processor	16F1825
clrc	macro
	bcf	3,0
	endm
clrz	macro
	bcf	3,2
	endm
setc	macro
	bsf	3,0
	endm
setz	macro
	bsf	3,2
	endm
skipc	macro
	btfss	3,0
	endm
skipz	macro
	btfss	3,2
	endm
skipnc	macro
	btfsc	3,0
	endm
skipnz	macro
	btfsc	3,2
	endm
indf	equ	0
indf0	equ	0
indf1	equ	1
pc	equ	2
pcl	equ	2
status	equ	3
fsr0l	equ	4
fsr0h	equ	5
fsr1l	equ	6
fsr1h	equ	7
bsr	equ	8
wreg	equ	9
intcon	equ	11
c	equ	1
z	equ	0
pclath	equ	10
	FNCALL	_main,_setup
	FNROOT	_main
	FNCALL	intlevel1,_isr
	global	intlevel1
	FNROOT	intlevel1
	global	_i2c_packet
	global	_i2c_bytes
	global	_INTCON
psect	intentry,class=CODE,delta=2
global __pintentry
__pintentry:
_INTCON	set	11
	global	_PORTA
_PORTA	set	12
	global	_PORTC
_PORTC	set	14
	global	_RC2
_RC2	set	114
	global	_RC3
_RC3	set	115
	global	_RC4
_RC4	set	116
	global	_RC5
_RC5	set	117
	global	_SSP1IF
_SSP1IF	set	139
	global	_OPTION_REG
_OPTION_REG	set	149
	global	_OSCCON
_OSCCON	set	153
	global	_TRISA
_TRISA	set	140
	global	_TRISC
_TRISC	set	142
	global	_SSP1IE
_SSP1IE	set	1163
	global	_ANSELA
_ANSELA	set	396
	global	_ANSELC
_ANSELC	set	398
	global	_SSPADD
_SSPADD	set	530
	global	_SSPBUF
_SSPBUF	set	529
	global	_SSPCON1
_SSPCON1	set	533
	global	_SSPCON2
_SSPCON2	set	534
	global	_SSPCON3
_SSPCON3	set	535
	global	_WPUA
_WPUA	set	524
	global	_WPUC
_WPUC	set	526
	global	_ACKSTAT
_ACKSTAT	set	4278
	global	_BF
_BF	set	4256
	global	_CKP
_CKP	set	4268
	global	_D_nA
_D_nA	set	4261
	global	_R_nW
_R_nW	set	4258
	global	_INLVLA
_INLVLA	set	908
	global	_INLVLC
_INLVLC	set	910
	file	"KiwiEmotion.as"
	line	#
psect cinit,class=CODE,delta=2
global start_initialization
start_initialization:

psect	bssCOMMON,class=COMMON,space=1
global __pbssCOMMON
__pbssCOMMON:
_i2c_bytes:
       ds      1

psect	bssBANK0,class=BANK0,space=1
global __pbssBANK0
__pbssBANK0:
_i2c_packet:
       ds      32

psect clrtext,class=CODE,delta=2
global clear_ram
;	Called with FSR0 containing the base address, and
;	WREG with the size to clear
clear_ram:
	clrwdt			;clear the watchdog before getting into this loop
clrloop:
	clrf	indf0		;clear RAM location pointed to by FSR
	addfsr	0,1
	decfsz wreg		;Have we reached the end of clearing yet?
	goto clrloop	;have we reached the end yet?
	retlw	0		;all done for this memory range, return
; Clear objects allocated to COMMON
psect cinit,class=CODE,delta=2
	global __pbssCOMMON
	clrf	((__pbssCOMMON)+0)&07Fh
; Clear objects allocated to BANK0
psect cinit,class=CODE,delta=2
	global __pbssBANK0
	movlw	low(__pbssBANK0)
	movwf	fsr0l
	movlw	high(__pbssBANK0)
	movwf	fsr0h
	movlw	020h
	fcall	clear_ram
psect cinit,class=CODE,delta=2
global end_of_initialization

;End of C runtime variable initialization code

end_of_initialization:
movlb 0
ljmp _main	;jump to C main() function
psect	cstackCOMMON,class=COMMON,space=1
global __pcstackCOMMON
__pcstackCOMMON:
	global	??_isr
??_isr:	; 0 bytes @ 0x0
	global	?_setup
?_setup:	; 0 bytes @ 0x0
	global	?_main
?_main:	; 0 bytes @ 0x0
	global	?_isr
?_isr:	; 2 bytes @ 0x0
	global	isr@data
isr@data:	; 1 bytes @ 0x0
	ds	1
	global	??_setup
??_setup:	; 0 bytes @ 0x1
	global	setup@n
setup@n:	; 1 bytes @ 0x1
	ds	1
	global	??_main
??_main:	; 0 bytes @ 0x2
	global	main@row
main@row:	; 1 bytes @ 0x2
	ds	1
	global	main@rowix
main@rowix:	; 1 bytes @ 0x3
	ds	1
	global	main@q
main@q:	; 1 bytes @ 0x4
	ds	1
	global	main@n
main@n:	; 1 bytes @ 0x5
	ds	1
;;Data sizes: Strings 0, constant 0, data 0, bss 33, persistent 0 stack 0
;;Auto spaces:   Size  Autos    Used
;; COMMON          14      6       7
;; BANK0           80      0      32
;; BANK1           80      0       0
;; BANK2           80      0       0
;; BANK3           80      0       0
;; BANK4           80      0       0
;; BANK5           80      0       0
;; BANK6           80      0       0
;; BANK7           80      0       0
;; BANK8           80      0       0
;; BANK9           80      0       0
;; BANK10          80      0       0
;; BANK11          80      0       0
;; BANK12          48      0       0

;;
;; Pointer list with targets:



;;
;; Critical Paths under _main in COMMON
;;
;;   _main->_setup
;;
;; Critical Paths under _isr in COMMON
;;
;;   None.
;;
;; Critical Paths under _main in BANK0
;;
;;   None.
;;
;; Critical Paths under _isr in BANK0
;;
;;   None.
;;
;; Critical Paths under _main in BANK1
;;
;;   None.
;;
;; Critical Paths under _isr in BANK1
;;
;;   None.
;;
;; Critical Paths under _main in BANK2
;;
;;   None.
;;
;; Critical Paths under _isr in BANK2
;;
;;   None.
;;
;; Critical Paths under _main in BANK3
;;
;;   None.
;;
;; Critical Paths under _isr in BANK3
;;
;;   None.
;;
;; Critical Paths under _main in BANK4
;;
;;   None.
;;
;; Critical Paths under _isr in BANK4
;;
;;   None.
;;
;; Critical Paths under _main in BANK5
;;
;;   None.
;;
;; Critical Paths under _isr in BANK5
;;
;;   None.
;;
;; Critical Paths under _main in BANK6
;;
;;   None.
;;
;; Critical Paths under _isr in BANK6
;;
;;   None.
;;
;; Critical Paths under _main in BANK7
;;
;;   None.
;;
;; Critical Paths under _isr in BANK7
;;
;;   None.
;;
;; Critical Paths under _main in BANK8
;;
;;   None.
;;
;; Critical Paths under _isr in BANK8
;;
;;   None.
;;
;; Critical Paths under _main in BANK9
;;
;;   None.
;;
;; Critical Paths under _isr in BANK9
;;
;;   None.
;;
;; Critical Paths under _main in BANK10
;;
;;   None.
;;
;; Critical Paths under _isr in BANK10
;;
;;   None.
;;
;; Critical Paths under _main in BANK11
;;
;;   None.
;;
;; Critical Paths under _isr in BANK11
;;
;;   None.
;;
;; Critical Paths under _main in BANK12
;;
;;   None.
;;
;; Critical Paths under _isr in BANK12
;;
;;   None.

;;
;;Main: autosize = 0, tempsize = 0, incstack = 0, save=0
;;

;;
;;Call Graph Tables:
;;
;; ---------------------------------------------------------------------------------
;; (Depth) Function   	        Calls       Base Space   Used Autos Params    Refs
;; ---------------------------------------------------------------------------------
;; (0) _main                                                11    11      0     255
;;                                              2 COMMON     4     4      0
;;                              _setup
;; ---------------------------------------------------------------------------------
;; (1) _setup                                                1     1      0      45
;;                                              1 COMMON     1     1      0
;; ---------------------------------------------------------------------------------
;; Estimated maximum stack depth 1
;; ---------------------------------------------------------------------------------
;; (Depth) Function   	        Calls       Base Space   Used Autos Params    Refs
;; ---------------------------------------------------------------------------------
;; (2) _isr                                                  1     1      0      15
;;                                              0 COMMON     1     1      0
;; ---------------------------------------------------------------------------------
;; Estimated maximum stack depth 2
;; ---------------------------------------------------------------------------------

;; Call Graph Graphs:

;; _main (ROOT)
;;   _setup
;;
;; _isr (ROOT)
;;

;; Address spaces:

;;Name               Size   Autos  Total    Cost      Usage
;;BIGRAM             3F0      0       0       0        0.0%
;;EEDATA             100      0       0       0        0.0%
;;NULL                 0      0       0       0        0.0%
;;CODE                 0      0       0       0        0.0%
;;BITCOMMON            E      0       0       1        0.0%
;;BITSFR0              0      0       0       1        0.0%
;;SFR0                 0      0       0       1        0.0%
;;COMMON               E      6       7       2       50.0%
;;BITSFR1              0      0       0       2        0.0%
;;SFR1                 0      0       0       2        0.0%
;;BITSFR2              0      0       0       3        0.0%
;;SFR2                 0      0       0       3        0.0%
;;STACK                0      0       1       3        0.0%
;;BITBANK0            50      0       0       4        0.0%
;;BITSFR3              0      0       0       4        0.0%
;;SFR3                 0      0       0       4        0.0%
;;BANK0               50      0      20       5       40.0%
;;BITSFR4              0      0       0       5        0.0%
;;SFR4                 0      0       0       5        0.0%
;;BITBANK1            50      0       0       6        0.0%
;;BITSFR5              0      0       0       6        0.0%
;;SFR5                 0      0       0       6        0.0%
;;BANK1               50      0       0       7        0.0%
;;BITSFR6              0      0       0       7        0.0%
;;SFR6                 0      0       0       7        0.0%
;;BITBANK2            50      0       0       8        0.0%
;;BITSFR7              0      0       0       8        0.0%
;;SFR7                 0      0       0       8        0.0%
;;BANK2               50      0       0       9        0.0%
;;BITSFR8              0      0       0       9        0.0%
;;SFR8                 0      0       0       9        0.0%
;;BITBANK3            50      0       0      10        0.0%
;;BITSFR9              0      0       0      10        0.0%
;;SFR9                 0      0       0      10        0.0%
;;BANK3               50      0       0      11        0.0%
;;BITSFR10             0      0       0      11        0.0%
;;SFR10                0      0       0      11        0.0%
;;BITBANK4            50      0       0      12        0.0%
;;BITSFR11             0      0       0      12        0.0%
;;SFR11                0      0       0      12        0.0%
;;BANK4               50      0       0      13        0.0%
;;BITSFR12             0      0       0      13        0.0%
;;SFR12                0      0       0      13        0.0%
;;BITBANK5            50      0       0      14        0.0%
;;BITSFR13             0      0       0      14        0.0%
;;SFR13                0      0       0      14        0.0%
;;BANK5               50      0       0      15        0.0%
;;BITSFR14             0      0       0      15        0.0%
;;SFR14                0      0       0      15        0.0%
;;BITBANK6            50      0       0      16        0.0%
;;BITSFR15             0      0       0      16        0.0%
;;SFR15                0      0       0      16        0.0%
;;BANK6               50      0       0      17        0.0%
;;BITSFR16             0      0       0      17        0.0%
;;SFR16                0      0       0      17        0.0%
;;BITBANK7            50      0       0      18        0.0%
;;BITSFR17             0      0       0      18        0.0%
;;SFR17                0      0       0      18        0.0%
;;BANK7               50      0       0      19        0.0%
;;BITSFR18             0      0       0      19        0.0%
;;SFR18                0      0       0      19        0.0%
;;BITSFR19             0      0       0      20        0.0%
;;SFR19                0      0       0      20        0.0%
;;ABS                  0      0      27      20        0.0%
;;BITBANK8            50      0       0      21        0.0%
;;BITSFR20             0      0       0      21        0.0%
;;SFR20                0      0       0      21        0.0%
;;BANK8               50      0       0      22        0.0%
;;BITSFR21             0      0       0      22        0.0%
;;SFR21                0      0       0      22        0.0%
;;BITBANK9            50      0       0      23        0.0%
;;BITSFR22             0      0       0      23        0.0%
;;SFR22                0      0       0      23        0.0%
;;BANK9               50      0       0      24        0.0%
;;BITSFR23             0      0       0      24        0.0%
;;SFR23                0      0       0      24        0.0%
;;BITBANK10           50      0       0      25        0.0%
;;BITSFR24             0      0       0      25        0.0%
;;SFR24                0      0       0      25        0.0%
;;BANK10              50      0       0      26        0.0%
;;BITSFR25             0      0       0      26        0.0%
;;SFR25                0      0       0      26        0.0%
;;BITBANK11           50      0       0      27        0.0%
;;BITSFR26             0      0       0      27        0.0%
;;SFR26                0      0       0      27        0.0%
;;BANK11              50      0       0      28        0.0%
;;BITSFR27             0      0       0      28        0.0%
;;SFR27                0      0       0      28        0.0%
;;BITBANK12           30      0       0      29        0.0%
;;BITSFR28             0      0       0      29        0.0%
;;SFR28                0      0       0      29        0.0%
;;BANK12              30      0       0      30        0.0%
;;BITSFR29             0      0       0      30        0.0%
;;SFR29                0      0       0      30        0.0%
;;BITSFR30             0      0       0      31        0.0%
;;SFR30                0      0       0      31        0.0%
;;DATA                 0      0      28      31        0.0%
;;BITSFR31             0      0       0      32        0.0%
;;SFR31                0      0       0      32        0.0%

	global	_main
psect	maintext,global,class=CODE,delta=2
global __pmaintext
__pmaintext:

;; *************** function _main *****************
;; Defined at:
;;		line 141 in file "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;  delay           4    0        unsigned long 
;;  n               1    5[COMMON] unsigned char 
;;  q               1    4[COMMON] unsigned char 
;;  rowix           1    3[COMMON] unsigned char 
;;  row             1    2[COMMON] unsigned char 
;;  b               1    0        unsigned char 
;;  g               1    0        unsigned char 
;;  r               1    0        unsigned char 
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, fsr1l, fsr1h, status,2, status,0, pclath, cstack
;; Tracked objects:
;;		On entry : 17F/0
;;		On exit  : 1F/0
;;		Unchanged: FFE00/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2   BANK3   BANK4   BANK5   BANK6   BANK7   BANK8   BANK9  BANK10  BANK11  BANK12
;;      Params:         0       0       0       0       0       0       0       0       0       0       0       0       0       0
;;      Locals:         4       0       0       0       0       0       0       0       0       0       0       0       0       0
;;      Temps:          0       0       0       0       0       0       0       0       0       0       0       0       0       0
;;      Totals:         4       0       0       0       0       0       0       0       0       0       0       0       0       0
;;Total ram usage:        4 bytes
;; Hardware stack levels required when called:    2
;; This function calls:
;;		_setup
;; This function is called by:
;;		Startup code after reset
;; This function uses a non-reentrant model
;;
psect	maintext
	file	"C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
	line	141
	global	__size_of_main
	__size_of_main	equ	__end_of_main-_main
	
_main:	
	opt	stack 14
; Regs used in _main: [wreg+fsr1l-status,0+pclath+cstack]
	line	143
	
l3262:	
;KiwiEmotion.c: 142: unsigned char r, g, b, q;
;KiwiEmotion.c: 143: unsigned char row = 0, rowix;
	clrf	(main@row)
	line	146
	
l3264:	
;KiwiEmotion.c: 144: unsigned char n;
;KiwiEmotion.c: 145: unsigned long delay;
;KiwiEmotion.c: 146: setup();
	fcall	_setup
	line	148
	
l3266:	
;KiwiEmotion.c: 148: RC5 = 1; asm( "NOP" ); asm( "NOP" );;
	movlb 0	; select bank0
	bsf	(117/8),(117)&7
	
l3268:	
# 148 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
psect	maintext
	
l3270:	
# 148 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
psect	maintext
	line	149
	
l3272:	
;KiwiEmotion.c: 149: RC3 = 1; asm( "NOP" ); asm( "NOP" ); RC3 = 0; asm( "NOP" ); asm( "NOP" );;
	movlb 0	; select bank0
	bsf	(115/8),(115)&7
	
l3274:	
# 149 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
psect	maintext
	
l3276:	
# 149 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
psect	maintext
	
l3278:	
	movlb 0	; select bank0
	bcf	(115/8),(115)&7
	
l3280:	
# 149 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
psect	maintext
	
l3282:	
# 149 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
psect	maintext
	line	150
	
l3284:	
;KiwiEmotion.c: 150: PORTA = ( PORTA & 0xF8 ) | row;
	movlb 0	; select bank0
	movf	(12),w
	andlw	0F8h
	iorwf	(main@row),w
	movwf	(12)	;volatile
	line	151
	
l3286:	
;KiwiEmotion.c: 151: RC5 = 0; asm( "NOP" ); asm( "NOP" );;
	bcf	(117/8),(117)&7
	
l3288:	
# 151 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
psect	maintext
	
l3290:	
# 151 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
psect	maintext
	line	152
	
l3292:	
;KiwiEmotion.c: 152: row = ( row + 1 ) & 7;
	incf	(main@row),f
	
l3294:	
	movlw	(07h)
	andwf	(main@row),f
	line	153
	
l3296:	
;KiwiEmotion.c: 153: rowix = row * 3;
	movf	(main@row),w
	addwf	(main@row),w
	addwf	(main@row),w
	movwf	(main@rowix)
	line	154
	
l3298:	
;KiwiEmotion.c: 154: q = i2c_packet[ rowix + 0 ];
	movf	(main@rowix),w
	addlw	_i2c_packet&0ffh
	movwf	fsr1l
	clrf fsr1h	
	
	movf	indf1,w
	movwf	(main@q)
	line	155
	
l3300:	
;KiwiEmotion.c: 155: for( n = 0; n < 8; n++ ) {
	clrf	(main@n)
	
l1481:	
	line	156
;KiwiEmotion.c: 156: RC2 = 0 != ( q & 0x80 );
	btfsc	(main@q),(7)&7
	goto	u101
	goto	u100
	
u101:
	movlb 0	; select bank0
	bsf	(114/8),(114)&7
	goto	u114
u100:
	movlb 0	; select bank0
	bcf	(114/8),(114)&7
u114:
	line	157
;KiwiEmotion.c: 157: q <<= 1;
	lslf	(main@q),f
	line	158
;KiwiEmotion.c: 158: RC4 = 1; asm( "NOP" ); asm( "NOP" ); RC4 = 0;;
	bsf	(116/8),(116)&7
# 158 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
# 158 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
psect	maintext
	movlb 0	; select bank0
	bcf	(116/8),(116)&7
	line	155
	
l3306:	
	incf	(main@n),f
	
l3308:	
	movlw	(08h)
	subwf	(main@n),w
	skipc
	goto	u121
	goto	u120
u121:
	goto	l1481
u120:
	line	160
	
l3310:	
;KiwiEmotion.c: 159: }
;KiwiEmotion.c: 160: q = i2c_packet[ rowix + 1 ];
	movf	(main@rowix),w
	addlw	_i2c_packet+01h&0ffh
	movwf	fsr1l
	clrf fsr1h	
	
	movf	indf1,w
	movwf	(main@q)
	line	161
	
l3312:	
;KiwiEmotion.c: 161: for( n = 0; n < 8; n++ ) {
	clrf	(main@n)
	
l1483:	
	line	162
;KiwiEmotion.c: 162: RC2 = 0 != ( q & 0x80 );
	btfsc	(main@q),(7)&7
	goto	u131
	goto	u130
	
u131:
	movlb 0	; select bank0
	bsf	(114/8),(114)&7
	goto	u144
u130:
	movlb 0	; select bank0
	bcf	(114/8),(114)&7
u144:
	line	163
;KiwiEmotion.c: 163: q <<= 1;
	lslf	(main@q),f
	line	164
;KiwiEmotion.c: 164: RC4 = 1; asm( "NOP" ); asm( "NOP" ); RC4 = 0;;
	bsf	(116/8),(116)&7
# 164 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
# 164 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
psect	maintext
	movlb 0	; select bank0
	bcf	(116/8),(116)&7
	line	161
	
l3318:	
	incf	(main@n),f
	
l3320:	
	movlw	(08h)
	subwf	(main@n),w
	skipc
	goto	u151
	goto	u150
u151:
	goto	l1483
u150:
	line	166
	
l3322:	
;KiwiEmotion.c: 165: }
;KiwiEmotion.c: 166: q = i2c_packet[ rowix + 2 ];
	movf	(main@rowix),w
	addlw	_i2c_packet+02h&0ffh
	movwf	fsr1l
	clrf fsr1h	
	
	movf	indf1,w
	movwf	(main@q)
	line	167
	
l3324:	
;KiwiEmotion.c: 167: for( n = 0; n < 8; n++ ) {
	clrf	(main@n)
	
l1485:	
	line	168
;KiwiEmotion.c: 168: RC2 = 0 != ( q & 0x80 );
	btfsc	(main@q),(7)&7
	goto	u161
	goto	u160
	
u161:
	movlb 0	; select bank0
	bsf	(114/8),(114)&7
	goto	u174
u160:
	movlb 0	; select bank0
	bcf	(114/8),(114)&7
u174:
	line	169
;KiwiEmotion.c: 169: q <<= 1;
	lslf	(main@q),f
	line	170
;KiwiEmotion.c: 170: RC4 = 1; asm( "NOP" ); asm( "NOP" ); RC4 = 0;;
	bsf	(116/8),(116)&7
# 170 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
# 170 "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
NOP ;#
psect	maintext
	movlb 0	; select bank0
	bcf	(116/8),(116)&7
	line	167
	
l3330:	
	incf	(main@n),f
	
l3332:	
	movlw	(08h)
	subwf	(main@n),w
	skipc
	goto	u181
	goto	u180
u181:
	goto	l1485
u180:
	goto	l3266
	global	start
	ljmp	start
	opt stack 0
psect	maintext
	line	173
GLOBAL	__end_of_main
	__end_of_main:
;; =============== function _main ends ============

	signat	_main,88
	global	_setup
psect	text43,local,class=CODE,delta=2
global __ptext43
__ptext43:

;; *************** function _setup *****************
;; Defined at:
;;		line 110 in file "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;  n               1    1[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, fsr1l, fsr1h, status,2, status,0
;; Tracked objects:
;;		On entry : 17F/0
;;		On exit  : 17F/4
;;		Unchanged: FFE80/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2   BANK3   BANK4   BANK5   BANK6   BANK7   BANK8   BANK9  BANK10  BANK11  BANK12
;;      Params:         0       0       0       0       0       0       0       0       0       0       0       0       0       0
;;      Locals:         1       0       0       0       0       0       0       0       0       0       0       0       0       0
;;      Temps:          0       0       0       0       0       0       0       0       0       0       0       0       0       0
;;      Totals:         1       0       0       0       0       0       0       0       0       0       0       0       0       0
;;Total ram usage:        1 bytes
;; Hardware stack levels used:    1
;; Hardware stack levels required when called:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text43
	file	"C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
	line	110
	global	__size_of_setup
	__size_of_setup	equ	__end_of_setup-_setup
	
_setup:	
	opt	stack 14
; Regs used in _setup: [wreg+fsr1l-status,0]
	line	112
	
l3228:	
;KiwiEmotion.c: 111: unsigned char n;
;KiwiEmotion.c: 112: OSCCON = 0b01110000;
	movlw	(070h)
	movlb 1	; select bank1
	movwf	(153)^080h	;volatile
	line	113
	
l3230:	
;KiwiEmotion.c: 113: ANSELA = 0b00000000;
	movlb 3	; select bank3
	clrf	(396)^0180h	;volatile
	line	114
	
l3232:	
;KiwiEmotion.c: 114: ANSELC = 0b00000000;
	clrf	(398)^0180h	;volatile
	line	115
	
l3234:	
;KiwiEmotion.c: 115: PORTA = 0b00000000;
	movlb 0	; select bank0
	clrf	(12)	;volatile
	line	116
	
l3236:	
;KiwiEmotion.c: 116: PORTC = 0b00000000;
	clrf	(14)	;volatile
	line	117
;KiwiEmotion.c: 117: TRISA = 0b11111000;
	movlw	(0F8h)
	movlb 1	; select bank1
	movwf	(140)^080h	;volatile
	line	118
;KiwiEmotion.c: 118: TRISC = 0b11000011;
	movlw	(0C3h)
	movwf	(142)^080h	;volatile
	line	120
	
l3238:	
;KiwiEmotion.c: 120: WPUA = 0b00000000;
	movlb 4	; select bank4
	clrf	(524)^0200h	;volatile
	line	121
	
l3240:	
;KiwiEmotion.c: 121: WPUC = 0b00000011;
	movlw	(03h)
	movwf	(526)^0200h	;volatile
	line	122
	
l3242:	
;KiwiEmotion.c: 122: OPTION_REG = 0b01111111;
	movlw	(07Fh)
	movlb 1	; select bank1
	movwf	(149)^080h	;volatile
	line	123
	
l3244:	
;KiwiEmotion.c: 123: INLVLA = 0b00111111;
	movlw	(03Fh)
	movlb 7	; select bank7
	movwf	(908)^0380h	;volatile
	line	124
	
l3246:	
;KiwiEmotion.c: 124: INLVLC = 0b00111111;
	movlw	(03Fh)
	movwf	(910)^0380h	;volatile
	line	126
	
l3248:	
;KiwiEmotion.c: 126: for( n = 0; n < 32; n++ ) {
	clrf	(setup@n)
	line	127
	
l3252:	
;KiwiEmotion.c: 127: i2c_packet[ n ] = 0;
	movf	(setup@n),w
	addlw	_i2c_packet&0ffh
	movwf	fsr1l
	clrf fsr1h	
	
	clrf	indf1
	line	126
	
l3254:	
	incf	(setup@n),f
	
l3256:	
	movlw	(020h)
	subwf	(setup@n),w
	skipc
	goto	u91
	goto	u90
u91:
	goto	l3252
u90:
	line	129
	
l3258:	
;KiwiEmotion.c: 128: }
;KiwiEmotion.c: 129: i2c_packet[ 10 ] = 0b00011000;
	movlw	(018h)
	movlb 0	; select bank0
	movwf	0+(_i2c_packet)+0Ah
	line	130
;KiwiEmotion.c: 130: i2c_packet[ 13 ] = 0b00011000;
	movlw	(018h)
	movwf	0+(_i2c_packet)+0Dh
	line	132
	
l3260:	
;KiwiEmotion.c: 132: SSP1IE = 1;
	movlb 1	; select bank1
	bsf	(1163/8)^080h,(1163)&7
	line	133
;KiwiEmotion.c: 133: SSPADD = 0x33 << 1;
	movlw	(066h)
	movlb 4	; select bank4
	movwf	(530)^0200h	;volatile
	line	134
;KiwiEmotion.c: 134: SSPCON3 = 0b00110000;
	movlw	(030h)
	movwf	(535)^0200h	;volatile
	line	135
;KiwiEmotion.c: 135: SSPCON2 = 0b00000001;
	movlw	(01h)
	movwf	(534)^0200h	;volatile
	line	136
;KiwiEmotion.c: 136: SSPCON1 = 0b00111110;
	movlw	(03Eh)
	movwf	(533)^0200h	;volatile
	line	137
;KiwiEmotion.c: 137: INTCON = 0b11000000;
	movlw	(0C0h)
	movwf	(11)	;volatile
	line	139
	
l1477:	
	return
	opt stack 0
GLOBAL	__end_of_setup
	__end_of_setup:
;; =============== function _setup ends ============

	signat	_setup,88
	global	_isr
psect	intentry

;; *************** function _isr *****************
;; Defined at:
;;		line 24 in file "C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;  data            1    0[COMMON] unsigned char 
;; Return value:  Size  Location     Type
;;                  2  1458[COMMON] int 
;; Registers used:
;;		wreg, fsr1l, fsr1h, status,2, status,0
;; Tracked objects:
;;		On entry : 0/0
;;		On exit  : 1B/0
;;		Unchanged: FFFE0/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2   BANK3   BANK4   BANK5   BANK6   BANK7   BANK8   BANK9  BANK10  BANK11  BANK12
;;      Params:         0       0       0       0       0       0       0       0       0       0       0       0       0       0
;;      Locals:         1       0       0       0       0       0       0       0       0       0       0       0       0       0
;;      Temps:          0       0       0       0       0       0       0       0       0       0       0       0       0       0
;;      Totals:         1       0       0       0       0       0       0       0       0       0       0       0       0       0
;;Total ram usage:        1 bytes
;; Hardware stack levels used:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		Interrupt level 1
;; This function uses a non-reentrant model
;;
psect	intentry
	file	"C:\Dropbox\RoboCortex\arduino\KiwiRay\KiwiEmotion\KiwiEmotion.c"
	line	24
	global	__size_of_isr
	__size_of_isr	equ	__end_of_isr-_isr
	
_isr:	
	opt	stack 14
; Regs used in _isr: [wreg+fsr1l-status,0]
psect	intentry
	pagesel	$
	line	27
	
i1l3200:	
;KiwiEmotion.c: 25: uint8_t data;
;KiwiEmotion.c: 26: static uint8_t last = 0;
;KiwiEmotion.c: 27: if( SSP1IF ) {
	movlb 0	; select bank0
	btfss	(139/8),(139)&7
	goto	u1_21
	goto	u1_20
u1_21:
	goto	i1l1472
u1_20:
	line	29
	
i1l3202:	
;KiwiEmotion.c: 29: SSP1IF = 0;
	bcf	(139/8),(139)&7
	line	30
;KiwiEmotion.c: 30: if( BF ) {
	movlb 4	; select bank4
	btfss	(4256/8)^0200h,(4256)&7
	goto	u2_21
	goto	u2_20
u2_21:
	goto	i1l3224
u2_20:
	line	32
	
i1l3204:	
;KiwiEmotion.c: 32: if( !R_nW ) {
	btfsc	(4258/8)^0200h,(4258)&7
	goto	u3_21
	goto	u3_20
u3_21:
	goto	i1l1463
u3_20:
	line	34
	
i1l3206:	
;KiwiEmotion.c: 34: data = SSPBUF;
	movf	(529)^0200h,w	;volatile
	movwf	(isr@data)
	line	36
	
i1l3208:	
;KiwiEmotion.c: 36: if( D_nA ) {
	btfss	(4261/8)^0200h,(4261)&7
	goto	u4_21
	goto	u4_20
u4_21:
	goto	i1l3226
u4_20:
	line	38
	
i1l3210:	
;KiwiEmotion.c: 38: if( i2c_bytes < 24 ) {
	movlw	(018h)
	subwf	(_i2c_bytes),w
	skipnc
	goto	u5_21
	goto	u5_20
u5_21:
	goto	i1l1466
u5_20:
	line	39
	
i1l3212:	
;KiwiEmotion.c: 39: i2c_packet[ i2c_bytes ] = data;
	movf	(_i2c_bytes),w
	addlw	_i2c_packet&0ffh
	movwf	fsr1l
	clrf fsr1h	
	
	movf	(isr@data),w
	movwf	indf1
	line	40
	
i1l3214:	
;KiwiEmotion.c: 40: i2c_bytes++;
	incf	(_i2c_bytes),f
	goto	i1l3226
	line	43
	
i1l1463:	
	line	45
;KiwiEmotion.c: 45: if( !D_nA ) {
	btfsc	(4261/8)^0200h,(4261)&7
	goto	u6_21
	goto	u6_20
u6_21:
	goto	i1l3218
u6_20:
	line	47
	
i1l3216:	
	movf	(529)^0200h,w	;volatile
	line	49
	
i1l3218:	
;KiwiEmotion.c: 48: }
;KiwiEmotion.c: 49: if( ( !D_nA ) || ( !ACKSTAT ) ) {
	btfss	(4261/8)^0200h,(4261)&7
	goto	u7_21
	goto	u7_20
u7_21:
	goto	i1l3222
u7_20:
	
i1l3220:	
	btfsc	(4278/8)^0200h,(4278)&7
	goto	u8_21
	goto	u8_20
u8_21:
	goto	i1l3226
u8_20:
	line	51
	
i1l3222:	
;KiwiEmotion.c: 51: SSPBUF = i2c_bytes;
	movf	(_i2c_bytes),w
	movwf	(529)^0200h	;volatile
	goto	i1l3226
	line	53
	
i1l1466:	
	line	54
;KiwiEmotion.c: 52: }
;KiwiEmotion.c: 53: }
;KiwiEmotion.c: 54: } else {
	goto	i1l3226
	line	57
	
i1l3224:	
;KiwiEmotion.c: 57: i2c_bytes = 0;
	clrf	(_i2c_bytes)
	line	60
	
i1l3226:	
;KiwiEmotion.c: 58: }
;KiwiEmotion.c: 60: CKP = 1;
	bsf	(4268/8)^0200h,(4268)&7
	line	62
	
i1l1472:	
	retfie
	opt stack 0
GLOBAL	__end_of_isr
	__end_of_isr:
;; =============== function _isr ends ============

	signat	_isr,90
psect	intentry
	global	btemp
	btemp set 07Eh

	DABS	1,126,2	;btemp
	global	wtemp0
	wtemp0 set btemp
	end
