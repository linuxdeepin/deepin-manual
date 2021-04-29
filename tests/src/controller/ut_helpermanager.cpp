/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wangmingliang <wangmingliang@uniontech.com>
* Maintainer: wangmingliang <wangmingliang@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ut_helpermanager.h"

#include "controller/helpermanager.h"
#include "src/third-party/stub/stub.h"
#include "base/utils.h"
#include "ut_filewatcher.h"
#include <QTimer>

ut_helperManager::ut_helperManager(QObject *parent) : QObject(parent)
{

}

ut_helperManager::~ut_helperManager()
{

}

void stub_initweb()
{
    return;
}

QDBusMessage::MessageType stub_ReplyMessage()
{
    return QDBusMessage::ReplyMessage;
}

QWebEnginePage* ut_helperManager::stub_page()
{
    return (QWebEnginePage*)this;
}

void ut_helperManager::stub_setWeb(QWebChannel * m_webchannel)
{
    webchannel = m_webchannel;
    return;
}

void ut_helperManager::SetUp()
{
    Stub s;
    s.set(ADDR(helperManager, initWeb), stub_initweb);
    m_hm = new helperManager();
}

void ut_helperManager::TearDown()
{
    delete m_hm;
}


TEST_F(ut_helperManager, initDbConfig)
{
    Stub s;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_helperManager, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_helperManager,stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), stub_initweb);
    m_hm->initWeb();
    m_hm->initDbConfig();
    delete webchannel;
}


TEST_F(ut_helperManager, getModuleInfo)
{
    QDir dir;
    dir.mkpath("./manual-assets/system/dde/dde/zh_CN/");
    dir.mkpath("./manual-assets/system/dde/dde11/zh_CN/");
    dir.mkpath("./manual-assets/application/deepin-voice-note/voice-note/zh_CN/");
    dir.mkpath("./manual-assets/professional/deepin-voice-note/zh_CN/");

    QFile file("./manual-assets/system/dde/dde/zh_CN/index.md");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    file.close();
    QFile file1("./manual-assets/application/deepin-voice-note/voice-note/zh_CN/index.md");
    file1.open(QIODevice::ReadWrite | QIODevice::Text);
    file1.close();
    QFile file2("./manual-assets/professional/deepin-voice-note/zh_CN/index.md");
    file2.open(QIODevice::ReadWrite | QIODevice::Text);
    file2.close();

    Utils ut;
    Stub s;
    s.set(ADDR(Utils, getSystemManualDir), ut_fileWatcher::stub_getSystemManualDir);

    m_hm->getModuleInfo();

    QDir dirRemove;
    dirRemove.rmpath("./manual-assets/");
}

TEST_F(ut_helperManager, initConnect)
{
    m_hm->initConnect();
}

TEST_F(ut_helperManager, handleDb)
{
    QStringList deleteList;
    QStringList addList;
    QStringList addTime;
    deleteList << "./manual-assets/application/deepin-terminal/zh_CN/index.md";
    addList << "/usr/share/deepin-manual/manual-assets/professional/deepin-terminal/zh_CN/index.md";
    addList << "/usr/share/deepin-manual/manual-assets/application/deepin-terminal/terminal/zh_CN/voice-note.md";
    addList << "/usr/share/deepin-manual/manual-assets/application/deepin-voice-note/voice-note/zh_CN/voice-note.md";
    addList << "/usr/share/deepin-manual/manual-assets/application/deepin-voice-note/voice-note/zh_CN/p_voice-note.md";
    addList << "/usr/share/deepin-manual/manual-assets/professional/deepin-terminal/zh_CN/index.md";

    addTime << "周二 7月 24 13:57:36 2018";
    addTime << "周二 7月 24 13:57:36 2018";
    addTime << "周二 7月 24 13:57:36 2018";
    addTime << "周二 7月 24 13:57:36 2018";
    addTime << "周二 7月 24 13:57:36 2018";
    Stub s;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_helperManager, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel),  ADDR(ut_helperManager,stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), stub_initweb);
    s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), stub_initweb);
    m_hm->initWeb();
    m_hm->handleDb(deleteList, addList, addTime);
    delete webchannel;

}

