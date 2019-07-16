#!/bin/bash
date=`date +%Y-%m-%d__%H:%M:%S`;
loadavg=`cat /proc/loadavg | cut -d " " -f -3`;
temp=`cat /sys/class/thermal/thermal_zone0/temp`;
temp=`echo $temp | awk '{printf("%.2f\n",$1/1000)}'`
tempforcmp=`echo $temp | awk '{printf("%d\n",$1*1000)}'`
if [[ $tempforcmp -lt 50000 ]]; then
    WarningLevel="normal";
elif [[ $tempforcmp -lt 70000 ]]; then
    WarningLevel="note";
else
    WarningLevel="warning";
fi
stat1=`cat /proc/stat | head -n 1 | cut -d " " -f 3-11 `;
sleep 0.5;
stat2=`cat /proc/stat | head -n 1 | cut -d " " -f 3-11 `;
for i in $stat1
do
    totalCpuTime1=$[${totalCpuTime1}+${i}];
done
for i in $stat2
do
    totalCpuTime2=$[${totalCpuTime2}+${i}];
done
totalCpuTime=$[$totalCpuTime2-$totalCpuTime1];
idle1=`echo $stat1 | cut -d " " -f 4`;
idle2=`echo $stat2 | cut -d " " -f 4`;
idle=$[$idle2-$idle1];
pcpu=`echo "$totalCpuTime $idle" | awk '{printf("%.2f%%\n", 100*($1-$2)/$1)}'`;

echo "$date $loadavg $pcpu $temp℃  $WarningLevel";
