LDAC 0
LDXC 0
LDYC 0

SETD videooffset
STRY

:loop
SETD string
ADDA
LDXM
INCA
LDYC 0
CMPXY
SETD end
JE
SETD videooffset
LDYM
LDHA 0x80
LDLA 0x00
ADDY
STRX
INCY
INCD
LDXC 0x07
STRX
INCY
SETD videooffset
STRY
JMPL loop

:end
PUSHA
POPX
LDYC 0
LDAC 1
INT 1

:loop_end
JMPL loop_end

:videooffset
NOP
:string
ASCIZ Welcome to ccpu!
