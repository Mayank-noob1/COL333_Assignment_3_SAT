#!/bin/bash

bin/main.exe 1 $1.satoutput $1.mapping $(awk 'NR==1 {print $1}' $1.graphs)