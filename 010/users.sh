#!/bin/bash
# List users belonging to more than the specified number of groups

gr_num=$1
int_regexp='^[0-9]|[1-9][0-9]*$'
if ! [[ $gr_num =~ $int_regexp ]] ; then
    echo -e "\e[31m\nHoy, the first arg must be a positive integer!\n\e[0m"
else
    cat /etc/passwd |
	nawk -F: '{print($1)}' |
	xargs groups |
	nawk -f color.awk -e '{ 
	     if (NF - 2 > '$gr_num')
	     	printf("%s: %s \n", GREEN($1), GREEN(NF - 2) );}' | sort
fi


