#!/bin/bash
#Arguments to this bash:
#   $1 is /path/to/outputdir/lsOutputMergedList_$TRAINNAME$STAGE.txt

START=$(date +%s)

isMC=1                 #toset
ispp=1                 #toset
doDplusFromEvt=1       #toset
doDsFromEvt=1          #toset
doDzeroFromEvt=1       #toset
doDstarFromEvt=1       #toset
doBplusFromEvt=0       #Classes not yet ready + not tested
doLcFromEvt=0          #toset
#doPID=0               #to be added

myfile=$1

if [ $doDplusFromEvt -eq 1 ]
then

DataTree="tree_Dplus"

while IFS='' read -r line || [[ -n "$line" ]]; do
g++ includeSkim/skimTreeDplusFromEvt.C $(root-config --cflags --libs) -g -o skimTreeDplusFromEvt.exe
./skimTreeDplusFromEvt.exe "${line}" "${line%.*}_Dplus_skimmed.root" "$DataTree" "$isMC" "$ispp"
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Dplus = All done"
rm -rf skimTreeDplusFromEvt.exe skimTreeDplusFromEvt.exe.dSYM

fi



if [ $doDsFromEvt -eq 1 ]
then

DataTree="tree_Ds"

while IFS='' read -r line || [[ -n "$line" ]]; do
g++ includeSkim/skimTreeDsFromEvt.C $(root-config --cflags --libs) -g -o skimTreeDsFromEvt.exe
./skimTreeDsFromEvt.exe "${line}" "${line%.*}_Ds_skimmed.root" "$DataTree" "$isMC" "$ispp"
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Ds = All done"
rm -rf skimTreeDsFromEvt.exe skimTreeDsFromEvt.exe.dSYM

fi



if [ $doDzeroFromEvt -eq 1 ]
then

DataTree="tree_D0"

while IFS='' read -r line || [[ -n "$line" ]]; do
g++ includeSkim/skimTreeDzeroFromEvt.C $(root-config --cflags --libs) -g -o skimTreeDzeroFromEvt.exe
./skimTreeDzeroFromEvt.exe "${line}" "${line%.*}_Dzero_skimmed.root" "$DataTree" "$isMC" "$ispp"
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Dzero = All done"
rm -rf skimTreeDzeroFromEvt.exe skimTreeDzeroFromEvt.exe.dSYM

fi


if [ $doDstarFromEvt -eq 1 ]
then

DataTree="tree_Dstar"

while IFS='' read -r line || [[ -n "$line" ]]; do
g++ includeSkim/skimTreeDstarFromEvt.C $(root-config --cflags --libs) -g -o skimTreeDstarFromEvt.exe
./skimTreeDstarFromEvt.exe "${line}" "${line%.*}_Dstar_skimmed.root" "$DataTree" "$isMC" "$ispp"
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Dstar = All done"
rm -rf skimTreeDstarFromEvt.exe skimTreeDstarFromEvt.exe.dSYM

fi


if [ $doBplusFromEvt -eq 1 ]
then

DataTree="tree_Bplus"

while IFS='' read -r line || [[ -n "$line" ]]; do
g++ includeSkim/skimTreeBplusFromEvt.C $(root-config --cflags --libs) -g -o skimTreeBplusFromEvt.exe
./skimTreeBplusFromEvt.exe "${line}" "${line%.*}_Bplus_skimmed.root" "$DataTree"
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Bplus = All done"
rm -rf skimTreeBplusFromEvt.exe skimTreeBplusFromEvt.exe.dSYM

fi



if [ $doLcFromEvt -eq 1 ]
then

DataTree="tree_LctopKpi"

while IFS='' read -r line || [[ -n "$line" ]]; do
g++ includeSkim/skimTreeLcFromEvt.C $(root-config --cflags --libs) -g -o skimTreeLcFromEvt.exe
./skimTreeLcFromEvt.exe "${line}" "${line%.*}_Lc_skimmed.root" "$DataTree" "$isMC" "$ispp"
done < "$myfile"

## would wait until those are completed
## before displaying all done message
wait
echo "Lc = All done"
rm -rf skimTreeLcFromEvt.exe skimTreeLcFromEvt.exe.dSYM

fi


END=$(date +%s)
DIFF=$(( $END - $START ))
echo "It took $DIFF seconds"
