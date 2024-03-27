#this script was a test to pack all icons in a single file to reduce load time
#this solution was discarded, the loading time was not there

cd imgs
ls *.png --color=never > bitmaps.idx
cat *.png > bitmaps.pack

cd 16
ls *.png --color=never > bitmaps.idx
cat *.png > bitmaps.pack
cd ..

cd 24
ls *.png --color=never > bitmaps.idx
cat *.png > bitmaps.pack
cd ..

cd 32
ls *.png --color=never > bitmaps.idx
cat *.png > bitmaps.pack
cd ..
