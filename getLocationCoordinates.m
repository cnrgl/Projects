function [Data] = getLocationCoordinates(osm_file)
% This code filter and gives possible Location coordinates over node positions in OSM Files
% Author: A.Caner Gül

Data = getXMLData(osm_file);
% Consider only tag name
filter = "name"; % filter by location names
Data = FilterByAttribute(Data,filter);

end

function coordinates = FilterByAttribute(Data, FilterKey)
coordinates = struct('Name', cell(1), 'Coordinates', cell(1));

Children = Data.Children;
len = length(Children);
vcount = 0;
for it = 1:len
    child = Children(it);
    name = child.Name;
    attr = permute(struct2cell(child.Attributes),[3,1,2]);

    if strcmp(char(name), "tag")
        if strcmp(attr(1,2),FilterKey) % check whether {'k'} eq to FilterKey
            coordinates = struct("Name",attr(2,2),'Coordinates',[]);
            continue;
        end
        continue;
    end
    
    fieldLat = strcmp(attr(:,1),'lat');
    fieldLon = strcmp(attr(:,1),'lon');
    if any(fieldLat) || any(fieldLon)

        Pchild = FilterByAttribute(child,FilterKey);
        if ~isempty(Pchild.Name) && isempty(Pchild.Coordinates)
            vcount = 1 + vcount ;
            coordinates(vcount) = struct("Name",Pchild.Name,"Coordinates",[str2double(attr(fieldLat,2)), str2double(attr(fieldLon,2))]);
        elseif ~isempty(Pchild.Name) && ~isempty(Pchild.Coordinates)
            coordinates = Pchild;
        end
    end
end
end

