#include <zorba\item_factory.h>
#include <zorba\singleton_item_sequence.h>
#include <zorba\diagnostic_list.h>
#include <zorba\empty_sequence.h>
#include <zorba\store_manager.h>
#include <zorba\user_exception.h>
#include <zorba\vector_item_sequence.h>
#include <zorba\serializer.h>

#include <vector>

#include "xqxq.h"

namespace zorba { namespace xqxq {

  /*******************************************************************************************
  *******************************************************************************************/
  
  ItemFactory* XQXQModule::theFactory = 0;

  XQXQModule::XQXQModule()
    : thePrepareMainModuleFunction(0), theIsContextItemUnboundFunction(0), 
      theGetUnboundVariablesFunction(0), theIsUpdatingFunction(0), 
      theIsSequentialFunction(0), theBindContextItemFunction(0),
      theBindContextPositionFunction(0), theBindContextSizeFunction(0), 
      theBindVariableFunction(0), theEvaluateFunction(0), 
      theEvaluateUpdatingFunction(0), thePrepareLibraryModuleFunction(0), 
      theEvaluateSequentialFunction(0), theDeleteQueryFunction(0)
  {
     queryMap = new QueryMap_t();
  }

  zorba::ExternalFunction*
    XQXQModule::getExternalFunction(const zorba::String& localName)
  {
      if(localName == "prepare-main-module"){
        if(!thePrepareMainModuleFunction){
          thePrepareMainModuleFunction = new PrepareMainModuleFunction(this);
        return thePrepareMainModuleFunction;
        } 
      }
      if(localName == "prepare-library-module"){
        if(!thePrepareLibraryModuleFunction){
          thePrepareLibraryModuleFunction = new PrepareLibraryModuleFunction(this);
        return thePrepareLibraryModuleFunction;
        } 
      }
      else if(localName == "context-item-unbound"){
        if(!theIsContextItemUnboundFunction){
          theIsContextItemUnboundFunction = new IsContextItemUnboundFunction(this);
        return theIsContextItemUnboundFunction;
        }
      }
      else if(localName == "unbound-variables"){
        if(!theGetUnboundVariablesFunction){
          theGetUnboundVariablesFunction = new GetUnboundVariablesFunction(this);
        return theGetUnboundVariablesFunction;
        }
      }
      else if(localName == "is-updating"){
        if(!theIsUpdatingFunction){
          theIsUpdatingFunction = new IsUpdatingFunction(this);
        return theIsUpdatingFunction;
        }
      }
      else if(localName == "is-sequential"){
        if(!theIsSequentialFunction){
          theIsSequentialFunction = new IsSequentialFunction(this);
        return theIsSequentialFunction;
        }
      }      
      else if(localName == "bind-context-item"){
        if(!theBindContextItemFunction){
          theBindContextItemFunction = new BindContextItemFunction(this);
        return theBindContextItemFunction;
        }
      }
      else if(localName == "bind-context-position"){
        if(!theBindContextPositionFunction){
          theBindContextPositionFunction = new BindContextPositionFunction(this);
        return theBindContextPositionFunction;
        }
      }
      else if(localName == "bind-context-size"){
        if(!theBindContextSizeFunction){
          theBindContextSizeFunction = new BindContextSizeFunction(this);
        return theBindContextSizeFunction;
        }
      }
      else if(localName == "bind-variable"){
        if(!theBindVariableFunction){
          theBindVariableFunction = new BindVariableFunction(this);
        return theBindVariableFunction;
        }
      }
      else if(localName == "evaluate"){
        if(!theEvaluateFunction){
          theEvaluateFunction = new EvaluateFunction(this);
        return theEvaluateFunction;
        }
      }
      else if(localName == "evaluate-updating"){
        if(!theEvaluateUpdatingFunction){
          theEvaluateUpdatingFunction = new EvaluateUpdatingFunction(this);
        return theEvaluateUpdatingFunction;
        }
      }
      else if(localName == "evaluate-sequential"){
        if(!theEvaluateSequentialFunction){
          theEvaluateSequentialFunction = new EvaluateSequentialFunction(this);
        return theEvaluateSequentialFunction;
        }
      }
      else if(localName == "delete-query"){
        if(!theDeleteQueryFunction){
          theDeleteQueryFunction = new DeleteQueryFunction(this);
        return theDeleteQueryFunction;
        }
      }
  }

