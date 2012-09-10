import module namespace xqxq = 'http://www.zorba-xquery.com/modules/xqxq';

declare namespace resolver = 'http://www.zorba-xquery.com/modules/xqxq/url-resolver';
declare function resolver:url-resolver($namespace as xs:string, $entity as xs:string) as xs:string? {
  if($namespace = 'http://test.xq')
  then "module namespace test = 'http://test'; declare function test:foo(){'foo'};"
  else ()
};

variable $queryID := xqxq:prepare-main-module("import module namespace test = 'http://test'; test:foo()", fn:QName('http://www.zorba-xquery.com/modules/xqxq/url-resolver', 'resolver:url-resolver'));
xqxq:evaluate($queryID)                                                                             