    jal ra label
    addi s0 s0 5
    jal x0 end
label:
    addi s0 s0 10
    jalr x0 ra 0
end:
