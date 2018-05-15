################################################################################
#										#
#		Linux TPM2 Utilities Makefile					#
#			     Written by Ken Goldman				#
#		       IBM Thomas J. Watson Research Center			#
#	      $Id: makefile 740 2016-08-29 18:26:38Z kgoldman $			#
#										#
# (c) Copyright IBM Corporation 2016						#
# 										#
# All rights reserved.								#
# 										#
# Redistribution and use in source and binary forms, with or without		#
# modification, are permitted provided that the following conditions are	#
# met:										#
# 										#
# Redistributions of source code must retain the above copyright notice,	#
# this list of conditions and the following disclaimer.				#
# 										#
# Redistributions in binary form must reproduce the above copyright		#
# notice, this list of conditions and the following disclaimer in the		#
# documentation and/or other materials provided with the distribution.		#
# 										#
# Neither the names of the IBM Corporation nor the names of its			#
# contributors may be used to endorse or promote products derived from		#
# this software without specific prior written permission.			#
# 										#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS		#
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT		#
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR		#
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT		#
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,	#
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT		#
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,		#
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY		#
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT		#
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE		#
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.		#
#										#
#################################################################################

ARM				:= 1
TIME_DEBUG		:= 1

ifeq ($(ARM), 1)
ARCH			:= arm
CROSS_COMPILE	:= arm-linux-gnueabihf-
else
ARCH			:= x86
CROSS_COMPILE	:= 
endif

OPENSSL_VERSION	= 1.0.0s

# C compiler

CC = $(CROSS_COMPILE)gcc

# compile - common flags for TSS library and applications

ifeq ($(ARM), 1)
CCFLAGS = 	-DTPM_POSIX -I../../crypto/openssl-$(OPENSSL_VERSION)/build_$(ARCH)/include
KERNEL_BUILD    := $(PATH_TO_KTPM_KERNEL)
else
CCFLAGS = 	-DTPM_POSIX
KERNEL_RELEASE  := $(shell uname -r)
KERNEL_BUILD    := /lib/modules/$(KERNEL_RELEASE)/build
endif

CCFLAGS	+=	-D__EXPORTED_HEADERS__ -I$(KERNEL_BUILD)/include/uapi -I$(KERNEL_BUILD)/include


ifeq ($(TIME_DEBUG), 1)
CCFLAGS +=	-DTIME_DEBUG
endif

# compile - for TSS library

CCLFLAGS = 	-I. -I../src -DTPM_TSS

# compile - for applications

CCAFLAGS = 	-I.

# link - common flags flags TSS library and applications

LNFLAGS = 	-DTPM_POSIX		\
		-L.
ifeq ($(ARM), 1)
LNFLAGS += 	-L../../crypto/openssl-$(OPENSSL_VERSION)/build_$(ARCH)/lib
endif

# link - for TSS library

LNLFLAGS = 

# link - for applications, TSS path, TSS and OpenSSl libraries

LNAFLAGS = -Wl,-rpath,.

LNALIBS =  -ltss -lcrypto

# shared library

LIBTSS=libtss.so

# executable extension

#EXE=

#include makefile-common

# default build target

all:	$(ALL)

# TSS library source shared with TPM (for TSS library)

Unmarshal.c:			../src/Unmarshal.c
				sed '/TPM_TSS/a #define TPM_TSS' ../src/Unmarshal.c > Unmarshal.c
#CpriSym.c:			../src/CpriSym.c
#				ln -s ../src/CpriSym.c .
#CpriHash.c:			../src/CpriHash.c
#				ln -s ../src/CpriHash.c .
#Commands.c:			../src/Commands.c
#				ln -s ../src/Commands.c .
#CommandAttributeData.c:		../src/CommandAttributeData.c
#				ln -s ../src/CommandAttributeData.c .

# TSS shared library source

#tss.o: 		$(TSS_HEADERS) tss.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC tss.c
#tssproperties.o: $(TSS_HEADERS) tssproperties.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC tssproperties.c
#tssauth.o: 	$(TSS_HEADERS) tssauth.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC tssauth.c
tssmarshal.o: 	$(TSS_HEADERS) tssmarshal.c
		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC tssmarshal.c
#tsscrypto.o: 	$(TSS_HEADERS) tsscrypto.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC tsscrypto.c
#tssutils.o: 	$(TSS_HEADERS) tssutils.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC tssutils.c
#tsssocket.o: 	$(TSS_HEADERS) tsssocket.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC tsssocket.c
#tssdev.o: 	$(TSS_HEADERS) tssdev.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC tssdev.c
#tsstransmit.o: 	$(TSS_HEADERS) tsstransmit.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC tsstransmit.c
#tssresponsecode.o: $(TSS_HEADERS) tssresponsecode.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC tssresponsecode.c
#tssccattributes.o: $(TSS_HEADERS) tssccattributes.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC tssccattributes.c
#fail.o: 	$(TSS_HEADERS) fail.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC fail.c
#tssprint.o: 	$(TSS_HEADERS) tssprint.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC tssprint.c
#Unmarshal.o: 	$(TSS_HEADERS) Unmarshal.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC Unmarshal.c
#Commands.o: 	$(TSS_HEADERS) Commands.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC Commands.c
#CommandAttributeData.o: 	$(TSS_HEADERS) CommandAttributeData.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC CommandAttributeData.c
#CpriHash.o: 	$(TSS_HEADERS) CpriHash.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC CpriHash.c
#CpriSym.o: 	$(TSS_HEADERS) CpriSym.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC CpriSym.c
#GetCommandCodeString.o:		$(TSE_HEADERS) GetCommandCodeString.c
#		$(CC) $(CCFLAGS) $(CCLFLAGS) -fPIC GetCommandCodeString.c
#
# TSS shared library build

$(LIBTSS): 	$(TSS_OBJS)
		$(CC) $(LNFLAGS) $(LNLFLAGS) -shared -o $(LIBTSS) $(TSS_OBJS)

.PHONY:		clean
.PRECIOUS:	%.o

clean:		
		rm -f *.o  *~ 	\
		../src/*.o	\
		h*.bin		\
		$(ALL)

# applications

#helper:			helper.o rpmb.o libtss.so
#			$(CC) $(LNFLAGS) $(LNAFLAGS) helper.o rpmb.o $(LNALIBS) -o helper


#t:		t.o
		$(CC) $(LNFLAGS) $(LNAFLAGS) t.o $(LNALIBS) -o t

# for applications, not for TSS library

%.o:		%.c tss2/tss.h 
		$(CC) $(CCFLAGS) $(CCAFLAGS) $< -o $@

