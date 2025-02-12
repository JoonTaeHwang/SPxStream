# Include configuration file if desired.
#
ifdef SPX_MAKE_CFG
include ../../SPxMakeCfg/spx-make-$(SPX_MAKE_CFG).txt
endif

#
# Set default architecture if not defined.
#
ifndef SPX_ARCH
    ifeq ($(shell getconf LONG_BIT),64)
        SPX_ARCH = x64
    else
        SPX_ARCH = x86
    endif
endif
ifeq ($(SPX_ARCH),x64)
    EXT = _64
    CC_OPTIONS += -m64
else
    CC_OPTIONS += -m32
endif

#
# Set default platform if not defined.
#
ifndef SPX_PLATFORM
    SPX_PLATFORM = linux
endif

#
# Define the tools we wish to use, unless already done.
#
CC = gcc
ifdef SPX_CC
    CC = $(SPX_CC)
endif
AR = ar
ifdef SPX_AR
	AR = $(SPX_AR)
endif
RM = rm -f
ifdef SPX_RM
	RM = $(SPX_RM)
endif

#
# Set default compiler options if not defined.
#
ifndef SPX_CC_OPTS
    SPX_CC_OPTS = -Wall -O3
endif

#
# Define the options for the tools.  The SPC_CC_* definitions allow
# additional options to be defined from the environment without having
# to edit this Makefile each time.
#
#	E.g. "export SPX_CC_OPTS=-g" to add debug.
#

# SPX 환경변수가 설정되지 않은 경우 기본값 설정
ifndef SPX
    SPX =..# /home/jt/WS/src
endif

CC_OPTIONS = $(SPX_CC_OPTS)
CC_DEFINES = $(SPX_CC_DEFS)
CC_INCLUDES = -I$(SPX)/Includes -I. -I../.. $(SPX_CC_INCS)

CC_FLAGS = $(CC_OPTIONS) $(CC_DEFINES) $(CC_INCLUDES)

#
# Define what we are actually building.
#
APPS = SPxDataStream SPxLiveStream SPxDataConverter

#
# Define what base files go into each app.
#
SPxDataStream_FILES = SPxDataStream.x
SPxLiveStream_FILES = SPxLiveStream.x
SPxDataConverter_FILES = SPxDataConverter.x

#
# From the list of base files, generate lists of source and object files for each app.
#
SPxDataStream_SRC = $(SPxDataStream_FILES:.x=.cpp)
SPxDataStream_OBJ = $(SPxDataStream_FILES:.x=.o)
SPxLiveStream_SRC = $(SPxLiveStream_FILES:.x=.cpp)
SPxLiveStream_OBJ = $(SPxLiveStream_FILES:.x=.o)
SPxDataConverter_SRC = $(SPxDataConverter_FILES:.x=.cpp)
SPxDataConverter_OBJ = $(SPxDataConverter_FILES:.x=.o)

SRC_FILES = $(SPxDataStream_SRC) $(SPxLiveStream_SRC) $(SPxDataConverter_SRC)
OBJ_FILES = $(SPxDataStream_OBJ) $(SPxLiveStream_OBJ) $(SPxDataConverter_OBJ)

#
# Set additional platform specific libraries to link with.
#
ifeq ($(SPX_PLATFORM),linux)
	EXTRA_LIBS = -lm -lpthread -lmxlin260$(EXT) -lusb -lrt -lstdc++
	# irc 라이브러리는 32비트 빌드에서만 필요
	ifeq ($(SPX_ARCH),x86)
		EXTRA_LIBS += -lirc
	endif
endif
ifeq ($(SPX_PLATFORM),sol-sparc)
	EXTRA_LIBS = -lm -lsocket -lnsl -lrt
ifeq ($(CC),gcc)
	EXTRA_LIBS += -lstdc++
endif
endif
ifeq ($(SPX_PLATFORM),sol-x86)
	EXTRA_LIBS = -lm -lsocket -lnsl -lrt
ifeq ($(CC),gcc)
	EXTRA_LIBS += -lstdc++
endif
endif
ifeq ($(SPX_PLATFORM),qnx-x86)
	EXTRA_LIBS = -lm -lsocket
endif

#
# Generic rules
#
.SUFFIXES: .cpp .o

# Rule to generate a .o from a .cpp, and add it to the archive.
.cpp.o:
	$(CC) $(CC_FLAGS) -c $<


#
# Define the default target to build all apps
#
all: $(APPS)

#
# Rules for building each app
#
SPxDataStream: $(SPxDataStream_OBJ) $(SPX)/Libs/$(SPX_PLATFORM)/libspx$(EXT).a
	$(CC) $(SPX_LINK_OPTS) -o $@ $(SPxDataStream_OBJ) \
	    -L$(SPX)/Libs/$(SPX_PLATFORM) -lspx$(EXT) $(EXTRA_LIBS) \
	    -lc -lz -lm -lpthread $(SPX_CC_LIBS)

SPxLiveStream: $(SPxLiveStream_OBJ) $(SPX)/Libs/$(SPX_PLATFORM)/libspx$(EXT).a
	$(CC) $(SPX_LINK_OPTS) -o $@ $(SPxLiveStream_OBJ) \
	    -L$(SPX)/Libs/$(SPX_PLATFORM) -lspx$(EXT) $(EXTRA_LIBS) \
	    -lc -lz -lm -lpthread $(SPX_CC_LIBS)

SPxDataConverter: $(SPxDataConverter_OBJ) $(SPX)/Libs/$(SPX_PLATFORM)/libspx$(EXT).a
	$(CC) $(SPX_LINK_OPTS) -o $@ $(SPxDataConverter_OBJ) \
	    -L$(SPX)/Libs/$(SPX_PLATFORM) -lspx$(EXT) $(EXTRA_LIBS) \
	    -lc -lz -lm -lpthread $(SPX_CC_LIBS)

#
# Define how to clean up at various levels.
#
# Basic 'clean' just removes the outputs of this build.
clean:
	$(RM) $(OBJ_FILES) $(APPS)

# distclean also removes unnecessary msvc files, backups etc. etc.
distclean:
	$(RM) *.vcproj.*.* *.ncb *.suo
	$(RM) *.cpp~ *.h~
	$(MAKE) clean

#
# Define automatic dependency listings.  To bootstrap this, do:
#	touch make.depend
#	make depend
#
.PHONY: depend
depend:
ifeq ($(SPX_PLATFORM),qnx-x86)
	$(RM) make.depend
	touch make.depend
else
	$(CC) $(CC_FLAGS) -MM $(SRC_FILES) > make.depend
endif

# Include the automatically generated dependency listings.
sinclude make.depend

#*********************************************************************
#
#	End of file
#
#*********************************************************************/
