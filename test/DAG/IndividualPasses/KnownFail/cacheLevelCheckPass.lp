:- cacheParentOf(X, Y), not cacheChildOf(Y, X).
:- cacheChildOf(X, Y), not cacheParentOf(Y, X).
:- not cacheIsParent(X, true), cacheParentOf(X,Y), cache(Y).
:- not cacheIsChild(X, true), cacheChildOf(X,Y), cache(Y).
