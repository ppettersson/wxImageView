#ifndef DIR_NOTIFIER_H
#define DIR_NOTIFIER_H

// This is currently platform specific.
#if __WXMSW__
#include <windows.h>
#include <wx/thread.h>

class DirNotifierCallback
{
public:
  virtual void DirNotifyCallback()	{ }
};

class DirNotifier : public wxThread
{
public:
  DirNotifier(LPTSTR lpDir, DirNotifierCallback *callback);
  ~DirNotifier();

  // Thread execution starts here.
  virtual void *Entry();

  // Called when the thread exits - whether it terminates normally or is
  // stopped with Delete() (but not when it is Kill()ed!).
  virtual void OnExit();

private:
  LPTSTR	directory;
  DirNotifierCallback *callback;
};

#endif // __WXMSW__

#endif // DIR_NOTIFIER_H
