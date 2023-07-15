function [Data] = getXMLData(filename)
% refer to below sample codes:
% https://www.mathworks.com/help/matlab/ref/xmlread.html
% https://www.mathworks.com/matlabcentral/fileexchange/28518-xml2struct
try
    tree = xmlread(filename);
catch
    error('Failed to read XML file %s.',filename);
end

Data = parseChildNodes(tree);
end

function [children] = parseChildNodes(tree)
if tree.hasChildNodes
    childNodes = tree.getChildNodes;
    numChildNodes = childNodes.getLength;

    children = struct('Name', cell(0), 'Attributes', cell(0),    ...
        'Data', cell(0), 'Children', cell(0)); % structure cell array prototype
    vcount = 0;

    for count=1:numChildNodes
        child = childNodes.item(count-1);
        name = child.getNodeName;
        if (strcmp(name,'#text') || strcmp(name,'#comment') || strcmp(name,'#cdata_dash_section')) % ignore other lines
            continue;
        end

        vcount = vcount + 1; % valid count
        nodeStruct = struct( ...
            'Name', char(name),       ...
            'Attributes', parseAttributes(child),  ...
            'Data', '',                              ...
            'Children', parseChildNodes(child)); 

        if any(strcmp(methods(child), 'getData'))
            nodeStruct.Data = char(child.getData);
        else
            nodeStruct.Data = '';
        end
        children(vcount) = nodeStruct;
    end
else
    %  error("There is no child nodes !!!");
    children = [];
end
end

function attributes = parseAttributes(theNode) % conventional function for handle the attributes in xml format
% Create attributes structure.
attributes = [];
if theNode.hasAttributes
    theAttributes = theNode.getAttributes;
    numAttributes = theAttributes.getLength;
    allocCell = cell(1, numAttributes);
    attributes = struct('Name', allocCell, 'Value', ...
        allocCell);

    for count = 1:numAttributes
        attrib = theAttributes.item(count-1);
        attributes(count).Name = char(attrib.getName);
        attributes(count).Value = char(attrib.getValue);
    end
end
end

