rm -rf build && mkdir build && cd build && cmake .. \
        && cmake --build . && cmake --build . --target update_translations
