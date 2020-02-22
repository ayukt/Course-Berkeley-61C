.import read_matrix.s
.import write_matrix.s
.import matmul.s
.import dot.s
.import relu.s
.import argmax.s
.import utils.s

.globl main

.text
main:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0: int argc
    #   a1: char** argv
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>

    # Exit if incorrect number of command line args

    addi t0 t0 5
    beq t0 a0 continue
    li a1 5
    jal exit2
	# =====================================
    # LOAD MATRICES
    # =====================================
continue:
    mv s1 a1

    # Load pretrained m0
    addi a0 x0 8
    jal ra malloc
    mv a1 a0
    mv s2 a0 # s2: row col for m0
    addi a2 a1 4
    lw a0 4(s1)
    jal ra read_matrix
    mv s3 a0 # s3: m0

    # Load pretrained m1
    addi a0 x0 8
    jal ra malloc
    mv a1 a0
    mv s4 a0 # s4: row col for m1
    addi a2 a1 4
    lw a0 8(s1)
    jal ra read_matrix
    mv s5 a0 # s5: m1

    # Load input matrix
    addi a0 x0 8
    jal ra malloc
    mv a1 a0
    mv s6 a0 # s6: row col for input
    addi a2 a1 4
    lw a0 12(s1)
    jal ra read_matrix
    mv s7 a0 # s7: input

    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input
    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)

    # 1
    lw t0 0(s2)
    lw t1 4(s6)
    mul a0 t0 t1
    slli a0 a0 2
    jal ra malloc
    mv s8 a0 # s8: pointer to hidden layer

    mv a0 s3
    lw a1 0(s2)
    lw a2 4(s2)
    mv a3 s7
    lw a4 0(s6)
    lw a5 4(s6)
    mv a6 s8
    jal ra matmul

    # 2
    mv a0 s8
    lw t0 0(s2)
    lw t1 4(s6)
    mul a1 t0 t1
    jal ra relu

    # 3
    lw t0 0(s4) # row of m1
    lw t1 4(s6) # col of input/hidden layer
    mul a0 t0 t1
    slli a0 a0 2
    jal ra malloc
    mv s9 a0 # s9: pointer to scores

    mv a0 s5
    lw a1 0(s4)
    lw a2 4(s4)
    mv a3 s8
    lw a4 0(s2)
    lw a5 4(s6)
    mv a6 s9
    jal ra matmul

    # write to file
    lw a0 16(s1)
    mv a1 s9
    lw a2 0(s4)
    lw a3 4(s6)
    jal ra write_matrix

    # argmax
    lw t0 0(s4)
    lw t1 4(s6)
    mv a0 s9
    mul a1 t0 t1
    jal ra argmax

    mv a1 a0
    jal ra print_int













    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix
    lw a0 16(s0) # Load pointer to output filename





    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax




    # Print classification




    # Print newline afterwards for clarity
    li a1 '\n'
    jal print_char

    jal exit
