xquery version "3.0";

module namespace xqxq = 'http://www.zorba-xquery.com/modules/xqxq';

declare namespace ann = "http://www.zorba-xquery.com/annotations";

declare namespace ver = "http://www.zorba-xquery.com/options/versioning";
declare option ver:module-version "1.0";

(:~
 : The function prepares a given XQuery program for execution.
 : If the program was successfully compiled, the function returns an
 : identifier as xs:anyURI. This URI can be passed to other functions
 : of this module (e.g. to actually evaluate the program). The URI
 : is opaque and its lilfetime is bound by the lifetime of the XQuery
 : program that invoked this function. Further reference or uses
 : of the identifier lead to unexpected results.
 :
 : Successfully prepared queries need to be deleted by passing the resulting
 : identifier to the xqxq:delete-query function of this module.
 :
 : @param $main-module-text the XQuery program that should be prepared.
 :   The program needs to be a XQuery main module.
 :
 : @return an identifier for the compiled program that can be passed
 :   as arguments to other functions of this module.
 :
 : @error any (static or type) error that may be raised during the compilation
 : of the query. For example, err:XPST0003 if the given XQuery program could
 : not be parsed.
 :)
declare %ann:sequential function xqxq:prepare-main-module($main-module-text as xs:string) as 
  xs:anyURI external;

(:~
 : This function compiles a given XQuery library module. It can be used
 : to compile-check a module. 
 :
 : @param $library-module-text the XQuery library module that should
 :  be prepared. 
 :
 : @return the function is declared as sequential.It returns the
 :  empty-sequence.
 :
 : @error any (static or type) error that may be raised during the compilation
 : of the library module. For example, err:XPST0003 if the given XQuery library
 : module could not be parsed.
 :)
declare %ann:sequential function xqxq:prepare-library-module($library-module-text as xs:string) as 
  empty-sequence() external ;

(:~
 : The function tests if the context-item is bound for the
 : execution of the query referred to by the given query identifier.
 :
 : @param $query-key the identifier for a compiled query
 :
 : @return true if the context-item is bound, false otherwise.
 :
 : @error xqxq:NoQueryMatch if no query with the given identifier
 :   was prepared.
 :)
declare function xqxq:is-bound-context-item($query-key as xs:anyURI) 
  as xs:boolean  external;
  

(:~
 : The function tests if the given variable is bound for the
 : execution of the query referred to by the given query identifier.
 :
 : @param $query-key the identifier for a compiled query
 :
 : @return true if the variable is bound, false otherwise.
 :
 : @error xqxq:NoQueryMatch if no query with the given identifier
 :   was prepared.
 :)
declare function xqxq:is-bound-variable($query-key as xs:anyURI, $var-name as 
  xs:QName) as xs:boolean  external;

(:~
 : The function returns the names of the external variables that
 : are declared in the given query (either in the main module or
 : in any of the imported library modules).
 :
 : @param $query-key the identifier for a compiled query
 :
 : @return the sequence of names of the said external variables.
 :
 : @error xqxq:NoQueryMatch if no query with the given identifier
 :   was prepared.
 :)
declare function xqxq:external-variables($query-key as xs:anyURI) as
  xs:QName* external ;
  
(:~
 : The function tests if the query identified by the given key
 : is an updating query.
 :
 : @param $query-key the identifier for a compiled query
 :
 : @return true if the query is an updating query, false otherwise.
 :
 : @error xqxq:NoQueryMatch if no query with the given identifier
 :   was prepared.
 :)
declare function xqxq:is-updating($query-key as xs:anyURI) as
  xs:boolean external;  

(:~
 : The function tests if the query identified by the given key
 : is sequential query.
 :
 : @param $query-key the identifier for a compiled query
 :
 : @return true if the query is a sequential, false otherwise.
 :
 : @error xqxq:NoQueryMatch if no query with the given identifier
 :   was prepared.
 :)
declare function xqxq:is-sequential($query-key as xs:anyURI) as
  xs:boolean external;
  
(:~
 : This function binds the context-item of the prepared query
 : identified by the given key to the $dot argument.
 :
 : @param $query-key the identifier for a compiled query
 : @param $dot the context item to bind
 :
 : @return the function has side effects and returns the empty
 :   sequence.
 :
 : @error xqxq:NoQueryMatch if no query with the given identifier
 :   was prepared.
 :)
declare %ann:sequential function xqxq:bind-context-item($query-key as xs:anyURI,
  $dot as item()) as empty-sequence() external ;

(:~
 : This function binds the variable with name $name of
 : the prepared query identified by $query-key to the given sequence.
 :
 : @param $query-key the identifier for a compiled query
 : @param $name the name of the external variable to bind
 : @param $value the sequence to which the external variable $name
 :  should be bound
 :
 : @return the function has side effects and returns the empty
 :   sequence.
 :
 : @error xqxq:NoQueryMatch if no query with the given identifier
 :   was prepared.
 :)
declare %ann:sequential function xqxq:bind-variable($query-key as xs:anyURI,
  $var as xs:QName, $value as item()*) as empty-sequence() external ;


(:~
 : Evaluates the given prepared query and returns the result
 : of the evaluation. The query must not be sequential or
 : updating.
 :
 : @param $query-key the identifier for a compiled query
 :
 : @return the result of evaluating the given query
 :
 : @error xqxq:NoQueryMatch if no query with the given identifier
 :   was prepared.
 :
 : @error xqxq:QueryIsUpdating if the query is an updating query.
 :
 : @error xqxq:QueryIsSequential if the query is sequential.
 :
 : @error any dynamic error that is raised by evaluating the
 :   given query.
 :
 :)
declare function xqxq:evaluate($query-key as xs:anyURI) as item()* external;

(:~
 : Evaluates the given prepared query and applies the updates
 : computed by this query. The query must be an updating query.
 :
 : @param $query-key the identifier for a compiled query
 :
 : @return the function has side effects because it applies
 :  the updates of the query. It returns the empty sequence.
 :
 : @error xqxq:NoQueryMatch if no query with the given identifier
 :   was prepared.
 :
 : @error xqxq:QueryNotUpdating if the query is not an updating query.
 :
 : @error xqxq:QueryIsSequential if the query is sequential.
 :
 : @error any dynamic error that is raised by evaluating the
 :   given query or applying its updates.
 :
 :)
declare function xqxq:evaluate-updating($query-key as xs:anyURI) external;

(:~ 
 : Evaluates the given prepared query and returns the result
 : of the evaluation. The query must be sequential.
 :
 : @param $query-key the identifier for a compiled query
 :
 : @return the result of evaluating the query.
 :
 : @error xqxq:NoQueryMatch if no query with the given identifier
 :   was prepared.
 :
 : @error xqxq:QueryNotSequential if the query is not sequential.
 :
 : @error xqxq:QueryIsUpdating if the query is an updating query.
 :
 : @error any dynamic error that is raised by evaluating the
 :   given query.
 :
 :)
declare %ann:sequential function xqxq:evaluate-sequential($query-key as
  xs:string) as item()* external;
  
(:~ 
 : Deletes the prepared query associated with the given identifier.
 : After the query is deleted, the corresponding identifier should
 : not be used as argument to any of the functions of this module.
 :
 : @param $query-key the identifier for a compiled query
 :
 : @return the function has side effects and returns the empty sequence.
 :
 : @error xqxq:NoQueryMatch if no query with the given identifier
 :   was prepared.
 :
 :)
declare %ann:sequential function xqxq:delete-query($query-key as xs:anyURI) as
  empty-sequence() external;
