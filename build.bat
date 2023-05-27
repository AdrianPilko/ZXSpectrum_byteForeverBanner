set PATH=PATH;e:\z88dk\;e:\z88dk\bin;e:\z88dk\lib\config\
cd C:\z88dk\ZXSpectrum_byteForeverBanner
del main.tap

zcc +zx -vn -m -startup=31 -clib=sdcc_iy main.c -o main -create-app -lm

if exist main.tap (
  call main.tap
  exit
) else (
  pause
)