  void XQXQModule::destroy() 
  {
    delete this;
  }

  XQXQModule::~XQXQModule()
  {
    delete thePrepareMainModuleFunction;

    for (QueryMap_t::const_iterator lIter = queryMap->begin();
      lIter != queryMap->end(); ++lIter)
    {
      delete lIter->second;
    }
    queryMap->clear();
  }

  /*******************************************************************************************
  *******************************************************************************************/
  XQXQFunction::XQXQFunction(const XQXQModule* aModule)
    : theModule(aModule)
  {
   
  }

  XQXQFunction::~XQXQFunction()
  {
    
  }

  String XQXQFunction::getURI() const 
  {
    return theModule->getURI();
  }

  String
    XQXQFunction::getOneStringArgument(const Arguments_t& aArgs, int aPos) const
  {
    Item lItem;
    Iterator_t args_iter = aArgs[aPos]->getIterator();
    args_iter->open();
    args_iter->next(lItem);
    String lTmpString = lItem.getStringValue();
    args_iter->close();
    return lTmpString;
  }

  Item
    XQXQFunction::getItemArgument(const Arguments_t& aArgs, int aPos) const
  {
    Item lItem;
    Iterator_t args_iter = aArgs[aPos]->getIterator();
    args_iter->open();
    args_iter->next(lItem);
    args_iter->close();

    return lItem;
  }

  void
    XQXQFunction::throwError(const char *err_localname, const std::string aErrorMessage)
  {
    String errNS(XQXQ_MODULE_NAMESPACE);
    String errName(err_localname);
    Item errQName = XQXQModule::getItemFactory()->createQName(errNS, errName);
    String errDescription(aErrorMessage);
    throw USER_EXCEPTION(errQName, errDescription);
  }
  
  /*******************************************************************************************
  *******************************************************************************************/
  
  QueryMap::QueryMap()
  {
    QueryMap::queryMap = new QueryMap_t();
  }

  bool 
    QueryMap::storeQuery(String aKeyName, XQuery_t aQuery)
  {
    std::pair<QueryMap_t::iterator,bool> ret;
    ret = queryMap->insert(std::pair<std::string, XQuery_t>(aKeyName.c_str(), aQuery));
    
    if(ret.second == false)
      return false;

    return true;
  }

  XQuery_t
    QueryMap::getQuery(String aKeyName)
  {
    QueryMap::QueryMap_t::iterator lIter = queryMap->find(aKeyName.c_str());

    if(lIter == queryMap->end())
      return NULL;
    
    XQuery_t lQuery = lIter->second;

    return lQuery;
  }

  bool
    QueryMap::deleteQuery(String aKeyName)
  {
    QueryMap::QueryMap_t::iterator lIter = queryMap->find(aKeyName.c_str());

    if(lIter == queryMap->end())
      return false;
    
    lIter->second->close();

    queryMap->erase(lIter);

    return true;
  }
  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    PrepareMainModuleFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {

    DynamicContext* lDynCtx = const_cast<DynamicContext*>(aDctx);
    void* lMap;
    QueryMap* lQueryMap;
    if(!lDynCtx->getExternalFunctionParam("querymap", lMap))
    {
      lQueryMap = new QueryMap();
      lDynCtx->addExternalFunctionParam("querymap", lQueryMap);     
    }
    else
    {
      lQueryMap = static_cast<QueryMap*>(lMap);
    }

    Zorba *lZorba = Zorba::getInstance(0);

    std::string lQueryString = getOneStringArgument(aArgs, 0).c_str(); 
    DiagnosticHandler* a = new DiagnosticHandler();
    XQuery_t lQuery = lZorba->createQuery(a);   
    lQuery->compile(lQueryString);
    
    String lKey = getKey(lQueryString);
    
    if(!lQueryMap->storeQuery(lKey, lQuery))
    {
      throwError("QueryAlreadyCreated", "This Query has already been created.");
    }
    
    return ItemSequence_t(new SingletonItemSequence(XQXQModule::getItemFactory()->createString(lKey)));
  }

