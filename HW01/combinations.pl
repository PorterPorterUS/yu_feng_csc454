combinationthree(0, [], _).
combinationthree(N, [Header|Tail], Out) :-
    append(_, [Header|Rest], Out),
    M is N-1,
    selectn(M, Tail, Rest).

combinations(K,N,Out) :-
    numlist(1, N, L),
    combinationthree(K, Out, L).