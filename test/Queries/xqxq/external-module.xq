import module namespace xqxq = 'http://www.zorba-xquery.com/modules/xqxq';

variable $query-key := xqxq:prepare-main-module('
    import module namespace
    excel="http://www.zorba-xquery.com/modules/excel/math";
    
    excel:even(1.5)
    ');
xqxq:evaluate($query-key)