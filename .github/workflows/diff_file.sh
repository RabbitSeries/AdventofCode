function diff_file {
    if [ -z $1 ] || [ -z $2 ]; then
        echo "[ERROR] Parameter error"
        exit -1
    fi
    if [ -e $1 ] && [ -e $2 ] && diff -u $1 $2; then
        echo "[SUCCESS] All pass"
    else
        echo "[ERROR] Files differ. See diff.log for details."
        exit -1
    fi
    exit 0
}
# } > diff.log

diff_file $1 $2