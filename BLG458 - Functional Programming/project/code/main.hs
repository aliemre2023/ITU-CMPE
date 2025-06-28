import Codec.Picture
import System.Directory (createDirectoryIfMissing)
import Control.Arrow (ArrowLoop(loop))

type Grid = [[Int]]
type Position = (Int, Int)

gridSize :: Int
gridSize = 20

cellSize :: Int
cellSize = 10

loopCount :: Int
loopCount = 40

imageSize :: Int
imageSize = gridSize * cellSize

colorFor :: Int -> PixelRGB8
colorFor 0 = PixelRGB8 255 255 255  -- white
colorFor 1 = PixelRGB8 0 0 255      -- blue
colorFor _ = PixelRGB8 0 0 0        -- error!

-- Expand with 0s
resizer :: Grid -> Position -> Position -> Grid
resizer source (targetH, targetW) (startY, startX) =
  [ [ getValue y x | x <- [0..targetW - 1] ] | y <- [0..targetH - 1] ]
  where
    sourceH = length source
    sourceW = length (head source)
    
    getValue y x
      | y >= startY && y < startY + sourceH &&
        x >= startX && x < startX + sourceW =
          source !! (y - startY) !! (x - startX)
      | otherwise = 0

-- (p4) Pinwheel
pattern1_ :: Grid
pattern1_ = [
  [0,0,0,0,0,0,1,1,0,0,0,0],
  [0,0,0,0,0,0,1,1,0,0,0,0],
  [0,0,0,0,0,0,0,0,0,0,0,0],
  [0,0,0,0,1,1,1,1,0,0,0,0],
  [1,1,0,1,0,0,1,0,1,0,0,0],
  [1,1,0,1,0,1,0,0,1,0,0,0],
  [0,0,0,1,0,0,0,1,1,0,1,1],
  [0,0,0,1,0,0,0,0,1,0,1,1],
  [0,0,0,0,1,1,1,1,0,0,0,0],
  [0,0,0,0,0,0,0,0,0,0,0,0],
  [0,0,0,0,1,1,0,0,0,0,0,0],
  [0,0,0,0,1,1,0,0,0,0,0,0]
  ]

-- (p2) Blinker
pattern2_ :: Grid
pattern2_ = [
  [0,0,0,1,1,1],
  [0,0,0,0,0,0],
  [0,0,0,0,0,0],
  [0,0,0,0,0,0],
  [0,0,0,0,0,0],
  [0,0,1,0,0,0],
  [0,0,1,0,0,0],
  [0,0,1,0,0,0]
  ]

-- (p5) Octagon 2
pattern3_ :: Grid
pattern3_ = [
  [0,0,0,1,1,0,0,0],
  [0,0,1,0,0,1,0,0],
  [0,1,0,0,0,0,1,0],
  [1,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,1],
  [0,1,0,0,0,0,1,0],
  [0,0,1,0,0,1,0,0],
  [0,0,0,1,1,0,0,0]
  ]

-- (p3) Glider
pattern4_ :: Grid
pattern4_ = [
  [0,1,0],
  [0,0,1],
  [1,1,1]
  ]

-- (p15) Pentadecathon
pattern5_ :: Grid
pattern5_ = [
  [0,0,1,0,0,0,0,1,0,0],
  [1,1,0,1,1,1,1,0,1,1],
  [0,0,1,0,0,0,0,1,0,0]
  ]

--  To see in a big picture
pattern1 = resizer pattern1_ (20,20) (5,5)
pattern2 = resizer pattern2_ (20,20) (5,5)
pattern3 = resizer pattern3_ (20,20) (5,5)
pattern4 = resizer pattern4_ (20,20) (5,5)
pattern5 = resizer pattern5_ (20,20) (5,5)



-- Get the element at a position in the grid
getCell :: Grid -> Position -> Int
getCell grid (x, y) = (grid !! x) !! y

-- Set the element at a position in the grid
setCell :: Grid -> Position -> Int -> Grid
setCell grid (x, y) value =
    let row = grid !! x
        newRow = take y row ++ [value] ++ drop (y+1) row
    in take x grid ++ [newRow] ++ drop (x+1) grid

-- Get neighbors for a cell
getNeighbors :: Grid -> Position -> [Int]
getNeighbors grid (x, y) = do
    dx <- [-1, 0, 1]
    dy <- [-1, 0, 1]
    if dx == 0 && dy == 0
        then []
        else 
            let nx = x + dx
                ny = y + dy
            in if 0 <= nx && nx < gridSize && 0 <= ny && ny < gridSize
                then return (getCell grid (nx, ny))
                else []

-- Count alive neighbors
countAlive :: [Int] -> Int
countAlive = length . filter (==1)

-- Next version of Grid according to Rules
nextGen :: Grid -> Grid
nextGen grid =
  [[cellNextState (x,y) | y <- [0..gridSize-1]] | x <- [0..gridSize-1]]
  where
    cellNextState pos =
      let neighbors = getNeighbors grid pos
          aliveCount = countAlive neighbors
          currentCell = getCell grid pos
      in case currentCell of
          1 -> if (aliveCount == 2 || aliveCount == 3) 
                then 1 
                else 0
          0 -> if (aliveCount == 3) 
                then 1 
                else 0
          _ -> 0



matrixToImage :: [[Int]] -> Image PixelRGB8
matrixToImage mat = generateImage pixelAt imageSize imageSize
  where
    pixelAt x y =
      let row = y `div` cellSize
          col = x `div` cellSize
          val = (mat !! row) !! col
      in colorFor val

main :: IO ()
main = do
  let patternGrids = zip [1..]
        [ 
          pattern1,
          pattern2,
          pattern3,
          pattern4,
          pattern5
        ]

  mapM_
    (\(i, grid) -> do
        let dirName = "output/frames" ++ show i
        createDirectoryIfMissing True dirName
        putStrLn $ "Folder Created: " ++ dirName
        
        let loop :: Grid -> Int -> IO ()
            loop _ j | j == loopCount + 1 = return ()
            loop matrix j = do
              let image = matrixToImage matrix 
                  fileName = dirName ++  "/frame" ++ show j ++ ".png"
              savePngImage fileName (ImageRGB8 image)
              putStrLn ("Saved: " ++ fileName)
              loop (nextGen matrix) (j + 1)
        loop grid 0

    )
    patternGrids
