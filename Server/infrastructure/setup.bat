del .\cpp\*.cpp
del .\include\*.h
cd ./slice
..\slice2cpp.exe -I. ./*.ice --output-dir ../cpp
move ..\cpp\*.h ..\include
cd ..


