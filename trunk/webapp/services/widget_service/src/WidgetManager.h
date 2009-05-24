#ifndef _WIDGETMANAGER_H_
#define _WIDGETMANAGER_H_

#include <list>
#include <map>
#include <string>
#include <staff/common/DataObject.h>

namespace widget
{
  //! common string:string map
  typedef std::map<std::string, std::string> TStringMap;

  //! string list
  typedef std::list<std::string> TStringList;

  //! widget description
  struct SWidget
  {
    std::string sId;              //!< widget instance id
    std::string sClass;           //!< widget class
    staff::CDataObject tdoProps;  //!< properties tree
  };

  //! widget map pair(widget id, widget instance)
  typedef std::map<std::string, SWidget> TWidgetMap;


  //! widget group
  struct SWidgetGroup
  {
    std::string sId;        //!<  group id
    std::string sDescr;     //!<  description
    TWidgetMap  mWidgets;   //!<  widgets in group
  };

  //! widget map pair(group id, widget group instance)
  typedef std::map<std::string, SWidgetGroup> TWidgetGroupMap;


  //!  Widget Manager
  class CWidgetManager
  {
  public:
    //!        destructor
    virtual ~CWidgetManager() {};

    //!         open widget db
    /*! \param  sProfile - db name
        */
    virtual void Open(const std::string& sProfile) = 0;

    //!         close db, commiting changes
    virtual void Close() = 0;

    //!         commit changes
    virtual void Commit() = 0;


    //!         get available widgets description
    /*! \return widget class map
    */
    virtual TStringMap GetWidgetClasses() = 0;


    //!         get active widget list
    /*! \return active widget list
    */
    virtual TWidgetMap GetActiveWidgets() const = 0;

    //!         add widget to active list
    /*! \param  rWidget - widget description
        */
    virtual void AddWidget(const SWidget& rWidget) = 0;

    //!         delete widget instance
    /*! \param  sId - id of widget instance
        */
    virtual void DeleteWidget(const std::string& sId) = 0;

    //!         delete widgets instances
    /*! \param  lsIds - ids list of widget instances
    */
    virtual void DeleteWidgets(const TStringList& lsIds) = 0;

    //!         alter widget
    /*! \param  rWidget - widget description
        */
    virtual void AlterWidget(const SWidget& rWidget) = 0;


    //!         get available widget groups
    /*! \return widget groups map(group id/description)
    */
    virtual TStringMap GetAvailableWidgetGroups() const = 0;

    //!         get widget groups map
    /*! \param  rlsWidgetGroups - list of widget group ids to retrive
        \return widget groups map(group id/full group info)
    */
    virtual TWidgetGroupMap GetWidgetGroups(const TStringList& lsWidgetGroups) const = 0;


    //!         add new widget group
    /*! \param  rWidgetGroup - widget group info
        */
    virtual void AddWidgetGroup(const SWidgetGroup& rWidgetGroup) = 0;

    //!         remove widget group
    /*! \param  sGroupId - group id
    */
    virtual void DeleteWidgetGroup(const std::string& sGroupId) = 0;
    
    //!         alter widget group
    /*! \param  rWidgetGroup - widget group info
        */
    virtual void AlterWidgetGroup(const SWidgetGroup& rWidgetGroup) = 0;


    //!         get active(auto-loadable) widget groups map
    /*! \return widget groups map(group id/description)
    */
    virtual TStringList GetActiveWidgetGroups() const = 0;

    //!         set active(auto-loadable) groups list
    /*! \param  lsActiveWidgetGroups - active groups list
    */
    virtual void SetActiveWidgetGroups(const TStringList& lsActiveWidgetGroups) = 0;

  };
}

#endif // _WIDGETMANAGER_H_