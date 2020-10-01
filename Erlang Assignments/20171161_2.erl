-module('20171161_2').
-export([main/1, mergesort/2, merge/3, append/2]).

append(L1, L2) ->
	L1++L2.

merge([], [], Result) ->
	Result;

merge(A, [], Result) ->
	A;

merge([], B, Result) ->
	B;

merge([A|Ca], [B|Cb], Result) ->
	if
		A < B ->
			[A | merge(Ca, [B|Cb], append(Result, [A]))];
		true ->
			[B | merge([A|Ca], Cb, append(Result, [B]))]
	end.

mergesort([], PID) -> 
	PID ! [];

mergesort([E], PID) -> 
	PID ! [E];

mergesort(L, PID) ->
	{A, B} = lists:split(trunc(length(L)/2), L),
  	spawn('20171161_2', mergesort, [A, self()]),
  	spawn('20171161_2', mergesort, [B, self()]),
	receive
		L1 -> ok
	end,
	receive
		L2 -> ok
	end,
  	PID ! merge(L2, L1, L).

main(Args) ->
    {ok, InFile} = file:open(lists:nth(1,Args), [read]),
	{ok, Line} = file:read_line(InFile),
	file:close(InFile),
	Numbers = lists:map(fun(X) -> {Int, _} = string:to_integer(X), 
                    Int end, string:tokens(string:trim(Line), " ")),
	spawn('20171161_2', mergesort, [Numbers, self()]),
	receive L -> ok end,
	{ok, OutFile} = file:open(lists:nth(2, Args), [write]),
	Lst = lists:map(fun(I) -> integer_to_list(I) end, L),
	Ans = string:join(Lst, " "),
	io:format(OutFile,"~s\n",[Ans]),
	file:close(OutFile).
