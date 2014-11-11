#!/bin/sh

dest="$HOME/Library/Developer/Xcode/Templates/Audio Unit"

rm -rf "$dest"
cp -a "Audio Unit" "$dest"
