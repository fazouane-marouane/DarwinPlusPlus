(* ::Package:: *)

pts = Flatten[Table[{x,y}, {x, 1,10}, {y,1,10}],1]
FindShortestTour[%]
pts[[Last[%]]]
Graphics[Line[%]]
