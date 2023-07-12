function [Einc] = readEFE(filename, nx, ny, nz)
% This function handle the reading data operation from FEKO EFE(Electromagnetic Field Export)
% files. Comments are ignored considering it's begin with '#' character and
% ** line also skipped internally please do not delete !!!
% Author : A.Caner Gül Date: 12.07.2023
% E-Field sizes could be parsed from comment in the file but that would be quite unnecessary

fil = fopen(filename);
Einc = zeros(nx,ny,nz,9); % for each of the 9 columns of .EFE files 

textscan(fil,'%c %*[^\n]',1,'CommentStyle','#'); % ignore '**' line

    for i=1:nz
        for j=1:ny
            for k=1:nx
                [Einc(i,j,k,:)] = cell2mat(textscan(fil,'%f %f %f %f %f %f %f %f %f',1,'CommentStyle','#'));
            end
        end
    end

    Einc_x = Einc(:,:,:,4)+1i*Einc(:,:,:,5);
    Einc_y = Einc(:,:,:,6)+1i*Einc(:,:,:,7);
    Einc_z = Einc(:,:,:,8)+1i*Einc(:,:,:,9);

    Einc = reshape([Einc_x; Einc_y; Einc_z],[3, nx, ny, nz]);
    fclose(fil);
end
