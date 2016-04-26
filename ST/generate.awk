function checkDUP(CheckMask,FF,MN)
{
	OUTCount++;
	if(OMASK != CodeMask) { MaskHDR=sprintf("@%04d %s ---> ",RecsRead,CodeMask); printf("\n");} else { MaskHDR=""; }
	printf("%28-s%s %14s %7s:\t",MaskHDR,CheckMask,FF,MN);
	OMASK=CodeMask;
	for(j=RecsRead+1;j<=ACount;j++)
	{
		split(ARX[j],CLine)
		if(CheckMask==CLine[3]){	printf("NOT Writing %04d,  In Source Later @%04d: %s %14-s\n",OUTCount--,j,CLine[3],CLine[5]);
									system("sleep 1");
									return 0; }
	}
	printf("OK, Writing %04d\n", OUTCount);
	return 1;			
}

function condWRITE(cwVLD,cwCount,cwMask,cwMarker,cwComment)
{
	if(substr(ValidEA[cwVLD],cwCount,1)==1 && checkDUP(cwMask,FuncName,Mnem))
	{
		printf ("\t%s, %s, \"%s\", %s %s, \"%s\", %s, %s %s %s\n", $1,$2,NewMask,$4,$5,$6,Source,Dest,$9,cwComment)>>SRCOut;
		printf ("%s,%s,%03d,%s,%s,%s,%s\n",cwMarker,cwMask,F,$5,Mnem,Source,Dest)>>DATFile;
	}
}

BEGIN {		Mask[0]="000"; RMode[0]="0x2007"; LMode[0]="0x2E00"; Lit[0]="Dn"
			Mask[1]="001"; RMode[1]="0x3007"; LMode[1]="0x3E00"; Lit[1]="An"
			Mask[2]="010"; RMode[2]="0x4007"; LMode[2]="0x4E00"; Lit[2]="(An)"
			Mask[3]="011"; RMode[3]="0x6007"; LMode[3]="0x6E00"; Lit[3]="(An)+"
			Mask[4]="100"; RMode[4]="0x7007"; LMode[4]="0x7E00"; Lit[4]="-(An)"
			Mask[5]="101"; RMode[5]="0x5007"; LMode[5]="0x5E00"; Lit[5]="d16(An)"
			Mask[6]="110"; RMode[6]="0x5017"; LMode[6]="0x5E10"; Lit[6]="d8(An,Xn.s)"
			Mask[7]="111";	XMask[0]="000"; XMode[0]="0xB000";	XLit[0]="xxxx.W"
							XMask[1]="001"; XMode[1]="0xB001";	XLit[1]="xxxx.L"
							XMask[2]="010"; XMode[2]="0x8000";	XLit[2]="d16(PC)"
							XMask[3]="011"; XMode[3]="0x8010";	XLit[3]="d8(PC,Xn.s)"
							XMask[4]="100"; XMode[4]="0xA000";	XLit[4]="#<data>.s"

			ValidEA[0]="11111111111"; ValidEA[1]="00111111111"; ValidEA[2]="10111111000";
			ValidEA[3]="11111111000"; ValidEA[4]="10111111111"; ValidEA[5]="00111111000";
			ValidEA[6]="10111111011"; ValidEA[7]="00100111011"; ValidEA[8]="10011111000";
			ValidEA[9]="00101111000"; ValidEA["A"]="00110111011";

			SRCOut="Init3.c";
			DATFile="optab.dat";
			HDRFile="optab.h";
			FNCFile="funcdefs.h";
			STBFile="stubs.h";
			GTDFile="gtdefs.h";

			GENFROM="	// Generated From:";
			NGEN=0;

			GENLIT="Generated: "DATE;
			
			printf("// Opcode Table Data File\n// %s\n// DST 2008\n",GENLIT) >DATFile;
			printf("// Header file for Instruction Table setup\n// %s\n// DST 2008\n\n",GENLIT) >HDRFile;
			printf("// Header file for Instruction Table function definitions\n// %s\n// DST 2008\n\n",GENLIT) >FNCFile;
			printf("// Generated from Init.c\n// %s\n\n",GENLIT) >SRCOut;
			printf("// Stub file to replace %s during testing\n// %s\n\n",FNCFile,GENLIT)>STBFile;

			printf("*** Processing SetTable calls in source file '%s' to expand EAs ***\n",ARGV[1]);

			FuncCount=0;
			MnemList="\\0";
			OUTCount=1;
			OMASK="xxxxxxxxxxxxxxxx";

			ACount=0;
			printf("Caching Records...");
			while( "cat "ARGV[1] | getline ARX[++ACount] ){	gsub(",","",ARX[ACount]);
															gsub("\"","",ARX[ACount]);	}

			printf("DONE\nThere are %d source entries to process.\n", ACount);
			RecsRead=0;
			}


