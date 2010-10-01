/*
  
  silcschedule.h
 
  Author: Pekka Riikonen <priikone@silcnet.org>
 
  Copyright (C) 1998 - 2005 Pekka Riikonen
 
  The contents of this file are subject to one of the Licenses specified 
  in the COPYING file;  You may not use this file except in compliance 
  with the License.
 
  The software distributed under the License is distributed on an "AS IS"
  basis, in the hope that it will be useful, but WITHOUT WARRANTY OF ANY
  KIND, either expressed or implied.  See the COPYING file for more
  information.

*/
 
/****h* silcutil/SILC Schedule Interface
 *
 * DESCRIPTION
 *
 * The SILC Scheduler is the heart of any application. The scheduler provides
 * the application's main loop that can handle incoming data, outgoing data,
 * timeouts and dispatch different kind of tasks.
 *
 * The SILC Scheduler supports file descriptor based tasks, timeout tasks
 * and generic tasks. File descriptor tasks are tasks that perform some 
 * operation over the specified file descriptor. These include network 
 * connections, for example. The timeout tasks are timeouts that are executed
 * after the specified timeout has elapsed. The generic tasks are tasks that
 * apply to all registered file descriptors thus providing one task that
 * applies to many independent connections.
 *
 * The SILC Scheduler is designed to be the sole main loop of the application
 * so that the application does not need any other main loop.  However,
 * SILC Scheduler does support running the scheduler only once, so that the
 * scheduler does not block, and thus providing a possiblity that some
 * external main loop is run over the SILC Scheduler. However, these 
 * applications are considered to be special cases.
 *
 * Typical application first initializes the scheduler and then registers
 * the very first tasks to the scheduler and then run the scheduler.  After
 * the scheduler's run function returns the application is considered to be 
 * ended.
 *
 * On WIN32 systems the SILC Scheduler is too designed to work as the main
 * loop of the GUI application. It can handle all Windows messages and
 * it dispatches them from the scheduler, and thus makes it possible to
 * create GUI applications. The scheduler can also handle all kinds of
 * WIN32 handles, this includes sockets created by the SILC Net API routines,
 * WSAEVENT handle objects created by Winsock2 routines and arbitrary 
 * WIN32 HANDLE objects.
 *
 * The SILC Scheduler supports multi-threads as well. The actual scheduler
 * must be run in single-thread but other threads may register new tasks
 * and unregister old tasks.  However, it is enforced that the actual
 * task is always run in the main thread.  The scheduler is context based
 * which makes it possible to allocate several schedulers for one application.
 * Since the scheduler must be run in single-thread, a multi-threaded
 * application could be created by allocating own scheduler for each of the
 * worker threads.
 *
 ***/

#ifndef SILCSCHEDULE_H
#define SILCSCHEDULE_H

/****s* silcutil/SilcScheduleAPI/SilcSchedule
 *
 * NAME
 * 
 *    typedef struct SilcScheduleStruct *SilcSchedule;
 *
 * DESCRIPTION
 *
 *    This context is the actual Scheduler and is allocated by
 *    the silc_schedule_init funtion.  The context is given as argument
 *    to all silc_schedule_* functions.  It must be freed by the 
 *    silc_schedule_uninit function.
 *
 ***/
typedef struct SilcScheduleStruct *SilcSchedule;

/****s* silcutil/SilcScheduleAPI/SilcTask
 *
 * NAME
 * 
 *    typedef struct SilcTaskStruct *SilcTask;
 *
 * DESCRIPTION
 *
 *    This object represents one task in the scheduler.  It is allocated
 *    by the silc_schedule_task_add function and freed by one of the
 *    silc_schedule_task_del* functions.
 *
 ***/
typedef struct SilcTaskStruct *SilcTask;

/****d* silcutil/SilcScheduleAPI/SilcTaskType
 *
 * NAME
 * 
 *    typedef enum { ... } SilcTaskType;
 *
 * DESCRIPTION
 *
 *    SILC has three types of tasks, non-timeout tasks (tasks that perform
 *    over file descriptors), timeout tasks and generic tasks (tasks that
 *    apply to every file descriptor). This type is sent as argument for the 
 *    task registering function, silc_schedule_task_add.
 *
 * SOURCE
 */
