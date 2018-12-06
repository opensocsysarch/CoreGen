%:- cacheParentOf(X, Y), not cacheChildOf(Y, X).
:- cacheChildOf(X, Y), not cacheParentOf(Y, X).
