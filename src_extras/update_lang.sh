#run this script from zinjai's root when you add/modify some translation ID to
regenerate src/lang_index_macros.h and launch mxLangTool to complete english translation
export LANG=en_US.ISO8859-1
if [ "$1" = "" ]; then
lang/tools/extract_enum.bin src/*.cpp src/mxCommonPopup.h
fi
lang/tools/mxLangTool.bin
