A plugin for pptpd that will send a mail when someone logs in / a client connects to pptpd

Based on the only pptpd plugin I could find: https://github.com/chrisballinger/poptop/blob/master/plugins/pptpd-logwtmp.c

Basically it's a modified copy of this plugin.


# C development

The other plugin (pptpd-logwtmp) is written in C and I have zero experience in C, so I have some notes on building:


## building - an introduction

introduction to Makefile: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

also, try to run ```gcc -o hellomake hellomake.c hellofunc.c -I.``` in the ```testing-c``` subdir

or run ```make``` which will run the Makefile. This is very picky with indents; it must be 1 tab.


## building - pptpd-logwtmp has a Makefile

There is a dependency with pppd.h.

Running ```apt-file search pppd/pppd.h``` gives ```ppp-dev: /usr/include/pppd/pppd.h```

So do: ```sudo apt-get install ppp-dev```

run ```make```, this should create a pptpd-sendmail.so


## running


# TODO

Git keyword expansion
http://git-memo.readthedocs.org/en/latest/git_archive.html




# Remove/old

* Do a checkout of https://github.com/wkz/pppd/tree/master/pppd such that pppd.h is checked out relative to this file in pppd/pppd.h
* one time: add pppd/ to .gitignore


