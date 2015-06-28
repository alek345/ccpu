; colors - put out a bunch of colors on the screen

LDYC 0x0
LDXC 0x0

:loop
LDHA 0x80
LDLA 0x00
ADDY
LDAC 176
STRA
INCD
STRX
INCX
INCY
INCY

JMPL loop
