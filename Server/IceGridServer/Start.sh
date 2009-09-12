#!/bin/sh
rm -rf db/node/*
rm -rf db/registry/*
icegridnode --Ice.Config=config.grid &
sleep 5
icegridadmin --Ice.Config=config.grid -e "application add 'application.xml'"
icegridadmin --Ice.Config=config.grid -e "server start LBANetGlacier2"

cd ../../patch_client
icepatch2calc .
icepatch2server --IcePatch2.Endpoints="tcp -p 10000" . &
