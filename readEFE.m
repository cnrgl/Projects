function [Einc, Coordinate] = readEFE(filename, nx, ny, nz, nf)
% This function handle the reading data operation from FEKO EFE(Electromagnetic Field Export)
% files. Comments are ignored considering it's begin with '#' character and
% ** line also skipped internally please do not delete !!!

% E-Field sizes could be parsed from comment in the file but that would be quite unnecessary

fil = fopen(filename);
Einc = zeros(nf,nz,ny,nx,9); % for each of the 9 columns of .EFE files
 textscan(fil,'%c %*[^\n]',1,'CommentStyle','#'); % ignore '**' line

 for f=1:nf
     for i=1:nz
         for j=1:ny
             for k=1:nx
                 try
                     [Einc(f,i,j,k,:)] = cell2mat(textscan(fil,'%f %f %f %f %f %f %f %f %f',1,'CommentStyle','#'));
                 catch ME
                     disp(['error:', ME.identifier, 'reading @freq:', f])
                     fff = cell2mat(textscan(fil,'%f %f %f %f %f %f %f %f %f',1,'CommentStyle','#'));
                     while size(fff,1) == 0
                         fff = cell2mat(textscan(fil,'%f %f %f %f %f %f %f %f %f',1,'CommentStyle','#'));
                     end
                     [Einc(f,i,j,k,:)] = fff;
                 end
             end
         end
     end
 end

Einc_x = Einc(:,:,:,:,4)+1i*Einc(:,:,:,:,5);
Einc_y = Einc(:,:,:,:,6)+1i*Einc(:,:,:,:,7);
Einc_z = Einc(:,:,:,:,8)+1i*Einc(:,:,:,:,9);

Xcord = Einc(:,:,:,:,1); 
Ycord = Einc(:,:,:,:,2);
Zcord = Einc(:,:,:,:,3);

Einc = cat(1,reshape(Einc_x,[1,size(Einc_x)]), reshape(Einc_y,[1,size(Einc_y)]), reshape(Einc_z,[1,size(Einc_z)]));
Coordinate = permute(cat(5,Xcord,Ycord,Zcord),[5,1,2,3,4]);
fclose(fil);
end
