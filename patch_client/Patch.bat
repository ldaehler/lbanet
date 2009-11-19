@ECHO OFF
ECHO Contacting the game server for updates. This can take a few minutes...
ECHO (in case of issues check the file patching.log)

patchingclient --IcePatch2.Remove=0 --IcePatch2.Endpoints="tcp -h 88.191.15.204 -p 10000" .
start LBANet.exe