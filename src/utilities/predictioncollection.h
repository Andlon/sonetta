#pragma once

#include <QString>
#include <Spotinetta/Spotinetta>
#include <QMultiMap>

namespace Sonetta {

class PredictionCollection {
public:
    explicit PredictionCollection(const QString &partial);

    void insert(const Spotinetta::Track & track);
    void insert(const Spotinetta::Artist & artist);
    void insert(const Spotinetta::Album & album);
    void insert(const Spotinetta::Playlist & playlist);

    QStringList predictions() const;

private:
    void insert(qreal score, const QString &match);

    QMultiMap<qreal, QString> m_predictions;
    QString m_partial;
};
}
