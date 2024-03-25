include config.${ARCH}
MAKE_ARGS=-f Makefile ARCH=${ARCH} -I ../../configs

ifdef WINDOWS
EXTRA_BINS=../${BIN}/shellexecute.exe ../${BIN}/lsdeps.exe ../log.exe ../${TEMP}/xmlres.o
EXT_FLAGS=-static-libgcc -static-libstdc++
WSOCK=-lwsock32
else
EXTRA_BINS=../zinjai
WSOCK
endif

all: ../${BIN}/updatem${EXT} ../${BIN}/runner${EXT} ${EXTRA_BINS}
	$(MAKE) -C complement ${MAKE_ARGS}
	$(MAKE) -C img_viewer ${MAKE_ARGS} 

clean: clean-bin
	
clean-bin:
	${RM} ../${BIN}/updatem${EXT} ../${BIN}/runner${EXT} ${EXTRA_BINS}
	$(MAKE) -C complement ${MAKE_ARGS} clean-bin
	$(MAKE) -C img_viewer ${MAKE_ARGS} clean-bin

	
../${BIN}/runner${EXT}: runner.cpp
	${GPP} ${CXXFLAGS} runner.cpp -o $@

../${BIN}/updatem${EXT}: updatem/updatem.cpp updatem/zockets.cpp updatem/zockets.h
	${GPP} ${CXXFLAGS} updatem/updatem.cpp updatem/zockets.cpp ${WSOCK} -o $@

ifdef WINDOWS
../${TEMP}/xmlres.o: manifest.xml xmlres.rc
	${WINDRES} -i xmlres.rc -o $@

../${BIN}/lsdeps.exe: lsdeps/lsdeps.cpp lsdeps/itlib.cpp lsdeps/itlib.h lsdeps/peliberr.cpp lsdeps/peliberr.h lsdeps/stdafx.h
	${GPP} ${CXXFLAGS} -static-libgcc -O2 lsdeps/lsdeps.cpp lsdeps/itlib.cpp lsdeps/peliberr.cpp  -o $@

../${BIN}/shellexecute.exe: shellexecute.cpp
	${GPP} ${CXXFLAGS} -mwindows shellexecute.cpp -o $@

../log.exe: log-w32.cpp
	${GPP} ${EXT_FLAGS} -mwindows log-w32.cpp -o $@
else
../zinjai: launcher.cpp
	${GPP} ${CXXFLAGS} launcher.cpp -O2 -ldl -o $@
endif
