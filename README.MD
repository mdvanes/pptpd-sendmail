# pppdp-sendmail

A plugin for pptpd that will send a mail when someone logs in / a client connects to pptpd

Based on the only pptpd plugin I could find: [pptpd-logwtmp.c](https://github.com/chrisballinger/poptop/blob/master/plugins/pptpd-logwtmp.c)

Basically it's a modified copy of this plugin.

Requires **sendmail** to be installed.


### Installation

There is a dependency with pppd.h.

```sudo apt-get install ppp-dev```

Build by running ```make``` in this dir. This should result in a **pptpd-sendmail.so**.

_Follow instructions of pppd plugin installation._?? Can't find documentation?

Place pptpd-sendmail.so in /etc/ppp/

Make sure that pptpd-sendmail.properties exists in the same dir as pptpd-sendmail.so

pptpd-sendmail.properties should contain the "to address":

```
to=example2@example.com
```

add plugin in /etc/ppp/pptpd-options (the value of "option" in /etc/pptpd.conf) at the bottom:
```
plugin pptpd-sendmail.so
```

restart the server:
```sudo /etc/init.d/pptpd restart```


## TODO

* issue: the body is now cut off and doesn't log the IP but it does log the "to" address, which it shouldn't.
* when building a lot of errors are logged that are not logged with the same functions in test.c
* make a library of the functions that can be called from both test.c and pptpd-sendmail.c
* complete instructions for installation
* Git keyword expansion http://git-memo.readthedocs.org/en/latest/git_archive.html


## C development

The other plugin (pptpd-logwtmp) is written in C and I have zero experience in C, so I have some notes on building:

### building - an introduction

introduction to Makefile: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

also, try to run ```gcc -o hellomake hellomake.c hellofunc.c -I.``` in the ```testing-c``` subdir

or run ```make``` which will run the Makefile. This is very picky with indents; it must be 1 tab.


### building - pptpd-logwtmp has a Makefile

There is a dependency with pppd.h.

Running ```apt-file search pppd/pppd.h``` gives ```ppp-dev: /usr/include/pppd/pppd.h```

So do: ```sudo apt-get install ppp-dev```

run ```make```, this should create a pptpd-sendmail.so


### running

Don't run ./pptpd-sendmail.so, it's a plugin and has no main.

Make sure that pptpd-sendmail.properties exists in the same dir as pptpd-sendmail.so

pptpd-sendmail.properties should contain the "to address":

```
to=example2@example.com
```

Go to ```testing-sendmail``` and run ```make```. Then run ```./test```.

