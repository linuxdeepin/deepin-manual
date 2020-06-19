# Text Editor|../common/deepin-editor.svg|

## Overview

Text Editor is a simple text editing tool. You can use it to write a simple text document, or use its advanced features as a code editing tool that supports code syntax highlighting.

![overview](jpg/overview.png)

## Guide

### Run Text Editor

Click ![deepin-launcher](icon/deepin-launcher.svg)on dock to enter the Launcher interface.  Locate Text Editor ![deepin-editor](icon/deepin-editor.svg)

by  scrolling the mouse wheel or searching "text editor" in the Launcher interface and click it to run.

In Launcher, right-click Text Editor and you can:

  - Click **Send to desktop** to create a desktop shortcut.
  - Click **Send to dock** to fix its icon on Dock.
  - Click **Add to startup** to add it to startup and it will auto run when system boots.

### Exit Text Editor

- On the Text Editor interface, click![close](icon/close_icon.svg) to exit.
- On the Text Editor interface, click![icon_menu](icon/icon_menu.svg) > **Exit** to exit.
- Right-click ![deepin-editor](icon/deepin-editor.svg) on Dock and select **Close All** to exit.

### View Shortcuts

On the Text Editor Interface, press **Ctrl** + **Shift** + **?** to view all the shortcuts. You can get them in Settings as well. Using shortcuts will greatly improve your work efficiency.

![1|hotkey](jpg/hotkey.png)

## Basic Operations

### Create Document

Click ![plus_icon](icon/plus_icon.svg) on the title bar, or press **Ctrl** + **N**, or select **New window** and **New tab** in the main menu to create a blank document in Text Editor.

### Open Files

You can open the text file in the following ways, and the selected file will open in the new tab:

- Drag the file directly to the Text Editor interface or its icon.
- Right-click the file and open it with Text Editor. After selecting Text Editor as the default program, you can double-click the file to open it directly.
- On the Text Editor interface, click ![icon_menu](icon/icon_menu.svg) > **Open file** and select the file.
- On the Text Editor interface, use **Ctrl** + **O** to open the file.

> ![tips](icon/tips.svg)Tips: You can open multiple files at one time.

### Save Files

- Press **Ctrl** + **S** to save the current document.
- Click ![icon_menu](icon/icon_menu.svg) > **Save** to save the file.

You can also press **Ctrl** + **Shift** + **S** to save the file as  a new one.

### Close Files

- Press **Ctrl** + **W** to close the file.
- Move the cursor to the title and click the **X** button there or the mouse middle button to close the file.
- Right-click the title and select **Close tab** or **Close other tabs**.
- Right-click the title and select **More ways to close**.
- Select **Close left tabs** to close all the tabs on the left of the current tab.
- Select **Close right tabs** to close all the tabs on the right of the current tab.
- Select **Close unchanged tabs** to close all the unchanged tabs.

> ![notes](icon/notes.svg)Notes: If you make changes to the file but have not saved it, Text Editor will prompt you to save it before closing.

### Print Documents

To print a file in Text Editor, you should connect and configure the printer first.

1. Click ![icon_menu](icon/icon_menu.svg) > **Print**, or press **Ctrl** + **P** to open the print preview interface.

2.  On the preview interface, preview the document and set the printing page. 

3.  Click the printer icon on the right of preview window. In the pop-up window, select the printer and set the parameters. 

4.  Click **Print** to start printing.

![preview](jpg/preview.jpg)
![pagesetup](jpg/pagesetup.jpg)
![printer](jpg/printer.jpg)

### Manage Tabs

In Text Editor, you can drag to reorder tabs within the same window, remove the tab out to create a new window, or move the tab from one window to another.

#### Tab Shortcuts

| Function   |  Shortcuts |
| ---------------------------------- | ------------ |
| New tab   |  Ctrl + T |
| Next tab | Ctrl + Tab |
| Previous tab  | Ctrl + Shift + Tab |
| Close tab | Ctrl + W |
| Close other tabs | Ctrl + Shift + W |
| Restore tab (Reopen closed tab) | Ctrl + Shift + T |

## Edit Texts

### Move Cursor

In addition to using the arrow keys and mouse clicks, you can also use the following shortcuts to move the cursor quickly:

