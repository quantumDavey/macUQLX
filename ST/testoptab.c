#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef USEGOTO
	#include "stubs.h"
	#include "optab.h"
#endif


#define IMPL "/Library/Application Support/MacQL"
#define PATH_MAX 256
#define V4 1

void TestTable(void)
{
	unsigned int	i;

	#ifdef USEGOTO
	#endif

	for(i=0; i<65536; i++)
	{
		#ifndef USEGOTO
			printf("%5ld, %8s:\t",i, optable[i].m);
			(optable[i].f)();
		#else
			#warning USEGOTO is defined
			goto run_it;

			#include "stubs.h"
			#include "optab.h"

			run_it :

			printf("%5ld, %8s:\t",i, optable[i].m);
			goto (optable[i].f);

			next :
			printf("next\t");
		#endif
	}

}

char *MnemPtr( char *theString )
{
	char	*p;
	int		thecount, i;

	for( p=OpMnem, thecount=0;*(++p) != '\0';thecount++ )
		if (!strcmp( theString, p)) return (p); else for(;*++p != '\0';);

	return(OpMnem);
}

long SetOpcode( OT *otbl, char *mask, int funcIndex, char *mnem, int smask, int dmask )
{
   	short		i,c1,c2;
	long		c,m,m0,Ents;
   	OT			*p;
	void		(*f)(void);

   	Cond		multiple=false;

	mnem=MnemPtr(mnem);
	f=OpX[funcIndex];
	
	if(V4) printf(" %8s %16s %04x\t",mnem,mask,f);
	
   	m=0x08000; c=0; Ents=0;
	
   	for(i=0;i<16;i++) { if(mask[i]=='1') c+=m; if(mask[i]=='x') multiple=true; m>>=1; }
	
   	if(!multiple) { otbl[c].f=f; otbl[c].m=mnem; otbl[c].s=smask; otbl[c].d=dmask; otbl[c].x=0; return(1); }
	
   	m0=1; i=15; c1=1;
	
   	while(mask[i]!='x') { m0<<=1; i--; }
	
   	while(mask[i--]=='x') c1<<=1;
	
   	while(i&&mask[i]!='x') i--;
	
   	if(i)
   	{
		c2=2; m=0x08000>>i;

		while(mask[--i]=='x') c2<<=1;
		
	   	while(c2--)
		{
			p=otbl+c; i=c1;
			
			while(i--) { p->f=f; p->m=mnem; p->s=smask; p->d=dmask; p->x=0; p+=m0; Ents++;}
			
			c+=m;
		}
   	}
   	else
   	{
		p=otbl+c;
		
		while(c1--) { p->f=f; p->m=mnem; p->s=smask; p->d=dmask; p->x=0; p+=m0; Ents++;}
	}
	return(Ents);
}

void SetOpsInvalid( OT *otbl, int funcIdx, char *mnem)
{
	long i;
	
	printf("Setting all opcodes in table to:  0x%x,%s,0x%x,0x%x ...", OpX[funcIdx],MnemPtr(mnem),0xF000,0xF000); 

	for(i=0;i<65536;i++)
	{
		otbl[i].f=OpX[funcIdx]; otbl[i].m=MnemPtr("Invalid"); otbl[i].s=0xF000; otbl[i].d=0xF000; otbl[i].x=0;
	}

	printf("DONE\n\n");
}

int ReadInOptab(void)
{
	FILE	*fp;

	char	buf[PATH_MAX+10], Mask[17], Name[15], Mnem[8], Cat[3];
	char 	*p;
	int		Idx, ArgL, ArgR, ScanRet, RecNo, FnLen, NewLen, NmLen;
	long	OpsSet, EntriesSet, TotalEntries, ValidEntries, InvalidEntries;
	Cond	inComments=false;
	
	char *entries[2]= { "Entry", "Entries" };

	strncpy(buf,IMPL,PATH_MAX); 
	p=buf+strlen(buf)-1;
	if (*p!='/') strncat(buf,"/",PATH_MAX);
	strncat(buf,"optab.dat",PATH_MAX+10);
	
    if(!(fp=fopen(buf,"r"))) { printf("Cannot open %s.\nMake sure it is readable.\n",buf); return(1); }

	printf("Opened instruction set data file OK\n\n");

	SetOpsInvalid( optable, 0, "Invalid");

	printf("Loading instruction table from %s...\n",buf);
	
	RecNo=1; NmLen=FnLen=0; OpsSet=0; InvalidEntries=ValidEntries=TotalEntries=EntriesSet=0;

	while(!feof(fp))
	{
		ScanRet=fscanf(fp,"%2s,",Cat);

		if ( *Cat != '/')
		{
			ScanRet=fscanf(fp,"%16s,%d,%[^,],%[^,],%x,%x\n",Mask,&Idx,Name,Mnem,&ArgL,&ArgR);
			if (ScanRet < 6) { printf("Format error at record %d, reached field %d. Stopping.\n",RecNo,ScanRet); return(1); }
			if (ScanRet > 6) { printf("Format error at record %d, too many fields %d. Stopping.\n",RecNo,ScanRet); return(1); }
		}
		else
		{
			if (!fgets(buf, 70, fp)) { printf("Format error at record %d, reading comment line.\nStopping.\n",RecNo); return(1);}
			if (!(p=strchr(buf,'\n'))) { printf("Format error at record %d Can't find newline.\nLine probably too long.\nStopping.\n",RecNo); return(1);}
			*p='\0';
			if( strlen(buf) > 56 ){ buf[57]='\0'; buf[54]=buf[55]=buf[56]='.'; }
		}

		if ( (*Cat == '/') != inComments) { printf("\n"); inComments=!inComments; }

		if( inComments ) { printf("%04d:[ %2s %56-s ]\t\tComment Line - Ignoring\n",RecNo++,Cat,buf); continue; }
		else printf("%04u:[ %s %s %03u %16-s %8-s %04x %04x ]\t", RecNo++, Cat, Mask, Idx, Name, Mnem, ArgL, ArgR );

		FnLen=((NewLen=strlen(Name))>FnLen?NewLen:FnLen);
		NmLen=((NewLen=strlen(Mnem))>NmLen?NewLen:NmLen);

		TotalEntries+=EntriesSet=SetOpcode(optable,Mask,Idx,Mnem,ArgL,ArgR);
		ValidEntries+=(Mnem[0]!='I'&&Mnem[1]!='n'&&Mnem[2]!='v')?EntriesSet:0;
		InvalidEntries+=(Mnem[0]=='I'&&Mnem[1]=='n'&&Mnem[2]=='v')?EntriesSet:0;
		OpsSet++;
		printf("%5lu %s\n",EntriesSet, EntriesSet==1?entries[0]:entries[1] );
	}
	printf("\n\nNum Valid Entries set: %ld\nNum of \"Invalid\"s set: %ld\n",ValidEntries,InvalidEntries);
	printf("Descreet  Opcodes set: %d\nLeft  Default Invalid:%ld\n", TotalEntries, 65536-TotalEntries);
	printf("Longest Function Name: %d\nLongest Mnemonic: %d\n", FnLen, NmLen);

	printf("\n\nClosing data file...");
	close(fp);
	printf("DONE\n");
}


int main(int argc, char **argv)
{

	if(!ReadInOptab()) { printf("Failed to load table.\n\n"); exit(-1); }

	TestTable();

}
