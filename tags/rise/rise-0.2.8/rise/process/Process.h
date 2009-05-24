#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <string>
#include <rise/common/types.h>
#include <rise/os/osprocess.h>
#include <rise/os/osdllexport.h>
#include <rise/threading/SharedPtr.h>

namespace rise
{
  namespace process
  {
    //!        класс запуска приложений
    class RISE_EXPORT CProcess
    {
    public:
      enum ESTATUS     //! код возврата
      {
        EESERROREXEC = g_nResultError,  //!<  ошибка при запуске приложения
        EESRUNNING,                     //!<  процесс выполняется
        EESNOTRUNNING,                  //!<  процесс не выполняется
        EESUNKNOWN                      //!<  неизвестное состояние
      };

    public:
      //!        конструктор
      CProcess();

      //!        деструктор
      virtual ~CProcess();

      //!        присоединиться к уже запущенному процессу
      /*
         \param  hProcess - идентификатор процесса
         \return none
      */
      bool Attach(HProcess hProcess);

      //!        запуск приложения
      /*
         аргументы разделяются символом табуляции '\t'
         \param  sCommand - приложение
         \return none
      */
      bool Exec(const CString& sCommand);

      //!        запущен ли процесс
      /*
         \return ESTATE
      */
      bool IsExists();

      //!        получить код возврата процесса
      /*
         \return код возврата
      */
      int GetExitStatus(bool bWait = false);

      //!        получить идентификатор процесса связанного с обьектом класса
      /*
         \return идентификатор процесса связанного с обьектом класса
      */
      HProcess GetProcessID();

      //!        получить идентификатор текущего процесса
      /*
         \return идентификатор процесса из которого была вызвана эта функция
      */
      static HProcess GetCurrentProcessID();

      //!         получить имя исполняемого файла с полным путем текущего процесса
      /*! \return имя исполняемого файла с полным путем текущего процесса
      */
      static rise::CString GetCurrentExecPath();

      //!        переключить процесс в фоновый режим
      /*
                 в WIN32 игнорируется
         \return 0, если успешно
      */
      static void Daemon();

      //!        приостановить выполнение процесса до прихода сообщения/сигнала
      static void Pause();

      //!        завершить процесс, связанный с обьектом класса
      /*
         исключения:     CLogicNoItemException - процесс не найден
                         CLogicPermissionException - нет прав
      */
      void Terminate();

      //!        завершить процесс
      /*
         исключения:     CLogicNoItemException - процесс не найден
                         CLogicPermissionException - нет прав
         \param  hProcess - идентификатор процесса
      */
      static void Terminate(HProcess hProcess);

    private:
      int              m_nExitStatus; //! статус
      HProcess         m_hProcess;    //! дескриптор процесса

    };
  } // namespace process
}  // namespace rise

#endif // _EXEC_H_