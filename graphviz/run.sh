#!/bin/bash


gen_png()
{
name=$1
out=$name.png
dot  -Tpng $name > $out
}

gen_png tile_explain_dfs.txt
gen_png tile_explain_bfs.txt
gen_png tile.txt
gen_png tile1.txt
gen_png tile2.txt
