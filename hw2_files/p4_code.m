%insert your code in Part_4/p4_code.cpp
%edit the file extension and web template to match your programing language

%insert your code in Part_3/p3_code.cpp
%edit the file extension and web template to match your programing language

I = imread("p4_search.png");
I = im2gray(I);
I = imbinarize(I);

X = imread("../Symbol_Cutouts/X.png");
X = im2gray(X);
X = imbinarize(X);

SE = strel("disk",2);
X = ~imerode(~X,SE);
% imshow(X)

Ix = imerode(~I, ~X);
Ix = ~imdilate(Ix, ~X);
% montage({I,Ix, Ix-I})

compMat = label_components(~Ix);

% Color each identified object/sqaure
numObjects = max(compMat,[],"all")
colors = rand(numObjects, 3);
% colors(1,:) = [1 1 1]; % set 1st element (outside border) to white
coloredMat = zeros(rows,cols,3);
for row = 1:rows
    for col = 1:cols
        if (compMat(row,col) > 0)
            coloredMat(row,col,:) = colors(compMat(row,col),:);
        end
    end
end



montage({I,objectMat,~I-objectMat,coloredMat})

%imwrite(coloredMat, "p4_image.png");