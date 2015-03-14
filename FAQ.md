# Frequently Asked Questions #

## Compiling ##

### What are HiDeO dependencies? ###

In order to build HiDeO, the following software packages are required:
  * [avg-gcc](http://gcc.gnu.org/) — GCC 4.x for Atmel AVR 8-bit RISC cross-development
  * [avr-libc](http://www.nongnu.org/avr-libc/) — A C and math library for the Atmel AVR controller family
  * [avrdude](http://www.bsdhome.com/avrdude/) ­— Program for programming the on-chip memory of Atmel AVR CPUs
  * [GNU Make](http://www.gnu.org/software/make/) or [BSD Make](http://www.freebsd.org/cgi/cvsweb.cgi/src/usr.bin/make/) — A progam to maintain program dependencies

We also provide a [KontrollerLab](http://sourceforge.net/projects/kontrollerlab/) project to ease development.

## Troubeshooting ##

### PS2 stuff does not work! Why? ###

Double-check the device fuses related to CPU frequency. It has to be configured to run at 8MHz to handle PS2 messages at the right rate. Using avrdude in terminal mode:
```
sudo avrdude -p m8535 -c stk200 -t
```
ensure you have:
```
avrdude> read lfuse
>>> read lfuse
0000  e4                                                |.               |

avrdude> read hfuse
>>> read hfuse
0000  d9                                                |.               |
```

Warning: "fresh" new microcontrollers are configured at 1MHz. Don't forget to configure them at 8Mhz:
```
avrdude> read lfuse
>>> read lfuse
0000  e1                                                |.               |

avrdude> write lfuse 0 0xe4
>>> write lfuse 0 0xe4

avrdude> read lfuse
>>> read lfuse
0000  e4                                                |.               |
```


### Why does my chip resets while ISP is plugged? ###

Double-check your parallel port is in '''EPP''' or '''ECP''' mode in the BIOS. Additionnaly, some kernel modules may put reset line always on such as '''lp''' (GNU/Linux) or '''lpt''' (FreeBSD).

Under FreeBSD, only '''ppi(4)''' module is needed.

Under GNU/Linux, only '''ppdev''' is needed.

### Why my 'left' and 'right' keys doesn't works? ###

Just unplugged your ISP: keyboard port are shared with ISP and two internals pull-ups haven't any effect if ISP is plugged.