| Function   |  Shortcuts |
| --------------- | ------------ |
| Save cursor position | Ctrl + Shift + > |
| Reset cursor position (Jump to cursor last saving position) | Ctrl + Shift + < |
| Forward word (Jump forward over a word) | Ctrl + Right |
| Backward word (Jump backward over a word) | Ctrl + Left |
| Move to end of line | End |
| Move to start of line | Home |
| Move to end of text | Ctrl + End |
| Move to start of text | Ctrl + Home |
| Move to line indentation | Ctrl + M |
| Forward over a pair (Jump forward after the right parenthesis) | Alt + P |
| Backward over a pair (Jump backward before the left parenthesis) | Alt + N |

### Change Cases

It helps you change the case of selected text. You can change it to lowercase, uppercase, or capitalize the initial.

1. Select the text.
2. Right-click and select **Change Case**.
3. Choose **Upper Case**, **Lower Case**, or **Capitalize**. The changes will take effect immediately.

You can also use **Alt** + **U** / **L** / **C** to quickly switch the cases.

### Highlight Texts
Text Editor supports highlighting different kinds of texts.

Click the triangle symbol at the far right corner of the status bar at the bottom, and select the type of text you want to highlight. The corresponding contents in the text will be highlighted automatically.

![highlight](jpg/highlight.png)

### Delete Texts

In addition to deleting characters one by one, you can quickly delete characters by using the following shortcuts:

| Function   |  Shortcuts |
| --------------- | ------------ |
| Delete to end of line | Ctrl + K |
| Delete current line | Ctrl + Shift + K |
| Delete backward word | Alt + Shift + N |
| Delete forward word | Alt + Shift + M |

### Undo

If you make a misoperation, press **Ctrl** + **Z** to undo it, or right-click to select **Undo**.

### Find Texts

1. Click ![menu](icon/icon_menu.svg), and select **Find**, or press **Ctrl** + **F** to open the "Find" box below.
2. Input the text you want to find. 
3. Click **Next**, **Previous** to find each match, or press **Enter** to find the next match.
4. Press **Esc** or click the close button to close the dialog box.

> ![tips](icon/tips.svg)Tips: Select the text and then press **Ctrl** + **F** and the text will be automatically displayed in the "Find" box.

### Replace Texts
1. Click ![menu](icon/icon_menu.svg), and select **Replace**, or press **Ctrl** + **H** to open the "Replace" box below.
2.  Input the text to be replaced and the new text.
3.  Click **Replace** to replace the matches one by one and click **Replace Rest** and **Replace All** to replace the rest or all the matches at once. Click **Skip** to skip the current matching text.
4.  Press **Esc** or click close button to close the dialog box.

### Go to Line
Use Go to Line to jump to the specific line directly.
Right-click and select **Go to Line**, or press **Ctrl** + **G**, and then input the line number to go to that line.

### Edit Line

Use the shortcuts below to edit lines easily:

| Function  |  Shortcuts |
| ------------ | ------------ |
| New line above (Insert one line above) | Ctrl + Enter |
| New line below (Insert one line below) | Ctrl + Shift + Enter |
| Duplicate line | Ctrl + Shift + D |
| Swap line up (Swap the current line with above line) | Ctrl + Shift + Up |
| Swap line down (Swap the current line with below line)  | Ctrl + Shift + Down |
| Scroll up one line | Super + Shift + Up |
| Scroll down one line | Super + Shift + Down |
| Mark | Alt + H |
| Unmark | Alt + Shift + H |
| Copy line | Super + C |
| Cut line | Super + X |
| Merge lines | Ctrl + J |

### Enable/Disable Read-only Mode

1. Open the document with Text Editor and right-click to select **Turn on Read-only Mode**.
2.  Right-click to select **Turn off Read-only Mode** under the read-only mode.

### Text to Speech  

You can perform the following operations after enabling the Text to Speech and Speech to Text features in Control Center.

1. Right-click to select Speech to Text and the input voices through microphone will be converted to texts and displayed in the document being edited.
2. Select a section of texts, right-click to select Text to Speech and the texts selected will be broadcast.

> ![notes](icon/notes.svg)Notes: The **Text to Speech** option in the right-click menu is grayed out and cannot be activated when no text is selected.

