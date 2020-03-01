#!/bin/bash
# List the current directory files, which are hard links to the specified file.
# The list should be sorted by access time.

file=$1
str_hello="Hard links to the file \"$file\":"

if [ -f "$file" ]; then
    echo $str_hello
    find . -samefile $file ! -name $file -printf "%f\n" |
	xargs -r ls -ltuF |
	nawk -f color.awk -e '{
	     for( i=6; i<=NF; i++ ){
	     	  if (i > 8)
		     printf("%s ", GREEN($i)); 
		  else 
		     printf("%s ", $i);
	      };
	      printf("\n")}
	      END { if (NR == 0) printf("%s ", RED("'$file' has no hard links\n"))	
	 }'
else
    echo -e "\e[31m\nOops, there\`s no file \"$file\" in this directory!\n\e[0m"
fi