TEST_F(ut_helperManager, dbusSend)
{
    QStringList deleteList;
    QStringList addList;
    deleteList << "aaa";
    addList << "bbb";
    m_hm->dbusSend(deleteList, addList);

    Stub s;
    s.set((QDBusMessage::MessageType (QDBusMessage::*)())ADDR(QDBusMessage, type), stub_ReplyMessage);
    m_hm->dbusSend(deleteList, addList);
}

TEST_F(ut_helperManager, handlePriority)
{

    QStringList addList;
    addList << "/usr/share/deepin-manual/manual-assets/professional/deepin-terminal/zh_CN/index.md";
    addList << "/usr/share/deepin-manual/manual-assets/application/deepin-terminal/terminal/zh_CN/voice-note.md";
    addList << "/usr/share/deepin-manual/manual-assets/application/deepin-voice-note/voice-note/zh_CN/voice-note.md";
    addList << "/usr/share/deepin-manual/manual-assets/application/deepin-voice-note/voice-note/zh_CN/p_voice-note.md";
    addList << "/usr/share/deepin-manual/manual-assets/professional/deepin-terminal/zh_CN/index.md";
    m_hm->handlePriority(addList);
}

TEST_F(ut_helperManager, onFilelistChange)
{
    QStringList deleteList;
    QStringList addList;
    QStringList addTime;

    m_hm->onFilelistChange(deleteList, addList, addTime);
}

TEST_F(ut_helperManager, webLoadFinish)
{
    m_hm->webLoadFinish(true);
}

