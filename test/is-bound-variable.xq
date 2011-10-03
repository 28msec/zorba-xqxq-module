import module namespace xqxq = 'http://www.zorba-xquery.com/modules/xqxq';

variable $query-key := xqxq:prepare-main-module("declare variable $a external; $a");
variable $is-not-bound := xqxq:is-bound-variable($query-key, xs:QName("a"));
xqxq:delete-query($query-key);
$is-not-bound