#!/bin/bash

#LOGFILE=$(pwd)/pack.log
#function error {
#	echo "Error: see $LOGFILE for details"
#	exit 1
#}

if ! [ -f zinjai.dir ]; then
	echo "Run this script from ZinjaI's root directory"
	exit 1
fi

VERSION=$(cat src/version.h |head -n 1|cut -d ' ' -f 3)

function pack_src {
	echo "=== compressing..."
	FNAME=dist/zinjai-src-$VERSION.tgz
	FLIST=$(git ls-tree -r master --name-only)
	tar -czvf $FNAME --transform 's,^,zinjai/,' $FLIST >/dev/null
	echo -n "===done: "
	ls -sh $FNAME
}

function pack_wine {
	clean
	make ARCH=wine$1
	ARCH=w$1
# 	rm -rf dist/pseint 
# 	mkdir -p dist/pseint
# 	rm -f bin/linux.txt
# 	rm -rf bin/bin 
# 	rm -rf bin/bin2 
# 	rm -rf bin/bin3
# 	cp -r bin/* dist/pseint/
# 	rm -f dist/pseint/config.here
# 	rm -f dist/pseint/lib
# 	rm -f dist/pseint/log.exe
# 	rm -f dist/pseint/updatem.exe
# 	cp dist/dlls$1/* dist/pseint/
# 	#find bin -type f | sed 's/bin/delete $$INSTDIR/' | sed 's/\//\\/g' > dist/uninstall.nsh
# 	#find bin -type d | sed 's/bin/rmdir $$INSTDIR/'  | sed 's/\//\\/g' | sort -r >>dist/uninstall.nsh
# 	#cd dist && wine c:/nsis/makensis.exe /DPRODUCT_VERSION\=`cat pseint/version` pseint.nsh
# 	cd dist
# 	wine C:/Inno/ISCC.exe installer-win$1.iss /DMyAppVersion=`cat pseint/version`
# 	cp config.here pseint/config.here
# 	#cp dlls/* pseint/
# 	rm -f pseint-${ARCH}-`cat pseint/version`.zip
# 	zip -qr -9 pseint-${ARCH}-`cat pseint/version`.zip pseint
# 	echo 
# 	echo -n "DONE: "
# 	cd ..
# 	ls -sh dist/pseint-${ARCH}-`cat bin/version`.*
# 	echo
}

function pack_lnx {
	echo "=== cleaning..."
	make clean-bin >/dev/null
	echo "=== compiling..."
	make ARCH=lnx -j 2 || exit 1

	echo "=== compressing..." 
	FNAME=dist/zinjai-l$1-$VERSION.tgz
	EXCLUDE="--exclude=imgs/src --exclude=guihelp/src"
	INCLUDE="autocomp colours guihelp lang parser samples templates toolchains zinjai.dir bin complements imgs libs readme.txt skins third zinjai"
	tar -czf $FNAME --transform 's,^,zinjai/,' $EXCLUDE $INCLUDE >/dev/null
	
	echo -n "=== done: "
	ls -sh $FNAME
}

# function pack_mac {
# 	clean
# 	ARCH=$1
# 	make ARCH=$ARCH || exit 1
# 	OSX_PREF=$2
# 	rm -rf dist/pseint.app
# 	mkdir -p dist/pseint.app
# 	mkdir -p dist/pseint.app/Contents
# 	mkdir -p dist/pseint.app/Contents/MacOS
# 	mkdir -p dist/pseint.app/Contents/Resources
# 	mkdir -p dist/pseint.app/Contents/Resources/Fonts
# 	cp -r bin/* dist/pseint.app/Contents/Resources
# 	mv dist/pseint.app/Contents/Resources/*.ttf dist/pseint.app/Contents/Resources/Fonts/
# 	mv dist/pseint.app/Contents/Resources/wxPSeInt dist/pseint.app/Contents/MacOS/pseint
# 	cp dist/Info.plist dist/pseint.app/Contents/
# 	cp bin/imgs/icon.icns dist/pseint.app/Contents/Resources/pseint.icns
# 	bash dist/fix_dylibs.sh	${OSX_PREF} dist/pseint.app
# 	tar -czf dist/pseint-${ARCH}-`cat bin/version`.tgz -C dist pseint.app
# 	echo 
# 	echo -n "DONE: "
# 	ls -sh dist/pseint-${ARCH}-`cat bin/version`.tgz
# 	echo
# }


if [ "$1" = "--noclean" ]; then
	CLEAN=0
else
	CLEAN=1
fi

if [ "$1" = "clean" ]; then
	clean
elif [ "$1" = "src" ]; then
# 	if [ "$CLEAN" = "1" ]; then clean; fi # no necesita limpiar, va a crear una copia fresca en otro lado
	pack_src
elif [ "$1" == "w32" ]; then
	if [ "$CLEAN" = "1" ]; then clean; fi
	pack_wine 32
elif [ "$1" == "w64" ]; then
	if [ "$CLEAN" = "1" ]; then clean; fi
	pack_wine 64
elif [ "$1" == "l32" ]; then
	if [ "$CLEAN" = "1" ]; then clean; fi
	pack_lnx 32
elif [ "$1" == "l64" ]; then
	if [ "$CLEAN" = "1" ]; then clean; fi
	pack_lnx 64
# elif [ "$1" == "mi64" ]; then
# 	if [ "$CLEAN" = "1" ]; then clean; fi
# 	pack_mac mi64 x86_64-apple-darwin20.4
# elif [ "$1" == "ma64" ]; then
# 	if [ "$CLEAN" = "1" ]; then clean; fi
# 	pack_mac ma64 arm64-apple-darwin20.4
else
	echo "usar: $0 [--noclean] TARGET"
	echo "target can be:  "
	echo "  - src: source code packed in a tgz file"
	echo "  - w32/w64: 32/64 bits windows installer (exe generated with wine32/wine64+mingw64+inno setup), and portable (zip) version"
	echo "  - l32/l64: GNU/Linux versions, compiled natively on current system/architecture"
#	echo "  - mi64/ma64: tgz package for MacOS systems based on intel x86_64/arm64 hardware (cross compiled with osxcross)"
fi
