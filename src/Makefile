include config.${ARCH}
OBJS_DIR=../${TEMP}/zinjai
CXXFLAGS+=${WX_CXXFLAGS}
LDFLAGS+=${WX_LDFLAGS}

ifdef WINDOWS
ICON=${OBJS_DIR}/icon.o
SARCH=${ARCH}
BIN=.
else
SARCH=`../src_extras/get-arch.sh`
ICON=
endif


all: ${OBJS_DIR} ../${BIN}/zinjai${EXT}

ifdef WINDOWS
${ICON}: zinjai.rc ../imgs/zinjai.ico ../imgs/filetype_*.ico manifest.xml
	${WINDRES} -I${WX_DIR}/include -i zinjai.rc -o $@
endif

include Makefile.common

clean-bin:
	${RM} ../${BIN}/zinjai${EXT} ../${BIN}/zinjai-${SARCH}.dbg ${ICON}

clean: clean-bin
	${RM} ${OBJS}

../${BIN}/zinjai${EXT}: ../${BIN} ${OBJS} ${ICON}
	${GPP} ${OBJS} ${LDFLAGS} -o $@
	${OBJCOPY} --only-keep-debug $@ ../${BIN}/zinjai-${SARCH}.dbg
	${STRIP} $@
	${OBJCOPY} --add-gnu-debuglink=../${BIN}/zinjai-${SARCH}.dbg $@

${OBJS_DIR}:
	${MKDIR} ${OBJS_DIR}

