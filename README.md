# Fly Fighters

Author: Zhengjia Cao

Design: Control your plane. Shoot the enemy planes and hide from their missles. Do not let them cross the bottom line.

Screen Shot:

![37a17daf08ac6ff4b5c6ee8b8db65fd1.png](https://www.imgge.com/images/2021/09/13/37a17daf08ac6ff4b5c6ee8b8db65fd1.png)

How Your Asset Pipeline Works:

The asset pipeline is inspired by https://github.com/xuxiaoqiao/15-466-f20-base1/blob/HEAD/asset_pipe_converter.cpp. The design is used with modification.

The source files are put under the directory `/assets/`. The `./build_tools_bin/asset_convert` program will convert the png file format into chunks using `load_save_png` function and `read_write_chunk.cpp` file. It reads the asset files under the asset directory, and will generate a `asset_res.h` file under the asset directory. The purpose of the file is to generate a mapping from the asset being used and the index its being stored. The palette pattern and the tile patterns are stored internally as two vectors, and externally under 2 seperated `.chunk` file under the `dist` diretory. Only these two files and the game executables are needed at distribution. 

Since the PPU466 unit only use two bits to index the color, any `.png` file in the asset directory cannnot have more colors than this without modifying the `drawing` function. If the file uses less than 4 colors, zero will be padded and stored to the chunk. 

How To Play:

Use the `left/right` arrow key to control the plane movement. Use the `space` key to shoot your enemies. They will never be cleared. Don't let them cross the bottom line and keep an eye on your health. Maximum score for the game is 999.

Known Issues:
- Plane lags sometime. You can see the past plane trajactory on the screen sometimes. 

Sources: 

https://github.com/xuxiaoqiao/15-466-f20-base1/

Assets created by Zhengjia Cao (ccjeff) & Zheyuan Zhou (Ajar)

This game was built with [NEST](NEST.md).

