%insert your code in Part_1/p1_matlab_code.m

I = imread("p1_search.png");
I = im2gray(I);
I = imbinarize(I);

[rows, cols] = size(I);
topLeft = [0 0];
bottomRight = [rows, cols];

notFound = 1;
row = 1;
while row <= rows && notFound
    col = 1;
    while col <= cols && notFound
        if (I(row,col) == 0)
            topLeft = [row col];
            notFound = 0;
        end
        col = col + 1;
    end
    row = row + 1;
end

notFound = 1;
row = rows;
while row > 0 && notFound
    col = cols;
    while col > 0 && notFound
        if (I(row,col) == 0)
            bottomRight = [row col];
            notFound = 0;
        end
        col = col - 1;
    end
    row = row - 1;
end

cropped = I(topLeft(1):bottomRight(1), topLeft(2):bottomRight(2));

imwrite(cropped, "p1_matlab_image.png")
