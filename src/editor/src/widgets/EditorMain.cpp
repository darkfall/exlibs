// ======================================================================================
// File         : EditorMain.cpp
// Author       : Wu Jie 
// Last Change  : 07/26/2009 | 09:47:07 AM | Sunday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "editor.h"
#include "EditorMain.h"
#include "ExternalDialog.h"
// TODO: #include "View3D.h"

///////////////////////////////////////////////////////////////////////////////
// global 
///////////////////////////////////////////////////////////////////////////////

static const quint16 ver_major = 1;
static const quint16 ver_minor = 0;
static const quint16 ver_patch = 0;

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EditorMain::EditorMain ( QWidget* _pParent )
    : QMainWindow (_pParent)
    , m_labelFps (NULL)
    , m_tickTimer (NULL)
    , m_view3D (NULL)
{
    setupUi(this);

    // TODO: shoule be changeStyle, changePalette { 
    // setup style
    if ( QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA ) {
        QApplication::setStyle(QStyleFactory::create("WindowsVista"));
    }
    else {
        QApplication::setStyle(QStyleFactory::create("Plastique"));
        // QApplication::setStyle(QStyleFactory::create("WindowsVista"));
        // QApplication::setStyle(QStyleFactory::create("Cleanlooks"));
    }

    // QPalette originalPalette = QApplication::palette(); // TODO: should save this.
    // QApplication::setPalette(QApplication::style()->standardPalette());
    // QApplication::setPalette(originalPalette);
    // } TODO end 

    resize ( 1024, 768 );
    updateTitle();
    updateMenu();

    // ======================================================== 
    // create signal slots
    // ======================================================== 

    m_tickTimer = new QTimer (this);
    connect ( m_tickTimer, SIGNAL(timeout()), this, SLOT(tick()) );
    connect ( action_TextEditor, SIGNAL(triggered()), this, SLOT(setupTextEditor()) );
    connect ( action_ImageEditor, SIGNAL(triggered()), this, SLOT(setupImageEditor()) );

    connect ( elementWidget, SIGNAL(currentItemChanged( QTreeWidgetItem*, QTreeWidgetItem* )), propertyWidget, SLOT(showProperty(QTreeWidgetItem*)) );
    connect ( elementWidget, SIGNAL(currentItemReseted( QTreeWidgetItem* )), propertyWidget, SLOT(showProperty(QTreeWidgetItem*)) );
    connect ( elementWidget, SIGNAL(currentItemSaved( QTreeWidgetItem* )), propertyWidget, SLOT(showProperty(QTreeWidgetItem*)) );
    connect ( propertyWidget, SIGNAL(valueChanged( QTreeWidgetItem* )), elementWidget, SLOT(updateItem(QTreeWidgetItem*)) );

    // ======================================================== 
    // setup status bar
    // ======================================================== 

    m_labelFps = new QLabel(this);
    m_labelFps->setObjectName(tr("m_labelFps"));
    QFont font;
    font.setPointSize(10);
    m_labelFps->setFont(font);
    m_labelFps->setText( tr("fps: ") );
    m_labelFps->setStyleSheet(tr("QLabel {\n"
                               " color: green;\n"
                               " padding: 1px\n"
                               " }"));
    this->statusBar()->addWidget(m_labelFps,1);

    // ======================================================== 
    // setup view menu 
    // ======================================================== 

    // add dock widgets to view menu 
    QList<QDockWidget *> dockwidgets = qFindChildren<QDockWidget *>(this);
    if ( dockwidgets.size() ) 
    {
        for ( int i = 0; i < dockwidgets.size(); ++i ) 
        {
            QDockWidget *dockWidget = dockwidgets.at(i);
            if (dockWidget->parentWidget() == this )
            {
                menu_View->addAction(dockwidgets.at(i)->toggleViewAction());
            }
        }
        menu_View->addSeparator();
    }
    // add toolbars to view menu 
    QList<QToolBar *> toolbars = qFindChildren<QToolBar *>(this);
    if ( toolbars.size() ) 
    {
        for ( int i = 0; i < toolbars.size(); ++i ) 
        {
            QToolBar *toolBar = toolbars.at(i);
            if ( toolBar->parentWidget() == this ) 
            {
                menu_View->addAction(toolbars.at(i)->toggleViewAction());
            }
        }
    }

    // ======================================================== 
    // setup default externals 
    // ======================================================== 

    ExternalTool tool;
    m_externals["text"] = ExternalTool("gvim.exe","$1");
    m_externals["image"] = ExternalTool("gimp.exe","$1");

    // HARDCODE { 
    // this->tabifyDockWidget( this->previewDock, this->propertyDock );
    this->setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
    this->setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
    this->setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea );
    this->setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );
    // } HARDCODE end 

    // KEEPME: this is the way to get widget from dock window. { 
    // COutputWidget* pOutput = qobject_cast<COutputWidget*>( OutputDock->widget() );
    // ex_assert ( pOutput, "OutputWidget not in OutputDock window" );
    // } KEEPME end 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EditorMain::~EditorMain ()
{
    m_tickTimer->stop();
}

