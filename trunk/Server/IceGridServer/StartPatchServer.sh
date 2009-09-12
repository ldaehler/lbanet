#!/bin/sh
cd ../../patch_client
icepatch2calc .
icepatch2server --IcePatch2.Endpoints="tcp -p 10000" . &
