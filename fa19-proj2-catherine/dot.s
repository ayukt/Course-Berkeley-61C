.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 is the pointer to the start of v0
#   a1 is the pointer to the start of v1
#   a2 is the length of the vectors
#   a3 is the stride of v0
#   a4 is the stride of v1
# Returns:
#   a0 is the dot product of v0 and v1
# =======================================================
dot:

    # Prologue
    add t0 x0 x0 # counter
    add t3 x0 x0 # result
loop_start:
    beq t0 a2 loop_end
    mv t1 t0
    mv t2 t0
    slli t1 t1 2
    slli t2 t2 2
    mul t1 t1 a3
    mul t2 t2 a4 # two offsets
    add t1 t1 a0
    add t2 t2 a1 # two addresses
    lw t4 0(t1)
    lw t5 0(t2)
    mul t4 t4 t5
    add t3 t3 t4
    addi t0 t0 1
    j loop_start

loop_end:
    mv a0 t3
    # Epilogue


    ret