typedef enum {
  /* File descriptor task that performs some event over file descriptors.
     These tasks are for example network connections. */
  SILC_TASK_FD           = 0,

  /* Timeout tasks are tasks that are executed after the specified 
     time has elapsed. After the task is executed the task is removed
     automatically from the scheduler. It is safe to re-register the
     task in task callback. It is also safe to unregister a task in
     the task callback. */
  SILC_TASK_TIMEOUT,

  /* Generic tasks are non-timeout tasks and they apply to all file 
     descriptors, except to those that have explicitly registered a 
     non-timeout task. These tasks are there to make it simpler and faster 
     to execute common code that applies to all connections. These are,
     for example, receiving packets from network and sending packets to
     network. It doesn't make much sense to register a task that receives
     a packet from network to every connection when you can have one task
     that applies to all connections. This is what generic tasks are for.
     Generic tasks are not bound to any specific file descriptor, however,
     the correct file descriptor must be passed as argument to task
     registering function. */
  SILC_TASK_GENERIC,
} SilcTaskType;
/***/

/****d* silcutil/SilcScheduleAPI/SilcTaskEvent
 *
 * NAME
 * 
 *    typedef enum { ... } SilcTaskEvent;
 *
 * DESCRIPTION
 *
 *    SILC Task event types.  The event type indicates the occurred
 *    event of the task.  This type will be given as argument to the
 *    SilcTaskCallback function to indicate the event for the caller.
 *    The SILC_TASK_READ and SILC_TASK_WRITE may be set by the caller
 *    of the silc_schedule_set_listen_fd, if the caller needs to control
 *    the events for the task. The SILC_TASK_EXPIRE is set always only
 *    by the scheduler when timeout expires for timeout task.  The
 *    SILC_TASK_INTERRUPT is set for signal callback.
 *
 * SOURCE
 */
typedef enum {
  SILC_TASK_READ         = 0x0001,	         /* Reading */
  SILC_TASK_WRITE        = 0x0002,		 /* Writing */
  SILC_TASK_EXPIRE       = 0x0004,		 /* Timeout */
  SILC_TASK_INTERRUPT    = 0x0008,		 /* Signal */
} SilcTaskEvent;
/***/

/****d* silcutil/SilcScheduleAPI/SilcTaskPriority
 *
 * NAME
 * 
 *    typedef enum { ... } SilcTaskPriority;
 *
 * DESCRIPTION
 *
 *    Task priorities. Tasks may be registered with different priorities.
 *    This type defines the different task priorities. The priorities
 *    behaves same for all type of tasks, fd tasks, timeout tasks and
 *    generic tasks.
 *
 * SOURCE
 */
typedef enum {
  /* Lowest priority. The task is scheduled to run after its timeout
     has expired only and only when every other task with higher priority 
     has already been run. For non-timeout tasks this priority behaves
     same way. Life is not fair for tasks with this priority. */
  SILC_TASK_PRI_LOW      = 0,

  /* Normal priority that is used mostly in SILC. This is priority that
     should always be used unless you specificly need some other priority.
     The scheduler will run this task as soon as its timeout has expired.
     For non-timeout tasks this priority behaves same way. Tasks are run 
     in FIFO (First-In-First-Out) order. */
  SILC_TASK_PRI_NORMAL,
} SilcTaskPriority;
/***/

/****f* silcutil/SilcScheduleAPI/SilcTaskCallback
 *
 * SYNOPSIS
 *
 *    typedef void (*SilcTaskCallback)(SilcSchedule schedule,
 *                                     void *app_context,
 *                                     SilcTaskEvent type, SilcUInt32 fd,
 *                                     void *context);
 *
 * DESCRIPTION
 *
 *    The task callback function.  This function will be called by the
 *    scheduler when some event of the task is performed.  For example,
 *    when data is available from the connection this will be called.
 *
 *    The `schedule' is the scheduler context, the `type' is the indicated
 *    event, the `fd' is the file descriptor of the task and the `context'
 *    is a caller specified context. If multiple events occurred this
 *    callback is called separately for all events.  The `app_context'
 *    is application specific context that was given as argument to the
 *    silc_schedule_init function.
 *
 *    To specify task callback function in the application using the
 *    SILC_TASK_CALLBACK and SILC_TASK_CALLBACK_GLOBAL macros is
 *    recommended.
 *
 ***/
typedef void (*SilcTaskCallback)(SilcSchedule schedule, void *app_context,
				 SilcTaskEvent type, SilcUInt32 fd,
				 void *context);