  String 
    PrepareMainModuleFunction::getKey(std::string query)
  {
    unsigned int hash = 0;
    for (unsigned int i = 0; i < query.length(); i++) 
    {
      hash = hash * 31 + query.at(i);
    }
    std::stringstream hashS;
    hashS << hash;
    
    std::string key = hashS.str();

    //Check if QNAME already in tree

    return key;
  }


  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    PrepareLibraryModuleFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    DynamicContext* lDynCtx = const_cast<DynamicContext*>(aDctx);
    void* lMap;
    QueryMap* lQueryMap;
    if(!lDynCtx->getExternalFunctionParam("querymap", lMap))
    {
      lQueryMap = new QueryMap();
      lDynCtx->addExternalFunctionParam("querymap", lQueryMap);     
    }
    else
    {
      lQueryMap = static_cast<QueryMap*>(lMap);
    }

    Zorba *lZorba = Zorba::getInstance(0);
    std::string lQueryString = getOneStringArgument(aArgs, 0).c_str();     
    DiagnosticHandler* a = new DiagnosticHandler();
    XQuery_t lQuery = lZorba->createQuery(a);   
    lQuery->compile(lQueryString);
    
    std::stringstream streamRes;
    try
    {
      streamRes << lQuery;
    }
    catch(ZorbaException* e)
    {
      Item lErrorQName = XQXQModule::getItemFactory()->createQName(e->diagnostic().qname().ns(),e->diagnostic().qname().localname());
      return ItemSequence_t(new SingletonItemSequence(lErrorQName));
    }
    
    String lKey = getKey(lQueryString);
    
    if(!lQueryMap->storeQuery(lKey, lQuery))
    {
      throwError("QueryAlreadyCreated", "This Query has already been created.");
    }
    
    return ItemSequence_t(new SingletonItemSequence(XQXQModule::getItemFactory()->createString(lKey)));
  }

  String 
    PrepareLibraryModuleFunction::getKey(std::string query)
  {
    unsigned int hash = 0;
    for (unsigned int i = 0; i < query.length(); i++) 
    {
      hash = hash * 31 + query.at(i);
    }
    std::stringstream hashS;
    hashS << hash;
    
    std::string key = hashS.str();

    //Check if QNAME already in tree

    return key;
  }


  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    IsContextItemUnboundFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {

    XQXQFunction::throwError("ImplementationError", "This function is not implemented yet");

    return ItemSequence_t(new EmptySequence());
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    GetUnboundVariablesFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    XQXQFunction::throwError("ImplementationError", "This function is not implemented yet");

    return ItemSequence_t(new EmptySequence());
  }
  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    IsUpdatingFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    String lQueryID = XQXQFunction::getOneStringArgument(aArgs, 0);

