#ifndef __COM_ZORBA_WWW_MODULES_XQXQ_H__
#define __COM_ZORBA_WWW_MODULES_XQXQ_H__

#include <map>

#include <zorba/zorba.h>
#include <zorba/external_module.h>
#include <zorba/function.h>
#include <zorba/dynamic_context.h>

#define XQXQ_MODULE_NAMESPACE "http://www.zorba-xquery.com/modules/xqxq"

namespace zorba { namespace xqxq {
  
  

  class XQXQModule : public ExternalModule {
		private:

      static ItemFactory* theFactory;

      ExternalFunction* thePrepareMainModuleFunction;
      ExternalFunction* thePrepareLibraryModuleFunction;
      ExternalFunction* theGetExternalVariablesFunction;
      ExternalFunction* theIsBoundContextItemFunction;
      ExternalFunction* theIsBoundVariableFunction;
      ExternalFunction* theIsUpdatingFunction;
      ExternalFunction* theIsSequentialFunction;
      ExternalFunction* theBindContextItemFunction;
      ExternalFunction* theBindContextPositionFunction;
      ExternalFunction* theBindContextSizeFunction;
      ExternalFunction* theBindVariableFunction;
      ExternalFunction* theEvaluateFunction;
      ExternalFunction* theEvaluateUpdatingFunction;
      ExternalFunction* theEvaluateSequentialFunction;
      ExternalFunction* theDeleteQueryFunction;

    public:

      XQXQModule();

      virtual ~XQXQModule();

      virtual zorba::String 
        getURI() const {return XQXQ_MODULE_NAMESPACE;}

      virtual zorba::ExternalFunction* 
        getExternalFunction(const String& localName);

      virtual void destroy();

      static ItemFactory*
        getItemFactory()
      {
        if(!theFactory)
        {
          theFactory = Zorba::getInstance(0)->getItemFactory();
        }

        return theFactory;
      }
  };


  class XQXQFunction : public ContextualExternalFunction
  {
    protected:
      const XQXQModule* theModule;

       String
         getOneStringArgument(const Arguments_t&, int) const;

       Item
         getItemArgument(const Arguments_t&, int) const;

      static void
        throwError(const char*, const std::string);

    public:

      XQXQFunction(const XQXQModule* module);

      virtual ~XQXQFunction();

      virtual String
        getURI() const;
  };

  class QueryMap{
    private:
      typedef std::map<std::string, XQuery_t> QueryMap_t;
      QueryMap_t* queryMap;

    public:
      QueryMap();
      bool 
        storeQuery(String, XQuery_t);
      XQuery_t
        getQuery(String);
      bool 
        deleteQuery(String);
  };

  class PrepareMainModuleFunction : public XQXQFunction{
    public:
      PrepareMainModuleFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~PrepareMainModuleFunction(){}
      
      virtual zorba::String
        getLocalName() const { return "prepare-main-module"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;

    protected:
       static String
         getKey(std::string);

  };

  class PrepareLibraryModuleFunction : public XQXQFunction{
    public:
      PrepareLibraryModuleFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~PrepareLibraryModuleFunction(){}
      
      virtual zorba::String
        getLocalName() const { return "prepare-library-module"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;

    protected:
       static String
         getKey(std::string);

  };

  class IsBoundContextItemFunction : public XQXQFunction{
    public:
      IsBoundContextItemFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~IsBoundContextItemFunction(){}

      virtual zorba::String
        getLocalName() const { return "is-bound-context-item"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class IsBoundVariableFunction : public XQXQFunction{
    public:
      IsBoundVariableFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~IsBoundVariableFunction(){}

      virtual zorba::String
        getLocalName() const { return "is-bound-variable"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class GetExternalVariablesFunction : public XQXQFunction{
    public:
      GetExternalVariablesFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~GetExternalVariablesFunction() {}

      virtual zorba::String
        getLocalName() const {return "get-external-variables"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class IsUpdatingFunction : public XQXQFunction{
    public:
      IsUpdatingFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~IsUpdatingFunction() {}

      virtual zorba::String
        getLocalName() const {return "is-updating"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;

  };

  class IsSequentialFunction : public XQXQFunction{
    public:
      IsSequentialFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~IsSequentialFunction() {}

      virtual zorba::String
        getLocalName() const {return "is-sequential"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class BindContextItemFunction : public XQXQFunction{
    public:
      BindContextItemFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~BindContextItemFunction() {}

      virtual zorba::String
        getLocalName() const {return "bind-context-item"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class BindContextPositionFunction : public XQXQFunction{
    public:
      BindContextPositionFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~BindContextPositionFunction() {}

      virtual zorba::String
        getLocalName() const {return "bind-context-position"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class BindContextSizeFunction : public XQXQFunction{
    public:
      BindContextSizeFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~BindContextSizeFunction() {}

      virtual zorba::String
        getLocalName() const {return "bind-context-size"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class BindVariableFunction : public XQXQFunction{
    public:
      BindVariableFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~BindVariableFunction() {}

      virtual zorba::String
        getLocalName() const {return "bind-variable"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class EvaluateFunction : public XQXQFunction{
    public:
      EvaluateFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~EvaluateFunction() {}

      virtual zorba::String
        getLocalName() const {return "evaluate"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class EvaluateUpdatingFunction : public XQXQFunction{
    public:
      EvaluateUpdatingFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~EvaluateUpdatingFunction() {}

      virtual zorba::String
        getLocalName() const {return "evaluate-updating"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class EvaluateSequentialFunction : public XQXQFunction{
    public:
      EvaluateSequentialFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~EvaluateSequentialFunction() {}

      virtual zorba::String
        getLocalName() const {return "evaluate-sequential"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;

      virtual String getURI() const {
        return theModule->getURI();
      }

    protected:
      const XQXQModule* theModule;
  };

  class DeleteQueryFunction : public XQXQFunction{
    public:
      DeleteQueryFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~DeleteQueryFunction() {}

      virtual zorba::String
        getLocalName() const {return "delete-query"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };



}/*xqxq namespace*/}/*zorba namespace*/


#endif //_COM_ZORBA_WWW_MODULES_XQXQ_H_