#include <stdio.h>

int main(int argc, char *argv[])
{ 

	int counter = 1;
	
	while ( counter < argc  )
	{
		int y = atoi(argv[counter++]); 
		if ((y % 100 != 0 && y % 4 == 0) || y % 400 == 0)
		{
			printf("%d yes\n", y);
			continue;
		} 
		printf("%d no\n", y);
	}

	return 0;
}
