# Makefile
#
# Makefile for the RadioHead project
#
# Author: Mike McCauley (mikem@airspayce.com)
# Copyright (C) 2014 Mike McCauley
# $Id: Makefile,v 1.4 2014/04/29 12:18:27 mikem Exp mikem $

PROJNAME = RadioHead
# Dont forget to also change the version at the top of RadioHead.h:
DISTFILE = $(PROJNAME)-1.6.zip

all:	doxygen dist upload

doxygen: 
	doxygen project.cfg

ci:
	(cd ..;ci -l `cat $(PROJNAME)/MANIFEST`)

dist:	
	(cd ..; zip $(PROJNAME)/$(DISTFILE) `cat $(PROJNAME)/MANIFEST`)

upload:
	rsync -avz $(DISTFILE) doc/ www.airspayce.com:public_html/mikem/arduino/$(PROJNAME)
