#!/usr/bin/env runhaskell

import Text.Pandoc.JSON
import Data.List as L
import System.Process as P
import Debug.Trace

yasnippetCommentEnd = "# --"

stripR :: Char -> String -> String
stripR x = reverse . dropWhile (==x) . reverse

genYasnippetExpandCommand :: String -> String
genYasnippetExpandCommand s = "(progn (package-initialize) (require 'yasnippet) (setq yas/indent-line nil) (yas-expand-snippet \""
                              ++ s ++ "\") (princ (buffer-substring-no-properties (point-min) (point-max))))"

doYasnippetExpand :: String -> IO String
doYasnippetExpand s = P.readProcess "emacs" ["-batch","--eval",genYasnippetExpandCommand s] ""

commentOutCPP :: String -> String
commentOutCPP s = "// " ++ s
skipYasnippetComment :: String -> IO String
skipYasnippetComment s = let ls = lines s in
                           doYasnippetExpand $ 
                             case L.elemIndex yasnippetCommentEnd $ L.map (stripR ' ') ls of
                               Just i -> L.intercalate "\n" $ (L.map commentOutCPP (L.take (i+1) ls)) ++ L.drop (i+1) ls
                               Nothing -> s


doPandocExpand :: Block -> IO Block
doPandocExpand cb@(CodeBlock (id,classes,namevals) contents) = 
  if elem "yasnippet" classes
    -- then return . (CodeBlock (id,classes,namevals)) =<< skipYasnippetComment contents
    then return . (CodeBlock (id,classes,namevals)) =<< skipYasnippetComment (trace ("content=" ++ contents) contents)
    else return cb
doPandocExpand x = return x

main :: IO ()
main = toJSONFilter doPandocExpand
