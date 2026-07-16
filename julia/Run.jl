

include("Proc.jl")
using .Proc
using .Proc.Reader

function main()
    folder = "Data/Mode_1"
    # Find all .raw files
    # Note: Using readdir if Glob is not installed, or simply just path manipulation
    raws = filter(x -> endswith(x, ".raw"), readdir(folder, join=true))
    
    if isempty(raws)
        println("No .raw files found in $folder")
        return
    end

    # dat = st_radar.read_data(raws[3]._str)
    # Note: julia 1-based indexing, python raws[3] is julia raws[4]
    filepath = raws[4]
    println("Processing file: $filepath")
    dat = read_data(filepath)
    
    beams = [
        dat[2], # python dat[1]
        dat[3], # python dat[2]
        dat[1], # python dat[0]
        dat[4], # python dat[3]
        dat[5], # python dat[4]
    ]
    
    visualize_spectra(beams)
    
    println("Press Enter to close the plot and exit...")
    readline()
end

if abspath(PROGRAM_FILE) == @__FILE__
    main()
end
