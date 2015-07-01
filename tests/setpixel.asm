; set a specific pixel
; x - 10, y - 5

; pos = (x + (y * w)) * 2
; y * w

LDAC 5
; this should be 80, but this thing has to somehow be multiplied by two and this is the easiest way
LDXC 160
SETD result
MUAXM

; x + (y * w)
LDAC 20

SETD result
LDXM
INCD
LDYM

LDHA 0x80
LDLA 0x00
ADDXY
ADDA

LDXC 0x40
STRX
INCD
LDXC 0x07
STRX

:hlt
JMPL hlt

:result
NOP
NOP