// ------------------------------------------------------------------ 
// Desc: 
extern void editor_RegisterClasses ();
extern void editor_RegisterEnums ();
// ------------------------------------------------------------------ 

bool EditorMain::init ()
{
    ex_check_return ( resourceWidget->init(), false, "failed to init resource widget" );
    ex_check_return ( elementWidget->init(), false, "failed to init element widget" );
    ex_check_return ( propertyWidget->init(), false, "failed to init property widget" );

    editor_RegisterClasses();
    editor_RegisterEnums();

    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::deinit ()
{
    resourceWidget->deinit();
    elementWidget->deinit();
    propertyWidget->deinit();

    return;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::explore ( const QString& _path )
{
    QProcess* pProcess = new QProcess(this);
    QStringList args; 
    args << _path;
    pProcess->start( "explorer", args );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::editText ( const QString& _file )
{
    if ( m_externals["text"].exec(_file) == false ) {
        setupTextEditor ();
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::editImage ( const QString& _file )
{
    if ( m_externals["image"].exec(_file) == false ) {
        setupImageEditor ();
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::startTick ()
{
    m_tickTimer->start(20); // NOTE: set tick to 0 will make gui hard to operate.
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::stopTick ()
{
    m_tickTimer->stop ();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::saveSettings ()
{
    QSettings settings( "exDev", "exEditor" );
    settings.setValue( "geometry", saveGeometry() );
    settings.setValue( "windowState", saveState() );
    settings.setValue( "editor", saveEditor() );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::readSettings ()
{
    QSettings settings("exDev", "exEditor");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    restoreEditor(settings.value("editor").toByteArray());
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

QByteArray EditorMain::saveEditor ()
{
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_0);

    //
    const quint32 magicNumber = 0x2D9D0CB;
    stream << magicNumber
           << ver_major
           << ver_minor
           << ver_patch
           << m_lastEditPath
           << m_externals
           ;
    return array;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool EditorMain::restoreEditor ( const QByteArray& _info )
{
    if (_info.size() < 4)
        return false;
    QDataStream stream(_info);
    stream.setVersion(QDataStream::Qt_4_0);

    // check if magic number is same
    const quint32 magicNumber = 0x2D9D0CB;
    quint32 storedMagicNumber;
    stream >> storedMagicNumber;
    if (storedMagicNumber != magicNumber)
        return false;

    // check if editor version is same
    bool version_notMatch = false;
    quint16 major = 0;
    quint16 minor = 0;
    quint16 patch = 0;
    stream >> major >> minor >> patch;
    if ( major != ver_major ||
         minor != ver_minor ||
         patch != ver_patch ) {
        version_notMatch = true;
    }

    //
    stream >> m_lastEditPath;
    stream >> m_externals;
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 
 
void EditorMain::closeEvent ( QCloseEvent* _event )
{
    // save settings
    this->saveSettings();

    //
    QMainWindow::closeEvent(_event);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::onBrowseResource()
{
    // show new project dialog
    QString dirPath = QFileDialog::getExistingDirectory ( this // parent
                                                          , tr("Select Browsing Directory") // caption
                                                          , m_lastEditPath // get last edit dir
                                                        );

    // if use cancle selection, return
    if ( dirPath == "" )
        return;

    m_lastEditPath = dirPath; 
    resourceWidget->explore (dirPath);
    // ex_log ( qPrintable(dirPath) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::onCloseResource()
{
    ex_log_func ();
}

// KEEPME { 
// // show new project dialog
// QString fileName = QFileDialog::getOpenFileName ( this, // parent
//                                                   tr("Select Config File"), // caption
//                                                   m_lastEditPath, // get last edit dir
//                                                   tr("Config File (*.xml)") // filter
//                                                 );

// // if use cancle selection, return
// if ( fileName == "" )
//     return;

// // 
// m_lastEditPath = QFileInfo(fileName).dir().absolutePath();
// elementWidget->explore ( fileName );
// } KEEPME end 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::onOpenActionData()
{
    // show new project dialog
    QString dirPath = QFileDialog::getExistingDirectory ( this // parent
                                                          , tr("Select Config File") // caption
                                                          , m_lastEditPath // get last edit dir
                                                        );

    // if use cancle selection, return
    if ( dirPath == "" )
        return;

    // 
    m_lastEditPath = dirPath;
    elementWidget->explore ( "ActionData", dirPath );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::onCloseFile()
{
    ex_log_func ();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::onSaveFile()
{
    ex_log_func ();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::tick ()
{		
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::updateTitle()
{
    // title
    QString title; 
    title.sprintf ( "dho_editor %d.%d.%d", ver_major, ver_minor, ver_patch );
    setWindowTitle( title );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::updateMenu()
{
    // TODO:
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::setupTextEditor()
{
    ExternalDialog dialog ( m_externals["text"], this);
    if ( dialog.exec() )
    {
        m_externals["text"] = ExternalTool ( dialog.pathText(), dialog.argsText() );
        ex_log ("set external text editor: %s, %s\n", qPrintable(dialog.pathText()), qPrintable(dialog.argsText()) );
        this->saveSettings();
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::setupImageEditor()
{
    ExternalDialog dialog ( m_externals["image"], this);
    if ( dialog.exec() )
    {
        m_externals["image"] = ExternalTool ( dialog.pathText(), dialog.argsText() );
        ex_log ("set external image editor: %s, %s\n", qPrintable(dialog.pathText()), qPrintable(dialog.argsText()) );
        this->saveSettings();
    }
}

// TEST { 
// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void EditorMain::rapidTest ()
{
#if 0
    ex_log ("trigger rapid test\n");

    const char* cwd = ex::core_t::config()->get<const char*>("cwd");
    ex_log( "the current working path: %s\n", cwd );

    const char* tags[] = {
        "normal slash 1",
        "normal slash 2",
        "normal slash 3",
        "normal slash 4",
        "heavy slash 1",
        "heavy slash 2",
        "heavy slash 3",
        "upper slash",
        "down slash",
        "on hit",
        "on hit ground",
        "on hit air",
        "run",
        "walk",
        "super slash lv1",
        "super slash lv2"
    };

    for ( int i = 0; i < EX_ARRAY_COUNT(tags); ++i ) {
        TestData testData;
        testData.setID(i);
        testData.setTag(tags[i]);

        ex::XmlSerializeNode saveNode ( ex::XmlNode::smart_ptr_t(ex_new ex::XmlNode("Root") ) );
        EX_SERIALIZE ( &saveNode, testData.getClassName(), testData );
        saveNode.saveToFile ( ex::path_t().printf("./_res/%d.xml", i).c_str() );
    }

    EX_HW_BREAK();
#else
    // elementWidget->doLuaFilter();

    // TODO { 
    // Try something like this. Don't forget to add error checking...

    // lua_newtable(L);
    // lua_setglobal(L,filename);
    // luaL_loadfile(L,filename);
    // lua_getglobal(L,filename);
    // lua_setfenv(L,-2);
    // lua_pcall(L,...);

    // Do I have to set the environment back after this? 
    // Because when I try calling a function I registered after this I get: 
    // "attempt to call global 'print' (a nil value)" – Homeliss Apr 29 at 1:01

    // Oh, right. The code I gave replaces the globals for that script by an empty 
    // table and it does not have print. I see two solutions: add an __index metamethod 
    // to that table pointing to _G or temporarily add a __newindex method to _G before 
    // lua_pcall so that all definitions in the script go to your table. 
    // I suggest you prototype this first using Lua and then move to C. – lhf Apr 29 at 1:27
    // } TODO end 

    // parse lua script
    lua_newtable( ex::lua::state() );
    lua_setglobal( ex::lua::state(), "test_01" );
    ex::lua::parse ( ex::lua::state(), luaL_loadfile(ex::lua::state(), "./_res/lua/test_01.lua" ) );

    lua_newtable( ex::lua::state() );
    lua_setglobal( ex::lua::state(), "test_02" );
    ex::lua::parse ( ex::lua::state(), luaL_loadfile(ex::lua::state(), "./_res/lua/test_02.lua" ) );

    // NOTE: check love2d: int luax_getfunction(lua_State * L, const char * mod, const char * fn)
    lua_getglobal( ex::lua::state(), "test_01" );
    lua_getfield( ex::lua::state(), -1, "process" );
    lua_pushnumber( ex::lua::state(), 20 );
    if ( lua_pcall( ex::lua::state(), 1, 0, 0 ) != 0 ) {
        ex_error( "%s\n", lua_tostring(ex::lua::state(), -1) );
    }

    lua_getglobal( ex::lua::state(), "test_02" );
    lua_getfield( ex::lua::state(), -1, "process" );
    lua_pushnumber( ex::lua::state(), 20 );
    if ( lua_pcall( ex::lua::state(), 1, 0, 0 ) != 0 ) {
        ex_error( "%s\n", lua_tostring(ex::lua::state(), -1) );
    }
#endif
}
// } TEST end 
