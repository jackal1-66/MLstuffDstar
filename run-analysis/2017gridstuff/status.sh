
#for dir in `cat runList.txt`;
#   do if [[ -d $dir ]];
#      then echo "$dir exists";
#      else echo "$dir does not exist";
#      fi;
#done;

##alternative
ntotfiles=0;
ntotdownload=0;
for runnum in `cat runList.txt`;
do
#check if catalogue (lists/*) exists for run number
   if ! [ -f "lists/$runnum.txt" ]; then
      echo "Run number $runnum is in list, but has no catalogue...";
      continue;
      fi
   nfiles=`wc -l < lists/$runnum.txt`;
   ntotfiles=$((ntotfiles + nfiles));
# If output dir not made, say so
   if ! [ -d $runnum ]; then
      echo "Run number $runnum: Download not started. 0% (0/$nfiles)";
      continue;
      fi
# get list of file numbers from local catalogue, check if TOFcalibTree.root downloaded
   ndownload=0;
   for f in `cat lists/$runnum.txt`;
     do
        filenum=$(echo $f | grep -o -E '[0-9]+/T' | cut -f1 -d "/")
        [[ -d $runnum/$filenum ]] || continue;
        if [ -e $runnum/$filenum/TOFcalibTree.root ]; then ((ndownload++)); fi
     done;
   ntotdownload=$((ntotdownload+ndownload))
   perc=$(echo "scale=2; $ndownload/$nfiles * 100" | bc -l)
   perc=${perc%.*};
   echo "Run $runnum: Download $perc% done ($ndownload/$nfiles)";


done;
perctot=$(echo "scale=2; $ntotdownload/$ntotfiles * 100" | bc -l)
perctot=${perctot%.*};
echo "Overall: $perctot% of files downloaded ($ntotdownload/$ntotfiles)";