    void* lMap;
    if(!aDctx->getExternalFunctionParam("querymap", lMap))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }
    QueryMap* lQueryMap = static_cast<QueryMap*>(lMap);

    XQuery_t lQuery;
    if(!(lQuery = lQueryMap->getQuery(lQueryID)))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");

    return ItemSequence_t(new SingletonItemSequence(Zorba::getInstance(0)->getItemFactory()->createBoolean(lQuery->isUpdating())));
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    IsSequentialFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    XQXQFunction::throwError("ImplementationError", "This function is not implemented yet");

    return ItemSequence_t(new EmptySequence());
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    BindContextItemFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    String lQueryID = XQXQFunction::getOneStringArgument(aArgs,0);

    void* lMap;
    if(!aDctx->getExternalFunctionParam("querymap", lMap))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }
    QueryMap* lQueryMap = static_cast<QueryMap*>(lMap);

    XQuery_t lQuery;
    if(!(lQuery = lQueryMap->getQuery(lQueryID)))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");

    Item lItemContext = XQXQFunction::getItemArgument(aArgs, 1);
    lQuery->getDynamicContext()->setContextItem(lItemContext);

    return ItemSequence_t(new EmptySequence());
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    BindContextPositionFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {

    XQXQFunction::throwError("ImplementationError", "This function is not implemented yet");

    return ItemSequence_t(new EmptySequence());
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    BindContextSizeFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {

    XQXQFunction::throwError("ImplementationError", "This function is not implemented yet");

    return ItemSequence_t(new EmptySequence());
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    BindVariableFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    String lQueryID = XQXQFunction::getOneStringArgument(aArgs,0);

    void* lMap;
    if(!aDctx->getExternalFunctionParam("querymap", lMap))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }
    QueryMap* lQueryMap = static_cast<QueryMap*>(lMap);

    XQuery_t lQuery;
    if(!(lQuery = lQueryMap->getQuery(lQueryID)))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");
        
    Item lVarQName = XQXQFunction::getItemArgument(aArgs, 1); 

    Item lVarValue = XQXQFunction::getItemArgument(aArgs, 2); 

    lQuery->getDynamicContext()->setVariable(lVarQName.getStringValue(), lVarValue);

    return ItemSequence_t(new EmptySequence());
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    EvaluateFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    String lQueryID = XQXQFunction::getOneStringArgument(aArgs,0);

    void* lMap;
    if(!aDctx->getExternalFunctionParam("querymap", lMap))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }
    QueryMap* lQueryMap = static_cast<QueryMap*>(lMap);

    XQuery_t lQuery;
    if(!(lQuery = lQueryMap->getQuery(lQueryID)))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");

    if(lQuery->isUpdating())
      XQXQFunction::throwError("QueryIsUpdating", "Executing Query shouldn't be updating.");
    
    //Uncomment as soon as isSequential is designed
    /*
    if(lQuery->isSequential())
      XQXQFunction::throwError("QueryIsSequential", "Executing Query shouldn't be sequential.");
    */

    std::vector<Item> lItemVector;

    Iterator_t lIterQuery = lQuery->iterator();
    lIterQuery->open();
    Item lItemQuery;
    while(lIterQuery->next(lItemQuery))
    {
      lItemVector.push_back(lItemQuery);
    }

    return ItemSequence_t(new VectorItemSequence(lItemVector));
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    EvaluateUpdatingFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    String lQueryID = XQXQFunction::getOneStringArgument(aArgs,0);

    void* lMap;
    if(!aDctx->getExternalFunctionParam("querymap", lMap))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }
    QueryMap* lQueryMap = static_cast<QueryMap*>(lMap);

    XQuery_t lQuery;
    if(!(lQuery = lQueryMap->getQuery(lQueryID)))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");

    Item item;
    lQuery->getDynamicContext()->getContextItem(item);

    //Uncomment as soon as isSequential is designed
    /*
    if(lQuery->isSequential())
      XQXQFunction::throwError("QueryIsSequential", "Executing Query shouldn't be sequential.");
    */

    if(!lQuery->isUpdating())
    {
      XQXQFunction::throwError("QueryNotUpdating", "Executing Query should be updating.") ; 
    }

    lQuery->execute();
    return ItemSequence_t(new EmptySequence());
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    EvaluateSequentialFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    String lQueryID = XQXQFunction::getOneStringArgument(aArgs,0);

    void* lMap;
    if(!aDctx->getExternalFunctionParam("querymap", lMap))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }
    QueryMap* lQueryMap = static_cast<QueryMap*>(lMap);

    XQuery_t lQuery;
    if(!(lQuery = lQueryMap->getQuery(lQueryID)))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");

    Item item;
    lQuery->getDynamicContext()->getContextItem(item);
    
    if(lQuery->isUpdating())
    {
      XQXQFunction::throwError("QueryIsUpdating", "Executing Query shouldn't be updating.");
    }

    //Uncomment as soon as isSequential is designed
    /*
    if(!lQuery->isSequential())
      XQXQFunction::throwError("QueryNotSequential", "Executing Query should be sequential.");
    */

    std::vector<Item> lItemVector;

    Iterator_t lIterQuery = lQuery->iterator();
    lIterQuery->open();
    Item lItemQuery;
    while(lIterQuery->next(lItemQuery))
    {
      lItemVector.push_back(lItemQuery);
    }

    return ItemSequence_t(new VectorItemSequence(lItemVector));
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    DeleteQueryFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    String lQueryID = XQXQFunction::getOneStringArgument(aArgs,0);

    void* lMap;
    if(!aDctx->getExternalFunctionParam("querymap", lMap))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }
    QueryMap* lQueryMap = static_cast<QueryMap*>(lMap);

    if(!lQueryMap->deleteQuery(lQueryID))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");
      
    return ItemSequence_t(new EmptySequence());
  }
 
}/*namespace xqxq*/ }/*namespace zorba*/

#ifdef WIN32
#  define DLL_EXPORT __declspec(dllexport)
#else
#  define DLL_EXPORT __attribute__ ((visibility("default")))
#endif

extern "C" DLL_EXPORT zorba::ExternalModule* createModule() {
  return new zorba::xqxq::XQXQModule();
}