import module namespace xqxq = 'http://www.zorba-xquery.com/modules/xqxq';

variable $query-key := xqxq:prepare-main-module('
import module namespace ddl = "http://www.zorba-xquery.com/modules/store/dynamic/collections/ddl";
declare namespace ann = "http://www.zorba-xquery.com/annotations";
declare %ann:sequential function local:foo() {
  ddl:create(xs:QName("hola"),(<a/>,<b/>));                  
  
  exit returning true();
};

local:foo()
');    
variable $xml := doc("context-item.xml");
xqxq:bind-context-item($query-key, $xml);  
xqxq:evaluate-sequential($query-key)
 
