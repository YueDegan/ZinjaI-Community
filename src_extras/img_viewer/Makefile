include config.${ARCH}
OBJS_DIR=../../${TEMP}/img_viewer
CXXFLAGS+=${WX_CXXFLAGS}
LDFLAGS+=${WX_LDFLAGS}

all: ../../${BIN}/img_viewer${EXT}

include Makefile.common

clean-bin:
	${RM} ../../${BIN}/img_viewer${EXT}

clean: clean-bin
	${RM} ${OBJS}

../../${BIN}/img_viewer${EXT}: ${OBJS_DIR} ${OBJS}
	${GPP} ${OBJS} ${LDFLAGS} -o $@

${OBJS_DIR}:
	mkdir -p ${OBJS_DIR}
