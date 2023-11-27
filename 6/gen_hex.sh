for png_path in tests/*.png; do
    hex_path=$(echo $png_path | sed "s/.png//g")
    ./png2hex < "$png_path" > "$hex_path"
done
