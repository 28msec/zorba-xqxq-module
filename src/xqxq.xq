xquery version "3.0";

module namespace xqxq = 'http://www.zorba-xquery.com/modules/xqxq';

declare namespace ann = "http://www.zorba-xquery.com/annotations";

declare namespace ver = "http://www.zorba-xquery.com/options/versioning";
declare option ver:module-version "1.0";

(:~
 : compile the input string as a main module, on an empty static and
 : dynamic context, returns an identifier/key for the prepared 
 : module -- as a string, a URI, or an integer
 :)
declare %ann:sequential function xqxq:prepare-main-module($main-module-text as xs:string) as 
  xs:QName external;

(:~
 : compile the input string as a library module, on an empty static and
 : dynamic context, returns an identifier/key for the prepared 
 : module or throws an error if it does not compile
 :)
declare %ann:sequential function xqxq:prepare-library-module($library-module-text as xs:string) as 
  xs:QName external;


(:~
 :
 :)
declare function xqxq:context-item-unbound($query-key as xs:QName) as
  xs:string  external;

(:~
 :
 :)
declare function xqxq:unbound-variables($query-key as xs:string) as
  xs:QName* external ;
  
(:~
 :
 :)
declare function xqxq:is-updating($query-key as xs:QName) as
  xs:boolean external;  

(:~
 :
 :)
declare function xqxq:is-side-effecting($query-key as xs:QName) as
  xs:boolean external;
  
(:~
 :
 :)
declare %ann:sequential function xqxq:bind-context-item($query-key as xs:QName,
  $dot as item()) as empty-sequence() external ;

(:~
 :
 :)
declare %ann:sequential function xqxq:bind-context-position($query-key as
  xs:QName, $position as xs:integer) as empty-sequence() external ;

(:~
 :
 :)
declare %ann:sequential function xqxq:bind-context-size($query-key as xs:QName,
  $last as xs:integer) as empty-sequence() external ;

(:~
 :
 :)
declare %ann:sequential function xqxq:bind-variable($query-key as xs:QName,
  $var as xs:QName, $value as item()*) as empty-sequence() external ;


(:~
 : in case the main expression of the module is a simple expression 
 :)
declare function xqxq:evaluate($query-key as xs:QName) as item()* external;

(:~ 
 : in case the main expression of the module is an updating
 : expression 
 :)
declare updating function xqxq:evaluate-updating($query-key as xs:QName) external;

(:~ 
 : in case the main expression of the module is an updating
 : expression, get the PUL and then apply it 
 :)
declare %ann:sequential function xqxq:evaluate-updating-and-apply($query-key as 
  xs:QName) as empty-sequence() external;

(:~ 
 : in case the main expression of the module is a sideffecting
 : expression 
 :)
declare %ann:sequential function xqxq:evaluate-side-effecting($query-key as
  xs:QName) as item()* external;
  
(:~
 :
 :)
declare %ann:sequential function xqxq:delete-query($query-key as xs:QName) as
  empty-sequence() external;