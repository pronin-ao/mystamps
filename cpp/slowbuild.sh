echo '>> qmake'
~/Qt/5.15.2/android/bin/qmake
read -p '>> make'
make -j8
read -p '>> make apk'
make apk
