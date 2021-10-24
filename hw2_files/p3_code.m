%insert your code in Part_3/p3_code.cpp
%edit the file extension and web template to match your programing language

I = imread("p3_search.png");
I = im2gray(I);
I = imbinarize(I);

[rows, cols] = size(I);
objectMat = zeros(rows+1,cols+1);
i = 1;
for row = 1:rows
    for col = 1:cols
        if (I(row,col))
            if (objectMat(row,col+1))
                objectMat(row+1,col+1) = objectMat(row,col+1);
                if (objectMat(row+1,col) && objectMat(row+1,col) ~= objectMat(row,col+1))
                    % missing equivalency table addition
                end
            elseif (objectMat(row+1,col))
                objectMat(row+1,col+1) = objectMat(row+1,col);
            else
                objectMat(row+1,col+1) = i;
                i = i + 1;
            end
        end
    end
end
objectMat = objectMat(2:end,2:end);

% Missing equivalency table simplification

% Color each identified object/sqaure
numObjects = max(objectMat,[],"all");
colors = rand(numObjects, 3);
colors(1,:) = [1 1 1]; % set 1st element (outside border) to white
coloredMat = zeros(rows,cols,3);
for row = 1:rows
    for col = 1:cols
        if (objectMat(row,col) > 0)
            coloredMat(row,col,:) = colors(objectMat(row,col),:);
        end
    end
end

montage({I,objectMat,coloredMat})

imwrite(coloredMat, "p3_image.png");