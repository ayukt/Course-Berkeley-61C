    addi t0 x0 1000
    addi s0 s0 5
    addi s1 s1 10
    beq x0 x0 label
    sw s0 0(t0)
label:
    sw s1 0(t0)