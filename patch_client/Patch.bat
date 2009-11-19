@ECHO OFF
ECHO Checking the game server for updates. This can take a few minutes...
ECHO (in case of issue check the file patching.log)

icepatch2client --IcePatch2.Remove=0 --IcePatch2.Endpoints="tcp -h 88.191.15.204 -p 10000" .