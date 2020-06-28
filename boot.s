
.option norvc
.section data
.section .text.init

.global _entry
_entry:
    csrr    t0, mhartid
    bnez    t0, _hang
    csrw    satp, zero
.option push
.option norelax
    la      gp, _global_pointer
.option pop
    la      a0, _bss_start
    la      a1, _bss_end
1:
    bgeu    a0, a1, 2f
    sd      zero, (a0)
    addi    a0, a0, 8
    j   1b
2:
    la      sp, _stack
    li		t0, (0b11 << 11) | (1 << 7) | (1 << 3)
    csrw    mstatus, t0
    la      t1, kmain
    csrw    mepc, t1
    la      t2, _trap_vector
    csrw    mtvec, t2
    li      t3, (1 << 3) | (1 << 7) | (1 << 11)
    csrw    mie, t3
    la      ra, _hang
    mret

    
_hang:
    wfi
    j _hang

.global _trap_vector
_trap_vector:
    mret

