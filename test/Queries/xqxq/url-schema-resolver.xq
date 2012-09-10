import module namespace xqxq = 'http://www.zorba-xquery.com/modules/xqxq';

declare namespace resolver = 'http://www.zorba-xquery.com/modules/xqxq/url-resolver';
declare function resolver:url-resolver($namespace as xs:string, $entity as xs:string) as xs:string? {
  if($namespace = 'http://test' and $entity = 'schema')
  then '<?xml version="1.0"?> 
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
  targetNamespace="http://test"
  xmlns="http://test"
  elementFormDefault="qualified">
  <xs:element name="test">
    <xs:complexType>
      <xs:sequence>
        <xs:element name="subtest" type="xs:string"/>
        <xs:element name="subtest2" type="xs:string"/>
      </xs:sequence>  
    </xs:complexType>
  </xs:element>
</xs:schema>'
  else ()
};

variable $queryID := xqxq:prepare-main-module("import schema namespace test = 'http://test'; validate {<test:test><test:subtest>a</test:subtest><test:subtest2>a</test:subtest2></test:test>}", fn:QName('http://www.zorba-xquery.com/modules/xqxq/url-resolver', 'resolver:url-resolver'));
xqxq:evaluate($queryID)                                                                             
