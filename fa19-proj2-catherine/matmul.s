.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# 	d = matmul(m0, m1)
#   If the dimensions don't match, exit with exit code 2
# Arguments:
# 	a0 is the pointer to the start of m0
#	a1 is the # of rows (height) of m0
#	a2 is the # of columns (width) of m0
#	a3 is the pointer to the start of m1
# 	a4 is the # of rows (height) of m1
#	a5 is the # of columns (width) of m1
#	a6 is the pointer to the the start of d
# Returns:
#	None, sets d = matmul(m0, m1)
# =======================================================
matmul:

    # Error if mismatched dimensions
    bne a2 a4 mismatched_dimensions
    # Prologue
    addi sp sp -32
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
    sw s5 20(sp)
    sw s6 24(sp)
    sw ra 28(sp)
    mv s0 a0
    mv s1 a1
    mv s2 a2
    mv s3 a3
    mv s4 a4
    mv s5 a5
    mv s6 a6
    
    add t0 x0 x0 # row counter to s1
    add t1 x0 x0 # column counter to s5

outer_loop_start:
    beq t0 s1 outer_loop_end
    mul t2 t0 s2 
    slli t2 t2 2 # offset
    add a0 s0 t2 # start address of row vector v0
    mv a2 s2 # length of vector
    addi a3 x0 1 # stride of v0

inner_loop_start:
    beq t1 s5 inner_loop_end
    mv t3 t1
    slli t3 t3 2
    add a1 s3 t3 # start address of column vector v1
    add a4 x0 s5 # stride of v1

    addi sp sp -36
    sw t0 0(sp)
    sw t1 4(sp)
    sw a0 8(sp)
    sw a1 12(sp)
    sw a2 16(sp)
    sw a3 20(sp)
    sw a4 24(sp)
    sw a5 28(sp)
    sw a6 32(sp)

    jal ra dot # function call

    mv t3 a0
    lw t0 0(sp)
    lw t1 4(sp)
    lw a0 8(sp)
    lw a1 12(sp)
    lw a2 16(sp)
    lw a3 20(sp)
    lw a4 24(sp)
    lw a5 28(sp)
    lw a6 32(sp)
    addi sp sp 36

    mul t2 t0 s5
    add t2 t2 t1
    slli t2 t2 2 # offset of ele (t0, t1) in d
    add t2 t2 s6
    sw t3 0(t2)

    addi t1 t1 1
    jal x0 inner_loop_start

inner_loop_end:
    addi t0 t0 1 # add row number
    mv t1 x0 # set column to 0
    jal x0 outer_loop_start

outer_loop_end:
    # Epilogue
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)
    lw s5 20(sp)
    lw s6 24(sp)
    lw ra 28(sp)
    addi sp sp 32

    ret


mismatched_dimensions:
    li a1 2
    jal exit2
