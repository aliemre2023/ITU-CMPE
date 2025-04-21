{-# OPTIONS_GHC -Wno-unrecognised-pragmas #-}
{-# HLINT ignore "Use camelCase" #-}
import Data.List
import Data.Ord

type Point = (Float,Float) -- 2D point locations
type Line = (Point, Point, Float) -- Two points for a line and the width
type Triangle = (Point, Point, Point) -- To save STL files

sortClockwise :: [Point] -> [Point]
sortClockwise ps = sortBy (flip $ comparing (angle c)) ps
  where
    c = (avg (map fst ps), avg (map snd ps))
    avg xs = sum xs / fromIntegral (length xs)
    angle (cx, cy) (x, y) = atan2 (y - cy) (x - cx)

-- Hilbert curve order 1 --
line_list :: [Line]
line_list = [
    ((-5, -5), (-5, 5), 1), -- left
    ((-5, 5), (5, 5), 1), -- top
    ((5, 5), (5, -5), 1) -- right
    ]

create_triangles_from_rectangle :: [Point] -> [Triangle]
create_triangles_from_rectangle corners = [tri1, tri2, tri3, tri4]
    where
        sorted = sortClockwise corners
        [p1, p2, p3, p4] = sorted
        (x1, y1) = p1
        (x2, y2) = p2
        (x3, y3) = p3
        (x4, y4) = p4
        middle_point = ((x1+x2+x3+x4)/4, (y1+y2+y3+y4)/4)
        tri1 = (p1, middle_point, p2)
        tri2 = (p2, middle_point, p3)
        tri3 = (p3, middle_point, p4)
        tri4 = (p4, middle_point, p1)

get_rectangle_corners :: Line -> [Point]
get_rectangle_corners ((x1,y1), (x2,y2), width) = 
    if len == 0 then
        [(x1 + width/2, y1 + width/2), 
         (x1 - width/2, y1 - width/2), 
         (x1 - width/2, y1 + width/2), 
         (x1 + width/2, y1 - width/2)]
    else
        [(x1 + hwx, y1 + hwy), 
         (x1 - hwx, y1 - hwy), 
         (x2 - hwx, y2 - hwy), 
         (x2 + hwx, y2 + hwy)]
    where
        len = sqrt((x2 - x1)^2 + (y2 - y1)^2)
        -- If length is zero, avoid division by zero
        (nx, ny) = if len == 0 then (0, 0) else (-(y2 - y1) / len, (x2 - x1) / len)
        hwx = nx * width / 2
        hwy = ny * width / 2 

linelist_to_rects :: [Line] -> [Triangle]
linelist_to_rects line_list = concat [create_triangles_from_rectangle (get_rectangle_corners line) | line <- line_list]

createTriangleDef :: Triangle -> String
createTriangleDef ((x1,y1),(x2,y2),(x3,y3)) = 
    "  facet normal 0 0 1\n" ++
    "    outer loop\n" ++
    "      vertex " ++ show x1 ++ " " ++ show y1 ++ " 0\n" ++
    "      vertex " ++ show x2 ++ " " ++ show y2 ++ " 0\n" ++
    "      vertex " ++ show x3 ++ " " ++ show y3 ++ " 0\n" ++
    "    endloop\n" ++
    "  endfacet\n"                            

createObjectModelString :: [Triangle] -> String 
createObjectModelString triangles = 
    "solid Object01\n" ++ 
    concatMap createTriangleDef triangles ++ 
    "endsolid Object01"

writeObjModel :: [Triangle] -> String -> IO ()
writeObjModel triangles filename = writeFile filename (createObjectModelString triangles)



------------------------------------------------------------------------------------------
{-
Approach:
    1-) Scale the component and create 4 different component by using it
    2-) Rotate bottom-left componenet clockwise 90 degree, Rotate bottom-right component counterclockwise 90 degree
    3-) To connect componenets, find the related corners and add new lines to connect corners
    4-) Continue recursively
-}

-- Scale a line's coordinates and width --
scaleLine :: Float -> Line -> Line
scaleLine factor ((x1,y1),(x2,y2),w) = ((x1*factor, y1*factor), (x2*factor, y2*factor), w*factor)

-- Translate a line by dx and dy --
translateLine :: (Float, Float) -> Line -> Line
translateLine (dx, dy) ((x1,y1),(x2,y2),w) = ((x1+dx, y1+dy), (x2+dx, y2+dy), w)

