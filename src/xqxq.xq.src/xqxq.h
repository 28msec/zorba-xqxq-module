#ifndef __COM_ZORBA_WWW_MODULES_XQXQ_H__
#define __COM_ZORBA_WWW_MODULES_XQXQ_H__

#include <map>

#include <zorba/zorba.h>
#include <zorba/external_module.h>
#include <zorba/function.h>

#define XQXQ_MODULE_NAMESPACE "http://www.zorba-xquery.com/modules/xqxq"
#define XQXQ_QUERY_KEY_NAMESPACE "http://www.zorba-xquery.com/modules/xqxq/querykey"
#define XQXQ_DYNAMIC_CONTEXT_NAMESPACE "http://www.zorba-xquery.com/modules/xqxq/dynamiccontext"

namespace zorba { namespace xqxq {
  
  

  class XQXQModule : public ExternalModule {
		private:

      static ItemFactory* theFactory;

      ExternalFunction* thePrepareMainModuleFunction;
      ExternalFunction* thePrepareLibraryModuleFunction;
      ExternalFunction* theIsContextItemUnboundFunction;
      ExternalFunction* theGetUnboundVariablesFunction;
      ExternalFunction* theIsUpdatingFunction;
      ExternalFunction* theIsSideEffectingFunction;
      ExternalFunction* theBindContextItemFunction;
      ExternalFunction* theBindContextPositionFunction;
      ExternalFunction* theBindContextSizeFunction;
      ExternalFunction* theBindVariableFunction;
      ExternalFunction* theEvaluateFunction;
      ExternalFunction* theEvaluateUpdatingFunction;
      ExternalFunction* theEvaluateAndApplyFunction;
      ExternalFunction* theEvaluateSideEffectingFunction;
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

      typedef std::map<std::string, XQuery_t> QueryMap_t;
      QueryMap_t* queryMap;
  };

  class XQXQFunction : public ContextualExternalFunction
  {
    protected:
      const XQXQModule* theModule;

      

      static void
        throwError(const char*, const std::string);

       String
         getOneStringArgument(const Arguments_t&, int) const;

       Item
         getItemArgument(const Arguments_t&, int) const;

    public:

      XQXQFunction(const XQXQModule* module);

      virtual ~XQXQFunction();

      virtual String
        getURI() const;
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
       static Item
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
       static Item
         getKey(std::string);

  };

  class IsContextItemUnboundFunction : public XQXQFunction{
    public:
      IsContextItemUnboundFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~IsContextItemUnboundFunction(){}

      virtual zorba::String
        getLocalName() const { return "context-item-unbound"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class GetUnboundVariablesFunction : public XQXQFunction{
    public:
      GetUnboundVariablesFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~GetUnboundVariablesFunction() {}

      virtual zorba::String
        getLocalName() const {return "unbound-variables"; }

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

  class IsSideEffectingFunction : public XQXQFunction{
    public:
      IsSideEffectingFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~IsSideEffectingFunction() {}

      virtual zorba::String
        getLocalName() const {return "is-side-effecting"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class CreateDynamicContextFunction : public XQXQFunction{
    public:
      CreateDynamicContextFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~CreateDynamicContextFunction() {}

      virtual zorba::String
        getLocalName() const {return "create-dynamic-context"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class DeleteDynamicContextFunction : public XQXQFunction{
    public:
      DeleteDynamicContextFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~DeleteDynamicContextFunction() {}

      virtual zorba::String
        getLocalName() const {return "delete-dynamic-context"; }

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

  class EvaluateAndApplyFunction : public XQXQFunction{
    public:
      EvaluateAndApplyFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~EvaluateAndApplyFunction() {}

      virtual zorba::String
        getLocalName() const {return "evaluate-updating-and-apply"; }

      virtual zorba::ItemSequence_t
        evaluate(const Arguments_t&,
                 const zorba::StaticContext*,
                 const zorba::DynamicContext*) const;
  };

  class EvaluateSideEffectingFunction : public XQXQFunction{
    public:
      EvaluateSideEffectingFunction(const XQXQModule* aModule) : XQXQFunction(aModule) {}

      virtual ~EvaluateSideEffectingFunction() {}

      virtual zorba::String
        getLocalName() const {return "evaluate-side-effecting"; }

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