/* Macros */

/****d* silcutil/SilcScheduleAPI/SILC_ALL_TASKS
 *
 * NAME
 * 
 *    #define SILC_ALL_TASKS ...
 *
 * DESCRIPTION
 *
 *    Marks for all tasks in the scheduler. This can be passed to 
 *    silc_schedule_task_del function to delete all tasks at once.
 *
 * SOURCE
 */
#define SILC_ALL_TASKS ((SilcTask)1)
/***/

/****d* silcutil/SilcScheduleAPI/SILC_TASK_CALLBACK
 *
 * NAME
 * 
 *    #define SILC_TASK_CALLBACK ...
 *
 * DESCRIPTION
 *
 *    Generic macro to define task callback functions. This defines a
 *    static function with name `func' as a task callback function.
 *
 * SOURCE
 */
#define SILC_TASK_CALLBACK(func)				\
static void func(SilcSchedule schedule, void *app_context,	\
		 SilcTaskEvent type,				\
		 SilcUInt32 fd, void *context)
/***/

/****d* silcutil/SilcScheduleAPI/SILC_TASK_CALLBACK_GLOBAL
 *
 * NAME
 * 
 *    #define SILC_TASK_CALLBACK_GLOBAL ...
 *
 * DESCRIPTION
 *
 *    Generic macro to define task callback functions. This defines a
 *    function with name `func' as a task callback function.  This
 *    differs from SILC_TASK_CALLBACK in that the defined function is
 *    not static.
 *
 * SOURCE
 */
#define SILC_TASK_CALLBACK_GLOBAL(func)					\
void func(SilcSchedule schedule, void *app_context, SilcTaskEvent type,	\
	  SilcUInt32 fd, void *context)
/***/

/* Prototypes */

/****f* silcutil/SilcScheduleAPI/silc_schedule_init
 *
 * SYNOPSIS
 *
 *    SilcSchedule silc_schedule_init(int max_tasks, void *app_context);
 *
 * DESCRIPTION
 *
 *    Initializes the scheduler. This returns the scheduler context that
 *    is given as argument usually to all silc_schedule_* functions.
 *    The `max_tasks' indicates the number of maximum tasks that the
 *    scheduler can handle. The `app_context' is application specific
 *    context that is delivered to all task callbacks. The caller must
 *    free that context.  The 'app_context' can be for example the
 *    application itself.
 *
 ***/
SilcSchedule silc_schedule_init(int max_tasks, void *app_context);

/****f* silcutil/SilcScheduleAPI/silc_schedule_uninit
 *
 * SYNOPSIS
 *
 *    bool silc_schedule_uninit(SilcSchedule schedule);
 *
 * DESCRIPTION
 *
 *    Uninitializes the scheduler. This is called when the program is ready
 *    to end. This removes all tasks from the scheduler. Returns FALSE if the
 *    scheduler could not be uninitialized. This happens when the scheduler
 *    is still valid and silc_schedule_stop has not been called.
 *
 ***/
bool silc_schedule_uninit(SilcSchedule schedule);

/****f* silcutil/SilcScheduleAPI/silc_schedule_reinit
 *
 * SYNOPSIS
 *
 *    SilcSchedule silc_schedule_reinit(int max_tasks);
 *
 * DESCRIPTION
 *
 *    This function can be called to enlarge the task handling capabilities
 *    of the scheduler indicated by `schedule'.  The `max_tasks' must be
 *    larger than what was set in silc_schedule_init function.  This function
 *    returns FALSE if it cannot reinit the scheduler.  This function does
 *    not do anything else except ready the scheduler to handle `max_tasks'
 *    number of tasks after this function returns.  It is safe to call this
 *    function at any time, and it is guaranteed that existing tasks remain
 *    as they are in the scheduler.
 *
 ***/
bool silc_schedule_reinit(SilcSchedule schedule, int max_tasks);

/****f* silcutil/SilcScheduleAPI/silc_schedule_stop
 *
 * SYNOPSIS
 *
 *    void silc_schedule_stop(SilcSchedule schedule);
 *
 * DESCRIPTION
 *
 *    Stops the scheduler even if it is not supposed to be stopped yet. 
 *    After calling this, one must call silc_schedule_uninit (after the 
 *    silc_schedule has returned).  After this is called it is guaranteed
 *    that next time the scheduler enters the main loop it will be stopped.
 *    However, untill it enters the main loop it will not detect that
 *    it is stopped for example if this is called from another thread.
 *
 ***/
