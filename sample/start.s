.section .text.init
.global _start

_start:
    call main
    call _start
