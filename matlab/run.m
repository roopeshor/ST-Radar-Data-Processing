% run_script.m
% Front end script to find data, read it, process and visualize.

raws = dir(fullfile('Data', 'Mode_1' , '*.raw'));

if isempty(raws)
    disp('No raw files found.');
    return;
end

% Note: Python uses index 3, which corresponds to index 4 in MATLAB
filepath = fullfile(raws(4).folder, raws(4).name);
disp(['Processing file: ', filepath]);

dat = Read_data(filepath);

% Reorder beams to match Python
beams = [
    dat(2)
    dat(3)
    dat(1)
    dat(4)
    dat(5)
];

visualize_spectra(beams);

disp('Processing complete.');
