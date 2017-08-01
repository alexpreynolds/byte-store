SHELL           := /bin/bash
CC               = gcc
BLDFLAGS         = -Wall -Wextra -Wno-unused-function -std=c99
BLDDFLAGS        = -Wall -Wextra -Wno-unused-function -std=c99 -pedantic
CFLAGS           = -D__USE_POSIX -D__STDC_CONSTANT_MACROS -D__STDINT_MACROS -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE=1 -O3 -ffast-math -march=native
CDFLAGS          = -D__USE_POSIX -D__STDC_CONSTANT_MACROS -D__STDINT_MACROS -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE=1 -DDEBUG=1 -O
LIBS             = -lm -lbz2 -lmicrohttpd
PLATFORM        := $(shell uname -s)
INCLUDES         = /usr/include
THIRD_PARTY      = $(PWD)/third-party
BZIP2_ARC        = $(THIRD_PARTY)/bzip2-1.0.6.tar.gz
BZIP2_DIR        = $(THIRD_PARTY)/bzip2-1.0.6
BZIP2_SYM_DIR    = $(THIRD_PARTY)/bzip2
BZIP2_INC_DIR    = $(BZIP2_SYM_DIR)
BZIP2_LIB_DIR    = $(BZIP2_SYM_DIR)
HTTPD_ARC        = $(THIRD_PARTY)/libmicrohttpd-0.9.53.tar.gz
HTTPD_DIR        = $(THIRD_PARTY)/libmicrohttpd-0.9.53
HTTPD_SYM_DIR    = $(THIRD_PARTY)/libmicrohttpd
HTTPD_INC_DIR    = $(HTTPD_DIR)/include
HTTPD_LIB_DIR    = $(HTTPD_DIR)/lib

.PHONY           = test third-party

# -----------------------------------------
#  - OS X Clang can't build static binaries
#
#  - CentOS 7 does not seem to offer static 
#    builds of gnutls and gcrypt. Bummer!
# -----------------------------------------

ifeq ($(PLATFORM),Darwin)
	CC = clang
	CXX = clang++
	FLAGS += -Weverything
endif
ifeq ($(PLATFORM), Linux)
	CFLAGS += -pthread 
	CDFLAGS += -pthread
	LIBS += -lrt
	INCLUDES += -I/usr/include/gnutls -I/usr/include/nettle
	LIB64 = -L"/usr/lib64"
endif

# ------------------------------------------
# To compile and run byte-store on an HPC 
# host, first add the following modules:
#
#   $ module add gcc
#   $ module add glibc
#   $ module add gnutls
#   $ module add libgcrypt
#
# Then run make:
#
#   $ make all
#
# These modules include the necessary headers 
# and libraries required for compilation and
# execution of the byte-store binary.
# ------------------------------------------

all: byte-store

# -----------
# Application
# -----------

prep: bzip2 libmicrohttpd

bzip2:
	@if [ ! -d "${BZIP2_DIR}" ]; then \
		mkdir "${BZIP2_DIR}"; \
		tar zxvf "${BZIP2_ARC}" -C "${THIRD_PARTY}"; \
		ln -sf ${BZIP2_DIR} ${BZIP2_SYM_DIR}; \
		${MAKE} -C ${BZIP2_SYM_DIR} libbz2.a CC=${CC}; \
	fi

# -------------------------------------------------------
# SSL support requires libgnutls and libgcrypt, which can
# be installed on OS X via Homebrew:
#
#  $ brew install libgnutls
#  $ brew install libgcrypt
# 
# Because of SIP protections in place with OS X 10.10
# and later, we can't make symbolic links in /usr/lib and
# so we specify a custom prefix for OS X.
#
# On CentOS 7, these can be installed via:
#
#  $ sudo yum install gnutls-devel
#  $ sudo yum install libgcrypt-devel
#
# However, these are dynamic libraries, not static, which
# means we cannot build static binaries on Linux at this
# time.
#
# This means the resulting libmicrohttpd shared library 
# may need to be put someplace safe from a 'make clean'
# on this folder. Also, this library's parent folder will
# need to be added to the LD_LIBRARY_PATH variable.
#
# Dynamic binaries are the sux0r!
# -------------------------------------------------------

libmicrohttpd:
	@if [ ! -d "${HTTPD_DIR}" ]; then \
		mkdir "${HTTPD_DIR}"; \
		tar zxvf "${HTTPD_ARC}" -C "${THIRD_PARTY}"; \
		ln -sf ${HTTPD_DIR} ${HTTPD_SYM_DIR}; \
		cd ${HTTPD_SYM_DIR}; \
		if [[ "$(PLATFORM)" == "Linux" ]]; then \
			export CPPFLAGS='-I/usr/include'; \
			export LDFLAGS='-L/usr/lib64'; \
			./configure --enable-https=yes --with-gnutls --prefix=${HTTPD_SYM_DIR}; \
		elif [[ "$(PLATFORM)" == "Darwin" ]]; then \
			./configure --enable-https=yes --with-libgcrypt-prefix=/usr/local --with-gnutls=/usr/local --prefix=${HTTPD_SYM_DIR}; \
		fi; \
		${MAKE} && ${MAKE} install; \
		cd ${PWD}; \
	fi

byte-store: prep
	$(CC) $(BLDFLAGS) $(CFLAGS) -c mt19937.c -o mt19937.o
	$(AR) rcs mt19937.a mt19937.o
	$(CC) $(BLDFLAGS) $(CFLAGS) -I${BZIP2_INC_DIR} -I${HTTPD_INC_DIR} -c byte-store.c -o byte-store.o
	$(CC) $(BLDFLAGS) $(CFLAGS) -I$(INCLUDES) -I${BZIP2_INC_DIR} -I${HTTPD_INC_DIR} $(LIB64) -L"${BZIP2_LIB_DIR}" -L"${HTTPD_LIB_DIR}" byte-store.o -o byte-store mt19937.a $(LIBS)

debug-byte-store: prep
	$(CC) -g $(BLDDFLAGS) $(CDFLAGS) -c mt19937.c -o mt19937.o
	$(AR) rcs mt19937.a mt19937.o
	$(CC) -g $(BLDDFLAGS) $(CDFLAGS) -I${BZIP2_INC_DIR} -I${HTTPD_INC_DIR} -c byte-store.c -o byte-store.o
	$(CC) -g $(BLDDFLAGS) $(CDFLAGS) -I$(INCLUDES) -I${BZIP2_INC_DIR} -L"${BZIP2_LIB_DIR}" -L"${HTTPD_LIB_DIR}" byte-store.o -o debug.byte-store mt19937.a $(LIBS)

clean:
	rm -f byte-store
	rm -f debug.byte-store
	rm -f *.a
	rm -f *.o
	rm -f *~
	rm -rf ${BZIP2_DIR}
	rm -rf ${BZIP2_SYM_DIR}
	rm -rf ${HTTPD_DIR}
	rm -rf ${HTTPD_SYM_DIR}
