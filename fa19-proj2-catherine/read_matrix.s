.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#   If any file operation fails or doesn't read the proper number of bytes,
#   exit the program with exit code 1.
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 is the pointer to string representing the filename
#   a1 is a pointer to an integer, we will set it to the number of rows
#   a2 is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 is the pointer to the matrix in memory
# ==============================================================================
read_matrix:

    # Prologue
	addi sp sp -28
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
    sw s5 20(sp)
    sw ra 24(sp)
    mv s0 a0
    mv s1 a1
    mv s2 a2

    mv a1 s0
    add a2 x0 x0
    jal ra fopen
    addi t0 x0 -1
    beq a0 t0 eof_or_error

    mv s3 a0 # file descriptor
    mv a1 s3
    mv a2 s1
    addi a3 x0 8
    jal ra fread
    addi t0 x0 8
    bne a0 t0 eof_or_error

    lw t1 0(s1)
    lw t2 0(s2)
    mul t1 t1 t2
    addi t2 x0 4
    mul t1 t1 t2 
    mv s5 t1 # number of bytes to allocate
    mv a0 t1
    jal ra malloc
    mv s4 a0 # pointer to matrix

    mv a1 s3
    mv a2 s4
    mv a3 s5
    jal ra fread
    bne a0 s5 eof_or_error

    mv a1 s3
    jal ra fclose
    bne a0 x0 eof_or_error

    mv a0 s4
    # Epilogue
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)
    lw s5 20(sp)
    lw ra 24(sp)
    addi sp sp 28

    ret

eof_or_error:
    li a1 1
    jal exit2
    