/^[	 ]*SetTable/{	gsub(",","",$0);
					gsub("\"","",$0);

					FuncName=$5;
					CodeMask=$3; 
					Mnem=$6; 

					if( !index(MnemList,"\\0"Mnem"\\0") ){ MnemList=MnemList""Mnem"\\0"; }
					
					if ( FuncID[FuncName]==""){ F=FuncCount;
												FuncNM[FuncName]=Mnem;
												FuncID[FuncName]=FuncCount++; }

					CSRC=substr($7,3,4); CDST=substr($8,3,4);
					SMODE="0x"CSRC; DMODE="0x"CDST;
					SVLD=substr($7,1,1); DVLD=substr($8,1,1);
					++RecsRead;
				}
	
!/^[ 	]*SetTable/{	print>>SRCOut; 
						printf("@%04d Write Non Call (%04d): %s\n",++RecsRead,++OUTCount,$0);
						next;
 }
/[0-9A]x003F/&&!/[0-9A]x1FC0/ {		#	RIGHT-HAND Only EAs

			NGEN=0
			printf ("\n\n%s %s %s %s %s %s %s\n\n",GENFROM,$3,$4,$5,$6,SMODE,DMODE)>>SRCOut;
			printf ("//,%s,%03d,%s,%s,%s,%s\n",CodeMask,F,$5,Mnem,SMODE,DMODE)>>DATFile;

			Op=substr(CodeMask,1,10);

			# Do Righthand Register EAs

			for(Count=0;Count<7;Count++)
			{
				if(CDST == "003F")	{ Source=SMODE; Dest=RMode[Count]; VLD=DVLD; }
							else		{ Source=RMode[Count]; Dest=DMODE; VLD=SVLD; } 

				NewMask=sprintf("%s%sxxx",Op,Mask[Count]);

				condWRITE(VLD,Count+1,NewMask,"AB","//srcEA");

				#if(substr(ValidEA[VLD],Count+1,1)==1 && CheckDUP(NewMask,FuncName,Mnem))
				#{
				#	printf ("	%s, %s, \"%s\", %s %s, \"%s\", %s, %s %s //srcEA\n",
				#			$1,$2,NewMask,$4,$5,$6,Source,Dest,$9)>>SRCOut;

				#	printf ("AB,%s%sxxx,%03d,%s,%s,%s,%s\n",
				#			Op,Mask[Count],F,$5,Mnem,Source,Dest)>>DATFile;
				#}
			}

			# Do Righthand Non Register EAs

			for(XCount=0;XCount<5;XCount++)
			{
				if(CDST == "003F")	{ Source=SMODE; Dest=XMode[XCount]; VLD=DVLD; }
							else		{ Source=XMode[XCount]; Dest=DMODE; VLD=SVLD; }

				NewMask=sprintf("%s111%s",Op,XMask[XCount]);

				condWRITE(VLD,XCount+7,NewMask,"AC","//srcXEA");

				#if(substr(ValidEA[VLD],XCount+7,1)==1)
				#{
				#	printf ("	%s, %s, \"%s111%s\", %s %s, \"%s\", %s, %s %s //srcXEA\n",
				#				$1,$2,Op,XMask[XCount],$4,$5,$6,Source,Dest,$9)>>SRCOut;

				#	printf ("AC,%s111%s,%03d,%s,%s,%s,%s\n",
				#			Op,XMask[XCount],F,$5,Mnem,Source,Dest)>>DATFile;
				#}
			}
		 }


