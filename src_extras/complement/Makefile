include config.${ARCH}
OBJS_DIR=../../${TEMP}/complement
CXXFLAGS+=${WX_CXXFLAGS}
LDFLAGS+=${WX_LDFLAGS}

all: ${OBJS_DIR} ../../${BIN}/complement${EXT} ../../${BIN}/complement_wrap${EXT}

include Makefile.common

clean-bin:
	${RM} ../../${BIN}/complement${EXT} ../../${BIN}/complement_wrap${EXT}

clean: clean-bin
	${RM} ${OBJS}

../../${BIN}/complement${EXT}: ${OBJS_DIR} ${OBJS}
	${GPP} ${OBJS} ${LDFLAGS} -o $@

../../${BIN}/complement_wrap${EXT}: complement_wrap.cpp
	${GPP} complement_wrap.cpp -O2 -o $@

${OBJS_DIR}:
	${MKDIR} ${OBJS_DIR}
