#!/usr/bin/env bash
# TODO generalize for all args
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

function validate_dir {
	if [ -z  $dir ]; then dir=$(pwd)
	fi
	if [ -d $dir ]; then
		if [ -L $dir ] ; then 
			echo "invalid argument: $dir is a symbolic link"
			exit 1
		fi
	else
		echo "invalid argument: $dir not exists"
		exit 1
	fi
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
      echo "usage: $0 [-u <username> | --user=<username>] [-d <directory> | --dir=<directory>] [-h | --help]" 1>&2 
      exit 1 
   fi 
   shift 
done

validate_usr
validate_dir

echo "user: $user"
echo "dir: $dir"
