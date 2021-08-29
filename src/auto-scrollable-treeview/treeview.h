#pragma once

#include <QTreeView>

class AutoScrollTreeView : public QTreeView
{
    Q_OBJECT

public:
    AutoScrollTreeView(QWidget *parent = nullptr);
    virtual ~AutoScrollTreeView() override {}

    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};
