echo '>> qmake'
~/Qt/5.15.2/android/bin/qmake
echo  '>> make'
make -j8
echo '>> make apk'
make apk