void silc_schedule_stop(SilcSchedule schedule);

/****f* silcutil/SilcScheduleAPI/silc_schedule
 *
 * SYNOPSIS
 *
 *    void silc_schedule(SilcSchedule schedule);
 *
 * DESCRIPTION
 *
 *    The SILC scheduler. This is actually the main routine in SILC programs.
 *    When this returns the program is to be ended. Before this function can
 *    be called, one must call silc_schedule_init function.
 *
 ***/
void silc_schedule(SilcSchedule schedule);

/****f* silcutil/SilcScheduleAPI/silc_schedule_one
 *
 * SYNOPSIS
 *
 *    bool silc_schedule_one(SilcSchedule schedule, int block);
 *
 * DESCRIPTION
 *
 *    Same as the silc_schedule but runs the scheduler only one round
 *    and then returns.  This function is handy when the SILC scheduler
 *    is used inside some other external scheduler, for example.  If
 *    the `timeout_usecs' is non-negative a timeout will be added to the
 *    scheduler.  The function will not return in this timeout unless
 *    some other event occurs.
 *
 ***/
bool silc_schedule_one(SilcSchedule schedule, int timeout_usecs);

/****f* silcutil/SilcScheduleAPI/silc_schedule_wakeup
 *
 * SYNOPSIS
 *
 *    void silc_schedule_wakeup(SilcSchedule schedule);
 *
 * DESCRIPTION
 *
 *    Wakes up the scheduler. This is used only in multi-threaded
 *    environments where threads may add new tasks or remove old tasks
 *    from the scheduler. This is called to wake up the scheduler in the
 *    main thread so that it detects the changes in the scheduler.
 *    If threads support is not compiled in this function has no effect.
 *    Implementation of this function may be platform specific.
 *
 ***/
void silc_schedule_wakeup(SilcSchedule schedule);

/****f* silcutil/SilcScheduleAPI/silc_schedule_get_context
 *
 * SYNOPSIS
 *
 *    void *silc_schedule_get_context(SilcSchedule schedule);
 *
 * DESCRIPTION
 *
 *    Returns the application specific context that was saved into the
 *    scheduler in silc_schedule_init function.  The context is also
 *    returned to application in task callback functions, but this function
 *    may be used to get it as well if needed.
 *
 ***/
void *silc_schedule_get_context(SilcSchedule schedule);

/****f* silcutil/SilcScheduleAPI/silc_schedule_get_context
 *
 * SYNOPSIS
 *
 *    SilcMutex silc_schedule_get_callback_lock(SilcSchedule schedule);
 *
 * DESCRIPTION
 *
 *    Returns the callback lock to allow other threads to lock
 *    around Silc function calls.
 *
 ***/
SilcMutex silc_schedule_get_callback_lock(SilcSchedule schedule);

/****f* silcutil/SilcScheduleAPI/silc_schedule_task_add
 *
 * SYNOPSIS
 *
 *    SilcTask silc_schedule_task_add(SilcSchedule schedule, SilcUInt32 fd,
 *                                    SilcTaskCallback callback, 
 *                                    void *context, 
 *                                    long seconds, long useconds, 
 *                                    SilcTaskType type, 
 *                                    SilcTaskPriority priority);
 *
 * DESCRIPTION
 *
 *    Registers a new task to the scheduler. This same function is used
 *    to register all types of tasks. The `type' argument tells what type
 *    of the task is. Note that when registering non-timeout tasks one
 *    should also pass 0 as timeout, as the timeout will be ignored anyway. 
 *    Also, note, that one cannot register timeout task with 0 timeout.
 *    There cannot be zero timeouts, passing zero means no timeout is used
 *    for the task and SILC_TASK_FD is used as default task type in
 *    this case.
 *
 *    The `schedule' is the scheduler context. The `fd' is the file
 *    descriptor of the task. On WIN32 systems the `fd' is not actual
 *    file descriptor but some WIN32 event handle. On WIN32 system the `fd'
 *    may be a socket created by the SILC Net API routines, WSAEVENT object
 *    created by Winsock2 network routines or arbitrary WIN32 HANDLE object.
 *    On Unix systems the `fd' is always the real file descriptor.
 *
 *    The `callback' is the task callback that will be called when some
 *    event occurs for this task. The `context' is sent as argument to
 *    the task `callback' function. For timeout tasks the callback is
 *    called after the specified timeout has elapsed.
 *
 *    If the `type' is SILC_TASK_TIMEOUT then `seconds' and `useconds'
 *    may be non-zero.  Otherwise they should be zero. The `priority'
 *    indicates the priority of the task.
 *
 *    It is always safe to call this function in any place. New tasks
 *    may be added also in task callbacks, and in multi-threaded environment
 *    in other threads as well.
 *   
 ***/
