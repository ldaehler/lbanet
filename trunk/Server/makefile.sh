#!/bin/sh


compil='g++'
executable='a.out'
exten='.cc'
inclusion='./include'
lib='./lib'


touch dep

while true
do
	case $1 in
	-comp ) shift;compil=$1;;
	-exec ) shift;silent=1;;
	-ext ) shift;exten=$1;;
	-inc ) shift;inclusion=$1;;
	-lib ) shift;lib=$1;;
	* ) break;;
	esac
shift

done

(echo "# Makefile realise a l'aide de MakefileGenerator"
echo "# $(date)"
echo 
templ1=$executable:
templ2=$compil
for i in *
	do

	[ .${i##*.} = $exten ] && templ1=$templ1" "${i%$exten}.o && templ2=$templ2" "${i%$exten}.o
	done

 templ2=$templ2" -o "$executable


echo $templ1
echo '	'$templ2

echo 

echo $exten.o
echo '	'$compil" -c $<"

echo
echo clean:
echo '	'"rm *.o *.a"
echo
echo depend:
echo '	'$compil" -MM *"$exten" >dep"
echo
echo include dep

)>Makefile
nc Makefile