/[0-9A]x1FC0/&&!/[0-9A]x003F/ {			# LEFT-HAND Only EAs
 
			NGEN=0;
			printf ("\n\n%s %s %s %s %s %s %s\n\n",GENFROM,$3,$4,$5,$6,SMODE,DMODE)>>SRCOut;
			printf ("//,%s,%03d,%s,%s,%s,%s\n",CodeMask,F,$5,Mnem,SMODE,DMODE)>>DATFile;

			Op=substr(CodeMask,1,4); Rest=substr(CodeMask,11,16);

			# Do Lefthand Register EAs

			for(Count=0;Count<7;Count++)
			{
				if(CDST == "1FC0")	{ Source=SMODE; Dest=LMode[Count]; VLD=DVLD; }
							else		{ Source=LMode[Count]; Dest=DMODE; VLD=SVLD; }

				NewMask=sprintf("%sxxx%s%s",Op,Mask[Count],Rest);

				condWRITE(VLD,Count+1,NewMask,"BB","//destEA");

				#if(substr(ValidEA[VLD],Count+1,1)==1)
				#{
				#	printf ("	%s, %s, \"%sxxx%s%s\", %s %s, \"%s\", %s, %s %s	//destEA\n",
				#			$1,$2,Op,Mask[Count],Rest,$4,$5,$6,Source,Dest,$9)>>SRCOut;

				#	printf ("BB,%sxxx%s%s,%03d,%s,%s,%s,%s\n",
				#			Op,Mask[Count],Rest,F,$5,Mnem,Source,Dest)>>DATFile; 
				#}
			}

			# Do Lefthand Non Register EAs

			for(XCount=0;XCount<5;XCount++)
			{
				if(CDST == "1FC0")	{ Source=SMODE; Dest=XMode[XCount]; VLD=DVLD; }
							else		{ Source=XMode[XCount]; Dest=DMODE; VLD=SVLD; }

				NewMask=sprintf("%s%s111%s",Op,XMask[XCount],Rest);
				condWRITE(VLD,XCount+7,NewMask,"BC","//destXEA");

				#if(substr(ValidEA[VLD],XCount+7,1)==1)
				#{
				#	printf ("	%s, %s, \"%s%s111%s\", %s %s, \"%s\", %s, %s %s	//destXEA\n",
				#			$1,$2,Op,XMask[XCount],Rest,$4,$5,$6,Source,Dest,$9)>>SRCOut;

				#	printf ("BC,%s%s111%s,%03d,%s,%s,%s,%s\n",
				#			Op,XMask[XCount],Rest,F,$5,Mnem,Source,Dest)>>DATFile; 
				#}
			}
		}


/[0-9A]x003F[	 ]*[0-9A]x1FC0/ {		#	LEFT and RIGHT HAND EAs

		NGEN=0
		printf ("\n\n%s %s %s %s %s %s %s\n\n",GENFROM,$3,$4,$5,$6,SMODE,DMODE)>>SRCOut;
		printf ("//,%s,%03d,%s,%s,%s,%s\n",CodeMask,F,$5,Mnem,SMODE,DMODE)>>DATFile;

		Op=substr(CodeMask,1,4); 

		# Do Lefthand (Dest) Register EAs

		for(LCount=0;LCount<7;LCount++)
		{
	
			Dest=LMode[LCount];

			if(substr(ValidEA[DVLD],LCount+1,1)==1)
			{

				# Do Righthand (Source) Register EAs within each Lefthand Register EA.

				for(RCount=0;RCount<7;RCount++)
				{
					Source=RMode[RCount]

					NewMask=sprintf("%sxxx%s%sxxx",Op,Mask[LCount],Mask[RCount]);
					condWRITE(SVLD,RCount+1,NewMask,"CB","//EAregEAreg");

					#if(substr(ValidEA[SVLD],RCount+1,1)==1)
					#{
			
					#	printf ("	%s, %s, \"%sxxx%s%sxxx\", %s %s, \"%s\", %s, %s %s	//EAregEAreg\n",
					#				$1,$2,Op,Mask[LCount],Mask[RCount],$4,$5,$6,Source,Dest,$9)>>SRCOut;

					#	printf ("CB,%sxxx%s%sxxx,%03d,%s,%s,%s,%s\n",
					#			Op,Mask[LCount],Mask[RCount],F,$5,Mnem,Source,Dest)>>DATFile; 
					#}
				}
	
				# Do Righthand Non Register EAs within each LeftHand Register EA
		
				for(XCount=0;XCount<5;XCount++)
				{
					Source=XMode[XCount];

					NewMask=sprintf("%sxxx%s111%s",Op,Mask[LCount],XMask[XCount]);
					condWRITE(SVLD,XCount+7,NewMask,"CC","//EAregEAabs");

					#if(substr(ValidEA[SVLD],XCount+7,1)==1)
					#{
					#	printf ("	%s, %s, \"%sxxx%s111%s\", %s %s, \"%s\", %s, %s %s	//EAregEAabs\n",
					#				$1,$2,Op,Mask[LCount],XMask[XCount],$4,$5,$6,Source,Dest,$9)>>SRCOut;

					#	printf ("CC,%sxxx%s111%s,%03d,%s,%s,%s,%s\n",
					#			Op,Mask[LCount],XMask[XCount],F,$5,Mnem,Source,Dest)>>DATFile; 
					#}
				}
			}
		}

		# Do Lefthand Non Register EAs
	
		for(LCount=0;LCount<5;LCount++)
		{
			
			Dest=XMode[LCount];

			if(substr(ValidEA[DVLD],LCount+7,1)==1)
			{
				# Do Righthand Register EAs within each Lefthand Non Register EA
	
				for(RCount=0;RCount<7;RCount++)
				{
					Source=RMode[RCount]
					NewMask=sprintf("%s%s111%sxxx",Op,XMask[LCount],Mask[RCount]);
					condWRITE(SVLD,RCount+1,NewMask,"CD","//EAabsEAreg");
	
					#if(substr(ValidEA[SVLD],RCount+1, 1)==1)
					#{
					#	printf ("	%s, %s, \"%s%s111%sxxx\", %s %s, \"%s\", %s, %s %s	//EAabsEAreg\n",
					#			$1,$2,Op,XMask[LCount],Mask[RCount],$4,$5,$6,Source,Dest,$9)>>SRCOut;
	
					#	printf ("CD,%s%s111%sxxx,%03d,%s,%s,%s,%s\n",
					#			Op,XMask[LCount],Mask[RCount],F,$5,Mnem,Source,Dest)>>DATFile; 
					#}
				}
	
				# Do Righthand Non Register EAs within each Lefthand Non Register EA
	
				for(XCount=0;XCount<5;XCount++)
				{
					Source=XMode[XCount];

					NewMask=sprintf("%s%s111111%s", Op, XMask[LCount], XMask[XCount]);
					condWRITE(SVLD,XCount+7,NewMask,"CE","//EAabsEAabs");
	
					#if(substr(ValidEA[SVLD],XCount+7,1)==1)
					#{
					#	printf ("	%s, %s, \"%s%s111111%s\", %s %s, \"%s\", %s, %s %s	//EAabsEAabs\n",
					#			$1,$2,Op,XMask[LCount],XMask[XCount],$4,$5,$6,Source,Dest,$9)>>SRCOut;

					#	printf ("CE,%s%s111111%s,%03d,%s,%s,%s,%s\n",
					#			Op,XMask[LCount],XMask[XCount],F,$5,Mnem,Source,Dest)>>DATFile; 
					#}
				}
			}
		}
}

