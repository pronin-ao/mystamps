export ANDROID_NDK_ROOT=~/Android/Ndk/android-ndk-r20b/
export ANDROID_NDK_PLATFORM=android-27
export ANDROID_SDK_ROOT=~/Android/Sdk/

pyqtdeploy-sysroot --verbose --target 'android-32' --qmake '/home/pronin-ao/Qt/5.15.2/android/bin/qmake' --python py3 sysroot.toml
read -p '>> sysroot was built'

pyqtdeploy-build pystamps.pdt --verbose --target android-32 --qmake '/home/pronin-ao/Qt/5.15.2/android/bin/qmake' --python /usr/bin/python3.7
read -p '>> pydeploy-build finished'

cd build-android-32
~/Qt/5.15.2/android/bin/qmake
read -p '>> qmake finished'
make
read -p '>> make finished'
make apk
