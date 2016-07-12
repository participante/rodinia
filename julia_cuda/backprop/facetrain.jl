#!/usr/bin/env julia

include("backprop.jl")
include("backprop_cuda_kernel.jl")

function backprop_face(layer_size)
    net = bpnn_create(layer_size, 16, 1) # (16, 1 cannot be changed)
    println("Input layer size : ", layer_size)

    units = net.input_units
    for i = 2:layer_size+1
        units[i] = float(rand()) / RAND_MAX
    end

    # Entering the training kernel, only one iteration.
    println("Starting training kernel")
    bpnn_train_cuda(net, ctx)

    if haskey(ENV, "OUTPUT")
        bpnn_save(net, "output.dat")
    end

    println("Training done")
end

################################################################################
# Program main
################################################################################
function main(args)
    if length(args) != 1
        println(STDERR, "usage: backprop <num of input elements>");
        exit(1)
    end

    layer_size = parse(Int, args[1])

    if layer_size % 16 != 0
        @printf(STDERR, "The number of input points must be divisible by 16\n")
        exit(1)
    end

    bpnn_initialize(7)
    backprop_face(layer_size)
end

dev = CuDevice(0)
ctx = CuContext(dev)

main(ARGS)

destroy(ctx)