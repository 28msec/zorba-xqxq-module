#include <zorba\item_factory.h>
#include <zorba\singleton_item_sequence.h>
#include <zorba\diagnostic_list.h>
#include <zorba\empty_sequence.h>
#include <zorba\store_manager.h>
#include <zorba\user_exception.h>
#include <zorba\vector_item_sequence.h>
#include <zorba\serializer.h>
#include <zorba\xquery.h>

#include <vector>

#include "xqxq.h"

namespace zorba { namespace xqxq {

  /*******************************************************************************************
  *******************************************************************************************/
  
  ItemFactory* XQXQModule::theFactory = 0;

  XQXQModule::XQXQModule()
    : thePrepareMainModuleFunction(0), theIsBoundVariableFunction(0), 
      theGetExternalVariablesFunction(0), theIsUpdatingFunction(0), 
      theIsSequentialFunction(0), theBindContextItemFunction(0),
      theBindContextPositionFunction(0), theBindContextSizeFunction(0), 
      theBindVariableFunction(0), theEvaluateFunction(0), 
      theEvaluateUpdatingFunction(0), thePrepareLibraryModuleFunction(0), 
      theEvaluateSequentialFunction(0), theDeleteQueryFunction(0),
      theIsBoundContextItemFunction(0)
  {
     
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
      else if(localName == "is-bound-context-item"){
        if(!theIsBoundContextItemFunction){
          theIsBoundContextItemFunction = new IsBoundContextItemFunction(this);
        return theIsBoundContextItemFunction;
        }
      }
      else if(localName == "is-bound-variable"){
        if(!theIsBoundVariableFunction){
          theIsBoundVariableFunction = new IsBoundVariableFunction(this);
        return theIsBoundVariableFunction;
        }
      }
      else if(localName == "get-external-variables"){
        if(!theGetExternalVariablesFunction){
          theGetExternalVariablesFunction = new GetExternalVariablesFunction(this);
        return theGetExternalVariablesFunction;
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
   
    QueryMap* lQueryMap;
    if(!(lQueryMap = dynamic_cast<QueryMap*>(lDynCtx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      lQueryMap = new QueryMap();
      lDynCtx->addExternalFunctionParameter("xqxqQueryMap", lQueryMap);     
    }

    Zorba *lZorba = Zorba::getInstance(0);

    std::string lQueryString = getOneStringArgument(aArgs, 0).c_str(); 
    
    StaticContext_t lStaticContext = lZorba->createStaticContext();

    XQuery_t lQuery = lZorba->compileQuery(lQueryString, lStaticContext);
    
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
    unsigned int qLength = query.length();
    for (unsigned int i = 0; i < qLength; i++) 
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
    Zorba *lZorba = Zorba::getInstance(0);
    std::string lQueryString = getOneStringArgument(aArgs, 0).c_str();     
    //DiagnosticHandler* a = new DiagnosticHandler();
    //XQuery_t lQuery = lZorba->createQuery(a);   
    try
    {
    XQuery_t lQuery = lZorba->compileQuery(lQueryString);
    std::stringstream streamRes;
    
      streamRes << lQuery;
    }
    catch(ZorbaException* e)
    {
      XQXQFunction::throwError("LibraryModuleError", e->what());
    }
    catch(...)
    {
      XQXQFunction::throwError("LibraryModuleError", "LibraryModule does not compile.");
    }
    return ItemSequence_t(new EmptySequence());
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    IsBoundContextItemFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {

    String lQueryID = XQXQFunction::getOneStringArgument(aArgs, 0);

    QueryMap* lQueryMap;
    if(!(lQueryMap= dynamic_cast<QueryMap*>(aDctx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }

    XQuery_t lQuery;
    if(!(lQuery = lQueryMap->getQuery(lQueryID)))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");

    Item lContextItem;

    //Remove try when bug ##### in launchpad is solved
    bool lIsContextItemBound;
    try
    {
      lIsContextItemBound = lQuery->getDynamicContext()->getContextItem(lContextItem);
    }
    catch (ZorbaException& ze)
    {
      if (!strcmp("XPDY0002",ze.diagnostic().qname().localname()))
        lIsContextItemBound = false;
      else
        XQXQFunction::throwError(ze.diagnostic().qname().localname(), ze.diagnostic().message()); 
    }
    return ItemSequence_t(new SingletonItemSequence(Zorba::getInstance(0)->getItemFactory()->createBoolean(lIsContextItemBound)));
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    IsBoundVariableFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    String lQueryID = XQXQFunction::getOneStringArgument(aArgs, 0);

    QueryMap* lQueryMap;
    if(!(lQueryMap= dynamic_cast<QueryMap*>(aDctx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }

    XQuery_t lQuery;
    if(!(lQuery = lQueryMap->getQuery(lQueryID)))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");

    Item lVarQName = XQXQFunction::getItemArgument(aArgs, 1);

    Item lVar;
    Iterator_t lVarIter;

    bool lIsBoundVariable;
    try
    {
      lIsBoundVariable = lQuery->getDynamicContext()->getVariable(lVarQName.getNamespace(),lVarQName.getLocalName(), lVar, lVarIter);
    }
    catch (ZorbaException& ze)
    {
      
      if (!strcmp("XPDY0002",ze.diagnostic().qname().localname()))
        lIsBoundVariable = false;
      else
        XQXQFunction::throwError(ze.diagnostic().qname().localname(), ze.diagnostic().message());  
    }
    return ItemSequence_t(new SingletonItemSequence(Zorba::getInstance(0)->getItemFactory()->createBoolean(lIsBoundVariable)));
  
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    GetExternalVariablesFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    
    String lQueryID = XQXQFunction::getOneStringArgument(aArgs, 0);

    QueryMap* lQueryMap;
    if(!(lQueryMap= dynamic_cast<QueryMap*>(aDctx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }

    XQuery_t lQuery;
    if(!(lQuery = lQueryMap->getQuery(lQueryID)))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");
    
    std::vector<Item> lVars;
    lQuery->getExternalVariables(lVars);
    return ItemSequence_t(new VectorItemSequence(lVars));
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

    QueryMap* lQueryMap;
    if(!(lQueryMap= dynamic_cast<QueryMap*>(aDctx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }

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
    String lQueryID = XQXQFunction::getOneStringArgument(aArgs, 0);

    QueryMap* lQueryMap;
    if(!(lQueryMap= dynamic_cast<QueryMap*>(aDctx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }

    XQuery_t lQuery;
    if(!(lQuery = lQueryMap->getQuery(lQueryID)))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");

    return ItemSequence_t(new SingletonItemSequence(Zorba::getInstance(0)->getItemFactory()->createBoolean(lQuery->isSequential())));
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

    QueryMap* lQueryMap;
    if(!(lQueryMap= dynamic_cast<QueryMap*>(aDctx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }

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

    QueryMap* lQueryMap;
    if(!(lQueryMap= dynamic_cast<QueryMap*>(aDctx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }

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

    QueryMap* lQueryMap;
    if(!(lQueryMap= dynamic_cast<QueryMap*>(aDctx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }

    XQuery_t lQuery;
    if(!(lQuery = lQueryMap->getQuery(lQueryID)))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");

    if(lQuery->isUpdating())
      XQXQFunction::throwError("QueryIsUpdating", "Executing Query shouldn't be updating.");
    
   
    if(lQuery->isSequential())
      XQXQFunction::throwError("QueryIsSequential", "Executing Query shouldn't be sequential.");
      

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

    QueryMap* lQueryMap;
    if(!(lQueryMap= dynamic_cast<QueryMap*>(aDctx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }

    XQuery_t lQuery;
    if(!(lQuery = lQueryMap->getQuery(lQueryID)))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");
    
    //Uncomment as soon as isSequential is designed
    if(lQuery->isSequential())
      XQXQFunction::throwError("QueryIsSequential", "Executing Query shouldn't be sequential.");   

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

    QueryMap* lQueryMap;
    if(!(lQueryMap= dynamic_cast<QueryMap*>(aDctx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }

    XQuery_t lQuery;
    if(!(lQuery = lQueryMap->getQuery(lQueryID)))
      XQXQFunction::throwError("NoQueryMatch","String identifying query does not exists.");

    Item item;
    lQuery->getDynamicContext()->getContextItem(item);
    
    if(lQuery->isUpdating())
    {
      XQXQFunction::throwError("QueryIsUpdating", "Executing Query shouldn't be updating.");
    }


    if(!lQuery->isSequential())
      XQXQFunction::throwError("QueryNotSequential", "Executing Query should be sequential.");    

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

    QueryMap* lQueryMap;
    if(!(lQueryMap= dynamic_cast<QueryMap*>(aDctx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      XQXQFunction::throwError("QueryMapNotFound", "There are no queries, be sure to call prepare-main-module first.");
    }

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