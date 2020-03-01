#!/bin/bash
function is_arg_empty {
        if [ -z $1 ]; then
                echo "invalid argument: empty value"
                exit 1
        fi
}

function chech_if_usr_exists {
        getent passwd $user > /dev/null
        if [ $? -ne 0 ]; then
                echo "user not found: $user"
                exit 1
        fi
} 

function validate_usr {
        if [ -z $user ]; then 
                user=$(whoami)
        fi
        chech_if_usr_exists
}

function can_delete {
    if [[ $(id -u $user) = $owner ]] ; then
        if [ ! -z $perm_o ]; then
               echo $1
        fi
    elif [[  $(id -g $user) = $group ]] ; then
        if [ ! -z $perm_g ] ; then
              echo $1
        fi
    elif [ ! -z $perm_e ] ; then
        echo $1
    fi  
}

function validate_dir {
        if [ -z  $dir ]; then dir=$(pwd)
        fi
        if [ -f $dir ]; then
            echo "invalid argument: $dir is a regular file"
            exit 1
        elif [ -d $dir ]; then
                if [ -L $dir ] ; then 
                        echo "invalid argument: $dir is a symbolic link"
                        exit 1
                fi
        else
                echo "invalid argument: $dir does not exists"
                exit 1
        fi
}

function chech_perm {
    for i in `ls`
    do
        if [ -d $i ]; then
            if [ ! -L $i ]; then
		read perm_o perm_g perm_e <<<$( |  -F@ 'BEGIN { RS = "" ; FS = "\n" }{ if (substr($2,9, 10) = "x") { print($2) " "} else { print("") " " } 
if (substr($2,9, 10) = "x") { print($3) " "} else { print("") " " }
if (substr($2,9, 10) = "x") { print($4) " "} else { print("") " " }')

		ls_str=$(ls -Vd $i)
		read perm_o perm_g perm_e <<<$( ls -Vd $i |
						    gawk 'BEGIN { RS = "" ; FS = "\n" } 
						    { gsub(/.*@:..x/,"x",$3);  gsub(/.*@:.*$/,"",$3); print $3 ; 
						      gsub(/.*@:..x/,"x",$2);  gsub(/.*@:.*$/,"",$2); print $2 ;
						      gsub(/.*@:..x/,"x",$1);  gsub(/.*@:.*$/,"",$2); print $1] ; }'


               # perm_o=$(ls -Vd $i | grep owner@:..x)
               # perm_g=$(ls -Vd $i | grep group@:..x)
               # perm_e=$(ls -Vd $i | grep everyone@:..x)
                owner=$(ls -n | awk '{ print($3) }')
                group=$(ls -n | awk '{ print($4) }')
                can_delete $i 
            fi
        fi
done
    
}

while [ 1 ] ; do
   if [ "${1#--user=}" != "$1" ] ; then 
      user="${1#--user=}"
      is_arg_empty $user
   elif [ "$1" = "-u" ] ; then 
      shift ; user="$1"
      is_arg_empty $user
   elif [ "${1#--dir=}" != "$1" ] ; then
      dir="${1#--dir=}"
      is_arg_empty $dir
   elif [ "$1" = "-d" ] ; then
      shift ; dir="$1"
      is_arg_empty $dir
   elif [ -z "$1" ] ; then 
      break # No more keys
   else
      echo "unknown option: $1" 1>&2
      echo "usage: $0 [-u <username> | --user=<username>] [-d <directory> | --dir=<directory>]" 1>&2 
      exit 1 
   fi 
   shift 
done

validate_usr
validate_dir
chech_perm 
