; ccpu rom
; resides at 0xF000-0xFFFF

; This is rom entry, here the IVT is setup
:rom_entry
LDYC 0x0
SETD ivt_0h
LDXHD
LDHA 0x03
LDLA 0x00
STRX
INCY
SETD ivt_0h
LDXLD
LDHA 0x03
LDLA 0x00
ADDY
STRX
INCY

SETD ivt_1h
LDXHD
LDHA 0x03
LDLA 0x00
ADDY
STRX
INCY
SETD ivt_1h
LDXLD
LDHA 0x03
LDLA 0x00
ADDY
STRX
INCY

; Init video
JSRL ivt_1h_init

LDAC 0
LDXC 0
LDYX 0
; We're done jump to user at 0x1000
LDHA 0x10
LDLA 0x00
JMP

; In case we for some reason can't jump to 0x1000
:rom_entry_loop
HLT
JMPL rom_entry_loop

; IVT 1h - Video
:ivt_1h_init
LDXC 0
LDHA 0x00
LDLA 0x24
STRX
LDLA 0x25
STRX
LDLA 0x26
LDXC 0x07
STRX
RET

:ivt_1h
PUSHA
PUSHX
PUSHY
LDXC 1
CMPAX
SETD ivt_1h_a1
JE
LDXC 2
CMPAX
SETD ivt_1h_a2
JE
LDXC 3
CMPAX
SETD ivt_1h_a3
JE
LDXC 4
CMPAX
SETD ivt_1h_a4
JE
LDXC 5
CMPAX
SETD ivt_1h_a5
JE
LDXC 6
CMPAX
SETD ivt_1h_a6
JE

RET
:ivt_1h_hltloop
JMPL ivt_1h_hltloop

; IVT 1h - A=1 - Set x and y, X=x, Y=y
:ivt_1h_a1
POPY
POPX
POPA
LDHA 0x00
LDLA 0x24
STRX
LDHA 0x00
LDLA 0x25
STRY
RET

; IVT 1h - A=2 - Set color, X=color
:ivt_1h_a2
POPY
POPX
POPA
LDHA 0x00
LDLA 0x26
STRX
RET

; IVT 1h - A=3 - Print char at X, advance cursor
:ivt_1h_a3
JSRL ivt_1h_a6

PUSHA
PUSHX
PUSHY
JSRL ivt_1h_a5

RET

; IVT 1h - A=4 - X:Y=String ending with zero
:ivt_1h_a4
POPY
POPX
POPA

PUSHX
PUSHY

:loop
; fetch char
SETD stringoffset
LDAM
LDHA 0x00
LDLA 0x00
POPY
POPX
ADDXY
POPX
POPY
ADDA
LDXM
LDYX 0
CMPXY
SETD ivt_1h_a4_end
JE
PUSHX

LDHA 0x00
LDLA 0x24
LDAM
PUSHA
LDHA 0x00
LDLA 0x25
LDAM
LDXC 80
SETD ivt_1h_a4_temp
MUAXM
PUSHHD
PUSHLD
POPY
POPX
LDHA 0x80
LDLA 0x00
ADDXY
POPA
ADDA
POPX
STRX

:ivt_1h_a4_end
RET
:ivt_1h_a4_temp
NOP
NOP
:stringoffset
NOP

; IVT 1h - A=5 - Advance cursor by one
:ivt_1h_a5
POPY
POPX
POPA
LDHA 0x00
LDLA 0x24
LDXM
LDYC 79
CMPXY
SETD ivt_1h_a5_incy
JE

JMPL ivt_1h_a5_noincy

:ivt_1h_a5_incy
LDHA 0x00
LDLA 0x25
LDYM
INCY
LDXC 0
JMPL ivt_1h_a5_set

:ivt_1h_a5_noincy
LDHA 0x00
LDLA 0x25
LDYM
LDLA 0x24
LDXM
INCX

:ivt_1h_a5_set
LDAC 1
PUSHA
PUSHX
PUSHY
JSRL ivt_1h_a1

RET

; IVT 1h - A=6 - set char to X, using current color, do not advance cursor
:ivt_1h_a6
; pop register saved in interrupt caller
POPY
POPX
POPA
; save char in X for later use, pop into y because this is accessed before the color
PUSHX
POPY

; get and push current color aswell
LDHA 0x00
LDLA 0x26
LDXM
PUSHX

; saves the char
PUSHY

LDHA 0x00
LDLA 0x25
LDAM
LDXC 2
MUAX

LDXC 80
SETD ivt_1h_a3_result
MUAXM

LDHA 0x00
LDLA 0x24
LDAM
LDXC 2
MUAX

SETD ivt_1h_a3_result
LDXM
INCD
LDYM

LDHA 0x80
LDLA 0x00
ADDXY
ADDA

POPX
STRX
INCD
POPX
STRX

RET
:ivt_1h_a3_result
NOP
NOP

; IVT 0h - Mainly keyboard routines
:ivt_0h
LDAC 0x1
CMPAX
SETD ivt_0h_x1
JE

RET
:ivt_0h_hltloop
HLT
JMPL ivt_0h_hltloop

; IVT 0h - X=1 - Wait for key, return in x
:ivt_0h_x1
LDHA 0x00
LDLA 0x21
LDXM
LDAC 0
CMPAX
SETD ivt_0h_x1
JE
LDHA 0x00
LDLA 0x00
DECX
ADDX
LDXM
LDYC 0
CMPXY
SETD ivt_0h_x1
JE

:ivt_0h_x1_ret
LDHA 0x00
LDLA 0x21
LDAM
DECA
STRA
RET
