#!/bin/bash

# Script for compiling and packing zinjai's linux releases (meant to be run in a virtual machine).
# Source package must exists in the real machine before starting (run src_extras/pack.sh src).
# Pass the version number to this script to generate the release file (compile and compress result).

function get_arch {
  if uname -a | grep x86_64 >/dev/null; then echo l64; else echo l32; fi
}

if ! test -e ./zinjai-packer.cfg; then
	touch ./zinjai-packer.cfg
	echo '# usuario, ip, y ruta hacia el directorio donde se encuentran los fuentes' >> ./zinjai-packer.cfg
	echo '# de zinjai, y donde se va a copiar luego el resultado' >> ./zinjai-packer.cfg
	echo 'SSHSRC="usuario@10.0.2.4:/home/usuario/zinjai/dist"' >> ./zinjai-packer.cfg
	echo '' >> ./zinjai-packer.cfg
	echo '# tipo de arquitectura para la que se compila, que se usara para el nombre' >> ./zinjai-packer.cfg
	echo '# del paquete generado (l32 o l64)' >> ./zinjai-packer.cfg
	echo 'ARCH="'`get_arch`'"' >> ./zinjai-packer.cfg
	echo '' >> ./zinjai-packer.cfg
	echo '# directorio con archivos adicionales a incluir en el paquete (como los .so' >> ./zinjai-packer.cfg
	echo '# de wxWidgets, los binarios de gprof2dot, lizard, etc)' >> ./zinjai-packer.cfg
	echo 'EXTRAFILES="extra-zinjai"' >> ./zinjai-packer.cfg
	echo '' >> ./zinjai-packer.cfg
	echo '# puede modificar aquí otras variables si fuera necesario' >> ./zinjai-packer.cfg
	echo '#PATH=/home/usuario/wx/bin:$PATH' >> ./zinjai-packer.cfg
	echo '' >> ./zinjai-packer.cfg
	echo "Creado un archivo \"zinjai-packer.cfg\" de ejemplo. Verifique su contenido y'vuelva a ejecutar este script."
	exit 1
fi
source ./zinjai-packer.cfg
if [ "$1" == "" ]; then
	echo "Debe utilizar la version de zinjai (por ej: 20200428) como argumento de este script."
	exit 2
fi

if ! [ "$2" = "fast" ]; then
	TAR_OPTS=""
	rm -rf zinjai
else
	TAR_OPTS="--keep-new-files"
fi
scp $SSHSRC/zinjai-src-$1.tgz . || exit 3
tar $TAR_OPTS -xzvf zinjai-src-$1.tgz

if ! [ "$EXTRAFILES" = "" ]; then 	
	mkdir -p zinjai/
	cp -rf "$EXTRAFILES"/* zinjai/
fi

cd zinjai
src_extras/pack.sh $ARCH
scp dist/zinjai-$ARCH-$1.tgz $SSHSRC/zinjai-$ARCH-$1.tgz

