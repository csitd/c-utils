#!/bin/sh

# (C) 2014, MIT license, "cruncher.sh", csitd

# Usage:  ./cruncher.sh binary1.c binary2.c binary3.c

# This only works with very simple utilities that do not
# overlapping function declarations.

rm crunched
rm main.c
touch crunched


cc(){
	gcc -static -Wall $@
} 


tokenizer()
{
     STRNG="${1}"
     DELIM="${2}"

     while :
     do
          NEW="${STRNG%${DELIM}}"
          while case "$NEW" in
                     *${DELIM}*);;
                     *)break;;
                esac 
          do   NEW="${NEW%${DELIM}*}"
          done
          TOKEN="${NEW%${DELIM}*}"
          STRNG="${STRNG#${TOKEN}${DELIM}}"
          printf "%s\n" "$TOKEN"

          case "$STRNG" in
               *${DELIM}*) ;;
               *) [ -n "$d" ] && break || d="1" ;;
          esac
     done
}



cat > temp  <<EOF
#include <stdio.h>
#include <string.h>
EOF

cat >> main.c <<EOF
int main (int argc, char *argv[])
{ 
	char *string = argv[0];
	size_t len = strlen(string);
	int count = len;
	while (string[count] != '/' && count >= 0 )
		count--;
	char actual[len - count];
	int cnt = 0;
	++count;
	while (string[count] > len )
	{ 
		actual[cnt++] = string[count++];
		actual[cnt + 1] = '\\0';
	}
	//printf("%s\\n", actual);
EOF



for i in $@
do 	tokenizer "$i" .
	REPLACE=$(tokenizer "$i" .) 
	printf "int ${REPLACE}main(int, char *[]);\n" >> temp
	cat "$i" > "crunch$i" 
	sed -i 's|argc|argcc|g' "crunch$i"
	sed -i 's|argv|argvv|g' "crunch$i"
	sed -i "s|main.*|${REPLACE}main(int argcc, char *argvv[])|g" "crunch$i"
	printf "\tif (strcmp(actual, \"${REPLACE}\") == 0 )\n" >> main.c
	printf "\t\t${REPLACE}main(argc, argv);\n" >> main.c 
	ln crunched ${REPLACE}
done



cat >> main.c << EOF
	return 0;
}
EOF

cat main.c >> temp
cat temp > main.c 

cc -o crunched crunch*.c main.c

rm crunch*c
rm main.c
rm temp


