#include <QApplication>
#include <QMainWindow>

#include "AdaptiveHuffmanCoder.h"


int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QMainWindow window;
    window.setFixedSize(300, 200);

    auto coder = AdaptiveHuffmanCoder();
    //coder.encode("alice.txt");
    //coder.decode("alice.ahf");

    window.show();
    return QApplication::exec();

}
