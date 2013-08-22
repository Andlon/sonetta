#ifndef SPSEARCHWATCHER_H
#define SPSEARCHWATCHER_H

#include "spsearch.h"
#include <QObject>

class SpSearchWatcher : public QObject {
    Q_OBJECT

public:
    explicit SpSearchWatcher(QObject * parent = 0);

    SpSearch search() const;
    void setSearch(const SpSearch &search);

signals:
    void searchChanged();
    void loaded();

private slots:
    void handleSearchUpdated(const SpSearch &search);

private:
    Q_DISABLE_COPY(SpSearchWatcher)
    SpSearch m_search;
};

#endif // SPSEARCHWATCHER_H
