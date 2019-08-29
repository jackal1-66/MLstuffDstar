
# Quit if no run number specified
#if [ -z $runnum ]; then
#    echo "Run number not specified! Quitting..."
#    exit
#    fi

for runnum in  {2..14};
do
# Quit if input list doesn't exist
if ! [ -f "lists/$runnum.txt" ]; then
   echo "File doesn't exist! Quitting..."
   exit
   fi
# If output dir not made, make it
if !  [ -d $runnum ]; then 
   mkdir $runnum;
   fi
#Get list of input files & download them
for f in `cat lists/$runnum.txt`;
   do
     filenum=$(echo $f | grep -o -E '[0-9]+/T' | cut -f1 -d "/")
     [[ -d $runnum/$filenum ]] || mkdir -p "$runnum/$filenum"
     if [ -e $runnum/$filenum/AnalysisResults.root ]; then continue; fi
     echo "$runnum/$filenum:"
     alien_cp "alien://$f" "$runnum/$filenum/" 
     done;

done;
