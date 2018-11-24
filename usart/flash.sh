flash() {
    ./stlink/build/Release/st-flash write ${PWD}/build/*.bin 0x8000000
}

if [ -x "./stlink/build/Release/st-flash" ]; then
    flash
elif [ -f "./stlink/Makefile" ]; then
    cd ./stlink/ && make -j4 release && cd - && flash
else
    echo "ERR: stlink is updated as submodule . Check README steps." >&2
fi