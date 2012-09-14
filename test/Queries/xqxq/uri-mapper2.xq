import module namespace xqxq = 'http://www.zorba-xquery.com/modules/xqxq';

declare namespace resolver = 'http://www.zorba-xquery.com/modules/xqxq/url-resolver';
declare namespace mapper = 'http://www.zorba-xquery.com/modules/xqxq/uri-mapper';

declare function resolver:url-resolver($namespace as xs:string, $entity as xs:string) {
  if($namespace = 'http://test2' and $entity = 'schema')
  then 
    doc('test.xsd')
  else 
    ()
};

declare function mapper:uri-mapper($namespace as xs:string, $entity as xs:string)
{
  if ($namespace = 'http://test' and $entity = 'schema')
  then ('../../../zorba_modules/xqxq/test/Queries/xqxq/test.xsd','../../../zorba_modules/xqxq-module/test/Queries/xqxq/test.xsd')
  else ()
};

variable $queryID := xqxq:prepare-main-module
(
  "import schema namespace test = 'http://test'; validate{<test:test><test:subtest>a</test:subtest><test:subtest2>a</test:subtest2></test:test>}", 
  fn:QName('http://www.zorba-xquery.com/modules/xqxq/url-resolver', 'resolver:url-resolver'), 
  fn:QName('http://www.zorba-xquery.com/modules/xqxq/uri-mapper', 'resolver:uri-mapper')
);
xqxq:evaluate($queryID)                                                                             