-- Found the mid point of lines --
findOrigin :: [Line] -> (Float, Float)
findOrigin lines = ((xmin + xmax) / 2, (ymin + ymax) / 2)
  where
    -- Concatenate all the points
    points = concatMap (\((x1, y1), (x2, y2), w) -> [(x1, y1), (x2, y2)]) lines
    xs = map fst points
    ys = map snd points

    xmin = minimum xs
    xmax = maximum xs
    ymin = minimum ys
    ymax = maximum ys

-- Rotate a list of lines 90 degree around a given origin --
rotateLineArray90CW, rotateLineArray90CCW :: (Float, Float) -> [Line] -> [Line]
rotateLineArray90CW (ox, oy) = map rotateLine
  where
    rotateLine ((x1, y1), (x2, y2), w) =
      let newP1 = rotate (x1, y1)
          newP2 = rotate (x2, y2)
      in (newP1, newP2, w)
    rotate (x, y) = (ox + (y - oy), oy - (x - ox))
rotateLineArray90CCW (ox, oy) = map rotateLine
  where
    rotateLine ((x1, y1), (x2, y2), w) =
      let newP1 = rotate (x1, y1)
          newP2 = rotate (x2, y2)
      in (newP1, newP2, w)
    rotate (x, y) = (ox - (y - oy), oy + (x - ox))

-- Create connectors between Lines, input: BottomLeft, TopLeft, TopRight, BottomRight
createConnector :: [Line] -> [Line] -> [Line] -> [Line] -> [Line]
createConnector lines1 lines2 lines3 lines4 =
    let
        -- Bottom Left
        points1 = concatMap (\((x1, y1), (x2, y2), _) -> [(x1, y1), (x2, y2)]) lines1
        ymax1 = maximum (map snd points1)
        xmin1 = minimum (map fst points1)

        -- Top Left
        points2 = concatMap (\((x1, y1), (x2, y2), _) -> [(x1, y1), (x2, y2)]) lines2
        xmin2 = minimum (map fst points2)
        ymin2 = minimum (map snd points2)
        xmax2 = maximum (map fst points2)

        -- Top Right
        points3 = concatMap (\((x1, y1), (x2, y2), _) -> [(x1, y1), (x2, y2)]) lines3
        ymin3 = minimum (map snd points3)
        xmin3 = minimum (map fst points3)
        xmax3 = maximum (map fst points3)

        -- Bottom Right
        points4 = concatMap (\((x1, y1), (x2, y2), _) -> [(x1, y1), (x2, y2)]) lines4
        xmax4 = maximum (map fst points4)
        ymax4 = maximum (map snd points4)
    
        (_, _, w) = head lines1
    in
        [((xmin2, ymin2), (xmin2, ymax1), w), ((xmax2, ymin2), (xmin3, ymin3), w), ((xmax3, ymin3), (xmax4, ymax4), w)]

-- Generate a Hilbert curve of order n
hilbertcurve :: Integer -> [Line]
hilbertcurve n 
    | n >= 1 = hilbert_n n
    | otherwise = []

-- Helper function for recursion
hilbert_n :: Integer -> [Line]
hilbert_n 1 = map (scaleLine 1) line_list
hilbert_n n =
    let 
        prev = hilbert_n (n-1)

        scale = 1 / (2^(fromIntegral n - 1))
        
        scaled = map (scaleLine scale) prev 

        lineHeight = max (abs (y2 - y1)) (abs (x2 - x1))
            where
                ((x1, y1), (x2, y2), _) = head prev

        bottomLeft_scaled = scaled 
        bottomLeft_origin = findOrigin bottomLeft_scaled
        bottomLeft = rotateLineArray90CW bottomLeft_origin bottomLeft_scaled
        bottomRight_scaled = map (translateLine (lineHeight, 0)) scaled
        bottomRight_origin = findOrigin bottomRight_scaled
        bottomRight = rotateLineArray90CCW bottomRight_origin bottomRight_scaled
        topLeft = map (translateLine (0, lineHeight)) scaled
        topRight = map (translateLine (lineHeight, lineHeight)) scaled
        connectors = createConnector bottomLeft topLeft topRight bottomRight

        combined = bottomLeft ++ bottomRight ++ topLeft ++ topRight ++ connectors
    in combined

main :: IO ()
main = writeObjModel (linelist_to_rects (hilbertcurve 3)) "hilbert.stl"