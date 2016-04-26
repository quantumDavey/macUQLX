/*
 * (c) UQLX - see COPYRIGHT
 */


#ifndef IE_XL

#include "QL68000.h"
#include <string.h>
#include "funcdefs.h"

#endif  /* IE_XL */

#ifdef IE_XL
#define LR  &&
#else
#define LR
#endif

#ifndef IE_XL
void SetTable(void (**otbl)(void), syntab *stbl, char *mask,void (*f)(void), char *mnem, int smask, int dmask)
{
	long c;
	long m,m0;
   	Cond multiplo=false;
   	register void (**p)(void);
	register syntab *q;
	
   	short i,c1,c2;
	
	if(V4) printf("SetTable: %s, insns %s, *code %x\n",OpCodePtr(mnem),mask,f);
	
   	m=0x08000; c=0;
	
   	for(i=0;i<16;i++)
   	{
		if(mask[i]=='1') c+=m;
   		if(mask[i]=='x') multiplo=true;
   		m>>=1;
   	}
	
   	if(!multiplo)
	{
		otbl[c]=f;
		stbl[c].m=OpCodePtr(mnem);
		stbl[c].s=smask; stbl[c].d=dmask; stbl[c].x=0;
		return;
	}
	
   	m0=1; i=15; c1=1;
	
   	while(mask[i]!='x') { m0<<=1; i--; }
	
   	while(mask[i--]=='x') c1<<=1;
	
   	while(i&&mask[i]!='x') i--;
	
   	if(i)
   	{
		c2=2;
		m=0x08000>>i;
		while(mask[--i]=='x') c2<<=1;
		
	   	while(c2--)
		{
			p=otbl+c;
			q=stbl+c;
			i=c1;
			
			while(i--)
			{
				*p=f;
				q->m=OpCodePtr(mnem);
				q->s=smask; q->d=dmask; q->x=0;
				p+=m0;
				q+=m0;
			}
			
			c+=m;
		}
   	}
   	else
   	{
		p=otbl+c;
		q=stbl+c;
		
		while(c1--)
		{
			*p=f;
			q->m=OpCodePtr(mnem);
			q->s=smask; q->d=dmask; q->x=0;
			p+=m0;
			q+=m0;
		}
	}
}

#endif

#ifndef IE_XL
void SetInvalEntries(void (**instbl)(void), syntab syntbl[], void *code, char *mnemonic, int arg1mask, int arg2mask)
{
	long i;
	
	for(i=8;i<61440;i++)
	{
		instbl[i]= code;
		syntbl[i].m=(char *)OpCodePtr(mnemonic);
		syntbl[i].s=arg1mask;syntbl[i].d=arg2mask;syntbl[i].x=0;
		
	}
}
#endif


#ifndef IE_XL
static void SetTabEntries(void (**instbl)(void), syntab syntbl[])
#endif

