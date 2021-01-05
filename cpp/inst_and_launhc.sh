echo '>> Check devices'
./check.sh
read -p '>> install'
./_inst.sh
read -p '>> run'
./_run.sh

