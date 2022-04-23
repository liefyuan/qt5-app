#ifndef QFILECHOSE_H
#define QFILECHOSE_H

#include <QFileDialog>

class qFileChose : public QFileDialog
{
    Q_OBJECT

public:
    qFileChose(QWidget *parent = 0);
    ~qFileChose();
    QString choseOpenFileName(QWidget *parent,QString caption,QString dir,QString filter,Options options);
};

#endif // QFILECHOSE_H