#if defined(IE_XL_II) || (!defined(IE_XL))
{   
	// New SetTable Syntax:
	//
	// SetTable( theTable, binarymask, function to call, mnemonic, key for source, key for dest )
	//
	// All bit field specifiers:
	//		R:nm 	R=addressing mode
	//				:nm = field of bits  n to n+m of instruction word  incl counting from right
	//
	//	operand mask syntax encoding:
	//
	//	ttttmmmmmmmmmmmm	Bitfield definition		t=operand type, m=bitmask for value
	//	1111000000000iii	Literal string,			iii=index into literal string array:
	//
	//	fieldmask			code	mnem	mode	description
	//
	//	0000mmmmmmmmmmmm	0xxx	EAMR	ea		EA [MMMRRR] defined by bit 0 to bit 5 + opt extension words
	//												0x003F "E05"
	//
	//	0001mmmmmmmmmmmm	1xxx	EARM	ea		EA [RRRMMM] defined by bit 6 to bit 11 + opt extension words
	//												0x1FC0 "F65"
	//
	//	0010mmmmmmmmmmmm	2xxx	D:nm	direct  Data reg defined by bit n to bit n+m incl:
	//												0x2007 "D02"
	//												0x2E00 "D92"
	//
	//	0011mmmmmmmmmmmm	3xxx	A:nm	direct  Addr reg defined by bit n to bit n+m incl
	//												0x3007 "A02"
	//												0x3E00 "A92"
	//
	//	0100mmmmmmmmmmmm	4xxx	a:nm	(An)	address reg indirect
	//												0x4007 "a02"
	//												0x4E00 "a92"
	//
	//	0101mmmmmmmmmmmm	5xxx	d:nm	d(An)	disp extword [0=D,1=A][idx][0=W,1=L][sc][0][signddisp]
	//												0x5007 "d02"
	//												0x5E00 "d92"
	//
	//	0110mmmmmmmmmmmm	6xxx	+:nm	(An)+	address reg indirect with post-increment
	//												0x6007 "+02"
	//												0x6E00 "+92"
	//
	//	0111mmmmmmmmmmmm	7xxx	-:nm	-(An)	address reg indirect with pre-decrement
	//												0x7007 "-02"
	//												0x7E00 "-92"
	//
	//	1000mmmmmmmmmmmm	8xxx	P		d(PC)	16 bit signed displacement from PC
	//
	//	1001mmmmmmmmmmmm	9xxx	G		disp	8 bit disp (=0 +16 bit extword, =255 +32 bit extword)
	//
	//	1010mmmmmmmmmmmm	Axxx	I:nm	immed	immediate data in instruction
	//
	//	1110000100000000	E100	Extword immed	Byte (Next word, LSB significant only)
	//												0xE100 "X"
	//
	//	1110001000000000	E200	Extword immed	Word (Next word, whole word siginificant)
	//												0xE200 "XX"
	//
	//	1110001100000000	E300	Extword immed	Long (Next 2 words, whole long significant)
	//												0xE300 "XXXX"
	//
	//	1110100000000000	E800	MA-D	immed	Word Register Spec Mask [A7...D0]
	//
	//	1110100100000000	E900	MD-A	immed	Word Register Spec Mask [D0...A7]
	//
	//	1111000000000iii	F00x	Literal	string	iii=index into literal string array:
	//													0xF000 "" ""
	//													0xF001 "#1"
	//													0xF002 "#2"
	//													0xF003 "#4"
	//													0xF004 "CCR"
	//													0xF005 "SR"
	//													0xF006 "USP"
	
	SetInvalEntries(instbl, syntbl, LR InvalidCode, "Invalid", 0xF000 /*" "*/, 0xF000 /*" "*/ );
	
	SetTable(instbl, syntbl, "0000000000xxxxxx", LR ori_b,			"ori.b",	0xE100 /*X*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000000000111100", LR ori_to_ccr,		"ori",		0xE100 /*X*/,		0xF004 /*CCR*/ );
	SetTable(instbl, syntbl, "0000000001xxxxxx", LR ori_w,			"ori.w",	0xE200 /*XX*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000000001111100", LR ori_to_sr,		"ori",		0xE200 /*XX*/,		0xF005 /*SR*/ );
	SetTable(instbl, syntbl, "0000000010xxxxxx", LR ori_l,			"ori.l",	0xE300 /*XXXX*/,	0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000xxx100xxxxxx", LR btst_d,			"btst",		0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000xxx101xxxxxx", LR bchg_d,			"bchg",		0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000xxx110xxxxxx", LR bclr_d,			"bclr",		0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000xxx111xxxxxx", LR bset_d,			"bset",		0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000xxx100001xxx", LR movep_w_mr,		"movep.w",	0x5007 /*d02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "0000xxx101001xxx", LR movep_l_mr,		"movep.l",	0x5007 /*d02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "0000xxx110001xxx", LR movep_w_rm,		"movep.w",	0x2E00 /*D92*/,		0x5007 /*d02*/ );
	SetTable(instbl, syntbl, "0000xxx111001xxx", LR movep_l_rm,		"movep.l",	0x2E00 /*D92*/,		0x5007 /*d02*/ );
	SetTable(instbl, syntbl, "0000001000xxxxxx", LR andi_b,			"andi.b",	0xE100 /*X*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000001001xxxxxx", LR andi_w,			"andi.w",	0xE200 /*XX*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000001010xxxxxx", LR andi_l,			"andi.l",	0xE300 /*XXXX*/,	0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000001000111100", LR andi_to_ccr,	"andi",		0xE100 /*X*/,		0xF004 /*CCR*/ );
	SetTable(instbl, syntbl, "0000001001111100", LR andi_to_sr,		"andi",		0xE200 /*XX*/,		0xF005 /*SR*/ );
	SetTable(instbl, syntbl, "0000010000xxxxxx", LR subi_b,			"subi.b",	0xE100 /*X*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000010001xxxxxx", LR subi_w,			"subi.w",	0xE200 /*XX*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000010010xxxxxx", LR subi_l,			"subi.l",	0xE300 /*XXXX*/,	0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000011000xxxxxx", LR addi_b,			"addi.b",	0xE100 /*X*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000011001xxxxxx", LR addi_w,			"addi.w",	0xE200 /*XX*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000011010xxxxxx", LR addi_l,			"addi.l",	0xE300 /*XXXX*/,	0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000100000xxxxxx", LR btst_s,			"btst.s",	0xE100 /*X*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000100001xxxxxx", LR bchg_s,			"bchg.s",	0xE100 /*X*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000100010xxxxxx", LR bclr_s,			"bclr.s",	0xE100 /*X*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000100011xxxxxx", LR bset_s,			"bset.s",	0xE100 /*X*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000101000xxxxxx", LR eori_b,			"eori.b",	0xE100 /*X*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000101001xxxxxx", LR eori_w,			"eori.w",	0xE200 /*XX*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000101010xxxxxx", LR eori_l,			"eori.l",	0xE300 /*XXXX*/,	0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000101000111100", LR eori_to_ccr,	"eori",		0xE200 /*XX*/,		0xF004 /*CCR*/ );
	SetTable(instbl, syntbl, "0000101001111100", LR eori_to_sr,		"eori",		0xE200 /*XX*/,		0xF005 /*SR*/ );
	SetTable(instbl, syntbl, "0000110000xxxxxx", LR cmpi_b,			"cmpi.b",	0xE100 /*X*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000110001xxxxxx", LR cmpi_w,			"cmpi.w",	0xE200 /*XX*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0000110010xxxxxx", LR cmpi_l,			"cmpi.l",	0xE300 /*XXXX*/,	0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0001xxxxxxxxxxxx", LR move_b,			"move.b",	0x003F /*E05*/,		0x1FC0 /*F65*/ );
	SetTable(instbl, syntbl, "0001xxxxxx000xxx", LR move_b_from_dn,	"move.b",	0x2007 /*D02*/,		0x1FC0 /*F65*/ );
	SetTable(instbl, syntbl, "0001xxx000xxxxxx", LR move_b_to_dn,	"move.b",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "0001xxx000000xxx", LR move_b_reg,		"move.b",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "0010xxxxxxxxxxxx", LR move_l,			"move.l",	0x003F /*E05*/,		0x1FC0 /*F65*/ );
	SetTable(instbl, syntbl, "0010xxxxxx000xxx", LR move_l_from_dn,	"move.l",	0x2007 /*D02*/,		0x1FC0 /*F65*/ );
	SetTable(instbl, syntbl, "0010xxx000xxxxxx", LR move_l_to_dn,	"move.l",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "0010xxx000000xxx", LR move_l_reg,		"move.l",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "0010xxx001xxxxxx", LR movea_l,		"movea.l",	0x3E00 /*A92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0010xxx001001xxx", LR movea_l_an,		"movea.l",	0x3007 /*A02*/,		0x3E00 /*A92*/ );
	SetTable(instbl, syntbl, "0011xxxxxxxxxxxx", LR move_w,			"move.w",	0x003F /*E05*/,		0x1FC0 /*F65*/ );
	SetTable(instbl, syntbl, "0011xxxxxx000xxx", LR move_w_from_dn,	"move.w",	0x2007 /*D02*/,		0x1FC0 /*F65*/ );
	SetTable(instbl, syntbl, "0011xxx000xxxxxx", LR move_w_to_dn,	"move.w",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "0011xxx000000xxx", LR move_w_reg,		"move.w",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "0011xxx001xxxxxx", LR movea_w,		"movea.w",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "0100000000xxxxxx", LR negx_b,			"negx.b",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100000001xxxxxx", LR negx_w,			"negx.w",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100000010xxxxxx", LR negx_l,			"negx.l",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100000011xxxxxx", LR move_from_sr,	"move",		0xF005 /*SR*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0100xxx110xxxxxx", LR chk,			"chk.w",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "0100xxx111xxxxxx", LR lea,			"lea",		0x003F /*E05*/,		0x2E00 /*D92*/ );
	/**/
	SetTable(instbl, syntbl, "0100001000xxxxxx", LR clr_b,			"clr.b",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100001001xxxxxx", LR clr_w,			"clr.w",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100001010xxxxxx", LR clr_l,			"clr.l",	0x003F /*E05*/,		0xF000 /*" "*/ );
	// SetTable(instbl, syntbl, "0100001011xxxxxx", LR move_from_ccr,	"move",		0xF004 /*CCR*/,		0x003F /*E05*/);
	SetTable(instbl, syntbl, "0100010000xxxxxx", LR neg_b,			"neg.b",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100010001xxxxxx", LR neg_w,			"neg.w",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100010010xxxxxx", LR neg_l,			"neg.l",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100010011xxxxxx", LR move_to_ccr,	"move",		0x003F /*E05*/,		0xF004 /*CCR*/ );
	SetTable(instbl, syntbl, "0100011000xxxxxx", LR not_b,			"not.b",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100011001xxxxxx", LR not_w,			"not.w",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100011010xxxxxx", LR not_l,			"not.l",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100011011xxxxxx", LR move_to_sr,		"move",		0x003F /*E05*/,		0xF005 /*SR*/ );
	SetTable(instbl, syntbl, "0100100000xxxxxx", LR nbcd,			"nbcd",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100100001xxxxxx", LR pea,			"pea",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100100001000xxx", LR swap,			"swap",		0x2007 /*D02*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100100010xxxxxx", LR movem_save_w,	"movem.w",	0xF900 /*MD-A*/,	0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0100100010011xxx", LR movem_save_w,	"movem.w",	0xE800 /*MA-D*/,	0x6007 /*+02*/ );
	SetTable(instbl, syntbl, "0100100010100xxx", LR movem_save_w,	"movem.w",	0xE800 /*MA-D*/,	0x7007 /*-02*/ );
	SetTable(instbl, syntbl, "0100100010000xxx", LR ext_w,			"ext.w",	0x2007 /*D02*/,		0xF000 /*" "*/  );
	SetTable(instbl, syntbl, "0100100011xxxxxx", LR movem_save_l,	"movem.l",	0xF900 /*MD-A*/,	0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0100100011011xxx", LR movem_save_l,	"movem.l",	0xE800 /*MA-D*/,	0x6007 /*+02*/ );
	SetTable(instbl, syntbl, "0100100011100xxx", LR movem_save_l,	"movem.l",	0xE800 /*MA-D*/,	0x7007 /*-02*/ );
	SetTable(instbl, syntbl, "0100100011000xxx", LR ext_l,			"ext.l",	0x2007 /*D02*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100101000xxxxxx", LR tst_b,			"tst.b",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100101001xxxxxx", LR tst_w,			"tst.w",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100101010xxxxxx", LR tst_l,			"tst.l",	0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100101011xxxxxx", LR tas,			"tas",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100101011111xxx", LR InvalidCode,	"Invalid",	0xF000 /*" "*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "010010101111100x", LR tas,			"tas",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100101011111100", LR illegal,		"illegal",	0xF000 /*" "*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100110010xxxxxx", LR movem_load_w,	"movem.w",	0x003F /*E05*/,		0xF900 /*MD-A*/ );
	SetTable(instbl, syntbl, "0100110010011xxx", LR movem_load_w,	"movem.w",	0x6007 /*+02*/,		0xE800 /*MA-D*/ );
	SetTable(instbl, syntbl, "0100110010100xxx", LR movem_load_w,	"movem.w",	0x7007 /*-02*/,		0xE800 /*MA-D*/ );
	SetTable(instbl, syntbl, "0100110011xxxxxx", LR movem_load_l,	"movem.l",	0x003F /*E05*/,		0xF900 /*MD-A*/ );
	SetTable(instbl, syntbl, "0100110011011xxx", LR movem_load_l,	"movem.l",	0x6007 /*+02*/,		0xE800 /*MA-D*/ );
	SetTable(instbl, syntbl, "0100110011100xxx", LR movem_load_l,	"movem.l",	0x7007 /*-02*/,		0xE800 /*MA-D*/ );
	SetTable(instbl, syntbl, "010011100100xxxx", LR trap,			"trap",		0xA00F /*I03*/,		0xF000 /*" "*/ );
#ifndef IE_XL_II
	SetTable(instbl, syntbl, "0100111001000000", LR trap0,			"trap#0",	0xF000 /*" "*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100111001000001", LR trap1,			"trap#1",	0xF000 /*" "*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100111001000010", LR trap2,			"trap#2",	0xF000 /*" "*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100111001000011", LR trap3,			"trap#3",	0xF000 /*" "*/,		0xF000 /*" "*/ );

	//SetTable(instbl, syntbl, "0100111001000100", LR trap4,			"trap#4",	0xF000 /*" "*/,		0xF000 /*" "*/ );
#endif
	SetTable(instbl, syntbl, "0100111001010xxx", LR link,			"link",		0x3007 /*A02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0100111001011xxx", LR unlk,			"unlk",		0x3007 /*A02*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100111001100xxx", LR move_to_usp,	"move",		0x3007 /*A02*/,		0xF006 /*USP*/ );
	SetTable(instbl, syntbl, "0100111001101xxx", LR move_from_usp,	"move",		0xF006 /*USP*/,		0x3007 /*A02*/ );
	SetTable(instbl, syntbl, "0100111001110000", LR reset,			"reset",	0xF000 /*" "*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100111001110001", LR nop,			"nop",		0xF000 /*" "*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100111001110010", LR stop,			"stop",		0xF000 /*" "*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100111001110011", LR rte,			"rte",		0xF000 /*" "*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100111001110101", LR rts,			"rts",		0xF000 /*" "*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100111001110110", LR trapv,			"trapv",	0xF000 /*" "*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100111001110111", LR rtr,			"rtr",		0xF000 /*" "*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100111010xxxxxx", LR jsr,			"jsr",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100111010111010", LR jsr_displ,		"jsr",		0x8000 /*d(PC)*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0100111011xxxxxx", LR jmp,			"jmp",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101xxx000xxxxxx", LR addq_b,			"addq.b",	0xAE00 /*I92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0101xxx001xxxxxx", LR addq_w,			"addq.w",	0xAE00 /*I92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0101xxx010xxxxxx", LR addq_l,			"addq.l",	0xAE00 /*I92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0101xxx001001xxx", LR addq_an,		"addq.w",	0xAE00 /*I92*/,		0x3007 /*A02*/ );
	SetTable(instbl, syntbl, "0101xxx010001xxx", LR addq_an,		"addq.l",	0xAE00 /*I92*/,		0x3007 /*A02*/ );
	SetTable(instbl, syntbl, "0101100001001xxx", LR addq_4_an,		"addq.w",	0xF003 /*#4*/,		0x3007 /*A02*/ );
	SetTable(instbl, syntbl, "0101100010001xxx", LR addq_4_an,		"addq.l",	0xF003 /*#4*/,		0x3007 /*A02*/ );
	SetTable(instbl, syntbl, "0101xxxx11xxxxxx", LR scc,			"sCC",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101000011xxxxxx", LR st,				"st",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101000111xxxxxx", LR sf,				"sf",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101001011xxxxxx", LR scc,			"shi",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101001111xxxxxx", LR scc,			"sls",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101010011xxxxxx", LR scc,			"scc",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101010111xxxxxx", LR scc,			"scs",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101011011xxxxxx", LR scc,			"sne",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101011111xxxxxx", LR scc,			"seq",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101100011xxxxxx", LR scc,			"svc",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101100111xxxxxx", LR scc,			"svs",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101101011xxxxxx", LR scc,			"spl",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101101111xxxxxx", LR scc,			"smi",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101110011xxxxxx", LR scc,			"sge",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101110111xxxxxx", LR scc,			"slt",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101111011xxxxxx", LR scc,			"sgt",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101111111xxxxxx", LR scc,			"sle",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0101xxxx11001xxx", LR dbcc,			"dbcc",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101000011001xxx", LR dbcc,			"dbt",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101000111001xxx", LR dbf,			"dbf",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101001011001xxx", LR dbcc,			"dbhi",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101001111001xxx", LR dbcc,			"dbls",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101010011001xxx", LR dbcc,			"dbcc",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101010111001xxx", LR dbcc,			"dbcs",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101011011001xxx", LR dbcc,			"dbne",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101011111001xxx", LR dbcc,			"dbeq",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101100011001xxx", LR dbcc,			"dbvc",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101100111001xxx", LR dbcc,			"dbvs",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101101011001xxx", LR dbcc,			"dbpl",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101101111001xxx", LR dbcc,			"dbmi",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101110011001xxx", LR dbcc,			"dbge",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101110111001xxx", LR dbcc,			"dblt",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101111011001xxx", LR dbcc,			"dbgt",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101111111001xxx", LR dbcc,			"dble",		0x2007 /*D02*/,		0xE200 /*XX*/ );
	SetTable(instbl, syntbl, "0101xxx100xxxxxx", LR subq_b,			"subq.b",	0xAE00 /*I92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0101xxx101xxxxxx", LR subq_w,			"subq.w",	0xAE00 /*I92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0101xxx110xxxxxx", LR subq_l,			"subq.l",	0xAE00 /*I92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "0101xxx101001xxx", LR subq_an,		"subq.w",	0xAE00 /*I92*/,		0x3007 /*A02*/ );
	SetTable(instbl, syntbl, "0101xxx110001xxx", LR subq_an,		"subq.l",	0xAE00 /*I92*/,		0x3007 /*A02*/ );
	SetTable(instbl, syntbl, "0101100101001xxx", LR subq_4_an,		"subq.w",	0xF003 /*#4*/,		0x3007 /*A02*/ );
	SetTable(instbl, syntbl, "0101100110001xxx", LR subq_4_an,		"subq.w",	0xF003 /*#4*/,		0x3007 /*A02*/ );
	SetTable(instbl, syntbl, "0110xxxxxxxxxxx0", LR bcc_s,			"bcc.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110xxxxxxxxxxx1", LR bcc_bad,		"bcc_BAD",	0xF000 /*" "*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01100010xxxxxxx0", LR bcc_s,			"bhi.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01100011xxxxxxx0", LR bcc_s,			"bls.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01100100xxxxxxx0", LR bccc_s,			"bccc.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01100101xxxxxxx0", LR bcs_s,			"bcs.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01100110xxxxxxx0", LR bne_s,			"bne.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01100111xxxxxxx0", LR beq_s,			"beq.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01101000xxxxxxx0", LR bcc_s,			"bvc.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01101001xxxxxxx0", LR bcc_s,			"bvs.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01101010xxxxxxx0", LR bpl_s,			"bpl.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01101011xxxxxxx0", LR bmi_s,			"bmi.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01101100xxxxxxx0", LR bge_s,			"bge.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01101101xxxxxxx0", LR blt_s,			"blt.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01101110xxxxxxx0", LR bgt_s,			"bgt.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01101111xxxxxxx0", LR ble_s,			"ble.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110xxxx00000000", LR bcc_l,			"bcc.l",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110001000000000", LR bcc_l,			"bhi.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110001100000000", LR bcc_l,			"bls.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110010000000000", LR bcc_l,			"bccc.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110010100000000", LR bcc_l,			"bcs.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110011000000000", LR bne_l,			"bne.l",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110011100000000", LR beq_l,			"beq.l",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110100000000000", LR bcc_l,			"bvc.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110100100000000", LR bcc_l,			"bvs.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110101000000000", LR bcc_l,			"bpl.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110101100000000", LR bcc_l,			"bmi.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110110000000000", LR bcc_l,			"bge.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110110100000000", LR bcc_l,			"blt.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110111000000000", LR bcc_l,			"bgt.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110111100000000", LR bcc_l,			"ble.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01100000xxxxxxx0", LR bra_s,			"bra.s",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0110000000000000", LR bra_l,			"bra.l",	0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "01100001xxxxxxxx", LR bsr,			"bsr",		0x90FF /*disp*/,	0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "0111xxx0xxxxxxxx", LR moveq,			"moveq",	0xA0FF /*I07*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1000xxx000xxxxxx", LR or_b_dn,		"or.b",		0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1000xxx001xxxxxx", LR or_w_dn,		"or.w",		0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1000xxx010xxxxxx", LR or_l_dn,		"or.l",		0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1000xxx100xxxxxx", LR or_b_ea,		"or.b",		0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1000xxx101xxxxxx", LR or_w_ea,		"or.w",		0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1000xxx110xxxxxx", LR or_l_ea,		"or.l",		0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1000xxx011xxxxxx", LR divu,			"divu.w",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1000xxx100000xxx", LR sbcd,			"sbcd",		0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1000xxx100001xxx", LR sbcd,			"sbcd",		0x7007 /*-02*/,		0x7E00 /*-92*/ );
	SetTable(instbl, syntbl, "1000xxx111xxxxxx", LR divs,			"divs.w",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1001xxx000xxxxxx", LR sub_b_dn,		"sub.b",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1001xxx001xxxxxx", LR sub_w_dn,		"sub.w",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1001xxx010xxxxxx", LR sub_l_dn,		"sub.l",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1001xxx100xxxxxx", LR sub_b_ea,		"sub.b",	0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1001xxx101xxxxxx", LR sub_w_ea,		"sub.w",	0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1001xxx110xxxxxx", LR sub_l_ea,		"sub.l",	0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1001xxx011xxxxxx", LR sub_w_an,		"sub.w",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1001xxx111xxxxxx", LR sub_l_an,		"sub.l",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1001xxx100000xxx", LR subx_b_r,		"subx.b",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1001xxx101000xxx", LR subx_w_r,		"subx.w",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1001xxx110000xxx", LR subx_l_r,		"subx.l",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1001xxx100001xxx", LR subx_b_m,		"subx.b",	0x7007 /*-02*/,		0x7E00 /*-92*/ );
	SetTable(instbl, syntbl, "1001xxx101001xxx", LR subx_w_m,		"subx.w",	0x7007 /*-02*/,		0x7E00 /*-92*/ );
	SetTable(instbl, syntbl, "1001xxx110001xxx", LR subx_l_m,		"subx.l",	0x7007 /*-02*/,		0x7E00 /*-92*/ );
	SetTable(instbl, syntbl, "1011xxx000xxxxxx", LR cmp_b,			"cmp.b",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1011xxx001xxxxxx", LR cmp_w,			"cmp.w", 	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1011xxx010xxxxxx", LR cmp_l,			"cmp.l", 	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1011xxx000000xxx", LR cmp_b_dn,		"cmp.b",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1011xxx000101xxx", LR cmp_b_dan,		"cmp.b",	0x5007 /*d02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1011xxx001000xxx", LR cmp_w_dn,		"cmp.w",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1011xxx010000xxx", LR cmp_l_dn,		"cmp.l",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1011xxx011xxxxxx", LR cmpa_w,			"cmpa.w",	0x003F /*E05*/,		0x3E00 /*A92*/ );
	SetTable(instbl, syntbl, "1011xxx111xxxxxx", LR cmpa_l,			"cmpa.l",	0x003F /*E05*/,		0x3E00 /*A92*/ );
	SetTable(instbl, syntbl, "1011xxx111001xxx", LR cmpa_l_an,		"cmpa.l",	0x3007 /*A02*/,		0x3E00 /*A92*/ );
	SetTable(instbl, syntbl, "1011xxx100xxxxxx", LR eor_b,			"eor.b",	0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1011xxx101xxxxxx", LR eor_w,			"eor.w",	0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1011xxx110xxxxxx", LR eor_l,			"eor.l",	0x2E00 /*D92*/,		0x003F /*E05*/ );
	/**/
	SetTable(instbl, syntbl, "1011xxx100001xxx", LR cmpm_b,			"cmpm.b",	0x6007 /*+02*/,		0x6E00 /*+92*/ );
	SetTable(instbl, syntbl, "1011xxx101001xxx", LR cmpm_w,			"cmpm.w",	0x6007 /*+02*/,		0x6E00 /*+92*/ );
	SetTable(instbl, syntbl, "1011xxx110001xxx", LR cmpm_l,			"cmpm.l",	0x6007 /*+02*/,		0x6E00 /*+92*/ );
	SetTable(instbl, syntbl, "1100xxx000xxxxxx", LR and_b_dn,		"and.b",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1100xxx001xxxxxx", LR and_w_dn,		"and.w",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1100xxx010xxxxxx", LR and_l_dn,		"and.l",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1100xxx010000xxx", LR and_l_dn_dn,	"and.l",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1100xxx100xxxxxx", LR and_b_ea,		"and.b",	0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1100xxx101xxxxxx", LR and_w_ea,		"and.w",	0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1100xxx110xxxxxx", LR and_l_ea,		"and.l",	0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1100xxx011xxxxxx", LR mulu,			"mulu.w",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1100xxx100000xxx", LR abcd,			"abcd",		0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1100xxx10000xxxx", LR abcd,			"abcd",		0x7007 /*-02*/,		0x7E00 /*-92*/ );
	SetTable(instbl, syntbl, "1100xxx101000xxx", LR exg_d,			"exg_d",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1100xxx101001xxx", LR exg_a,			"exg_a",	0x3007 /*A02*/,		0x3E00 /*A92*/ );
	SetTable(instbl, syntbl, "1100xxx110001xxx", LR exg_ad,			"exg_ad",	0x3007 /*A02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1100xxx111xxxxxx", LR muls,			"muls.w",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1101xxx000xxxxxx", LR add_b_dn,		"add.b",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1101xxx001xxxxxx", LR add_w_dn,		"add.w",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1101xxx010xxxxxx", LR add_l_dn,		"add.l",	0x003F /*E05*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1101xxx000000xxx", LR add_b_dn_dn,	"add.b",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1101xxx001000xxx", LR add_w_dn_dn,	"add.w",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1101xxx010000xxx", LR add_l_dn_dn,	"add.l",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1101xxx100xxxxxx", LR add_b_ea,		"add.b",	0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1101xxx101xxxxxx", LR add_w_ea,		"add.w",	0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1101xxx110xxxxxx", LR add_l_ea,		"add.l",	0x2E00 /*D92*/,		0x003F /*E05*/ );
	SetTable(instbl, syntbl, "1101xxx011xxxxxx", LR add_w_an,		"adda.w",	0x003F /*E05*/,		0x3E00 /*A92*/ );
	SetTable(instbl, syntbl, "1101xxx111xxxxxx", LR add_l_an,		"adda.l",	0x003F /*E05*/,		0x3E00 /*A92*/ );
	SetTable(instbl, syntbl, "1101xxx011000xxx", LR add_w_an_dn,	"adda.w",	0x2007 /*D02*/,		0x3E00 /*A92*/ );
	SetTable(instbl, syntbl, "1101xxx111000xxx", LR add_l_an_dn,	"adda.l",	0x2007 /*D02*/,		0x3E00 /*A92*/ );
	SetTable(instbl, syntbl, "1101xxx100000xxx", LR addx_b_r,		"addx.b",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1101xxx101000xxx", LR addx_w_r,		"addx.w",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1101xxx110000xxx", LR addx_l_r,		"addx.l",	0x2007 /*D02*/,		0x2E00 /*D92*/ );
	SetTable(instbl, syntbl, "1101xxx100001xxx", LR addx_b_m,		"addx.b",	0x7007 /*-02*/,		0x7E00 /*-92*/ );
	SetTable(instbl, syntbl, "1101xxx101001xxx", LR addx_w_m,		"addx.w",	0x7007 /*-02*/,		0x7E00 /*-92*/ );
	SetTable(instbl, syntbl, "1101xxx110001xxx", LR addx_l_m,		"addx.l",	0x7007 /*-02*/,		0x7E00 /*-92*/ );
	
	SetTable(instbl, syntbl, "1110xxx000001xxx", LR lsr_b_i,		"lsr.b",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx100001xxx", LR lsl_b_i,		"lsl.b",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110001000001xxx", LR lsr1_b,			"lsr.b",	0xF001 /*#1*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110001100001xxx", LR lsl1_b,			"lsl.b",	0xF001 /*#1*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx001001xxx", LR lsr_w_i,		"lsr.w",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx101001xxx", LR lsl_w_i,		"lsl.w",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110001001001xxx", LR lsr1_w,			"lsr.w",	0xF001 /*#1*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110001101001xxx", LR lsl1_w,			"lsl.w",	0xF001 /*#1*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx010001xxx", LR lsr_l_i,		"lsr.l",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx110001xxx", LR lsl_l_i,		"lsl.l",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110001010001xxx", LR lsr1_l,			"lsr.l",	0xF001 /*#1*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110001110001xxx", LR lsl1_l,			"lsl.l",	0xF001 /*#1*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110010110001xxx", LR lsl2_l,			"lsl.l",	0xF002 /*#2*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx000101xxx", LR lsr_b_r,		"lsr.b",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx100101xxx", LR lsl_b_r,		"lsl.b",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx001101xxx", LR lsr_w_r,		"lsr.w",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx101101xxx", LR lsl_w_r,		"lsl.w",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx010101xxx", LR lsr_l_r,		"lsr.l",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx110101xxx", LR lsl_l_r,		"lsl.l",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx000000xxx", LR asr_b_i,		"asr.b",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx100000xxx", LR asl_b_i,		"asl.b",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx001000xxx", LR asr_w_i,		"asr.w",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx101000xxx", LR asl_w_i,		"asl.w",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx010000xxx", LR asr_l_i,		"asr.l",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx110000xxx", LR asl_l_i,		"asl.l",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx000100xxx", LR asr_b_r,		"asr.b",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx100100xxx", LR asl_b_r,		"asl.b",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx001100xxx", LR asr_w_r,		"asr.w",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx101100xxx", LR asl_w_r,		"asl.w",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx010100xxx", LR asr_l_r,		"asr.l",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx110100xxx", LR asl_l_r,		"asl.l",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx000010xxx", LR roxr_b_i,		"roxr.b",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx100010xxx", LR roxl_b_i,		"roxl.b",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx001010xxx", LR roxr_w_i,		"roxr.w",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx101010xxx", LR roxl_w_i,		"roxl.w",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx010010xxx", LR roxr_l_i,		"roxr.l",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx110010xxx", LR roxl_l_i,		"roxl.l",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx000110xxx", LR roxr_b_r,		"roxr.b",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx100110xxx", LR roxl_b_r,		"roxl.b",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx001110xxx", LR roxr_w_r,		"roxr.w",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx101110xxx", LR roxl_w_r,		"roxl.w",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx010110xxx", LR roxr_l_r,		"roxr.l",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx110110xxx", LR roxl_l_r,		"roxl.l",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx000011xxx", LR ror_b_i,		"ror.b",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx100011xxx", LR rol_b_i,		"rol.b",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx001011xxx", LR ror_w_i,		"ror.w",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx101011xxx", LR rol_w_i,		"rol.w",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx010011xxx", LR ror_l_i,		"ror.l",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx110011xxx", LR rol_l_i,		"rol.l",	0xAE00 /*I92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx000111xxx", LR ror_b_r,		"ror.b",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx100111xxx", LR rol_b_r,		"rol.b",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx001111xxx", LR ror_w_r,		"ror.w",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx101111xxx", LR rol_w_r,		"rol.w",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx010111xxx", LR ror_l_r,		"ror.l",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110xxx110111xxx", LR rol_l_r,		"rol.l",	0x2E00 /*D92*/,		0x2007 /*D02*/ );
	SetTable(instbl, syntbl, "1110000011xxxxxx", LR asr_m,			"asr",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "1110000111xxxxxx", LR asl_m,			"asl",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "1110001011xxxxxx", LR lsr_m,			"lsr",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "1110001111xxxxxx", LR lsl_m,			"lsl",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "1110010011xxxxxx", LR roxr_m,			"roxr",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "1110010111xxxxxx", LR roxl_m,			"roxl",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "1110011011xxxxxx", LR ror_m,			"ror",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "1110011111xxxxxx", LR rol_m,			"rol",		0x003F /*E05*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "1010xxxxxxxxxxxx", LR code1010,		"a_line",	0xAFFF /*I0b*/,		0xF000 /*" "*/ );
	SetTable(instbl, syntbl, "1111xxxxxxxxxxxx", LR code1111,		"f_line",	0xAFFF /*I0b*/,		0xF000 /*" "*/ );
	
}
#endif

#ifndef IE_XL
void EmulatorTable(Ptr ibuffer, syntab *synbuffer)
{  
	table=ibuffer;
	syntable=synbuffer;
	SetTabEntries(ibuffer, (syntab *)synbuffer);
}
#endif