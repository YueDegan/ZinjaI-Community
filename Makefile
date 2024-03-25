ifdef ARCH

MAKE_ARGS=-I../configs -f Makefile ARCH=${ARCH}

all: 
	${MAKE} -C src         ${MAKE_ARGS}
	${MAKE} -C parser      ${MAKE_ARGS}
	${MAKE} -C src_extras  ${MAKE_ARGS}

clean: 
	${MAKE} -C src         ${MAKE_ARGS} clean
	${MAKE} -C parser      ${MAKE_ARGS} clean
	${MAKE} -C src_extras  ${MAKE_ARGS} clean

clean-bin: 
	${MAKE} -C src         ${MAKE_ARGS} clean-bin
	${MAKE} -C parser      ${MAKE_ARGS} clean-bin
	${MAKE} -C src_extras  ${MAKE_ARGS} clean-bin

else
all:
	@echo " "
	@echo "Para compilar directo (en un sistema para ese mismo sistema), usar:"
	@echo "   make ARCH=lnx             para compilar todo en GNU/Linux" # o macOS"
	@echo "   mingw32-make ARCH=w32     para compilar todo en Microsoft Windows"
	@echo " "
	@echo "Para compilar cruzado (en GNU/Linux, para otro sistema), usar:"
	@echo "   make ARCH=wine??          para compilar todo para Microsoft Windows con MinGW64"
	@echo "                             sobre wine?? (donde ?? debe ser 32 o 64)."
#	@echo "   make ARCH=m?64            para compilar todo para macOS con osxcross, (donde"
#	@echo "                             ? debe ser i para x86_64, y a para arm64)"
	@echo " "
#	@echo "En caso de error, o para mas informacion, consultar la documentación"
#	@echo "adicional del directorio \"doc\"."
#	@echo " "

endif