SilcTask silc_schedule_task_add(SilcSchedule schedule, SilcUInt32 fd,
				SilcTaskCallback callback, void *context, 
				long seconds, long useconds, 
				SilcTaskType type, 
				SilcTaskPriority priority);

/****f* silcutil/SilcScheduleAPI/silc_schedule_task_del
 *
 * SYNOPSIS
 *
 *    void silc_schedule_task_del(SilcSchedule schedule, SilcTask task);
 *
 * DESCRIPTION
 *
 *    Deletes the `task' from the scheduler indicated by the `schedule'.
 *    After deleting the task it is guaranteed that the task callback
 *    will not be called. If the `task' is SILC_ALL_TASKS then all
 *    tasks is removed from the scheduler.
 *
 *    It is safe to call this function in any place. Tasks may be removed
 *    in task callbacks (including in the task's own task callback) and
 *    in multi-threaded environment in other threads as well.
 *
 ***/
void silc_schedule_task_del(SilcSchedule schedule, SilcTask task);

/****f* silcutil/SilcScheduleAPI/silc_schedule_task_del_by_fd
 *
 * SYNOPSIS
 *
 *    void silc_schedule_task_del_by_fd(SilcSchedule schedule, SilcUInt32 fd);
 *
 * DESCRIPTION
 *
 *    Deletes a task from the scheduler by the specified `fd'.
 *
 *    It is safe to call this function in any place. Tasks may be removed
 *    in task callbacks (including in the task's own task callback) and
 *    in multi-threaded environment in other threads as well.
 *
 *    Note that generic tasks cannot be deleted using this function
 *    since generic tasks does not match any specific fd.
 *
 ***/
void silc_schedule_task_del_by_fd(SilcSchedule schedule, SilcUInt32 fd);

/****f* silcutil/SilcScheduleAPI/silc_schedule_task_del_by_callback
 *
 * SYNOPSIS
 *
 *    void silc_schedule_task_del_by_callback(SilcSchedule schedule,
 *                                            SilcTaskCallback callback);
 *
 * DESCRIPTION
 *
 *    Deletes a task from the scheduler by the specified `callback' task
 *    callback function.
 *
 *    It is safe to call this function in any place. Tasks may be removed
 *    in task callbacks (including in the task's own task callback) and
 *    in multi-threaded environment in other threads as well.
 *
 ***/
void silc_schedule_task_del_by_callback(SilcSchedule schedule,
					SilcTaskCallback callback);

/****f* silcutil/SilcScheduleAPI/silc_schedule_task_del_by_context
 *
 * SYNOPSIS
 *
 *    void silc_schedule_task_del_by_context(SilcSchedule schedule, 
 *                                           void *context);
 *
 * DESCRIPTION
 *
 *    Deletes a task from the scheduler by the specified `context'.
 *
 *    It is safe to call this function in any place. Tasks may be removed
 *    in task callbacks (including in the task's own task callback) and
 *    in multi-threaded environment in other threads as well.
 *
 ***/
void silc_schedule_task_del_by_context(SilcSchedule schedule, void *context);

/****f* silcutil/SilcScheduleAPI/silc_schedule_set_listen_fd
 *
 * SYNOPSIS
 *
 *    void silc_schedule_set_listen_fd(SilcSchedule schedule, SilcUInt32 fd,
 *                                     SilcTaskEvent mask, bool send_events);
 *
 * DESCRIPTION
 *
 *    Sets a file descriptor `fd' to be listened by the scheduler for
 *    `mask' events.  To tell scheduler not to listen anymore for this
 *    file descriptor call the silc_schedule_unset_listen_fd function.
 *    When new task is created with silc_schedule_task_add the event
 *    for the task's fd is initially set to SILC_TASK_READ. If you need
 *    to control the task's fd's events you must call this function
 *    whenever you need to change the events. This can be called multiple
 *    times to change the events.
 *
 *    If the `send_events' is TRUE then this function sends the events
 *    in `mask' to the application.  If FALSE then they are sent only
 *    after the event occurs in reality.  In normal cases the `send_events'
 *    is set to FALSE.
 *
 ***/
