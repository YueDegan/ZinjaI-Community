include config.${ARCH}
OBJS_DIR=../${TEMP}/parser
#todo: analizar esto warnings o actualizar/reemplazar cbrowser por algo mejor
SUPRESS_WARNINGS=-Wno-implicit-function-declaration -Wno-builtin-declaration-mismatch
CFLAGS+=-DZASKAR -Dpanic=printf -Dckfree=free -Dckalloc=malloc -Dckrealloc=realloc -DHAVE_CONFIG_H -Ihyper -Icommon -Imisc -Icpp/cpplib -I. ${SUPRESS_WARNINGS}
LDFLAGS=-s

all: ${OBJS_DIR} ../bin/cbrowser${EXT}

include Makefile.common

${OBJS_DIR}:
	${MKDIR} ${OBJS_DIR}

../bin/cbrowser${EXT}: ../${BIN} ${OBJS}
	${GPP} ${OBJS} ${LDFLAGS} -o $@

clean-bin:
	${RM} ../${BIN}/cbrowser${EXT}

clean: clean-bin
	${RM} ${OBJS}