TEST_F(ut_helperManager, onRecvParseMsg)
{
    QString pathCN = "/usr/share/deepin-manual/manual-assets/professional/deepin-voice-note/zh_CN/index.md";
    QString pathUS = "/usr/share/deepin-manual/manual-assets/professional/deepin-voice-note/en_US/index.md";
    QString pathCA = "/usr/share/deepin-manual/manual-assets/professional/deepin-voice-note/ec_AS/index.md";
    QString msgCN = "[[\"h0\",\"语音记事本\",\"\"],[\"h1\",\"概述\",\"语音记事本是一款设计简洁、美观易用的集文本、录音来记事的软件。\"],[\"h2\",\"使用入门\",\"通过以下方式运行、关闭语音记事本，同时还可以创建快捷方式。\"],[\"h3\",\"运行语音记事本\",\"单击桌面底部的 <img src=\\\"../common/deepin_launcher.svg\\\" >，进入启动器界面。上下滚动鼠标滚轮浏览或通过搜索，找到语音记事本 <img src=\\\"../common/deepin_voice_note01.svg\\\" >单击运行。右键单击 <img src=\\\"../common/deepin_voice_note01.svg\\\" >，您可以：单击 发送到桌面，在桌面创建快捷方式。单击 发送到任务栏，将应用程序固定到任务栏。单击 开机自动启动，将应用程序添加到开机启动项，在电脑开机时自动运行该应用。<img src=\\\"../common/notes.svg\\\" >说明：如果语音记事本已经默认固定在任务栏上，您也可以点击任务栏上的 <img src=\\\"../common/deepin_voice_note01.svg\\\" >来运行。\"],[\"h4\",\"关闭语音记事本\",\"在语音记事本界面点击 <img src=\\\"../common/close_icon.svg\\\" >，退出语音记事本。在任务栏右键单击 <img src=\\\"../common/deepin_voice_note01.svg\\\" >，选择 关闭所有 来退出语音记事本。在语音记事本界面点击 <img src=\\\"../common/icon_menu.svg\\\" >，选择 退出 来退出语音记事本。\"],[\"h5\",\"查看快捷键\",\"在语音记事本界面上，按下键盘上的 Ctrl + Shift + ? 组合键来查看快捷键，熟练的使用快捷键，将大大提升您的操作效率。<img src=\\\"fig/hotkey.png\\\" >\"],[\"h6\",\"操作介绍\",\"\"],[\"h7\",\"新建记事本\",\"当语音记事本中尚无记事本时，单击 新建记事本，创建一个新的记事本。当第一次新建记事本后，系统默认帮您创建一个文本，此时在文本列表内单击 <img src=\\\"../common/circlebutton_add2.svg\\\" > 按钮可继续创建新文本。<img src=\\\"fig/create1.png\\\" >\"],[\"h8\",\"录制/回放音频\",\"在语音记事本界面上单击 <img src=\\\"../common/record_normal.svg\\\" >，开始录音。录音过程中您可以单击 <img src=\\\"../common/pause_red_normal.svg\\\" > 按钮暂停录音，再次单击 <img src=\\\"../common/record_normal.svg\\\" > 按钮继续录音。单击 <img src=\\\"../common/finish_normal.svg\\\" > 按钮完成录音。<img src=\\\"../common/notes.svg\\\" >说明：录音的限制时长为60分钟。录制完成后，单击 <img src=\\\"../common/play_blue_normal.svg\\\" > 播放按钮可以回放音频。<img src=\\\"fig/recorder1.png\\\" ><img src=\\\"fig/recorder2.png\\\" >\"],[\"h9\",\"搜索\",\"在语音记事本顶部搜索框中，您可以：单击 <img src=\\\"../common/search.svg\\\" >，输入关键字。按下键盘上的 Enter 键后快速定位。当搜索到匹配的信息时，在界面显示搜索结果列表。当没有搜索到匹配的信息时，在界面显示 “无搜索结果”。在搜索文本框中点击 <img src=\\\"../common/close_icon.svg\\\" > 清除当前输入的关键字或取消搜索。\"],[\"h10\",\"语音转文字\",\"在语音记事本界面上，选择一个录音文件。单击鼠标右键选择 语音转文字。<img src=\\\"fig/toVoice.png\\\" >\"],[\"h11\",\"保存语音\",\"在文本列表中，选择一个有录音文件的笔记。单击鼠标右键，选择 保存语音。将录音文件保存至储存设备中。<img src=\\\"../common/tips.svg\\\" >窍门：您也可以右键单击录音文件，选择 保存为MP3 将录音文件保存。\"],[\"h12\",\"添加文字笔记\",\"在语音记事本界面上单击 <img src=\\\"../common/circlebutton_add2.svg\\\" > 按钮，添加新的文本笔记。将光标移入右侧详情页内，单击鼠标左键后输入文字笔记。<img src=\\\"fig/txtinput.png\\\" >\"],[\"h13\",\"保存为TXT文件\",\"在语音记事本界面上，选择一个文字笔记。单击鼠标右键选择 保存为TXT。将文字笔记保存至储存设备中。<img src=\\\"fig/toTXT.png\\\" >\"],[\"h14\",\"语音朗读\",\"在“控制中心” > “辅助功能”界面，开启 语音朗读。在语音记事本界面选中一段文字，单击右键选择 语音朗读，语音播报选中的文字。在语音朗读模式下，单击右键选择 停止朗读 或者单击左键停止朗读。<img src=\\\"../common/notes.svg\\\" >说明：当未选中文字时，语音朗读呈置灰状态，不可操作。当只选中录音文件时，语音朗读不可操作；若选中的录音文件包含语音转文字内容，可进行语音朗读的操作。<img src=\\\"fig/speech.png\\\" >\"],[\"h15\",\"语音听写\",\"在“控制中心” > “辅助功能”界面，开启 语音听写 并选择语言。在语音记事本文本输入框中，单击鼠标右键选择 语音听写，此时您可通过麦克风输入语音，转译文字将被编辑在文本输入框内。在听写模式下点击界面空白区域停止语音听写。<img src=\\\"../common/tips.svg\\\" >窍门：录制音频时可同时开启语音听写的功能。<img src=\\\"fig/listen.png\\\" >\"],[\"h16\",\"文本翻译\",\"在“控制中心” > “辅助功能”界面，开启 文本翻译 并选择语言“中译英”或者“英译中”。在语音记事本界面选中一段文字，单击鼠标右键选择 文本翻译，选中的文字将被翻译成目标语言。<img src=\\\"../common/notes.svg\\\" >说明：当只选中录音文件时，文本翻译不可操作；若选中的录音文件包含语音转文字内容，可进行文本翻译的操作。<img src=\\\"fig/translate.png\\\" >\"],[\"h17\",\"重命名记事本\",\"记事文件完成后，文件将自动保存名为“记事本（n）”，其中n≥0，n为整数，您可以重命名记事本。在语音记事本界面上，右键单击一个新记事本。选择 重命名。输入文件名称。鼠标点击界面空白处或者按下键盘上的 Enter 键完成重命名。<img src=\\\"../common/tips.svg\\\" >窍门: 您也可以对记事本下的各个文本笔记进行重命名。<img src=\\\"fig/rename.png\\\" >\"],[\"h18\",\"删除录音\",\"在语音记事本界面上，选择一个录音文件。单击鼠标右键选择 删除。在弹出的对话框中，单击 确定。\"],[\"h19\",\"删除文字笔记\",\"在语音记事本界面上，选择一个文字笔记。单击鼠标右键选择 删除。在弹出的对话框中，单击 确定。\"],[\"h20\",\"删除记事本\",\"在语音记事本界面上，选择一个记事本。单击鼠标右键选择 删除。在弹出的对话框中，单击 确定。\"],[\"h21\",\"主菜单\",\"在主菜单中，您可以设置声音来源、查看隐私政策、切换窗口主题、查看帮助手册，了解语音记事本的更多信息。\"],[\"h22\",\"设置\",\"您可对声音来源进行设置，包含系统声音和麦克风声音。在语音记事本界面，单击 <img src=\\\"../common/icon_menu.svg\\\" >。设置声音来源：选择 系统声音，录音时只录制系统内置声音。选择 麦克风声音，录音时只录制麦克风声音。\"],[\"h23\",\"主题\",\"窗口主题包含浅色主题、深色主题和跟随系统主题。在语音记事本界面，单击 <img src=\\\"../common/icon_menu.svg\\\" >。选择 主题，选择一个主题颜色。\"],[\"h24\",\"帮助\",\"查看帮助手册，通过帮助进一步让您了解和使用语音记事本。在语音记事本界面，单击 <img src=\\\"../common/icon_menu.svg\\\" >。选择 帮助。查看关于语音记事本的帮助手册。\"],[\"h25\",\"关于\",\"在语音记事本界面，单击 <img src=\\\"../common/icon_menu.svg\\\" >。选择 关于。查看关于语音记事本的版本和介绍。\"],[\"h26\",\"退出\",\"在语音记事本界面，单击 <img src=\\\"../common/icon_menu.svg\\\" >。选择 退出。\"]]";
    QString msgUS = "[[\"h0\",\"Voice Notes\",\"\"],[\"h1\",\"Overview\",\"Voice Notes is a simple memo software with texts and voice recordings. You are able to save the recordings as MP3 format or texts. <img src=\\\"fig/main.png\\\" >\"],[\"h2\",\"Guide\",\"You can run, exit, and create a shortcut for Voice Notes in the following ways.\"],[\"h3\",\"Run Voice Notes\",\"Click <img src=\\\"../common/deepin_launcher.svg\\\" > in the dock to enter the Launcher interface.Locate <img src=\\\"../common/deepin_voice_note.svg\\\" > by scrolling the mouse wheel or searching \\\"voice notes\\\" in the Launcher interface, and click it to run.Right-click <img src=\\\"../common/deepin_voice_note.svg\\\" >, and you can:Click Send to desktop to create a desktop shortcut.Click Send to dock to fix it in the Dock.Click Add to startup to add the application to startup, and it will automatically run when the system starts up.<img src=\\\"../common/notes.svg\\\" > Notes: If Voice Notes has been fixed in the dock by default, you can click <img src=\\\"../common/deepin_voice_note.svg\\\" > in the dock to run it.\"],[\"h4\",\"Exit Voice Notes\",\"Click <img src=\\\"../common/close_icon.svg\\\" > on the Voice Notes interface to exit Voice Notes.Right-click <img src=\\\"../common/deepin_voice_note.svg\\\" > in the dock, and select Close All to exit.Click <img src=\\\"../common/icon_menu.svg\\\" > on the Voice Notes interface, and select Exit to exit.\"],[\"h5\",\"View Shortcuts\",\"On the Voice Notes interface, press Ctrl + Shift + ? on the keyboard to view shortcuts. Proficiency in shortcuts will greatly improve your efficiency.<img src=\\\"fig/hotkey.png\\\" >\"],[\"h6\",\"Operations\",\"\"],[\"h7\",\"Create Notebooks\",\"Click Create Notebook to create a new notebook when there is none in Voice Notes.<img src=\\\"fig/create.png\\\" >             When a notebook is created in Voice Note, a text is also created by default. Click the <img src=\\\"../common/circlebutton_add2.svg\\\" > icon on the Voice Notes interface to create more new texts.You can start recording voices or writing texts after creating a text.The default naming rule for Voice Notes is Notebook(n), in which n is an integer no less than 0.<img src=\\\"fig/create1.png\\\" >\"],[\"h8\",\"Record Voice Notes\",\"You can record audios simply in two steps:Click the <img src=\\\"../common/record_normal.svg\\\" > icon on the Voice Notes interface to start recording.Click the <img src=\\\"../common/finish_normal.svg\\\" > icon to finish recording.<img src=\\\"../common/tips.svg\\\" > Tips: Click the <img src=\\\"../common/pause_red_normal.svg\\\" > icon to pause recording during the recording process, and click <img src=\\\"../common/record_normal.svg\\\" > again to resume recording.<img src=\\\"../common/notes.svg\\\" > Notes: The maximum recording time is 60 minutes. Voice notes can only be created in the text notes. <img src=\\\"fig/recorder2.png\\\" >\"],[\"h9\",\"Playback Voice Notes\",\"The recorded voice files are displayed on the Voice Notes interface in a list. Select a voice file, and click the<img src=\\\"../common/play_blue_normal.svg\\\" > icon to listen to the playback.<img src=\\\"../common/tips.svg\\\" > Tips: Click the <img src=\\\"../common/pause_blue_normal.svg\\\" > icon to pause or stop playback, and click <img src=\\\"../common/play_blue_normal.svg\\\" > again to resume playback when you are listening to the recording.<img src=\\\"fig/replay.png\\\" >\"],[\"h10\",\"Search Voice Notes\",\"In the search box located in the top center area of the Voice Notes interface, you can click <img src=\\\"../common/search.svg\\\" > to enter keywords.Press the Enter key on the keyboard to fast locate it.When there is matched information, the search result list will display on the right.When there is no matched information, No search results will display on the right. Click <img src=\\\"../common/close_icon.svg\\\" > in the search box  to clear the current keywords or cancel the search. \"],[\"h11\",\"Convert Voices to Texts\",\"Select a voice file on the Voice Notes interface.Right-click, and select Voice to Text.<img src=\\\"fig/toVoice.png\\\" >\"],[\"h12\",\"Save Voices as MP3 Format\",\"  Select a voice file on the Voice Notes interface.  Right-click, and select Save as MP3.  Save it to the storage device.\"],[\"h13\",\"Add Text Notes\",\"Select a notebook, right-click it, and select New note; or click the<img src=\\\"../common/circlebutton_add2.svg\\\" > icon on the Voice Notes interface to add a new text note. Move the cursor to the right column to click and input text. <img src=\\\"../common/tips.svg\\\" > Tips: You can record audios in notebooks. Please refer to [Record Voice Notes](#Record Voice Notes) for details.<img src=\\\"fig/main.png\\\" >\"],[\"h14\",\"Save as TXT\",\" Select a text note on the Voice Notes interface. Right-click, and select Save as TXT. Save the text note to the storage device in TXT format.<img src=\\\"fig/toTxT.png\\\" >\"],[\"h15\",\"Text to Speech\",\"Go to Control Center > Assistive Tools, and enable Text to Speech.Select a paragraph of text on the Voice Notes interface, and right-click to select Text to Speech. The text selected will be converted to speech and read accordingly. In the text to speech mode, right-click to select Stop reading or left-click to stop reading.<img src=\\\"../common/notes.svg\\\" > Notes:When no text is selected, Text to Speech is greyed out and inoperable. When only an audio file is selected, Text to Speech is inoperable; if the audio file selected contains voice-to-text content, Text to Speech can be operated.<img src=\\\"fig/speech.png\\\" >\"],[\"h16\",\"Speech to Text\",\"Go to Control Center > Assistive Tools. Enable Speech to Text, and select Language.In the text input box on the Voice Notes interface, right-click and select Speech to Text. Now you can input audios via microphone, and the translated text will be edited and displayed in the text input box. In the speech to text mode, click the the blank area on the Voice Notes interface to stop text editing and display. <img src=\\\"../common/tips.svg\\\" > Tips: You can enable Speech to Text while recording audios. <img src=\\\"fig/listen.png\\\" >\"],[\"h17\",\"Translate Text Notes\",\"Go to Control Center > Assistive Tools. Enable Translation, and select the Language option from en-cn or cn-en.Select a paragraph of text on the Voice Notes interface, and right-click to select Translate. The text selected will be translated to target language.<img src=\\\"../common/notes.svg\\\" > Notes: When only an audio file is selected, Translate is inoperable; if the audio file selected contains voice-to-text content, Translate can be operated.<img src=\\\"fig/translate.png\\\" >\"],[\"h18\",\"Rename Notebooks\",\"The notebook created will be automatically saved as Note (n). You can rename it in the following steps. On the Voice Notes interface, right-click a notebook.Select Rename.Enter the file name.Click in the blank area on the interface, or press the Enter key on the keyboard to rename.<img src=\\\"../common/tips.svg\\\" > Tips: You can also rename each text note under a notebook. <img src=\\\"fig/rename.png\\\" >\"],[\"h19\",\"Delete Voice Notes\",\" Select a voice file on the Voice Notes interface. Right-click, and select Delete. Click Confirm in the pop-up prompt box.\"],[\"h20\",\"Delete Text Notes\",\"  Select a text note on the Voice Notes interface. Right-click, and select Delete. Click Confirm in the pop-up prompt box.\"],[\"h21\",\"Delete Notebooks\",\"Select a notebook on the Voice Notes interface.Right-click, and select Delete.Click Confirm in the pop-up prompt box.\"],[\"h22\",\"Main Menu\",\"On the main menu, you can set audio source, check privacy policy, switch window themes, view help manual, and get more information about Voice Notes.\"],[\"h23\",\"Settings\",\"You can set up audio source from Internal and Microphone (default) options.On the Voice Notes interface, click <img src=\\\"../common/icon_menu.svg\\\" >.Set up audio source:Select Internal source, and only system built-in sound will be recorded. Select Microphone source, and only microphone sound will be recorded. \"],[\"h24\",\"Theme\",\"The window theme provides three theme types, namely Light Theme, Dark Theme, and System Theme.On the Voice Notes interface, click <img src=\\\"../common/icon_menu.svg\\\" >.Click Theme to select one theme.\"],[\"h25\",\"Help\",\"On the Voice Notes interface, click <img src=\\\"../common/icon_menu.svg\\\" >.Click Help to view the manual of Voice Notes.\"],[\"h26\",\"About\",\"On the Voice Notes interface, click <img src=\\\"../common/icon_menu.svg\\\" >. Click About to view the version and introduction of Voice Notes.\"],[\"h27\",\"Exit\",\"On the Voice Notes interface, click <img src=\\\"../common/icon_menu.svg\\\" >.Click Exit to exit Voice Notes.\"]]";


    m_hm->onRecvParseMsg(msgUS, pathUS);
    m_hm->onRecvParseMsg(msgCN, pathCN);
    m_hm->onRecvParseMsg(msgCN, pathCA);
}

TEST_F(ut_helperManager, onTimerOut)
{
    m_hm->onTimerOut();
}

