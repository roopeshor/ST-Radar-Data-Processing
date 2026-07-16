include("Proc.jl")
using .Proc
using .Proc.Reader

function read_python_binary(filepath::String, shape::Tuple)
    total_elements = prod(shape)
    data = zeros(Float64, total_elements)
    open(filepath, "r") do io
        read!(io, data)
    end
    # Numpy arrays are flattened in C-order (row-major).
    # Julia arrays are Column-major. So we read as 1D, then transpose/reshape accordingly.
    if length(shape) == 3
        # python shape: (incoh, bins, nfft)
        # julia reshape expects first dim to vary fastest (column-major).
        # So we reshape to (nfft, bins, incoh) and then permutedims.
        rev_shape = reverse(shape)
        arr = reshape(data, rev_shape)
        return permutedims(arr, (3, 2, 1))
    elseif length(shape) == 2
        # python shape: (bins, nfft)
        rev_shape = reverse(shape)
        arr = reshape(data, rev_shape)
        return permutedims(arr, (2, 1))
    elseif length(shape) == 1
        return data
    end
end

function main()
    folder = "../Data/Mode_1"
    raws = filter(x -> endswith(x, ".raw"), readdir(folder, join=true))
    
    filepath = raws[4] # corresponds to python index 3
    println("Reading $filepath with Julia...")
    dat = read_data(filepath)
    
    beam_0 = dat[1] # python dat[0]
    julia_data_matrix = beam_0.BeamData
    incoh, bins, nfft = size(julia_data_matrix)
    
    println("Julia Data Matrix Shape: ", size(julia_data_matrix))
    
    py_real = read_python_binary("py_raw_data_real.bin", (incoh, bins, nfft))
    py_imag = read_python_binary("py_raw_data_imag.bin", (incoh, bins, nfft))
    py_data_matrix = py_real .+ py_imag .* im
    
    # Compare raw data
    raw_diff = maximum(abs.(julia_data_matrix .- py_data_matrix))
    println("Max difference in raw data: ", raw_diff)
    if raw_diff < 1e-5
        println("=> Raw data parser MATCHES perfectly!")
    else
        println("=> Raw data parser MISMATCH!")
    end
    
    # Run julia processing
    julia_spectra, julia_L = process_radar_data(julia_data_matrix)
    
    # Load python processing outputs
    py_spectra = read_python_binary("py_spectra.bin", (bins, nfft))
    py_L = read_python_binary("py_L.bin", (bins,))
    
    spectra_diff = maximum(abs.(julia_spectra .- py_spectra))
    L_diff = maximum(abs.(julia_L .- py_L))
    
    println("Max difference in spectra: ", spectra_diff)
    if spectra_diff < 1e-5
        println("=> Spectra processing MATCHES perfectly!")
    else
        println("=> Spectra processing MISMATCH!")
    end
    
    println("Max difference in noise level L: ", L_diff)
    if L_diff < 1e-5
        println("=> Noise level processing MATCHES perfectly!")
    else
        println("=> Noise level processing MISMATCH!")
    end
end

main()
