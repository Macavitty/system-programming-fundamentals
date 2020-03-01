#!/bin/bash

err_file="./lab1_err.log"

while true; do
    echo '1. Print a current directory'
    echo '2. Change a directory'
    echo '3. Execute a command'
    echo '4. Create a new directory'
    echo '5. Remove directory recursively'
    echo '6. Quit'
    read opt || break

    case "$opt" in
        1) pwd;;
        2)
            echo 'Enter the path:'
	    read new_path
            cd "$new_path" 2>>$err_file || echo 'Error: cd' 1>&2
	    echo "$PWD"
        ;;
        3)
            echo 'Enter the command:'
            read cmd
            echo "" | xargs "$cmd"  2>>$err_file || echo "Error: command $cmd does not exists" 1>&2
        ;;
        4)
            echo 'Enter directory name:'
            read dir
            mkdir "$dir" 2>>$err_file || echo 'Error: ' 1>&2
        ;;
        5)
            echo 'Enter directory name:'
            read dir
            echo "rm: remove $dir (y/n)?"
            read confirm
            if [ "$confirm" = "y" ]
            then
                rm -rf "$dir" 2>>$err_file || echo 'Error: rm'  1>&2
            fi
            ;;
	6) break;;
	*) echo "No such an option: \"$opt\""
    esac
    echo ''
done

