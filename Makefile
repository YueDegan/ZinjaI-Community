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
	@echo "   make ARCH=wine??          para compilar todo para Microsoft Windows con"
	@echo "                             MinGW64 sobre wine?? (donde ?? debe ser 32 o 64)."
#	@echo "   make ARCH=m?64            para compilar todo para macOS con osxcross, (donde"
#	@echo "                             ? debe ser i para x86_64, y a para arm64)"
	@echo " "
	@echo "Los targets disponibles en todos los makefiles son: all clean clean-bin."
	@echo "clean y clean-bin se pueden correr sin especificar arquitectura (equivale"
	@echo "a wine32, wine64 y lnx)"
	@echo " "
#	@echo "En caso de error, o para mas informacion, consultar la documentación"
#	@echo "adicional del directorio \"doc\"."
#	@echo " "

clean:
	${MAKE} ARCH=wine32 clean
	${MAKE} ARCH=wine64 clean
	${MAKE} ARCH=lnx clean

clean-bin:
	${MAKE} ARCH=wine32 clean-bin
	${MAKE} ARCH=wine64 clean-bin
	${MAKE} ARCH=lnx clean-bin
	
endif

