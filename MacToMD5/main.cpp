#include <QtCore/QCoreApplication>
#include <QString>
#include <QByteArray>
#include <iostream>
#include <QCryptographicHash>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv); //renamed the a to app
	QString scret="Little-Star", md5;
	
	if(argc >1)
	{
		QString mac = argv[1];//00:21:CC:C9:55:97
		QByteArray ba;  
		ba = QCryptographicHash::hash ( (scret+mac).toAscii(), QCryptographicHash::Md5 );  
		md5.append(ba.toHex());  
		std::cout << md5.toUtf8().data();
	}else{
		std::cout <<  "input mac address like XX:XX:XX:XX:XX:XX \n";
	}
	QTimer::singleShot(5000, &app, SLOT(quit()));
	
    return app.exec(); //and we run the application
}