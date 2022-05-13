//
// Created by gunSlaveUnit on 13.04.2022.
//

#ifndef ADAPTIVE_HUFFMAN_MAINWINDOW_H
#define ADAPTIVE_HUFFMAN_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QGraphicsView>
#include <QLabel>
#include <QFileDialog>
#include <QSplitter>
#include <QProgressBar>
#include <QMenu>
#include <QContextMenuEvent>

#include "../coder/AdaptiveHuffmanCoder.h"


class MainWindow final : public QMainWindow {
Q_OBJECT
public:
    MainWindow();

    ~MainWindow() override;

public slots:

    void openFileDialog();
    void connectMethodDependMode();

private:
    const qint32 WINDOW_WIDTH = 300, WINDOW_HEIGHT = 200;
    const QString WINDOW_TITLE = "ZipFile";
    QGraphicsView *centralWidget;
    QGridLayout *centralLayout;
    QLabel *fileLabel;
    QLabel *selectedFileName;
    QPushButton *selectFileButton;
    QPushButton *startButton;
    QPushButton *closeButton;
    QLabel *elapsedTimeLabel;
    QLabel *elapsedTimeTextValue;
    QLabel *compressionRatioLabel;
    QLabel *compressionRatioTextValue;
    QProgressBar *progressBar;
    QLabel *sourceFileSize;
    QLabel *sourceFileSizeValue;
    QLabel *receivedFileSize;
    QLabel *receivedFileSizeValue;

    enum WORKING_MODES {
        ENCODE, DECODE
    };
    uint32_t MODE = ENCODE;
    AdaptiveHuffmanCoder coder;

    void setWorkingModeDependFileExt(const QString &ext);
    #ifndef QT_NO_CONTEXTMENU
        void contextMenuEvent(QContextMenuEvent *event) override;
    #endif // QT_NO_CONTEXTMENU
};


#endif //ADAPTIVE_HUFFMAN_MAINWINDOW_H
