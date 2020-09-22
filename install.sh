#!/bin/bash

gcc -W -g prototype.c -lpthread -lncursesw -D_XOPEN_SOURCE_EXTENDED -lvlc player.h player.c -o prototype



