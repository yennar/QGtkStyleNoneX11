QGtkStyleNoneX11
================

**Enable your Qt Windows applications with fancy QGtkStyle by just a few copy-and-paste!**
**DO NOT** have to recompile anything! 

Examples:

Qt-Designer rendered with fancy Nodoka-Midnight
![qtdesignerwithnodokamidnight](https://raw.githubusercontent.com/yennar/QGtkStyleNoneX11/master/web/images/qt_designer_with_nodoka_midnight.png)

WingIDE, a famous Python IDE developed by PyQt
![wingidewithmurrinachrome](https://raw.githubusercontent.com/yennar/QGtkStyleNoneX11/master/web/images/wingide_with_murrina_chrome.png)



 Install
---------

1. Unzip zip/GTK_2_10_WIN32.zip, add its /bin to `%PATH`
1. Unzip zip/qt-opensource-windows-[arch]-[vcver]-[qtver].zip, overwrite your existed QtCoreX.dll and QtGuiX.dll with the ones in the zipball

 Enable Gtk Style
------------------

#### For applications either developed by you or not, use one of these:

* launch the application with -style GTK+
* set `%QT_GTKSTYLE = 1` and select a style by gtk2_prefs.exe, see http://sourceforge.net/projects/gtk-win/files/GTK%2B%20Preference%20Tool/0.4.1/
* set `%GTK2_RC_FILES` to one of the themes in GTK directory

#### You may a qt developer and may want select GTK theme inside your code:

* See new example: style_gtk


 Development of This project
-----------------------------
Required by LGPL v2.0, the source code of hacked qt-gtkstyle are also provided. Suggest re-compiling:

* Download offical pre-compiled package.
* Copy the source code from this project and overwrite offical source.

#### For Qt4

* `configure -fast -release -no-qmake -nomake demos -nomake examples`
* cd /tools/configure and make/nmake
* configure again
* make/nmake





