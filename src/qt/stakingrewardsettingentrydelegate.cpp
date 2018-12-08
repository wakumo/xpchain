// Copyright (c) 018 The XPChain Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/stakingrewardsettingentrydelegate.h>
#include <qt/stakingrewardsettingmodel.h>

#include <QSpinBox>
#include <QAbstractSpinBox>

QWidget *StakingRewardSettingEntryDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == StakingRewardSettingTableModel::DistributionPercentage)
    {
        // Provide a special spinbox for editing distribution percentage
        QSpinBox *spinbox = new QSpinBox(parent);
        spinbox->setFrame(false);
        spinbox->setMinimum(1);
        spinbox->setMaximum(100);
        spinbox->setSuffix(" %");
        spinbox->setAlignment(Qt::AlignRight);
        spinbox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        connect(spinbox, &QAbstractSpinBox::editingFinished, this, &StakingRewardSettingEntryDelegate::commitAndCloseSpinBox);

        return spinbox;
    } else {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void StakingRewardSettingEntryDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == StakingRewardSettingTableModel::DistributionPercentage)
    {
        quint8 pct = index.data(Qt::EditRole).value<quint8>();
        QSpinBox *spinbox = qobject_cast<QSpinBox*>(editor);
        spinbox->setValue(pct);
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}
void StakingRewardSettingEntryDelegate::setModelData(QWidget *editor,
        QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == StakingRewardSettingTableModel::DistributionPercentage)
    {
        QSpinBox *spinbox = qobject_cast<QSpinBox*>(editor);
        spinbox->interpretText();
        quint8 pct = spinbox->value();
        model->setData(index, pct, Qt::EditRole);
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void StakingRewardSettingEntryDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void StakingRewardSettingEntryDelegate::commitAndCloseSpinBox()
{
    QSpinBox *spinbox = qobject_cast<QSpinBox*>(sender());
    spinbox->interpretText();
    quint8 pct = spinbox->value();
    if(pct < 1 || pct > 100)
    {
        return;
    }
    Q_EMIT commitData(spinbox);
    Q_EMIT closeEditor(spinbox);
}
