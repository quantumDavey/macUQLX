#include <stdio.h>
#include <stdlib.h>

static char OpCode[]="\0Invalid\0a_line\0abcd\0add.b\0add.l\0add.w\0adda.l\0adda.w\0addi.b\0addi.l\0addi.w\0addq.b\0addq.l\0addq.w\0addx.b\0addx.l\0addx.w\0and.b\0and.l\0and.w\0andi\0andi.b\0andi.l\0andi.w\0asl\0asl.b\0asl.l\0asl.w\0asr\0asr.b\0asr.l\0asr.w\0bcc.l\0bcc.s\0bcc_BAD\0bccc.s\0bchg\0bchg.s\0bclr\0bclr.s\0bcs.s\0beq.l\0beq.s\0bge.s\0bgt.s\0bhi.s\0ble.s\0bls.s\0blt.s\0bmi.s\0bne.l\0bne.s\0bpl.s\0bra.l\0bra.s\0bset\0bset.s\0bsr\0btst\0btst.s\0bvc.s\0bvs.s\0chk.w\0clr.b\0clr.l\0clr.w\0cmp.b\0cmp.l\0cmp.w\0cmpa.l\0cmpa.w\0cmpi.b\0cmpi.l\0cmpi.w\0cmpm.b\0cmpm.l\0cmpm.w\0dbcc\0dbcs\0dbeq\0dbf\0dbge\0dbgt\0dbhi\0dble\0dbls\0dblt\0dbmi\0dbne\0dbpl\0dbt\0dbvc\0dbvs\0divs.w\0divu.w\0eor.b\0eor.l\0eor.w\0eori\0eori.b\0eori.l\0eori.w\0exg_a\0exg_ad\0exg_d\0ext.l\0ext.w\0f_line\0illegal\0jmp\0jsr\0lea\0link\0lsl\0lsl.b\0lsl.l\0lsl.w\0lsr\0lsr.b\0lsr.l\0lsr.w\0move\0move.b\0move.l\0move.w\0movea.l\0movea.w\0movem.l\0movem.w\0movep.l\0movep.w\0moveq\0muls.w\0mulu.w\0nbcd\0neg.b\0neg.l\0neg.w\0negx.b\0negx.l\0negx.w\0nop\0not.b\0not.l\0not.w\0or.b\0or.l\0or.w\0ori\0ori.b\0ori.l\0ori.w\0pea\0reset\0rol\0rol.b\0rol.l\0rol.w\0ror\0ror.b\0ror.l\0ror.w\0roxl\0roxl.b\0roxl.l\0roxl.w\0roxr\0roxr.b\0roxr.l\0roxr.w\0rte\0rtr\0rts\0sCC\0sbcd\0scc\0scs\0seq\0sf\0sge\0sgt\0shi\0sle\0sls\0slt\0smi\0sne\0spl\0st\0stop\0sub.b\0sub.l\0sub.w\0subi.b\0subi.l\0subi.w\0subq.b\0subq.l\0subq.w\0subx.b\0subx.l\0subx.w\0svc\0svs\0swap\0tas\0trap\0trap#0\0trap#1\0trap#2\0trap#3\0trap#4\0trapv\0tst.b\0tst.l\0tst.w\0unlk\0\0";

char *sparsefind( char *theString )
{
	char	*p;
	int		thecount;
	int 	i;

	printf("Looking for %s\n", theString);
	for( p=OpCode, thecount=0;*(++p) != '\0';thecount++ )
	{
		printf(" Checking item %3d at %08x: %7s\r", thecount, p, p );
		for (i=0; i < 1024; i++) printf(" \r");
		if (!strcmp( theString, p)) return (p);
		for(;*++p != '\0';);
	}
	return(OpCode);

}

int main(int argc, char **argv)
{
	char *q;
	char input[255];

	for(;;)
	{
		printf("\nEnter Mnemonic >");
	
		gets(input);

		if( !strcmp(input, "finish" )) exit(0);

		q=sparsefind(input);
		if (*q) printf("\n\nFound %s at %08x\n", q, q); else printf("\n\nNot found\n");
	}
}
