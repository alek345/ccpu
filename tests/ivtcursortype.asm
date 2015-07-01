; set cursor pos
LDAC 1
LDXC 0
LDYC 0
INT 1

; set color
LDAC 2
LDXC 0x07
INT 1

; wait for key - return in X
:loop
LDXC 1
INT 0

; print key, jump to loop
LDAC 3
INT 1

JMPL loop
