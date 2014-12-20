
#include <QtGui>

#include "widgetgallery.h"


WidgetGallery::WidgetGallery(QWidget *parent)
    : QDialog(parent)
{
    originalPalette = QApplication::palette();

    styleComboBox = new QComboBox;
    styleComboBox->addItems(QStyleFactory::keys());

    styleSubComboBox = new QComboBox();
    styleSubComboBox->addItems(qApp->property("QTGTK_RC_THEMES").toStringList());
    GtkThemeName = "Default";

    styleSubFontComboBox = new QComboBox();
        
    QStringList fontFamilyList = QFontDatabase().families();
    GtkThemeFontFamily = QApplication::font().family();
    int currentID = fontFamilyList.lastIndexOf(GtkThemeFontFamily);
    styleSubFontComboBox->addItems(fontFamilyList);
    styleSubFontComboBox->setCurrentIndex(currentID);
        
    styleSubFontSizeComboBox = new QComboBox();
    QList<int> fontSizeListI = QFontDatabase().pointSizes(QApplication::font().family());
    QStringList fontSizeList;
    foreach (int i,fontSizeListI) {
        fontSizeList.append(QString("%1").arg(i));
    }
    GtkThemeFontSize = QApplication::font().pointSize();
    currentID = fontSizeList.lastIndexOf(QString("%1").arg(GtkThemeFontSize));
        
    styleSubFontSizeComboBox->addItems(fontSizeList);
    styleSubFontSizeComboBox->setCurrentIndex(currentID);

    styleLabel = new QLabel(tr("&Style:"));
    styleLabel->setBuddy(styleComboBox);

    useStylePaletteCheckBox = new QCheckBox(tr("&Use style's standard palette"));
    useStylePaletteCheckBox->setChecked(true);

    disableWidgetsCheckBox = new QCheckBox(tr("&Disable widgets"));

    createTopLeftGroupBox();
    createTopRightGroupBox();
    createBottomLeftTabWidget();
    createBottomRightGroupBox();
    createProgressBar();



    connect(styleComboBox, SIGNAL(activated(QString)),
            this, SLOT(changeStyle(QString)));
    connect(styleSubComboBox, SIGNAL(activated(QString)),
            this, SLOT(changeSubStyle(QString)));
    connect(styleSubFontComboBox, SIGNAL(activated(QString)),
            this, SLOT(changeSubStyleFont(QString)));
    connect(styleSubFontSizeComboBox, SIGNAL(activated(QString)),
            this, SLOT(changeSubStyleFontSize(QString)));
    connect(useStylePaletteCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(changePalette()));
    connect(disableWidgetsCheckBox, SIGNAL(toggled(bool)),
            topLeftGroupBox, SLOT(setDisabled(bool)));
    connect(disableWidgetsCheckBox, SIGNAL(toggled(bool)),
            topRightGroupBox, SLOT(setDisabled(bool)));
    connect(disableWidgetsCheckBox, SIGNAL(toggled(bool)),
            bottomLeftTabWidget, SLOT(setDisabled(bool)));
    connect(disableWidgetsCheckBox, SIGNAL(toggled(bool)),
            bottomRightGroupBox, SLOT(setDisabled(bool)));



    QHBoxLayout * topLayout1 = new QHBoxLayout;
    topLayout1->addWidget(styleLabel);
    topLayout1->addWidget(styleComboBox);  
    topLayout1->addWidget(styleSubComboBox);
    topLayout1->addWidget(styleSubFontComboBox); 
    topLayout1->addWidget(styleSubFontSizeComboBox);
    topLayout1->addStretch(1);
        
    QHBoxLayout * topLayout2 = new QHBoxLayout;
    topLayout2->addWidget(useStylePaletteCheckBox);
    topLayout2->addWidget(disableWidgetsCheckBox);
    topLayout2->addStretch(1);
        
    QVBoxLayout * topLayout = new QVBoxLayout;
    topLayout->addLayout(topLayout1);
    topLayout->addLayout(topLayout2);


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(topLayout, 0, 0, 1, 2);
    mainLayout->addWidget(topLeftGroupBox, 1, 0);
    mainLayout->addWidget(topRightGroupBox, 1, 1);
    mainLayout->addWidget(bottomLeftTabWidget, 2, 0);
    mainLayout->addWidget(bottomRightGroupBox, 2, 1);
    mainLayout->addWidget(progressBar, 3, 0, 1, 2);
    mainLayout->setRowStretch(1, 1);
    mainLayout->setRowStretch(2, 1);
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 1);
    setLayout(mainLayout);

    setWindowTitle(tr("Styles"));
    changeStyle("Windows");
}