!/[0-9A]x1FC0/&&!/[0-9A]x003F/{		# NO EAs

		if(NGEN++==0){	printf("\n@%04d Not Generated Header (%04d-%04d)\n",RecsRead, ++OUTCount, OUTCount+=3);
						printf("\n\n	// Not Generated - Direct Copy\n\n")>>SRCOut; }
		TheString=sprintf("\t%s, %s, \"%s\", %s %s, \"%s\", %s, %s %s %s %s %s",
				$1,$2,$3,$4,$5,$6,SMODE,DMODE,$9,$10,$11,$12,$13)

		printf("%s\n", TheString) >>SRCOut;

		if( SMODE == "0xF000" ) DMODE="0xF000";

		printf ("@%04d Not Generated Call (%04d): %s\n", RecsRead, ++OUTCount, TheString );
		printf ("DA,%s,%03d,%s,%s,%s,%s\n",CodeMask,F,$5,Mnem,SMODE,DMODE)>>DATFile; }


END {	FuncOrder=0; ColCount=-1; Comma="";Tabs="";

		#Generate function index array
		
		printf("typedef struct { void (*f)(void); char *m; int s; int d; int x; } OT;\n\n")>>HDRFile;
		printf("typedef void (*OX)(void);\n\n" ) >>HDRFile;
		printf("OT optable[65536];\n\n") >>HDRFile;
		printf("#define Z %s\n\n", FuncCount) >>HDRFile;
		printf("OX OpX[Z]={\t",FuncCount) >>HDRFile;

		printf("#ifdef USEGOTO\n\t#define NEXT goto next;\n")>>STBFile;
		printf("\t#define IDECL(LabelName) LabelName :\n")>>STBFile;
		printf("#else\n\t#define NEXT\n\t#define IDECL(LabelName) void (LabelName)(void)\n#endif\n\n")>>STBFile; 

		for( FuncOrder=0; FuncOrder < FuncCount;)
			for( FuncName in FuncID )
				if(FuncOrder==FuncID[FuncName])
				{
					printf("IDECL(%s)\n{ printf(\"Function %s called\\n\");\n NEXT; \n}\n", FuncName, FuncName) >>STBFile;
					printf("void %s(void);\n",FuncName)>>FNCFile;
					
					if(++ColCount==6) { ColCount=0; printf(",\n\t\t\t") >>HDRFile }
							else	{ printf("%s%s",Comma,Tabs) >>HDRFile; Comma=","; }
					printf("&%s", FuncName) >>HDRFile;
					Tabs=substr("                 ", length(FuncName)+1 );
					if( ++FuncOrder == FuncCount ) printf("\t};\n") >>HDRFile;
				}

		printf("\n\nchar OpMnem[]=\"%s\\0\";",MnemList) >>HDRFile;
		printf("\ntypedef unsigned char Cond;\n#define true 1\n#define false 0\n\n") >>STBFile
		printf("\n\n") >>HDRFile
	}
