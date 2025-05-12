module Main where

import System.Environment (getArgs)
import System.IO
import Data.List (foldl')
import System.CPUTime (getCPUTime)

{-
Approach:   
    1-) Determine unhappy agents and empty cells
    2-) Place unhappy agent to the empty cells
    3-) Recursively do those in each step
-}

-- Type definitions for 2D grid
type Grid = [[Int]]
type Position = (Int, Int)

-- Parameters
gridSize :: Int
gridSize = 200

tolerance :: Float
tolerance = 0.7

steps :: Int
steps = 25

-- Simple RNG function, GCC version
nextRandom :: Int -> Int
nextRandom seed = mod (1103515245 * seed + 12345) 2147483648

-- Generate a random number between min and max
randomRange :: Int -> Int -> Int -> (Int, Int)
randomRange minVal maxVal seed = 
    let newSeed = nextRandom seed
        range = maxVal - minVal + 1
        value = minVal + (mod newSeed range)
    in (value, newSeed)

-- Load grid from file
loadGrid :: FilePath -> IO Grid
loadGrid filename = do
    content <- readFile filename
    let values = map read (words content) :: [Int]
    return (chunksOf gridSize values)
    where
        chunksOf _ [] = []
        chunksOf n xs = take n xs : chunksOf n (drop n xs)

-- Save grid to file
saveGrid :: FilePath -> Grid -> IO ()
saveGrid filename grid = do
    let flatGrid = concat grid
    writeFile filename (unlines (map show flatGrid))
    putStrLn ("Simulation finished. Output saved to " ++ filename)

-- Get the element at a position in the grid
getCell :: Grid -> Position -> Int
getCell grid (x, y) = (grid !! x) !! y

-- Set the element at a position in the grid
setCell :: Grid -> Position -> Int -> Grid
setCell grid (x, y) value =
    let row = grid !! x
        newRow = take y row ++ [value] ++ drop (y+1) row
    in take x grid ++ [newRow] ++ drop (x+1) grid

-- Fisher-Yates shuffle algorithm with simple RNG
shuffle :: [a] -> Int -> ([a], Int)
shuffle xs seed = 
    shuffleHelper xs [] seed
    where
        shuffleHelper [] acc seed = (acc, seed)
        shuffleHelper xs acc seed = 
            let len = length xs
                (idx, newSeed) = randomRange 0 (len-1) seed
                x = xs !! idx
                rest = take idx xs ++ drop (idx+1) xs
            in shuffleHelper rest (x:acc) newSeed

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

-- Check if agent is happy
isHappy :: Grid -> Position -> Bool
isHappy grid pos =
    let agent = getCell grid pos
    in if agent == 0
       then True
       else let neighbors = getNeighbors grid pos
                similar = length (filter (== agent) neighbors)
                total = length (filter (/= 0) neighbors)
            in if total == 0
               then False -- all of the neighbors are different from agent
               else (fromIntegral similar / fromIntegral total) >= tolerance

-- Find all unhappy agents and empty cells
findUnhappyAndEmpty :: Grid -> ([Position], [Position])
findUnhappyAndEmpty grid = 
    let positions = [(x, y) | x <- [0..gridSize-1], y <- [0..gridSize-1]]
        isEmpty pos = getCell grid pos == 0
        isUnhappyAgent pos = 
            let v = getCell grid pos 
            in (v /= 0) && not (isHappy grid pos)
        emptyCells = filter isEmpty positions
        unhappyAgents = filter isUnhappyAgent positions
    in (unhappyAgents, emptyCells)

-- Move an agent to an empty position
moveAgent :: Grid -> (Position, Position) -> Grid
moveAgent grid (unhappyPos, emptyPos) =
    let agent = getCell grid unhappyPos
        grid' = setCell grid unhappyPos 0
    in setCell grid' emptyPos agent

-- One step 
schellingStep :: Grid -> Int -> (Grid, Int)
schellingStep grid seed =
    let (unhappyAgents, emptyCells) = findUnhappyAndEmpty grid
        (shuffledUnhappy, seed') = shuffle unhappyAgents seed
        (shuffledEmpty, seed'') = shuffle emptyCells seed'
        moves = min (length shuffledUnhappy) (length shuffledEmpty)
        movePairs = zip (take moves shuffledUnhappy) (take moves shuffledEmpty)
    in (foldl' moveAgent grid movePairs, seed'')

-- Run simulation
runSimulation :: Grid -> Int -> Int -> (Grid, Int)
runSimulation grid seed 0 = (grid, seed)
runSimulation grid seed n =
    let (grid', seed') = schellingStep grid seed
    in runSimulation grid' seed' (n-1)

main :: IO ()
main = do
    args <- getArgs
    
    let inputFile = if length args > 0 then head args else "cells.txt"
    let outputFile = if length args > 1 then args !! 1 else "cells_out.txt"
    
    -- A random seed
    -- seed <- fmap ((`mod` 2147483647) . fromIntegral) getCPUTime
    let seed = 42
    
    -- 2D grid
    grid <- loadGrid inputFile
    
    -- Run recursively
    let (finalGrid, _) = runSimulation grid seed steps
    
    -- Save 
    saveGrid outputFile finalGrid