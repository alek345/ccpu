; colors - put out a bunch of colors on the screen

LDYC 0x1
LDXC 0x0

:loop
LDHA 0x80
LDLA 0x00
ADDY
STRX
INCX
INCY
INCY

JMPL loop
