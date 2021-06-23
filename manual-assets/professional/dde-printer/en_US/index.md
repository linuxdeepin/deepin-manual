# Printer Manager|../common/dde-printer.svg|

## Overview

Printer Manager is an easy-to-use CUPS-based tool that manages several printers at a time with a simple interface providing users with great convenience to add printers and install drivers quickly.

![0|homepage](jpg/homepage.png)

## Guide

You can run, close or create a shortcut for Printer Manager as follows.

### Run Printer Manager

1. Click ![deepin-launcher](icon/deepin-launcher.svg) on the Dock to enter the interface of launcher.
2. Locate ![dde-printer](icon/dde-printer.svg) by scrolling the mouse wheel or searching "Printer Manager" in the Launcher interface and click it to run. 
3. Right-click![dde-printer](icon/dde-printer.svg) to:
   - Select **Send to desktop** to create a shortcut on the desktop.
   - Select  **Send to dock** to fix it onto the dock.
   - Select **Add to startup** to run it automatically when the computer is turned on.

### Exit Printer Manager

- On the main interface, click  ![close_icon](icon/close.svg) to exit.
- Right-click ![dde-printer](icon/dde-printer.svg) on the Dock, select **Close all** or **Force Quit** to exit.
- On the main interface, click ![icon_menu](icon/icon_menu.svg)  and select **Quit** to exit.

## Operations

### Add Printer

On the main interface, click![+](icon/+.svg) and you will have three options including **Discover Printer**, **Find Printer**, and **Enter URI**. 

#### Discover Printer

1. Click **Discover Printer** to load printer list automatically. Select the printer you want to add.

2. You can see a drop-down list of drivers for the printer selected. The recommended driver is selected by default. If you select "Select a driver" from the drop-down list, you will skip into an interface where you can select a driver you want.

   ![0|2automatic search](jpg/2automatic search.png)

3. Click  **Install Driver** to enter installing interface.


#### Find Printer

1. Click **Find Printer** , input host name or IP address to search printers through different protocols. 

   ![0|3manual search](jpg/3manual search.png)

2. After you select a printer, its driver list is loaded below, and the recommended driver is selected by default. If no list is loaded, you can choose "Select a driver" from the drop down list to enter an interface to choose a driver.

3. Click **Install Driver** to enter installing interface. 


#### Enter URI

1. If no printers are found by  **Discover Printer** and  **Find Printer** , you can find and install printer driver by entering URI.  

2. Click **Enter URI** and input printer's URI.

   ![0|4URI search](jpg/4URI search.png)

3. "Select a driver" is set by default. Click **Next** to enter driver installing interface.  

4. Choose the correct driver and click **Install Driver** to enter installing interface. 

> ![notes](icon/notes.svg)Notes: Prerequisite of this operation is that user knows the printer's URI and applicable protocol. 


### Select drivers

1. Default driver: The system will choose recommended driver by default if there is a correct driver for the printer you select. 

2. Select a driver: you can choose "select a driver". Sources of drivers include: 


   * Local driver: select vendor and model from drop-down box to query local drivers.

     ![0|5local driver](jpg/5local driver.png)

   * Local PPD files: drag and drop local PPD files here or click **Select a PPD file** to find one in local folder, for example, you can select a PPD file under the directory of /usr/share/ppd.  

   > ![notes](icon/notes.svg)Notes: Prerequisite of this operation is that user has installed driver locally. If not, you will get a hint "Driver install failed".

   ![0|6ppd document](jpg/6ppd document.png)

   * Search for a driver: input particular vendor and model, and the system will search in background driver library. Results will be listed in the drop-down box. 

   ![0|7search driver](jpg/7search driver.png)

### Install Printer

After you add a printer and select the correct driver, click **Install Driver** to enter installing interface.  

- Successfully Installed 
  A window pops up saying the printer is successfully installed, in which you can click **Print Test Page** to verify if printing works normally, or, you can click **View Printer** to enter the interface of printer manager. 

  ![0|8successful installion](jpg/8successful install.png)

- Install Failed
  
  You can choose to reinstall if you get a hint saying "Install Failed". 

  

### Printer Manager Interface

Once a printer is added successfully, when you click the printer in the interface of printer manager, you can see four buttons on the right, which are "Properties", "Print Queue", "Print Test Page", and "Troubleshoot". 

![0|9printer management](jpg/9printer-management.png)

#### Properties 

1. Click **Properties** to view all settings of the printer.

2. You can view the info about the printer's driver, URI, location, description, color mode and resolution and so on.

3. Settable items include: paper source, paper type, paper size, duplex, margins, orientation, page order and so on. You can set them according to your actual needs.
> ![notes](icon/notes.svg)Notes: **Properties** are related to the printer model and its driver. Please refer to the actual situation. 


#### Print Queue

1. Click **Print Queue**, you will enter an interface, you can select options such as "All", "Print queue","Completed" and "Refresh". **Print Queue** is displayed by default. 

2. The list contains: "Job", "User", "Document", "Printer", "Size", "Time Submitted", "Status" and "Action". 

3. Select a job, right-click and you can select **Cancel**, **Delete**, **Pause**, **Resume**, **Print first**,  and **Reprint**. 




![0|11right click setting](jpg/11right click setting.png)



#### Print Test Page

Click **Print Test Page** to test if printing is completed successfully. 

   - Print successfully: if printing is completed successfully, you can continue with other tasks.

   - Print failed: if printing fails, you can reinstall a printer or start troubleshooting. 

#### Troubleshoot

If printing fails, you can click **Troubleshoot**, the checklist includes following items: 

![0|12troubleshooting ](jpg/12troubleshooting.png)

   - Check if CUPS server is valid.

   - Check if driver is valid.

   - Check if printer settings are ok, i.e. if printer is started up, and, if tasks are accepted.

   - Check if printer connection is valid.

     
#### Right-click options 

Select the printer and right-click, you will have four options such as "Shared", "Enabled", "Accept jobs", and "Set as default". 

![0|9right click setting](jpg/9right click setting.png)

### Delete Printer

On the main interface, click![-](icon/-.svg) to delete selected printer. 

![0|14delete](jpg/14delete.png)


## Main Menu

In the main menu, you can set basic servers, switch themes, view help manual, and so on.

### Settings

1. Click ![icon_menu](icon/icon_menu.svg)on the interface.

2. Click **Settings**. Basic server settings include: 

   - Publish shared printers connected to this system: Allow printing from the Internet
   - Allow remote administration
   - Save debugging information for troubleshooting 

   


### Help

click Help to get the manual, which will help you further know and use Printer Manager.

1. Click ![icon_menu](icon/icon_menu.svg)on the interface.
2. Click **Help** to view the manual.


### About

1. Click ![icon_menu](icon/icon_menu.svg)on the interface.
2. Click **About** to view version information and introduction about Printer Manager.

### Exit

1. Click ![icon_menu](icon/icon_menu.svg)on the interface.
2. Click  **Exit**.


<div class="version-info"><span>Update Date: 2021-06-23</span><span> Version: 0.7</span></div>