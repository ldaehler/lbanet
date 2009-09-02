del .\cpp\*.cpp
del .\include\*.h
cd ./slice
slice2cpp -I. ./*.ice --output-dir ../cpp
move ..\cpp\*.h ..\include
cd ..


