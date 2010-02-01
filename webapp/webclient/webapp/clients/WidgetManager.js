// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT
namespace('widget');

///////////////////////////////////////////////////////////////////////////////////////////////////////
// struct serializators
function SerializeStruct_widget_SWidget(tOperation, rstStruct, tNode)
{
  tOperation.AddParameter('sId', rstStruct.sId, tNode);
  tOperation.AddParameter('sClass', rstStruct.sClass, tNode);
  rstStruct.tdoProps.ToElement(tOperation.AddParameter('tdoProps', null, tNode));
}

function SerializeStruct_widget_SWidgetGroup(tOperation, rstStruct, tNode)
{
  tOperation.AddParameter('sId', rstStruct.sId, tNode);
  tOperation.AddParameter('sDescr', rstStruct.sDescr, tNode);
  SerializeTypedef_widget_TWidgetMap(tOperation, rstStruct.mWidgets, tOperation.AddParameter('mWidgets', '', tNode));
}

function SerializeStruct_widget_SBaseProfile(tOperation, rstStruct, tNode)
{
  tOperation.AddParameter('sId', rstStruct.sId, tNode);
  tOperation.AddParameter('sName', rstStruct.sName, tNode);
}

function SerializeStruct_widget_SProfile(tOperation, rstStruct, tNode)
{
  tOperation.AddParameter('sId', rstStruct.sId, tNode);
  tOperation.AddParameter('sName', rstStruct.sName, tNode);
  tOperation.AddParameter('sBase', rstStruct.sBase, tNode);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// struct deserializators
function DeserializeStruct_widget_SWidget(tOperation, tNode)
{
  var tResult = {};

  tResult.sId = tOperation.SubNodeText("sId", tNode);
  tResult.sClass = tOperation.SubNodeText("sClass", tNode);
  tResult.tdoProps = new staff.DataObject();
  tResult.tdoProps.FromElement(tOperation.SubNode("tdoProps", tNode));
  return tResult;
}

function DeserializeStruct_widget_SWidgetGroup(tOperation, tNode)
{
  var tResult = {};

  tResult.sId = tOperation.SubNodeText("sId", tNode);
  tResult.sDescr = tOperation.SubNodeText("sDescr", tNode);
  tResult.mWidgets = DeserializeTypedef_widget_TWidgetMap(tOperation, tOperation.SubNode("mWidgets", tNode));
  return tResult;
}

function DeserializeStruct_widget_SBaseProfile(tOperation, tNode)
{
  var tResult = {};

  tResult.sId = tOperation.SubNodeText("sId", tNode);
  tResult.sName = tOperation.SubNodeText("sName", tNode);
  return tResult;
}

function DeserializeStruct_widget_SProfile(tOperation, tNode)
{
  var tResult = {};

  tResult.sId = tOperation.SubNodeText("sId", tNode);
  tResult.sName = tOperation.SubNodeText("sName", tNode);
  tResult.sBase = tOperation.SubNodeText("sBase", tNode);
  return tResult;
}

//-----------------------------------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////////////////////////////
// typedef serializators

// ::widget::TStringMap  Typedef.DataType.Type template std::map
function SerializeTypedef_widget_TStringMap(tOperation, rtType, tNode)
{
  for(var tKey in rtType)
  {
    var tValue = rtType[tKey];
    if(typeof tValue != 'function')
    {
      var tItem = tOperation.AddParameter('Item', '', tNode);
      tOperation.AddParameter('Key', tKey, tItem);
      tOperation.AddParameter('Value', tValue, tItem);
    }
  }
  return tNode;
}

// ::widget::TStringList  Typedef.DataType.Type template std::list
function SerializeTypedef_widget_TStringList(tOperation, rtType, tNode)
{
  for(var i = 0; i != rtType.length; ++i)
  {
// Typedef.DataType.TemplateParams.TemplateParam1.Type = string
  tOperation.AddParameter('Item', rtType[i], tNode);
  }
  return tNode;
}

// ::widget::TWidgetMap  Typedef.DataType.Type template std::map
function SerializeTypedef_widget_TWidgetMap(tOperation, rtType, tNode)
{
  for(var tKey in rtType)
  {
    var tValue = rtType[tKey];
    if(typeof tValue != 'function')
    {
      var tItem = tOperation.AddParameter('Item', '', tNode);
      tOperation.AddParameter('Key', tKey, tItem);
      SerializeStruct_widget_SWidget(tOperation, tValue, tOperation.AddParameter('Value', '', tItem));
    }
  }
  return tNode;
}

// ::widget::TWidgetList  Typedef.DataType.Type template std::list
function SerializeTypedef_widget_TWidgetList(tOperation, rtType, tNode)
{
  for(var i = 0; i != rtType.length; ++i)
  {
// Typedef.DataType.TemplateParams.TemplateParam1.Type = struct
    SerializeStruct_widget_SWidget(tOperation, rtType[i], tOperation.AddParameter('Item', '', tNode));
  }
  return tNode;
}

// ::widget::TWidgetGroupMap  Typedef.DataType.Type template std::map
function SerializeTypedef_widget_TWidgetGroupMap(tOperation, rtType, tNode)
{
  for(var tKey in rtType)
  {
    var tValue = rtType[tKey];
    if(typeof tValue != 'function')
    {
      var tItem = tOperation.AddParameter('Item', '', tNode);
      tOperation.AddParameter('Key', tKey, tItem);
      SerializeStruct_widget_SWidgetGroup(tOperation, tValue, tOperation.AddParameter('Value', '', tItem));
    }
  }
  return tNode;
}

// ::widget::TBaseProfileList  Typedef.DataType.Type template std::list
function SerializeTypedef_widget_TBaseProfileList(tOperation, rtType, tNode)
{
  for(var i = 0; i != rtType.length; ++i)
  {
// Typedef.DataType.TemplateParams.TemplateParam1.Type = struct
    SerializeStruct_widget_SBaseProfile(tOperation, rtType[i], tOperation.AddParameter('Item', '', tNode));
  }
  return tNode;
}

// ::widget::TProfileList  Typedef.DataType.Type template std::list
function SerializeTypedef_widget_TProfileList(tOperation, rtType, tNode)
{
  for(var i = 0; i != rtType.length; ++i)
  {
// Typedef.DataType.TemplateParams.TemplateParam1.Type = struct
    SerializeStruct_widget_SProfile(tOperation, rtType[i], tOperation.AddParameter('Item', '', tNode));
  }
  return tNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// typedef deserializators
function DeserializeTypedef_widget_TStringMap(tOperation, tNode)
{
// container :: std::map< std::string, std::string >
  var tItem = null;

  var tResult = tNode == null ? tOperation.ResultElement() : tNode;
  var aResult = {};

  for (var i = 0; i < tResult.childNodes.length; i++)
  {
    if( tResult.childNodes[i].nodeName == "Item")
    {
//template std::map<std::string, std::string>
      var pKeyElem = tOperation.SubNode("Key", tResult.childNodes[i]);
      var pValueElem = tOperation.SubNode("Value", tResult.childNodes[i]);

      var tKey = pKeyElem.firstChild != null ? pKeyElem.firstChild.nodeValue : ""; // *** generic std::string
      var tValue = pValueElem.firstChild != null ? pValueElem.firstChild.nodeValue : ""; // *** generic std::string
      aResult[tKey] = tValue;
    }
  }

  return aResult;
}

function DeserializeTypedef_widget_TStringList(tOperation, tNode)
{
// container :: std::list< std::string >
  var tItem = null;

  var tResult = tNode == null ? tOperation.ResultElement() : tNode;
  var aResult = new Array();
  var j = 0;

  for (var i = 0; i < tResult.childNodes.length; i++)
  {
    if( tResult.childNodes[i].nodeName == "Item")
    {
//template std::list<std::string>
    aResult[j++] = tResult.childNodes[i].firstChild != null ? tResult.childNodes[i].firstChild.nodeValue : ""; // *** generic std::string
    }
  }

  return aResult;
}

function DeserializeTypedef_widget_TWidgetMap(tOperation, tNode)
{
// container :: std::map< std::string, ::widget::SWidget >
  var tItem = null;

  var tResult = tNode == null ? tOperation.ResultElement() : tNode;
  var aResult = {};

  for (var i = 0; i < tResult.childNodes.length; i++)
  {
    if( tResult.childNodes[i].nodeName == "Item")
    {
//template std::map<std::string, ::widget::SWidget>
      var pKeyElem = tOperation.SubNode("Key", tResult.childNodes[i]);
      var pValueElem = tOperation.SubNode("Value", tResult.childNodes[i]);

      var tKey = pKeyElem.firstChild != null ? pKeyElem.firstChild.nodeValue : ""; // *** generic std::string
      var tValue = DeserializeStruct_widget_SWidget(tOperation, pValueElem); // *** struct ::widget::SWidget
      aResult[tKey] = tValue;
    }
  }

  return aResult;
}

function DeserializeTypedef_widget_TWidgetList(tOperation, tNode)
{
// container :: std::list< ::widget::SWidget >
  var tItem = null;

  var tResult = tNode == null ? tOperation.ResultElement() : tNode;
  var aResult = new Array();
  var j = 0;

  for (var i = 0; i < tResult.childNodes.length; i++)
  {
    if( tResult.childNodes[i].nodeName == "Item")
    {
//template std::list<::widget::SWidget>
    aResult[j++] = DeserializeStruct_widget_SWidget(tOperation, tResult.childNodes[i]); // *** struct ::widget::SWidget
    }
  }

  return aResult;
}

function DeserializeTypedef_widget_TWidgetGroupMap(tOperation, tNode)
{
// container :: std::map< std::string, ::widget::SWidgetGroup >
  var tItem = null;

  var tResult = tNode == null ? tOperation.ResultElement() : tNode;
  var aResult = {};

  for (var i = 0; i < tResult.childNodes.length; i++)
  {
    if( tResult.childNodes[i].nodeName == "Item")
    {
//template std::map<std::string, ::widget::SWidgetGroup>
      var pKeyElem = tOperation.SubNode("Key", tResult.childNodes[i]);
      var pValueElem = tOperation.SubNode("Value", tResult.childNodes[i]);

      var tKey = pKeyElem.firstChild != null ? pKeyElem.firstChild.nodeValue : ""; // *** generic std::string
      var tValue = DeserializeStruct_widget_SWidgetGroup(tOperation, pValueElem); // *** struct ::widget::SWidgetGroup
      aResult[tKey] = tValue;
    }
  }

  return aResult;
}

function DeserializeTypedef_widget_TBaseProfileList(tOperation, tNode)
{
// container :: std::list< ::widget::SBaseProfile >
  var tItem = null;

  var tResult = tNode == null ? tOperation.ResultElement() : tNode;
  var aResult = new Array();
  var j = 0;

  for (var i = 0; i < tResult.childNodes.length; i++)
  {
    if( tResult.childNodes[i].nodeName == "Item")
    {
//template std::list<::widget::SBaseProfile>
    aResult[j++] = DeserializeStruct_widget_SBaseProfile(tOperation, tResult.childNodes[i]); // *** struct ::widget::SBaseProfile
    }
  }

  return aResult;
}

function DeserializeTypedef_widget_TProfileList(tOperation, tNode)
{
// container :: std::list< ::widget::SProfile >
  var tItem = null;

  var tResult = tNode == null ? tOperation.ResultElement() : tNode;
  var aResult = new Array();
  var j = 0;

  for (var i = 0; i < tResult.childNodes.length; i++)
  {
    if( tResult.childNodes[i].nodeName == "Item")
    {
//template std::list<::widget::SProfile>
    aResult[j++] = DeserializeStruct_widget_SProfile(tOperation, tResult.childNodes[i]); // *** struct ::widget::SProfile
    }
  }

  return aResult;
}


//-----------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////////
// class: widget.WidgetManager

widget.WidgetManager = Class.create();
widget.WidgetManager.prototype = 
{
  initialize: function(sServiceUri, sSessionId, sTargetNamespace)
  {
    if (!sServiceUri)
    {
      sServiceUri = webapp.Env.protocol + Session.sHost + (Session.sPort ? (':' + Session.sPort) : '') + '/axis2/services/widget.WidgetManager';
    }
    
    if (!sTargetNamespace)
    {
      sTargetNamespace = sServiceUri;
    }
    
    this.sTargetNamespace = sTargetNamespace || sServiceUri;

    this.tClient = new staff.Client(sServiceUri, sSessionId || Session.sID || "");
  },
  
  SetDataObjectAsXml: function(bDataObjectAsXml)
  {
    this.bDataObjectAsXml = bDataObjectAsXml;
  },
  
  SetID: function(sSessionId)
  {
    this.tClient.SetSessionId(sSessionId);
  },

  GetBaseProfiles: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetBaseProfiles', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(DeserializeTypedef_widget_TBaseProfileList(tOperation), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return DeserializeTypedef_widget_TBaseProfileList(tOperation);
    }
  },

  GetProfiles: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetProfiles', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(DeserializeTypedef_widget_TProfileList(tOperation), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return DeserializeTypedef_widget_TProfileList(tOperation);
    }
  },

  AddProfile: function(stProfile, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('AddProfile', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeStruct_widget_SProfile(tOperation, stProfile, tOperation.AddParameter('stProfile'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  DeleteProfile: function(sProfile, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('DeleteProfile', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    tOperation.AddParameter('sProfile', sProfile);
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  SetProfile: function(stProfile, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('SetProfile', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeStruct_widget_SProfile(tOperation, stProfile, tOperation.AddParameter('stProfile'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  Open: function(sProfile, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('Open', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    tOperation.AddParameter('sProfile', sProfile);
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  Close: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('Close', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  Commit: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('Commit', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  GetWidgetClasses: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetWidgetClasses', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(DeserializeTypedef_widget_TStringMap(tOperation), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return DeserializeTypedef_widget_TStringMap(tOperation);
    }
  },

  GetActiveWidgets: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetActiveWidgets', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(DeserializeTypedef_widget_TWidgetMap(tOperation), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return DeserializeTypedef_widget_TWidgetMap(tOperation);
    }
  },

  AddWidget: function(rWidget, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('AddWidget', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeStruct_widget_SWidget(tOperation, rWidget, tOperation.AddParameter('rWidget'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  AddWidgets: function(rWidgets, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('AddWidgets', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeTypedef_widget_TWidgetMap(tOperation, rWidgets, tOperation.AddParameter('rWidgets'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  DeleteWidget: function(sId, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('DeleteWidget', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    tOperation.AddParameter('sId', sId);
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  DeleteWidgets: function(lsIds, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('DeleteWidgets', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeTypedef_widget_TStringList(tOperation, lsIds, tOperation.AddParameter('lsIds'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  AlterWidget: function(rWidget, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('AlterWidget', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeStruct_widget_SWidget(tOperation, rWidget, tOperation.AddParameter('rWidget'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  AlterWidgetsListAndCommit: function(rlsWidgets, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('AlterWidgetsListAndCommit', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeTypedef_widget_TWidgetList(tOperation, rlsWidgets, tOperation.AddParameter('rlsWidgets'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  GetAvailableWidgetGroups: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetAvailableWidgetGroups', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(DeserializeTypedef_widget_TStringMap(tOperation), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return DeserializeTypedef_widget_TStringMap(tOperation);
    }
  },

  GetWidgetGroups: function(lsWidgetGroups, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetWidgetGroups', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeTypedef_widget_TStringList(tOperation, lsWidgetGroups, tOperation.AddParameter('lsWidgetGroups'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(DeserializeTypedef_widget_TWidgetGroupMap(tOperation), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return DeserializeTypedef_widget_TWidgetGroupMap(tOperation);
    }
  },

  AddWidgetGroup: function(rWidgetGroup, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('AddWidgetGroup', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeStruct_widget_SWidgetGroup(tOperation, rWidgetGroup, tOperation.AddParameter('rWidgetGroup'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  DeleteWidgetGroup: function(sGroupId, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('DeleteWidgetGroup', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    tOperation.AddParameter('sGroupId', sGroupId);
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  AlterWidgetGroup: function(rWidgetGroup, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('AlterWidgetGroup', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeStruct_widget_SWidgetGroup(tOperation, rWidgetGroup, tOperation.AddParameter('rWidgetGroup'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  GetActiveWidgetGroups: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetActiveWidgetGroups', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(DeserializeTypedef_widget_TStringList(tOperation), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return DeserializeTypedef_widget_TStringList(tOperation);
    }
  },

  SetActiveWidgetGroups: function(lsActiveWidgetGroups, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('SetActiveWidgetGroups', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeTypedef_widget_TStringList(tOperation, lsActiveWidgetGroups, tOperation.AddParameter('lsActiveWidgetGroups'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  }
}

