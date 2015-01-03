
void strcat(char src[], char dest[])
{
	int i, j;
	i = j = 0;
	while (src[i] != '\0') 
		i++;
	while ((src[i++] = dest[j++]) != '\0') 
		;
}

