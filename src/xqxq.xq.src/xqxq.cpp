#include <zorba/item_factory.h>
#include <zorba/singleton_item_sequence.h>
#include <zorba/diagnostic_list.h>
#include <zorba/empty_sequence.h>
#include <zorba/store_manager.h>
#include <zorba/user_exception.h>
#include <zorba/vector_item_sequence.h>
#include <zorba/serializer.h>
#include <zorba/xquery.h>
#include <time.h>
#include <stdio.h>

#include <vector>

#include "xqxq.h"

namespace zorba { namespace xqxq {

  /*******************************************************************************************
  *******************************************************************************************/
  
  ItemFactory* XQXQModule::theFactory = 0;

  zorba::ExternalFunction*
    XQXQModule::getExternalFunction(const zorba::String& localName)
  {
    
    FuncMap_t::iterator lIte = theFunctions.find(localName);
    
    ExternalFunction*& lFunc = theFunctions[localName];

    if (lIte == theFunctions.end())
    {
      if (localName == "prepare-main-module")
      {
        lFunc = new PrepareMainModuleFunction(this);
      }
      if (localName == "prepare-library-module")
      {
        lFunc = new PrepareLibraryModuleFunction(this);
      }
      else if (localName == "is-bound-context-item")
      {
        lFunc = new IsBoundContextItemFunction(this);
      }
      else if (localName == "is-bound-variable")
      {
        lFunc = new IsBoundVariableFunction(this);
      }
      else if (localName == "external-variables")
      {
        lFunc = new GetExternalVariablesFunction(this);
      }
      else if (localName == "is-updating")
      {
        lFunc = new IsUpdatingFunction(this);        
      }
      else if (localName == "is-sequential")
      {
        lFunc = new IsSequentialFunction(this);
      }      
      else if (localName == "bind-context-item")
      {
        lFunc = new BindContextItemFunction(this);
      }
      else if (localName == "bind-variable")
      {
        lFunc = new BindVariableFunction(this);
      }
      else if (localName == "evaluate")
      {
        lFunc = new EvaluateFunction(this);
      }
      else if (localName == "evaluate-updating")
      {
        lFunc = new EvaluateUpdatingFunction(this);
      }
      else if (localName == "evaluate-sequential")
      {
        lFunc = new EvaluateSequentialFunction(this);
      }
      else if (localName == "delete-query")
      {
        lFunc = new DeleteQueryFunction(this);
      }
    }

    return lFunc;
  }

  void XQXQModule::destroy() 
  {
    delete this;
  }

  XQXQModule::~XQXQModule()
  {
    for (FuncMap_t::const_iterator lIter = theFunctions.begin();
       lIter != theFunctions.end(); ++lIter) 
    {
      delete lIter->second;
    }
    theFunctions.clear();
  }

