.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 is the pointer to the start of the vector
#	a1 is the # of elements in the vector
# Returns:
#	a0 is the first index of the largest element
# =================================================================
argmax:

    # Prologue

    add t0 x0 x0 # counter
    add t1 x0 x0 # index
    add t2 x0 x0 # max ele
loop_start:
    beq t0 a1 loop_end

    slli t0 t0 2
    add t3 t0 a0 # t3: address of current ele
    srli t0 t0 2
    lw t4 0(t3) # t4: current ele

    slt t5 t2 t4 # t5 = 1 if current > max
    bne t5 x0 loop_continue
    addi t0 t0 1
    jal x0 loop_start

loop_continue:
    mv t1 t0
    mv t2 t4
    addi t0 t0 1
    jal x0 loop_start

loop_end:
    add a0 t1 x0
    # Epilogue


    ret
