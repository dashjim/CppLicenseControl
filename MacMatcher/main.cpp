
#include <QtCore/QCoreApplication>
#include <QNetworkInterface>
#include <QFile>
#include <iostream>
#include <QCryptographicHash>

/**
	1, get mac address
	2, read the license file
	3, calculate the MD5
	4, compare the MD5
*/

QString getMacAddress();
QString getFileContent();

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QString scret="Little-Star", mac, license;
	license = getFileContent();
	std::cout <<"license: " <<  license.toUtf8().data() <<"\n";

	foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
		mac = netInterface.hardwareAddress();
		std::cout <<"mac: " <<  mac.toUtf8().data() <<"\n";
		QByteArray ba;  
		ba = QCryptographicHash::hash ( (scret+mac).toAscii(), QCryptographicHash::Md5 );  
		QString md5;
		md5.append(ba.toHex());  
		if(md5 == license) 
		{
			std::cout <<"matched!" <<"\n";
			break;
		}
		
    }
	std::cout <<"End" <<"\n";

	return a.exec();
}


QString getFileContent()
{
	QFile f("license");
	QString content;
	if (f.open(QFile::ReadOnly | QFile::Text)){
		QTextStream in(&f);
		content = in.readAll();
       // qDebug() << f.size() << content;
	}
	return content;
}