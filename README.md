# clrmsg-c

%{Red:%s}         ->    <Red>%s<Reset>
%{Red:a string}   ->    <Red>a string<Reset>
%{Red}            ->    <Red> # a closing <reset> will be appended to end of
                              # buffer if not provided
%{Reset}          ->    <Reset>