void silc_schedule_set_listen_fd(SilcSchedule schedule, SilcUInt32 fd,
				 SilcTaskEvent mask, bool send_events);

/****f* silcutil/SilcScheduleAPI/silc_schedule_unset_listen_fd
 *
 * SYNOPSIS
 *
 *    void silc_schedule_unset_listen_fd(SilcSchedule schedule, SilcUInt32 fd);
 *
 * DESCRIPTION
 *
 *    Tells the scheduler not to listen anymore for the specified
 *    file descriptor `fd'. No events will be detected for the `fd'
 *    after calling this function.
 *
 ***/
void silc_schedule_unset_listen_fd(SilcSchedule schedule, SilcUInt32 fd);

/****f* silcutil/SilcScheduleAPI/silc_schedule_signal_register
 *
 * SYNOPSIS
 *
 *    void silc_schedule_signal_register(SilcSchedule schedule, 
 *                                       SilcUInt32 signal,
 *					 SilcTaskCallback callback,
 * 					 void *context);
 *
 * DESCRIPTION
 *
 *    Register signal indicated by `signal' to the scheduler.  Application
 *    should register all signals it is going to use to the scheduler.
 *    The `callback' with `context' will be called after the application
 *    has called silc_schedule_signal_call function in the real signal 
 *    callback.  Application is responsible of calling that, and the 
 *    signal system will not work without calling silc_schedule_signal_call
 *    function.  The specified `signal' value will be also delivered to
 *    the `callback' as the fd-argument.  The event type in the callback
 *    will be SILC_TASK_INTERRUPT.  It is safe to use any SILC routines
 *    in the `callback' since it is actually called after the signal really
 *    happened.
 *
 *    On platform that does not support signals calling this function has 
 *    no effect.
 *
 * EXAMPLE
 *
 *    Typical signal usage case on Unix systems:
 *
 *    struct sigaction sa;
 *    sa.sa_handler = signal_handler;
 *    sigaction(SIGHUP, &sa, NULL);
 *    sigaction(SIGINT, &sa, NULL);
 *    silc_schedule_signal_register(schedule, SIGHUP, hup_signal, context);
 *    silc_schedule_signal_register(schedule, SIGINT, int_signal, context);
 *
 *    static void signal_handler(int sig)
 *    {
 *      silc_schedule_signal_call(schedule, sig);
 *    }
 *
 *    The `signal_handler' can be used as generic signal callback in the
 *    application that merely calls silc_schedule_signal_call, which then
 *    eventually will deliver for example the `hup_signal' callback.  The 
 *    same `signal_handler' can be used with all signals.
 *
 ***/
void silc_schedule_signal_register(SilcSchedule schedule, SilcUInt32 signal,
				   SilcTaskCallback callback, void *context);

/****f* silcutil/SilcScheduleAPI/silc_schedule_signal_unregister
 *
 * SYNOPSIS
 *
 *    void silc_schedule_signal_unregister(SilcSchedule schedule, 
 *                                         SilcUInt32 signal,
 *					   SilcTaskCallback callback,
 * 					   void *context);
 *
 * DESCRIPTION
 *
 *    Unregister a signal indicated by `signal' from the scheduler.  On
 *    platform that does not support signals calling this function has no
 *    effect.
 *
 ***/
void silc_schedule_signal_unregister(SilcSchedule schedule, SilcUInt32 signal,
				     SilcTaskCallback callback, void *context);

/****f* silcutil/SilcScheduleAPI/silc_schedule_signal_call
 *
 * SYNOPSIS
 *
 *    void silc_schedule_signal_call(SilcSchedule schedule, 
 *                                   SilcUInt32 signal);
 *
 * DESCRIPTION
 *
 *    Mark the `signal' to be called later.  Every signal that has been
 *    registered by silc_schedule_signal_register is delivered by calling
 *    this function.  When signal really occurs, the application is 
 *    responsible of calling this function in the signal handler.  After
 *    signal is over the scheduler will then safely deliver the callback
 *    that was given to silc_schedule_signal_register function.
 *
 ***/
void silc_schedule_signal_call(SilcSchedule schedule, SilcUInt32 signal);

#endif
