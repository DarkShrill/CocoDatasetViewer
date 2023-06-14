#ifndef IMAGELABELER_H
#define IMAGELABELER_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QString>
#include <QFile>
#include <QList>
#include <QTextStream>
#include <QQuickView>
#include <QQmlContext>
#include <QAbstractListModel>

class Label : public QObject {
    Q_OBJECT
    Q_PROPERTY(int id READ id NOTIFY idChanged)
    Q_PROPERTY(float x READ x NOTIFY xChanged)
    Q_PROPERTY(float y READ y NOTIFY yChanged)
    Q_PROPERTY(float width READ width NOTIFY widthChanged)
    Q_PROPERTY(float height READ height NOTIFY heightChanged)

public:
    explicit Label(QObject *parent = nullptr) : QObject(parent), m_id(0), m_x(0), m_y(0), m_width(0), m_height(0) {}

    int id() const {
        return m_id;
    }

    float x() const {
        return m_x;
    }

    float y() const {
        return m_y;
    }

    float width() const {
        return m_width;
    }

    float height() const {
        return m_height;
    }

public slots:
    void setId(int id) {
        if (m_id != id) {
            m_id = id;
            emit idChanged();
        }
    }

    void setX(float x) {
        if (m_x != x) {
            m_x = x;
            emit xChanged();
        }
    }

    void setY(float y) {
        if (m_y != y) {
            m_y = y;
            emit yChanged();
        }
    }

    void setWidth(float width) {
        if (m_width != width) {
            m_width = width;
            emit widthChanged();
        }
    }

    void setHeight(float height) {
        if (m_height != height) {
            m_height = height;
            emit heightChanged();
        }
    }

signals:
    void idChanged();
    void xChanged();
    void yChanged();
    void widthChanged();
    void heightChanged();

private:
    int m_id;
    float m_x;
    float m_y;
    float m_width;
    float m_height;
};

class ImageLabeler : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString imageSource READ imageSource WRITE setImageSource NOTIFY imageSourceChanged)
    Q_PROPERTY(QString labelFilePath READ labelFilePath WRITE setLabelFilePath NOTIFY labelFilePathChanged)
    Q_PROPERTY(QList<Label*> labels READ labels NOTIFY labelsChanged)

private:
    QString m_imageSource;
    QString m_labelFilePath;
    QList<Label*> m_labels;

public:

    typedef enum{
        ID          = 0,
        X           = 1,
        Y           = 2,
        WIDTH       = 3,
        HEIGHT      = 4
    }IMAGE_ELEMENT;

    int rowCount(const QModelIndex &parent) const{
        return this->m_labels.count();
    };
    QVariant data(const QModelIndex &index, int role) const{
        Label *tmp = m_labels.at(index.row());

        switch(role)
        {
            case ID:
                return QVariant::fromValue(tmp->id());
            case X:
                return QVariant::fromValue(tmp->x());
            case Y:
                return QVariant::fromValue(tmp->y());
            case WIDTH:
                return QVariant::fromValue(tmp->width());
            case HEIGHT:
                return QVariant::fromValue(tmp->height());
        }

        return QVariant();
    };
    QHash<int, QByteArray> roleNames() const{
        QHash<int, QByteArray> roleName;
        roleName.insert(ID,        "element_id");
        roleName.insert(X,         "element_x");
        roleName.insert(Y,         "element_y");
        roleName.insert(WIDTH,     "element_width");
        roleName.insert(HEIGHT,    "element_height");
        return roleName;
    };

public:
    explicit ImageLabeler(QObject *parent = nullptr);

    QString imageSource() const {
        return m_imageSource;
    }

    void setImageSource(const QString& source) {
        qDebug() << source;
        if (m_imageSource != source) {
            m_imageSource = source;
            emit imageSourceChanged();
        }
    }

    QString labelFilePath() const {
        return m_labelFilePath;
    }

    void setLabelFilePath(const QString& path) {

        if (m_labelFilePath != path) {
            m_labelFilePath = path;
            emit labelFilePathChanged();
        }
        qDebug() << m_labelFilePath;
    }

    QList<Label*> labels() const {
        return m_labels;
    }

    Q_INVOKABLE void loadLabels() {
        beginResetModel();
        m_labels.clear();
        QFile file(m_labelFilePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList parts = line.split(" ");

                Label* label = new Label();
                label->setId(parts[0].toInt());
                label->setX(parts[1].toFloat());
                label->setY(parts[2].toFloat());
                label->setWidth(parts[3].toFloat());
                label->setHeight(parts[4].toFloat());

                m_labels.append(label);
            }

            file.close();
            emit labelsChanged();
        }

        endResetModel();
    }

signals:
    void imageSourceChanged();
    void labelFilePathChanged();
    void labelsChanged();


};

#endif // IMAGELABELER_H
