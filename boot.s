
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
    addi    sp, sp, -264
    sd      ra, 0(sp)
    sd      sp, 8(sp)
    sd      gp, 16(sp)
    sd      tp, 24(sp)
    sd      t0, 32(sp)
    sd      t1, 40(sp)
    sd      t2, 48(sp)
    sd      s0, 56(sp)
    sd      s1, 64(sp)
    sd      a0, 72(sp)
    sd      a1, 80(sp)
    sd      a2, 88(sp)
    sd      a3, 96(sp)
    sd      a4, 104(sp)
    sd      a5, 112(sp)
    sd      a6, 120(sp)
    sd      a7, 128(sp)
    sd      s2, 136(sp)
    sd      s3, 144(sp)
    sd      s4, 152(sp)
    sd      s5, 160(sp)
    sd      s6, 168(sp)
    sd      s7, 176(sp)
    sd      s8, 184(sp)
    sd      s9, 192(sp)
    sd      s10, 200(sp)
    sd      s11, 208(sp)
    sd      t3, 216(sp)
    sd      t4, 224(sp)
    sd      t5, 232(sp)
    sd      t6, 240(sp)
    csrr    a0, mcause
    add     a1, sp, 0
    csrr    a2, mepc
    sd      a2, 248(sp)
    call    generic_interrupt
    add     sp, a0, 0
    ld      a0, 248(sp)
    csrw    mepc, a0
    ld      ra, 0(sp)
    ld      sp, 8(sp)
    ld      gp, 16(sp)
    ld      t0, 32(sp)
    ld      t1, 40(sp)
    ld      t2, 48(sp)
    ld      s0, 56(sp)
    ld      s1, 64(sp)
    ld      a0, 72(sp)
    ld      a1, 80(sp)
    ld      a2, 88(sp)
    ld      a3, 96(sp)
    ld      a4, 104(sp)
    ld      a5, 112(sp)
    ld      a6, 120(sp)
    ld      a7, 128(sp)
    ld      s2, 136(sp)
    ld      s3, 144(sp)
    ld      s4, 152(sp)
    ld      s5, 160(sp)
    ld      s6, 168(sp)
    ld      s7, 176(sp)
    ld      s8, 184(sp)
    ld      s9, 192(sp)
    ld      s10, 200(sp)
    ld      s11, 208(sp)
    ld      t3, 216(sp)
    ld      t4, 224(sp)
    ld      t5, 232(sp)
    ld      t6, 240(sp)

    addi    sp, sp, 264

    mret

