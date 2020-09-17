# Cloud Print|../common/deepin-cloud-print.svg|

## Overview

Cloud Print is a new printing solution. It will connect your printer to the network, and is enabled for network printing via daily used applications. Cloud Print is suitable for desktops, laptops, some tablets and other networking devices that you have authorized to print.

Cloud Print consists of Server (in Windows) and Client (in UOS), which will be configured respectively.

## Server Configurations

You can install the application program for Cloud Print Server and configure the authorization code in Windows.

### Install Cloud Print

1. Download the installation program for Cloud Print Server.
2. Install in Windows.
3. Finish the installation by wizard.

### Set The Authorization Code

The server IP address will be acquired in Windows automatically after finished the installation, then you can set the authorization code.

> ![notes](icon/notes.svg): If you need to change the authorization code, please input a new one in the textbox, and then click on **Update**.

1. Click the start menu to open the program of Cloud Print Server in Windows.
2. Acquire the IP address automatically.
3. Input the authorization code in the settings interface of Cloud Print Server.
4. Click on **Update**, then ![spinner](icon/icon_spinner.png) will be displayed.
5. After updated the authorization code, ![success](icon/icon_success.svg) will be displayed.

 ![0|windowssetting](jpg/windowssetting.jpg)

## Client Configurations

You can open Cloud Print Client in UOS to add one or more cloud printers.

### Open Cloud Print

1. Click on ![launcher](icon/deepin-launcher.svg) to enter launcher.
2. Click on ![printer-24](icon/printer-24.svg) to enter the interface of Cloud Print.

### Set Cloud Print

1. On the interface of Cloud Print, input the IP address and authorization code of Cloud Print Server from Windows.
2. Click on ![icon_next](icon/icon_next.svg) to show all printers from Windows.
3. Click on ![icon_plus](icon/icon_plus.svg) to add the printer, and then its status will be displayed in "Added".
4. Click on ![icon_setting](icon/icon_setting.svg) to enter the interface of print settings.
5. You can view whether the printer is added successfully.

 ![1|deepinssetting](jpg/deepinssetting.png)

> ![notes](icon/notes.svg): If you need to delete the printer, please enter "Print Settings" to select the printer, then select **Delete** in the context menu.

## Print Test 

You can use cloud print to print files in UOS, taking .doc files as the example.

1. Open .doc files by WPS.
2. Click on **Writer** on the top left corner.
3. Select **Print** to enter the interface of print settings.
4. Select the printer in the pull-down option of **Name**, and set other print parameters.
5. Click on **OK**.

 ![0|printsetting](jpg/printsetting.jpg)

> ![attention](icon/attention.svg): If authorization code has been updated in Windows, when you are printing a file, there will be a prompt of "The authorization code of cloud print server has been updated, please input a new authorization code". Please contact the administrator to get the new one to print. If errors occurred during printing, please reset according to the errors.

<div class="version-info"><span>Update Date: 2020-08-24</span><span> Version: (无)
</span></div>