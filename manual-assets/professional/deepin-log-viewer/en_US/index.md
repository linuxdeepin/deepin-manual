# Log Viewer|../common/deepin-log-viewer.svg|

## Overview 

Log viewer is a tool that collects logs generated when an application is running, for example, logs generated when operating system and applications start up and run. You can do trouble-shooting and solve problems quickly by analyzing logs. 

![0|homepage](jpg/homepage.png)

## Guide

You can run, close or create a shortcut for Log Viewer.

### Run Log Viewer 

1. Click ![deepin_launcher](icon/deepin_launcher.svg)on the Dock to enter the interface of launcher.
2. Locate ![deepin_log_viewer](icon/deepin_log_viewer.svg) by scrolling the mouse wheel or searching "Log Viewer" in the Launcher interface and click it to run.
3. Right-click![deepin_log_viewer](icon/deepin_log_viewer.svg) to:
   - Select **Send to desktop** to create a shortcut on the desktop.

   - Select  **Send to dock** to fix it onto the dock.

   - Select **Add to startup** to run it automatically when the computer is turned on.

### Exit Log Viewer 

- On the main interface, click  ![close_icon](icon/close_icon.svg) to exit.
- Right-click ![deepin_log_viewer](icon/deepin_log_viewer.svg) on the Dock, select **Close all** to exit.
- On interface of Log Viewer, click ![icon_menu](icon/icon_menu.svg) and select **Exit** to exit.

## Operations

### Search

1. Click![search](icon/search.svg)in the search box.
2. Input keywords. And the results are displayed after input. 
3. Click ![close_icon](icon/close_icon.svg) to clear up current searching.

![0|search](jpg/search.png)

### Filter

 Filtering parameters include **period**, **level**, **status**, **application list** and **event type**.

<table border="1">
   <tr>
    <th>Filtering Type</th>
 <th>Description </th>
</tr>
   <tr>
    <td>Filtering by period</td>
    <td>to filter in the order of log generating date
      <ul>
          <li>Options include: "All", "Today", "3 days", "1 week", "1 month", and "3 months". Default option is "All".</li>
          <li>Applicable for: "period" is not displayed only for boot logs, other kinds of logs support filtering by period.</li>
      </ul>
 </td>
</tr>
   <tr>
    <td>Filtering by level</td>
    <td>to filter by seriousness levels of events
    <ul>
          <li>Options include: "All", "Emergency", "Alert", "Critical", "Error", "Warning", "Notice", "Info"and "Debug". Default option is "Info".</li>
          <li>Applicable for: system logs and application logs</li>
      </ul>
      <tr>
    <td>Filtering by status</td>
    <td>
    <ul>
          <li>Options include: "All", "OK", and "Failed". Default option is "All".</li>
          <li>Applicable for: boot logs only</li>
      </ul>
          <tr>
    <td>Filtering by application list: </td>
    <td>to filter log files of applications, default option is the first one. 
        Applicable for: application logs only.  </td>
              <tr>
    <td>Filtering by event type</td>
    <td>
    <ul>
          <li>Options include:  "All", "Login", "Reboot", and "Shutdown", the default option is "all".  </li>
          <li>Applicable for: boot-shutdown events only.</li>
      </ul>
 </td>
   </tr>
   </table>

### View Logs

#### System Log

1. On the main interface, click **System Log**. 
2. You can see a list of system logs which are detailed by "Level", "Process", "Date and Time", and "Info".
3. Click a log, you can see its particular info below the list, including process, user, PID, time and info.

![0|rsyslog](jpg/rsyslog.png)

#### Kernel Log

1. On the main interface, click  **Kernel Log**.
2. Input password in the popped up authentication window. 
3. You can view the status and information of kernel logs.
4. Click a log, you can see its particular info below the list, including time, user, process, info, etc.



![0|kernlog](jpg/kernlog.png)

#### Boot Log

1. On the main interface, click **Boot Log**.
2. Input password in the popped up authentication window. 
3. You can view the status and information of boot logs.
4. Click a log, you can see its particular info below the list, including user, status,  info, etc.



![0|bootlog](jpg/bootlog.png)

#### dpkg Log

1. On the main interface, click **dpkg Log**. 
2. You can view time and information of dpkg logs.
3. Click a log, you can see its particular info below the list, including user, action, time, info, etc.

![0|dpkglog](jpg/dpkglog.png)

#### Xorg Log

1. On the main interface, click **Xorg Log**.
2. You can view time and information of Xorg logs.
3. Click a log, you can see its particular info below the list, including user, time, info, etc.

![0|xorglog](jpg/xorglog.png)

#### Application Log

1. On the main interface, click  **Application Log**.
2. You can view level, time, source and information of application logs.
3. Click a log, you can see its particular info below the list, including user, time info, etc.

![0|applog](jpg/applog.png)


#### Boot-Shutdown Event

1. On the main interface, click  **Boot-Shutdown Event**.
2. You can view event type, username, time and information of boot-shutdown events.
3. Click a log, you can see its particular info below the list, including user, time, event type, detailed info and so on.
![0|powerevent](jpg/powerevent.png)

### Export

You can export logs and save them in format of  txt/doc/xls/html.

1. On the main interface, click **Export**. 
2. An interface of file manager pops up. Select the storing path, input a file name and select a format. 
3. Click **Save**. When the log file is exported successfully, a hint pops up in the lower part of the interface.

### Refresh
1. On the interface of File Manager, select a log type in the left panel and right-click.
2. Select **Refresh** to reload the log and update log list.


### Display in File Manager

**Display in file manager** is only applicable for kernel log, boot log, dpkg log, Xorg log and application log.

1. On the interface of File Manager, select a log type in the left panel and right-click.
2. Select **Display in file manager**, the folder path of the current log will be opened. 

![display](jpg/displayinfm.png)

### Clear Log

**Clear log** is only applicable for kernel log, boot log, dpkg log, Xorg log and application log.

1. On the interface of File Manager, select a log type in the left panel and right-click.
2. Select **Clear log**, click **Confirm** button in the pop-up window to clear current logs.
3. If authentication window pops up, please input password to finish the operation.


## Main Menu

In the main menu, you can switch themes, view help manual, and so on.

### Theme

The window theme includes Light Theme, Dark Theme and System Theme.

1. Click ![icon_menu](icon/icon_menu.svg)on the interface.
2. Click **Theme** to select one.


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


<div class="version-info"><span>Update Date: 2022-01-13</span><span> Version: 5.8</span></div>