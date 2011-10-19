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
  xs:string external;

(:~
 : compile the input string as a library module, on an empty static and
 : dynamic context, returns an identifier/key for the prepared 
 : module or throws an error if it does not compile
 :)
declare %ann:sequential function xqxq:prepare-library-module($library-module-text as xs:string) as 
  empty-sequence() external ;

(:~
 :  returns true if the context item is bound to a value
 :)
declare function xqxq:is-bound-context-item($query-key as xs:string) 
  as xs:boolean  external;
  

(:~
 :  returns true if the specified variable has been assigned
 :)
declare function xqxq:is-bound-variable($query-key as xs:string, $var-name as 
  xs:QName) as xs:boolean  external;



(:~
 :  returns a sequence of qnames of the variables that were defined as external
 :  in the query.
 :)
declare function xqxq:get-external-variables($query-key as xs:string) as
  xs:QName* external ;
  
(:~
 :  returns true if the specified query is an updating query
 :)
declare function xqxq:is-updating($query-key as xs:string) as
  xs:boolean external;  

(:~
 :  returns true if the specified query is a sequential query
 :)
declare function xqxq:is-sequential($query-key as xs:string) as
  xs:boolean external;
  
(:~
 :  used to assign a value to the context item of the dynamic context
 :  for the specified query
 :)
declare %ann:sequential function xqxq:bind-context-item($query-key as xs:string,
  $dot as item()) as empty-sequence() external ;

(:~
 :
 :)
declare %ann:sequential function xqxq:bind-context-position($query-key as
  xs:string, $position as xs:integer) as empty-sequence() external ;

(:~
 :
 :)
declare %ann:sequential function xqxq:bind-context-size($query-key as xs:string,
  $last as xs:integer) as empty-sequence() external ;

(:~
 :  used to assign a value to a external variable in the dynamic context
 :  of the specified query
 :)
declare %ann:sequential function xqxq:bind-variable($query-key as xs:string,
  $var as xs:QName, $value as item()*) as empty-sequence() external ;


(:~
 :  in case the main expression of the module is a simple expression 
 :)
declare function xqxq:evaluate($query-key as xs:string) as item()* external;

(:~ 
 :  in case the main expression of the module is an updating
 :  expression 
 :)
declare function xqxq:evaluate-updating($query-key as xs:string) external;

(:~ 
 :  in case the main expression of the module is a sideffecting
 :  expression 
 :)
declare %ann:sequential function xqxq:evaluate-sequential($query-key as
  xs:string) as item()* external;
  
(:~
 :  used to delete query from the module
 :)
declare %ann:sequential function xqxq:delete-query($query-key as xs:string) as
  empty-sequence() external;