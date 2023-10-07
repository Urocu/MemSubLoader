
# MemSubLoader
<p align="center">
  <img width="128" height="128" src="https://raw.githubusercontent.com/senolem/MemSubLoader/main/logo/logo.png">
</p>

MemSubLoader is a software that reads memory and loads subtitles for corresponding audio. It's made for old games that don't have any subtitles or when it's hard to edit them.

## How it works

The tool first reads everything from a .txt file. It contains:
1. base address and offets that lead to address with audio ID
2. Text for subtitles
 
After that it reads memory to check which dialog it should load.

MemSubLoader is really limited right now and it will be updated with better and new features in the future.
