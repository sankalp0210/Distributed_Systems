-module('20171161_1').
-export([main/1, ring/4]).

ring(0, NumProcs, TokenValue, OutFile)->
    io:format(OutFile, "Process ~w received token ~w from process ~w\n", [0, TokenValue, NumProcs]);

ring(ProcId, NumProcs, TokenValue, OutFile)->
    spawn('20171161_1', ring, [ProcId-1, NumProcs, TokenValue, OutFile]),
    io:format(OutFile, "Process ~w received token ~w from process ~w\n", [NumProcs-ProcId+1, TokenValue, NumProcs-ProcId]).

main(Args)->
    {_, InFile} = file:open(lists:nth(1,Args), [read]),
    {_, Binary} = file:read_line(InFile),
    file:close(InFile),
    Lst = lists:map(fun(X) -> {Int, _} = string:to_integer(X), 
                    Int end,
          string:tokens(string:trim(Binary), " ")),
    NumProcs = lists:nth(1, Lst),
    TokenValue = lists:nth(2, Lst),
    {_, OutFile} = file:open(lists:nth(2, Args), [write]),
    ring(NumProcs-1, NumProcs-1, TokenValue, OutFile),
    file:close(OutFile).
