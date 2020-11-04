# Log Viewer|../common/deepin-log-viewer.svg|

## Overview 

Log viewer is a tool that collects logs generated when an application is running, for example, logs generated when operating system and applications start up and run. You can do trouble-shooting and solve problems quickly by analyzing logs. 

![0|homepage](jpg/homepage.png)

## Guide

You can run, close or create a shortcut for Log Viewer.

### Run Log Viewer 

1. Click ![deepin-launcher](icon/deepin-launcher.svg)on the Dock to enter the interface of launcher.
2. Locate ![deepin-log-viewer](icon/deepin-log-viewer.svg) by scrolling the mouse wheel or searching "Log Viewer" in the Launcher interface and click it to run.
3. Right-click![deepin-log-viewer](icon/deepin-log-viewer.svg) to:
   - Select **Send to desktop** to create a shortcut on the desktop.

   - Select  **Send to dock** to fix it onto the dock.

   - Select **Add to startup** to run it automatically when the computer is turned on.

### Exit Log Viewer 

- On the main interface, click  ![close_icon](icon/close_icon.svg) to exit.
- Right-click ![deepin-log-viewer](icon/deepin-log-viewer.svg) on the Dock, select **Close all** or **Force Quit** to exit.
- On interface of Log Viewer, click ![icon_menu](icon/icon_menu.svg) and select **Exit** to exit.

## Operations

### Search

1. Log Viewer supports searching by keywords.
   - Click![search](icon/search.svg)to input keywords.
2. Results are displayed after input. If no matched service is found, you will see a hint **No search results**.
3. Click ![close_icon](icon/close_icon.svg) to close current searching.

![0|search](jpg/search.png)

### Filter

 Filtering parameters include **period**, **level**, **status**, **application list** and **event type**.. 

  - Filtering by period: Filtering in the order of log generating date

    Options include: "All", "Today", "3 days", "1 week", "1 month", and "3 months". Default option is "All".

    Applicable for: "period" is not displayed only for boot logs, other kinds of logs support filtering by period.

  - Filtering by level: filtering by seriousness levels of events

    Options include: "All", "Emergency", "Allert", "Critical", "Error", "Warning", "Notice", "Info"and "Debug". Default option is "Info".

    Applicable for: system logs, kernel logs, dnf logs and application logs

  - Filtering by status: 

    only applicable for boot logs, options include "All", "OK", and "Failed". Default option is "All".

  - Filtering by application list: 

    only applicable for application logs, it's used to filter log files of applications, default option is the first one. 

  - Filtering by event type: 

    only applicable for boot-shutdown events, options include:  "All", "Login", "Reboot", and "Shutdown", the default option is "all".  

    


### System Log

1. On the main interface, click **System Log **. 
2. You can see a list of system logs which are detailed by "Level", "Process", "Date and Time", and "Info".
3. Click a log, you can see its particular info below the list, including process, user, PID, time and info.

![0|rsyslog](jpg/rsyslog.png)

### Kernel Log

1. On the main interface, click  **Kernel Log **.
2. You can see a list of kernel logs which are detailed by "Date and time", "User", "Process" and "Info".
3. Click a log, you can see its particular info below the list, including time, user, process, and info.
> ![notes](icon/notes.svg)Notes: To view kernel log, a prompt appears saying **Authentication is required to view the log**, you can input login password to continue viewing.

![0|kernlog](jpg/kernlog.png)

### Boot Log

1. On the main interface, click **Boot Log**.
2. You can see a list of boot logs which are detailed by "Status" and "Info".
3. Click a log, you can see its particular info below the list, including process, user, status and info.
> ![notes](icon/notes.svg)Notes: To view boot log, a prompt appears saying **Authentication is required to view the log**, you can input login password to continue viewing.

![0|bootlog](jpg/bootlog.png)

### dnf Log

1. On the main interface, click **dnf Log**. 
2. You can see a list of dnf logs which are detailed by "Date and time" and "Info".
3. Click a log, you can see its particular info below the list, including user, level and info.

![0|dpkglog](jpg/dnflog.png)

### Xorg Log

1. On the main interface, click **Xorg Log**.
2. You can see a list of Xorg logs which are detailed by "Date and time" and "Info".
3. Click a log, you can see its particular info below the list, including user, time and info.

![0|xorglog](jpg/xorglog.png)

### Application Log

1. On the main interface, click  **Application Log **.
2. You can see a list of application logs which are detailed by "Level", "Date and Time", "Source" and "Info".
3. Click a log, you can see its particular info below the list, including source, user, time, level and info.

![0|applog](jpg/applog.png)


### Boot-Shutdown Event

1. On the main interface, click  **Boot-Shutdown Event**.
2. You can see a list of boot-shutdown events which are detailed by "Event Type", "Username", "Date and Time", and "Info".
3. Click a log, you can see its particular info below the list, including user, event type, username, and detailed info.
![0|power event](jpg/power-event.png)

### Export

You can export logs and save them in format of  txt/doc/xls/html.

1. On the main interface, click **Export**. 
2. An interface of file manager pops up. Input a file name and select a format. 
3. Click **Save**. 

![0|export](jpg/export.png)



### Display in File Manager

1. On the interface of File Manager, select a log type in the left panel and right-click.
2. Select **Display in file manager**, the folder path of the current log will be opened. 

> ![notes](icon/notes.svg)Notes: **Display in file manager** is only applicable for boot log, dnf log, Xorg log and application log.

### Clear Log
1. On the interface of File Manager, select a log type in the left panel and right-click.
2. Select **Clear log**, click **Confirm** button in the pop-up window to clear current logs.
3. If authentication window pops up, please input password to finish the operation.
> ![notes](icon/notes.svg)Notes: **Clear log** is only applicable for kernel log, boot log, dnf log, Xorg log and application log.

### Refresh
1. On the interface of File Manager, select a log type in the left panel and right-click.
2. Select **Refresh** to reload the log and update log list.

## Main Menu

In the main menu, you can switch themes, view help manual, and so on.

### Theme

The window theme includes Light Theme, Dark Theme and System Theme.

1. Click ![icon_menu](icon/icon_menu.svg)on the interface.
2. Click  **Theme** to select one.


### Help

Click Help to get the manual, which will help you further know and use Log Viewer.

1. Click ![icon_menu](icon/icon_menu.svg)on the interface.
2. Click **Help** to view the manual.


### About

1. Click ![icon_menu](icon/icon_menu.svg)on the interface.
2. Click **About** to view version information and introduction about Log Viewer.

### Exit

1. Click ![icon_menu](icon/icon_menu.svg)on the interface.
2. Click  **Exit**.

