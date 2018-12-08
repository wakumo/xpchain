#ifndef BITCOIN_QT_MINTINGFILTERPROXY_H
#define BITCOIN_QT_MINTINGFILTERPROXY_H

#include <QSortFilterProxyModel>

class MintingFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit MintingFilterProxy(QObject *parent = 0);
};

#endif // BITCOIN_QT_MINTINGFILTERPROXY_H
