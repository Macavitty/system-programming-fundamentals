#!/bin/bash
function is_arg_empty {
        if [ -z $1 ]; then
                echo "invalid argument: empty value"
                exit 1
        fi
}

function validate_file {
    is_arg_empty $file
    if [ -d $file ]; then
        if [ -L $file ] ; then 
            echo "invalid argument: $file is a symbolic link"
            exit 1
      
        else
            echo "invalid argument: $file is a directory"
            exit 1
        fi
    elif [ ! -f $file ]; then
        echo "invalid argument: file  $file does not exists"
        exit 1
    fi
}

function check_permission {
    if [[ $(id -u $1) = $owner ]] ; then
        if [ ! -z $perm_o ]; then
               echo $1
        fi
    elif [[  $(id -g $1) = $group ]] ; then
        if [ ! -z $perm_g ] ; then
              echo $1
        fi
    elif [ ! -z $perm_e ] ; then
        echo $1
    fi  
}

function find_users {
    read owner group <<<$(ls -n $file | nawk '{ print($3 " " $4) }')
    perm_o=$(ls -V $file | grep owner@:.w)
    perm_g=$(ls -V $file | grep group@:.w)
    perm_e=$(ls -V $file | grep everyone@:.w)
    for i in  `getent passwd | nawk -F: '{print($1)}'`
    do
        check_permission $i
    done    
}

while [ 1 ] ; do
   if [ "${1#--file=}" != "$1" ] ; then
      file="${1#--file=}"
      is_arg_empty $file
   elif [ "$1" = "-f" ] ; then
      shift ; file="$1"
      is_arg_empty $file
   elif [ -z "$1" ] ; then 
      break # No more keys
   else
      echo "unknown option: $1" 1>&2
      echo "usage: $0 [-u <username> | --user=<username>] [-f <filename> | --file=<filename>]" 1>&2 
      exit 1 
   fi 
   shift 
done

validate_file
find_users


