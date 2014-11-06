/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#include <QDateTime>
#include <QtConcurrentRun>
#include <QProcess>
#include <QUuid>
#include <QFile>
#include <QHash>
#include <QMutex>
#include <QMetaType>

#include "Logger.h"
#include "Private/Logger_p.h"
#include "exTargomanBase.h"

namespace Targoman {
namespace Common {

Logger* Logger::Instance = NULL;

Logger::Logger(QObject *parent) :
    QObject(parent),pPrivate(new Private::LoggerPrivate)
{
    this->pPrivate->LogSettings[enuLogType::Debug].canBeShown(0);
    this->pPrivate->LogSettings[enuLogType::Error].canBeShown(0);
    this->pPrivate->LogSettings[enuLogType::Warning].canBeShown(0);
    this->pPrivate->LogSettings[enuLogType::Info].canBeShown(0);
    this->pPrivate->LogSettings[enuLogType::Happy].canBeShown(0);
    this->registerActor(&this->pPrivate->ActorUUID,"BaseLogger");
}

bool Logger::init(const QString &_fileName,
                  QMap<enuLogType::Type, clsLogSettings> _logSettings,
                  quint64 _maxSize,
                  bool _show)
{
    qRegisterMetaType<Targoman::Common::enuLogType::Type>("Targoman::Common::enuLogType::Type");
    this->pPrivate->LogFile.setFileName(_fileName.size() ? _fileName : "/dev/null");
    this->pPrivate->LogSettings[enuLogType::Debug] = _logSettings.value(enuLogType::Debug);
    this->pPrivate->LogSettings[enuLogType::Error] = _logSettings.value(enuLogType::Error);
    this->pPrivate->LogSettings[enuLogType::Warning] = _logSettings.value(enuLogType::Warning);
    this->pPrivate->LogSettings[enuLogType::Info] = _logSettings.value(enuLogType::Info);
    this->pPrivate->LogSettings[enuLogType::Happy] = _logSettings.value(enuLogType::Happy);
    this->pPrivate->MaxFileSize = _maxSize * 1024 * 1024;
    this->setVisible(_show);
    this->setActive();
    return this->pPrivate->open();
}

void Logger::write(const QString &_actorID,
                   enuLogType::Type _type,
                   quint8 _level,
                   const QString &_message,
                   bool _newLine)
{
    if (this->pPrivate->LogSettings[_type].canBeShown(_level) == false)
        return;

    QString Actor = this->pPrivate->Actors.value(_actorID);
    if (!_actorID.isEmpty() && Actor.isEmpty())
        throw exLogger(QString("Actor ID %1 Not registerd: message(%2)").arg(_actorID).arg(_message));

    QByteArray LogMessage=
            QString("[" + QDateTime().currentDateTime().toString("dd-MM-yyyy hh:mm:ss.zzz") + "]").toUtf8();

    LogMessage+= QString("[%1]").arg(enuLogType::toStr(_type));
    LogMessage += "[" + QString::number(_level) + "]";

    LogMessage += QString("[%1] ").arg(Actor.isEmpty() ? "LOG" : Actor);

    if (_newLine)
        LogMessage += _message+"\n";


    QMutexLocker Locker(&this->pPrivate->mxLog);
    if (this->pPrivate->LogFile.fileName().size()){
        if (!this->pPrivate->LogFile.isOpen() ||
                !this->pPrivate->LogFile.isWritable())
            this->pPrivate->open();

        this->pPrivate->LogFile.write(LogMessage);
    }
    switch(_type){
    case enuLogType::Debug:
        fprintf(stderr,"%s%s%s", TARGOMAN_COLOR_DEBUG, LogMessage.constData(), TARGOMAN_COLOR_NORMAL);
        break;
    case enuLogType::Info:
        fprintf(stderr,"%s%s%s", TARGOMAN_COLOR_INFO, LogMessage.constData(), TARGOMAN_COLOR_NORMAL);
        break;
    case enuLogType::Warning:
        fprintf(stderr,"%s%s%s", TARGOMAN_COLOR_WARNING, LogMessage.constData(), TARGOMAN_COLOR_NORMAL);
        break;
    case enuLogType::Happy:
        fprintf(stderr,"%s%s%s", TARGOMAN_COLOR_HAPPY, LogMessage.constData(), TARGOMAN_COLOR_NORMAL);
        break;
    case enuLogType::Error:
        fprintf(stderr,"%s%s%s", TARGOMAN_COLOR_ERROR, LogMessage.constData(), TARGOMAN_COLOR_NORMAL);
        break;
    default:
        break;
    }

    if (this->pPrivate->LogFile.fileName().size()){
        this->pPrivate->LogFile.flush();
        this->pPrivate->rotateLog();
    }
    Locker.unlock();
    emit this->sigLogAdded(QDateTime().currentDateTime(), _actorID, _type, _level, _message);
}

void Logger::registerActor(QString *_actorUUID, const QString &_actorName)
{
    TargomanDebug(5, qPrintable("Registering "+ *_actorUUID + " / " + _actorName));
    QMutexLocker Locker(&this->pPrivate->mxLog);

    if (_actorName.isEmpty())
        throw exLogger("Invalid Null Actor Name: " + *_actorUUID);

    if (_actorUUID->isEmpty())
        *_actorUUID = QUuid::createUuid().toString();
    else if (this->pPrivate->Actors.contains(*_actorUUID))
        throw exLogger(QString ("Invalid Actor UUID: %1 it is in use by %2").arg(
                           *_actorUUID).arg(this->pPrivate->Actors.value(*_actorUUID)));

    if (this->pPrivate->Actors.values().contains(_actorName) &&
            this->pPrivate->Actors.key(_actorName,0) != *_actorUUID)
        throw exLogger(QString ("Invalid Actor UUID/Name: %1/%2 it has been previously registered as: %3/%4").arg(
                           *_actorUUID).arg(_actorName).arg(this->pPrivate->Actors.key(_actorName)).arg(_actorName));

    this->pPrivate->Actors.insert(*_actorUUID, _actorName);
    TargomanInfo(6, qPrintable(_actorName + " Registerd with UUID: " + *_actorUUID) );
}

const QHash<QString, QString> &Logger::actors()
{
    return this->pPrivate->Actors;
}
Logger::~Logger()
{
    //It is defined to suppress error on QScoppedPointer
}

void Logger::setActive(bool _state)
{
    this->pPrivate->GlobalSettings = (this->pPrivate->GlobalSettings & 0xFE)  | (_state ? 1 : 0);
}

bool Logger::isActive()
{
    return this->pPrivate->GlobalSettings & 0x01 ? true : false;
}

void Logger::setVisible(bool _state)
{
    this->pPrivate->GlobalSettings = (this->pPrivate->GlobalSettings & 0xFD)  | (_state ? 2 : 0);
}

bool Logger::isVisible()
{
    return this->pPrivate->GlobalSettings & 0x02 ? true : false;
}


/***************************************************************************/
Private::LoggerPrivate::LoggerPrivate()
{
    this->LogSettings = new clsLogSettings[enuLogType::getCount()];
}

bool Private::LoggerPrivate::open()
{
    if (this->LogFile.isOpen())
        this->LogFile.close();

    this->LogFile.open(QIODevice::Append);

    if ( ! this->LogFile.isWritable()) {
        TargomanError("%s",QString("Unable to open <%1> for writing").arg(this->LogFile.fileName()).toUtf8().constData());
        return false;
    }
    return true;
}

void Private::LoggerPrivate::rotateLog()
{
    if (((quint64)this->LogFile.size() > this->MaxFileSize) ) {
        TargomanDebug(7, "Rotating Logs");
        this->LogFile.close();
        QString NewFileName = this->LogFile.fileName() + QDateTime().currentDateTime().toString("yyyy_MM_dd_hh:mm:ss");
        QFile::rename(this->LogFile.fileName(), NewFileName);
#ifdef Q_OS_UNIX
        QProcess::startDetached("gzip",QStringList(NewFileName));
#else
        throw exTargomanNotImplemented("Compressing files on OSs other than linux is not yet implemented");
#endif
    }
}

}
}

