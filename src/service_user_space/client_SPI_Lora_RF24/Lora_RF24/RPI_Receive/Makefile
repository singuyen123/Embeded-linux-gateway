#############################################################################
#
# Makefile for librf24 examples on Linux
#
# License: GPL (General Public License)
# Author:  gnulnulf <arco@appeltaart.mine.nu>
# Date:    2013/02/07 (version 1.0)
#
# Description:
# ------------
# use make all and make install to install the examples
#

ifeq ($(wildcard ../Makefile.inc), )
 $(error Configuration not found. Run ./configure first)
endif

include ../Makefile.inc

# define all programs	testRF testLR
PROGRAMS = lora_rf24_receive 

include Makefile.examples