  /*******************************************************************************************
  *******************************************************************************************/
  XQXQFunction::XQXQFunction(const XQXQModule* aModule)
    : theModule(aModule)
  {
    srand(time(NULL));
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

  Iterator_t
    XQXQFunction::getIterArgument(const Arguments_t& aArgs, int aPos) const
  {
    Iterator_t args_iter = aArgs[aPos]->getIterator();
    return args_iter;
  }

  XQuery_t
  XQXQFunction::getQuery(
      const zorba::DynamicContext* aDctx,
      const zorba::String& aIdent) const
  {
    QueryMap* lQueryMap;
    if (!(lQueryMap= dynamic_cast<QueryMap*>(aDctx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      throwError("NoQueryMatch", "No query with the given identifier was found");
    }

    XQuery_t lQuery;
    if (!(lQuery = lQueryMap->getQuery(aIdent)))
      throwError("NoQueryMatch", "No query with the given identifier was found");

    return lQuery;
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
    QueryMap::storeQuery(const String& aKeyName, XQuery_t aQuery)
  {
    std::pair<QueryMap_t::iterator,bool> ret;
    ret = queryMap->insert(std::pair<std::string, XQuery_t>(aKeyName.c_str(), aQuery));
    
    return ret.second;
  }

  XQuery_t
    QueryMap::getQuery(const String& aKeyName)
  {
    QueryMap::QueryMap_t::iterator lIter = queryMap->find(aKeyName.c_str());

    if(lIter == queryMap->end())
      return NULL;
    
    XQuery_t lQuery = lIter->second;

    return lQuery;
  }

  bool
    QueryMap::deleteQuery(const String& aKeyName)
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

    String lQueryString = getOneStringArgument(aArgs, 0); 
    
    StaticContext_t lStaticContext = lZorba->createStaticContext();

    XQuery_t lQuery;
    
    try
    {
      lQuery = lZorba->compileQuery(lQueryString, lStaticContext);
    }
    catch (ZorbaException& e)
    {
      const zorba::Diagnostic& d = e.diagnostic();
      std::ostringstream err;
      err << e;
      Item errQName = XQXQModule::getItemFactory()->createQName(
        d.qname().ns(), d.qname().localname()
      );
      throw USER_EXCEPTION(errQName, err.str());
    }

    String lUUID = getUUID();
    
    lQueryMap->storeQuery(lUUID, lQuery);

    return ItemSequence_t(new SingletonItemSequence(XQXQModule::getItemFactory()->createAnyURI(lUUID)));
  }

  String 
    PrepareMainModuleFunction::S4 ()
  {
    unsigned long randNum = (1 + rand() * 0x10000)|0;
    char* randBuff= new char[20];
    sprintf(randBuff, "%lx", randNum);
    String lString(randBuff);
    delete[] randBuff;
    return lString;
  }

  String 
    PrepareMainModuleFunction::getUUID()
  {
    return (String("urn:uuid:") + S4()+"-"+S4()+"-"+S4()+"-"+S4()+"-"+S4()+S4());
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
    String lQueryString = getOneStringArgument(aArgs, 0);     
    Zorba_CompilerHints_t hints;
    StaticContext_t lStaticContext =  lZorba->createStaticContext();
    hints.lib_module = true;
    try
    {
      lStaticContext->loadProlog(lQueryString, hints);
    }
    catch(ZorbaException& e)
    {
      const zorba::Diagnostic& d = e.diagnostic();
      std::ostringstream err;
      err << e;
      Item errQName = XQXQModule::getItemFactory()->createQName(
        d.qname().ns(), d.qname().localname()
      );
      throw USER_EXCEPTION(errQName, err.str());
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

    XQuery_t lQuery = getQuery(aDctx, lQueryID);

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
    return ItemSequence_t(new SingletonItemSequence(XQXQModule::getItemFactory()->createBoolean(lIsContextItemBound)));
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
    
    XQuery_t lQuery = getQuery(aDctx, lQueryID);

    Item lVarQName = XQXQFunction::getItemArgument(aArgs, 1);
    bool lIsBoundVariable = false;

    try
    {
      lIsBoundVariable = lQuery->isBoundExternalVariable(lVarQName.getNamespace(),lVarQName.getLocalName());
    }
    catch (ZorbaException& ze)
    {
      if (!strcmp("XPDY0002",ze.diagnostic().qname().localname()))
        lIsBoundVariable = false;
      else
        XQXQFunction::throwError(ze.diagnostic().qname().localname(), ze.diagnostic().message());  
    }
    return ItemSequence_t(new SingletonItemSequence(XQXQModule::getItemFactory()->createBoolean(lIsBoundVariable)));
  
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

    XQuery_t lQuery = getQuery(aDctx, lQueryID);
    
    std::vector<Item> lVars;
    Iterator_t lVarsIterator;
    lQuery->getExternalVariables(lVarsIterator);

    Item lVar;
    lVarsIterator->open();
    while(lVarsIterator->next(lVar))
      lVars.push_back(lVar);
    lVarsIterator->close();

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

    XQuery_t lQuery = getQuery(aDctx, lQueryID);

    return ItemSequence_t(new SingletonItemSequence(XQXQModule::getItemFactory()->createBoolean(lQuery->isUpdating())));
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

    XQuery_t lQuery = getQuery(aDctx, lQueryID);

    return ItemSequence_t(new SingletonItemSequence(XQXQModule::getItemFactory()->createBoolean(lQuery->isSequential())));
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

    XQuery_t lQuery = getQuery(aDctx, lQueryID);

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

    XQuery_t lQuery = getQuery(aDctx, lQueryID);
    
    Item lVarQName = XQXQFunction::getItemArgument(aArgs, 1); 

    Iterator_t lVarValue = XQXQFunction::getIterArgument(aArgs, 2); 

    lQuery->getDynamicContext()->setVariable(lVarQName.getNamespace(), lVarQName.getLocalName() , lVarValue);

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

    XQuery_t lQuery = getQuery(aDctx, lQueryID);

    if(lQuery->isUpdating())
    {
      throwError("QueryIsUpdating", "Executing Query shouldn't be updating.");
    }
    
   
    if(lQuery->isSequential())
    {
      throwError("QueryIsSequential", "Executing Query shouldn't be sequential.");
    }
      
    Iterator_t lIterQuery = lQuery->iterator();
    return ItemSequence_t(new EvaluateItemSequence(lIterQuery));
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

    XQuery_t lQuery = getQuery(aDctx, lQueryID);
    
    if(lQuery->isSequential())
    {
      throwError("QueryIsSequential", "Executing Query shouldn't be sequential.");   
    }

    if(!lQuery->isUpdating())
    {
      throwError("QueryNotUpdating", "Executing Query should be updating.") ; 
    }
    
    Iterator_t lIterQuery = lQuery->iterator();
    return ItemSequence_t(new EvaluateItemSequence(lIterQuery));
}

  /*******************************************************************************************
  *******************************************************************************************/
  zorba::ItemSequence_t
    EvaluateSequentialFunction::evaluate(
      const Arguments_t& aArgs,
      const zorba::StaticContext* aSctx,
      const zorba::DynamicContext* aDctx) const 
  {
    String lQueryID = XQXQFunction::getOneStringArgument(aArgs, 0);

    XQuery_t lQuery = getQuery(aDctx, lQueryID);

    if(lQuery->isUpdating())
    {
      throwError("QueryIsUpdating", "Executing Query shouldn't be updating.");
    }

    if(!lQuery->isSequential())
    {
      throwError("QueryNotSequential", "Executing Query should be sequential.");    
    }

    Iterator_t lIterQuery = lQuery->iterator();
    return ItemSequence_t(new EvaluateItemSequence(lIterQuery));
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
    if (!(lQueryMap= dynamic_cast<QueryMap*>(aDctx->getExternalFunctionParameter("xqxqQueryMap"))))
    {
      throwError("NoQueryMatch", "String identifying query does not exists.");
    }

    if (!lQueryMap->deleteQuery(lQueryID))
    {
      throwError("NoQueryMatch","String identifying query does not exists.");
    }
      
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