void WidgetGallery::changeStyle(const QString &styleName)
{
    style = styleName;
    QApplication::setStyle(QStyleFactory::create(styleName));
    changePalette();
}

void WidgetGallery::changeSubStyle(const QString &styleName)
{
    GtkThemeName = styleName;
    if (style != "GTK+") {
        return;
    }
    applyGtkTheme();       
    changePalette();
}

void WidgetGallery::changeSubStyleFont(const QString &fontFamily)
{
    if ( style != "GTK+" ) {
        GtkThemeFontFamily = fontFamily;
        QFont font = QApplication::font();
        font.setFamily(fontFamily);
        QApplication::setFont(font);
        return;
    }

    GtkThemeFontFamily = fontFamily;
    applyGtkTheme();
    changePalette(); 
        
    QList<int> fontSizeListI = QFontDatabase().pointSizes(fontFamily);
    QStringList fontSizeList;
    foreach(int i,fontSizeListI) {
        fontSizeList.append(QString("%1").arg(i));
    }
    QString currentSize = styleSubFontSizeComboBox->currentText();
    int currentID = fontSizeList.lastIndexOf(currentSize);
        
    styleSubFontSizeComboBox->clear();
    styleSubFontSizeComboBox->addItems(fontSizeList);  
    styleSubFontSizeComboBox->setCurrentIndex(currentID);
}

void WidgetGallery::changeSubStyleFontSize(const QString &fontSize)
{
    if ( style != "GTK+" ) {
        GtkThemeFontSize = fontSize.toInt();
        QFont font = QApplication::font();
        font.setPointSize(GtkThemeFontSize);
        QApplication::setFont(font);
        return;
    }
    GtkThemeFontSize = fontSize.toInt();
    applyGtkTheme();
    changePalette();
}

void WidgetGallery::changePalette()
{
    if (useStylePaletteCheckBox->isChecked())
        QApplication::setPalette(QApplication::style()->standardPalette());
    else
        QApplication::setPalette(originalPalette);
}
void WidgetGallery::applyGtkTheme()
{
    QTemporaryFile tempFile;
    if (tempFile.open()) {
        tempFile.write(qPrintable(QString("gtk-theme-name = \"%1\"\n\n").arg(GtkThemeName)));
        tempFile.write(qPrintable(QString("style \"user-font\"\n{\nfont_name=\"%1 %2\"\n}\n").arg(GtkThemeFontFamily).arg(GtkThemeFontSize)));
        tempFile.write(qPrintable(QString("widget_class \"*\" style \"user-font\"")));
        tempFile.close();
        qApp->setProperty("QTGTK_RC_FILE",tempFile.fileName());
    }
    QApplication::setStyle(QStyleFactory::create("GTK+"));
    QApplication::setPalette(QApplication::style()->standardPalette());
}
            

void WidgetGallery::advanceProgressBar()
{
    int curVal = progressBar->value();
    int maxVal = progressBar->maximum();
    progressBar->setValue(curVal + (maxVal - curVal) / 100);
}



