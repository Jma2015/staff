// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Service Implementation

#include "TasksImpl.h"

namespace samples
{
namespace optional
{

TasksImpl::TasksImpl()
{
}

TasksImpl::~TasksImpl()
{
}

void TasksImpl::OnCreate()
{
  // this function is called when service instance is created and registered
}

void TasksImpl::OnDestroy()
{
  // this function is called immediately before service instance destruction
}

int TasksImpl::Add(const Task& rstTask)
{
  Task& rstAddedTask = *m_lsTasks.insert(m_lsTasks.end(), rstTask);
  rstAddedTask.nId = m_lsTasks.size();

  return *rstAddedTask.nId;
}

void TasksImpl::UpdateOwner(int nTaskId, const staff::Optional< int >& rtnOwnerId)
{
  for (TasksList::iterator itTask = m_lsTasks.begin();
       itTask != m_lsTasks.end(); ++itTask)
  {
    if (itTask->nId == nTaskId)
    {
      itTask->tnOwnerId = rtnOwnerId;
      break;
    }
  }
}

void TasksImpl::UpdateAttachInfo(int nTaskId, const staff::Optional< AttachInfo >& rtnAttachInfo)
{
  for (TasksList::iterator itTask = m_lsTasks.begin();
       itTask != m_lsTasks.end(); ++itTask)
  {
    if (itTask->nId == nTaskId)
    {
      itTask->tstAttachInfo = rtnAttachInfo;
      break;
    }
  }
}

staff::Optional< AttachInfo > samples::optional::TasksImpl::GetAttachInfo(int nTaskId)
{
  for (TasksList::iterator itTask = m_lsTasks.begin();
       itTask != m_lsTasks.end(); ++itTask)
  {
    if (itTask->nId == nTaskId)
    {
      return itTask->tstAttachInfo;
    }
  }

  return staff::Optional< AttachInfo >();
}


::samples::optional::TasksList TasksImpl::GetAllTasks() const
{
  return m_lsTasks;
}

staff::Optional< std::list<std::string> > TasksImpl::EchoOpt(
  const staff::Optional< std::list<std::string> >& opt)
{
  return opt;
}

}
}

