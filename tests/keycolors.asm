; Ask user for a color
LDXC 1
INT 0

LDYC 0x30
LDAX
SUAY
SETD color
STRA

; color is at color
LDXC 0
LDAC 0
:keyloop
LDXC 1
INT 0
SETD offset
LDAM
LDHA 0x80
LDLA 0x00
ADDA
STRX
INCA
SETD color
LDXM
LDHA 0x80
LDLA 0x00
ADDA
INCA
STRX
SETD offset
STRA
JMPL keyloop

:color
NOP
:offset
