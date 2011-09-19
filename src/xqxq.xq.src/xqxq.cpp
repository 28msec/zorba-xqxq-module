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
  zorba::ItemSequence_t
    PrepareMainModuleFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    Zorba *lZorba = Zorba::getInstance(0);

    std::string lQueryString = getOneStringArgument(aArgs, 0).c_str(); 

    DiagnosticHandler* a = new DiagnosticHandler();

    XQuery_t lQuery = lZorba->createQuery(a);
    
    lQuery->compile(lQueryString);
    
    Item lKey = getKey(lQueryString);

    theModule->queryMap->insert(std::pair<std::string, XQuery_t>(lKey.getStringValue().c_str(), lQuery));
    
    return ItemSequence_t(new SingletonItemSequence(lKey));
  }

  Item 
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
    String queryNamespace(XQXQ_QUERY_KEY_NAMESPACE);
    String queryId("ID"+key);

    Item keyQName = XQXQModule::getItemFactory()->createQName(queryNamespace, queryId);

    //Check if QNAME already in tree

    return keyQName;
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

    Zorba_CompilerHints_t lHints;
    lHints.lib_module = true;

    XQuery_t lQuery = lZorba->compileQuery(lQueryString, lHints);
    
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
    Item lKey = getKey(lQueryString);

    theModule->queryMap->insert(std::pair<std::string, XQuery_t>(lKey.getStringValue().c_str(), lQuery));
    
    return ItemSequence_t(new SingletonItemSequence(lKey));
  }

  Item 
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
    String queryNamespace(XQXQ_QUERY_KEY_NAMESPACE);
    String queryId("ID"+key);

    Item keyQName = XQXQModule::getItemFactory()->createQName(queryNamespace, queryId);

    //Check if QNAME already in tree

    return keyQName;
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

    Item lQName = XQXQFunction::getItemArgument(aArgs,0);

    if(lQName.getNamespace().compare(XQXQ_QUERY_KEY_NAMESPACE))
      XQXQFunction::throwError("WrongQNameNamespace", "QName assigned to parameter:1 has a wrong namespace.");

    XQXQModule::QueryMap_t::iterator lIter = theModule->queryMap->find(lQName.getStringValue().c_str());

    if(lIter == theModule->queryMap->end())
      XQXQFunction::throwError("NoQueryMatch","QName identifying query does not exists.");
    
    XQuery_t lQuery = lIter->second;

    return ItemSequence_t(new SingletonItemSequence(Zorba::getInstance(0)->getItemFactory()->createBoolean(lQuery->isUpdating())));
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    IsSideEffectingFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    return ItemSequence_t(new SingletonItemSequence(Zorba::getInstance(0)->getItemFactory()->createInteger(4)));
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    BindContextItemFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    Item lQName = XQXQFunction::getItemArgument(aArgs,0);

    if(lQName.getNamespace().compare(XQXQ_QUERY_KEY_NAMESPACE))
      XQXQFunction::throwError("WrongQNameNamespace", "QName assigned to parameter:1 has a wrong namespace.");

    XQXQModule::QueryMap_t::iterator lIter = theModule->queryMap->find(lQName.getStringValue().c_str());

    if(lIter == theModule->queryMap->end())
      XQXQFunction::throwError("NoQueryMatch","QName identifying query does not exists.");
    
    XQuery_t lQuery = lIter->second;

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
    Item lQName = XQXQFunction::getItemArgument(aArgs,0);

    if(lQName.getNamespace().compare(XQXQ_QUERY_KEY_NAMESPACE))
      XQXQFunction::throwError("WrongQNameNamespace", "QName assigned to parameter:1 has a wrong namespace.");

    XQXQModule::QueryMap_t::iterator lIter = theModule->queryMap->find(lQName.getStringValue().c_str());

    if(lIter == theModule->queryMap->end())
      XQXQFunction::throwError("NoQueryMatch","QName identifying query does not exists.");
    
    XQuery_t lQuery = lIter->second;
        
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
    Item lQName = XQXQFunction::getItemArgument(aArgs, 0);

    if(lQName.getNamespace().compare(XQXQ_QUERY_KEY_NAMESPACE))
      XQXQFunction::throwError("WrongQNameNamespace", "QName assigned to parameter:1 has a wrong namespace.");

    XQXQModule::QueryMap_t::iterator lIter = theModule->queryMap->find(lQName.getStringValue().c_str());

    if(lIter == theModule->queryMap->end())
      XQXQFunction::throwError("NoQueryMatch","QName identifying query does not exists.");
    
    XQuery_t lQuery = lIter->second;

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
    Item lQName = XQXQFunction::getItemArgument(aArgs, 0);

    if(lQName.getNamespace().compare(XQXQ_QUERY_KEY_NAMESPACE))
      XQXQFunction::throwError("WrongQNameNamespace", "QName assigned to parameter:1 has a wrong namespace.");

    XQXQModule::QueryMap_t::iterator lIter = theModule->queryMap->find(lQName.getStringValue().c_str());

    if(lIter == theModule->queryMap->end())
      XQXQFunction::throwError("NoQueryMatch","QName identifying query does not exists.");
    
    XQuery_t lQuery = lIter->second;

    Item item;
    lQuery->getDynamicContext()->getContextItem(item);

    if(lQuery->isUpdating())
    {
      lQuery->execute();
    }

    std::vector<Item> lItemVector;

    return ItemSequence_t(new EmptySequence());
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    EvaluateAndApplyFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    return ItemSequence_t(new SingletonItemSequence(Zorba::getInstance(0)->getItemFactory()->createInteger(4)));
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    EvaluateSideEffectingFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    return ItemSequence_t(new SingletonItemSequence(Zorba::getInstance(0)->getItemFactory()->createInteger(4)));
  }

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    DeleteQueryFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    Item lQName = XQXQFunction::getItemArgument(aArgs, 0);
    XQXQModule::QueryMap_t::iterator lIter = theModule->queryMap->find(lQName.getStringValue().c_str());
    if(lIter == theModule->queryMap->end())
      XQXQFunction::throwError("NoQueryMatch","QName identifying query, does not exists.");
    
    lIter->second->close();
    theModule->queryMap->erase(lIter);
    
    return ItemSequence_t(new EmptySequence());
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
  
  ItemFactory* XQXQModule::theFactory = 0;

  XQXQModule::XQXQModule()
    : thePrepareMainModuleFunction(0), theIsContextItemUnboundFunction(0), 
      theGetUnboundVariablesFunction(0), theIsUpdatingFunction(0), 
      theIsSideEffectingFunction(0), theBindContextItemFunction(0),
      theBindContextPositionFunction(0), theBindContextSizeFunction(0), 
      theBindVariableFunction(0), theEvaluateFunction(0), 
      theEvaluateUpdatingFunction(0), theEvaluateAndApplyFunction(0), 
      theEvaluateSideEffectingFunction(0), theDeleteQueryFunction(0),
      thePrepareLibraryModuleFunction(0)
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
      else if(localName == "is-side-effecting"){
        if(!theIsSideEffectingFunction){
          theIsSideEffectingFunction = new IsSideEffectingFunction(this);
        return theIsSideEffectingFunction;
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
      else if(localName == "evaluate-updating-and-apply"){
        if(!theEvaluateAndApplyFunction){
          theEvaluateAndApplyFunction = new EvaluateAndApplyFunction(this);
        return theEvaluateAndApplyFunction;
        }
      }
      else if(localName == "evaluate-side-effecting"){
        if(!theEvaluateSideEffectingFunction){
          theEvaluateSideEffectingFunction = new EvaluateSideEffectingFunction(this);
        return theEvaluateSideEffectingFunction;
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
 
}/*namespace xqxq*/ }/*namespace zorba*/

#ifdef WIN32
#  define DLL_EXPORT __declspec(dllexport)
#else
#  define DLL_EXPORT __attribute__ ((visibility("default")))
#endif

extern "C" DLL_EXPORT zorba::ExternalModule* createModule() {
  return new zorba::xqxq::XQXQModule();
}