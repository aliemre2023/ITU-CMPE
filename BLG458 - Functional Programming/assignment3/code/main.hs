

{-# LANGUAGE OverloadedStrings #-}

import qualified Data.Map.Strict as Map
import qualified Data.Set as Set
import qualified Data.Sequence as Seq
import qualified Data.Text as T
import qualified Data.Text.IO as TIO
import System.Environment (getArgs)

import Data.Maybe (mapMaybe)

type MID = T.Text
type Graph = Map.Map MID [MID]
type NameMap = Map.Map MID T.Text

-- Parse one line of freebase.tsv into (source, destination)
parseEdgeLine :: T.Text -> Maybe (MID, MID)
parseEdgeLine line =
    case T.splitOn "\t" line of
        (src : _rel : dst : _) -> Just (T.strip src, T.strip dst)
        _ -> Nothing

-- Parse one line of mid2name.tsv into (MID, Name)
parseNameLine :: T.Text -> Maybe (MID, T.Text)
parseNameLine line =
    case T.splitOn "\t" line of
        (mid : name : _) -> Just (T.strip mid, T.strip name)
        _ -> Nothing

-- Build the graph as an adjacency list
buildGraph :: [(MID, MID)] -> Graph
buildGraph = foldr insertEdge Map.empty
  where
    insertEdge (src, dst) = Map.insertWith insertOnce src [dst]
    insertOnce new old = if elem (head new) (old) then old else new ++ old

-- BFS to find the shortest path
bfs :: Graph -> MID -> MID -> Maybe ([MID], Int)
bfs graph start goal = go Set.empty (Seq.singleton (start, [start]))
  where
    go _ Seq.Empty = Nothing
    go visited ((current, path) Seq.:<| queue)
      | current == goal = Just (path, length path - 1)
      | Set.member current visited = go visited queue
      | otherwise =
          let neighbors = Map.findWithDefault [] current graph
              newPaths = [(n, path ++ [n]) | n <- neighbors]
          in go (Set.insert current visited) (queue Seq.>< Seq.fromList newPaths)

-- Convert a list of MIDs to names using the map, fallback to MID if not found
resolveNames :: NameMap -> [MID] -> [T.Text]
resolveNames nameMap = map (\mid -> Map.findWithDefault mid mid nameMap)

-- Main
-- run: runhaskell main.hs freebase.tsv mid2name.tsv <MID1> <MID2>
main :: IO ()
main = do
    args <- getArgs

    let [freebaseFile, midnameFile, startMid, endMid] = take 4 args
    putStrLn $ "Search: " ++ startMid ++ " to " ++ endMid

    -- Load freebase edges
    freebaseContent <- TIO.readFile freebaseFile
    let edgeList = mapMaybe parseEdgeLine (T.lines freebaseContent)
        graph = buildGraph edgeList

    let startT = T.pack startMid
        endT = T.pack endMid

    -- Load mid to name mapping, keeping only the first appearance
    midnameContent <- TIO.readFile midnameFile
    let nameMap = foldl insertIfAbsent Map.empty (mapMaybe parseNameLine (T.lines midnameContent))
          where
            insertIfAbsent acc (mid, name) =
                if Map.member mid acc
                    then acc
                    else Map.insert mid name acc

    -- Perform BFS
    case bfs graph startT endT of
        Nothing -> error "No path found between the given MIDs."
        Just (path, dist) -> do
            putStrLn $ "Distance: " ++ show dist
            putStrLn "\nMID - Name"
            let names = resolveNames nameMap path
            mapM_ (\(mid, name) -> putStrLn $ T.unpack mid ++ " - " ++ T.unpack name) (zip path names)
