# https://gist.github.com/neatshell/5283811


#!/bin/bash

script="new-project"
#Declare the number of mandatory args
margs=1

# Common functions - BEGIN
function example {
    echo -e "example: $script -name NAME -p PATH"
}

function usage {
    echo -e "usage: $script NAME [PATH]\n"
}

function help {
  usage
    echo -e "MANDATORY:"
    echo -e "  -name,  NAME  The name of the project"
    echo -e "OPTION:"
    echo -e "  -p,     PATH  Optional output directory"
    echo -e "  -h,   --help             Prints this help\n"
  example
}

# Ensures that the number of passed args are at least equals
# to the declared number of mandatory args.
# It also handles the special case of the -h or --help arg.
function margs_precheck {
	if [ $2 ] && [ $1 -lt $margs ]; then
		if [ $2 == "--help" ] || [ $2 == "-h" ]; then
			help
			exit
		else
	    	usage
			example
	    	exit 1 # error
		fi
	fi
}

# Ensures that all the mandatory args are not empty
function margs_check {
	if [ $# -lt $margs ]; then
	    usage
	  	example
	    exit 1 # error
	fi
}
# Common functions - END

# Custom functions - BEGIN
# Put here your custom functions
# Custom functions - END

# Main
margs_precheck $# $1

marg0=
oarg0="./"

# Args while-loop
while [ "$1" != "" ];
do
   case $1 in
   -name | --mandatory0 )  shift
                          marg0=$1
                		  ;;
   -m1  | --mandatory1 )  shift
   						  marg1=$1
			              ;;
   -p   | --optional0  )  shift
                          oarg0=$1
                          ;;
   -o1  | --optional1  )  shift
                          oarg1=$1
                          ;;
   -h   | --help )        help
                          exit
                          ;;
   *)                     
                          echo "$script: illegal option $1"
                          usage
						  example
						  exit 1 # error
                          ;;
    esac
    shift
done

# Pass here your mandatory args for check
margs_check $marg0 $marg1


mkdir -p $oarg0/$marg0/
cp -R ./GameTemplate/. $oarg0/$marg0/.
mv $oarg0/$marg0/src/GameTemplate.cpp $oarg0/$marg0/src/$marg0.cpp
mv $oarg0/$marg0/src/GameTemplate.hpp $oarg0/$marg0/src/$marg0.hpp


oldstring=GameTemplate
newstring=$marg0

grep -rl $oldstring $oarg0/$marg0/ | xargs sed -i s@$oldstring@$newstring@g

