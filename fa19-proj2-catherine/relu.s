.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 is the pointer to the array
#	a1 is the # of elements in the array
# Returns:
#	None
# ==============================================================================
relu:
    # Prologue

    add t0 x0 x0 # counter
loop_start:
    beq t0 a1 loop_end

    slli t0 t0 2
    add t3 t0 a0 # t3:address of current ele
    srli t0 t0 2
    lw t1 0(t3)

    slti t2 t1 0
    addi t0 t0 1
    bne t2 x0 loop_continue # t1 < 0
    jal x0 loop_start

loop_continue:
    sw x0 0(t3)
    jal x0 loop_start

loop_end:


    # Epilogue

    
	ret
