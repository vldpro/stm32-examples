if [ -x "$(which st-flash)" ]; then
    st-flash write build/*.bin 0x8000000 
else
    echo "ERR: st-flash is not installed. Check README steps." >&2
fi