void WidgetGallery::createTopLeftGroupBox()
{
    topLeftGroupBox = new QGroupBox(tr("Group 1"));

    radioButton1 = new QRadioButton(tr("Radio button 1"));
    radioButton2 = new QRadioButton(tr("Radio button 2"));
    radioButton3 = new QRadioButton(tr("Radio button 3"));
    radioButton1->setChecked(true);

    checkBox = new QCheckBox(tr("Tri-state check box"));
    checkBox->setTristate(true);
    checkBox->setCheckState(Qt::PartiallyChecked);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(radioButton1);
    layout->addWidget(radioButton2);
    layout->addWidget(radioButton3);
    layout->addWidget(checkBox);
    layout->addStretch(1);
    topLeftGroupBox->setLayout(layout);
}


void WidgetGallery::createTopRightGroupBox()
{
    topRightGroupBox = new QGroupBox(tr("Group 2"));

    defaultPushButton = new QPushButton(tr("Default Push Button"));
    defaultPushButton->setDefault(true);

    togglePushButton = new QPushButton(tr("Toggle Push Button"));
    togglePushButton->setCheckable(true);
    togglePushButton->setChecked(true);

    flatPushButton = new QPushButton(tr("Flat Push Button"));
    flatPushButton->setFlat(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(defaultPushButton);
    layout->addWidget(togglePushButton);
    layout->addWidget(flatPushButton);
    layout->addStretch(1);
    topRightGroupBox->setLayout(layout);
}

void WidgetGallery::createBottomLeftTabWidget()
{
    bottomLeftTabWidget = new QTabWidget;
    bottomLeftTabWidget->setSizePolicy(QSizePolicy::Preferred,
                                       QSizePolicy::Ignored);

    QWidget *tab1 = new QWidget;
    tableWidget = new QTableWidget(10, 10);

    QHBoxLayout *tab1hbox = new QHBoxLayout;
    tab1hbox->setMargin(5);
    tab1hbox->addWidget(tableWidget);
    tab1->setLayout(tab1hbox);

    QWidget *tab2 = new QWidget;
    textEdit = new QTextEdit;

    textEdit->setPlainText(tr("Twinkle, twinkle, little star,\n"
                              "How I wonder what you are.\n"
                              "Up above the world so high,\n"
                              "Like a diamond in the sky.\n"
                              "Twinkle, twinkle, little star,\n"
                              "How I wonder what you are!\n"));
                     
    QHBoxLayout *tab2hbox = new QHBoxLayout;
    tab2hbox->setMargin(5);
    tab2hbox->addWidget(textEdit);
    tab2->setLayout(tab2hbox);

    bottomLeftTabWidget->addTab(tab1, tr("&Table"));
    bottomLeftTabWidget->addTab(tab2, tr("Text &Edit"));
}

void WidgetGallery::createBottomRightGroupBox()
{
    bottomRightGroupBox = new QGroupBox(tr("Group 3"));
    bottomRightGroupBox->setCheckable(true);
    bottomRightGroupBox->setChecked(true);

    lineEdit = new QLineEdit("s3cRe7");
    lineEdit->setEchoMode(QLineEdit::Password);

    spinBox = new QSpinBox(bottomRightGroupBox);
    spinBox->setValue(50);

    dateTimeEdit = new QDateTimeEdit(bottomRightGroupBox);
    dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    slider = new QSlider(Qt::Horizontal, bottomRightGroupBox);
    slider->setValue(40);

    scrollBar = new QScrollBar(Qt::Horizontal, bottomRightGroupBox);
    scrollBar->setValue(60);

    dial = new QDial(bottomRightGroupBox);
    dial->setValue(30);
    dial->setNotchesVisible(true);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(lineEdit, 0, 0, 1, 2);
    layout->addWidget(spinBox, 1, 0, 1, 2);
    layout->addWidget(dateTimeEdit, 2, 0, 1, 2);
    layout->addWidget(slider, 3, 0);
    layout->addWidget(scrollBar, 4, 0);
    layout->addWidget(dial, 3, 1, 2, 1);
    layout->setRowStretch(5, 1);
    bottomRightGroupBox->setLayout(layout);
}


void WidgetGallery::createProgressBar()
{
    progressBar = new QProgressBar;
    progressBar->setRange(0, 10000);
    progressBar->setValue(0);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(advanceProgressBar()));
    timer->start(1000);
}

