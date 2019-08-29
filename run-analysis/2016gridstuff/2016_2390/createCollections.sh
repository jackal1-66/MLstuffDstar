#!/bin/bash
#simple script to create collections from LEGO train outputs on AliEn.

runnum=$1
[[ -d lists ]] || mkdir lists
for runnum in {1..7}
   do
# find train outputs from Stage1 merging (path to train must be newly defined for each run!)
   alien_find -c collection.xml  /alice/cern.ch/user/a/alitrain/PWGHF/D2H_pp/2390_20190702-1541_child_$runnum AnalysisResults.root > "lists/$runnum.txt"
# trim first and last 2 lines (only want paths, not status messages)
   sed -i '$d' lists/$runnum.txt
   sed -i '$d' lists/$runnum.txt
   sed -i '1d' lists/$runnum.txt

 # Check that file has nonzero length
      result=`wc -l < lists/$runnum.txt`

      if [ $result -gt 0 ]; then
         echo "$runnum OK";
   # if it's zero, fall back to unmerged outputs (in case stage1 purged but unmerged still available)
      else 
         echo "$runnum not OK, fallback"; 
         alien_find -c collection.xml  /alice/cern.ch/user/a/alitrain/PWGHF/D2H_pp/2390_20190702-1541_child_$runnum AnalysisResults.root > "lists/$runnum.txt"
# trim leading/tailing lines
         sed -i '$d' lists/$runnum.txt
         sed -i '$d' lists/$runnum.txt
         sed -i '1d' lists/$runnum.txt
# Check whether it's worked now. If not, give warning.
         result=`wc -l < lists/$runnum.txt`
         if [ $result -gt 0 ]; then echo "it's OK now";
         else echo "still not ok. Check this run number.";
         fi
      fi
   done
cat lists/*.txt > totallist.txt
root -l merge.C