### Translate

Enable **Translation** in Control Center > Assistive Tools and set the corresponding language according to your needs. Select the text to be translated in Text Editor, right-click to select **Translate** and the text selected will be translated into the target language.

![translation](jpg/translation.png)

### Color Mark

Select the text to be marked in Text Editor, right-click to select **Color Mark** and select the corresponding options in the drop-down list as needed.

 ![colormark](jpg/colormark.png)

- Select **Mark Current Line** to mark the current line. There are four styles, with each representing a color.

- Select **Cancel Last Mark** to cancel the last mark operation.

- Select **Cancel Mark All Line** to cancel all the current marks.

- Select **Mark All Line** to mark all the contents in Text Editor. There are four styles, with each representing a color.


### Bookmark Management

It is available to click any line on the Text Editor interface to add a bookmark for the current line.

- Add bookmark

   + Click any line on the left column to add a bookmark for the line.
   + Right-click any line on the left column and select **Add bookmark** to  add a bookmark for the line.
   + Place the cursor on any line and press **Ctrl** + **F2** to add a bookmark for the line.

-   Delete bookmark  

   + Click the bookmark icon to delete the bookmark of the line directly.
   + Right-click the bookmark to select **Cancel bookmark** to delete the bookmark in the line.
   + Right-click a bookmark to select **Clear bookmark** to delete all the bookmarks in the text.

### Comment Management

You can add comments to the text in code format on the Text  Editor interface.

- Add comment
  - Select the text needed and right-click to select **Add comment**.
  - Select the text needed and press **Alt** + **A**.

- Cancel comment
     - Select the text needed and right-click to select **Cancel comment**.

     - Select the text needed and press **Alt** + **Z**.


> ![notes](icon/notes.svg)Notes: It supports comments in different languages, mainly based on the actual situation. For example, C, C# and Java comment symbols are // and /* */ and Python comment symbols are #.

## Main Menu

On the main menu, you can create new windows and new tabs, search the contents to be replaced, save documents, switch window themes, view help manual and get more information about Text Editor.

### Settings

It is available to set the basic information, shortcuts and advanced information in settings.

#### Basic Settings

Click ![icon_menu](icon/icon_menu.svg) > **Settings** and you can perform the following in the Settings window:

- Select the Font and Font Size.
- Check or uncheck  **Word Wrap**.
- Check **Code Fold Flag** and ![next](icon/next.svg)or![next](icon/next-down.svg) are displayed in the edit area. Right-click to select Fold/Unfold Current Level or Fold/Unfold All Levels to perform the corresponding operations.
- Check **Show Line Number** to display the line number in the edit area.

> ![tips](icon/tips.svg)Tips: You can also use **Ctrl** + ![+](icon/+.svg)/ ![-](icon/-.svg) to adjust the font size, and press **Ctrl** + **0** to reset font size.

#### Shortcuts Settings
1. Click ![icon_menu](icon/icon_menu.svg) > **Settings** to view the current shortcut in the **Shortcuts** option.
2. Select a keymap in **Shortcuts** and view the shortcuts.
3. You can click and press new shortcuts to customize it.

> ![notes](icon/notes.svg)Notes: Choose a proper keymap to fit your habits, including standard keymap, Emacs keymap, or customize it as you like.

#### Advanced Settings

1. Click ![icon_menu](icon/icon_menu.svg) > **Settings**.
2. In the **Advanced** option, you can set:
   - Window size: Normal, Maximum, or Fullscreen;
   - Tab space: The indentation width when pressing Tab.

> ![attention](icon/attention.svg)Attention: Clicking  **Restore Defaults** will restore all settings to the default.

### Theme

The window theme includes Light Theme, Dark Theme and System Theme (default).

1. On the Text Editor interface, click![icon_menu](icon/icon_menu.svg).

2.  Click **Theme** to select a theme.

### Help

1.  On the Text Editor interface, click ![menu](icon/icon_menu.svg).
2.  Select **Help**.
3.  View the manual.

### About

1. On the Text Editor interface, click  ![menu](icon/icon_menu.svg).
2.  Select **About**.
3.  View the version and description.

### Exit

1. On the Text Editor interface, click ![menu](icon/icon_menu.svg).
2. Click **Exit** to exit.  
