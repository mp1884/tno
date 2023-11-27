for png_path in tests/*.png; do
    hex_path=$(echo $png_path | sed "s/.png//g")
    exp_ans=$(echo $png_path | grep -Po '^tests\/.[0-9]+_\K[0-9]+')
    ans=$(./task < "$hex_path")
    if [ "$exp_ans" != "$ans" ]; then
        printf "%s failed: expected %d, found %d\n" "$hex_path" "$exp_ans" "$ans"
        exit 1
    fi
    printf "%s ok\n" "$hex_path"
done
