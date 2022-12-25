#!/bin/bash

# test.ll must be generated after program run

llc build/test.ll # build ll ir
as build/test.s -o build/test.o # build assembly from llc
ld build/test.o -L /Library/Developer/CommandLineTools/SDKs/MacOSX13.1.sdk/usr/lib -lSystem -o build/a.out # link with libSystem
