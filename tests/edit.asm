; I once made a simple x86 program with bios interupts called edit, and all it did was let you
; use the entire screen for text editing

; make sure color and cursor is right
LDAC 1
LDXC 0
LDYC 0
INT 1

LDAC 2
LDXC 0x07
INT 1

:loop
LDXC 1
INT 0

LDAC 8
CMPAX
SETD key_backspace
JE

LDAC 13
CMPAX
SETD key_enter
JE

LDAC 82
CMPAX
SETD key_up
JE

LDAC 81
CMPAX
SETD key_down
JE

LDAC 80
CMPAX
SETD key_left
JE

LDAC 79
CMPAX
SETD key_right
JE

; no special key, just print it
LDAC 3
INT 1
JMPL loop

:key_backspace
LDHA 0x00
LDLA 0x24
LDAM

LDXC 0
CMPAX
SETD loop
JE

DECA
LDHA 0x00
LDLA 0x24
STRA

LDAC 6
LDXC 0
INT 1

JMPL loop

:key_enter
LDHA 0x00
LDLA 0x25
LDAM

LDXC 29
CMPAX
SETD loop
JE

INCA
LDHA 0x00
LDLA 0x25
STRA

LDAC 0
LDHA 0x00
LDLA 0x24
STRA

JMPL loop

:key_up
LDHA 0x00
LDLA 0x25
LDAM

LDXC 0
CMPAX
SETD loop
JE

DECA
LDHA 0x00
LDLA 0x25
STRA

JMPL loop

:key_down
LDHA 0x00
LDLA 0x25
LDAM

LDXC 29
CMPAX
SETD loop
JE

INCA
LDHA 0x00
LDLA 0x25
STRA

JMPL loop

:key_left
LDHA 0x00
LDLA 0x24
LDAM

LDXC 0
CMPAX
SETD loop
JE

DECA
LDHA 0x00
LDLA 0x24
STRA
JMPL loop

:key_right
LDHA 0x00
LDLA 0x24
LDAM

LDXC 79
CMPAX
SETD loop
JE

INCA
LDHA 0x00
LDLA 0x24
STRA
JMPL loop
