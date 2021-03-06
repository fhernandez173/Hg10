/*
 * SmileyPickerController.cpp
 *
 *  Created on: 5 avr. 2014
 *      Author: pierre
 */
#include "SmileyPickerController.hpp"



#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QRegExp>
#include <QFile>
#include <QDir>

#include <bb/cascades/AbstractPane>
#include <bb/cascades/GroupDataModel>

#include  "Image/HFRNetworkAccessManager.hpp"
#include  "Image/WebResourceManager.h"
#include  "DataObjects.h"

SmileyPickerController::SmileyPickerController(QObject *parent) : QObject(parent), m_ListView(NULL) {

    bool check = connect(WebResourceManager::get(), SIGNAL(onImageReady(const QString &, const QString &)), this, SLOT(onImageReady(const QString &, const QString &)));
    Q_ASSERT(check);
    Q_UNUSED(check);


};

void SmileyPickerController::loadDefautSmiley() {
    getSmiley("http://amonchakai.github.io/emoji/emoji.xml");
}

void SmileyPickerController::getSmiley(const QString &url_str) {
	// list green + yellow flags
	const QUrl url(url_str);

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


	QNetworkReply* reply = HFRNetworkAccessManager::get()->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(checkReply()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);

}



void SmileyPickerController::checkReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer(reply->readAll());
				response = QString::fromUtf8(buffer);


				QRegExp item("<item url=\"([^\"]+)");
				int pos = 0;

				while((pos = item.indexIn(response, pos)) != -1) {

				    WebResourceManager::get()->getImage(item.cap(1));

				    pos += item.matchedLength();
				}

			}
		} else {
			response = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
			qDebug() << response;
		}

		reply->deleteLater();
	}
}

void SmileyPickerController::onImageReady(const QString &url, const QString &diskPath) {

    if(diskPath == "loading")
        return;

    if(url[0] == '/')
        return;

    qDebug() << url << diskPath;


    m_Mutex.lockForWrite();

    for(int i = 0 ; i < m_Stickers.length() ; ++i)
        if(m_Stickers.at(i)->getDistUrl() == url) {
            m_Mutex.unlock();
            return;
        }


    Sticker *s = new Sticker;
    s->setDistUrl(url);
    s->setLocalUrl(diskPath);
    m_Stickers.push_back(s);

    pushToView();

    m_Mutex.unlock();
}

void SmileyPickerController::pushToView() {
    // ----------------------------------------------------------------------------------------------
        // get the dataModel of the listview if not already available
        using namespace bb::cascades;

        if(m_ListView == NULL) {
            qWarning() << "did not received the listview. quit.";
            return;
        }

        GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
        if (!dataModel) {

            qDebug() << "create new model";
            dataModel = new GroupDataModel(
                    QStringList() << "distUrl"
                                  << "localUrl"
                     );
            m_ListView->setDataModel(dataModel);
        }

        // ----------------------------------------------------------------------------------------------
        // push data to the view

        Sticker *s = new Sticker;
        s->setDistUrl(m_Stickers.last()->getDistUrl());
        s->setLocalUrl(m_Stickers.last()->getLocalUrl());

        dataModel->insert(s);
}


void SmileyPickerController::updateView() {

    // ----------------------------------------------------------------------------------------------
    // get the dataModel of the listview if not already available
    using namespace bb::cascades;

    if(m_ListView == NULL) {
        qWarning() << "did not received the listview. quit.";
        return;
    }

    GroupDataModel* dataModel = dynamic_cast<GroupDataModel*>(m_ListView->dataModel());
    if (dataModel) {
        dataModel->clear();
    } else {
        qDebug() << "create new model";
        dataModel = new GroupDataModel(
                QStringList() << "distUrl"
                              << "localUrl"
                 );
        m_ListView->setDataModel(dataModel);
    }

    // ----------------------------------------------------------------------------------------------
    // push data to the view

    QList<QObject*> datas;
    for(int i = 0 ; i < m_Stickers.size() ; ++i) {

        Sticker *s = new Sticker;
        s->setDistUrl(m_Stickers.at(i)->getDistUrl());
        s->setLocalUrl(m_Stickers.at(i)->getLocalUrl());
        datas.push_back(s);


    }

    dataModel->clear();
    dataModel->insertList(datas);


}

void SmileyPickerController::refresh() {
    QString directory = QDir::homePath() + "/Cache";
    if (QFile::exists(directory)) {
        QDir dir(directory);
        dir.setNameFilters(QStringList() << "*.*");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList()) {
            dir.remove(dirFile);
        }
    }

    loadDefautSmiley();

}


