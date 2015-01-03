#include <stdio.h>
#include <stdlib.h>
#include <s.h>

void expand_tokens(char *, char*);
int main()
{

	size_t len = 256;
	char *s = ";ls<ls>>ls>ls;ls|ls&ls;ls -la||ss&&ls;cd ..;;ls;ls -la;ss&&ls;cd ..";

	/* Try to make tests skips already expanded ss */
	//char *s = " ; ls < ls >> ls > ls ; ls | ls & ls ; ls -la || ss ";
	char *output = malloc(len);
	memset(output, 0, len); 
	
	expand_tokens(s, output); 

	printf ("%s\n", output); 
     
	return 0;

}

void expand_tokens(char *s, char* output)
{ 

	size_t len = 256; 
	size_t i = 0;
	size_t j = 0;
	while ( i < len - 1 )
	{
		
		if ( i == 255 || s[i] == '\0' ) break ; 

		/* ; */
		if ( i != 0 && s[i] == ';') 
		{ 
			output[j] = ' '; 
				++j;
			output[j] = ';';
				++j;
			output[j] = ' ';

		} 
		/* < */
                else if (s[i] == '<' )
                {
                        output[j] = ' ';
                                ++j;
                        output[j] = '<';
                                ++j;
               		output[j] = ' '; 
                }
		/* | */
		else if (s[i - 1] != '|' && s[i] == '|'
                         && s[i + 1] != '|'  )
                {
                        output[j] = ' ';
                                ++j;
                        output[j] = '|';
                                ++j;
                	output[j] = ' '; 
                }
		/* || */
                else if ( s[i] == '|' && s[i + 1] == '|' )
                {
                        output[j] = ' ';
                                ++j;
                        output[j] = '|';
                                ++j;
                        output[j] = '|';
                                ++j;
                        output[j] = ' ';
                                ++i; 
                }

		/* && */
		else if ( s[i] == '&' && s[i + 1] == '&' )
		{
			output[j] = ' ';
                        	++j;
                	output[j] = '&';
                        	++j;
			output[j] = '&';
                                ++j;
                	output[j] = ' ';
				++i; 
		}
		/* & */
		else if ( s[i - 1] != '&' && s[i] == '&'
			 && s[i + 1] != '&')
		{
			output[j] = ' ';
                                ++j;
                        output[j] = '&';
				++j;
			output[j] = ' ';
		}
		/* >> */
		else if (s[i] == '>' && s[i + 1] == '>' ) 
		{
			output[j] = ' ';
                                ++j;
                        output[j] = '>';
                                ++j;
                        output[j] = '>';
                                ++j;
                        output[j] = ' ';
                                ++i;
		/* > */
		}	
		else if (i != 0 && s[i - 1] != '>' && s[i] == '>'
                         && s[i + 1] != '>' )
		{	
			output[j] = ' ';
                       		++j; 
                        output[j] = '>'; 
                                ++j; 
                        output[j] = ' ';
		}
		/* normal */
		else output[j] = s[i];

		++j;
		++i; 
		
	} 
}
