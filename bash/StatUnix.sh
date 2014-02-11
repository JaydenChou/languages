#!/bin/ksh
# Program:    StatUnix.sh
# Type:       Shell Script
# Arguments:  -
# Obs.:       monitor of process, filesystems, memory and cpu state, in unix/linux
# Return:     -
#   Feb/2005: Javier Fuchs                                    |\_/|   
#             Phone (+54 2944) 525173                        /     \  
#             http://southfox.servehttp.com                 /_.~ ~,_\ 
#                                                              \@/    
#

# file size limit (in kbytes)
FileLimit=30000

# development file system maxim limit (en %)
MountDevLimit=70

# file system limit
MountLimit=95

# oracle filesystem limit 
MountLimitOracle=100

FncMailSent()
{
   Disk=$1
   Percent=$2
   Limit=$3
   MAILFIND=$(find . ${Disk} -mount -size +${FileLimit}k -ls | grep -v "/devices/")
mailx -s "Achtung!!!, the disk $Disk is $Percent %" j_fuchs@crm.com.ar << mailbuffer
Machine: $(uname -a)
Date: $(date)
Disk: ${Disk}
Limit "${Limit} %"

--------------------------------------------------------------------------------
DF -K
--------------------------------------------------------------------------------
$(df -k ${Disk})
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
FIND [DISK] -SIZE +${FileLimit}K -LS
--------------------------------------------------------------------------------
${MAILFIND}
--------------------------------------------------------------------------------

mailbuffer
}

FncFreeSpace()
{
   Disk=$1
   Limit=$2
   let iSize=$(df -k ${Disk} | grep -v Filesystem | awk '{print $5}' | cut -d% -f1)
   if [ $iSize -gt $Limit ]
   then
      FncMailSent $Disk $iSize $Limit
   fi
}

NameLog=$(uname -n).$(date '+%d.%m.%y_%H%M%S')

DATSTAT=/data19/admunix/DAT/

VMLog=${DATSTAT}vmstat.${NameLog}.log
VMErr=${DATSTAT}vmstat.${NameLog}.err

IOLog=${DATSTAT}iostat.${NameLog}.log
IOErr=${DATSTAT}iostat.${NameLog}.err

MPLog=${DATSTAT}mpstat.${NameLog}.log
MPErr=${DATSTAT}mpstat.${NameLog}.err

DFLog=${DATSTAT}df_k.${NameLog}.log
DFErr=${DATSTAT}df_k.${NameLog}.err

PSLog=${DATSTAT}ps_e_o.${NameLog}.log
PSErr=${DATSTAT}ps_e_o.${NameLog}.err

PTLog=${DATSTAT}ptree_k.${NameLog}.log
PTErr=${DATSTAT}ptree_k.${NameLog}.err

WHLog=${DATSTAT}whodo.${NameLog}.log
WHErr=${DATSTAT}whodo.${NameLog}.err

vmstat 10 10 1>${VMLog} 2>${VMErr} &

iostat 10 10 1>${IOLog} 2>${IOErr} &

mpstat 10 10 1>${MPLog} 2>${MPErr} &

df -k 1>${DFLog} 2>${DFErr} &

ps -e -o ppid,pid,user,time,etime,stime,fname,comm 1>${PSLog} 2>${PSErr} &

/usr/proc/bin/ptree 1>${PTLog} 2>${PTErr} &

whodo 1>${WHLog} 2>${WHErr} &
who -b  1>>${WHLog} 2>>${WHErr} &
who -a  1>>${WHLog} 2>>${WHErr} &

for xDisk in $(df -k | awk '{print $6}' | grep -v -w "Mounted")
do
   # Disks for development
   test "$xDisk" = "/data1" -o "$xDisk" = "/data2" -o "$xDisk" = "/tmp" -o "$xDisk" = "/"
   if [ $? -eq 0 ]
   then
      Limit=${MountDevLimit}
   else
      case "$xDisk" in
       /data5 | /data7)
         Limit=${MountLimitOracle}
         ;;
       *) Limit=${MountLimit}
         ;;
      fi
   fi
   FncFreeSpace $xDisk $Limit
done

