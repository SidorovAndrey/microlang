#!/bin/bash

llc $1 -o $1.s # build ll ir
as $1.s -o $1.o # build assembly from llc
ld $1.o -L /Library/Developer/CommandLineTools/SDKs/MacOSX13.1.sdk/usr/lib -lSystem -o $1.out # link with libSystem
