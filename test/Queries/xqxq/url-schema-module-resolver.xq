import module namespace xqxq = 'http://www.zorba-xquery.com/modules/xqxq';

declare namespace resolver = 'http://www.zorba-xquery.com/modules/xqxq/url-resolver';
declare function resolver:url-resolver($namespace as xs:string, $entity as xs:string){
  if($entity = 'module')
  then
    if($namespace = 'http://test')
    then
      "module namespace test = 'http://test'; declare function test:foo(){<test:test><test:subtest>a</test:subtest><test:subtest2>a</test:subtest2></test:test>};"
    else
      ()
  else if ($entity = 'schema')
    then
      if($namespace = 'http://test')
      then
        doc('test.xsd')
      else
        ()
    else
      ()
};

variable $queryID := xqxq:prepare-main-module("import module namespace modtest = 'http://test'; import schema namespace test = 'http://test'; validate{let $x := modtest:foo() return $x}", fn:QName('http://www.zorba-xquery.com/modules/xqxq/url-resolver', 'resolver:url-resolver'));
xqxq:evaluate($queryID)                                                                             