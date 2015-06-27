; ccpu rom
; resides at 0x100-0x1FF

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

; IVT 0h - Mainly keyboard routines
:ivt_0h
LDAC 0x1
CMPAX
SETD ivt_0h_x1
JE

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
