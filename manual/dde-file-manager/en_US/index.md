# Deepin File Manager|../common/deepin-file-manager.svg|

## Overview

Deepin File Manager is developed by Deepin Technology team. As the core component to manage files in deepin, it is a powerful, easy-to-use file manager with an elegant and simple interface. Full-featured, well-designed, efficient-improved, Deepin File Manager will be a good help in your daily life and work.

![1|overview](jpg/overview.jpg)

## Guide

You can run, close and create a shortcut for Deepin File Manager by the following ways.

### Run Deepin File Manager

1. Click ![Launcher](icon/launcher_icon.svg) on dock to enter Launcher interface.
2. Locate ![File Manager](icon/deepin-file-manager.svg) by scrolling mouse wheel or searching by "deepin file manager" in Launcher interface.
3. Right click on ![File Manager](icon/deepin-file-manager.svg), you can
 - Click on **Send to desktop** to create a desktop shortcut.
 - Click on **Send to dock** to fix the application on dock.
 - Click on **Add to startup** to add the application to startup, it will automatically run when the system starts up.

> ![tips](icon/tips.svg): In Control Center, you can set Deepin File Manager as the default file manager. Specific operations refer to [Default Application Settings](dman:///dde#Default Application Settings).

### Exit Deepin File Manager

- On Deepin File Manager interface, click on ![close](icon/close_icon.svg) to exit.
- Right click on ![File Manager](icon/deepin-file-manager.svg) on Dock, select **Close All** to exit Deepin File Manager.
- Click on ![Settings](icon/icon_menu.svg) and select **Exit** to exit Deepin File Manager.

## Main Interface

On Deepin File Manager interface, users can execute many operations with the functional bars. It is designed to be easy-to-use, user-friendly and efficient improving.

![1|maininterface](jpg/main-interface.png)

<table class="block1">
    <caption></caption>
    <tbody>
        <tr>
            <td width="20px">1</td>
            <td width="90px">Navigation bar</td>
            <td>Displays user directory, mounted disks, bookmarks and tags.</td>
        </tr>
        <tr>
            <td>2</td>
            <td>Address bar</td>
            <td>Input keywords to search or input directory path to access corresponding contents. Use ![previous](icon/previous.png) ![next](icon/next.png) to go back or forward. Use breadcrumb bar to switch among parent directories and sub directories.</td>
        </tr>
        <tr>
            <td>3</td>
            <td>View</td>
            <td>User can switch view modes by clicking on ![icon view](icon/icon_view_normal.svg) and ![list view](icon/list_view_normal.svg).</td>
        </tr>
        <tr>
            <td>4</td>
            <td>Main menu</td>
            <td>User can create new window, switch to dark theme, set share password, make settings, view manual and application information, and exit the application.</td>
        </tr>
        <tr>
            <td>5</td>
            <td>Status bar</td>
            <td>Displays the number of files or selected files. User can drag the slider to adjust the size of icons in the directory.</td>
        </tr>
    </tbody>
</table>

> ![tips](icon/tips.svg): User can drag the separation line on the right of navigation bar to change its width.


## Basic Functions

Deepin File Manager is featured with the basic functions as a file manager, user can easily create, copy, rename or delete files/folders with it.

### New Document/Folder

#### New Folder

1. On Deepin File Manager interface, right click and select **New folder**.
2. Input a name for the new folder, and press ![enter](icon/Enter.svg), or click on blank area.

![0|newfolder](jpg/newfolder.jpg)

#### New Document

1. On Deepin File Manager interface, right click and select **New document**.
2. In the popup menu, select the document format you want to create.
3. Input a name for the new document, and press ![enter](icon/Enter.svg), or click on blank area.

![0|newdoc](jpg/newdoc.png)

### Rename File/Folder

1. Selected the file/folder, right click to select **Rename**.
2. Input a new name for the file/folder, and press ![enter](icon/Enter.svg), or click on blank area.

![0|rename](jpg/rename.jpg)

> ![tips](icon/tips.svg): Check "Hide file extension when rename" in **Settings** to rename file more conveniently.

### Batch Rename

#### Replace Text

1. Selected several files/folders, right click to select **Rename**.
2. Input the content to be replaced in Find box, and input content to rename in Replace box.
3. Press ![enter](icon/Enter.svg), or click on blank area.

#### Add Text

1. Selected several files/folders, right click to select **Rename**.
2. Input the content to add in Add box, and select the location.
3. Press ![enter](icon/Enter.svg), or click on blank area.

#### Custom Text

1. Selected several files/folders, right click to select **Rename**.
2. Input the file name and SN.
3. Press ![enter](icon/Enter.svg), or click on blank area.

![1|batch_rename](jpg/batch_rename.jpg)

### Copy File/Folder

1. Select the files/folders to be copied, right click and select **Copy**.
2. Enter the directory to which the files/folders will be pasted, right click and select **Paste**.

![0|copy](jpg/copy.jpg) ![0|paste](jpg/paste.jpg)

> ![notes](icon/notes.svg): A window will popup to show the progress as it starts copying. By clicking on ![close](icon/close_icon.svg) you can cancel the copying process. By hovering the mouse pointer to the round progress bar, it will switch to a pause button, you can pause the copying process by clicking on it.

### Compress File/Folder

1. Select the files/folders to be compressed, right click and select **Compress**.
2. Input a file name and select a file format for the to be compressed file.
3. Select a location to store the to be compressed file.
4. Select **Create** to generate the compressed file.

![0|compress](jpg/compress.jpg)

> ![tips](icon/tips.svg): In **Other Options**, user can choose to encrypt the compressed file and the file list, and also to split the to be compressed file into several parts to reduce individual file sizes of the compressed file.

### Delete File/Folder

1. Right click on file/folder to select **Delete**.
2. The file/folder will be moved to Trash.
  - To permanently delete, open Trash and right click on the file/folder to select **Delete** or right click on Trash to select **Empty**.
  - To restore deleted files/folders, open Trash and right click on the file/folder to select **Restore**, the files/folders will be moved to their original directory.

![0|delete](jpg/delete.jpg)

> ![notes](icon/notes.svg): File/folder in external device will be permanently deleted and can not be restored when you select **Delete**.

### Undo Operation
In file manager,  use ![Ctrl](icon/Ctrl.svg) + ![Z](icon/Z.svg) to undo the previous steps, including:
- Delete the newly created file/folder.
- Resume the renamed file/folder to the previous name.
- Restore the deleted file/folder from trash.
- Restore the moved (via drag or cut) file/folder to the previous location.
- Delete the copied and pasted file/folder.

> ![attention](icon/attention.svg): If the above operations are continuous, you can undo all of them; if there is file overwriting or permanently file deleting, then you can only return to that step.

### Options to Open a File

Users can select to open a file with different applications.

1. On Deepin File Manager, right click on the file to open.
2. In the context menu, select **Open with**.
3. Click on **Select default program** to open the application list.
4. Select the application you want to open the file.
5. Click on **Choose** to open the file by this application
6. You can also check **Set as default** to set the application as the default one.

If the application you want to open the file is not in the list, you can:

1. Click on **Select other programs**.
2. Select an application you want to open.
3. Click on **Open** to open the file by this application.

> ![notes](icon/notes.svg): It will use the default application to open the file when you double clicked on it. Users can right click on the file, select **Properties** and choose a default application in **Open with ** list. Users can also set the options in the [Default Application Settings](dman:///dde#Default Application Settings) of the Control Center to change the global default applications of the specified file types.

![0|open_with](jpg/open_with.png)

> ![tips](icon/tips.svg): You can right click multiple files with the same type, and select **Open with** to open them just once.

### Display Modes

Click on ![icon](icon/icon_view_normal.svg) or ![list](icon/list_view_normal.svg) on Deepin File Manager interface to switch between icon view and list view.

![0|display](jpg/display.png)

- Icon view: It will show the icon, name and thumbnail of the files.

![1|icon-view](jpg/icon-view.jpg)

- List view: It will show the icon, name, modified time, size and type of the files.

![1|list-view](jpg/list-view.jpg)

> ![tips](icon/tips.svg): User can drag the line between two columns to change the column width, or double click on the line to auto fit the content width.

> ![tips](icon/tips.svg): Press ![ctrl](icon/Ctrl.svg) +1 and ![ctrl](icon/Ctrl.svg) +2 to switch between icon view and list view.

### Sorting

1. Right click in the blank area on Deepin File Manager, select **Sort by**.
2. Choose among the sorting options, which include **Name**, **Time modified**, **Size** and **Type**.

![0|sort](jpg/sort.png)

> ![tips](icon/tips.svg): Users can click on the head of the columns in the list view to change the sort order.

### File/Folder Properties

Users can view the file/folder properties.

1. On Deepin File Manager interface, right click on a file/folder or more and select **Properties**.
2. The window of properties will pop up according to the file/ folder quantity:
  - File/ folder is less than 16, there will be a window of properties for each file/folder.
  - File/ folder is more than 16, there will be a general window of properties for all files/folders.
3. In the popup window, you can view the file/folder information.

![0|info](jpg/info.jpg)


> ![notes](icon/notes.svg): You can use shortcuts![Ctrl](icon/Ctrl.svg) + ![I](icon/I.svg) to view the properties as well.

## Common Operations

Deepin File Manager is featured with many useful functions, such as breadcrumb bar, tabs, bookmarks, file sharing, accessing computers in LAN, opening file/folder in Terminal and so on. They are all designed to ease the commonly used operations.

### Switch Address Bar

There are three sections in address bar, namely back/forward buttons, breadcrumb bar, and directory/search bar.

By default, breadcrumb bar is displayed in the address bar. By right clicking the path to edit address or clicking on ![search](icon/search-normal.svg), it will switch to directory/search bar.

By inputting a path or keyword in the bar and pressing ![enter](icon/Enter.svg), user can access the target directory or search for target items.

![0|addressbar1](jpg/addressbar1.png)

> ![tips](icon/tips.svg): When user accesses a sub directory, all the parent directories of the sub directory will become a breadcrumb in the address bar. By clicking on the breadcrumbs, user can easily switch among the sub directories and their parent directories.

### Search Items

1. Click the search button or press ![Ctrl](icon/Ctrl.svg) + ![F](icon/F.svg)  to switch to the search bar.
  - To search in a specific directory, please enter the directory and then search.
2. Input the keywords and press ![enter](icon/Enter.svg), the search results will be shown in the window.

> ![tips](icon/tips.svg): In **Settings**, check "Auto index internal disk" and "Index external storage device after connected to computer" to quicken the searching speed.

#### Advanced Search
1. In search bar, input the keywords and press enter, click ![filter](icon/filter.svg) to show advanced search.
2. Select the searching range, file type and size, and modified time to narrow the search results.

![0|filter](jpg/filter.jpg)

### Recent Files
By default, there is a **Recent** entry on left panel. Click it to view the recently used files. Files are sorted by default in reverse order of access time.

![recent](jpg/recent.jpg)

> ![notes](icon/notes.svg): You can hide it on left panel by unchecking "Display recent file entry in left panel" under "Hidden files" in Settings. To hide the access record of a specific file, right click on the file and select **Remove** which will not delete the file.

### Manage Tabs

Deepin File Manager supports multi-tab view.

1. On Deepin File Manager, right click on the folder to open in a new tab.
2. In the popup context menu, select **Open in new tab**.
3. When the window has multi-tabs, user can:
 - Click on + in tab bar to add a new tab.
 - Click on × on the right tab of the tab bar or middle-click to close an existing tab.

> ![tips](icon/tips.svg): When the Deepin File Manager has only one tab, the tab bar is hidden, you can press ![Ctrl](icon/Ctrl.svg) + ![T](icon/T.svg) to add a new tab.

![1|newtab](jpg/newtab.jpg)

### Manage Bookmarks

#### Add Bookmarks

Users can add folders to bookmarks in Deepin File Manager.

1. On Deepin File Manager, right click on the folder to add as bookmark.
2. In the popup context menu, select **Add to bookmark**.

![0|add_bookmark](jpg/add_bookmark.jpg)

#### Move Bookmarks

Users can move bookmarks within the bookmark area. Drag and drop the bookmark to the designated location.

#### Delete Bookmarks

Users can delete bookmarks by right clicking on the bookmark and select **Remove**, or right clicking on the bookmarked folder and select **Remove bookmark**, or drag and drop the bookmark out of the navigation bar to delete it.

### Manage Tags

By adding tags on file/folder, you can classify and manage your files better.

#### Add Tags

##### By Tag information

1. Right click on a file/folder, select **Tag information**.
2. Input tag info, and use comma to separate tags.
3. Press  ![enter](icon/Enter.svg) to finish adding tag.

> ![notes](icon/notes.svg): The tag color is randomly assigned from eight default colors.


![1|tag info](jpg/tag info.jpg)



![1|input tag](jpg/input tag.jpg)

##### By Color Tag

Right click on a file/folder, select a "color button" to create a color tag.

![1|color tag](jpg/color tag.jpg)

> ![notes](icon/notes.svg): You can add tag to multiple files/folders at once, or add multiple tags to a single file/folder; the tag appears in the left navigation bar.

> ![attention](icon/attention.svg): If a file has a tag, copy or cut and then paste it, the new file still has the same tag.

#### Rename Tag

Right click on a tag in  the left navigation bar,  select **Rename** to rename the tag.

![1|rename tag](jpg/rename tag.jpg)

#### Change Tag Color

Right click on a tag in  the left navigation bar,  select a "Color button" to change the tag color.

#### Change Tag Order

Drag and drop the tag upwards or downwards to change its order.

#### Delete Tag
Right click on a tag in left navigation bar,  select **Remove** to delete the tag.

### Preview Files

Deepin File Manager supports file preview. Select a file and press the space bar on the keyboard to quickly preview the file info, including image resolution, document size, text content, gif, audio and video playback, etc.

#### Preview Video

1. Select a video and press  the space bar to start video preview.
2. Click in the preview window to pause video playback.
3. Drag the progress bar to play forward or backward.
4. Click **Open** at the bottom to open the video by default program.

![1|video preview](jpg/video preview.jpg)

![notes](icon/notes.svg): When selecting multiple files to  preview, use ![previous](icon/previous.png) or ![next](icon/next.png) to jump to the previous or next file.

### Permission Management
In file/folder properties, click **Permission Management** to set permission for different roles.

![0|permission](jpg/permission.jpg)

### Computers in LAN

Users can visit computers in LAN to access files and folders.

1. On Deepin File Manager interface, click on ![network](icon/network_server.svg).
2. Double click on the computer icon to access.
3. Select to connect as **Anonymous ** or **Registered user**. For **Registered user**, user needs to input username and password to connect.
4. Click on **Connect** to establish connection.

![0|network](jpg/network.jpg)

### My Shares

#### Share Folders

Users can share folders in Deepin File Manager, if there are some folders shared, the share icon will show on the navigation bar, if not, the share icon will be hidden.

1. On Deepin File Manager interface, right click on a folder, select **Properties**.
2. In the popup window, click **Share Management**.
3. To share this folder, click **Share this folder **.
4. To change share options, input **Share name**, and select to choose **Permission** and **Anonymous **.
5. To set share password, click ![menu](icon/icon_menu.svg) and select **Set share password**.

![0|share](jpg/share.jpg)

#### Manage Share

Users can manage shared folders if it has shared some folders in Deepin File Manager.

1. On Deepin File Manager interface, click ![share](icon/icon_share.svg) on the navigation bar.
2. All shared folders will show on the interface.
3. Users can cancel sharing by right clicking the folder and select **Cancel sharing**.

![1|viewshare](jpg/viewshare.jpg)

### Open in Terminal

1. On Deepin File Manager interface, enter the folder.
2. Click on blank area in the folder, and select **Open in terminal**.

![0|openinterminal](jpg/openinterminal.jpg)

### Create Link

Users can create links for items, and put the links in easy-to-reach folders (such as Desktop) to access the items faster.

1. On Deepin File Manager interface, select the item to create a link for.
2. Right click and select **Create link**.
3. In the popup window, select a folder to store the link.
4. Click on **Save** to finish creating the link.

![0|create_link](jpg/create_link.jpg)

> ![tips](icon/tips.svg): By selecting **Send to desktop** for an item, user can create a link on desktop with one click.

### Send To Other Device

When there is an external device inserted, you can directly send file/folder to it.

1. Selected a file/folder.
2. Right click and select **Send to**.
3. Select the device you want to send to.
4. File/folder will be copied to the device.

![0|send_to](jpg/send_to.png)

## Disk Management

On Deepin File Manager interface, you can mount/unmount, remove, format and decrypt disks.

### Display Disks

Mounted disks are displayed in the left navigation bar of Deepin File Manager. Including:
- Local disk, which includes all the partitions of local disk.
- External disk, which includes mobile hard disk, CD/DVD and USB flash disk.
- Mobile device, which includes mobile phone storage, storage card, SD card and so on.

### Unmount/Eject/Safely Remove Disks

1. In the navigation bar of Deepin File Manager, right click on the disk.

2. For local disk, select **Unmount** to unmount the disk. The disk will be still in the disk list.

3. For external disk, select **Unmount** to unmount and eject the removable disk/device, the disk will be removed from the disk list.

4. Select **Safely Remove** to power off and remove the external device.

![0|disk](jpg/disk.jpg)

> ![tips](icon/tips.svg): Users can click on ![unmount](icon/unmount_normal.svg) in the navigation bar to unmount the disk.

### Rename Disk
Unmount the disk, right click on it and select **Rename** to change the disk label.

### Format Disks

1. In the navigation bar of Deepin File Manager, right click on the disk to format.
2. Select **Format**.
3. In the popup window, choose the type and volume label for the disk.
4. Click on **Format**.

> ![notes](icon/notes.svg): Fast format can be finished in a few seconds, but the data could be restored by file recovery tools. If you don't want these data to be restored, you can uncheck this option box and format the disk.


## Main Menu

Users can create new window, switch to dark theme, make settings, view manual and about information, exit the application.


### New Window

1. On Deepin File Manager interface, click on ![menu](icon/icon_menu.svg).
2. Select **New window** to open a new window.

### Dark Theme

1. On Deepin File Manager interface, click on ![main_menu](icon/icon_menu.svg).
2. Select **Dark Theme**.
3. The interface will switch to dark theme.

### Settings

1. On Deepin File Manager interface, click on ![menu](icon/icon_menu.svg).
2. Select **Settings** to enter the detailed settings interface.
3. Modify the settings according to your needs:
 - In **Open behavior**, users can set whether to open folder in current window or new window, one click or double click to open it.
 - In **New window and tab**, users can set the default path to open in a new window/tab.
 - In **View**, users can set file icon size and default view.
 - In **Hidden files**, users can set whether to show the hidden file, and hide file extension when rename.
 - In **Index**, users can set whether to auto index internal disk and connected external storage device.
 - In **Preview**, users can set whether to show the preview image of the specified file types.
 - In **Mount**, users can set to auto mount and open after auto mount.
 - In **Dialog**, users can set whether to use the file chooser dialog of the Deepin File Manager.
 - In **Other**, users can set whether to hide system disk.
 - Users can click on **Restore Defaults** to restore all settings.

![1|settings](jpg/settings.jpg)

### Help

1. On Deepin File Manager interface, click on ![menu](icon/icon_menu.svg).
2. Select **Help**.
3. View the manual.



### About

1. On Deepin File Manager interface, click on ![menu](icon/icon_menu.svg).
2. Select **About**.
3. View the version and description.



### Exit

1. On Deepin File Manager interface, click on ![menu](icon/icon_menu.svg).
2. Click on **Exit** to exit.
