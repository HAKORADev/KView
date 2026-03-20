#include "FileHandler.h"
#include <QMimeDatabase>
#include <QMimeType>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDesktopServices>
#include <QFileInfo>
#include <KArchive/KZip>
#include <KArchive/KTar>
#include <KArchive/K7Zip>
#include <QDebug>

FileHandler::FileHandler(QObject *parent) : QObject(parent) {}

void FileHandler::setFilePath(const QString &path) {
    if (m_filePath == path) return;
    
    QFileInfo info(path);
    m_filePath = path;
    m_fileName = info.fileName();
    
    qint64 size = info.size();
    if (size < 1024) m_fileSize = QString::number(size) + " B";
    else if (size < 1024 * 1024) m_fileSize = QString::number(size / 1024.0, 'f', 2) + " KB";
    else if (size < 1024 * 1024 * 1024) m_fileSize = QString::number(size / (1024.0 * 1024.0), 'f', 2) + " MB";
    else m_fileSize = QString::number(size / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
    
    emit fileNameChanged();
    emit fileUrlChanged();
    emit fileSizeChanged();
    
    detectFileType();
    updateDirList();
}

void FileHandler::updateDirList() {
    QFileInfo info(m_filePath);
    QDir dir = info.dir();
    QStringList filters;
    m_dirFiles = dir.entryList(QDir::Files, QDir::Name | QDir::IgnoreCase);
    
    m_currentIndex = m_dirFiles.indexOf(m_fileName);
    emit totalFilesChanged();
    emit currentIndexChanged();
}

void FileHandler::nextFile() {
    if (m_dirFiles.isEmpty()) return;

    if (m_currentIndex == m_dirFiles.size() - 1) {
        if (!m_atEndWarned) {
            emit messageRequested("This is the end, press again to start from the start");
            m_atEndWarned = true;
            return;
        }
        m_currentIndex = 0;
    } else {
        m_currentIndex++;
    }

    m_atEndWarned = false;
    m_atStartWarned = false;
    QString newPath = QFileInfo(m_filePath).dir().absoluteFilePath(m_dirFiles[m_currentIndex]);
    setFilePath(newPath);
    emit messageRequested(QString("%1 (%2/%3)").arg(m_fileName).arg(m_currentIndex + 1).arg(m_dirFiles.size()));
}

void FileHandler::previousFile() {
    if (m_dirFiles.isEmpty()) return;

    if (m_currentIndex == 0) {
        if (!m_atStartWarned) {
            emit messageRequested("This is the start, press again to start from the bottom");
            m_atStartWarned = true;
            return;
        }
        m_currentIndex = m_dirFiles.size() - 1;
    } else {
        m_currentIndex--;
    }

    m_atEndWarned = false;
    m_atStartWarned = false;
    QString newPath = QFileInfo(m_filePath).dir().absoluteFilePath(m_dirFiles[m_currentIndex]);
    setFilePath(newPath);
    emit messageRequested(QString("%1 (%2/%3)").arg(m_fileName).arg(m_currentIndex + 1).arg(m_dirFiles.size()));
}

void FileHandler::openFileExternally() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(m_filePath));
}

void FileHandler::detectFileType() {
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(m_filePath);
    m_fileType = mime.name();

    if (mime.inherits("text/plain")) {
        loadTextContent();
    } else if (mime.inherits("application/zip") || mime.inherits("application/x-tar") || mime.inherits("application/x-7z-compressed")) {
        loadArchiveContent();
    }

    emit fileTypeChanged();
}

void FileHandler::loadTextContent() {
    QFile file(m_filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        m_content = in.readAll();
        emit contentChanged();
    }
}

void FileHandler::loadArchiveContent() {
    m_archiveContents.clear();
    KArchive* archive = nullptr;

    if (m_fileType.contains("zip")) {
        archive = new KZip(m_filePath);
    } else if (m_fileType.contains("tar") || m_fileType.contains("xz")) {
        archive = new KTar(m_filePath);
    } else if (m_fileType.contains("7z")) {
        archive = new K7Zip(m_filePath);
    }

    if (archive && archive->open(QIODevice::ReadOnly)) {
        const KArchiveDirectory* root = archive->directory();
        m_archiveContents = root->entries();
        archive->close();
    }

    delete archive;
    emit archiveContentsChanged();
}
