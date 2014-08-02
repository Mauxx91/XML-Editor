/**
 * File: main.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/16
*/

#include "View/MainWindowView.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindowView w;
    w.show();

    if(argc > 1)
        w.OpenXmlFile(argv[1], true);

    //For fast test
    w.OpenXmlFile("C:/Users/maurizio.biancucci/Desktop/prova - Copy.xml", true);
    //w.OpenXmlFile("E:/workspaces/motogp14/build/DebugGame/Data/Graphic/Bike/BikeBehaviourConfig.xml", true);

    return a.exec();
}
