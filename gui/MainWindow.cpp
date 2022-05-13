//
// Created by gunSlaveUnit on 13.04.2022.
//

#include "MainWindow.h"

MainWindow::MainWindow() {
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowTitle(WINDOW_TITLE);

    centralWidget = new QGraphicsView(this);
    centralWidget->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    centralLayout = new QGridLayout(centralWidget);

    fileLabel = new QLabel(tr("File: "));
    centralLayout->addWidget(fileLabel, 0, 0);
    selectedFileName = new QLabel("not selected");
    centralLayout->addWidget(selectedFileName, 0, 1);

    selectFileButton = new QPushButton(tr("Select"));
    centralLayout->addWidget(selectFileButton, 1, 0, 1, 2);
    connect(selectFileButton, SIGNAL(clicked(bool)), this, SLOT(openFileDialog()));

    startButton = new QPushButton(tr("Start"));
    centralLayout->addWidget(startButton, 2, 0);

    closeButton = new QPushButton(tr("Close"));
    centralLayout->addWidget(closeButton, 2, 1);
    connect(closeButton, &QPushButton::clicked, this,
            [this]() {
                this->close();
            });

    elapsedTimeLabel = new QLabel(tr("Elapsed time: "));
    centralLayout->addWidget(elapsedTimeLabel, 3, 0);

    elapsedTimeTextValue = new QLabel(tr("12.4s"));
    centralLayout->addWidget(elapsedTimeTextValue, 3, 1);

    sourceFileSize = new QLabel(tr("Source size: "));
    centralLayout->addWidget(sourceFileSize, 4, 0);
    sourceFileSizeValue = new QLabel(tr("0 B"));
    centralLayout->addWidget(sourceFileSizeValue, 4, 1);

    receivedFileSize = new QLabel(tr("Received size: "));
    centralLayout->addWidget(receivedFileSize, 5, 0);
    receivedFileSizeValue = new QLabel(tr("0 B"));
    centralLayout->addWidget(receivedFileSizeValue, 5, 1);

    compressionRatioLabel = new QLabel(tr("Compression ratio: "));
    centralLayout->addWidget(compressionRatioLabel, 6, 0);

    compressionRatioTextValue = new QLabel(tr("0 %"));
    centralLayout->addWidget(compressionRatioTextValue, 6, 1);

    progressBar = new QProgressBar;
    progressBar->setMaximum(100);
    centralLayout->addWidget(progressBar, 7, 0, 1, 2);
    QPalette p = palette();
    p.setColor(QPalette::Highlight, Qt::darkCyan);
    setPalette(p);

    connectMethodDependMode();

    connect(&coder, SIGNAL(send_opened_filename(std::string)),
            this, SLOT(setOpenedFileSizeLabelValue(const std::string &)));
    connect(&coder, SIGNAL(send_created_filename(std::string)),
            this, SLOT(setCreatedFileSizeLabelValue(const std::string &)));
    connect(&coder, SIGNAL(byte_processed(const uint_fast32_t &, const uint_fast32_t &)),
            this, SLOT(setProgressBarProcessedBytesValue(const uint_fast32_t &, const uint_fast32_t &)));
}

MainWindow::~MainWindow() {
    delete sourceFileSize;
    delete sourceFileSizeValue;
    delete receivedFileSize;
    delete receivedFileSizeValue;
    delete progressBar;
    delete compressionRatioTextValue;
    delete compressionRatioLabel;
    delete elapsedTimeTextValue;
    delete elapsedTimeLabel;
    delete selectedFileName;
    delete selectFileButton;
    delete closeButton;
    delete centralLayout;
    delete centralWidget;
}

void MainWindow::openFileDialog() {
    auto dialog = QFileDialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    auto filename = dialog.getOpenFileName();
    QFileInfo fi(filename);
    filename = fi.fileName();
    selectedFileName->setText(filename.isEmpty() ? "Not Selected" : filename);
    auto ext = fi.completeSuffix();
    setWorkingModeDependFileExt(ext);
}

void MainWindow::setWorkingModeDependFileExt(const QString &ext) {
    if (ext == "ahf")
        MODE = DECODE;
    else
        MODE = ENCODE;

    connectMethodDependMode();
}

void MainWindow::connectMethodDependMode() {
    startButton->disconnect();

    if (MODE == ENCODE)
        connect(startButton, &QPushButton::clicked, this,
                [this]() {
                    coder.encode(selectedFileName->text().toStdString());
                });
    else
        connect(startButton, &QPushButton::clicked, this,
                [this]() {
                    coder.decode(selectedFileName->text().toStdString());
                });
}

#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    //menu.addAction(cutAct);
    //menu.addAction(copyAct);
    //menu.addAction(pasteAct);
    menu.exec(event->globalPos());
}

void MainWindow::setOpenedFileSizeLabelValue(const std::string & filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    opened_file_size = file.tellg();
    sourceFileSizeValue->setText(humanFileSize(opened_file_size, true, 2));
    file.close();
}

void MainWindow::setCreatedFileSizeLabelValue(const std::string & filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    created_file_size = file.tellg();
    receivedFileSizeValue->setText(humanFileSize(created_file_size, true, 2));
    file.close();

    set_compression_ratio_value();
}

QString MainWindow::humanFileSize(const uint_fast32_t &bytes,
                                  const bool &si,
                                  const uint_fast32_t &precision) {

    double size = bytes;
    uint_fast32_t thresh = si ? 1000 : 1024;

    if (std::abs(size) < thresh) {
        return QString::number(size) + " B";
    }

    std::array<std::array<QString, 8>, 2> suffixes {
            "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB",
            "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB"
    };

    uint_fast32_t u = -1;
    const uint_fast32_t r = pow(10, precision);

    do {
        size /= thresh;
        ++u;
    } while (std::round(std::abs(size) * r) / r >= thresh && u < 7);


    return QString::number(size) + " " + suffixes[si][u];
}

void MainWindow::setProgressBarProcessedBytesValue(const uint_fast32_t & processed, const uint_fast32_t & size) {
    progressBar->setValue(ceil(processed * 100.0 / size));
}

void MainWindow::set_compression_ratio_value() {
    auto ratio = ceil(created_file_size * 100.0 / opened_file_size );
    compressionRatioTextValue->setText(QString::number(ratio) + " %");
}


#endif // QT_NO_CONTEXTMENU
