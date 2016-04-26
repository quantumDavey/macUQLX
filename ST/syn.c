

char *MnemPtr( char *theString )
{
	char	*p;
	int		thecount, i;

	for( p=MnemList, thecount=0;*(++p) != '\0';thecount++ )
		if (!strcmp( theString, p)) return (p); else for(;*++p != '\0';);

	return(MnemList);
}
