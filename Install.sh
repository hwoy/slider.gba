#!/bin/sh

find . -name "Makefile.?" | xargs -I{} make -f {} install
