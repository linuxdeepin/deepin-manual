# System Monitor |../common/deepin-system-monitor.svg|

## Overview

System Monitor is a system tool monitoring and managing hardware load, program running and system services. It's capable to real-time monitor CPU status, memory footprint, and upload/download speed, manage system and application processes, search and force end processes as well.


## Basic Operations

### Search Process

1. In the searching box, you can :
- Click ![search](icon/search.svg) to input keywords. 
- Click ![button_voice](icon/button_voice.svg) to speak, and your speech will be turned into words and displayed in the searching box. 
2. Press  **Enter** to search. 
- All matching information will be displayed in a list. 
- If searching fails, "No results" will be displayed. 

![1|search](jpg/search.png)



### Hardware Monitoring

System Monitor can monitor the CPU, memory and network status of your system:

- CPU monitoring, you can view the CPU usage and usage trends through graphics, number and curve.

  - In "Expand view", CPU load is displayed in doughnut chart with percentage. The curve inside the doughnut shows the recent CPU load summary, the height of which automatically fits the inner height of the doughnut height. 
  ![1|monitor](jpg/expand.png)
  - In "Compact view", CPU load is displayed in oscillogram with percentage. The curve shows the recent CPU load summary, the height of which automatically fits the height of the oscillogram. 
  ![1|monitor](jpg/compact.png)
- Memory monitoring shows the memory real-time usage, total memory and current usage, total swap space and current usage in percentage and graphics.
- Network monitoring shows the current downloading and uploading speed, and the trend in recent time in waves.
- Disk monitoring shows the current reading and writing speed, and the trend in recent time in waves.


### Process Management

#### Switch Process Tabs

Click the top right tabs on interface to view "Applications", "My processes" and "All processes".

- Click ![application](icon/application.png) to switch to **Applications**. 
- Click ![user](icon/user.png) to switch to **My processes**. 
- Click ![all](icon/all.png) to switch to **All processes**.

![1|monitor](jpg/tab-switch.png)

#### Adjust Process Order

You can adjust the process order by Name, CPU, User, Memory, Upload, Download, Disk read, Disk write, PID, Nice and Priority.

1. On System Monitor interface, click the top tabs of process list to sort the processes. Multiple clicks to sort from low to high or high to low.
3. Right-click the top tabs to check options you want to view or to uncheck options you want to hide.

![1|sort](jpg/sort.png)


#### End Process

System Monitor can be used to end processes.

1. On System Monitor interface, right-click the process you want to end.
2. Select **End Process**.
3. Click **End** in the pop-up window to confirm.

![1|kill](jpg/kill.png)

#### End Application

1. On System Monitor interface, click ![icon_menu](icon/icon_menu.svg).

2. Select **Force end application**.
> ![notes](icon/notes.svg)：This function can only end GUI processes.

3. Click the opened application window under the prompt.

4. Click **Force End** in the pop-up window to confirm.

> ![tips](icon/tips.svg): You can select a process, right-click and select **End Process**. 


#### Suspend/Resume Process

1. On System Monitor interface, right-click the process you want to suspend.
2. Select **Suspend process**. The process will be tagged with **(Suspend)** and becomes red in the list. 
3. Right-click it again and select **Resume process** to resume it.

![1|pause](jpg/pause.png)


#### Change Process Priority 

1. On System Monitor interface, right-click a process.

2. Select **Change priority** and select a level.

![notes](icon/notes.svg): You can also custom a priority level.

![1|priority](jpg/priority.png)


#### View Process Location

1. On System Monitor interface, right-click the process you want to view.
2. Select **View command location** to open the location in File Manager. 

#### View Process Properties

1. On System Monitor interface, right-click the process you want to view.
2. Select **Properties** to view the process name, command line and start time.

![0|info](jpg/info.png)

### System service management

You can start, stop, restart, set startup type, refresh system service processes.
In the list of system services, force end application is prohibited.
>![attention](icon/attention.svg): For better system operation, please don't end any processes of the system services and root processes.

#### Start system service
1. Select **Services** tab on System Monitor interface.
2. Select a process that is not started yet, right-click and select **Start**.
3. Please enter password if authentication window pops up.
4. Right-click and select **Startup type**.
5. Right-click and select **Refresh**.

**Active** is displayed in the **Active** column.

#### Stop system service

1. Select **Services** tab on System Monitor interface.
2. Select a process that is started already, right-click and select **Stop**.
3. Please enter password if authentication window pops up.
4. Right-click and select **Refresh**.

**Inactive** is displayed in the **Active** column.

You can also right-click a system process and select **Restart**. 

## Main Menu

### Switch Theme

The window theme includes Light Theme, Dark Theme and System Theme (default).

1. Click![icon_menu](icon/icon_menu.svg) on the interface.
2. Click **Theme** to choose one.

### View

System Monitor offers expand view and compact view for you to choose.

1. Click![icon_menu](icon/icon_menu.svg) on the interface.
2. Click **View** to choose one mode.


### Help

You can click to view the manual, which will help you further know and use System Monitor.

1. Click![icon_menu](icon/icon_menu.svg) on the interface.
2. Click **Help**.
3. View the manual.

### About

1. Click![icon_menu](icon/icon_menu.svg) on the interface.
2. Click **About**.
3. View the version description.

### Exit

1. Click![icon_menu](icon/icon_menu.svg) on the interface.
2. Click **Exit**.
