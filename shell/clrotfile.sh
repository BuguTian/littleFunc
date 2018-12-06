#!/bin/bash

echo "$0 is running...."
#Configuration. According to the your case configuration to configure.
INOTIFYDIR="/mpeg/mpeg1";
DELETETIME=`date --date="7 days ago" +%s`;
LOGDIR=`pwd`;

#Default Configuration.Please don't change.
RUNDIR=`pwd`;

function scandir() {
    local cur_dir parent_dir workdir
    workdir=$1
    cd ${workdir}
    if [ ${workdir} = "/" ]
    then
        cur_dir=""
    else
        cur_dir=`pwd`
    fi

    for dirlist in $(ls ${cur_dir})
    do
        if test -d ${dirlist};then
            echo -e "\033[0m"; 
            echo -e "\rCHANNEL: ${dirlist}"
            cd ${dirlist}
            scandir ${cur_dir}/${dirlist}
            cd ..
        else
            abs_dir=${cur_dir}/${dirlist};
            a=`stat -c %Y ${abs_dir}`;
            echo -n -e "\r\033[K\033[0m"; 
            echo -n -e "\033[34m"
            delflag=`expr $a - $DELETETIME`
            echo -n -e "$abs_dir\t$delflag" 
            if [ $DELETETIME -gt $a ];then
		echo "rm -f ${abs_dir}" >> "${LOGDIR}/rmfile.log"
	        rm -f ${abs_dir}
           else
                if [ ${cur_dir} != ${INOTIFYDIR} ];then
                     return;
                fi
            fi
            echo -n -e "\033[0m"; 
        fi
    done
}

if test -d $INOTIFYDIR
then	
	#Scand dir a new loop.
    scandir $INOTIFYDIR 
elif test -f $INOTIFYDIR 
then
    echo "You set a file as INOTIFYDIR but not a directory,pleas reinput and try again"
    exit 1
else
    echo "The INOTIFYDIR  isn't exist which you input,pls input a new one!!"
    exit 1
fi
cd ${RUNDIR}
echo -n -e "\033[0m"; 
echo "$0 has been finished....."

