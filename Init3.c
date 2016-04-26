// Generated from Init.c
// Generated: 22/05/2008 04:32:31

/*
 * (c) UQLX - see COPYRIGHT
 */


#ifndef IE_XL
#include "QL68000.h"
#include <strings.h>
#include "funcdefs.h"
char *OpCodePtr(char *);
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
	
	if(V4) printf("SetTable: %s, insns %s, *code %p\n",OpCodePtr(mnem),mask,f);
	
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
	//	0101mmmmmmmmmmmm	5x0x	d:nm  d16(An)	An + displacement defined by extword 16 Bit signed
	//									 			0x5007 "d02"
	//												0x5E00 "d92"
	//
	//	0101mmmmmmmmmmmm	5x1x	d:nm d8(An,Xn)	An + 8bit displacement + index register
	//												defined by extword [0=D,1=A][idx][0=W,1=L][sc][0][signddisp]
	//									 			0x5017 "d02"
	//												0x5E10 "d92"
	//
	//	0110mmmmmmmmmmmm	6xxx	+:nm	(An)+	address reg indirect with post-increment
	//												0x6007 "+02"
	//												0x6E00 "+92"
	//
	//	0111mmmmmmmmmmmm	7xxx	-:nm	-(An)	address reg indirect with pre-decrement
	//												0x7007 "-02"
	//												0x7E00 "-92"
	//
	//	1000mmmmmmmmmmmm	8x0x	P		d(PC)	PC + 16bit signed displacement defined by extword
	//
	//	1000mmmmmmmmmmmm	8x1x	P	d8(PC,Xn)	PC + 8bit displacement + index register
	//												defined by extword [0=D,1=A][idx][0=W,1=L][sc][0][signddisp]
	//
	//	1001mmmmmmmmmmmm	9xxx	G		disp	8 bit disp (=0 +16 bit extword, =255 +32 bit extword)
	//
	//	1010mmmmmmmmmmmm	Axxx	I:nm	immed	immediate data in instruction
	//
	//	1011000000000000	B000	S		ABS S	SRC EA - Absolute short (16 bit Extword)
	//
	//	1011000000000001	B001	L		ABS L	SRC EA - Absoulte long (32 bit Extword)
	//
	//	1011000100000001	B100	S		ABS L	DEST EA - Absoulte long (32 bit Extword)
	//
	//	1011000100000001	B101	L		ABS L	DEST EA - Absoulte long (32 bit Extword)
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

	SetInvalEntries(instbl, syntbl, LR InvalidCode, "Invalid", 0xF000, 0xF000 );
	


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "1111111111111111", LR InvalidCode, "Invalid", 0xF000, 0xF000 );   


	// Generated From: 0000000000xxxxxx LR ori_b ori.b 0xE100 0x003F

	SetTable(instbl, syntbl, "0000000000000xxx", LR ori_b, "ori.b", 0xE100, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000000000010xxx", LR ori_b, "ori.b", 0xE100, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000000000011xxx", LR ori_b, "ori.b", 0xE100, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000000000100xxx", LR ori_b, "ori.b", 0xE100, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000000000101xxx", LR ori_b, "ori.b", 0xE100, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000000000110xxx", LR ori_b, "ori.b", 0xE100, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000000000111000", LR ori_b, "ori.b", 0xE100, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000000000111001", LR ori_b, "ori.b", 0xE100, 0xB001 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0000000000111100", LR ori_to_ccr, "ori", 0xE100, 0xF004 );   


	// Generated From: 0000000001xxxxxx LR ori_w ori.w 0xE200 0x003F

	SetTable(instbl, syntbl, "0000000001000xxx", LR ori_w, "ori.w", 0xE200, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000000001010xxx", LR ori_w, "ori.w", 0xE200, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000000001011xxx", LR ori_w, "ori.w", 0xE200, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000000001100xxx", LR ori_w, "ori.w", 0xE200, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000000001101xxx", LR ori_w, "ori.w", 0xE200, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000000001110xxx", LR ori_w, "ori.w", 0xE200, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000000001111000", LR ori_w, "ori.w", 0xE200, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000000001111001", LR ori_w, "ori.w", 0xE200, 0xB001 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0000000001111100", LR ori_to_sr, "ori", 0xE200, 0xF005 );   


	// Generated From: 0000000010xxxxxx LR ori_l ori.l 0xE300 0x003F

	SetTable(instbl, syntbl, "0000000010000xxx", LR ori_l, "ori.l", 0xE300, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000000010010xxx", LR ori_l, "ori.l", 0xE300, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000000010011xxx", LR ori_l, "ori.l", 0xE300, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000000010100xxx", LR ori_l, "ori.l", 0xE300, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000000010101xxx", LR ori_l, "ori.l", 0xE300, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000000010110xxx", LR ori_l, "ori.l", 0xE300, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000000010111000", LR ori_l, "ori.l", 0xE300, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000000010111001", LR ori_l, "ori.l", 0xE300, 0xB001 ); //srcXEA


	// Generated From: 0000xxx100xxxxxx LR btst_d btst 0x2E00 0x003F

	SetTable(instbl, syntbl, "0000xxx100000xxx", LR btst_d, "btst", 0x2E00, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx100010xxx", LR btst_d, "btst", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx100011xxx", LR btst_d, "btst", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx100100xxx", LR btst_d, "btst", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx100101xxx", LR btst_d, "btst", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx100110xxx", LR btst_d, "btst", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx100111000", LR btst_d, "btst", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000xxx100111001", LR btst_d, "btst", 0x2E00, 0xB001 ); //srcXEA
	SetTable(instbl, syntbl, "0000xxx100111011", LR btst_d, "btst", 0x2E00, 0x8010 ); //srcXEA
	SetTable(instbl, syntbl, "0000xxx100111100", LR btst_d, "btst", 0x2E00, 0xA000 ); //srcXEA


	// Generated From: 0000xxx101xxxxxx LR bchg_d bchg 0x2E00 0x003F

	SetTable(instbl, syntbl, "0000xxx101000xxx", LR bchg_d, "bchg", 0x2E00, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx101010xxx", LR bchg_d, "bchg", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx101011xxx", LR bchg_d, "bchg", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx101100xxx", LR bchg_d, "bchg", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx101101xxx", LR bchg_d, "bchg", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx101110xxx", LR bchg_d, "bchg", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx101111000", LR bchg_d, "bchg", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000xxx101111001", LR bchg_d, "bchg", 0x2E00, 0xB001 ); //srcXEA


	// Generated From: 0000xxx110xxxxxx LR bclr_d bclr 0x2E00 0x003F

	SetTable(instbl, syntbl, "0000xxx110000xxx", LR bclr_d, "bclr", 0x2E00, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx110010xxx", LR bclr_d, "bclr", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx110011xxx", LR bclr_d, "bclr", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx110100xxx", LR bclr_d, "bclr", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx110101xxx", LR bclr_d, "bclr", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx110110xxx", LR bclr_d, "bclr", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx110111000", LR bclr_d, "bclr", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000xxx110111001", LR bclr_d, "bclr", 0x2E00, 0xB001 ); //srcXEA


	// Generated From: 0000xxx111xxxxxx LR bset_d bset 0x2E00 0x003F

	SetTable(instbl, syntbl, "0000xxx111000xxx", LR bset_d, "bset", 0x2E00, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx111010xxx", LR bset_d, "bset", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx111011xxx", LR bset_d, "bset", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx111100xxx", LR bset_d, "bset", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx111101xxx", LR bset_d, "bset", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx111110xxx", LR bset_d, "bset", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000xxx111111000", LR bset_d, "bset", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000xxx111111001", LR bset_d, "bset", 0x2E00, 0xB001 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0000xxx100001xxx", LR movep_w_mr, "movep.w", 0x5007, 0x2E00 );   
	SetTable(instbl, syntbl, "0000xxx101001xxx", LR movep_l_mr, "movep.l", 0x5007, 0x2E00 );   
	SetTable(instbl, syntbl, "0000xxx110001xxx", LR movep_w_rm, "movep.w", 0x2E00, 0x5007 );   
	SetTable(instbl, syntbl, "0000xxx111001xxx", LR movep_l_rm, "movep.l", 0x2E00, 0x5007 );   


	// Generated From: 0000001000xxxxxx LR andi_b andi.b 0xE100 0x003F

	SetTable(instbl, syntbl, "0000001000000xxx", LR andi_b, "andi.b", 0xE100, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000001000010xxx", LR andi_b, "andi.b", 0xE100, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000001000011xxx", LR andi_b, "andi.b", 0xE100, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000001000100xxx", LR andi_b, "andi.b", 0xE100, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000001000101xxx", LR andi_b, "andi.b", 0xE100, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000001000110xxx", LR andi_b, "andi.b", 0xE100, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000001000111000", LR andi_b, "andi.b", 0xE100, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000001000111001", LR andi_b, "andi.b", 0xE100, 0xB001 ); //srcXEA


	// Generated From: 0000001001xxxxxx LR andi_w andi.w 0xE200 0x003F

	SetTable(instbl, syntbl, "0000001001000xxx", LR andi_w, "andi.w", 0xE200, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000001001010xxx", LR andi_w, "andi.w", 0xE200, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000001001011xxx", LR andi_w, "andi.w", 0xE200, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000001001100xxx", LR andi_w, "andi.w", 0xE200, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000001001101xxx", LR andi_w, "andi.w", 0xE200, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000001001110xxx", LR andi_w, "andi.w", 0xE200, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000001001111000", LR andi_w, "andi.w", 0xE200, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000001001111001", LR andi_w, "andi.w", 0xE200, 0xB001 ); //srcXEA


	// Generated From: 0000001010xxxxxx LR andi_l andi.l 0xE300 0x003F

	SetTable(instbl, syntbl, "0000001010000xxx", LR andi_l, "andi.l", 0xE300, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000001010010xxx", LR andi_l, "andi.l", 0xE300, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000001010011xxx", LR andi_l, "andi.l", 0xE300, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000001010100xxx", LR andi_l, "andi.l", 0xE300, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000001010101xxx", LR andi_l, "andi.l", 0xE300, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000001010110xxx", LR andi_l, "andi.l", 0xE300, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000001010111000", LR andi_l, "andi.l", 0xE300, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000001010111001", LR andi_l, "andi.l", 0xE300, 0xB001 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0000001000111100", LR andi_to_ccr, "andi", 0xE100, 0xF004 );   
	SetTable(instbl, syntbl, "0000001001111100", LR andi_to_sr, "andi", 0xE200, 0xF005 );   


	// Generated From: 0000010000xxxxxx LR subi_b subi.b 0xE100 0x003F

	SetTable(instbl, syntbl, "0000010000000xxx", LR subi_b, "subi.b", 0xE100, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000010000001xxx", LR subi_b, "subi.b", 0xE100, 0x3007 ); //srcEA
	SetTable(instbl, syntbl, "0000010000010xxx", LR subi_b, "subi.b", 0xE100, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000010000011xxx", LR subi_b, "subi.b", 0xE100, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000010000100xxx", LR subi_b, "subi.b", 0xE100, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000010000101xxx", LR subi_b, "subi.b", 0xE100, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000010000110xxx", LR subi_b, "subi.b", 0xE100, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000010000111000", LR subi_b, "subi.b", 0xE100, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000010000111001", LR subi_b, "subi.b", 0xE100, 0xB001 ); //srcXEA


	// Generated From: 0000010001xxxxxx LR subi_w subi.w 0xE200 0x003F

	SetTable(instbl, syntbl, "0000010001000xxx", LR subi_w, "subi.w", 0xE200, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000010001001xxx", LR subi_w, "subi.w", 0xE200, 0x3007 ); //srcEA
	SetTable(instbl, syntbl, "0000010001010xxx", LR subi_w, "subi.w", 0xE200, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000010001011xxx", LR subi_w, "subi.w", 0xE200, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000010001100xxx", LR subi_w, "subi.w", 0xE200, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000010001101xxx", LR subi_w, "subi.w", 0xE200, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000010001110xxx", LR subi_w, "subi.w", 0xE200, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000010001111000", LR subi_w, "subi.w", 0xE200, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000010001111001", LR subi_w, "subi.w", 0xE200, 0xB001 ); //srcXEA


	// Generated From: 0000010010xxxxxx LR subi_l subi.l 0xE300 0x003F

	SetTable(instbl, syntbl, "0000010010000xxx", LR subi_l, "subi.l", 0xE300, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000010010001xxx", LR subi_l, "subi.l", 0xE300, 0x3007 ); //srcEA
	SetTable(instbl, syntbl, "0000010010010xxx", LR subi_l, "subi.l", 0xE300, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000010010011xxx", LR subi_l, "subi.l", 0xE300, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000010010100xxx", LR subi_l, "subi.l", 0xE300, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000010010101xxx", LR subi_l, "subi.l", 0xE300, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000010010110xxx", LR subi_l, "subi.l", 0xE300, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000010010111000", LR subi_l, "subi.l", 0xE300, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000010010111001", LR subi_l, "subi.l", 0xE300, 0xB001 ); //srcXEA


	// Generated From: 0000011000xxxxxx LR addi_b addi.b 0xE100 0x003F

	SetTable(instbl, syntbl, "0000011000000xxx", LR addi_b, "addi.b", 0xE100, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000011000010xxx", LR addi_b, "addi.b", 0xE100, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000011000011xxx", LR addi_b, "addi.b", 0xE100, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000011000100xxx", LR addi_b, "addi.b", 0xE100, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000011000101xxx", LR addi_b, "addi.b", 0xE100, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000011000110xxx", LR addi_b, "addi.b", 0xE100, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000011000111000", LR addi_b, "addi.b", 0xE100, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000011000111001", LR addi_b, "addi.b", 0xE100, 0xB001 ); //srcXEA


	// Generated From: 0000011001xxxxxx LR addi_w addi.w 0xE200 0x003F

	SetTable(instbl, syntbl, "0000011001000xxx", LR addi_w, "addi.w", 0xE200, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000011001010xxx", LR addi_w, "addi.w", 0xE200, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000011001011xxx", LR addi_w, "addi.w", 0xE200, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000011001100xxx", LR addi_w, "addi.w", 0xE200, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000011001101xxx", LR addi_w, "addi.w", 0xE200, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000011001110xxx", LR addi_w, "addi.w", 0xE200, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000011001111000", LR addi_w, "addi.w", 0xE200, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000011001111001", LR addi_w, "addi.w", 0xE200, 0xB001 ); //srcXEA


	// Generated From: 0000011010xxxxxx LR addi_l addi.l 0xE300 0x003F

	SetTable(instbl, syntbl, "0000011010000xxx", LR addi_l, "addi.l", 0xE300, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000011010010xxx", LR addi_l, "addi.l", 0xE300, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000011010011xxx", LR addi_l, "addi.l", 0xE300, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000011010100xxx", LR addi_l, "addi.l", 0xE300, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000011010101xxx", LR addi_l, "addi.l", 0xE300, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000011010110xxx", LR addi_l, "addi.l", 0xE300, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000011010111000", LR addi_l, "addi.l", 0xE300, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000011010111001", LR addi_l, "addi.l", 0xE300, 0xB001 ); //srcXEA


	// Generated From: 0000100000xxxxxx LR btst_s btst.s 0xE100 0x003F

	SetTable(instbl, syntbl, "0000100000000xxx", LR btst_s, "btst.s", 0xE100, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000100000010xxx", LR btst_s, "btst.s", 0xE100, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000100000011xxx", LR btst_s, "btst.s", 0xE100, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000100000100xxx", LR btst_s, "btst.s", 0xE100, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000100000101xxx", LR btst_s, "btst.s", 0xE100, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000100000110xxx", LR btst_s, "btst.s", 0xE100, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000100000111000", LR btst_s, "btst.s", 0xE100, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000100000111001", LR btst_s, "btst.s", 0xE100, 0xB001 ); //srcXEA
	SetTable(instbl, syntbl, "0000100000111011", LR btst_s, "btst.s", 0xE100, 0x8010 ); //srcXEA
	SetTable(instbl, syntbl, "0000100000111100", LR btst_s, "btst.s", 0xE100, 0xA000 ); //srcXEA


	// Generated From: 0000100001xxxxxx LR bchg_s bchg.s 0xE100 0x003F

	SetTable(instbl, syntbl, "0000100001000xxx", LR bchg_s, "bchg.s", 0xE100, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000100001010xxx", LR bchg_s, "bchg.s", 0xE100, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000100001011xxx", LR bchg_s, "bchg.s", 0xE100, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000100001100xxx", LR bchg_s, "bchg.s", 0xE100, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000100001101xxx", LR bchg_s, "bchg.s", 0xE100, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000100001110xxx", LR bchg_s, "bchg.s", 0xE100, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000100001111000", LR bchg_s, "bchg.s", 0xE100, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000100001111001", LR bchg_s, "bchg.s", 0xE100, 0xB001 ); //srcXEA


	// Generated From: 0000100010xxxxxx LR bclr_s bclr.s 0xE100 0x003F

	SetTable(instbl, syntbl, "0000100010000xxx", LR bclr_s, "bclr.s", 0xE100, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000100010010xxx", LR bclr_s, "bclr.s", 0xE100, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000100010011xxx", LR bclr_s, "bclr.s", 0xE100, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000100010100xxx", LR bclr_s, "bclr.s", 0xE100, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000100010101xxx", LR bclr_s, "bclr.s", 0xE100, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000100010110xxx", LR bclr_s, "bclr.s", 0xE100, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000100010111000", LR bclr_s, "bclr.s", 0xE100, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000100010111001", LR bclr_s, "bclr.s", 0xE100, 0xB001 ); //srcXEA


	// Generated From: 0000100011xxxxxx LR bset_s bset.s 0xE100 0x003F

	SetTable(instbl, syntbl, "0000100011000xxx", LR bset_s, "bset.s", 0xE100, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000100011010xxx", LR bset_s, "bset.s", 0xE100, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000100011011xxx", LR bset_s, "bset.s", 0xE100, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000100011100xxx", LR bset_s, "bset.s", 0xE100, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000100011101xxx", LR bset_s, "bset.s", 0xE100, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000100011110xxx", LR bset_s, "bset.s", 0xE100, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000100011111000", LR bset_s, "bset.s", 0xE100, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000100011111001", LR bset_s, "bset.s", 0xE100, 0xB001 ); //srcXEA


	// Generated From: 0000101000xxxxxx LR eori_b eori.b 0xE100 0x003F

	SetTable(instbl, syntbl, "0000101000000xxx", LR eori_b, "eori.b", 0xE100, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000101000010xxx", LR eori_b, "eori.b", 0xE100, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000101000011xxx", LR eori_b, "eori.b", 0xE100, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000101000100xxx", LR eori_b, "eori.b", 0xE100, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000101000101xxx", LR eori_b, "eori.b", 0xE100, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000101000110xxx", LR eori_b, "eori.b", 0xE100, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000101000111000", LR eori_b, "eori.b", 0xE100, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000101000111001", LR eori_b, "eori.b", 0xE100, 0xB001 ); //srcXEA


	// Generated From: 0000101001xxxxxx LR eori_w eori.w 0xE200 0x003F

	SetTable(instbl, syntbl, "0000101001000xxx", LR eori_w, "eori.w", 0xE200, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000101001010xxx", LR eori_w, "eori.w", 0xE200, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000101001011xxx", LR eori_w, "eori.w", 0xE200, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000101001100xxx", LR eori_w, "eori.w", 0xE200, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000101001101xxx", LR eori_w, "eori.w", 0xE200, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000101001110xxx", LR eori_w, "eori.w", 0xE200, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000101001111000", LR eori_w, "eori.w", 0xE200, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000101001111001", LR eori_w, "eori.w", 0xE200, 0xB001 ); //srcXEA


	// Generated From: 0000101010xxxxxx LR eori_l eori.l 0xE300 0x003F

	SetTable(instbl, syntbl, "0000101010000xxx", LR eori_l, "eori.l", 0xE300, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000101010010xxx", LR eori_l, "eori.l", 0xE300, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000101010011xxx", LR eori_l, "eori.l", 0xE300, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000101010100xxx", LR eori_l, "eori.l", 0xE300, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000101010101xxx", LR eori_l, "eori.l", 0xE300, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000101010110xxx", LR eori_l, "eori.l", 0xE300, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000101010111000", LR eori_l, "eori.l", 0xE300, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000101010111001", LR eori_l, "eori.l", 0xE300, 0xB001 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0000101000111100", LR eori_to_ccr, "eori", 0xE200, 0xF004 );   
	SetTable(instbl, syntbl, "0000101001111100", LR eori_to_sr, "eori", 0xE200, 0xF005 );   


	// Generated From: 0000110000xxxxxx LR cmpi_b cmpi.b 0xE100 0x003F

	SetTable(instbl, syntbl, "0000110000000xxx", LR cmpi_b, "cmpi.b", 0xE100, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000110000010xxx", LR cmpi_b, "cmpi.b", 0xE100, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000110000011xxx", LR cmpi_b, "cmpi.b", 0xE100, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000110000100xxx", LR cmpi_b, "cmpi.b", 0xE100, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000110000101xxx", LR cmpi_b, "cmpi.b", 0xE100, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000110000110xxx", LR cmpi_b, "cmpi.b", 0xE100, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000110000111000", LR cmpi_b, "cmpi.b", 0xE100, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000110000111001", LR cmpi_b, "cmpi.b", 0xE100, 0xB001 ); //srcXEA
	SetTable(instbl, syntbl, "0000110000111011", LR cmpi_b, "cmpi.b", 0xE100, 0x8010 ); //srcXEA
	SetTable(instbl, syntbl, "0000110000111100", LR cmpi_b, "cmpi.b", 0xE100, 0xA000 ); //srcXEA


	// Generated From: 0000110001xxxxxx LR cmpi_w cmpi.w 0xE200 0x003F

	SetTable(instbl, syntbl, "0000110001000xxx", LR cmpi_w, "cmpi.w", 0xE200, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000110001010xxx", LR cmpi_w, "cmpi.w", 0xE200, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000110001011xxx", LR cmpi_w, "cmpi.w", 0xE200, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000110001100xxx", LR cmpi_w, "cmpi.w", 0xE200, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000110001101xxx", LR cmpi_w, "cmpi.w", 0xE200, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000110001110xxx", LR cmpi_w, "cmpi.w", 0xE200, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000110001111000", LR cmpi_w, "cmpi.w", 0xE200, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000110001111001", LR cmpi_w, "cmpi.w", 0xE200, 0xB001 ); //srcXEA
	SetTable(instbl, syntbl, "0000110001111011", LR cmpi_w, "cmpi.w", 0xE200, 0x8010 ); //srcXEA
	SetTable(instbl, syntbl, "0000110001111100", LR cmpi_w, "cmpi.w", 0xE200, 0xA000 ); //srcXEA


	// Generated From: 0000110010xxxxxx LR cmpi_l cmpi.l 0xE300 0x003F

	SetTable(instbl, syntbl, "0000110010000xxx", LR cmpi_l, "cmpi.l", 0xE300, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0000110010010xxx", LR cmpi_l, "cmpi.l", 0xE300, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0000110010011xxx", LR cmpi_l, "cmpi.l", 0xE300, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0000110010100xxx", LR cmpi_l, "cmpi.l", 0xE300, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0000110010101xxx", LR cmpi_l, "cmpi.l", 0xE300, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0000110010110xxx", LR cmpi_l, "cmpi.l", 0xE300, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0000110010111000", LR cmpi_l, "cmpi.l", 0xE300, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0000110010111001", LR cmpi_l, "cmpi.l", 0xE300, 0xB001 ); //srcXEA
	SetTable(instbl, syntbl, "0000110010111011", LR cmpi_l, "cmpi.l", 0xE300, 0x8010 ); //srcXEA
	SetTable(instbl, syntbl, "0000110010111100", LR cmpi_l, "cmpi.l", 0xE300, 0xA000 ); //srcXEA


	// Generated From: 0001xxxxxxxxxxxx LR move_b move.b 0x003F 0x1FC0

	SetTable(instbl, syntbl, "0001xxx000001xxx", LR move_b, "move.b", 0x3007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx000010xxx", LR move_b, "move.b", 0x4007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx000011xxx", LR move_b, "move.b", 0x6007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx000100xxx", LR move_b, "move.b", 0x7007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx000101xxx", LR move_b, "move.b", 0x5007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx000110xxx", LR move_b, "move.b", 0x5017, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx000111000", LR move_b, "move.b", 0xB000, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx000111001", LR move_b, "move.b", 0xB001, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx000111010", LR move_b, "move.b", 0x8000, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx000111011", LR move_b, "move.b", 0x8010, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx000111100", LR move_b, "move.b", 0xA000, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx011000xxx", LR move_b, "move.b", 0x2007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx011001xxx", LR move_b, "move.b", 0x3007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx011010xxx", LR move_b, "move.b", 0x4007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx011011xxx", LR move_b, "move.b", 0x6007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx011100xxx", LR move_b, "move.b", 0x7007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx011101xxx", LR move_b, "move.b", 0x5007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx011110xxx", LR move_b, "move.b", 0x5017, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx011111000", LR move_b, "move.b", 0xB000, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx011111001", LR move_b, "move.b", 0xB001, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx011111010", LR move_b, "move.b", 0x8000, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx011111011", LR move_b, "move.b", 0x8010, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx011111100", LR move_b, "move.b", 0xA000, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx100000xxx", LR move_b, "move.b", 0x2007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx100001xxx", LR move_b, "move.b", 0x3007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx100010xxx", LR move_b, "move.b", 0x4007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx100011xxx", LR move_b, "move.b", 0x6007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx100100xxx", LR move_b, "move.b", 0x7007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx100101xxx", LR move_b, "move.b", 0x5007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx100110xxx", LR move_b, "move.b", 0x5017, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx100111000", LR move_b, "move.b", 0xB000, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx100111001", LR move_b, "move.b", 0xB001, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx100111010", LR move_b, "move.b", 0x8000, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx100111011", LR move_b, "move.b", 0x8010, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx100111100", LR move_b, "move.b", 0xA000, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx101000xxx", LR move_b, "move.b", 0x2007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx101001xxx", LR move_b, "move.b", 0x3007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx101010xxx", LR move_b, "move.b", 0x4007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx101011xxx", LR move_b, "move.b", 0x6007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx101100xxx", LR move_b, "move.b", 0x7007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx101101xxx", LR move_b, "move.b", 0x5007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx101110xxx", LR move_b, "move.b", 0x5017, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx101111000", LR move_b, "move.b", 0xB000, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx101111001", LR move_b, "move.b", 0xB001, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx101111010", LR move_b, "move.b", 0x8000, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx101111011", LR move_b, "move.b", 0x8010, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx101111100", LR move_b, "move.b", 0xA000, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx110000xxx", LR move_b, "move.b", 0x2007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx110001xxx", LR move_b, "move.b", 0x3007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx110010xxx", LR move_b, "move.b", 0x4007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx110011xxx", LR move_b, "move.b", 0x6007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx110100xxx", LR move_b, "move.b", 0x7007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx110101xxx", LR move_b, "move.b", 0x5007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx110110xxx", LR move_b, "move.b", 0x5017, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0001xxx110111000", LR move_b, "move.b", 0xB000, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx110111001", LR move_b, "move.b", 0xB001, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx110111010", LR move_b, "move.b", 0x8000, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx110111011", LR move_b, "move.b", 0x8010, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001xxx110111100", LR move_b, "move.b", 0xA000, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0001000111000xxx", LR move_b, "move.b", 0x2007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001000111001xxx", LR move_b, "move.b", 0x3007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001000111010xxx", LR move_b, "move.b", 0x4007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001000111011xxx", LR move_b, "move.b", 0x6007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001000111100xxx", LR move_b, "move.b", 0x7007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001000111101xxx", LR move_b, "move.b", 0x5007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001000111110xxx", LR move_b, "move.b", 0x5017, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001000111111000", LR move_b, "move.b", 0xB000, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0001000111111001", LR move_b, "move.b", 0xB001, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0001000111111010", LR move_b, "move.b", 0x8000, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0001000111111011", LR move_b, "move.b", 0x8010, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0001000111111100", LR move_b, "move.b", 0xA000, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0001001111000xxx", LR move_b, "move.b", 0x2007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001001111001xxx", LR move_b, "move.b", 0x3007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001001111010xxx", LR move_b, "move.b", 0x4007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001001111011xxx", LR move_b, "move.b", 0x6007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001001111100xxx", LR move_b, "move.b", 0x7007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001001111101xxx", LR move_b, "move.b", 0x5007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001001111110xxx", LR move_b, "move.b", 0x5017, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0001001111111000", LR move_b, "move.b", 0xB000, 0xB001 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0001001111111001", LR move_b, "move.b", 0xB001, 0xB001 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0001001111111010", LR move_b, "move.b", 0x8000, 0xB001 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0001001111111011", LR move_b, "move.b", 0x8010, 0xB001 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0001001111111100", LR move_b, "move.b", 0xA000, 0xB001 ); //EAabsEAabs


	// Generated From: 0001xxxxxx000xxx LR move_b_from_dn move.b 0x2007 0x1FC0

	SetTable(instbl, syntbl, "0001xxx011000xxx", LR move_b_from_dn, "move.b", 0x2007, 0x6E00 ); //destEA
	SetTable(instbl, syntbl, "0001xxx100000xxx", LR move_b_from_dn, "move.b", 0x2007, 0x7E00 ); //destEA
	SetTable(instbl, syntbl, "0001xxx101000xxx", LR move_b_from_dn, "move.b", 0x2007, 0x5E00 ); //destEA
	SetTable(instbl, syntbl, "0001xxx110000xxx", LR move_b_from_dn, "move.b", 0x2007, 0x5E10 ); //destEA
	SetTable(instbl, syntbl, "0001000111000xxx", LR move_b_from_dn, "move.b", 0x2007, 0xB000 ); //destXEA
	SetTable(instbl, syntbl, "0001001111000xxx", LR move_b_from_dn, "move.b", 0x2007, 0xB001 ); //destXEA


	// Generated From: 0001xxx000xxxxxx LR move_b_to_dn move.b 0x003F 0x2E00

	SetTable(instbl, syntbl, "0001xxx000001xxx", LR move_b_to_dn, "move.b", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0001xxx000010xxx", LR move_b_to_dn, "move.b", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0001xxx000011xxx", LR move_b_to_dn, "move.b", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0001xxx000100xxx", LR move_b_to_dn, "move.b", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0001xxx000101xxx", LR move_b_to_dn, "move.b", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0001xxx000110xxx", LR move_b_to_dn, "move.b", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0001xxx000111000", LR move_b_to_dn, "move.b", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0001xxx000111001", LR move_b_to_dn, "move.b", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0001xxx000111010", LR move_b_to_dn, "move.b", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0001xxx000111011", LR move_b_to_dn, "move.b", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0001xxx000111100", LR move_b_to_dn, "move.b", 0xA000, 0x2E00 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0001xxx000000xxx", LR move_b_reg, "move.b", 0x2007, 0x2E00 );   


	// Generated From: 0010xxxxxxxxxxxx LR move_l move.l 0x003F 0x1FC0

	SetTable(instbl, syntbl, "0010xxx000001xxx", LR move_l, "move.l", 0x3007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx000010xxx", LR move_l, "move.l", 0x4007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx000011xxx", LR move_l, "move.l", 0x6007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx000100xxx", LR move_l, "move.l", 0x7007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx000101xxx", LR move_l, "move.l", 0x5007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx000110xxx", LR move_l, "move.l", 0x5017, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx000111000", LR move_l, "move.l", 0xB000, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx000111001", LR move_l, "move.l", 0xB001, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx000111010", LR move_l, "move.l", 0x8000, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx000111011", LR move_l, "move.l", 0x8010, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx000111100", LR move_l, "move.l", 0xA000, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx011000xxx", LR move_l, "move.l", 0x2007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx011001xxx", LR move_l, "move.l", 0x3007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx011010xxx", LR move_l, "move.l", 0x4007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx011011xxx", LR move_l, "move.l", 0x6007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx011100xxx", LR move_l, "move.l", 0x7007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx011101xxx", LR move_l, "move.l", 0x5007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx011110xxx", LR move_l, "move.l", 0x5017, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx011111000", LR move_l, "move.l", 0xB000, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx011111001", LR move_l, "move.l", 0xB001, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx011111010", LR move_l, "move.l", 0x8000, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx011111011", LR move_l, "move.l", 0x8010, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx011111100", LR move_l, "move.l", 0xA000, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx100000xxx", LR move_l, "move.l", 0x2007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx100001xxx", LR move_l, "move.l", 0x3007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx100010xxx", LR move_l, "move.l", 0x4007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx100011xxx", LR move_l, "move.l", 0x6007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx100100xxx", LR move_l, "move.l", 0x7007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx100101xxx", LR move_l, "move.l", 0x5007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx100110xxx", LR move_l, "move.l", 0x5017, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx100111000", LR move_l, "move.l", 0xB000, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx100111001", LR move_l, "move.l", 0xB001, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx100111010", LR move_l, "move.l", 0x8000, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx100111011", LR move_l, "move.l", 0x8010, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx100111100", LR move_l, "move.l", 0xA000, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx101000xxx", LR move_l, "move.l", 0x2007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx101001xxx", LR move_l, "move.l", 0x3007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx101010xxx", LR move_l, "move.l", 0x4007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx101011xxx", LR move_l, "move.l", 0x6007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx101100xxx", LR move_l, "move.l", 0x7007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx101101xxx", LR move_l, "move.l", 0x5007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx101110xxx", LR move_l, "move.l", 0x5017, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx101111000", LR move_l, "move.l", 0xB000, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx101111001", LR move_l, "move.l", 0xB001, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx101111010", LR move_l, "move.l", 0x8000, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx101111011", LR move_l, "move.l", 0x8010, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx101111100", LR move_l, "move.l", 0xA000, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx110000xxx", LR move_l, "move.l", 0x2007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx110001xxx", LR move_l, "move.l", 0x3007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx110010xxx", LR move_l, "move.l", 0x4007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx110011xxx", LR move_l, "move.l", 0x6007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx110100xxx", LR move_l, "move.l", 0x7007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx110101xxx", LR move_l, "move.l", 0x5007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx110110xxx", LR move_l, "move.l", 0x5017, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0010xxx110111000", LR move_l, "move.l", 0xB000, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx110111001", LR move_l, "move.l", 0xB001, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx110111010", LR move_l, "move.l", 0x8000, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx110111011", LR move_l, "move.l", 0x8010, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010xxx110111100", LR move_l, "move.l", 0xA000, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0010000111000xxx", LR move_l, "move.l", 0x2007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010000111001xxx", LR move_l, "move.l", 0x3007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010000111010xxx", LR move_l, "move.l", 0x4007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010000111011xxx", LR move_l, "move.l", 0x6007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010000111100xxx", LR move_l, "move.l", 0x7007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010000111101xxx", LR move_l, "move.l", 0x5007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010000111110xxx", LR move_l, "move.l", 0x5017, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010000111111000", LR move_l, "move.l", 0xB000, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0010000111111001", LR move_l, "move.l", 0xB001, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0010000111111010", LR move_l, "move.l", 0x8000, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0010000111111011", LR move_l, "move.l", 0x8010, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0010000111111100", LR move_l, "move.l", 0xA000, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0010001111000xxx", LR move_l, "move.l", 0x2007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010001111001xxx", LR move_l, "move.l", 0x3007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010001111010xxx", LR move_l, "move.l", 0x4007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010001111011xxx", LR move_l, "move.l", 0x6007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010001111100xxx", LR move_l, "move.l", 0x7007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010001111101xxx", LR move_l, "move.l", 0x5007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010001111110xxx", LR move_l, "move.l", 0x5017, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0010001111111000", LR move_l, "move.l", 0xB000, 0xB001 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0010001111111001", LR move_l, "move.l", 0xB001, 0xB001 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0010001111111010", LR move_l, "move.l", 0x8000, 0xB001 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0010001111111011", LR move_l, "move.l", 0x8010, 0xB001 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0010001111111100", LR move_l, "move.l", 0xA000, 0xB001 ); //EAabsEAabs


	// Generated From: 0010xxxxxx000xxx LR move_l_from_dn move.l 0x2007 0x1FC0

	SetTable(instbl, syntbl, "0010xxx011000xxx", LR move_l_from_dn, "move.l", 0x2007, 0x6E00 ); //destEA
	SetTable(instbl, syntbl, "0010xxx100000xxx", LR move_l_from_dn, "move.l", 0x2007, 0x7E00 ); //destEA
	SetTable(instbl, syntbl, "0010xxx101000xxx", LR move_l_from_dn, "move.l", 0x2007, 0x5E00 ); //destEA
	SetTable(instbl, syntbl, "0010xxx110000xxx", LR move_l_from_dn, "move.l", 0x2007, 0x5E10 ); //destEA
	SetTable(instbl, syntbl, "0010000111000xxx", LR move_l_from_dn, "move.l", 0x2007, 0xB000 ); //destXEA
	SetTable(instbl, syntbl, "0010001111000xxx", LR move_l_from_dn, "move.l", 0x2007, 0xB001 ); //destXEA


	// Generated From: 0010xxx000xxxxxx LR move_l_to_dn move.l 0x003F 0x2E00

	SetTable(instbl, syntbl, "0010xxx000001xxx", LR move_l_to_dn, "move.l", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0010xxx000010xxx", LR move_l_to_dn, "move.l", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0010xxx000011xxx", LR move_l_to_dn, "move.l", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0010xxx000100xxx", LR move_l_to_dn, "move.l", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0010xxx000101xxx", LR move_l_to_dn, "move.l", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0010xxx000110xxx", LR move_l_to_dn, "move.l", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0010xxx000111000", LR move_l_to_dn, "move.l", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0010xxx000111001", LR move_l_to_dn, "move.l", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0010xxx000111010", LR move_l_to_dn, "move.l", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0010xxx000111011", LR move_l_to_dn, "move.l", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0010xxx000111100", LR move_l_to_dn, "move.l", 0xA000, 0x2E00 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0010xxx000000xxx", LR move_l_reg, "move.l", 0x2007, 0x2E00 );   


	// Generated From: 0010xxx001xxxxxx LR movea_l movea.l 0x003F 0x3E00

	SetTable(instbl, syntbl, "0010xxx001000xxx", LR movea_l, "movea.l", 0x2007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "0010xxx001010xxx", LR movea_l, "movea.l", 0x4007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "0010xxx001011xxx", LR movea_l, "movea.l", 0x6007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "0010xxx001100xxx", LR movea_l, "movea.l", 0x7007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "0010xxx001101xxx", LR movea_l, "movea.l", 0x5007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "0010xxx001110xxx", LR movea_l, "movea.l", 0x5017, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "0010xxx001111000", LR movea_l, "movea.l", 0xB000, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "0010xxx001111001", LR movea_l, "movea.l", 0xB001, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "0010xxx001111010", LR movea_l, "movea.l", 0x8000, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "0010xxx001111011", LR movea_l, "movea.l", 0x8010, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "0010xxx001111100", LR movea_l, "movea.l", 0xA000, 0x3E00 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0010xxx001001xxx", LR movea_l_an, "movea.l", 0x3007, 0x3E00 );   


	// Generated From: 0011xxxxxxxxxxxx LR move_w move.w 0x003F 0x1FC0

	SetTable(instbl, syntbl, "0011xxx000001xxx", LR move_w, "move.w", 0x3007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx000010xxx", LR move_w, "move.w", 0x4007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx000011xxx", LR move_w, "move.w", 0x6007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx000100xxx", LR move_w, "move.w", 0x7007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx000101xxx", LR move_w, "move.w", 0x5007, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx000110xxx", LR move_w, "move.w", 0x5017, 0x2E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx000111000", LR move_w, "move.w", 0xB000, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx000111001", LR move_w, "move.w", 0xB001, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx000111010", LR move_w, "move.w", 0x8000, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx000111011", LR move_w, "move.w", 0x8010, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx000111100", LR move_w, "move.w", 0xA000, 0x2E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx011000xxx", LR move_w, "move.w", 0x2007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx011001xxx", LR move_w, "move.w", 0x3007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx011010xxx", LR move_w, "move.w", 0x4007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx011011xxx", LR move_w, "move.w", 0x6007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx011100xxx", LR move_w, "move.w", 0x7007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx011101xxx", LR move_w, "move.w", 0x5007, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx011110xxx", LR move_w, "move.w", 0x5017, 0x6E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx011111000", LR move_w, "move.w", 0xB000, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx011111001", LR move_w, "move.w", 0xB001, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx011111010", LR move_w, "move.w", 0x8000, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx011111011", LR move_w, "move.w", 0x8010, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx011111100", LR move_w, "move.w", 0xA000, 0x6E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx100000xxx", LR move_w, "move.w", 0x2007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx100001xxx", LR move_w, "move.w", 0x3007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx100010xxx", LR move_w, "move.w", 0x4007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx100011xxx", LR move_w, "move.w", 0x6007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx100100xxx", LR move_w, "move.w", 0x7007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx100101xxx", LR move_w, "move.w", 0x5007, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx100110xxx", LR move_w, "move.w", 0x5017, 0x7E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx100111000", LR move_w, "move.w", 0xB000, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx100111001", LR move_w, "move.w", 0xB001, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx100111010", LR move_w, "move.w", 0x8000, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx100111011", LR move_w, "move.w", 0x8010, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx100111100", LR move_w, "move.w", 0xA000, 0x7E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx101000xxx", LR move_w, "move.w", 0x2007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx101001xxx", LR move_w, "move.w", 0x3007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx101010xxx", LR move_w, "move.w", 0x4007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx101011xxx", LR move_w, "move.w", 0x6007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx101100xxx", LR move_w, "move.w", 0x7007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx101101xxx", LR move_w, "move.w", 0x5007, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx101110xxx", LR move_w, "move.w", 0x5017, 0x5E00 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx101111000", LR move_w, "move.w", 0xB000, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx101111001", LR move_w, "move.w", 0xB001, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx101111010", LR move_w, "move.w", 0x8000, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx101111011", LR move_w, "move.w", 0x8010, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx101111100", LR move_w, "move.w", 0xA000, 0x5E00 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx110000xxx", LR move_w, "move.w", 0x2007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx110001xxx", LR move_w, "move.w", 0x3007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx110010xxx", LR move_w, "move.w", 0x4007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx110011xxx", LR move_w, "move.w", 0x6007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx110100xxx", LR move_w, "move.w", 0x7007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx110101xxx", LR move_w, "move.w", 0x5007, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx110110xxx", LR move_w, "move.w", 0x5017, 0x5E10 ); //EAregEAreg
	SetTable(instbl, syntbl, "0011xxx110111000", LR move_w, "move.w", 0xB000, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx110111001", LR move_w, "move.w", 0xB001, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx110111010", LR move_w, "move.w", 0x8000, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx110111011", LR move_w, "move.w", 0x8010, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011xxx110111100", LR move_w, "move.w", 0xA000, 0x5E10 ); //EAregEAabs
	SetTable(instbl, syntbl, "0011000111000xxx", LR move_w, "move.w", 0x2007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011000111001xxx", LR move_w, "move.w", 0x3007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011000111010xxx", LR move_w, "move.w", 0x4007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011000111011xxx", LR move_w, "move.w", 0x6007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011000111100xxx", LR move_w, "move.w", 0x7007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011000111101xxx", LR move_w, "move.w", 0x5007, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011000111110xxx", LR move_w, "move.w", 0x5017, 0xB000 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011000111111000", LR move_w, "move.w", 0xB000, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0011000111111001", LR move_w, "move.w", 0xB001, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0011000111111010", LR move_w, "move.w", 0x8000, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0011000111111011", LR move_w, "move.w", 0x8010, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0011000111111100", LR move_w, "move.w", 0xA000, 0xB000 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0011001111000xxx", LR move_w, "move.w", 0x2007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011001111001xxx", LR move_w, "move.w", 0x3007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011001111010xxx", LR move_w, "move.w", 0x4007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011001111011xxx", LR move_w, "move.w", 0x6007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011001111100xxx", LR move_w, "move.w", 0x7007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011001111101xxx", LR move_w, "move.w", 0x5007, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011001111110xxx", LR move_w, "move.w", 0x5017, 0xB001 ); //EAabsEAreg
	SetTable(instbl, syntbl, "0011001111111000", LR move_w, "move.w", 0xB000, 0xB001 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0011001111111001", LR move_w, "move.w", 0xB001, 0xB001 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0011001111111010", LR move_w, "move.w", 0x8000, 0xB001 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0011001111111011", LR move_w, "move.w", 0x8010, 0xB001 ); //EAabsEAabs
	SetTable(instbl, syntbl, "0011001111111100", LR move_w, "move.w", 0xA000, 0xB001 ); //EAabsEAabs


	// Generated From: 0011xxxxxx000xxx LR move_w_from_dn move.w 0x2007 0x1FC0

	SetTable(instbl, syntbl, "0011xxx011000xxx", LR move_w_from_dn, "move.w", 0x2007, 0x6E00 ); //destEA
	SetTable(instbl, syntbl, "0011xxx100000xxx", LR move_w_from_dn, "move.w", 0x2007, 0x7E00 ); //destEA
	SetTable(instbl, syntbl, "0011xxx101000xxx", LR move_w_from_dn, "move.w", 0x2007, 0x5E00 ); //destEA
	SetTable(instbl, syntbl, "0011xxx110000xxx", LR move_w_from_dn, "move.w", 0x2007, 0x5E10 ); //destEA
	SetTable(instbl, syntbl, "0011000111000xxx", LR move_w_from_dn, "move.w", 0x2007, 0xB000 ); //destXEA
	SetTable(instbl, syntbl, "0011001111000xxx", LR move_w_from_dn, "move.w", 0x2007, 0xB001 ); //destXEA


	// Generated From: 0011xxx000xxxxxx LR move_w_to_dn move.w 0x003F 0x2E00

	SetTable(instbl, syntbl, "0011xxx000001xxx", LR move_w_to_dn, "move.w", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0011xxx000010xxx", LR move_w_to_dn, "move.w", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0011xxx000011xxx", LR move_w_to_dn, "move.w", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0011xxx000100xxx", LR move_w_to_dn, "move.w", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0011xxx000101xxx", LR move_w_to_dn, "move.w", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0011xxx000110xxx", LR move_w_to_dn, "move.w", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0011xxx000111000", LR move_w_to_dn, "move.w", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0011xxx000111001", LR move_w_to_dn, "move.w", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0011xxx000111010", LR move_w_to_dn, "move.w", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0011xxx000111011", LR move_w_to_dn, "move.w", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0011xxx000111100", LR move_w_to_dn, "move.w", 0xA000, 0x2E00 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0011xxx000000xxx", LR move_w_reg, "move.w", 0x2007, 0x2E00 );   


	// Generated From: 0011xxx001xxxxxx LR movea_w movea.w 0x003F 0x2E00

	SetTable(instbl, syntbl, "0011xxx001000xxx", LR movea_w, "movea.w", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0011xxx001001xxx", LR movea_w, "movea.w", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0011xxx001010xxx", LR movea_w, "movea.w", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0011xxx001011xxx", LR movea_w, "movea.w", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0011xxx001100xxx", LR movea_w, "movea.w", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0011xxx001101xxx", LR movea_w, "movea.w", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0011xxx001110xxx", LR movea_w, "movea.w", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0011xxx001111000", LR movea_w, "movea.w", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0011xxx001111001", LR movea_w, "movea.w", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0011xxx001111010", LR movea_w, "movea.w", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0011xxx001111011", LR movea_w, "movea.w", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0011xxx001111100", LR movea_w, "movea.w", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 0100000000xxxxxx LR negx_b negx.b 0x003F 0xF000

	SetTable(instbl, syntbl, "0100000000000xxx", LR negx_b, "negx.b", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000000010xxx", LR negx_b, "negx.b", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000000011xxx", LR negx_b, "negx.b", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000000100xxx", LR negx_b, "negx.b", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000000101xxx", LR negx_b, "negx.b", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000000110xxx", LR negx_b, "negx.b", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000000111000", LR negx_b, "negx.b", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100000000111001", LR negx_b, "negx.b", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0100000001xxxxxx LR negx_w negx.w 0x003F 0xF000

	SetTable(instbl, syntbl, "0100000001000xxx", LR negx_w, "negx.w", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000001010xxx", LR negx_w, "negx.w", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000001011xxx", LR negx_w, "negx.w", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000001100xxx", LR negx_w, "negx.w", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000001101xxx", LR negx_w, "negx.w", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000001110xxx", LR negx_w, "negx.w", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000001111000", LR negx_w, "negx.w", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100000001111001", LR negx_w, "negx.w", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0100000010xxxxxx LR negx_l negx.l 0x003F 0xF000

	SetTable(instbl, syntbl, "0100000010000xxx", LR negx_l, "negx.l", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000010010xxx", LR negx_l, "negx.l", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000010011xxx", LR negx_l, "negx.l", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000010100xxx", LR negx_l, "negx.l", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000010101xxx", LR negx_l, "negx.l", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000010110xxx", LR negx_l, "negx.l", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100000010111000", LR negx_l, "negx.l", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100000010111001", LR negx_l, "negx.l", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0100000011xxxxxx LR move_from_sr move 0xF005 0x003F

	SetTable(instbl, syntbl, "0100000011000xxx", LR move_from_sr, "move", 0xF005, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0100000011010xxx", LR move_from_sr, "move", 0xF005, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0100000011011xxx", LR move_from_sr, "move", 0xF005, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0100000011100xxx", LR move_from_sr, "move", 0xF005, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0100000011101xxx", LR move_from_sr, "move", 0xF005, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0100000011110xxx", LR move_from_sr, "move", 0xF005, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0100000011111000", LR move_from_sr, "move", 0xF005, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0100000011111001", LR move_from_sr, "move", 0xF005, 0xB001 ); //srcXEA


	// Generated From: 0100xxx110xxxxxx LR chk chk.w 0x003F 0x2E00

	SetTable(instbl, syntbl, "0100xxx110000xxx", LR chk, "chk.w", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0100xxx110010xxx", LR chk, "chk.w", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0100xxx110011xxx", LR chk, "chk.w", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0100xxx110100xxx", LR chk, "chk.w", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0100xxx110101xxx", LR chk, "chk.w", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0100xxx110110xxx", LR chk, "chk.w", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0100xxx110111000", LR chk, "chk.w", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0100xxx110111001", LR chk, "chk.w", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0100xxx110111010", LR chk, "chk.w", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0100xxx110111011", LR chk, "chk.w", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0100xxx110111100", LR chk, "chk.w", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 0100xxx111xxxxxx LR lea lea 0x003F 0x2E00

	SetTable(instbl, syntbl, "0100xxx111010xxx", LR lea, "lea", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0100xxx111101xxx", LR lea, "lea", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0100xxx111110xxx", LR lea, "lea", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "0100xxx111111000", LR lea, "lea", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0100xxx111111001", LR lea, "lea", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0100xxx111111011", LR lea, "lea", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "0100xxx111111100", LR lea, "lea", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 0100001000xxxxxx LR clr_b clr.b 0x003F 0xF000

	SetTable(instbl, syntbl, "0100001000000xxx", LR clr_b, "clr.b", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001000010xxx", LR clr_b, "clr.b", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001000011xxx", LR clr_b, "clr.b", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001000100xxx", LR clr_b, "clr.b", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001000101xxx", LR clr_b, "clr.b", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001000110xxx", LR clr_b, "clr.b", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001000111000", LR clr_b, "clr.b", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100001000111001", LR clr_b, "clr.b", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0100001001xxxxxx LR clr_w clr.w 0x003F 0xF000

	SetTable(instbl, syntbl, "0100001001000xxx", LR clr_w, "clr.w", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001001010xxx", LR clr_w, "clr.w", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001001011xxx", LR clr_w, "clr.w", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001001100xxx", LR clr_w, "clr.w", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001001101xxx", LR clr_w, "clr.w", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001001110xxx", LR clr_w, "clr.w", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001001111000", LR clr_w, "clr.w", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100001001111001", LR clr_w, "clr.w", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0100001010xxxxxx LR clr_l clr.l 0x003F 0xF000

	SetTable(instbl, syntbl, "0100001010000xxx", LR clr_l, "clr.l", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001010010xxx", LR clr_l, "clr.l", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001010011xxx", LR clr_l, "clr.l", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001010100xxx", LR clr_l, "clr.l", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001010101xxx", LR clr_l, "clr.l", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001010110xxx", LR clr_l, "clr.l", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100001010111000", LR clr_l, "clr.l", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100001010111001", LR clr_l, "clr.l", 0xB001, 0xF000 ); //srcXEA

//	---------------------------------------------------------------------------------------------
//	SetTable(instbl, syntbl, "0100001011xxxxxx", LR move_from_ccr,	"move",		0xF004,	2x003F );
//	---------------------------------------------------------------------------------------------



	// Generated From: 0100010000xxxxxx LR neg_b neg.b 0x003F 0xF000

	SetTable(instbl, syntbl, "0100010000000xxx", LR neg_b, "neg.b", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010000010xxx", LR neg_b, "neg.b", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010000011xxx", LR neg_b, "neg.b", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010000100xxx", LR neg_b, "neg.b", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010000101xxx", LR neg_b, "neg.b", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010000110xxx", LR neg_b, "neg.b", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010000111000", LR neg_b, "neg.b", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100010000111001", LR neg_b, "neg.b", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0100010001xxxxxx LR neg_w neg.w 0x003F 0xF000

	SetTable(instbl, syntbl, "0100010001000xxx", LR neg_w, "neg.w", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010001010xxx", LR neg_w, "neg.w", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010001011xxx", LR neg_w, "neg.w", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010001100xxx", LR neg_w, "neg.w", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010001101xxx", LR neg_w, "neg.w", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010001110xxx", LR neg_w, "neg.w", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010001111000", LR neg_w, "neg.w", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100010001111001", LR neg_w, "neg.w", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0100010010xxxxxx LR neg_l neg.l 0x003F 0xF000

	SetTable(instbl, syntbl, "0100010010000xxx", LR neg_l, "neg.l", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010010010xxx", LR neg_l, "neg.l", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010010011xxx", LR neg_l, "neg.l", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010010100xxx", LR neg_l, "neg.l", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010010101xxx", LR neg_l, "neg.l", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010010110xxx", LR neg_l, "neg.l", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100010010111000", LR neg_l, "neg.l", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100010010111001", LR neg_l, "neg.l", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0100010011xxxxxx LR move_to_ccr move 0x003F 0xF004

	SetTable(instbl, syntbl, "0100010011000xxx", LR move_to_ccr, "move", 0x2007, 0xF004 ); //srcEA
	SetTable(instbl, syntbl, "0100010011010xxx", LR move_to_ccr, "move", 0x4007, 0xF004 ); //srcEA
	SetTable(instbl, syntbl, "0100010011011xxx", LR move_to_ccr, "move", 0x6007, 0xF004 ); //srcEA
	SetTable(instbl, syntbl, "0100010011100xxx", LR move_to_ccr, "move", 0x7007, 0xF004 ); //srcEA
	SetTable(instbl, syntbl, "0100010011101xxx", LR move_to_ccr, "move", 0x5007, 0xF004 ); //srcEA
	SetTable(instbl, syntbl, "0100010011110xxx", LR move_to_ccr, "move", 0x5017, 0xF004 ); //srcEA
	SetTable(instbl, syntbl, "0100010011111000", LR move_to_ccr, "move", 0xB000, 0xF004 ); //srcXEA
	SetTable(instbl, syntbl, "0100010011111001", LR move_to_ccr, "move", 0xB001, 0xF004 ); //srcXEA
	SetTable(instbl, syntbl, "0100010011111010", LR move_to_ccr, "move", 0x8000, 0xF004 ); //srcXEA
	SetTable(instbl, syntbl, "0100010011111011", LR move_to_ccr, "move", 0x8010, 0xF004 ); //srcXEA
	SetTable(instbl, syntbl, "0100010011111100", LR move_to_ccr, "move", 0xA000, 0xF004 ); //srcXEA


	// Generated From: 0100011000xxxxxx LR not_b not.b 0x003F 0xF000

	SetTable(instbl, syntbl, "0100011000000xxx", LR not_b, "not.b", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011000010xxx", LR not_b, "not.b", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011000011xxx", LR not_b, "not.b", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011000100xxx", LR not_b, "not.b", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011000101xxx", LR not_b, "not.b", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011000110xxx", LR not_b, "not.b", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011000111000", LR not_b, "not.b", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100011000111001", LR not_b, "not.b", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0100011001xxxxxx LR not_w not.w 0x003F 0xF000

	SetTable(instbl, syntbl, "0100011001000xxx", LR not_w, "not.w", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011001010xxx", LR not_w, "not.w", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011001011xxx", LR not_w, "not.w", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011001100xxx", LR not_w, "not.w", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011001101xxx", LR not_w, "not.w", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011001110xxx", LR not_w, "not.w", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011001111000", LR not_w, "not.w", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100011001111001", LR not_w, "not.w", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0100011010xxxxxx LR not_l not.l 0x003F 0xF000

	SetTable(instbl, syntbl, "0100011010000xxx", LR not_l, "not.l", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011010010xxx", LR not_l, "not.l", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011010011xxx", LR not_l, "not.l", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011010100xxx", LR not_l, "not.l", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011010101xxx", LR not_l, "not.l", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011010110xxx", LR not_l, "not.l", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100011010111000", LR not_l, "not.l", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100011010111001", LR not_l, "not.l", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0100011011xxxxxx LR move_to_sr move 0x003F 0xF005

	SetTable(instbl, syntbl, "0100011011000xxx", LR move_to_sr, "move", 0x2007, 0xF005 ); //srcEA
	SetTable(instbl, syntbl, "0100011011010xxx", LR move_to_sr, "move", 0x4007, 0xF005 ); //srcEA
	SetTable(instbl, syntbl, "0100011011011xxx", LR move_to_sr, "move", 0x6007, 0xF005 ); //srcEA
	SetTable(instbl, syntbl, "0100011011100xxx", LR move_to_sr, "move", 0x7007, 0xF005 ); //srcEA
	SetTable(instbl, syntbl, "0100011011101xxx", LR move_to_sr, "move", 0x5007, 0xF005 ); //srcEA
	SetTable(instbl, syntbl, "0100011011110xxx", LR move_to_sr, "move", 0x5017, 0xF005 ); //srcEA
	SetTable(instbl, syntbl, "0100011011111000", LR move_to_sr, "move", 0xB000, 0xF005 ); //srcXEA
	SetTable(instbl, syntbl, "0100011011111001", LR move_to_sr, "move", 0xB001, 0xF005 ); //srcXEA
	SetTable(instbl, syntbl, "0100011011111010", LR move_to_sr, "move", 0x8000, 0xF005 ); //srcXEA
	SetTable(instbl, syntbl, "0100011011111011", LR move_to_sr, "move", 0x8010, 0xF005 ); //srcXEA
	SetTable(instbl, syntbl, "0100011011111100", LR move_to_sr, "move", 0xA000, 0xF005 ); //srcXEA


	// Generated From: 0100100000xxxxxx LR nbcd nbcd 0x003F 0xF000

	SetTable(instbl, syntbl, "0100100000000xxx", LR nbcd, "nbcd", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100100000010xxx", LR nbcd, "nbcd", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100100000011xxx", LR nbcd, "nbcd", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100100000100xxx", LR nbcd, "nbcd", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100100000101xxx", LR nbcd, "nbcd", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100100000110xxx", LR nbcd, "nbcd", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100100000111000", LR nbcd, "nbcd", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100100000111001", LR nbcd, "nbcd", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0100100001xxxxxx LR pea pea 0x003F 0xF000

	SetTable(instbl, syntbl, "0100100001010xxx", LR pea, "pea", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100100001101xxx", LR pea, "pea", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100100001110xxx", LR pea, "pea", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100100001111000", LR pea, "pea", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100100001111001", LR pea, "pea", 0xB001, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100100001111011", LR pea, "pea", 0x8010, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100100001111100", LR pea, "pea", 0xA000, 0xF000 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0100100001000xxx", LR swap, "swap", 0x2007, 0xF000 );   


	// Generated From: 0100100010xxxxxx LR movem_save_w movem.w 0xF900 0x003F

	SetTable(instbl, syntbl, "0100100010010xxx", LR movem_save_w, "movem.w", 0xF900, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0100100010100xxx", LR movem_save_w, "movem.w", 0xF900, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0100100010101xxx", LR movem_save_w, "movem.w", 0xF900, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0100100010110xxx", LR movem_save_w, "movem.w", 0xF900, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0100100010111000", LR movem_save_w, "movem.w", 0xF900, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0100100010111001", LR movem_save_w, "movem.w", 0xF900, 0xB001 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0100100010000xxx", LR ext_w, "ext.w", 0x2007, 0xF000 );   


	// Generated From: 0100100011xxxxxx LR movem_save_l movem.l 0xF900 0x003F

	SetTable(instbl, syntbl, "0100100011010xxx", LR movem_save_l, "movem.l", 0xF900, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0100100011100xxx", LR movem_save_l, "movem.l", 0xF900, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0100100011101xxx", LR movem_save_l, "movem.l", 0xF900, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0100100011110xxx", LR movem_save_l, "movem.l", 0xF900, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0100100011111000", LR movem_save_l, "movem.l", 0xF900, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0100100011111001", LR movem_save_l, "movem.l", 0xF900, 0xB001 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0100100011000xxx", LR ext_l, "ext.l", 0x2007, 0xF000 );   


	// Generated From: 0100101000xxxxxx LR tst_b tst.b 0x003F 0xF000

	SetTable(instbl, syntbl, "0100101000000xxx", LR tst_b, "tst.b", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101000010xxx", LR tst_b, "tst.b", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101000011xxx", LR tst_b, "tst.b", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101000100xxx", LR tst_b, "tst.b", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101000101xxx", LR tst_b, "tst.b", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101000110xxx", LR tst_b, "tst.b", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101000111000", LR tst_b, "tst.b", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100101000111001", LR tst_b, "tst.b", 0xB001, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100101000111011", LR tst_b, "tst.b", 0x8010, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100101000111100", LR tst_b, "tst.b", 0xA000, 0xF000 ); //srcXEA


	// Generated From: 0100101001xxxxxx LR tst_w tst.w 0x003F 0xF000

	SetTable(instbl, syntbl, "0100101001000xxx", LR tst_w, "tst.w", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101001010xxx", LR tst_w, "tst.w", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101001011xxx", LR tst_w, "tst.w", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101001100xxx", LR tst_w, "tst.w", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101001101xxx", LR tst_w, "tst.w", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101001110xxx", LR tst_w, "tst.w", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101001111000", LR tst_w, "tst.w", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100101001111001", LR tst_w, "tst.w", 0xB001, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100101001111011", LR tst_w, "tst.w", 0x8010, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100101001111100", LR tst_w, "tst.w", 0xA000, 0xF000 ); //srcXEA


	// Generated From: 0100101010xxxxxx LR tst_l tst.l 0x003F 0xF000

	SetTable(instbl, syntbl, "0100101010000xxx", LR tst_l, "tst.l", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101010010xxx", LR tst_l, "tst.l", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101010011xxx", LR tst_l, "tst.l", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101010100xxx", LR tst_l, "tst.l", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101010101xxx", LR tst_l, "tst.l", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101010110xxx", LR tst_l, "tst.l", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101010111000", LR tst_l, "tst.l", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100101010111001", LR tst_l, "tst.l", 0xB001, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100101010111011", LR tst_l, "tst.l", 0x8010, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100101010111100", LR tst_l, "tst.l", 0xA000, 0xF000 ); //srcXEA


	// Generated From: 0100101011xxxxxx LR tas tas 0x003F 0xF000

	SetTable(instbl, syntbl, "0100101011000xxx", LR tas, "tas", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101011010xxx", LR tas, "tas", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101011011xxx", LR tas, "tas", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101011100xxx", LR tas, "tas", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101011101xxx", LR tas, "tas", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101011110xxx", LR tas, "tas", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100101011111000", LR tas, "tas", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100101011111001", LR tas, "tas", 0xB001, 0xF000 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0100101011111100", LR illegal, "illegal", 0xF000, 0xF000 );   


	// Generated From: 0100110010xxxxxx LR movem_load_w movem.w 0x003F 0xF900

	SetTable(instbl, syntbl, "0100110010010xxx", LR movem_load_w, "movem.w", 0x4007, 0xF900 ); //srcEA
	SetTable(instbl, syntbl, "0100110010011xxx", LR movem_load_w, "movem.w", 0x6007, 0xF900 ); //srcEA
	SetTable(instbl, syntbl, "0100110010101xxx", LR movem_load_w, "movem.w", 0x5007, 0xF900 ); //srcEA
	SetTable(instbl, syntbl, "0100110010110xxx", LR movem_load_w, "movem.w", 0x5017, 0xF900 ); //srcEA
	SetTable(instbl, syntbl, "0100110010111000", LR movem_load_w, "movem.w", 0xB000, 0xF900 ); //srcXEA
	SetTable(instbl, syntbl, "0100110010111001", LR movem_load_w, "movem.w", 0xB001, 0xF900 ); //srcXEA
	SetTable(instbl, syntbl, "0100110010111011", LR movem_load_w, "movem.w", 0x8010, 0xF900 ); //srcXEA
	SetTable(instbl, syntbl, "0100110010111100", LR movem_load_w, "movem.w", 0xA000, 0xF900 ); //srcXEA


	// Generated From: 0100110011xxxxxx LR movem_load_l movem.l 0x003F 0xF900

	SetTable(instbl, syntbl, "0100110011010xxx", LR movem_load_l, "movem.l", 0x4007, 0xF900 ); //srcEA
	SetTable(instbl, syntbl, "0100110011011xxx", LR movem_load_l, "movem.l", 0x6007, 0xF900 ); //srcEA
	SetTable(instbl, syntbl, "0100110011101xxx", LR movem_load_l, "movem.l", 0x5007, 0xF900 ); //srcEA
	SetTable(instbl, syntbl, "0100110011110xxx", LR movem_load_l, "movem.l", 0x5017, 0xF900 ); //srcEA
	SetTable(instbl, syntbl, "0100110011111000", LR movem_load_l, "movem.l", 0xB000, 0xF900 ); //srcXEA
	SetTable(instbl, syntbl, "0100110011111001", LR movem_load_l, "movem.l", 0xB001, 0xF900 ); //srcXEA
	SetTable(instbl, syntbl, "0100110011111011", LR movem_load_l, "movem.l", 0x8010, 0xF900 ); //srcXEA
	SetTable(instbl, syntbl, "0100110011111100", LR movem_load_l, "movem.l", 0xA000, 0xF900 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "010011100100xxxx", LR trap, "trap", 0xA00F, 0xF000 );   

#ifndef IE_XL_II
	SetTable(instbl, syntbl, "0100111001000000", LR trap0, "trap#0", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "0100111001000001", LR trap1, "trap#1", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "0100111001000010", LR trap2, "trap#2", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "0100111001000011", LR trap3, "trap#3", 0xF000, 0xF000 );   
#endif

//	---------------------------------------------------------------------------------------------
//	SetTable(instbl, syntbl, "0100111001000100", LR trap4,			"trap#4",	0xF000,	0xF000 );
//	---------------------------------------------------------------------------------------------

	SetTable(instbl, syntbl, "0100111001010xxx", LR link, "link", 0x3007, 0xE200 );   
	SetTable(instbl, syntbl, "0100111001011xxx", LR unlk, "unlk", 0x3007, 0xF000 );   
	SetTable(instbl, syntbl, "0100111001100xxx", LR move_to_usp, "move", 0x3007, 0xF006 );   
	SetTable(instbl, syntbl, "0100111001101xxx", LR move_from_usp, "move", 0xF006, 0x3007 );   
	SetTable(instbl, syntbl, "0100111001110000", LR reset, "reset", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "0100111001110001", LR nop, "nop", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "0100111001110010", LR stop, "stop", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "0100111001110011", LR rte, "rte", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "0100111001110101", LR rts, "rts", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "0100111001110110", LR trapv, "trapv", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "0100111001110111", LR rtr, "rtr", 0xF000, 0xF000 );   


	// Generated From: 0100111010xxxxxx LR jsr jsr 0x003F 0xF000

	SetTable(instbl, syntbl, "0100111010010xxx", LR jsr, "jsr", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100111010101xxx", LR jsr, "jsr", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100111010110xxx", LR jsr, "jsr", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100111010111000", LR jsr, "jsr", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100111010111001", LR jsr, "jsr", 0xB001, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100111010111011", LR jsr, "jsr", 0x8010, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100111010111100", LR jsr, "jsr", 0xA000, 0xF000 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0100111010111010", LR jsr_displ, "jsr", 0x8000, 0xF000 );   


	// Generated From: 0100111011xxxxxx LR jmp jmp 0x003F 0xF000

	SetTable(instbl, syntbl, "0100111011010xxx", LR jmp, "jmp", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100111011101xxx", LR jmp, "jmp", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100111011110xxx", LR jmp, "jmp", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0100111011111000", LR jmp, "jmp", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100111011111001", LR jmp, "jmp", 0xB001, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100111011111011", LR jmp, "jmp", 0x8010, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0100111011111100", LR jmp, "jmp", 0xA000, 0xF000 ); //srcXEA


	// Generated From: 0101xxx000xxxxxx LR addq_b addq.b 0xAE00 0x003F

	SetTable(instbl, syntbl, "0101xxx000000xxx", LR addq_b, "addq.b", 0xAE00, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx000001xxx", LR addq_b, "addq.b", 0xAE00, 0x3007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx000010xxx", LR addq_b, "addq.b", 0xAE00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx000011xxx", LR addq_b, "addq.b", 0xAE00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx000100xxx", LR addq_b, "addq.b", 0xAE00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx000101xxx", LR addq_b, "addq.b", 0xAE00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx000110xxx", LR addq_b, "addq.b", 0xAE00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx000111000", LR addq_b, "addq.b", 0xAE00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0101xxx000111001", LR addq_b, "addq.b", 0xAE00, 0xB001 ); //srcXEA


	// Generated From: 0101xxx001xxxxxx LR addq_w addq.w 0xAE00 0x003F

	SetTable(instbl, syntbl, "0101xxx001000xxx", LR addq_w, "addq.w", 0xAE00, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx001010xxx", LR addq_w, "addq.w", 0xAE00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx001011xxx", LR addq_w, "addq.w", 0xAE00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx001100xxx", LR addq_w, "addq.w", 0xAE00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx001101xxx", LR addq_w, "addq.w", 0xAE00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx001110xxx", LR addq_w, "addq.w", 0xAE00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx001111000", LR addq_w, "addq.w", 0xAE00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0101xxx001111001", LR addq_w, "addq.w", 0xAE00, 0xB001 ); //srcXEA


	// Generated From: 0101xxx010xxxxxx LR addq_l addq.l 0xAE00 0x003F

	SetTable(instbl, syntbl, "0101xxx010000xxx", LR addq_l, "addq.l", 0xAE00, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx010010xxx", LR addq_l, "addq.l", 0xAE00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx010011xxx", LR addq_l, "addq.l", 0xAE00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx010100xxx", LR addq_l, "addq.l", 0xAE00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx010101xxx", LR addq_l, "addq.l", 0xAE00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx010110xxx", LR addq_l, "addq.l", 0xAE00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx010111000", LR addq_l, "addq.l", 0xAE00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0101xxx010111001", LR addq_l, "addq.l", 0xAE00, 0xB001 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0101xxx001001xxx", LR addq_an, "addq.w", 0xAE00, 0x3007 );   
	SetTable(instbl, syntbl, "0101xxx010001xxx", LR addq_an, "addq.l", 0xAE00, 0x3007 );   
	SetTable(instbl, syntbl, "0101100001001xxx", LR addq_4_an, "addq.w", 0xF003, 0x3007 );   
	SetTable(instbl, syntbl, "0101100010001xxx", LR addq_4_an, "addq.l", 0xF003, 0x3007 );   


	// Generated From: 0101xxxx11xxxxxx LR scc sCC 0x003F 0xF000

	SetTable(instbl, syntbl, "0101xxxx11000xxx", LR scc, "sCC", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101xxxx11010xxx", LR scc, "sCC", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101xxxx11011xxx", LR scc, "sCC", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101xxxx11100xxx", LR scc, "sCC", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101xxxx11101xxx", LR scc, "sCC", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101xxxx11110xxx", LR scc, "sCC", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101xxxx11111000", LR scc, "sCC", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101xxxx11111001", LR scc, "sCC", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101000011xxxxxx LR st st 0x003F 0xF000

	SetTable(instbl, syntbl, "0101000011000xxx", LR st, "st", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101000011010xxx", LR st, "st", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101000011011xxx", LR st, "st", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101000011100xxx", LR st, "st", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101000011101xxx", LR st, "st", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101000011110xxx", LR st, "st", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101000011111000", LR st, "st", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101000011111001", LR st, "st", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101000111xxxxxx LR sf sf 0x003F 0xF000

	SetTable(instbl, syntbl, "0101000111000xxx", LR sf, "sf", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101000111010xxx", LR sf, "sf", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101000111011xxx", LR sf, "sf", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101000111100xxx", LR sf, "sf", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101000111101xxx", LR sf, "sf", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101000111110xxx", LR sf, "sf", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101000111111000", LR sf, "sf", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101000111111001", LR sf, "sf", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101001011xxxxxx LR scc shi 0x003F 0xF000

	SetTable(instbl, syntbl, "0101001011000xxx", LR scc, "shi", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101001011010xxx", LR scc, "shi", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101001011011xxx", LR scc, "shi", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101001011100xxx", LR scc, "shi", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101001011101xxx", LR scc, "shi", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101001011110xxx", LR scc, "shi", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101001011111000", LR scc, "shi", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101001011111001", LR scc, "shi", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101001111xxxxxx LR scc sls 0x003F 0xF000

	SetTable(instbl, syntbl, "0101001111000xxx", LR scc, "sls", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101001111010xxx", LR scc, "sls", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101001111011xxx", LR scc, "sls", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101001111100xxx", LR scc, "sls", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101001111101xxx", LR scc, "sls", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101001111110xxx", LR scc, "sls", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101001111111000", LR scc, "sls", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101001111111001", LR scc, "sls", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101010011xxxxxx LR scc scc 0x003F 0xF000

	SetTable(instbl, syntbl, "0101010011000xxx", LR scc, "scc", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101010011010xxx", LR scc, "scc", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101010011011xxx", LR scc, "scc", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101010011100xxx", LR scc, "scc", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101010011101xxx", LR scc, "scc", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101010011110xxx", LR scc, "scc", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101010011111000", LR scc, "scc", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101010011111001", LR scc, "scc", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101010111xxxxxx LR scc scs 0x003F 0xF000

	SetTable(instbl, syntbl, "0101010111000xxx", LR scc, "scs", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101010111010xxx", LR scc, "scs", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101010111011xxx", LR scc, "scs", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101010111100xxx", LR scc, "scs", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101010111101xxx", LR scc, "scs", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101010111110xxx", LR scc, "scs", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101010111111000", LR scc, "scs", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101010111111001", LR scc, "scs", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101011011xxxxxx LR scc sne 0x003F 0xF000

	SetTable(instbl, syntbl, "0101011011000xxx", LR scc, "sne", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101011011010xxx", LR scc, "sne", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101011011011xxx", LR scc, "sne", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101011011100xxx", LR scc, "sne", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101011011101xxx", LR scc, "sne", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101011011110xxx", LR scc, "sne", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101011011111000", LR scc, "sne", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101011011111001", LR scc, "sne", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101011111xxxxxx LR scc seq 0x003F 0xF000

	SetTable(instbl, syntbl, "0101011111000xxx", LR scc, "seq", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101011111010xxx", LR scc, "seq", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101011111011xxx", LR scc, "seq", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101011111100xxx", LR scc, "seq", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101011111101xxx", LR scc, "seq", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101011111110xxx", LR scc, "seq", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101011111111000", LR scc, "seq", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101011111111001", LR scc, "seq", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101100011xxxxxx LR scc svc 0x003F 0xF000

	SetTable(instbl, syntbl, "0101100011000xxx", LR scc, "svc", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101100011010xxx", LR scc, "svc", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101100011011xxx", LR scc, "svc", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101100011100xxx", LR scc, "svc", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101100011101xxx", LR scc, "svc", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101100011110xxx", LR scc, "svc", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101100011111000", LR scc, "svc", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101100011111001", LR scc, "svc", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101100111xxxxxx LR scc svs 0x003F 0xF000

	SetTable(instbl, syntbl, "0101100111000xxx", LR scc, "svs", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101100111010xxx", LR scc, "svs", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101100111011xxx", LR scc, "svs", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101100111100xxx", LR scc, "svs", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101100111101xxx", LR scc, "svs", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101100111110xxx", LR scc, "svs", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101100111111000", LR scc, "svs", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101100111111001", LR scc, "svs", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101101011xxxxxx LR scc spl 0x003F 0xF000

	SetTable(instbl, syntbl, "0101101011000xxx", LR scc, "spl", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101101011010xxx", LR scc, "spl", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101101011011xxx", LR scc, "spl", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101101011100xxx", LR scc, "spl", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101101011101xxx", LR scc, "spl", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101101011110xxx", LR scc, "spl", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101101011111000", LR scc, "spl", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101101011111001", LR scc, "spl", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101101111xxxxxx LR scc smi 0x003F 0xF000

	SetTable(instbl, syntbl, "0101101111000xxx", LR scc, "smi", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101101111010xxx", LR scc, "smi", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101101111011xxx", LR scc, "smi", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101101111100xxx", LR scc, "smi", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101101111101xxx", LR scc, "smi", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101101111110xxx", LR scc, "smi", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101101111111000", LR scc, "smi", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101101111111001", LR scc, "smi", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101110011xxxxxx LR scc sge 0x003F 0xF000

	SetTable(instbl, syntbl, "0101110011000xxx", LR scc, "sge", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101110011010xxx", LR scc, "sge", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101110011011xxx", LR scc, "sge", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101110011100xxx", LR scc, "sge", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101110011101xxx", LR scc, "sge", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101110011110xxx", LR scc, "sge", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101110011111000", LR scc, "sge", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101110011111001", LR scc, "sge", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101110111xxxxxx LR scc slt 0x003F 0xF000

	SetTable(instbl, syntbl, "0101110111000xxx", LR scc, "slt", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101110111010xxx", LR scc, "slt", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101110111011xxx", LR scc, "slt", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101110111100xxx", LR scc, "slt", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101110111101xxx", LR scc, "slt", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101110111110xxx", LR scc, "slt", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101110111111000", LR scc, "slt", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101110111111001", LR scc, "slt", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101111011xxxxxx LR scc sgt 0x003F 0xF000

	SetTable(instbl, syntbl, "0101111011000xxx", LR scc, "sgt", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101111011010xxx", LR scc, "sgt", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101111011011xxx", LR scc, "sgt", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101111011100xxx", LR scc, "sgt", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101111011101xxx", LR scc, "sgt", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101111011110xxx", LR scc, "sgt", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101111011111000", LR scc, "sgt", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101111011111001", LR scc, "sgt", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 0101111111xxxxxx LR scc sle 0x003F 0xF000

	SetTable(instbl, syntbl, "0101111111000xxx", LR scc, "sle", 0x2007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101111111010xxx", LR scc, "sle", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101111111011xxx", LR scc, "sle", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101111111100xxx", LR scc, "sle", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101111111101xxx", LR scc, "sle", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101111111110xxx", LR scc, "sle", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "0101111111111000", LR scc, "sle", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "0101111111111001", LR scc, "sle", 0xB001, 0xF000 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0101xxxx11001xxx", LR dbcc, "dbcc", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101000011001xxx", LR dbcc, "dbt", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101000111001xxx", LR dbf, "dbf", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101001011001xxx", LR dbcc, "dbhi", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101001111001xxx", LR dbcc, "dbls", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101010011001xxx", LR dbcc, "dbcc", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101010111001xxx", LR dbcc, "dbcs", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101011011001xxx", LR dbcc, "dbne", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101011111001xxx", LR dbcc, "dbeq", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101100011001xxx", LR dbcc, "dbvc", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101100111001xxx", LR dbcc, "dbvs", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101101011001xxx", LR dbcc, "dbpl", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101101111001xxx", LR dbcc, "dbmi", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101110011001xxx", LR dbcc, "dbge", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101110111001xxx", LR dbcc, "dblt", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101111011001xxx", LR dbcc, "dbgt", 0x2007, 0xE200 );   
	SetTable(instbl, syntbl, "0101111111001xxx", LR dbcc, "dble", 0x2007, 0xE200 );   


	// Generated From: 0101xxx100xxxxxx LR subq_b subq.b 0xAE00 0x003F

	SetTable(instbl, syntbl, "0101xxx100000xxx", LR subq_b, "subq.b", 0xAE00, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx100001xxx", LR subq_b, "subq.b", 0xAE00, 0x3007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx100010xxx", LR subq_b, "subq.b", 0xAE00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx100011xxx", LR subq_b, "subq.b", 0xAE00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx100100xxx", LR subq_b, "subq.b", 0xAE00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx100101xxx", LR subq_b, "subq.b", 0xAE00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx100110xxx", LR subq_b, "subq.b", 0xAE00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx100111000", LR subq_b, "subq.b", 0xAE00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0101xxx100111001", LR subq_b, "subq.b", 0xAE00, 0xB001 ); //srcXEA


	// Generated From: 0101xxx101xxxxxx LR subq_w subq.w 0xAE00 0x003F

	SetTable(instbl, syntbl, "0101xxx101000xxx", LR subq_w, "subq.w", 0xAE00, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx101010xxx", LR subq_w, "subq.w", 0xAE00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx101011xxx", LR subq_w, "subq.w", 0xAE00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx101100xxx", LR subq_w, "subq.w", 0xAE00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx101101xxx", LR subq_w, "subq.w", 0xAE00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx101110xxx", LR subq_w, "subq.w", 0xAE00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx101111000", LR subq_w, "subq.w", 0xAE00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0101xxx101111001", LR subq_w, "subq.w", 0xAE00, 0xB001 ); //srcXEA


	// Generated From: 0101xxx110xxxxxx LR subq_l subq.l 0xAE00 0x003F

	SetTable(instbl, syntbl, "0101xxx110000xxx", LR subq_l, "subq.l", 0xAE00, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx110010xxx", LR subq_l, "subq.l", 0xAE00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx110011xxx", LR subq_l, "subq.l", 0xAE00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx110100xxx", LR subq_l, "subq.l", 0xAE00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx110101xxx", LR subq_l, "subq.l", 0xAE00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx110110xxx", LR subq_l, "subq.l", 0xAE00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "0101xxx110111000", LR subq_l, "subq.l", 0xAE00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "0101xxx110111001", LR subq_l, "subq.l", 0xAE00, 0xB001 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "0101xxx101001xxx", LR subq_an, "subq.w", 0xAE00, 0x3007 );   
	SetTable(instbl, syntbl, "0101xxx110001xxx", LR subq_an, "subq.l", 0xAE00, 0x3007 );   
	SetTable(instbl, syntbl, "0101100101001xxx", LR subq_4_an, "subq.w", 0xF003, 0x3007 );   
	SetTable(instbl, syntbl, "0101100110001xxx", LR subq_4_an, "subq.w", 0xF003, 0x3007 );   
	SetTable(instbl, syntbl, "0110xxxxxxxxxxx0", LR bcc_s, "bcc.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110xxxxxxxxxxx1", LR bcc_bad, "bcc_BAD", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "01100010xxxxxxx0", LR bcc_s, "bhi.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01100011xxxxxxx0", LR bcc_s, "bls.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01100100xxxxxxx0", LR bccc_s, "bccc.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01100101xxxxxxx0", LR bcs_s, "bcs.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01100110xxxxxxx0", LR bne_s, "bne.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01100111xxxxxxx0", LR beq_s, "beq.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01101000xxxxxxx0", LR bcc_s, "bvc.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01101001xxxxxxx0", LR bcc_s, "bvs.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01101010xxxxxxx0", LR bpl_s, "bpl.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01101011xxxxxxx0", LR bmi_s, "bmi.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01101100xxxxxxx0", LR bge_s, "bge.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01101101xxxxxxx0", LR blt_s, "blt.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01101110xxxxxxx0", LR bgt_s, "bgt.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01101111xxxxxxx0", LR ble_s, "ble.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110xxxx00000000", LR bcc_l, "bcc.l", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110001000000000", LR bcc_l, "bhi.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110001100000000", LR bcc_l, "bls.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110010000000000", LR bcc_l, "bccc.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110010100000000", LR bcc_l, "bcs.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110011000000000", LR bne_l, "bne.l", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110011100000000", LR beq_l, "beq.l", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110100000000000", LR bcc_l, "bvc.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110100100000000", LR bcc_l, "bvs.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110101000000000", LR bcc_l, "bpl.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110101100000000", LR bcc_l, "bmi.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110110000000000", LR bcc_l, "bge.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110110100000000", LR bcc_l, "blt.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110111000000000", LR bcc_l, "bgt.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110111100000000", LR bcc_l, "ble.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01100000xxxxxxx0", LR bra_s, "bra.s", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0110000000000000", LR bra_l, "bra.l", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "01100001xxxxxxxx", LR bsr, "bsr", 0x90FF, 0xF000 );   
	SetTable(instbl, syntbl, "0111xxx0xxxxxxxx", LR moveq, "moveq", 0xA0FF, 0x2E00 );   


	// Generated From: 1000xxx000xxxxxx LR or_b_dn or.b 0x003F 0x2E00

	SetTable(instbl, syntbl, "1000xxx000000xxx", LR or_b_dn, "or.b", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx000010xxx", LR or_b_dn, "or.b", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx000011xxx", LR or_b_dn, "or.b", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx000100xxx", LR or_b_dn, "or.b", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx000101xxx", LR or_b_dn, "or.b", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx000110xxx", LR or_b_dn, "or.b", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx000111000", LR or_b_dn, "or.b", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx000111001", LR or_b_dn, "or.b", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx000111010", LR or_b_dn, "or.b", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx000111011", LR or_b_dn, "or.b", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx000111100", LR or_b_dn, "or.b", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1000xxx001xxxxxx LR or_w_dn or.w 0x003F 0x2E00

	SetTable(instbl, syntbl, "1000xxx001000xxx", LR or_w_dn, "or.w", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx001010xxx", LR or_w_dn, "or.w", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx001011xxx", LR or_w_dn, "or.w", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx001100xxx", LR or_w_dn, "or.w", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx001101xxx", LR or_w_dn, "or.w", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx001110xxx", LR or_w_dn, "or.w", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx001111000", LR or_w_dn, "or.w", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx001111001", LR or_w_dn, "or.w", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx001111010", LR or_w_dn, "or.w", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx001111011", LR or_w_dn, "or.w", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx001111100", LR or_w_dn, "or.w", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1000xxx010xxxxxx LR or_l_dn or.l 0x003F 0x2E00

	SetTable(instbl, syntbl, "1000xxx010000xxx", LR or_l_dn, "or.l", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx010010xxx", LR or_l_dn, "or.l", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx010011xxx", LR or_l_dn, "or.l", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx010100xxx", LR or_l_dn, "or.l", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx010101xxx", LR or_l_dn, "or.l", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx010110xxx", LR or_l_dn, "or.l", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx010111000", LR or_l_dn, "or.l", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx010111001", LR or_l_dn, "or.l", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx010111010", LR or_l_dn, "or.l", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx010111011", LR or_l_dn, "or.l", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx010111100", LR or_l_dn, "or.l", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1000xxx100xxxxxx LR or_b_ea or.b 0x2E00 0x003F

	SetTable(instbl, syntbl, "1000xxx100010xxx", LR or_b_ea, "or.b", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx100011xxx", LR or_b_ea, "or.b", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx100100xxx", LR or_b_ea, "or.b", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx100101xxx", LR or_b_ea, "or.b", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx100110xxx", LR or_b_ea, "or.b", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx100111000", LR or_b_ea, "or.b", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx100111001", LR or_b_ea, "or.b", 0x2E00, 0xB001 ); //srcXEA


	// Generated From: 1000xxx101xxxxxx LR or_w_ea or.w 0x2E00 0x003F

	SetTable(instbl, syntbl, "1000xxx101010xxx", LR or_w_ea, "or.w", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx101011xxx", LR or_w_ea, "or.w", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx101100xxx", LR or_w_ea, "or.w", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx101101xxx", LR or_w_ea, "or.w", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx101110xxx", LR or_w_ea, "or.w", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx101111000", LR or_w_ea, "or.w", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx101111001", LR or_w_ea, "or.w", 0x2E00, 0xB001 ); //srcXEA


	// Generated From: 1000xxx110xxxxxx LR or_l_ea or.l 0x2E00 0x003F

	SetTable(instbl, syntbl, "1000xxx110010xxx", LR or_l_ea, "or.l", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx110011xxx", LR or_l_ea, "or.l", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx110100xxx", LR or_l_ea, "or.l", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx110101xxx", LR or_l_ea, "or.l", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx110110xxx", LR or_l_ea, "or.l", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx110111000", LR or_l_ea, "or.l", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx110111001", LR or_l_ea, "or.l", 0x2E00, 0xB001 ); //srcXEA


	// Generated From: 1000xxx011xxxxxx LR divu divu.w 0x003F 0x2E00

	SetTable(instbl, syntbl, "1000xxx011000xxx", LR divu, "divu.w", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx011010xxx", LR divu, "divu.w", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx011011xxx", LR divu, "divu.w", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx011100xxx", LR divu, "divu.w", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx011101xxx", LR divu, "divu.w", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx011110xxx", LR divu, "divu.w", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx011111000", LR divu, "divu.w", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx011111001", LR divu, "divu.w", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx011111010", LR divu, "divu.w", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx011111011", LR divu, "divu.w", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx011111100", LR divu, "divu.w", 0xA000, 0x2E00 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "1000xxx100000xxx", LR sbcd, "sbcd", 0x2007, 0x2E00 );   
	SetTable(instbl, syntbl, "1000xxx100001xxx", LR sbcd, "sbcd", 0x7007, 0x7E00 );   


	// Generated From: 1000xxx111xxxxxx LR divs divs.w 0x003F 0x2E00

	SetTable(instbl, syntbl, "1000xxx111000xxx", LR divs, "divs.w", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx111010xxx", LR divs, "divs.w", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx111011xxx", LR divs, "divs.w", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx111100xxx", LR divs, "divs.w", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx111101xxx", LR divs, "divs.w", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx111110xxx", LR divs, "divs.w", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1000xxx111111000", LR divs, "divs.w", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx111111001", LR divs, "divs.w", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx111111010", LR divs, "divs.w", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx111111011", LR divs, "divs.w", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1000xxx111111100", LR divs, "divs.w", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1001xxx000xxxxxx LR sub_b_dn sub.b 0x003F 0x2E00

	SetTable(instbl, syntbl, "1001xxx000000xxx", LR sub_b_dn, "sub.b", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx000001xxx", LR sub_b_dn, "sub.b", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx000010xxx", LR sub_b_dn, "sub.b", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx000011xxx", LR sub_b_dn, "sub.b", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx000100xxx", LR sub_b_dn, "sub.b", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx000101xxx", LR sub_b_dn, "sub.b", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx000110xxx", LR sub_b_dn, "sub.b", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx000111000", LR sub_b_dn, "sub.b", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx000111001", LR sub_b_dn, "sub.b", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx000111010", LR sub_b_dn, "sub.b", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx000111011", LR sub_b_dn, "sub.b", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx000111100", LR sub_b_dn, "sub.b", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1001xxx001xxxxxx LR sub_w_dn sub.w 0x003F 0x2E00

	SetTable(instbl, syntbl, "1001xxx001000xxx", LR sub_w_dn, "sub.w", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx001001xxx", LR sub_w_dn, "sub.w", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx001010xxx", LR sub_w_dn, "sub.w", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx001011xxx", LR sub_w_dn, "sub.w", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx001100xxx", LR sub_w_dn, "sub.w", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx001101xxx", LR sub_w_dn, "sub.w", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx001110xxx", LR sub_w_dn, "sub.w", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx001111000", LR sub_w_dn, "sub.w", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx001111001", LR sub_w_dn, "sub.w", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx001111010", LR sub_w_dn, "sub.w", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx001111011", LR sub_w_dn, "sub.w", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx001111100", LR sub_w_dn, "sub.w", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1001xxx010xxxxxx LR sub_l_dn sub.l 0x003F 0x2E00

	SetTable(instbl, syntbl, "1001xxx010000xxx", LR sub_l_dn, "sub.l", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx010001xxx", LR sub_l_dn, "sub.l", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx010010xxx", LR sub_l_dn, "sub.l", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx010011xxx", LR sub_l_dn, "sub.l", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx010100xxx", LR sub_l_dn, "sub.l", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx010101xxx", LR sub_l_dn, "sub.l", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx010110xxx", LR sub_l_dn, "sub.l", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx010111000", LR sub_l_dn, "sub.l", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx010111001", LR sub_l_dn, "sub.l", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx010111010", LR sub_l_dn, "sub.l", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx010111011", LR sub_l_dn, "sub.l", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx010111100", LR sub_l_dn, "sub.l", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1001xxx100xxxxxx LR sub_b_ea sub.b 0x2E00 0x003F

	SetTable(instbl, syntbl, "1001xxx100010xxx", LR sub_b_ea, "sub.b", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx100011xxx", LR sub_b_ea, "sub.b", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx100100xxx", LR sub_b_ea, "sub.b", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx100101xxx", LR sub_b_ea, "sub.b", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx100110xxx", LR sub_b_ea, "sub.b", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx100111000", LR sub_b_ea, "sub.b", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx100111001", LR sub_b_ea, "sub.b", 0x2E00, 0xB001 ); //srcXEA


	// Generated From: 1001xxx101xxxxxx LR sub_w_ea sub.w 0x2E00 0x003F

	SetTable(instbl, syntbl, "1001xxx101010xxx", LR sub_w_ea, "sub.w", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx101011xxx", LR sub_w_ea, "sub.w", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx101100xxx", LR sub_w_ea, "sub.w", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx101101xxx", LR sub_w_ea, "sub.w", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx101110xxx", LR sub_w_ea, "sub.w", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx101111000", LR sub_w_ea, "sub.w", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx101111001", LR sub_w_ea, "sub.w", 0x2E00, 0xB001 ); //srcXEA


	// Generated From: 1001xxx110xxxxxx LR sub_l_ea sub.l 0x2E00 0x003F

	SetTable(instbl, syntbl, "1001xxx110010xxx", LR sub_l_ea, "sub.l", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx110011xxx", LR sub_l_ea, "sub.l", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx110100xxx", LR sub_l_ea, "sub.l", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx110101xxx", LR sub_l_ea, "sub.l", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx110110xxx", LR sub_l_ea, "sub.l", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx110111000", LR sub_l_ea, "sub.l", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx110111001", LR sub_l_ea, "sub.l", 0x2E00, 0xB001 ); //srcXEA


	// Generated From: 1001xxx011xxxxxx LR sub_w_an suba.w 0x003F 0x2E00

	SetTable(instbl, syntbl, "1001xxx011000xxx", LR sub_w_an, "suba.w", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx011001xxx", LR sub_w_an, "suba.w", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx011010xxx", LR sub_w_an, "suba.w", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx011011xxx", LR sub_w_an, "suba.w", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx011100xxx", LR sub_w_an, "suba.w", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx011101xxx", LR sub_w_an, "suba.w", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx011110xxx", LR sub_w_an, "suba.w", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx011111000", LR sub_w_an, "suba.w", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx011111001", LR sub_w_an, "suba.w", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx011111010", LR sub_w_an, "suba.w", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx011111011", LR sub_w_an, "suba.w", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx011111100", LR sub_w_an, "suba.w", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1001xxx111xxxxxx LR sub_l_an suba.l 0x003F 0x2E00

	SetTable(instbl, syntbl, "1001xxx111000xxx", LR sub_l_an, "suba.l", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx111001xxx", LR sub_l_an, "suba.l", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx111010xxx", LR sub_l_an, "suba.l", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx111011xxx", LR sub_l_an, "suba.l", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx111100xxx", LR sub_l_an, "suba.l", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx111101xxx", LR sub_l_an, "suba.l", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx111110xxx", LR sub_l_an, "suba.l", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1001xxx111111000", LR sub_l_an, "suba.l", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx111111001", LR sub_l_an, "suba.l", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx111111010", LR sub_l_an, "suba.l", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx111111011", LR sub_l_an, "suba.l", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1001xxx111111100", LR sub_l_an, "suba.l", 0xA000, 0x2E00 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "1001xxx100000xxx", LR subx_b_r, "subx.b", 0x2007, 0x2E00 );   
	SetTable(instbl, syntbl, "1001xxx101000xxx", LR subx_w_r, "subx.w", 0x2007, 0x2E00 );   
	SetTable(instbl, syntbl, "1001xxx110000xxx", LR subx_l_r, "subx.l", 0x2007, 0x2E00 );   
	SetTable(instbl, syntbl, "1001xxx100001xxx", LR subx_b_m, "subx.b", 0x7007, 0x7E00 );   
	SetTable(instbl, syntbl, "1001xxx101001xxx", LR subx_w_m, "subx.w", 0x7007, 0x7E00 );   
	SetTable(instbl, syntbl, "1001xxx110001xxx", LR subx_l_m, "subx.l", 0x7007, 0x7E00 );   


	// Generated From: 1011xxx000xxxxxx LR cmp_b cmp.b 0x003F 0x2E00

	SetTable(instbl, syntbl, "1011xxx000001xxx", LR cmp_b, "cmp.b", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx000010xxx", LR cmp_b, "cmp.b", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx000011xxx", LR cmp_b, "cmp.b", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx000100xxx", LR cmp_b, "cmp.b", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx000110xxx", LR cmp_b, "cmp.b", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx000111000", LR cmp_b, "cmp.b", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx000111001", LR cmp_b, "cmp.b", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx000111010", LR cmp_b, "cmp.b", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx000111011", LR cmp_b, "cmp.b", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx000111100", LR cmp_b, "cmp.b", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1011xxx001xxxxxx LR cmp_w cmp.w 0x003F 0x2E00

	SetTable(instbl, syntbl, "1011xxx001001xxx", LR cmp_w, "cmp.w", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx001010xxx", LR cmp_w, "cmp.w", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx001011xxx", LR cmp_w, "cmp.w", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx001100xxx", LR cmp_w, "cmp.w", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx001101xxx", LR cmp_w, "cmp.w", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx001110xxx", LR cmp_w, "cmp.w", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx001111000", LR cmp_w, "cmp.w", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx001111001", LR cmp_w, "cmp.w", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx001111010", LR cmp_w, "cmp.w", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx001111011", LR cmp_w, "cmp.w", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx001111100", LR cmp_w, "cmp.w", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1011xxx010xxxxxx LR cmp_l cmp.l 0x003F 0x2E00

	SetTable(instbl, syntbl, "1011xxx010001xxx", LR cmp_l, "cmp.l", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx010010xxx", LR cmp_l, "cmp.l", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx010011xxx", LR cmp_l, "cmp.l", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx010100xxx", LR cmp_l, "cmp.l", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx010101xxx", LR cmp_l, "cmp.l", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx010110xxx", LR cmp_l, "cmp.l", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx010111000", LR cmp_l, "cmp.l", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx010111001", LR cmp_l, "cmp.l", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx010111010", LR cmp_l, "cmp.l", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx010111011", LR cmp_l, "cmp.l", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx010111100", LR cmp_l, "cmp.l", 0xA000, 0x2E00 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "1011xxx000000xxx", LR cmp_b_dn, "cmp.b", 0x2007, 0x2E00 );   
	SetTable(instbl, syntbl, "1011xxx000101xxx", LR cmp_b_dan, "cmp.b", 0x5007, 0x2E00 );   
	SetTable(instbl, syntbl, "1011xxx001000xxx", LR cmp_w_dn, "cmp.w", 0x2007, 0x2E00 );   
	SetTable(instbl, syntbl, "1011xxx010000xxx", LR cmp_l_dn, "cmp.l", 0x2007, 0x2E00 );   


	// Generated From: 1011xxx011xxxxxx LR cmpa_w cmpa.w 0x003F 0x3E00

	SetTable(instbl, syntbl, "1011xxx011000xxx", LR cmpa_w, "cmpa.w", 0x2007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx011001xxx", LR cmpa_w, "cmpa.w", 0x3007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx011010xxx", LR cmpa_w, "cmpa.w", 0x4007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx011011xxx", LR cmpa_w, "cmpa.w", 0x6007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx011100xxx", LR cmpa_w, "cmpa.w", 0x7007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx011101xxx", LR cmpa_w, "cmpa.w", 0x5007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx011110xxx", LR cmpa_w, "cmpa.w", 0x5017, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx011111000", LR cmpa_w, "cmpa.w", 0xB000, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx011111001", LR cmpa_w, "cmpa.w", 0xB001, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx011111010", LR cmpa_w, "cmpa.w", 0x8000, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx011111011", LR cmpa_w, "cmpa.w", 0x8010, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx011111100", LR cmpa_w, "cmpa.w", 0xA000, 0x3E00 ); //srcXEA


	// Generated From: 1011xxx111xxxxxx LR cmpa_l cmpa.l 0x003F 0x3E00

	SetTable(instbl, syntbl, "1011xxx111000xxx", LR cmpa_l, "cmpa.l", 0x2007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx111010xxx", LR cmpa_l, "cmpa.l", 0x4007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx111011xxx", LR cmpa_l, "cmpa.l", 0x6007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx111100xxx", LR cmpa_l, "cmpa.l", 0x7007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx111101xxx", LR cmpa_l, "cmpa.l", 0x5007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx111110xxx", LR cmpa_l, "cmpa.l", 0x5017, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx111111000", LR cmpa_l, "cmpa.l", 0xB000, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx111111001", LR cmpa_l, "cmpa.l", 0xB001, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx111111010", LR cmpa_l, "cmpa.l", 0x8000, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx111111011", LR cmpa_l, "cmpa.l", 0x8010, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx111111100", LR cmpa_l, "cmpa.l", 0xA000, 0x3E00 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "1011xxx111001xxx", LR cmpa_l_an, "cmpa.l", 0x3007, 0x3E00 );   


	// Generated From: 1011xxx100xxxxxx LR eor_b eor.b 0x2E00 0x003F

	SetTable(instbl, syntbl, "1011xxx100000xxx", LR eor_b, "eor.b", 0x2E00, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx100010xxx", LR eor_b, "eor.b", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx100011xxx", LR eor_b, "eor.b", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx100100xxx", LR eor_b, "eor.b", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx100101xxx", LR eor_b, "eor.b", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx100110xxx", LR eor_b, "eor.b", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx100111000", LR eor_b, "eor.b", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx100111001", LR eor_b, "eor.b", 0x2E00, 0xB001 ); //srcXEA


	// Generated From: 1011xxx101xxxxxx LR eor_w eor.w 0x2E00 0x003F

	SetTable(instbl, syntbl, "1011xxx101000xxx", LR eor_w, "eor.w", 0x2E00, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx101010xxx", LR eor_w, "eor.w", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx101011xxx", LR eor_w, "eor.w", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx101100xxx", LR eor_w, "eor.w", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx101101xxx", LR eor_w, "eor.w", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx101110xxx", LR eor_w, "eor.w", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx101111000", LR eor_w, "eor.w", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx101111001", LR eor_w, "eor.w", 0x2E00, 0xB001 ); //srcXEA


	// Generated From: 1011xxx110xxxxxx LR eor_l eor.l 0x2E00 0x003F

	SetTable(instbl, syntbl, "1011xxx110000xxx", LR eor_l, "eor.l", 0x2E00, 0x2007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx110010xxx", LR eor_l, "eor.l", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx110011xxx", LR eor_l, "eor.l", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx110100xxx", LR eor_l, "eor.l", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx110101xxx", LR eor_l, "eor.l", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx110110xxx", LR eor_l, "eor.l", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1011xxx110111000", LR eor_l, "eor.l", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1011xxx110111001", LR eor_l, "eor.l", 0x2E00, 0xB001 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "1011xxx100001xxx", LR cmpm_b, "cmpm.b", 0x6007, 0x6E00 );   
	SetTable(instbl, syntbl, "1011xxx101001xxx", LR cmpm_w, "cmpm.w", 0x6007, 0x6E00 );   
	SetTable(instbl, syntbl, "1011xxx110001xxx", LR cmpm_l, "cmpm.l", 0x6007, 0x6E00 );   


	// Generated From: 1100xxx000xxxxxx LR and_b_dn and.b 0x003F 0x2E00

	SetTable(instbl, syntbl, "1100xxx000000xxx", LR and_b_dn, "and.b", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx000010xxx", LR and_b_dn, "and.b", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx000011xxx", LR and_b_dn, "and.b", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx000100xxx", LR and_b_dn, "and.b", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx000101xxx", LR and_b_dn, "and.b", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx000110xxx", LR and_b_dn, "and.b", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx000111000", LR and_b_dn, "and.b", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx000111001", LR and_b_dn, "and.b", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx000111010", LR and_b_dn, "and.b", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx000111011", LR and_b_dn, "and.b", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx000111100", LR and_b_dn, "and.b", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1100xxx001xxxxxx LR and_w_dn and.w 0x003F 0x2E00

	SetTable(instbl, syntbl, "1100xxx001000xxx", LR and_w_dn, "and.w", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx001010xxx", LR and_w_dn, "and.w", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx001011xxx", LR and_w_dn, "and.w", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx001100xxx", LR and_w_dn, "and.w", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx001101xxx", LR and_w_dn, "and.w", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx001110xxx", LR and_w_dn, "and.w", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx001111000", LR and_w_dn, "and.w", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx001111001", LR and_w_dn, "and.w", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx001111010", LR and_w_dn, "and.w", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx001111011", LR and_w_dn, "and.w", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx001111100", LR and_w_dn, "and.w", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1100xxx010xxxxxx LR and_l_dn and.l 0x003F 0x2E00

	SetTable(instbl, syntbl, "1100xxx010010xxx", LR and_l_dn, "and.l", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx010011xxx", LR and_l_dn, "and.l", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx010100xxx", LR and_l_dn, "and.l", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx010101xxx", LR and_l_dn, "and.l", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx010110xxx", LR and_l_dn, "and.l", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx010111000", LR and_l_dn, "and.l", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx010111001", LR and_l_dn, "and.l", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx010111010", LR and_l_dn, "and.l", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx010111011", LR and_l_dn, "and.l", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx010111100", LR and_l_dn, "and.l", 0xA000, 0x2E00 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "1100xxx010000xxx", LR and_l_dn_dn, "and.l", 0x2007, 0x2E00 );   


	// Generated From: 1100xxx100xxxxxx LR and_b_ea and.b 0x2E00 0x003F

	SetTable(instbl, syntbl, "1100xxx100010xxx", LR and_b_ea, "and.b", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx100011xxx", LR and_b_ea, "and.b", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx100100xxx", LR and_b_ea, "and.b", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx100101xxx", LR and_b_ea, "and.b", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx100110xxx", LR and_b_ea, "and.b", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx100111000", LR and_b_ea, "and.b", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx100111001", LR and_b_ea, "and.b", 0x2E00, 0xB001 ); //srcXEA


	// Generated From: 1100xxx101xxxxxx LR and_w_ea and.w 0x2E00 0x003F

	SetTable(instbl, syntbl, "1100xxx101010xxx", LR and_w_ea, "and.w", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx101011xxx", LR and_w_ea, "and.w", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx101100xxx", LR and_w_ea, "and.w", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx101101xxx", LR and_w_ea, "and.w", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx101110xxx", LR and_w_ea, "and.w", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx101111000", LR and_w_ea, "and.w", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx101111001", LR and_w_ea, "and.w", 0x2E00, 0xB001 ); //srcXEA


	// Generated From: 1100xxx110xxxxxx LR and_l_ea and.l 0x2E00 0x003F

	SetTable(instbl, syntbl, "1100xxx110010xxx", LR and_l_ea, "and.l", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx110011xxx", LR and_l_ea, "and.l", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx110100xxx", LR and_l_ea, "and.l", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx110101xxx", LR and_l_ea, "and.l", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx110110xxx", LR and_l_ea, "and.l", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx110111000", LR and_l_ea, "and.l", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx110111001", LR and_l_ea, "and.l", 0x2E00, 0xB001 ); //srcXEA


	// Generated From: 1100xxx011xxxxxx LR mulu mulu.w 0x003F 0x2E00

	SetTable(instbl, syntbl, "1100xxx011000xxx", LR mulu, "mulu.w", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx011010xxx", LR mulu, "mulu.w", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx011011xxx", LR mulu, "mulu.w", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx011100xxx", LR mulu, "mulu.w", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx011101xxx", LR mulu, "mulu.w", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx011110xxx", LR mulu, "mulu.w", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx011111000", LR mulu, "mulu.w", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx011111001", LR mulu, "mulu.w", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx011111010", LR mulu, "mulu.w", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx011111011", LR mulu, "mulu.w", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx011111100", LR mulu, "mulu.w", 0xA000, 0x2E00 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "1100xxx100000xxx", LR abcd, "abcd", 0x2007, 0x2E00 );   
	SetTable(instbl, syntbl, "1100xxx10000xxxx", LR abcd, "abcd", 0x7007, 0x7E00 );   
	SetTable(instbl, syntbl, "1100xxx101000xxx", LR exg_d, "exg_d", 0x2007, 0x2E00 );   
	SetTable(instbl, syntbl, "1100xxx101001xxx", LR exg_a, "exg_a", 0x3007, 0x3E00 );   
	SetTable(instbl, syntbl, "1100xxx110001xxx", LR exg_ad, "exg_ad", 0x3007, 0x2E00 );   


	// Generated From: 1100xxx111xxxxxx LR muls muls.w 0x003F 0x2E00

	SetTable(instbl, syntbl, "1100xxx111000xxx", LR muls, "muls.w", 0x2007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx111010xxx", LR muls, "muls.w", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx111011xxx", LR muls, "muls.w", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx111100xxx", LR muls, "muls.w", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx111101xxx", LR muls, "muls.w", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx111110xxx", LR muls, "muls.w", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1100xxx111111000", LR muls, "muls.w", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx111111001", LR muls, "muls.w", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx111111010", LR muls, "muls.w", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx111111011", LR muls, "muls.w", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1100xxx111111100", LR muls, "muls.w", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1101xxx000xxxxxx LR add_b_dn add.b 0x003F 0x2E00

	SetTable(instbl, syntbl, "1101xxx000001xxx", LR add_b_dn, "add.b", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx000010xxx", LR add_b_dn, "add.b", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx000011xxx", LR add_b_dn, "add.b", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx000100xxx", LR add_b_dn, "add.b", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx000101xxx", LR add_b_dn, "add.b", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx000110xxx", LR add_b_dn, "add.b", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx000111000", LR add_b_dn, "add.b", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx000111001", LR add_b_dn, "add.b", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx000111010", LR add_b_dn, "add.b", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx000111011", LR add_b_dn, "add.b", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx000111100", LR add_b_dn, "add.b", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1101xxx001xxxxxx LR add_w_dn add.w 0x003F 0x2E00

	SetTable(instbl, syntbl, "1101xxx001001xxx", LR add_w_dn, "add.w", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx001010xxx", LR add_w_dn, "add.w", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx001011xxx", LR add_w_dn, "add.w", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx001100xxx", LR add_w_dn, "add.w", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx001101xxx", LR add_w_dn, "add.w", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx001110xxx", LR add_w_dn, "add.w", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx001111000", LR add_w_dn, "add.w", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx001111001", LR add_w_dn, "add.w", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx001111010", LR add_w_dn, "add.w", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx001111011", LR add_w_dn, "add.w", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx001111100", LR add_w_dn, "add.w", 0xA000, 0x2E00 ); //srcXEA


	// Generated From: 1101xxx010xxxxxx LR add_l_dn add.l 0x003F 0x2E00

	SetTable(instbl, syntbl, "1101xxx010001xxx", LR add_l_dn, "add.l", 0x3007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx010010xxx", LR add_l_dn, "add.l", 0x4007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx010011xxx", LR add_l_dn, "add.l", 0x6007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx010100xxx", LR add_l_dn, "add.l", 0x7007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx010101xxx", LR add_l_dn, "add.l", 0x5007, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx010110xxx", LR add_l_dn, "add.l", 0x5017, 0x2E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx010111000", LR add_l_dn, "add.l", 0xB000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx010111001", LR add_l_dn, "add.l", 0xB001, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx010111010", LR add_l_dn, "add.l", 0x8000, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx010111011", LR add_l_dn, "add.l", 0x8010, 0x2E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx010111100", LR add_l_dn, "add.l", 0xA000, 0x2E00 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "1101xxx000000xxx", LR add_b_dn_dn, "add.b", 0x2007, 0x2E00 );   
	SetTable(instbl, syntbl, "1101xxx001000xxx", LR add_w_dn_dn, "add.w", 0x2007, 0x2E00 );   
	SetTable(instbl, syntbl, "1101xxx010000xxx", LR add_l_dn_dn, "add.l", 0x2007, 0x2E00 );   


	// Generated From: 1101xxx100xxxxxx LR add_b_ea add.b 0x2E00 0x003F

	SetTable(instbl, syntbl, "1101xxx100010xxx", LR add_b_ea, "add.b", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx100011xxx", LR add_b_ea, "add.b", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx100100xxx", LR add_b_ea, "add.b", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx100101xxx", LR add_b_ea, "add.b", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx100110xxx", LR add_b_ea, "add.b", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx100111000", LR add_b_ea, "add.b", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx100111001", LR add_b_ea, "add.b", 0x2E00, 0xB001 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx100111010", LR add_b_ea, "add.b", 0x2E00, 0x8000 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx100111011", LR add_b_ea, "add.b", 0x2E00, 0x8010 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx100111100", LR add_b_ea, "add.b", 0x2E00, 0xA000 ); //srcXEA


	// Generated From: 1101xxx101xxxxxx LR add_w_ea add.w 0x2E00 0x003F

	SetTable(instbl, syntbl, "1101xxx101010xxx", LR add_w_ea, "add.w", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx101011xxx", LR add_w_ea, "add.w", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx101100xxx", LR add_w_ea, "add.w", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx101101xxx", LR add_w_ea, "add.w", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx101110xxx", LR add_w_ea, "add.w", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx101111000", LR add_w_ea, "add.w", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx101111001", LR add_w_ea, "add.w", 0x2E00, 0xB001 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx101111010", LR add_w_ea, "add.w", 0x2E00, 0x8000 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx101111011", LR add_w_ea, "add.w", 0x2E00, 0x8010 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx101111100", LR add_w_ea, "add.w", 0x2E00, 0xA000 ); //srcXEA


	// Generated From: 1101xxx110xxxxxx LR add_l_ea add.l 0x2E00 0x003F

	SetTable(instbl, syntbl, "1101xxx110010xxx", LR add_l_ea, "add.l", 0x2E00, 0x4007 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx110011xxx", LR add_l_ea, "add.l", 0x2E00, 0x6007 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx110100xxx", LR add_l_ea, "add.l", 0x2E00, 0x7007 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx110101xxx", LR add_l_ea, "add.l", 0x2E00, 0x5007 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx110110xxx", LR add_l_ea, "add.l", 0x2E00, 0x5017 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx110111000", LR add_l_ea, "add.l", 0x2E00, 0xB000 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx110111001", LR add_l_ea, "add.l", 0x2E00, 0xB001 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx110111010", LR add_l_ea, "add.l", 0x2E00, 0x8000 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx110111011", LR add_l_ea, "add.l", 0x2E00, 0x8010 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx110111100", LR add_l_ea, "add.l", 0x2E00, 0xA000 ); //srcXEA


	// Generated From: 1101xxx011xxxxxx LR add_w_an adda.w 0x003F 0x3E00

	SetTable(instbl, syntbl, "1101xxx011001xxx", LR add_w_an, "adda.w", 0x3007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx011010xxx", LR add_w_an, "adda.w", 0x4007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx011011xxx", LR add_w_an, "adda.w", 0x6007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx011100xxx", LR add_w_an, "adda.w", 0x7007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx011101xxx", LR add_w_an, "adda.w", 0x5007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx011110xxx", LR add_w_an, "adda.w", 0x5017, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx011111000", LR add_w_an, "adda.w", 0xB000, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx011111001", LR add_w_an, "adda.w", 0xB001, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx011111010", LR add_w_an, "adda.w", 0x8000, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx011111011", LR add_w_an, "adda.w", 0x8010, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx011111100", LR add_w_an, "adda.w", 0xA000, 0x3E00 ); //srcXEA


	// Generated From: 1101xxx111xxxxxx LR add_l_an adda.l 0x003F 0x3E00

	SetTable(instbl, syntbl, "1101xxx111001xxx", LR add_l_an, "adda.l", 0x3007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx111010xxx", LR add_l_an, "adda.l", 0x4007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx111011xxx", LR add_l_an, "adda.l", 0x6007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx111100xxx", LR add_l_an, "adda.l", 0x7007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx111101xxx", LR add_l_an, "adda.l", 0x5007, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx111110xxx", LR add_l_an, "adda.l", 0x5017, 0x3E00 ); //srcEA
	SetTable(instbl, syntbl, "1101xxx111111000", LR add_l_an, "adda.l", 0xB000, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx111111001", LR add_l_an, "adda.l", 0xB001, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx111111010", LR add_l_an, "adda.l", 0x8000, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx111111011", LR add_l_an, "adda.l", 0x8010, 0x3E00 ); //srcXEA
	SetTable(instbl, syntbl, "1101xxx111111100", LR add_l_an, "adda.l", 0xA000, 0x3E00 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "1101xxx011000xxx", LR add_w_an_dn, "adda.w", 0x2007, 0x3E00 );   
	SetTable(instbl, syntbl, "1101xxx111000xxx", LR add_l_an_dn, "adda.l", 0x2007, 0x3E00 );   
	SetTable(instbl, syntbl, "1101xxx100000xxx", LR addx_b_r, "addx.b", 0x2007, 0x2E00 );   
	SetTable(instbl, syntbl, "1101xxx101000xxx", LR addx_w_r, "addx.w", 0x2007, 0x2E00 );   
	SetTable(instbl, syntbl, "1101xxx110000xxx", LR addx_l_r, "addx.l", 0x2007, 0x2E00 );   
	SetTable(instbl, syntbl, "1101xxx100001xxx", LR addx_b_m, "addx.b", 0x7007, 0x7E00 );   
	SetTable(instbl, syntbl, "1101xxx101001xxx", LR addx_w_m, "addx.w", 0x7007, 0x7E00 );   
	SetTable(instbl, syntbl, "1101xxx110001xxx", LR addx_l_m, "addx.l", 0x7007, 0x7E00 );   
	SetTable(instbl, syntbl, "1110xxx000001xxx", LR lsr_b_i, "lsr.b", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx100001xxx", LR lsl_b_i, "lsl.b", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110001000001xxx", LR lsr1_b, "lsr.b", 0xF001, 0x2007 );   
	SetTable(instbl, syntbl, "1110001100001xxx", LR lsl1_b, "lsl.b", 0xF001, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx001001xxx", LR lsr_w_i, "lsr.w", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx101001xxx", LR lsl_w_i, "lsl.w", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110001001001xxx", LR lsr1_w, "lsr.w", 0xF001, 0x2007 );   
	SetTable(instbl, syntbl, "1110001101001xxx", LR lsl1_w, "lsl.w", 0xF001, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx010001xxx", LR lsr_l_i, "lsr.l", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx110001xxx", LR lsl_l_i, "lsl.l", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110001010001xxx", LR lsr1_l, "lsr.l", 0xF001, 0x2007 );   
	SetTable(instbl, syntbl, "1110001110001xxx", LR lsl1_l, "lsl.l", 0xF001, 0x2007 );   
	SetTable(instbl, syntbl, "1110010110001xxx", LR lsl2_l, "lsl.l", 0xF002, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx000101xxx", LR lsr_b_r, "lsr.b", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx100101xxx", LR lsl_b_r, "lsl.b", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx001101xxx", LR lsr_w_r, "lsr.w", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx101101xxx", LR lsl_w_r, "lsl.w", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx010101xxx", LR lsr_l_r, "lsr.l", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx110101xxx", LR lsl_l_r, "lsl.l", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx000000xxx", LR asr_b_i, "asr.b", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx100000xxx", LR asl_b_i, "asl.b", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx001000xxx", LR asr_w_i, "asr.w", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx101000xxx", LR asl_w_i, "asl.w", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx010000xxx", LR asr_l_i, "asr.l", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx110000xxx", LR asl_l_i, "asl.l", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx000100xxx", LR asr_b_r, "asr.b", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx100100xxx", LR asl_b_r, "asl.b", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx001100xxx", LR asr_w_r, "asr.w", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx101100xxx", LR asl_w_r, "asl.w", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx010100xxx", LR asr_l_r, "asr.l", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx110100xxx", LR asl_l_r, "asl.l", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx000010xxx", LR roxr_b_i, "roxr.b", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx100010xxx", LR roxl_b_i, "roxl.b", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx001010xxx", LR roxr_w_i, "roxr.w", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx101010xxx", LR roxl_w_i, "roxl.w", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx010010xxx", LR roxr_l_i, "roxr.l", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx110010xxx", LR roxl_l_i, "roxl.l", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx000110xxx", LR roxr_b_r, "roxr.b", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx100110xxx", LR roxl_b_r, "roxl.b", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx001110xxx", LR roxr_w_r, "roxr.w", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx101110xxx", LR roxl_w_r, "roxl.w", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx010110xxx", LR roxr_l_r, "roxr.l", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx110110xxx", LR roxl_l_r, "roxl.l", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx000011xxx", LR ror_b_i, "ror.b", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx100011xxx", LR rol_b_i, "rol.b", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx001011xxx", LR ror_w_i, "ror.w", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx101011xxx", LR rol_w_i, "rol.w", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx010011xxx", LR ror_l_i, "ror.l", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx110011xxx", LR rol_l_i, "rol.l", 0xAE00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx000111xxx", LR ror_b_r, "ror.b", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx100111xxx", LR rol_b_r, "rol.b", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx001111xxx", LR ror_w_r, "ror.w", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx101111xxx", LR rol_w_r, "rol.w", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx010111xxx", LR ror_l_r, "ror.l", 0x2E00, 0x2007 );   
	SetTable(instbl, syntbl, "1110xxx110111xxx", LR rol_l_r, "rol.l", 0x2E00, 0x2007 );   


	// Generated From: 1110000011xxxxxx LR asr_m asr 0x003F 0xF000

	SetTable(instbl, syntbl, "1110000011010xxx", LR asr_m, "asr", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110000011011xxx", LR asr_m, "asr", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110000011100xxx", LR asr_m, "asr", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110000011101xxx", LR asr_m, "asr", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110000011110xxx", LR asr_m, "asr", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110000011111000", LR asr_m, "asr", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "1110000011111001", LR asr_m, "asr", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 1110000111xxxxxx LR asl_m asl 0x003F 0xF000

	SetTable(instbl, syntbl, "1110000111010xxx", LR asl_m, "asl", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110000111011xxx", LR asl_m, "asl", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110000111100xxx", LR asl_m, "asl", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110000111101xxx", LR asl_m, "asl", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110000111110xxx", LR asl_m, "asl", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110000111111000", LR asl_m, "asl", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "1110000111111001", LR asl_m, "asl", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 1110001011xxxxxx LR lsr_m lsr 0x003F 0xF000

	SetTable(instbl, syntbl, "1110001011010xxx", LR lsr_m, "lsr", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110001011011xxx", LR lsr_m, "lsr", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110001011100xxx", LR lsr_m, "lsr", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110001011101xxx", LR lsr_m, "lsr", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110001011110xxx", LR lsr_m, "lsr", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110001011111000", LR lsr_m, "lsr", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "1110001011111001", LR lsr_m, "lsr", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 1110001111xxxxxx LR lsl_m lsl 0x003F 0xF000

	SetTable(instbl, syntbl, "1110001111010xxx", LR lsl_m, "lsl", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110001111011xxx", LR lsl_m, "lsl", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110001111100xxx", LR lsl_m, "lsl", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110001111101xxx", LR lsl_m, "lsl", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110001111110xxx", LR lsl_m, "lsl", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110001111111000", LR lsl_m, "lsl", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "1110001111111001", LR lsl_m, "lsl", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 1110010011xxxxxx LR roxr_m roxr 0x003F 0xF000

	SetTable(instbl, syntbl, "1110010011010xxx", LR roxr_m, "roxr", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110010011011xxx", LR roxr_m, "roxr", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110010011100xxx", LR roxr_m, "roxr", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110010011101xxx", LR roxr_m, "roxr", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110010011110xxx", LR roxr_m, "roxr", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110010011111000", LR roxr_m, "roxr", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "1110010011111001", LR roxr_m, "roxr", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 1110010111xxxxxx LR roxl_m roxl 0x003F 0xF000

	SetTable(instbl, syntbl, "1110010111010xxx", LR roxl_m, "roxl", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110010111011xxx", LR roxl_m, "roxl", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110010111100xxx", LR roxl_m, "roxl", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110010111101xxx", LR roxl_m, "roxl", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110010111110xxx", LR roxl_m, "roxl", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110010111111000", LR roxl_m, "roxl", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "1110010111111001", LR roxl_m, "roxl", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 1110011011xxxxxx LR ror_m ror 0x003F 0xF000

	SetTable(instbl, syntbl, "1110011011010xxx", LR ror_m, "ror", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110011011011xxx", LR ror_m, "ror", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110011011100xxx", LR ror_m, "ror", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110011011101xxx", LR ror_m, "ror", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110011011110xxx", LR ror_m, "ror", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110011011111000", LR ror_m, "ror", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "1110011011111001", LR ror_m, "ror", 0xB001, 0xF000 ); //srcXEA


	// Generated From: 1110011111xxxxxx LR rol_m rol 0x003F 0xF000

	SetTable(instbl, syntbl, "1110011111010xxx", LR rol_m, "rol", 0x4007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110011111011xxx", LR rol_m, "rol", 0x6007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110011111100xxx", LR rol_m, "rol", 0x7007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110011111101xxx", LR rol_m, "rol", 0x5007, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110011111110xxx", LR rol_m, "rol", 0x5017, 0xF000 ); //srcEA
	SetTable(instbl, syntbl, "1110011111111000", LR rol_m, "rol", 0xB000, 0xF000 ); //srcXEA
	SetTable(instbl, syntbl, "1110011111111001", LR rol_m, "rol", 0xB001, 0xF000 ); //srcXEA


	// Not Generated - Direct Copy

	SetTable(instbl, syntbl, "1010xxxxxxxxxxxx", LR code1010, "a_line", 0xAFFF, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010100000", LR InitROM, "ROMINI", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010100001", LR MdvIO, "MDVIO", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010100010", LR MdvOpen, "MDVO", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010100011", LR MdvClose, "MDVC", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010100100", LR MdvSlaving, "MDVSL", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010100101", LR MdvFormat, "MDVFO", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010100110", LR SerIO, "OSERIO", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010100111", LR SerOpen, "OSERO", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010101000", LR SerClose, "OSERC", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010101001", LR SchedulerCmd, "SCHED", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010101010", LR DrvIO, "DEVIO", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010101011", LR DrvOpen, "DEVO", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010101100", LR DrvClose, "DEVC", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010101101", LR KbdCmd, "MIPC", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010101110", LR PollCmd, "POLL", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010101111", LR KBencCmd, "KBENC", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010110000", LR BASEXTCmd, "BASXT", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010110001", LR devpefio_cmd, "DPF", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010101010110010", LR devpefo_cmd, "DPEFO", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010110111000000", LR UseIPC, "IPC", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010110111000001", LR ReadIPC, "IPCR", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010110111000010", LR WriteIPC, "IPCW", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010110111000111", LR FastStartup, "FSTART", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1010110111001000", LR QL_KeyTrans, "KYTRNS", 0xF000, 0xF000 );   
	SetTable(instbl, syntbl, "1111xxxxxxxxxxxx", LR code1111, "f_line", 0xAFFF, 0xF000 );   
	
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
